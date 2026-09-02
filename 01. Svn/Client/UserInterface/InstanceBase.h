// Find this line:
void AttachSpecialEffect(DWORD effect, float fScale = 1.0f);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	void AttachSpecialEffectOverHead(DWORD effect);
	void ClearSpecialEffectOverHead();
	void SetEquippedTitle(DWORD dwTitleID) { m_dwEquippedTitle = dwTitleID; }
	DWORD GetEquippedTitle() const { return m_dwEquippedTitle; }
#endif

// Find this line:
DWORD m_specialRouletteEffect;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	DWORD m_dwOverHeadSpecialEffect;
	DWORD m_dwEquippedTitle;
#endif
