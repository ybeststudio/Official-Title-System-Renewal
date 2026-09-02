// Find this line:
CDynamicPool<CParticleSystemInstance> CParticleSystemInstance::ms_kPool;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
bool CParticleSystemInstance::ms_isForceRenderWithoutFrustum = false;
#endif

// Add the following `CParticleSystemInstance::SetForceRenderWithoutFrustum` function anywhere in this file:
#if defined(ENABLE_TITLE_SYSTEM)
void CParticleSystemInstance::SetForceRenderWithoutFrustum(bool bFlag)
{
	ms_isForceRenderWithoutFrustum = bFlag;
}
#endif

// Add the following `CParticleSystemInstance::RefreshActiveParticleScale` function anywhere in this file:
#if defined(ENABLE_TITLE_SYSTEM)
void CParticleSystemInstance::RefreshActiveParticleScale(float fOldParticleScale, float fNewParticleScale)
{
	if (fOldParticleScale <= 0.0f || fOldParticleScale == fNewParticleScale)
	{
		return;
	}

	const float fScaleRatio = fNewParticleScale / fOldParticleScale;
	for (DWORD dwFrameIndex = 0; dwFrameIndex < m_ParticleInstanceListVector.size(); ++dwFrameIndex)
	{
		TParticleInstanceList::iterator itor = m_ParticleInstanceListVector[dwFrameIndex].begin();
		for (; itor != m_ParticleInstanceListVector[dwFrameIndex].end(); ++itor)
		{
			CParticleInstance* pInstance = *itor;
			pInstance->m_v2HalfSize *= fScaleRatio;
		}
	}
}
#endif
