// Add the following packet definitions in the related packet section:
#if defined(ENABLE_TITLE_SYSTEM)
	HEADER_CG_TITLE_SYSTEM = 185,
#endif

// Add the following packet definitions in the related packet section:
#if defined(ENABLE_TITLE_SYSTEM)
	HEADER_GC_TITLE_SYSTEM = 189,
#endif

// Add the following packet definitions in the related packet section:
#if defined(ENABLE_TITLE_SYSTEM)
typedef struct SPacketCGTitleSystem
{
	BYTE bHeader;
	BYTE bSubHeader;
	DWORD dwTitleIndex;
} TPacketCGTitleSystem;

typedef struct SPacketGCTitleSystem
{
	BYTE bHeader;
	WORD wSize;
	BYTE bSubHeader;
	WORD wCount;
} TPacketGCTitleSystem;

typedef struct SPacketGCTitleSystemTableRow
{
	DWORD dwTitleIndex;
	BYTE bTitleType;
	char szName[32];
	char szCondition[128];
	BYTE bIsPermanent;
	DWORD dwOpenTime;
	int iFontColor;
	WORD wResourceIndex;
	char szEffectPath[128];
	char szImagePath[128];
} TPacketGCTitleSystemTableRow;

typedef struct SPacketGCTitleSystemPlayerRow
{
	DWORD dwTitleIndex;
	DWORD dwEndTime;
	BYTE bIsObtain;
	BYTE bIsEquip;
} TPacketGCTitleSystemPlayerRow;

typedef struct SPacketGCTitleSystemEquipRow
{
	DWORD dwTitleIndex;
} TPacketGCTitleSystemEquipRow;

typedef struct SPacketGCTitleSystemNotifyRow
{
	BYTE bMessage;
	DWORD dwTitleIndex;
} TPacketGCTitleSystemNotifyRow;
#endif
