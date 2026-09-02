#include "stdafx.h"

#if defined(__TITLE_SYSTEM__)

#include "title_system.h"
#include "char.h"
#include "desc.h"
#include "item.h"
#include "log.h"
#include "config.h"
#include "db.h"
#include "utils.h"
#include "buffer_manager.h"

namespace
{
	const char* TITLE_EFFECT_CLEAR_TOKEN = "__TITLE_EFFECT_CLEAR__";
	const char* TITLE_EQUIPPED_FLAG = "title_system.active";
	const char* TITLE_WARNED_FLAG_FMT = "title_system.warn.%u";

	const int TITLE_ACTION_COOLDOWN_SEC = 1;
	const char* TITLE_COOLDOWN_FLAG = "title_system.cooldown";

	bool CanAct(LPCHARACTER ch)
	{
		if (!ch)
			return false;

		const int iNow = get_global_time();
		const int iCooldown = ch->GetQuestFlag(TITLE_COOLDOWN_FLAG);
		if (iCooldown > iNow)
			return false;

		ch->SetQuestFlag(TITLE_COOLDOWN_FLAG, iNow + TITLE_ACTION_COOLDOWN_SEC);
		return true;
	}

	DWORD GetItemTitleIndex(LPITEM item)
	{
		if (!item)
			return 0;

		const long lSocketTitle = item->GetSocket(1);
		if (lSocketTitle > 0)
			return static_cast<DWORD>(lSocketTitle);

		const long lValue = item->GetValue(0);
		if (lValue > 0)
			return static_cast<DWORD>(lValue);

		return 0;
	}

	int GetItemDurationSec(LPITEM item)
	{
		if (!item)
			return 0;

		const long lSocketEnd = item->GetSocket(0);
		if (lSocketEnd > 0)
			return static_cast<int>(lSocketEnd);

		int iDuration = item->GetValue(1);
		if (iDuration <= 0)
			iDuration = item->GetValue(2);

		return iDuration;
	}
}

void CTitleSystemManager::Initialize()
{
	if (!LoadProtoTable())
		sys_err("TITLE_SYSTEM: failed to load title_proto (import title_system.sql)");
}

bool CTitleSystemManager::LoadProtoTable()
{
	m_mapProto.clear();
	m_vecProtoOrder.clear();

	struct SStaticProto
	{
		WORD wDisplayOrder;
		DWORD dwTitleIndex;
		BYTE bTitleType;
		const char* szName;
		const char* szCondition;
		BYTE bIsPermanent;
		DWORD dwOpenTime;
		DWORD dwDefaultDuration;
		int iFontColor;
		WORD wResourceIndex;
		const char* szEffectPath;
		const char* szImagePath;
	};

	static const SStaticProto s_aProtos[] =
	{
		{ 1, 1, 3, "Son kurtulan", "Battle Royale'da 1. siraya ulas.", 0, 0, 10800, -7434610, 2000, "d:/ymir work/effect/battleroyale/crown01.mse", "" },
		{ 2, 2, 3, "Yenilmez", "Savas bolgesi siralamasinda 1. siraya ulas.", 0, 0, 604800, -7434610, 2001, "d:/ymir work/effect/etc/ranking_battle/ranker_battle_01.mse", "" },
		{ 3, 3, 3, "Savas habercisi", "Savas bolgesi siralamasinda 2. siraya ulas.", 0, 0, 604800, -7434610, 2002, "d:/ymir work/effect/etc/ranking_battle/ranker_battle_02.mse", "" },
		{ 4, 4, 3, "Kasap", "Savas bolgesi siralamasinda 3. siraya ulas.", 0, 0, 604800, -7434610, 2003, "d:/ymir work/effect/etc/ranking_battle/ranker_battle_03.mse", "" },
		{ 5, 6, 3, "Alevli Fatih", "Meley'in Ini (Lonca) haftalik siralama listesinde 1. siraya ulas.", 0, 0, 604800, -7434610, 3005, "d:/ymir work/effect/etc/title/title_dragon_red.mse", "" },
		{ 6, 7, 3, "Sogukkanli Hakim", "Balathor'un Mahzeni (Lonca) haftalik siralama listesinde 1. siraya ulas.", 0, 0, 604800, -7434610, 3006, "d:/ymir work/effect/etc/title/title_dragon_gw.mse", "" },
		{ 7, 1000, 4, "Sansli", "Aylik Metin+ cekilisinde kazan.", 1, 1754643600, 0, -7434610, 1005, "", "" },
		{ 8, 1001, 4, "Metin+", "Aktif bir Metin+ uyeligine sahip ol.", 0, 1754643600, 0, -7434610, 3000, "d:/ymir work/effect/etc/title/title_01_shield.mse", "" },
		{ 9, 1002, 4, "Destansi", "Metin+ uyeliginde Destansi veya daha yuksek rutbe.", 0, 1754643600, 0, -7434610, 1003, "", "" },
		{ 10, 1003, 4, "Efsanevi", "Metin+ uyeliginde Efsanevi veya daha yuksek rutbe.", 0, 1754643600, 0, -7434610, 1003, "", "" },
		{ 11, 1004, 4, "Mistik", "Metin+ uyeliginde Mistik veya daha yuksek rutbe.", 0, 1754643600, 0, -7434610, 1003, "", "" },
		{ 12, 1005, 4, "Gunessever", "2025 Yaz Tombalasi'nda kazanilabilir.", 1, 1754006400, 0, -7434610, 1000, "d:/ymir work/effect/etc/title/title_06_banner_gold.mse", "" },
		{ 13, 1006, 4, "Kutup Yildizi", "2025 Kis Tombalasi'nda kazanilabilir.", 1, 1766620800, 0, -7434610, 1002, "d:/ymir work/effect/etc/title/title_08_banner_blue.mse", "" },
		{ 14, 1007, 4, "Nyx Sampiyonu", "Yohara Nyx sunucusundan Seviye I+ transfer.", 1, 1770195600, 0, -7434610, 1003, "", "" },
		{ 15, 1008, 4, "Chione Sampiyonu", "Yohara Chione sunucusundan Seviye I+ transfer.", 1, 1770195600, 0, -7434610, 1003, "", "" },
		{ 16, 1009, 4, "Lodos Sampiyonu", "Yohara Lodos sunucusundan Seviye I+ transfer.", 1, 1770195600, 0, -7434610, 1003, "", "" },
		{ 17, 1010, 4, "Firtina Sampiyonu", "Yohara Firtina sunucusundan Seviye I+ transfer.", 1, 1770195600, 0, -7434610, 1003, "", "" },
		{ 18, 1011, 4, "Okey Buyuk Ustadi", "TREASURE sunucusunda Okey turunda 500+ puan.", 1, 1775001600, 0, -7434610, 1001, "d:/ymir work/effect/etc/title/title_07_banner_red.mse", "" },
		{ 19, 1012, 4, "Jeton Avcisi", "TREASURE sunucusunda toplam 10.000 Dublon topla.", 1, 1775001600, 0, -7434610, 1004, "", "" }
	};

	for (const auto& r : s_aProtos)
	{
		STitleProto proto = {};
		proto.wDisplayOrder = r.wDisplayOrder;
		proto.dwTitleIndex = r.dwTitleIndex;
		proto.bTitleType = r.bTitleType;
		strlcpy(proto.szName, r.szName, sizeof(proto.szName));
		strlcpy(proto.szCondition, r.szCondition, sizeof(proto.szCondition));
		proto.bIsPermanent = r.bIsPermanent;
		proto.dwOpenTime = r.dwOpenTime;
		proto.dwDefaultDuration = r.dwDefaultDuration;
		proto.iFontColor = r.iFontColor;
		proto.wResourceIndex = r.wResourceIndex;
		strlcpy(proto.szEffectPath, r.szEffectPath, sizeof(proto.szEffectPath));
		strlcpy(proto.szImagePath, r.szImagePath, sizeof(proto.szImagePath));

		m_mapProto[proto.dwTitleIndex] = proto;
		m_vecProtoOrder.push_back(proto.dwTitleIndex);
	}

	sys_log(0, "TITLE_SYSTEM: loaded %u static proto rows", m_mapProto.size());
	return !m_mapProto.empty();
}

const CTitleSystemManager::STitleProto* CTitleSystemManager::FindProto(DWORD dwTitleIndex) const
{
	std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.find(dwTitleIndex);
	if (it == m_mapProto.end())
		return NULL;
	return &it->second;
}

const CTitleSystemManager::STitleProto* CTitleSystemManager::FindProtoByName(const std::string& name) const
{
	std::string cleanName = name;
	cleanName.erase(0, cleanName.find_first_not_of(" \t\r\n"));
	cleanName.erase(cleanName.find_last_not_of(" \t\r\n") + 1);

	for (std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.begin(); it != m_mapProto.end(); ++it)
	{
		if (cleanName == it->second.szName)
			return &it->second;

		if (strcasecmp(it->second.szName, cleanName.c_str()) == 0)
			return &it->second;

		auto checkMatch = [](const char* s1, const char* s2) -> bool {
			while (*s1 && *s2) {
				unsigned char c1 = *s1;
				unsigned char c2 = *s2;
				if (c1 >= 'A' && c1 <= 'Z') c1 = c1 - 'A' + 'a';
				if (c2 >= 'A' && c2 <= 'Z') c2 = c2 - 'A' + 'a';
				if (c1 == 0xdd) c1 = 0x69; // İ -> i
				if (c2 == 0xdd) c2 = 0x69;
				if (c1 == 0x49) c1 = 0xfd; // I -> ı (Turkish cp1254/latin5)
				if (c2 == 0x49) c2 = 0xfd;
				if (c1 != c2) return false;
				s1++;
				s2++;
			}
			return *s1 == *s2;
		};

		if (checkMatch(it->second.szName, cleanName.c_str()))
			return &it->second;
	}
	return NULL;
}

const CTitleSystemManager::STitleProto* CTitleSystemManager::FindProtoByResource(WORD wResourceIndex) const
{
	for (std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.begin(); it != m_mapProto.end(); ++it)
	{
		if (it->second.wResourceIndex == wResourceIndex)
			return &it->second;
	}
	return NULL;
}

const char* CTitleSystemManager::GetEffectPath(DWORD dwTitleIndex) const
{
	const STitleProto* p = FindProto(dwTitleIndex);
	if (!p || !p->szEffectPath[0])
		return "";
	return p->szEffectPath;
}

void CTitleSystemManager::LoadPlayerTitles(LPCHARACTER ch)
{
	if (!ch)
		return;

	// Clear all title quest flags to avoid legacy state leaking
	for (std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.begin(); it != m_mapProto.end(); ++it)
	{
		char szOwned[64];
		char szExpire[64];
		char szWarn[64];
		snprintf(szOwned, sizeof(szOwned), "title_system.owned.%u", it->first);
		snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", it->first);
		snprintf(szWarn, sizeof(szWarn), TITLE_WARNED_FLAG_FMT, it->first);
		ch->SetQuestFlag(szOwned, 0);
		ch->SetQuestFlag(szExpire, 0);
		ch->SetQuestFlag(szWarn, 0);
	}

	char szQuery[256];
	snprintf(szQuery, sizeof(szQuery),
		"SELECT `title_index`, `end_time` FROM player_title%s WHERE `pid` = %u",
		get_table_postfix(), ch->GetPlayerID());

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
	SQLResult* pRes = pMsg->Get();
	if (!pRes)
		return;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(pRes->pSQLResult)))
	{
		DWORD dwTitleIndex = 0;
		DWORD dwEndTime = 0;
		str_to_number(dwTitleIndex, row[0]);
		str_to_number(dwEndTime, row[1]);

		bool bIsLegacy = false;
		const STitleProto* p = FindProto(dwTitleIndex);
		if (!p)
		{
			p = FindProtoByResource(static_cast<WORD>(dwTitleIndex));
			if (p)
			{
				char szDel[256];
				snprintf(szDel, sizeof(szDel),
					"DELETE FROM player_title%s WHERE `pid` = %u AND `title_index` = %u",
					get_table_postfix(), ch->GetPlayerID(), dwTitleIndex);
				DBManager::instance().DirectQuery(szDel);

				dwTitleIndex = p->dwTitleIndex;
				bIsLegacy = true;
			}
			else
			{
				continue;
			}
		}

		if (dwEndTime == 0)
		{
			if (p->bIsPermanent)
				dwEndTime = get_global_time() + 31536000; // 365 days
			else
				dwEndTime = get_global_time() + (p->dwDefaultDuration > 0 ? p->dwDefaultDuration : 31536000);

			SavePlayerTitle(ch, dwTitleIndex, dwEndTime);
		}
		else
		{
			if (bIsLegacy)
			{
				SavePlayerTitle(ch, dwTitleIndex, dwEndTime);
			}
			else
			{
				char szOwned[64];
				char szExpire[64];
				snprintf(szOwned, sizeof(szOwned), "title_system.owned.%u", dwTitleIndex);
				snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", dwTitleIndex);
				ch->SetQuestFlag(szOwned, 1);
				ch->SetQuestFlag(szExpire, dwEndTime);
			}
		}
	}
}

void CTitleSystemManager::SavePlayerTitle(LPCHARACTER ch, DWORD dwTitleIndex, DWORD dwEndTime)
{
	if (!ch)
		return;

	char szQuery[512];
	snprintf(szQuery, sizeof(szQuery),
		"REPLACE INTO player_title%s (`pid`, `title_index`, `end_time`) VALUES (%u, %u, %u)",
		get_table_postfix(), ch->GetPlayerID(), dwTitleIndex, dwEndTime);

	DBManager::instance().DirectQuery(szQuery);

	char szOwned[64];
	char szExpire[64];
	snprintf(szOwned, sizeof(szOwned), "title_system.owned.%u", dwTitleIndex);
	snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", dwTitleIndex);
	ch->SetQuestFlag(szOwned, 1);
	ch->SetQuestFlag(szExpire, dwEndTime);
}

void CTitleSystemManager::DeletePlayerTitle(LPCHARACTER ch, DWORD dwTitleIndex)
{
	if (!ch)
		return;

	char szQuery[256];
	snprintf(szQuery, sizeof(szQuery),
		"DELETE FROM player_title%s WHERE `pid` = %u AND `title_index` = %u",
		get_table_postfix(), ch->GetPlayerID(), dwTitleIndex);
	DBManager::instance().DirectQuery(szQuery);

	char szOwned[64];
	char szExpire[64];
	snprintf(szOwned, sizeof(szOwned), "title_system.owned.%u", dwTitleIndex);
	snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", dwTitleIndex);
	ch->SetQuestFlag(szOwned, 0);
	ch->SetQuestFlag(szExpire, 0);

	if (GetEquippedTitle(ch) == dwTitleIndex)
		UnequipTitle(ch, true);

	SendPlayerTitles(ch);
}

void CTitleSystemManager::SaveEquippedTitle(LPCHARACTER ch)
{
	if (!ch)
		return;

	ch->SetQuestFlag(TITLE_EQUIPPED_FLAG, static_cast<int>(GetEquippedTitle(ch)));
}

bool CTitleSystemManager::IsTitleOwned(LPCHARACTER ch, DWORD dwTitleIndex) const
{
	if (!ch)
		return false;

	char szOwned[64];
	snprintf(szOwned, sizeof(szOwned), "title_system.owned.%u", dwTitleIndex);
	return ch->GetQuestFlag(szOwned) > 0;
}

bool CTitleSystemManager::IsTitleExpired(LPCHARACTER ch, DWORD dwTitleIndex) const
{
	if (!ch)
		return true;

	char szExpire[64];
	snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", dwTitleIndex);
	const int iExpireAt = ch->GetQuestFlag(szExpire);
	if (iExpireAt <= 0)
		return false;

	return iExpireAt <= get_global_time();
}

DWORD CTitleSystemManager::GetEquippedTitle(LPCHARACTER ch) const
{
	if (!ch)
		return 0;

	const int iActive = ch->GetQuestFlag(TITLE_EQUIPPED_FLAG);
	return iActive > 0 ? static_cast<DWORD>(iActive) : 0;
}

void CTitleSystemManager::SendOpen(LPCHARACTER ch)
{
	if (!ch || !ch->GetDesc())
		return;

	LPDESC d = ch->GetDesc();

	{
		TEMP_BUFFER buf;
		for (size_t iOrder = 0; iOrder < m_vecProtoOrder.size(); ++iOrder)
		{
			std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.find(m_vecProtoOrder[iOrder]);
			if (it == m_mapProto.end())
				continue;
			const STitleProto& r = it->second;
			TPacketGCTitleSystemTableRow row = {};
			row.dwTitleIndex = r.dwTitleIndex;
			row.bTitleType = r.bTitleType;
			strlcpy(row.szName, r.szName, sizeof(row.szName));
			strlcpy(row.szCondition, r.szCondition, sizeof(row.szCondition));
			row.bIsPermanent = r.bIsPermanent;
			row.dwOpenTime = r.dwOpenTime;
			row.iFontColor = r.iFontColor;
			row.wResourceIndex = r.wResourceIndex;
			strlcpy(row.szEffectPath, r.szEffectPath, sizeof(row.szEffectPath));
			strlcpy(row.szImagePath, r.szImagePath, sizeof(row.szImagePath));
			buf.write(&row, sizeof(row));
		}

		TPacketGCTitleSystem pack = {};
		pack.bHeader = HEADER_GC_TITLE_SYSTEM;
		pack.wSize = sizeof(pack) + buf.size();
		pack.bSubHeader = SUBHEADER_GC_TITLE_TABLE;
		pack.wCount = static_cast<WORD>(buf.size() / sizeof(TPacketGCTitleSystemTableRow));
		sys_err("TITLE_SYSTEM DEBUG: SendOpen TABLE wSize=%d buf.size()=%d wCount=%d sizeof(row)=%d", pack.wSize, buf.size(), pack.wCount, sizeof(TPacketGCTitleSystemTableRow));
		if (buf.size())
		{
			d->BufferedPacket(&pack, sizeof(pack));
			d->Packet(buf.read_peek(), buf.size());
		}
		else
			d->Packet(&pack, sizeof(pack));
	}

	SendPlayerTitles(ch);

	SendEquipped(ch, GetEquippedTitle(ch));

	TPacketGCTitleSystem endPack = {};
	endPack.bHeader = HEADER_GC_TITLE_SYSTEM;
	endPack.wSize = sizeof(endPack);
	endPack.bSubHeader = SUBHEADER_GC_TITLE_END;
	endPack.wCount = 0;
	d->Packet(&endPack, sizeof(endPack));
}

void CTitleSystemManager::SendPlayerTitles(LPCHARACTER ch)
{
	if (!ch || !ch->GetDesc())
		return;

	LPDESC d = ch->GetDesc();
	TEMP_BUFFER buf;

	for (size_t iOrder = 0; iOrder < m_vecProtoOrder.size(); ++iOrder)
	{
		const DWORD dwTitleIndex = m_vecProtoOrder[iOrder];
		if (!IsTitleOwned(ch, dwTitleIndex) || IsTitleExpired(ch, dwTitleIndex))
			continue;

		char szExpire[64];
		snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", dwTitleIndex);

		TPacketGCTitleSystemPlayerRow row = {};
		row.dwTitleIndex = dwTitleIndex;
		row.dwEndTime = static_cast<DWORD>(ch->GetQuestFlag(szExpire));
		row.bIsObtain = 1;
		row.bIsEquip = (GetEquippedTitle(ch) == dwTitleIndex) ? 1 : 0;
		buf.write(&row, sizeof(row));
	}

	TPacketGCTitleSystem pack = {};
	pack.bHeader = HEADER_GC_TITLE_SYSTEM;
	pack.wSize = sizeof(pack) + buf.size();
	pack.bSubHeader = SUBHEADER_GC_TITLE_PLAYER;
	pack.wCount = static_cast<WORD>(buf.size() / sizeof(TPacketGCTitleSystemPlayerRow));

	if (buf.size())
	{
		d->BufferedPacket(&pack, sizeof(pack));
		d->Packet(buf.read_peek(), buf.size());
	}
	else
	{
		d->Packet(&pack, sizeof(pack));
	}
}

void CTitleSystemManager::SendEquipped(LPCHARACTER ch, DWORD dwTitleIndex)
{
	if (!ch || !ch->GetDesc())
		return;

	TPacketGCTitleSystem pack = {};
	pack.bHeader = HEADER_GC_TITLE_SYSTEM;
	pack.wSize = sizeof(pack) + sizeof(TPacketGCTitleSystemEquipRow);
	pack.bSubHeader = SUBHEADER_GC_TITLE_EQUIPPED;
	pack.wCount = 0;

	TPacketGCTitleSystemEquipRow row = {};
	row.dwTitleIndex = dwTitleIndex;
	ch->GetDesc()->BufferedPacket(&pack, sizeof(pack));
	ch->GetDesc()->Packet(&row, sizeof(row));
}

void CTitleSystemManager::SendNotify(LPCHARACTER ch, BYTE bMessage, DWORD dwTitleIndex)
{
	if (!ch || !ch->GetDesc())
		return;

	TPacketGCTitleSystem pack = {};
	pack.bHeader = HEADER_GC_TITLE_SYSTEM;
	pack.wSize = sizeof(pack) + sizeof(TPacketGCTitleSystemNotifyRow);
	pack.bSubHeader = SUBHEADER_GC_TITLE_NOTIFY;
	pack.wCount = 0;

	TPacketGCTitleSystemNotifyRow row = {};
	row.bMessage = bMessage;
	row.dwTitleIndex = dwTitleIndex;
	ch->GetDesc()->BufferedPacket(&pack, sizeof(pack));
	ch->GetDesc()->Packet(&row, sizeof(row));
}

void CTitleSystemManager::ClearEquippedTitleVisual(LPCHARACTER ch)
{
	if (!ch)
		return;

	ch->SpecificEffectPacket(TITLE_EFFECT_CLEAR_TOKEN);
}

void CTitleSystemManager::BroadcastEquippedTitle(LPCHARACTER ch)
{
	if (!ch)
		return;

	const DWORD dwTitleIndex = GetEquippedTitle(ch);
	if (dwTitleIndex == 0)
	{
		ClearEquippedTitleVisual(ch);
		return;
	}

	if (!IsTitleOwned(ch, dwTitleIndex))
	{
		ch->SetQuestFlag(TITLE_EQUIPPED_FLAG, 0);
		SaveEquippedTitle(ch);
		ClearEquippedTitleVisual(ch);
		SendEquipped(ch, 0);
		return;
	}

	const char* c_szEffect = GetEffectPath(dwTitleIndex);
	if (c_szEffect && *c_szEffect)
	{
		char szBuf[128];
		snprintf(szBuf, sizeof(szBuf), "%s|%u", c_szEffect, dwTitleIndex);
		ch->SpecificEffectPacket(szBuf);
	}
	else
	{
		char szBuf[128];
		snprintf(szBuf, sizeof(szBuf), "d:/ymir work/effect/etc/title/none.mse|%u", dwTitleIndex);
		ch->SpecificEffectPacket(szBuf);
	}
}

bool CTitleSystemManager::GrantTitle(LPCHARACTER ch, DWORD dwTitleIndex, int iDurationSec, bool bAutoEquip)
{
	if (!ch)
		return false;

	const STitleProto* p = FindProto(dwTitleIndex);
	if (!p)
		return false;

	if (IsTitleOwned(ch, dwTitleIndex) && !IsTitleExpired(ch, dwTitleIndex))
	{
		SendNotify(ch, TITLE_MSG_ALREADY_GET, dwTitleIndex);
		return false;
	}

	DWORD dwEndTime = 0;
	if (p->bIsPermanent)
	{
		dwEndTime = get_global_time() + 31536000; // 365 days
	}
	else
	{
		int iDur = iDurationSec;
		if (iDur <= 0 && p->dwDefaultDuration > 0)
			iDur = static_cast<int>(p->dwDefaultDuration);

		if (iDur > 0)
			dwEndTime = get_global_time() + iDur;
	}

	SavePlayerTitle(ch, dwTitleIndex, dwEndTime);
	SendPlayerTitles(ch);
	SendNotify(ch, TITLE_MSG_GET_TITLE, dwTitleIndex);
	SendNotify(ch, TITLE_MSG_CHECK_UI, dwTitleIndex);

	if (bAutoEquip && GetEquippedTitle(ch) == 0)
		EquipTitle(ch, dwTitleIndex);

	return true;
}

bool CTitleSystemManager::EquipTitle(LPCHARACTER ch, DWORD dwTitleIndex)
{
	if (!ch)
		return false;

	if (!CanAct(ch))
	{
		SendNotify(ch, TITLE_MSG_TRY_LATER, dwTitleIndex);
		return false;
	}

	const STitleProto* p = FindProto(dwTitleIndex);
	if (!p)
	{
		SendNotify(ch, TITLE_MSG_WRONG_APPROACH, dwTitleIndex);
		return false;
	}

	if (!IsTitleOwned(ch, dwTitleIndex) || IsTitleExpired(ch, dwTitleIndex))
	{
		SendNotify(ch, TITLE_MSG_WRONG_APPROACH, dwTitleIndex);
		return false;
	}

	const DWORD dwCurrent = GetEquippedTitle(ch);
	if (dwCurrent == dwTitleIndex)
		return true;

	ClearEquippedTitleVisual(ch);
	ch->SetQuestFlag(TITLE_EQUIPPED_FLAG, static_cast<int>(dwTitleIndex));
	SaveEquippedTitle(ch);

	BroadcastEquippedTitle(ch);
	SendEquipped(ch, dwTitleIndex);
	SendNotify(ch, TITLE_MSG_EQUIP_TITLE, dwTitleIndex);
	return true;
}

bool CTitleSystemManager::UnequipTitle(LPCHARACTER ch, bool bSystemAction)
{
	if (!ch)
		return false;

	if (!bSystemAction && !CanAct(ch))
	{
		SendNotify(ch, TITLE_MSG_TRY_LATER, 0);
		return false;
	}

	const DWORD dwCurrent = GetEquippedTitle(ch);
	if (dwCurrent == 0)
		return true;

	ch->SetQuestFlag(TITLE_EQUIPPED_FLAG, 0);
	SaveEquippedTitle(ch);
	ClearEquippedTitleVisual(ch);
	SendEquipped(ch, 0);
	SendNotify(ch, TITLE_MSG_UNEQUIP_TITLE, dwCurrent);
	return true;
}

bool CTitleSystemManager::UseTitleItem(LPCHARACTER ch, LPITEM item)
{
	if (!ch || !item)
		return false;

	const int iDuration = GetItemDurationSec(item);
	DWORD dwTitleIndex = GetItemTitleIndex(item);
	const DWORD dwItemVnum = item->GetVnum();

	// Auto-resolve title index for certificates with single associated title
	if (dwTitleIndex == 0)
	{
		if (dwItemVnum == 57001) // Mavi Sertifika -> Şanslı (1000)
		{
			dwTitleIndex = 1000;
		}
	}

	if (dwTitleIndex == 0)
	{
		SendNotify(ch, TITLE_MSG_WRONG_ITEM, 0);
		return false;
	}

	// Validate certificate VNUM matches the allowed title index
	if (dwItemVnum == 57000) // Kırmızı Sertifika -> Mistik (1004), Efsanevi (1003), Destansı (1002), Metin+ (1001)
	{
		if (dwTitleIndex < 1001 || dwTitleIndex > 1004)
		{
			SendNotify(ch, TITLE_MSG_WRONG_ITEM, dwTitleIndex);
			return false;
		}
	}
	else if (dwItemVnum == 57001) // Mavi Sertifika -> Şanslı (1000)
	{
		if (dwTitleIndex != 1000)
		{
			SendNotify(ch, TITLE_MSG_WRONG_ITEM, dwTitleIndex);
			return false;
		}
	}
	else if (dwItemVnum == 57002) // Yeşil Sertifika -> Güneşsever (1005), Kutup Yıldızı (1006)
	{
		if (dwTitleIndex != 1005 && dwTitleIndex != 1006)
		{
			SendNotify(ch, TITLE_MSG_WRONG_ITEM, dwTitleIndex);
			return false;
		}
	}

	const STitleProto* p = FindProto(dwTitleIndex);
	if (!p)
	{
		SendNotify(ch, TITLE_MSG_WRONG_ITEM, dwTitleIndex);
		return false;
	}

	int iGrantDuration = iDuration;
	if (iGrantDuration <= 0 && p->dwDefaultDuration > 0)
		iGrantDuration = static_cast<int>(p->dwDefaultDuration);

	if (!GrantTitle(ch, dwTitleIndex, iGrantDuration, false))
		return false;

	item->SetCount(item->GetCount() - 1);
	SendOpen(ch);
	return true;
}

void CTitleSystemManager::CheckExpiredTitles(LPCHARACTER ch, bool bNotify)
{
	if (!ch)
		return;

	std::vector<DWORD> vecExpired;
	for (std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.begin(); it != m_mapProto.end(); ++it)
	{
		const DWORD dwTitleIndex = it->first;
		if (!IsTitleOwned(ch, dwTitleIndex))
			continue;
		if (!IsTitleExpired(ch, dwTitleIndex))
			continue;
		vecExpired.push_back(dwTitleIndex);
	}

	for (size_t i = 0; i < vecExpired.size(); ++i)
	{
		const DWORD dwTitleIndex = vecExpired[i];
		if (bNotify)
			SendNotify(ch, TITLE_MSG_END_TITLE, dwTitleIndex);
		DeletePlayerTitle(ch, dwTitleIndex);
	}

	if (!vecExpired.empty())
		SendOpen(ch);
}

void CTitleSystemManager::CheckOneHourWarning(LPCHARACTER ch)
{
	if (!ch)
		return;

	const int iNow = get_global_time();
	for (std::map<DWORD, STitleProto>::const_iterator it = m_mapProto.begin(); it != m_mapProto.end(); ++it)
	{
		const DWORD dwTitleIndex = it->first;
		if (!IsTitleOwned(ch, dwTitleIndex))
			continue;

		char szExpire[64];
		snprintf(szExpire, sizeof(szExpire), "title_system.expire.%u", dwTitleIndex);
		const int iExpireAt = ch->GetQuestFlag(szExpire);
		if (iExpireAt <= 0)
			continue;

		const int iRemain = iExpireAt - iNow;
		if (iRemain <= 0 || iRemain > 3600)
			continue;

		char szWarn[64];
		snprintf(szWarn, sizeof(szWarn), TITLE_WARNED_FLAG_FMT, dwTitleIndex);
		if (ch->GetQuestFlag(szWarn) > 0)
			continue;

		ch->SetQuestFlag(szWarn, 1);
		SendNotify(ch, TITLE_MSG_ONE_HOUR_LEFT, dwTitleIndex);
	}
}

void CTitleSystemManager::Process(LPCHARACTER ch)
{
	if (!ch)
		return;

	CheckExpiredTitles(ch, true);
	CheckOneHourWarning(ch);
}

void CTitleSystemManager::OnCharacterLoad(LPCHARACTER ch)
{
	if (!ch)
		return;

	LoadPlayerTitles(ch);
	CheckExpiredTitles(ch, false);

	const DWORD dwEquipped = GetEquippedTitle(ch);
	if (dwEquipped > 0 && !IsTitleOwned(ch, dwEquipped))
	{
		UnequipTitle(ch, true);
	}

	SendOpen(ch);
	BroadcastEquippedTitle(ch);
}

void CTitleSystemManager::OnCharacterDisconnect(LPCHARACTER ch)
{
	(void)ch;
}

void CTitleSystemManager::RecvCGPacket(LPCHARACTER ch, const TPacketCGTitleSystem* p)
{
	if (!ch || !p)
		return;

	switch (p->bSubHeader)
	{
		case SUBHEADER_CG_TITLE_OPEN:
			SendOpen(ch);
			break;

		case SUBHEADER_CG_TITLE_EQUIP:
			EquipTitle(ch, p->dwTitleIndex);
			break;

		case SUBHEADER_CG_TITLE_UNEQUIP:
			UnequipTitle(ch);
			break;

		default:
			sys_err("CTitleSystemManager::RecvCGPacket unknown subheader %u (%s)", p->bSubHeader, ch->GetName());
			break;
	}
}

#endif
