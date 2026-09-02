// In `void CEffectManager::Render()`, find this block:
	STATEMANAGER.SetTexture(0, NULL);
	STATEMANAGER.SetTexture(1, NULL);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	// Title UI preview uses a dedicated instance id (see PythonTitleSystem.cpp).
	const DWORD TITLE_UI_PREVIEW_EFFECT_INSTANCE = 0x5449544C;
#endif

// In the same function the two render loops must skip that instance.

// Before
	if (m_isDisableSortRendering)
	{
		for (TEffectInstanceMap::iterator itor = m_kEftInstMap.begin(); itor != m_kEftInstMap.end();)
		{
			CEffectInstance* pEffectInstance = itor->second;
#if defined(ENABLE_GRAPHIC_ON_OFF)
			if (CPythonGraphicOnOff::Instance().CanRenderDropItem(1) || pEffectInstance->GetID() != GetDropEffectID())
			{
				if (pEffectInstance->CanRender())
					pEffectInstance->Render();
			}
#else
			pEffectInstance->Render();
#endif
			++itor;
		}
	}
	else
	{
		static std::vector<CEffectInstance*> s_kVct_pkEftInstSort;
		s_kVct_pkEftInstSort.clear();

		TEffectInstanceMap& rkMap_pkEftInstSrc = m_kEftInstMap;
		TEffectInstanceMap::iterator i;
#if defined(ENABLE_GRAPHIC_ON_OFF)
		for (i = rkMap_pkEftInstSrc.begin(); i != rkMap_pkEftInstSrc.end(); ++i)
		{
			if (CPythonGraphicOnOff::Instance().CanRenderDropItem(1) || i->second->GetID() != GetDropEffectID())
				s_kVct_pkEftInstSort.push_back(i->second);
		}
#else
		for (i = rkMap_pkEftInstSrc.begin(); i != rkMap_pkEftInstSrc.end(); ++i)
			s_kVct_pkEftInstSort.push_back(i->second);
#endif

		std::sort(s_kVct_pkEftInstSort.begin(), s_kVct_pkEftInstSort.end(), CEffectManager_LessEffectInstancePtrRenderOrder());
		std::for_each(s_kVct_pkEftInstSort.begin(), s_kVct_pkEftInstSort.end(), CEffectManager_FEffectInstanceRender());
	}

// After
	if (m_isDisableSortRendering)
	{
		for (TEffectInstanceMap::iterator itor = m_kEftInstMap.begin(); itor != m_kEftInstMap.end();)
		{
#if defined(ENABLE_TITLE_SYSTEM)
			if (itor->first == TITLE_UI_PREVIEW_EFFECT_INSTANCE)
			{
				++itor;
				continue;
			}
#endif
			CEffectInstance* pEffectInstance = itor->second;
#if defined(ENABLE_GRAPHIC_ON_OFF)
			if (CPythonGraphicOnOff::Instance().CanRenderDropItem(1) || pEffectInstance->GetID() != GetDropEffectID())
			{
				if (pEffectInstance->CanRender())
					pEffectInstance->Render();
			}
#else
			pEffectInstance->Render();
#endif
			++itor;
		}
	}
	else
	{
		static std::vector<CEffectInstance*> s_kVct_pkEftInstSort;
		s_kVct_pkEftInstSort.clear();

		TEffectInstanceMap& rkMap_pkEftInstSrc = m_kEftInstMap;
		TEffectInstanceMap::iterator i;
#if defined(ENABLE_GRAPHIC_ON_OFF)
		for (i = rkMap_pkEftInstSrc.begin(); i != rkMap_pkEftInstSrc.end(); ++i)
		{
#if defined(ENABLE_TITLE_SYSTEM)
			if (i->first == TITLE_UI_PREVIEW_EFFECT_INSTANCE)
				continue;
#endif
			if (CPythonGraphicOnOff::Instance().CanRenderDropItem(1) || i->second->GetID() != GetDropEffectID())
				s_kVct_pkEftInstSort.push_back(i->second);
		}
#else
		for (i = rkMap_pkEftInstSrc.begin(); i != rkMap_pkEftInstSrc.end(); ++i)
#if defined(ENABLE_TITLE_SYSTEM)
		{
			if (i->first == TITLE_UI_PREVIEW_EFFECT_INSTANCE)
				continue;
			s_kVct_pkEftInstSort.push_back(i->second);
		}
#endif
#endif

		std::sort(s_kVct_pkEftInstSort.begin(), s_kVct_pkEftInstSort.end(), CEffectManager_LessEffectInstancePtrRenderOrder());
		std::for_each(s_kVct_pkEftInstSort.begin(), s_kVct_pkEftInstSort.end(), CEffectManager_FEffectInstanceRender());
	}
