#pragma once

#if defined(ENABLE_TITLE_SYSTEM)

#include "../eterBase/Singleton.h"

#if defined(RENDER_TARGET)
class CGraphicImageInstance;
#endif

class CPythonTitleSystem : public CSingleton<CPythonTitleSystem>
{
public:
	enum ETitleSystemType
	{
		TYPE_NONE = 0,
		TYPE_TEXT = 1,
		TYPE_IMAGE = 2,
		TYPE_EFFECT = 3,
		TYPE_NAMEPLATE = 4,
	};

	enum ETitleSystemMessage
	{
		ALREAY_GET = 0,
		WRONG_APPROACH = 1,
		WRONG_ITEM = 2,
		END_TITLE = 3,
		GET_TITLE = 4,
		EQUIP_TITLE = 5,
		UNEQUIP_TITLE = 6,
		ONE_HOUR_LEFT = 7,
		TRY_LATER = 8,
		CHECK_UI = 9,
	};

	enum EDataColumn
	{
		DATA_COLUMN_TITLE_INDEX = 0,
		DATA_COLUMN_TITLE_TYPE = 1,
		DATA_COLUMN_NAME = 2,
		DATA_COLUMN_CONDITION_TOOLTIP = 3,
		DATA_COLUMN_IS_PERMANENT = 4,
		DATA_COLUMN_OPEN_TIME = 5,
		DATA_COLUMN_FONT_COLOR = 6,
		DATA_COLUMN_RESOURCE_INDEX = 7,
		DATA_COLUMN_MAX = 8,
	};

	enum EPlayerColumn
	{
		PLAYER_COLUMN_TITLE_INDEX = 0,
		PLAYER_COLUMN_END_TIME = 1,
		PLAYER_COLUMN_IS_EQUIP = 2,
		PLAYER_COLUMN_IS_OBTAIN = 3,
		PLAYER_COLUMN_MAX = 4,
	};

	enum EImageTag
	{
		IMAGE_TAG = 0,
		IMAGE_PATH = 1,
		IMAGE_MAX = 2,
		TAG_LEFT_IMAGE = 3,
		TAG_RIGHT_IMAGE = 4,
		TAG_MIDDLE_IMAGE = 5,
		TAG_SPRITES = 6,
		SPRITE_SIZE_X = 0,
		SPRITE_SIZE_Y = 1,
		SPRITE_SIZE_MAX = 2,
	};

	enum EGCSubHeader
	{
		SUBHEADER_GC_TITLE_TABLE = 0,
		SUBHEADER_GC_TITLE_PLAYER = 1,
		SUBHEADER_GC_TITLE_EQUIPPED = 2,
		SUBHEADER_GC_TITLE_NOTIFY = 3,
		SUBHEADER_GC_TITLE_END = 4,
	};

	enum ECGSubHeader
	{
		SUBHEADER_CG_TITLE_OPEN = 0,
		SUBHEADER_CG_TITLE_EQUIP = 1,
		SUBHEADER_CG_TITLE_UNEQUIP = 2,
	};

	struct STitleData
	{
		DWORD dwTitleIndex;
		BYTE bTitleType;
		std::string stName;
		std::string stCondition;
		BYTE bIsPermanent;
		DWORD dwOpenTime;
		int iFontColor;
		WORD wResourceIndex;
		std::string stEffectPath;
		std::string stImagePath;
	};

	struct SPlayerData
	{
		DWORD dwTitleIndex;
		DWORD dwEndTime;
		BYTE bIsEquip;
		BYTE bIsObtain;
	};

	CPythonTitleSystem();
	virtual ~CPythonTitleSystem();

	void Clear();
	void LoadLocaleProtos();
	void SetHandler(PyObject* poHandler);
	bool IsDataReceived() const { return m_bDataReceived; }

	void RecvPacket(BYTE bSubHeader, WORD wCount, const char* c_pData, int iRemainSize);

	bool RequestOpen();
	bool RequestEquip(DWORD dwTitleIndex);
	bool RequestUnEquip();

	DWORD GetEquippedTitle() const { return m_dwEquippedTitle; }
	bool IsTitleEquipped() const { return m_dwEquippedTitle != 0; }

	PyObject* BuildAllTitleDataTuple() const;
	PyObject* BuildShowListTuple() const;

	const STitleData* GetTitleData(DWORD dwTitleIndex) const;
	const SPlayerData* GetPlayerData(DWORD dwTitleIndex) const;

	bool IsItemUsable(DWORD dwTitleIndex) const;
	std::string GetItemTooltip(DWORD dwTitleIndex) const;
	bool IsTitleAvailableMap() const { return true; }

	void SetShow(bool bShow) { m_bShowTitle = bShow; }
	bool GetShow() const { return m_bShowTitle; }

	std::string GetImagePath(WORD wResourceIndex) const;
	PyObject* GetSpriteImages(WORD wResourceIndex) const;
	PyObject* GetSpriteInfo(WORD wResourceIndex) const;
	bool GetNameplateImages(WORD wResourceIndex, std::string& outLeft, std::string& outMiddle, std::string& outRight) const;
	CResource* GetSpriteFrameResource(WORD wResourceIndex, int iFrame) const;

#if defined(RENDER_TARGET)
	void CreatePreviewBackground(DWORD dwWidth, DWORD dwHeight);
	void SetPreviewResource(WORD wResourceIndex, DWORD dwTitleIndex = 0);
	void ShowPreviewEffect();
	void HidePreviewEffect();
	void UpdatePreview();
	void RenderPreviewFrame() const;
	void RenderPreviewEffectOverlay() const;
	void RenderPreviewBackground() const;
	void RenderPreviewEffect() const;
	bool IsPreviewShown() const { return m_bPreviewShow; }
	bool IsPreviewEffectInstance(DWORD dwInstanceIndex) const;
	PyObject* BuildPreviewDebugTuple() const;
#endif

private:
	void OnNotify(BYTE bMessage, DWORD dwTitleIndex);
	PyObject* BuildTitleRowTuple(const STitleData& r) const;
	PyObject* BuildPlayerRowTuple(const SPlayerData& r) const;

	PyObject* m_poHandler;
	bool m_bDataReceived;
	bool m_bShowTitle;
	DWORD m_dwEquippedTitle;

	std::map<DWORD, STitleData> m_mapTitleData;
	std::map<DWORD, SPlayerData> m_mapPlayerData;
	std::map<WORD, BYTE> m_mapResourceTypes;
	std::map<WORD, std::string> m_mapResourceSubFiles;
	std::vector<DWORD> m_vecDisplayOrder;
	bool m_bLocaleLoaded;

#if defined(RENDER_TARGET)
	bool ParseSubEffectPath(WORD wResourceIndex, std::string& outPath) const;
	bool ResolvePreviewEffectPath(std::string& outPath) const;
	void RenderPreviewEffectInRT(const RECT& rectRender) const;
	CGraphicImageInstance* m_pPreviewBackground;
	bool m_bPreviewShow;
	WORD m_wPreviewResourceIndex;
	DWORD m_dwPreviewTitleIndex;
	DWORD m_dwPreviewEffectInstance;
	DWORD m_dwPreviewEffectCRC;
	int m_iPreviewLastError;
	std::string m_stPreviewLastEffectPath;
	mutable int m_iPreviewRenderSkipReason;
#endif
};

void initTitleSystem();

#endif
