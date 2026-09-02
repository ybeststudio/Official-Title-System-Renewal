// Find this line:
void initpassiveAttr();

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
void initTitleSystem();
#endif
