// Add the following `CActorInstance::SetAttachingEffectTranslation` function anywhere in this file:
#if defined(ENABLE_TITLE_SYSTEM)
void CActorInstance::SetAttachingEffectTranslation(DWORD dwEffectIndex, const D3DXMATRIX& matTranslation)
{
	for (auto it = m_AttachingEffectList.begin(); it != m_AttachingEffectList.end(); ++it)
	{
		if (it->dwEffectIndex == dwEffectIndex)
		{
			it->matTranslation = matTranslation;
			break;
		}
	}
}
#endif
