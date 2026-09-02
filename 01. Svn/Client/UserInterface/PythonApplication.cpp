// In `void CPythonApplication::RenderGame()`, find this line:
		CPythonMyShopDecoManager::Instance().RenderBackground();
#endif

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
		m_pyTitleSystem.RenderPreviewBackground();
#endif

// In the same function, find this line:
		CPythonYutnoriManager::Instance().RenderModel();
#endif

// Add after it (this must run right before m_kChrMgr.Render()):
#if defined(ENABLE_TITLE_SYSTEM)
		m_pyTitleSystem.RenderPreviewEffect();
#endif

// In `void CPythonApplication::UpdateGame()`, find this line:
	CPythonYutnoriManager::Instance().UpdateModel();

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	m_pyTitleSystem.UpdatePreview();
#endif

// In `bool CPythonApplication::Initialize(HINSTANCE hInstance)`, find this line:
	if (!CRenderTargetManager::Instance().CreateA8R8G8B8Texture(m_dwWidth, m_dwHeight, CRenderTargetManager::RENDER_TARGET_INDEX_YUTNORI))
		return false;
#endif

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	if (!CRenderTargetManager::Instance().CreateA8R8G8B8Texture(m_dwWidth, m_dwHeight, CRenderTargetManager::RENDER_TARGET_INDEX_TITLE))
		return false;
#endif

// Add the following `CPythonApplication::TitleUIBackImageCreate` function anywhere in this file:
#if defined(ENABLE_TITLE_SYSTEM)
void CPythonApplication::TitleUIBackImageCreate()
{
	m_pyTitleSystem.CreatePreviewBackground(m_dwWidth, m_dwHeight);
}
#endif
