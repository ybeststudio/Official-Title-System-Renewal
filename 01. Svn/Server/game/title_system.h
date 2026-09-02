#pragma once

#include <vector>
#include <string>

#include "../../common/service.h"

#if defined(__TITLE_SYSTEM__)

#	include "../../common/tables.h"
#	include "packet.h"

class CTitleSystemManager : public singleton<CTitleSystemManager>
{
public:
	enum ETitleSystemType : BYTE
	{
		TITLE_TYPE_NONE = 0,
		TITLE_TYPE_TEXT = 1,
		TITLE_TYPE_IMAGE = 2,
		TITLE_TYPE_EFFECT = 3,
		TITLE_TYPE_NAMEPLATE = 4,
	};

	enum ETitleSystemMessage : BYTE
	{
		TITLE_MSG_ALREADY_GET = 0,
		TITLE_MSG_WRONG_APPROACH = 1,
		TITLE_MSG_WRONG_ITEM = 2,
		TITLE_MSG_END_TITLE = 3,
		TITLE_MSG_GET_TITLE = 4,
		TITLE_MSG_EQUIP_TITLE = 5,
		TITLE_MSG_UNEQUIP_TITLE = 6,
		TITLE_MSG_ONE_HOUR_LEFT = 7,
		TITLE_MSG_TRY_LATER = 8,
		TITLE_MSG_CHECK_UI = 9,
	};

	enum ETitleSystemCGSubHeader : BYTE
	{
		SUBHEADER_CG_TITLE_OPEN = 0,
		SUBHEADER_CG_TITLE_EQUIP = 1,
		SUBHEADER_CG_TITLE_UNEQUIP = 2,
	};

	enum ETitleSystemGCSubHeader : BYTE
	{
		SUBHEADER_GC_TITLE_TABLE = 0,
		SUBHEADER_GC_TITLE_PLAYER = 1,
		SUBHEADER_GC_TITLE_EQUIPPED = 2,
		SUBHEADER_GC_TITLE_NOTIFY = 3,
		SUBHEADER_GC_TITLE_END = 4,
	};

	struct STitleProto
	{
		WORD wDisplayOrder;
		DWORD dwTitleIndex;
		BYTE bTitleType;
		char szName[32];
		char szCondition[128];
		BYTE bIsPermanent;
		DWORD dwOpenTime;
		DWORD dwDefaultDuration;
		int iFontColor;
		WORD wResourceIndex;
		char szEffectPath[128];
		char szImagePath[128];
	};

	struct SPlayerTitle
	{
		DWORD dwTitleIndex;
		DWORD dwEndTime;
	};

	void Initialize();

	void RecvCGPacket(LPCHARACTER ch, const TPacketCGTitleSystem* p);

	void OnCharacterLoad(LPCHARACTER ch);
	void OnCharacterDisconnect(LPCHARACTER ch);
	void Process(LPCHARACTER ch);

	bool GrantTitle(LPCHARACTER ch, DWORD dwTitleIndex, int iDurationSec, bool bAutoEquip);
	bool UseTitleItem(LPCHARACTER ch, LPITEM item);

	bool EquipTitle(LPCHARACTER ch, DWORD dwTitleIndex);
	bool UnequipTitle(LPCHARACTER ch, bool bSystemAction = false);
	void DeletePlayerTitle(LPCHARACTER ch, DWORD dwTitleIndex);

	DWORD GetEquippedTitle(LPCHARACTER ch) const;
	bool IsTitleOwned(LPCHARACTER ch, DWORD dwTitleIndex) const;
	bool IsTitleExpired(LPCHARACTER ch, DWORD dwTitleIndex) const;

	void BroadcastEquippedTitle(LPCHARACTER ch);
	void ClearEquippedTitleVisual(LPCHARACTER ch);

	const STitleProto* FindProto(DWORD dwTitleIndex) const;
	const STitleProto* FindProtoByName(const std::string& name) const;
	const STitleProto* FindProtoByResource(WORD wResourceIndex) const;
	const char* GetEffectPath(DWORD dwTitleIndex) const;

private:
	bool LoadProtoTable();
	void LoadPlayerTitles(LPCHARACTER ch);
	void SavePlayerTitle(LPCHARACTER ch, DWORD dwTitleIndex, DWORD dwEndTime);
	
	void SaveEquippedTitle(LPCHARACTER ch);

	void SendOpen(LPCHARACTER ch);
	void SendPlayerTitles(LPCHARACTER ch);
	void SendNotify(LPCHARACTER ch, BYTE bMessage, DWORD dwTitleIndex);
	void SendEquipped(LPCHARACTER ch, DWORD dwTitleIndex);

	void CheckExpiredTitles(LPCHARACTER ch, bool bNotify);
	void CheckOneHourWarning(LPCHARACTER ch);

	std::map<DWORD, STitleProto> m_mapProto;
	std::vector<DWORD> m_vecProtoOrder;
};

#endif
