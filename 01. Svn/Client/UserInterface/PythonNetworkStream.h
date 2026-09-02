// Find this line:
bool RecvPassiveAttr();

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
public:
	bool SendTitleSystemPacket(BYTE bSubHeader, DWORD dwTitleIndex);
	bool RecvTitleSystem();
#endif
