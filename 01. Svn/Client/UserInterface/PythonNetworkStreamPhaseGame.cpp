// In `void CPythonNetworkStream::GamePhase()`, extend the switch statement with:
#if defined(ENABLE_TITLE_SYSTEM)
			case HEADER_GC_TITLE_SYSTEM:
				ret = RecvTitleSystem();
				break;
#endif

// Add to includes:
#if defined(ENABLE_TITLE_SYSTEM)
#	include "PythonTitleSystem.h"

bool CPythonNetworkStream::SendTitleSystemPacket(BYTE bSubHeader, DWORD dwTitleIndex)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGTitleSystem pack = {};
	pack.bHeader = HEADER_CG_TITLE_SYSTEM;
	pack.bSubHeader = bSubHeader;
	pack.dwTitleIndex = dwTitleIndex;

	if (!Send(sizeof(pack), &pack))
	{
		Tracen("CPythonNetworkStream::SendTitleSystemPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvTitleSystem()
{
	TPacketGCTitleSystem pack = {};
	if (!Recv(sizeof(pack), &pack))
	{
		Tracen("RecvTitleSystem header Error");
		return false;
	}

	int iPayloadSize = 0;
	switch (pack.bSubHeader)
	{
		case CPythonTitleSystem::SUBHEADER_GC_TITLE_TABLE:
			iPayloadSize = static_cast<int>(pack.wCount) * static_cast<int>(sizeof(TPacketGCTitleSystemTableRow));
			break;
		case CPythonTitleSystem::SUBHEADER_GC_TITLE_PLAYER:
			iPayloadSize = static_cast<int>(pack.wCount) * static_cast<int>(sizeof(TPacketGCTitleSystemPlayerRow));
			break;
		case CPythonTitleSystem::SUBHEADER_GC_TITLE_EQUIPPED:
			iPayloadSize = static_cast<int>(sizeof(TPacketGCTitleSystemEquipRow));
			break;
		case CPythonTitleSystem::SUBHEADER_GC_TITLE_NOTIFY:
			iPayloadSize = static_cast<int>(sizeof(TPacketGCTitleSystemNotifyRow));
			break;
		case CPythonTitleSystem::SUBHEADER_GC_TITLE_END:
			iPayloadSize = 0;
			break;
		default:
			Tracenf("RecvTitleSystem unknown subheader %u", pack.bSubHeader);
			return false;
	}

	std::vector<char> vecPayload;
	if (iPayloadSize > 0)
	{
		vecPayload.resize(iPayloadSize);
		if (!Recv(iPayloadSize, &vecPayload[0]))
		{
			Tracen("RecvTitleSystem payload Error");
			return false;
		}
	}

	CPythonTitleSystem::Instance().RecvPacket(
		pack.bSubHeader,
		pack.wCount,
		vecPayload.empty() ? nullptr : &vecPayload[0],
		iPayloadSize);

	return true;
}
#endif
