// Add to includes:
#if defined(__TITLE_SYSTEM__)
#	include "title_system.h"

void CInputMain::TitleSystem(LPCHARACTER ch, const char* c_pData)
{
	if (!ch)
		return;

	const TPacketCGTitleSystem* p = reinterpret_cast<const TPacketCGTitleSystem*>(c_pData);
	if (!p)
		return;

	CTitleSystemManager::instance().RecvCGPacket(ch, p);
}
#endif

// In `int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)`, extend the switch statement with:
#if defined(__TITLE_SYSTEM__)
		case HEADER_CG_TITLE_SYSTEM:
			TitleSystem(ch, c_pData);
			break;
#endif
