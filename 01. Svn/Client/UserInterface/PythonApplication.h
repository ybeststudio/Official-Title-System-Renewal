// Add to includes:
#if defined(ENABLE_TITLE_SYSTEM)
	#include "PythonTitleSystem.h"
#endif

// In the CPythonApplication class, find this line:
	void SetYutnoriCameraPosition();

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	void TitleUIBackImageCreate();
#endif

// In the member section, find this line:
	CPythonPassiveAttr m_pyPassiveAttr;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	CPythonTitleSystem m_pyTitleSystem;
#endif
