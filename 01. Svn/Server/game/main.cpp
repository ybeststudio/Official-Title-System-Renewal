// Add to includes:
#if defined(__TITLE_SYSTEM__)
#	include "title_system.h"
#endif

// Find this line:
CPassiveAttrManager passive_attr_manager;

// Add after it:
#if defined(__TITLE_SYSTEM__)
	CTitleSystemManager title_system_manager;
#endif

// Find this line:
TreasureHuntManager.Initialize();

// Add after it:
#if defined(__TITLE_SYSTEM__)
	CTitleSystemManager::instance().Initialize();
#endif
