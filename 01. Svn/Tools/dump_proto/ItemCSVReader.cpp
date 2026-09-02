// DumpProto keeps its own copy of the server feature flags.
// Add near the other feature flags at the top of this file:
#define __TITLE_SYSTEM__

// Find this line:
"USE_SECRET_DUNGEON_SCROLL",

// Add after it:
#if defined(__TITLE_SYSTEM__)
		"USE_TITLE", 				// 46
#endif
