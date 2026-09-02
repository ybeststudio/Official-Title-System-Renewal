// Add these file-scope helpers above CInstanceBase::Update, near the other static
// helpers at the top of this file:
#if defined(ENABLE_TITLE_SYSTEM)
static bool IsBannerCrownTitle(DWORD dwTitleID)
{
	switch (dwTitleID)
	{
		case 1005:
		case 1006:
		case 1011:
			return true;
		default:
			return false;
	}
}

static bool UseFixedOverHeadTitleTransform(DWORD dwTitleID)
{
	return IsBannerCrownTitle(dwTitleID);
}

static float GetBannerCrownTransformScale(float fScale)
{
	return 1.0f + ((fScale - 1.0f) * 0.35f);
}
#endif

// In `void CInstanceBase::Update()`, find this line:
	while (ProcessDamage());

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	if (m_dwOverHeadSpecialEffect)
	{
		CCamera* pCurrentCamera = CCameraManager::Instance().GetCurrentCamera();
		if (pCurrentCamera)
		{
			const D3DXVECTOR3& c_rv3Eye = pCurrentCamera->GetEye();
			const D3DXVECTOR3& c_rv3Pos = m_GraphicThingInstance.GetPosition();
			D3DXVECTOR3 v3Diff = c_rv3Pos - c_rv3Eye;
			float fCameraDistance = D3DXVec3Length(&v3Diff);
			if (fCameraDistance > 0.0f)
			{
				float fScale = fCameraDistance / 1500.0f;
				if (fScale < 0.2f) fScale = 0.2f;
				if (fScale > 8.0f) fScale = 8.0f;

				CEffectManager& rkEftMgr = CEffectManager::Instance();
				const DWORD dwEquippedTitle = GetEquippedTitle();
				const bool bUseFixedTransform = UseFixedOverHeadTitleTransform(dwEquippedTitle);
				const float fTransformScale = bUseFixedTransform ? GetBannerCrownTransformScale(fScale) : fScale;
				const float fEffectScale = (fTransformScale > 0.0f) ? (fScale / fTransformScale) : 1.0f;
				if (rkEftMgr.SelectEffectInstance(m_dwOverHeadSpecialEffect))
				{
					if (bUseFixedTransform)
					{
						rkEftMgr.SetEffectInstanceParticleScale(fEffectScale);
						rkEftMgr.SetEffectInstanceMeshScale(D3DXVECTOR3(fEffectScale, fEffectScale, fEffectScale));
					}
					else
					{
						rkEftMgr.SetEffectInstanceParticleScale(1.0f);
						rkEftMgr.SetEffectInstanceMeshScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
					}
				}

				float fBaseOffset = 60.0f;
				if (dwEquippedTitle >= 1 && dwEquippedTitle <= 4)
				{
					fBaseOffset = -50.0f;
				}

				D3DXMATRIX matTranslation;
				if (bUseFixedTransform)
				{
					D3DXMatrixScaling(&matTranslation, fTransformScale, fTransformScale, fTransformScale);
					matTranslation._43 = GetBaseHeight() + m_GraphicThingInstance.GetHeight() + 10.0f + (fBaseOffset * fScale);
				}
				else
				{
					D3DXMatrixScaling(&matTranslation, fScale, fScale, fScale);
					matTranslation._43 = GetBaseHeight() + m_GraphicThingInstance.GetHeight() + 10.0f + (fBaseOffset * fScale);
				}
				m_GraphicThingInstance.SetAttachingEffectTranslation(m_dwOverHeadSpecialEffect, matTranslation);
			}
		}
	}
#endif

// In `void CInstanceBase::__Initialize()`, find this line:
	m_specialRouletteEffect = 0;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	m_dwOverHeadSpecialEffect = 0;
	m_dwEquippedTitle = 0;
#endif
