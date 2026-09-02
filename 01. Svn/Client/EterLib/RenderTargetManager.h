// Find this line:
RENDER_TARGET_INDEX_YUTNORI,

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
		RENDER_TARGET_INDEX_TITLE = 4,
#endif
