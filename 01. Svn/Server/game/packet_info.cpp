// Find this line:
Set(HEADER_CG_PASSIVE_ATTR, sizeof(TPacketCGPassiveAttr), "PassiveAttr", false);

// Add after it:
#if defined(__TITLE_SYSTEM__)
	Set(HEADER_CG_TITLE_SYSTEM, sizeof(TPacketCGTitleSystem), "TitleSystem", false);
#endif
