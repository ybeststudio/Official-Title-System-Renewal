#include "StdAfx.h"

#if defined(ENABLE_TITLE_SYSTEM)

#include "PythonTitleSystem.h"
#include "PythonNetworkStream.h"
#include "Packet.h"
#include "Locale.h"
#include "../EterPack/EterPackManager.h"
#include "../EterBase/FileLoader.h"

static_assert(sizeof(TPacketGCTitleSystem) == 6, "TPacketGCTitleSystem size error");
static_assert(sizeof(TPacketGCTitleSystemTableRow) == 432, "TPacketGCTitleSystemTableRow size error");
static_assert(sizeof(TPacketGCTitleSystemPlayerRow) == 10, "TPacketGCTitleSystemPlayerRow size error");

#if defined(RENDER_TARGET)
#include "../EffectLib/EffectManager.h"
#include "../EffectLib/EffectData.h"
#include "../EffectLib/ParticleSystemInstance.h"
#include "../EterLib/RenderTargetManager.h"
#include "../EterLib/Camera.h"
#include "../EterLib/GrpImageInstance.h"
#include "../EterLib/ResourceManager.h"
#include "../EterLib/StateManager.h"
#include "PythonApplication.h"
#include "PythonBackground.h"
#endif

#include <algorithm>
#include <ctime>
#include <vector>

namespace
{
	const int TITLE_FONT_COLOR = -1; // 0xFFFFFFFF (Pure White)

#if defined(RENDER_TARGET)
	const DWORD TITLE_PREVIEW_EFFECT_INSTANCE = 0x5449544C;
	const float TITLE_PREVIEW_PANEL_WIDTH = 224.0f;
	const float TITLE_PREVIEW_PANEL_HEIGHT = 346.0f;
#endif

	bool LoadPackTextFile(const char* path, CMemoryTextFileLoader& outLoader)
	{
		CMappedFile kFile;
		LPCVOID pData = nullptr;
		if (!CEterPackManager::Instance().Get(kFile, path, &pData) || !pData)
			return false;
		outLoader.Bind(kFile.Size(), pData);
		return true;
	}

	void SplitTabs(const std::string& line, std::vector<std::string>& outCols)
	{
		outCols.clear();
		std::string cur;
		for (size_t i = 0; i < line.size(); ++i)
		{
			const char c = line[i];
			if (c == '\t')
			{
				outCols.push_back(cur);
				cur.clear();
				continue;
			}
			if (c == '\r')
				continue;
			cur.push_back(c);
		}
		outCols.push_back(cur);
	}

	DWORD ParseOpenTime(const char* szDateTime)
	{
		if (!szDateTime || !*szDateTime)
			return 0;

		int y = 0;
		int mo = 0;
		int d = 0;
		int h = 0;
		int mi = 0;
		if (sscanf(szDateTime, "%d-%d-%d %d:%d", &y, &mo, &d, &h, &mi) != 5)
			return 0;

		struct tm tmVal = {};
		tmVal.tm_year = y - 1900;
		tmVal.tm_mon = mo - 1;
		tmVal.tm_mday = d;
		tmVal.tm_hour = h;
		tmVal.tm_min = mi;
		tmVal.tm_isdst = -1;
		return static_cast<DWORD>(mktime(&tmVal));
	}

	BYTE InferTitleType(DWORD dwTitleIndex, WORD wResourceIndex, const std::map<WORD, BYTE>& mapResourceTypes)
	{
		std::map<WORD, BYTE>::const_iterator it = mapResourceTypes.find(wResourceIndex);
		if (it != mapResourceTypes.end())
		{
			BYTE bType = it->second;
			if (bType == 0)
				return CPythonTitleSystem::TYPE_NAMEPLATE;
			else if (bType == 1)
				return CPythonTitleSystem::TYPE_IMAGE;
			else
				return CPythonTitleSystem::TYPE_EFFECT;
		}

		if (dwTitleIndex == 1)
			return CPythonTitleSystem::TYPE_IMAGE;
		if (wResourceIndex == 3000)
			return CPythonTitleSystem::TYPE_NAMEPLATE;
		if (dwTitleIndex == 1001 || dwTitleIndex == 1011 || dwTitleIndex == 1012)
			return CPythonTitleSystem::TYPE_NAMEPLATE;
		return CPythonTitleSystem::TYPE_EFFECT;
	}

	const char* GetDefaultEffectPath(DWORD dwTitleIndex)
	{
		switch (dwTitleIndex)
		{
			case 1: return "d:/ymir work/effect/battleroyale/crown01.mse";
			case 2: return "d:/ymir work/effect/etc/ranking_battle/ranker_battle_01.mse";
			case 3: return "d:/ymir work/effect/etc/ranking_battle/ranker_battle_02.mse";
			case 4: return "d:/ymir work/effect/etc/ranking_battle/ranker_battle_03.mse";
			case 6: return "d:/ymir work/effect/etc/title/title_dragon_red.mse";
			case 7: return "d:/ymir work/effect/etc/title/title_dragon_gw.mse";
			case 1001: return "d:/ymir work/effect/etc/title/title_01_shield.mse";
			case 1005: return "d:/ymir work/effect/etc/title/title_06_banner_gold.mse";
			case 1006: return "d:/ymir work/effect/etc/title/title_08_banner_blue.mse";
			case 1011: return "d:/ymir work/effect/etc/title/title_07_banner_red.mse";
			default:
				return "";
		}
	}

	const char* GetDefaultImagePath(DWORD dwTitleIndex)
	{
		if (dwTitleIndex == 1)
			return "d:/ymir work/effect/battleroyale/crown01.tga";
		return "";
	}

	WORD GetPreviewResourceIndex(DWORD dwTitleIndex, WORD wResourceIndex)
	{
		switch (dwTitleIndex)
		{
			default:
				return wResourceIndex;
		}
	}
}

CPythonTitleSystem::CPythonTitleSystem()
	: m_poHandler(nullptr)
	, m_bDataReceived(false)
	, m_bShowTitle(false)
	, m_dwEquippedTitle(0)
	, m_bLocaleLoaded(false)
#if defined(RENDER_TARGET)
	, m_pPreviewBackground(nullptr)
	, m_bPreviewShow(false)
	, m_wPreviewResourceIndex(0)
	, m_dwPreviewTitleIndex(0)
	, m_dwPreviewEffectInstance(0)
	, m_dwPreviewEffectCRC(0)
	, m_iPreviewLastError(0)
	, m_iPreviewRenderSkipReason(0)
#endif
{
}

CPythonTitleSystem::~CPythonTitleSystem()
{
	Clear();
}

void CPythonTitleSystem::Clear()
{
#if defined(RENDER_TARGET)
	HidePreviewEffect();
	if (m_pPreviewBackground)
	{
		delete m_pPreviewBackground;
		m_pPreviewBackground = nullptr;
	}
	m_wPreviewResourceIndex = 0;
	m_dwPreviewTitleIndex = 0;
#endif

	Py_XDECREF(m_poHandler);
	m_poHandler = nullptr;
	m_bDataReceived = false;
	m_dwEquippedTitle = 0;
	m_mapPlayerData.clear();
	m_mapResourceTypes.clear();
	m_mapResourceSubFiles.clear();
}

void CPythonTitleSystem::LoadLocaleProtos()
{
	m_mapTitleData.clear();
	m_vecDisplayOrder.clear();
	m_bLocaleLoaded = false;
	m_mapResourceTypes.clear();
	m_mapResourceSubFiles.clear();

	const char* resCandidates[] = {
		"locale/locale/common/title_resource_list.txt",
		"locale/common/title_resource_list.txt",
	};

	CMemoryTextFileLoader resLoader;
	bool bResLoaded = false;
	for (size_t i = 0; i < sizeof(resCandidates) / sizeof(resCandidates[0]); ++i)
	{
		if (LoadPackTextFile(resCandidates[i], resLoader))
		{
			bResLoaded = true;
			break;
		}
	}

	if (bResLoaded)
	{
		std::vector<std::string> resCols;
		for (DWORD line = 0; line < resLoader.GetLineCount(); ++line)
		{
			const std::string& lineStr = resLoader.GetLineString(line);
			if (lineStr.empty() || lineStr[0] == '#')
				continue;

			SplitTabs(lineStr, resCols);
			if (resCols.size() < 3)
				continue;

			const WORD wResourceIndex = static_cast<WORD>(strtoul(resCols[0].c_str(), nullptr, 10));
			const std::string stSubPath = resCols[1];
			const BYTE bResourceType = static_cast<BYTE>(strtoul(resCols[2].c_str(), nullptr, 10));

			m_mapResourceTypes[wResourceIndex] = bResourceType;
			m_mapResourceSubFiles[wResourceIndex] = stSubPath;
		}
	}

	const char* localePath = LocaleService_GetLocalePath();
	char szLocaleName[32] = "tr";

	if (localePath && *localePath)
	{
		const char* pszPath = localePath;
		if (strncmp(pszPath, "locale/", 7) == 0)
			pszPath += 7;
		snprintf(szLocaleName, sizeof(szLocaleName), "%s", pszPath);
	}

	char szWzPathA[256];
	char szWzPathB[256];
	char szWzPathC[256];
	char szGfPathA[256];
	char szGfPathB[256];
	char szGfPathC[256];

	snprintf(szWzPathA, sizeof(szWzPathA), "locale/locale/%s/title_wz.txt", szLocaleName);
	snprintf(szWzPathB, sizeof(szWzPathB), "locale/%s/title_wz.txt", szLocaleName);
	snprintf(szGfPathA, sizeof(szGfPathA), "locale/locale/%s/title_gf.txt", szLocaleName);
	snprintf(szGfPathB, sizeof(szGfPathB), "locale/%s/title_gf.txt", szLocaleName);

	if (localePath && *localePath)
	{
		snprintf(szWzPathC, sizeof(szWzPathC), "%s/title_wz.txt", localePath);
		snprintf(szGfPathC, sizeof(szGfPathC), "%s/title_gf.txt", localePath);
	}
	else
	{
		szWzPathC[0] = '\0';
		szGfPathC[0] = '\0';
	}

	const char* wzCandidates[] = {
		szWzPathA,
		szWzPathB,
		szWzPathC,
		"locale/locale/tr/title_wz.txt",
		"locale/tr/title_wz.txt",
	};
	const char* gfCandidates[] = {
		szGfPathA,
		szGfPathB,
		szGfPathC,
		"locale/locale/tr/title_gf.txt",
		"locale/tr/title_gf.txt",
	};

	CMemoryTextFileLoader loader;
	bool bLoadedAny = false;
	std::vector<std::string> cols;

	for (size_t i = 0; i < sizeof(wzCandidates) / sizeof(wzCandidates[0]); ++i)
	{
		if (!wzCandidates[i] || !*wzCandidates[i])
			continue;
		if (!LoadPackTextFile(wzCandidates[i], loader))
			continue;

		for (DWORD line = 0; line < loader.GetLineCount(); ++line)
		{
			const std::string& lineStr = loader.GetLineString(line);
			if (lineStr.empty() || lineStr[0] == '#')
				continue;

			SplitTabs(lineStr, cols);
			if (cols.size() < 4)
				continue;

			const DWORD dwTitleIndex = static_cast<DWORD>(strtoul(cols[0].c_str(), nullptr, 10));
			const WORD wResourceIndex = static_cast<WORD>(strtoul(cols[1].c_str(), nullptr, 10));
			if (dwTitleIndex == 0)
				continue;

			STitleData data = {};
			data.dwTitleIndex = dwTitleIndex;
			data.stName = cols[3];
			data.stCondition = (cols.size() >= 5) ? cols[4] : "";
			data.bIsPermanent = 0;
			data.dwOpenTime = 0;
			data.iFontColor = TITLE_FONT_COLOR;
			data.wResourceIndex = wResourceIndex;
			data.bTitleType = InferTitleType(dwTitleIndex, wResourceIndex, m_mapResourceTypes);
			data.stEffectPath = GetDefaultEffectPath(dwTitleIndex);
			data.stImagePath = GetDefaultImagePath(dwTitleIndex);

			m_mapTitleData[dwTitleIndex] = data;
			m_vecDisplayOrder.push_back(dwTitleIndex);
		}

		bLoadedAny = true;
		break;
	}

	for (size_t i = 0; i < sizeof(gfCandidates) / sizeof(gfCandidates[0]); ++i)
	{
		if (!gfCandidates[i] || !*gfCandidates[i])
			continue;
		if (!LoadPackTextFile(gfCandidates[i], loader))
			continue;

		for (DWORD line = 0; line < loader.GetLineCount(); ++line)
		{
			const std::string& lineStr = loader.GetLineString(line);
			if (lineStr.empty() || lineStr[0] == '#')
				continue;

			SplitTabs(lineStr, cols);
			if (cols.size() < 4)
				continue;

			const DWORD dwTitleIndex = static_cast<DWORD>(strtoul(cols[0].c_str(), nullptr, 10));
			const WORD wResourceIndex = static_cast<WORD>(strtoul(cols[1].c_str(), nullptr, 10));
			if (dwTitleIndex == 0)
				continue;

			STitleData data = {};
			data.dwTitleIndex = dwTitleIndex;
			data.wResourceIndex = wResourceIndex;
			data.stName = cols[3];
			data.stCondition = (cols.size() >= 5) ? cols[4] : "";
			data.dwOpenTime = (cols.size() >= 7) ? ParseOpenTime(cols[6].c_str()) : 0;
			data.bIsPermanent = (dwTitleIndex >= 1001 && dwTitleIndex <= 1004) ? 0 : 1;
			data.iFontColor = TITLE_FONT_COLOR;
			data.bTitleType = InferTitleType(dwTitleIndex, wResourceIndex, m_mapResourceTypes);
			data.stEffectPath = GetDefaultEffectPath(dwTitleIndex);
			data.stImagePath = GetDefaultImagePath(dwTitleIndex);

			m_mapTitleData[dwTitleIndex] = data;
			m_vecDisplayOrder.push_back(dwTitleIndex);
		}

		bLoadedAny = true;
		break;
	}

	m_bLocaleLoaded = bLoadedAny && !m_vecDisplayOrder.empty();
}

void CPythonTitleSystem::SetHandler(PyObject* poHandler)
{
	Py_XINCREF(poHandler);
	Py_XDECREF(m_poHandler);
	m_poHandler = poHandler;
}

void CPythonTitleSystem::OnNotify(BYTE bMessage, DWORD dwTitleIndex)
{
	if (!m_poHandler || !Py_IsInitialized())
		return;

	PyCallClassMemberFunc(m_poHandler, "OnTitleNotify", Py_BuildValue("(ii)", bMessage, dwTitleIndex));
}

void CPythonTitleSystem::RecvPacket(BYTE bSubHeader, WORD wCount, const char* c_pData, int iRemainSize)
{
	switch (bSubHeader)
	{
		case SUBHEADER_GC_TITLE_TABLE:
		{
			if (!m_bLocaleLoaded)
			{
				m_mapTitleData.clear();
				m_vecDisplayOrder.clear();
			}

			const int iNeed = static_cast<int>(wCount) * static_cast<int>(sizeof(TPacketGCTitleSystemTableRow));
			if (iRemainSize < iNeed)
				return;

			for (WORD i = 0; i < wCount; ++i)
			{
				const TPacketGCTitleSystemTableRow* pRow =
					reinterpret_cast<const TPacketGCTitleSystemTableRow*>(c_pData + i * sizeof(TPacketGCTitleSystemTableRow));

				STitleData data;
				std::map<DWORD, STitleData>::iterator it = m_mapTitleData.find(pRow->dwTitleIndex);
				if (it != m_mapTitleData.end())
					data = it->second;

				data.dwTitleIndex = pRow->dwTitleIndex;
				data.bTitleType = pRow->bTitleType;
				if (!m_bLocaleLoaded || data.stName.empty())
					data.stName = pRow->szName;
				if (!m_bLocaleLoaded || data.stCondition.empty())
					data.stCondition = pRow->szCondition;
				data.bIsPermanent = pRow->bIsPermanent;
				if (data.dwOpenTime == 0)
					data.dwOpenTime = pRow->dwOpenTime;
				data.iFontColor = pRow->iFontColor;
				if (data.wResourceIndex == 0)
					data.wResourceIndex = pRow->wResourceIndex;
				if (data.stEffectPath.empty() && pRow->szEffectPath[0])
					data.stEffectPath = pRow->szEffectPath;
				if (data.dwTitleIndex == 1000 || data.dwTitleIndex == 1002 || data.dwTitleIndex == 1003 || data.dwTitleIndex == 1004 ||
					(data.dwTitleIndex >= 1007 && data.dwTitleIndex <= 1010) || data.dwTitleIndex == 1012)
				{
					data.stEffectPath.clear();
				}
				if (data.stImagePath.empty() && pRow->szImagePath[0])
					data.stImagePath = pRow->szImagePath;

				m_mapTitleData[data.dwTitleIndex] = data;
				if (!m_bLocaleLoaded)
					m_vecDisplayOrder.push_back(data.dwTitleIndex);
			}
			break;
		}

		case SUBHEADER_GC_TITLE_PLAYER:
		{
			m_mapPlayerData.clear();
			const int iNeed = static_cast<int>(wCount) * static_cast<int>(sizeof(TPacketGCTitleSystemPlayerRow));
			if (iRemainSize < iNeed)
				return;

			for (WORD i = 0; i < wCount; ++i)
			{
				const TPacketGCTitleSystemPlayerRow* pRow =
					reinterpret_cast<const TPacketGCTitleSystemPlayerRow*>(c_pData + i * sizeof(TPacketGCTitleSystemPlayerRow));

				SPlayerData data;
				data.dwTitleIndex = pRow->dwTitleIndex;
				data.dwEndTime = pRow->dwEndTime;
				data.bIsEquip = pRow->bIsEquip;
				data.bIsObtain = pRow->bIsObtain;
				m_mapPlayerData[data.dwTitleIndex] = data;
			}
			break;
		}

		case SUBHEADER_GC_TITLE_EQUIPPED:
		{
			if (iRemainSize < static_cast<int>(sizeof(TPacketGCTitleSystemEquipRow)))
				return;

			const TPacketGCTitleSystemEquipRow* pRow =
				reinterpret_cast<const TPacketGCTitleSystemEquipRow*>(c_pData);
			m_dwEquippedTitle = pRow->dwTitleIndex;
			break;
		}

		case SUBHEADER_GC_TITLE_NOTIFY:
		{
			if (iRemainSize < static_cast<int>(sizeof(TPacketGCTitleSystemNotifyRow)))
				return;

			const TPacketGCTitleSystemNotifyRow* pRow =
				reinterpret_cast<const TPacketGCTitleSystemNotifyRow*>(c_pData);
			OnNotify(pRow->bMessage, pRow->dwTitleIndex);
			break;
		}

		case SUBHEADER_GC_TITLE_END:
			m_bDataReceived = true;
			if (m_poHandler && Py_IsInitialized())
				PyCallClassMemberFunc(m_poHandler, "OnTitleDataEnd", Py_BuildValue("()"));
			break;
	}
}

bool CPythonTitleSystem::RequestOpen()
{
	if (!m_bLocaleLoaded || m_vecDisplayOrder.empty())
		LoadLocaleProtos();
	return CPythonNetworkStream::Instance().SendTitleSystemPacket(SUBHEADER_CG_TITLE_OPEN, 0);
}

bool CPythonTitleSystem::RequestEquip(DWORD dwTitleIndex)
{
	return CPythonNetworkStream::Instance().SendTitleSystemPacket(SUBHEADER_CG_TITLE_EQUIP, dwTitleIndex);
}

bool CPythonTitleSystem::RequestUnEquip()
{
	return CPythonNetworkStream::Instance().SendTitleSystemPacket(SUBHEADER_CG_TITLE_UNEQUIP, 0);
}

const CPythonTitleSystem::STitleData* CPythonTitleSystem::GetTitleData(DWORD dwTitleIndex) const
{
	std::map<DWORD, STitleData>::const_iterator it = m_mapTitleData.find(dwTitleIndex);
	if (it == m_mapTitleData.end())
		return nullptr;
	return &it->second;
}

const CPythonTitleSystem::SPlayerData* CPythonTitleSystem::GetPlayerData(DWORD dwTitleIndex) const
{
	std::map<DWORD, SPlayerData>::const_iterator it = m_mapPlayerData.find(dwTitleIndex);
	if (it == m_mapPlayerData.end())
		return nullptr;
	return &it->second;
}

PyObject* CPythonTitleSystem::BuildTitleRowTuple(const STitleData& r) const
{
	return Py_BuildValue(
		"(iissiiii)",
		r.dwTitleIndex,
		r.bTitleType,
		r.stName.c_str(),
		r.stCondition.c_str(),
		r.bIsPermanent,
		r.dwOpenTime,
		r.iFontColor,
		r.wResourceIndex
	);
}

PyObject* CPythonTitleSystem::BuildPlayerRowTuple(const SPlayerData& r) const
{
	return Py_BuildValue("(iiii)", r.dwTitleIndex, r.dwEndTime, r.bIsEquip, r.bIsObtain);
}

PyObject* CPythonTitleSystem::BuildAllTitleDataTuple() const
{
	PyObject* poList = PyList_New(0);
	const DWORD now = static_cast<DWORD>(time(nullptr));

	for (size_t i = 0; i < m_vecDisplayOrder.size(); ++i)
	{
		const DWORD dwIndex = m_vecDisplayOrder[i];
		std::map<DWORD, STitleData>::const_iterator it = m_mapTitleData.find(dwIndex);
		if (it == m_mapTitleData.end())
			continue;
		if (it->second.dwOpenTime > now)
			continue;

		PyObject* poRow = BuildTitleRowTuple(it->second);
		PyList_Append(poList, poRow);
		Py_DECREF(poRow);
	}
	return poList;
}

PyObject* CPythonTitleSystem::BuildShowListTuple() const
{
	PyObject* poList = PyList_New(0);
	const DWORD now = static_cast<DWORD>(time(nullptr));

	for (size_t i = 0; i < m_vecDisplayOrder.size(); ++i)
	{
		const DWORD dwIndex = m_vecDisplayOrder[i];
		std::map<DWORD, STitleData>::const_iterator itTitle = m_mapTitleData.find(dwIndex);
		if (itTitle == m_mapTitleData.end())
			continue;
		if (itTitle->second.dwOpenTime > now)
			continue;

		SPlayerData playerRow;
		playerRow.dwTitleIndex = dwIndex;
		playerRow.dwEndTime = 0;
		playerRow.bIsEquip = (m_dwEquippedTitle == dwIndex) ? 1 : 0;
		playerRow.bIsObtain = 0;

		std::map<DWORD, SPlayerData>::const_iterator itPlayer = m_mapPlayerData.find(dwIndex);
		if (itPlayer != m_mapPlayerData.end())
			playerRow = itPlayer->second;

		PyObject* poRow = BuildPlayerRowTuple(playerRow);
		PyList_Append(poList, poRow);
		Py_DECREF(poRow);
	}
	return poList;
}

bool CPythonTitleSystem::IsItemUsable(DWORD dwTitleIndex) const
{
	const SPlayerData* pPlayer = GetPlayerData(dwTitleIndex);
	if (pPlayer && pPlayer->bIsObtain)
		return false;
	return GetTitleData(dwTitleIndex) != nullptr;
}

std::string CPythonTitleSystem::GetItemTooltip(DWORD dwTitleIndex) const
{
	const STitleData* p = GetTitleData(dwTitleIndex);
	if (!p)
		return "";
	return p->stCondition;
}

std::string CPythonTitleSystem::GetImagePath(WORD wResourceIndex) const
{
	std::map<WORD, std::string>::const_iterator it = m_mapResourceSubFiles.find(wResourceIndex);
	if (it != m_mapResourceSubFiles.end())
	{
		char szPath[128];
		snprintf(szPath, sizeof(szPath), "d:/ymir work/ui/game/title/titles/%s", it->second.c_str());
		return szPath;
	}

	char szPath[128];
	snprintf(szPath, sizeof(szPath), "d:/ymir work/ui/game/title/titles/title_%04d.sub", wResourceIndex);
	return szPath;
}

PyObject* CPythonTitleSystem::GetSpriteImages(WORD wResourceIndex) const
{
	WORD wSpriteIndex = wResourceIndex;
	if (wResourceIndex == 3000)
		wSpriteIndex = 1000;

	PyObject* poList = PyList_New(0);
	for (int i = 0; i < 36; ++i)
	{
		char szPath[128];
		snprintf(szPath, sizeof(szPath), "d:/ymir work/ui/game/title/titles/sprites_%04d/%02d.sub", wSpriteIndex, i);
		PyObject* poPath = PyString_FromString(szPath);
		PyList_Append(poList, poPath);
		Py_DECREF(poPath);
	}
	return poList;
}

PyObject* CPythonTitleSystem::GetSpriteInfo(WORD wResourceIndex) const
{
	return Py_BuildValue("(ii)", 208, 128);
}

bool CPythonTitleSystem::GetNameplateImages(WORD wResourceIndex, std::string& outLeft, std::string& outMiddle, std::string& outRight) const
{
	outLeft.clear();
	outMiddle.clear();
	outRight.clear();

	char szSubPath[128];
	std::map<WORD, std::string>::const_iterator it = m_mapResourceSubFiles.find(wResourceIndex);
	if (it != m_mapResourceSubFiles.end())
		snprintf(szSubPath, sizeof(szSubPath), "d:/ymir work/ui/game/title/titles/%s", it->second.c_str());
	else
		snprintf(szSubPath, sizeof(szSubPath), "d:/ymir work/ui/game/title/titles/title_%04d.sub", wResourceIndex);

	CMemoryTextFileLoader loader;
	if (!LoadPackTextFile(szSubPath, loader))
		return false;

	for (DWORD line = 0; line < loader.GetLineCount(); ++line)
	{
		const std::string& lineStr = loader.GetLineString(line);
		if (lineStr.empty() || lineStr[0] == '#')
			continue;

		const size_t tabPos = lineStr.find('\t');
		if (tabPos == std::string::npos)
			continue;

		std::string key = lineStr.substr(0, tabPos);
		std::string val = lineStr.substr(tabPos + 1);
		while (!val.empty() && (val[0] == ' ' || val[0] == '\r' || val[0] == '\t'))
			val.erase(val.begin());
		while (!val.empty() && (val.back() == ' ' || val.back() == '\r' || val.back() == '\t'))
			val.pop_back();

		if (key == "left_image")
			outLeft = val;
		else if (key == "middle_image")
			outMiddle = val;
		else if (key == "right_image")
			outRight = val;
	}

	auto normalizePath = [](std::string& path) {
		if (path.empty()) return;
		for (size_t i = 0; i < path.size(); ++i)
		{
			if (path[i] == '\\')
				path[i] = '/';
		}
		if (path.find("d:/ymir work/") != 0 && path.find("D:/ymir work/") != 0)
		{
			if (path.find("ui/") == 0)
				path = std::string("d:/ymir work/") + path;
			else
				path = std::string("d:/ymir work/ui/") + path;
		}
	};

	normalizePath(outLeft);
	normalizePath(outMiddle);
	normalizePath(outRight);

	return !outLeft.empty() && !outMiddle.empty() && !outRight.empty();
}

CResource* CPythonTitleSystem::GetSpriteFrameResource(WORD wResourceIndex, int iFrame) const
{
	WORD wSpriteIndex = wResourceIndex;
	if (wResourceIndex == 3000)
		wSpriteIndex = 1000;

	char szPath[128];
	snprintf(szPath, sizeof(szPath), "d:/ymir work/ui/game/title/titles/sprites_%04d/%02d.sub", wSpriteIndex, iFrame);
	return CResourceManager::Instance().GetResourcePointer(szPath);
}

#if defined(RENDER_TARGET)
bool CPythonTitleSystem::ParseSubEffectPath(WORD wResourceIndex, std::string& outPath) const
{
	outPath.clear();

	char szSubPath[128];
	std::map<WORD, std::string>::const_iterator it = m_mapResourceSubFiles.find(wResourceIndex);
	if (it != m_mapResourceSubFiles.end())
		snprintf(szSubPath, sizeof(szSubPath), "d:/ymir work/ui/game/title/titles/%s", it->second.c_str());
	else
		snprintf(szSubPath, sizeof(szSubPath), "d:/ymir work/ui/game/title/titles/title_%04d.sub", wResourceIndex);

	CMemoryTextFileLoader loader;
	if (!LoadPackTextFile(szSubPath, loader))
		return false;

	std::string stRelPath;
	for (DWORD line = 0; line < loader.GetLineCount(); ++line)
	{
		const std::string& lineStr = loader.GetLineString(line);
		if (lineStr.empty() || lineStr[0] == '#')
			continue;

		const size_t tabPos = lineStr.find('\t');
		if (tabPos == std::string::npos)
			continue;

		const std::string key = lineStr.substr(0, tabPos);
		if (key != "effect")
			continue;

		stRelPath = lineStr.substr(tabPos + 1);
		while (!stRelPath.empty() && (stRelPath[0] == ' ' || stRelPath[0] == '\r'))
			stRelPath.erase(stRelPath.begin());
		break;
	}

	if (stRelPath.empty())
		return false;

	for (size_t i = 0; i < stRelPath.size(); ++i)
	{
		if (stRelPath[i] == '\\')
			stRelPath[i] = '/';
	}

	if (stRelPath.find("d:/ymir work/") == 0 || stRelPath.find("D:/ymir work/") == 0)
		outPath = stRelPath;
	else
		outPath = std::string("d:/ymir work/") + stRelPath;

	return true;
}

bool CPythonTitleSystem::ResolvePreviewEffectPath(std::string& outPath) const
{
	outPath.clear();

	if (ParseSubEffectPath(m_wPreviewResourceIndex, outPath) && !outPath.empty())
		return true;

	if (m_dwPreviewTitleIndex != 0)
	{
		const STitleData* pTitle = GetTitleData(m_dwPreviewTitleIndex);
		if (pTitle && !pTitle->stEffectPath.empty())
		{
			outPath = pTitle->stEffectPath;
			return true;
		}
	}

	for (std::map<DWORD, STitleData>::const_iterator it = m_mapTitleData.begin(); it != m_mapTitleData.end(); ++it)
	{
		if (it->second.wResourceIndex == m_wPreviewResourceIndex && !it->second.stEffectPath.empty())
		{
			outPath = it->second.stEffectPath;
			return true;
		}
	}

	return false;
}

void CPythonTitleSystem::CreatePreviewBackground(DWORD dwWidth, DWORD dwHeight)
{
	if (m_pPreviewBackground)
		return;

	CResource* pResource = CResourceManager::Instance().GetResourcePointer("d:/ymir work/ui/game/title/window/model_view_background.sub");
	if (!pResource)
		return;

	m_pPreviewBackground = new CGraphicImageInstance;
	m_pPreviewBackground->SetImagePointer(static_cast<CGraphicImage*>(pResource));
	m_pPreviewBackground->SetPosition(0.0f, 0.0f);
	// Illustrated / MyShopDeco ile ayni: RT tam client cozunurlugunde, asset 224x346.
	m_pPreviewBackground->SetScale(
		static_cast<float>(dwWidth) / TITLE_PREVIEW_PANEL_WIDTH,
		static_cast<float>(dwHeight) / TITLE_PREVIEW_PANEL_HEIGHT);
}

void CPythonTitleSystem::SetPreviewResource(WORD wResourceIndex, DWORD dwTitleIndex)
{
	m_wPreviewResourceIndex = GetPreviewResourceIndex(dwTitleIndex, wResourceIndex);
	m_dwPreviewTitleIndex = dwTitleIndex;
}

bool CPythonTitleSystem::IsPreviewEffectInstance(DWORD dwInstanceIndex) const
{
	return m_bPreviewShow && dwInstanceIndex == TITLE_PREVIEW_EFFECT_INSTANCE;
}

static bool IsDragonPreviewTitle(DWORD dwTitleIndex)
{
	return dwTitleIndex == 6 || dwTitleIndex == 7;
}

static bool IsScaledParticlePreviewTitle(DWORD dwTitleIndex)
{
	switch (dwTitleIndex)
	{
		case 1001: /* Metin+ (title_01_shield.mse) */
		case 1005:
		case 1006:
		case 6:
		case 7:
			return true;
		default:
			return false;
	}
}

/* Onizleme paneli icin scale ve z offset (dragon + shield particle) */
static const float DRAGON_PREVIEW_PARTICLE_SCALE = 0.75f;
static const float DRAGON_PREVIEW_Z_OFFSET = 3.0f;

static float GetPreviewParticleScale(DWORD dwTitleIndex)
{
	return IsScaledParticlePreviewTitle(dwTitleIndex) ? DRAGON_PREVIEW_PARTICLE_SCALE : 1.0f;
}

static void SetSelectedPreviewEffectGlobalMatrix(CEffectManager& rkEftMgr, float fX, float fZ)
{
	D3DXMATRIX matGlobal;
	D3DXMatrixIdentity(&matGlobal);
	matGlobal._41 = fX;
	matGlobal._42 = 0.0f;
	matGlobal._43 = fZ;
	rkEftMgr.SetEffectInstanceGlobalMatrix(matGlobal);
}

static void SetSelectedTitlePreviewPlacement(CEffectManager& rkEftMgr, DWORD dwTitleIndex, float fWidth, float fHeight)
{
	const float fParticleScale = GetPreviewParticleScale(dwTitleIndex);

	if (IsScaledParticlePreviewTitle(dwTitleIndex))
	{
		SetSelectedPreviewEffectGlobalMatrix(rkEftMgr, 0.0f, DRAGON_PREVIEW_Z_OFFSET);
		rkEftMgr.SetEffectInstanceParticleScale(fParticleScale);
		return;
	}

	SetSelectedPreviewEffectGlobalMatrix(rkEftMgr, 0.0f, 25.0f);
	rkEftMgr.SetEffectInstanceParticleScale(fParticleScale);
}

void CPythonTitleSystem::ShowPreviewEffect()
{
	HidePreviewEffect();
	m_iPreviewLastError = 0;
	m_stPreviewLastEffectPath.clear();

	std::string stEffectPath;
	if (!ResolvePreviewEffectPath(stEffectPath) || stEffectPath.empty())
	{
		m_iPreviewLastError = 1;
		TraceError("TitlePreview: ResolvePreviewEffectPath failed (res=%u title=%u)",
			m_wPreviewResourceIndex, m_dwPreviewTitleIndex);
		return;
	}

	m_stPreviewLastEffectPath = stEffectPath;

	CEffectManager& rkEftMgr = CEffectManager::Instance();
	if (!rkEftMgr.RegisterEffect2(stEffectPath.c_str(), &m_dwPreviewEffectCRC, true))
	{
		m_iPreviewLastError = 2;
		TraceError("TitlePreview: RegisterEffect2 failed path=%s", stEffectPath.c_str());
		return;
	}

	m_dwPreviewEffectInstance = TITLE_PREVIEW_EFFECT_INSTANCE;
	if (rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance))
		rkEftMgr.DestroyEffectInstance(m_dwPreviewEffectInstance);

	const float fParticleScale = GetPreviewParticleScale(m_dwPreviewTitleIndex);

	rkEftMgr.CreateEffectInstance(m_dwPreviewEffectInstance, m_dwPreviewEffectCRC, fParticleScale);
	if (!rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance))
	{
		m_iPreviewLastError = 3;
		TraceError("TitlePreview: CreateEffectInstance failed path=%s", stEffectPath.c_str());
		return;
	}

	rkEftMgr.SelectEffectInstance(m_dwPreviewEffectInstance);
	if (IsScaledParticlePreviewTitle(m_dwPreviewTitleIndex))
		SetSelectedTitlePreviewPlacement(rkEftMgr, m_dwPreviewTitleIndex, 0.0f, 0.0f);
	else
		SetSelectedPreviewEffectGlobalMatrix(rkEftMgr, 0.0f, 0.0f);
	rkEftMgr.ShowEffect();
	m_bPreviewShow = true;
	m_iPreviewLastError = 0;

	if (IsDragonPreviewTitle(m_dwPreviewTitleIndex))
	{
		TraceError("TitlePreview: Show title=%u res=%u inst=0x%X crc=0x%X alive=%d path=%s",
			m_dwPreviewTitleIndex,
			m_wPreviewResourceIndex,
			m_dwPreviewEffectInstance,
			m_dwPreviewEffectCRC,
			rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance) ? 1 : 0,
			m_stPreviewLastEffectPath.c_str());
	}
}

void CPythonTitleSystem::HidePreviewEffect()
{
	m_bPreviewShow = false;

	CEffectManager& rkEftMgr = CEffectManager::Instance();
	if (m_dwPreviewEffectInstance && rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance))
	{
		rkEftMgr.SelectEffectInstance(m_dwPreviewEffectInstance);
		rkEftMgr.HideEffect();
		rkEftMgr.DestroyEffectInstance(m_dwPreviewEffectInstance);
	}

	m_dwPreviewEffectInstance = 0;
	m_dwPreviewEffectCRC = 0;
}

void CPythonTitleSystem::UpdatePreview()
{
	if (!m_bPreviewShow)
		return;

	CEffectManager& rkEftMgr = CEffectManager::Instance();
	if (!m_dwPreviewEffectInstance || !rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance))
	{
		if (IsDragonPreviewTitle(m_dwPreviewTitleIndex))
		{
			TraceError("TitlePreview: Update recreate title=%u res=%u inst=0x%X alive=%d",
				m_dwPreviewTitleIndex,
				m_wPreviewResourceIndex,
				m_dwPreviewEffectInstance,
				(m_dwPreviewEffectInstance && rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance)) ? 1 : 0);
		}
		ShowPreviewEffect();
	}
}

void CPythonTitleSystem::RenderPreviewEffectInRT(const RECT& rectRender) const
{
	m_iPreviewRenderSkipReason = 0;
	static DWORD s_dwRtTraceCounter = 0;

	if (!m_bPreviewShow)
	{
		m_iPreviewRenderSkipReason = 1;
		return;
	}

	if (!m_dwPreviewEffectInstance)
	{
		m_iPreviewRenderSkipReason = 2;
		return;
	}

	CEffectManager& rkEftMgr = CEffectManager::Instance();
	if (!rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance))
	{
		m_iPreviewRenderSkipReason = 3;
		return;
	}

	CPythonGraphic::Instance().ClearDepthBuffer();

	const float fFov = CPythonGraphic::Instance().GetFOV();
	const float fAspect = CPythonGraphic::Instance().GetAspect();
	const float fNearY = CPythonGraphic::Instance().GetNearY();
	const float fFarY = CPythonGraphic::Instance().GetFarY();

#if defined(ENABLE_FOG_FIX)
	const BOOL bIsFog = CPythonBackground::Instance().GetFogMode();
#else
	const BOOL bIsFog = STATEMANAGER.GetRenderState(D3DRS_FOGENABLE);
#endif
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, FALSE);

	const float fWidth = static_cast<float>(rectRender.right - rectRender.left);
	const float fHeight = static_cast<float>(rectRender.bottom - rectRender.top);
	if (fWidth <= 0.0f || fHeight <= 0.0f)
	{
		m_iPreviewRenderSkipReason = 6;
		STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, bIsFog);
		return;
	}

	CCameraManager::Instance().SetCurrentCamera(CCameraManager::DEFAULT_ILLUSTRATED_CAMERA);
	CCamera* pCam = CCameraManager::Instance().GetCurrentCamera();

	CPythonGraphic::instance().SetViewport(0.0f, 0.0f, fWidth, fHeight);
	CPythonGraphic::Instance().PushState();
	CPythonGraphic::Instance().SetGameRenderState();
	STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_LIGHTING, FALSE);

	if (pCam)
	{
		pCam->SetViewParams(
			D3DXVECTOR3(0.0f, -1000.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 1.0f));
		CPythonGraphic::Instance().UpdateViewMatrix();
	}

	CPythonGraphic::Instance().SetOrtho3D(fWidth, fHeight, -10000.0f, 10000.0f);

	rkEftMgr.SelectEffectInstance(m_dwPreviewEffectInstance);
	SetSelectedTitlePreviewPlacement(rkEftMgr, m_dwPreviewTitleIndex, fWidth, fHeight);
	CParticleSystemInstance::SetForceRenderWithoutFrustum(true);
	rkEftMgr.RenderEffect();
	CParticleSystemInstance::SetForceRenderWithoutFrustum(false);

	if (IsDragonPreviewTitle(m_dwPreviewTitleIndex) && ((++s_dwRtTraceCounter % 120) == 1))
	{
		TraceError("TitlePreview: RenderRT title=%u res=%u rect=(%ld,%ld,%ld,%ld) size=(%.0f,%.0f) inst=0x%X alive=%d err=%d skip=%d",
			m_dwPreviewTitleIndex,
			m_wPreviewResourceIndex,
			rectRender.left,
			rectRender.top,
			rectRender.right,
			rectRender.bottom,
			fWidth,
			fHeight,
			m_dwPreviewEffectInstance,
			rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance) ? 1 : 0,
			m_iPreviewLastError,
			m_iPreviewRenderSkipReason);
	}

	CCameraManager::Instance().ResetToPreviousCamera();
	CPythonGraphic::instance().RestoreViewport();
	CPythonGraphic::Instance().PopState();
	CPythonGraphic::Instance().SetPerspective(fFov, fAspect, fNearY, fFarY);
	STATEMANAGER.RestoreRenderState(D3DRS_LIGHTING);
	STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, bIsFog);
	m_iPreviewRenderSkipReason = 0;
}

void CPythonTitleSystem::RenderPreviewFrame() const
{
	if (!m_bShowTitle)
		return;

	RECT rectRender;
	if (!CRenderTargetManager::Instance().GetRenderTargetRect(
		CRenderTargetManager::RENDER_TARGET_INDEX_TITLE, &rectRender))
	{
		m_iPreviewRenderSkipReason = 4;
		return;
	}

	if (!CRenderTargetManager::Instance().ChangeRenderTarget(
		CRenderTargetManager::RENDER_TARGET_INDEX_TITLE))
	{
		m_iPreviewRenderSkipReason = 5;
		return;
	}

	CRenderTargetManager::Instance().ClearRenderTarget();

	if (m_pPreviewBackground)
	{
		CPythonGraphic::Instance().SetInterfaceRenderState();
		m_pPreviewBackground->Render();
	}

	CRenderTargetManager::Instance().ResetRenderTarget();
}

void CPythonTitleSystem::RenderPreviewEffectOverlay() const
{
	static DWORD s_dwOverlayTraceCounter = 0;

	if (!m_bShowTitle || !m_bPreviewShow || !m_dwPreviewEffectInstance)
		return;

	CEffectManager& rkEftMgr = CEffectManager::Instance();
	if (!rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance))
		return;

	RECT rectRender;
	if (!CRenderTargetManager::Instance().GetRenderTargetRect(
		CRenderTargetManager::RENDER_TARGET_INDEX_TITLE, &rectRender))
		return;

	const float fWidth = static_cast<float>(rectRender.right - rectRender.left);
	const float fHeight = static_cast<float>(rectRender.bottom - rectRender.top);
	if (fWidth <= 0.0f || fHeight <= 0.0f)
		return;

	const float fFov = CPythonGraphic::Instance().GetFOV();
	const float fAspect = CPythonGraphic::Instance().GetAspect();
	const float fNearY = CPythonGraphic::Instance().GetNearY();
	const float fFarY = CPythonGraphic::Instance().GetFarY();

#if defined(ENABLE_FOG_FIX)
	const BOOL bIsFog = CPythonBackground::Instance().GetFogMode();
#else
	const BOOL bIsFog = STATEMANAGER.GetRenderState(D3DRS_FOGENABLE);
#endif
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, FALSE);

	CCameraManager::Instance().SetCurrentCamera(CCameraManager::DEFAULT_ILLUSTRATED_CAMERA);
	CCamera* pCam = CCameraManager::Instance().GetCurrentCamera();

	CPythonGraphic::instance().SetViewport(
		static_cast<float>(rectRender.left),
		static_cast<float>(rectRender.top),
		fWidth,
		fHeight);
	CPythonGraphic::Instance().PushState();
	CPythonGraphic::Instance().SetGameRenderState();
	STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
	STATEMANAGER.SaveRenderState(D3DRS_LIGHTING, FALSE);

	if (pCam)
	{
		pCam->SetViewParams(
			D3DXVECTOR3(0.0f, -1000.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 1.0f));
		CPythonGraphic::Instance().UpdateViewMatrix();
	}

	CPythonGraphic::Instance().SetOrtho3D(fWidth, fHeight, -10000.0f, 10000.0f);

	rkEftMgr.SelectEffectInstance(m_dwPreviewEffectInstance);
	SetSelectedTitlePreviewPlacement(rkEftMgr, m_dwPreviewTitleIndex, fWidth, fHeight);
	CParticleSystemInstance::SetForceRenderWithoutFrustum(true);
	rkEftMgr.RenderEffect();
	CParticleSystemInstance::SetForceRenderWithoutFrustum(false);

	if (IsDragonPreviewTitle(m_dwPreviewTitleIndex) && ((++s_dwOverlayTraceCounter % 120) == 1))
	{
		TraceError("TitlePreview: RenderOverlay title=%u res=%u rect=(%ld,%ld,%ld,%ld) size=(%.0f,%.0f) inst=0x%X alive=%d path=%s",
			m_dwPreviewTitleIndex,
			m_wPreviewResourceIndex,
			rectRender.left,
			rectRender.top,
			rectRender.right,
			rectRender.bottom,
			fWidth,
			fHeight,
			m_dwPreviewEffectInstance,
			rkEftMgr.IsAliveEffect(m_dwPreviewEffectInstance) ? 1 : 0,
			m_stPreviewLastEffectPath.c_str());
	}

	CCameraManager::Instance().ResetToPreviousCamera();
	CPythonGraphic::instance().RestoreViewport();
	CPythonGraphic::Instance().PopState();
	CPythonGraphic::Instance().SetPerspective(fFov, fAspect, fNearY, fFarY);
	STATEMANAGER.RestoreRenderState(D3DRS_LIGHTING);
	STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, bIsFog);
}

void CPythonTitleSystem::RenderPreviewBackground() const
{
}

void CPythonTitleSystem::RenderPreviewEffect() const
{
}

PyObject* CPythonTitleSystem::BuildPreviewDebugTuple() const
{
	int iHasBg = m_pPreviewBackground ? 1 : 0;
	int iHasRtTex = CRenderTargetManager::Instance().GetRenderTargetTexture(
		CRenderTargetManager::RENDER_TARGET_INDEX_TITLE) ? 1 : 0;
	int iRectValid = 0;
	int iL = 0;
	int iT = 0;
	int iR = 0;
	int iB = 0;

	RECT rectRender;
	if (CRenderTargetManager::Instance().GetRenderTargetRect(
		CRenderTargetManager::RENDER_TARGET_INDEX_TITLE, &rectRender))
	{
		iRectValid = 1;
		iL = rectRender.left;
		iT = rectRender.top;
		iR = rectRender.right;
		iB = rectRender.bottom;
	}

	return Py_BuildValue(
		"(iiiiiiiiiiiiiis)",
		m_bPreviewShow ? 1 : 0,
		static_cast<int>(m_wPreviewResourceIndex),
		static_cast<int>(m_dwPreviewTitleIndex),
		static_cast<int>(m_dwPreviewEffectInstance),
		static_cast<int>(m_dwPreviewEffectCRC),
		iHasBg,
		iHasRtTex,
		iRectValid,
		iL,
		iT,
		iR,
		iB,
		m_iPreviewLastError,
		m_iPreviewRenderSkipReason,
		m_stPreviewLastEffectPath.c_str());
}
#endif

PyObject* titleSystemClear(PyObject* poSelf, PyObject* poArgs)
{
	CPythonTitleSystem::Instance().Clear();
	return Py_BuildNone();
}

PyObject* titleSystemSetHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler = nullptr;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BadArgument();
	CPythonTitleSystem::Instance().SetHandler(poHandler);
	return Py_BuildNone();
}

PyObject* titleSystemIsDataReceived(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonTitleSystem::Instance().IsDataReceived() ? 1 : 0);
}

PyObject* titleSystemGetAllTitleData(PyObject* poSelf, PyObject* poArgs)
{
	return CPythonTitleSystem::Instance().BuildAllTitleDataTuple();
}

PyObject* titleSystemGetShowList(PyObject* poSelf, PyObject* poArgs)
{
	return CPythonTitleSystem::Instance().BuildShowListTuple();
}

PyObject* titleSystemGetEquippedTitle(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonTitleSystem::Instance().GetEquippedTitle());
}

PyObject* titleSystemIsTitleEquipped(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonTitleSystem::Instance().IsTitleEquipped() ? 1 : 0);
}

PyObject* titleSystemRequestOpen(PyObject* poSelf, PyObject* poArgs)
{
	CPythonTitleSystem::Instance().RequestOpen();
	return Py_BuildNone();
}

PyObject* titleSystemReloadLocale(PyObject* poSelf, PyObject* poArgs)
{
	CPythonTitleSystem::Instance().LoadLocaleProtos();
	return Py_BuildNone();
}

PyObject* titleSystemRequestEquip(PyObject* poSelf, PyObject* poArgs)
{
	int iTitleIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iTitleIndex))
		return Py_BadArgument();
	if (iTitleIndex <= 0)
		CPythonTitleSystem::Instance().RequestUnEquip();
	else
		CPythonTitleSystem::Instance().RequestEquip(static_cast<DWORD>(iTitleIndex));
	return Py_BuildNone();
}

PyObject* titleSystemIsItemUsable(PyObject* poSelf, PyObject* poArgs)
{
	int iTitleIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iTitleIndex))
		return Py_BadArgument();
	return Py_BuildValue("i", CPythonTitleSystem::Instance().IsItemUsable(static_cast<DWORD>(iTitleIndex)) ? 1 : 0);
}

PyObject* titleSystemGetItemTooltip(PyObject* poSelf, PyObject* poArgs)
{
	int iTitleIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iTitleIndex))
		return Py_BadArgument();
	const std::string stText = CPythonTitleSystem::Instance().GetItemTooltip(static_cast<DWORD>(iTitleIndex));
	return PyString_FromString(stText.c_str());
}

PyObject* titleSystemIsTitleAvailableMap(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonTitleSystem::Instance().IsTitleAvailableMap() ? 1 : 0);
}

PyObject* titleSystemSetShow(PyObject* poSelf, PyObject* poArgs)
{
	int iShow = 1;
	PyTuple_GetInteger(poArgs, 0, &iShow);
	CPythonTitleSystem::Instance().SetShow(iShow != 0);
	return Py_BuildNone();
}

PyObject* titleSystemSetPreviewResource(PyObject* poSelf, PyObject* poArgs)
{
	int iResourceIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iResourceIndex))
		return Py_BadArgument();
	int iTitleIndex = 0;
	PyTuple_GetInteger(poArgs, 1, &iTitleIndex);
#if defined(RENDER_TARGET)
	CPythonTitleSystem::Instance().SetPreviewResource(
		static_cast<WORD>(iResourceIndex),
		static_cast<DWORD>(iTitleIndex));
#endif
	return Py_BuildNone();
}

PyObject* titleSystemShowEffect(PyObject* poSelf, PyObject* poArgs)
{
#if defined(RENDER_TARGET)
	CPythonTitleSystem::Instance().ShowPreviewEffect();
	return Py_BuildValue("i", CPythonTitleSystem::Instance().IsPreviewShown() ? 1 : 0);
#else
	return Py_BuildValue("i", 0);
#endif
}

PyObject* titleSystemHideEffect(PyObject* poSelf, PyObject* poArgs)
{
#if defined(RENDER_TARGET)
	CPythonTitleSystem::Instance().HidePreviewEffect();
#endif
	return Py_BuildNone();
}

PyObject* titleSystemGetPreviewDebug(PyObject* poSelf, PyObject* poArgs)
{
#if defined(RENDER_TARGET)
	return CPythonTitleSystem::Instance().BuildPreviewDebugTuple();
#else
	return Py_BuildValue("(iiiiiiiiiiiiiis)", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "");
#endif
}

PyObject* titleSystemGetImagePath(PyObject* poSelf, PyObject* poArgs)
{
	int iResourceIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iResourceIndex))
		return Py_BadArgument();
	const std::string stPath = CPythonTitleSystem::Instance().GetImagePath(static_cast<WORD>(iResourceIndex));
	return PyString_FromString(stPath.c_str());
}

PyObject* titleSystemGetSpriteImages(PyObject* poSelf, PyObject* poArgs)
{
	int iResourceIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iResourceIndex))
		return Py_BadArgument();
	return CPythonTitleSystem::Instance().GetSpriteImages(static_cast<WORD>(iResourceIndex));
}

PyObject* titleSystemGetSpriteInfo(PyObject* poSelf, PyObject* poArgs)
{
	int iResourceIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iResourceIndex))
		return Py_BadArgument();
	return CPythonTitleSystem::Instance().GetSpriteInfo(static_cast<WORD>(iResourceIndex));
}

void initTitleSystem()
{
	CPythonTitleSystem::Instance().LoadLocaleProtos();

	static PyMethodDef s_methods[] =
	{
		{ "Clear", titleSystemClear, METH_VARARGS },
		{ "SetHandler", titleSystemSetHandler, METH_VARARGS },
		{ "ReloadLocale", titleSystemReloadLocale, METH_VARARGS },
		{ "IsDataReceived", titleSystemIsDataReceived, METH_VARARGS },
		{ "GetAllTitleData", titleSystemGetAllTitleData, METH_VARARGS },
		{ "GetShowList", titleSystemGetShowList, METH_VARARGS },
		{ "GetEquippedTitle", titleSystemGetEquippedTitle, METH_VARARGS },
		{ "IsTitleEquipped", titleSystemIsTitleEquipped, METH_VARARGS },
		{ "RequestOpen", titleSystemRequestOpen, METH_VARARGS },
		{ "RequestEquip", titleSystemRequestEquip, METH_VARARGS },
		{ "IsItemUsable", titleSystemIsItemUsable, METH_VARARGS },
		{ "GetItemTooltip", titleSystemGetItemTooltip, METH_VARARGS },
		{ "IsTitleAvailableMap", titleSystemIsTitleAvailableMap, METH_VARARGS },
		{ "SetShow", titleSystemSetShow, METH_VARARGS },
		{ "SetPreviewResource", titleSystemSetPreviewResource, METH_VARARGS },
		{ "ShowEffect", titleSystemShowEffect, METH_VARARGS },
		{ "HideEffect", titleSystemHideEffect, METH_VARARGS },
		{ "GetPreviewDebug", titleSystemGetPreviewDebug, METH_VARARGS },
		{ "GetImagePath", titleSystemGetImagePath, METH_VARARGS },
		{ "GetSpriteImages", titleSystemGetSpriteImages, METH_VARARGS },
		{ "GetSpriteInfo", titleSystemGetSpriteInfo, METH_VARARGS },
		{ nullptr, nullptr, 0 },
	};

	PyObject* poModule = Py_InitModule("titleSystem", s_methods);

	PyModule_AddIntConstant(poModule, "TYPE_NONE", CPythonTitleSystem::TYPE_NONE);
	PyModule_AddIntConstant(poModule, "TYPE_TEXT", CPythonTitleSystem::TYPE_TEXT);
	PyModule_AddIntConstant(poModule, "TYPE_IMAGE", CPythonTitleSystem::TYPE_IMAGE);
	PyModule_AddIntConstant(poModule, "TYPE_EFFECT", CPythonTitleSystem::TYPE_EFFECT);
	PyModule_AddIntConstant(poModule, "TYPE_NAMEPLATE", CPythonTitleSystem::TYPE_NAMEPLATE);

	PyModule_AddIntConstant(poModule, "DATA_COLUMN_TITLE_INDEX", CPythonTitleSystem::DATA_COLUMN_TITLE_INDEX);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_TITLE_TYPE", CPythonTitleSystem::DATA_COLUMN_TITLE_TYPE);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_NAME", CPythonTitleSystem::DATA_COLUMN_NAME);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_CONDITION_TOOLTIP", CPythonTitleSystem::DATA_COLUMN_CONDITION_TOOLTIP);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_IS_PERMANENT", CPythonTitleSystem::DATA_COLUMN_IS_PERMANENT);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_OPEN_TIME", CPythonTitleSystem::DATA_COLUMN_OPEN_TIME);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_FONT_COLOR", CPythonTitleSystem::DATA_COLUMN_FONT_COLOR);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_RESOURCE_INDEX", CPythonTitleSystem::DATA_COLUMN_RESOURCE_INDEX);
	PyModule_AddIntConstant(poModule, "DATA_COLUMN_MAX", CPythonTitleSystem::DATA_COLUMN_MAX);

	PyModule_AddIntConstant(poModule, "PLAYER_COLUMN_TITLE_INDEX", CPythonTitleSystem::PLAYER_COLUMN_TITLE_INDEX);
	PyModule_AddIntConstant(poModule, "PLAYER_COLUMN_END_TIME", CPythonTitleSystem::PLAYER_COLUMN_END_TIME);
	PyModule_AddIntConstant(poModule, "PLAYER_COLUMN_IS_EQUIP", CPythonTitleSystem::PLAYER_COLUMN_IS_EQUIP);
	PyModule_AddIntConstant(poModule, "PLAYER_COLUMN_IS_OBTAIN", CPythonTitleSystem::PLAYER_COLUMN_IS_OBTAIN);
	PyModule_AddIntConstant(poModule, "PLAYER_COLUMN_MAX", CPythonTitleSystem::PLAYER_COLUMN_MAX);

	PyModule_AddIntConstant(poModule, "ALREAY_GET", CPythonTitleSystem::ALREAY_GET);
	PyModule_AddIntConstant(poModule, "WRONG_APPROACH", CPythonTitleSystem::WRONG_APPROACH);
	PyModule_AddIntConstant(poModule, "WRONG_ITEM", CPythonTitleSystem::WRONG_ITEM);
	PyModule_AddIntConstant(poModule, "END_TITLE", CPythonTitleSystem::END_TITLE);
	PyModule_AddIntConstant(poModule, "GET_TITLE", CPythonTitleSystem::GET_TITLE);
	PyModule_AddIntConstant(poModule, "EQUIP_TITLE", CPythonTitleSystem::EQUIP_TITLE);
	PyModule_AddIntConstant(poModule, "UNEQUIP_TITLE", CPythonTitleSystem::UNEQUIP_TITLE);
	PyModule_AddIntConstant(poModule, "ONE_HOUR_LEFT", CPythonTitleSystem::ONE_HOUR_LEFT);
	PyModule_AddIntConstant(poModule, "TRY_LATER", CPythonTitleSystem::TRY_LATER);
	PyModule_AddIntConstant(poModule, "CHECK_UI", CPythonTitleSystem::CHECK_UI);

	PyModule_AddIntConstant(poModule, "IMAGE_TAG", CPythonTitleSystem::IMAGE_TAG);
	PyModule_AddIntConstant(poModule, "IMAGE_PATH", CPythonTitleSystem::IMAGE_PATH);
	PyModule_AddIntConstant(poModule, "IMAGE_MAX", CPythonTitleSystem::IMAGE_MAX);
	PyModule_AddIntConstant(poModule, "TAG_LEFT_IMAGE", CPythonTitleSystem::TAG_LEFT_IMAGE);
	PyModule_AddIntConstant(poModule, "TAG_RIGHT_IMAGE", CPythonTitleSystem::TAG_RIGHT_IMAGE);
	PyModule_AddIntConstant(poModule, "TAG_MIDDLE_IMAGE", CPythonTitleSystem::TAG_MIDDLE_IMAGE);
	PyModule_AddIntConstant(poModule, "TAG_SPRITES", CPythonTitleSystem::TAG_SPRITES);
	PyModule_AddIntConstant(poModule, "SPRITE_SIZE_X", CPythonTitleSystem::SPRITE_SIZE_X);
	PyModule_AddIntConstant(poModule, "SPRITE_SIZE_Y", CPythonTitleSystem::SPRITE_SIZE_Y);
	PyModule_AddIntConstant(poModule, "SPRITE_SIZE_MAX", CPythonTitleSystem::SPRITE_SIZE_MAX);
}

#endif
