// In `appYutnoriCreate(PyObject* poSelf, PyObject* poArgs)`, find this block:
{
	CPythonApplication::Instance().YutnoriCreate();
	return Py_BuildNone();
}

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
PyObject* appTitleUIBackImageCreate(PyObject* poSelf, PyObject* poArgs)
{
	CPythonApplication::Instance().TitleUIBackImageCreate();
	return Py_BuildNone();
}
#endif

// Find this line:
{ "YutnoriCreate", appYutnoriCreate, METH_VARARGS },

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
		{ "TitleUIBackImageCreate", appTitleUIBackImageCreate, METH_VARARGS },
#endif

// Find this line:
PyModule_AddIntConstant(poModule, "RENDER_TARGET_INDEX_YUTNORI", CRenderTargetManager::RENDER_TARGET_INDEX_YUTNORI);

// Add after it:
	#if defined(ENABLE_TITLE_SYSTEM)
	PyModule_AddIntConstant(poModule, "RENDER_TARGET_INDEX_TITLE", CRenderTargetManager::RENDER_TARGET_INDEX_TITLE);
	#endif

// Find this line:
PyModule_AddIntConstant(poModule, "ENABLE_MOUNT_UPGRADE_SYSTEM", 0);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	PyModule_AddIntConstant(poModule, "ENABLE_TITLE_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_TITLE_SYSTEM", 0);
#endif
