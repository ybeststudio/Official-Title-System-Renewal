// Find this line:
PyModule_AddIntConstant(poModule, "USE_SECRET_DUNGEON_SCROLL", CItemData::USE_SECRET_DUNGEON_SCROLL);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	PyModule_AddIntConstant(poModule, "USE_TITLE", CItemData::USE_TITLE);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_TITLE_ITEM_END_TIME", 0);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_TITLE_INDEX", 1);
#endif
