// Find this line:
DWORD AttachEffectByID(DWORD dwParentPartIndex, const char* c_pszBoneName, DWORD dwEffectID, const D3DXVECTOR3* c_pv3Position = NULL, float fParticleScale = 1.0f, const D3DXVECTOR3* c_pv3MeshScale = NULL);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	void SetAttachingEffectTranslation(DWORD dwEffectIndex, const D3DXMATRIX& matTranslation);
#endif
