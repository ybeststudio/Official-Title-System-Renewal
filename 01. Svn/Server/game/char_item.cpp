// Add to includes:
#if defined(__TITLE_SYSTEM__)
#	include "title_system.h"
#endif

// In `bool CHARACTER::UseItemEx(LPITEM item, TItemPos DestCell)`, extend the switch statement with:
#if defined(__TITLE_SYSTEM__)
				case USE_TITLE:
					CTitleSystemManager::instance().UseTitleItem(this, item);
					break;
#endif
