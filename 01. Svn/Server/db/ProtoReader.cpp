// Find this line:
"USE_SECRET_DUNGEON_SCROLL",

// Add after it:
#if defined(__TITLE_SYSTEM__)
		"USE_TITLE", 				// 46
#endif
