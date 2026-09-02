// Find this line:
rkUIMgr.Update();

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM) && defined(RENDER_TARGET)
	m_pyTitleSystem.RenderPreviewFrame();
#endif

// Find this line:
rkUIMgr.Render();

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM) && defined(RENDER_TARGET)
	m_pyTitleSystem.RenderPreviewEffectOverlay();
#endif
