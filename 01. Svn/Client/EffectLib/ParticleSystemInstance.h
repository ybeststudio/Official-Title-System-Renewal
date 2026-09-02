// In the CParticleSystemInstance class, find this line:
	static void Delete(CParticleSystemInstance* pkData);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	static void SetForceRenderWithoutFrustum(bool bFlag);
#endif

// In the template method `ForEachParticleRendering`, find this block:
			TParticleInstanceList::iterator itor = m_ParticleInstanceListVector[dwFrameIndex].begin();
			for (; itor != m_ParticleInstanceListVector[dwFrameIndex].end(); ++itor)
			{
				if (!InFrustum(*itor))
					return;
				FunObj(*itor);
			}

// Replace this block with:
			TParticleInstanceList::iterator itor = m_ParticleInstanceListVector[dwFrameIndex].begin();
			for (; itor != m_ParticleInstanceListVector[dwFrameIndex].end(); ++itor)
			{
#if defined(ENABLE_TITLE_SYSTEM)
				if (!ms_isForceRenderWithoutFrustum && !InFrustum(*itor))
					continue;
#else
				if (!InFrustum(*itor))
					return;
#endif
				FunObj(*itor);
			}

// Find this line:
	void CreateParticles(float fElapsedTime);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	void RefreshActiveParticleScale(float fOldParticleScale, float fNewParticleScale);
#endif

// In the protected member section, find this line:
	CEmitterProperty* m_pEmitterProperty;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	static bool ms_isForceRenderWithoutFrustum;
#endif
