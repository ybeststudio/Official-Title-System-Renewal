// Find this line:
void PassiveAttr(LPCHARACTER ch, const char* c_pData);

// Add after it:
#if defined(__TITLE_SYSTEM__)
	void TitleSystem(LPCHARACTER ch, const char* c_pData);
#endif
