// Add the following `CEffectInstance::SetParticleScale` function anywhere in this file:
#if defined(ENABLE_TITLE_SYSTEM)
void CEffectInstance::SetParticleScale(float fParticleScale)
{
	const float fOldParticleScale = m_fParticleScale;
	m_fParticleScale = fParticleScale;
	for (size_t i = 0; i < m_ParticleInstanceVector.size(); ++i)
	{
		m_ParticleInstanceVector[i]->RefreshActiveParticleScale(fOldParticleScale, fParticleScale);
		m_ParticleInstanceVector[i]->SetParticleScale(fParticleScale);
	}
}
#else
void CEffectInstance::SetParticleScale(float fParticleScale)
{
	m_fParticleScale = fParticleScale;
	for (size_t i = 0; i < m_ParticleInstanceVector.size(); ++i)
		m_ParticleInstanceVector[i]->SetParticleScale(fParticleScale);
}
#endif
