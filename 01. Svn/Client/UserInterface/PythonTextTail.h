// Find this line:
CGraphicTextInstance* pTitleTextInstance;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
		CGraphicExpandedImageInstance* pTitleBgLeftInstance;
		CGraphicExpandedImageInstance* pTitleBgMiddleInstance;
		CGraphicExpandedImageInstance* pTitleBgRightInstance;
		CGraphicExpandedImageInstance* pTitleSpriteInstance;
		CGraphicTextInstance* pCustomTitleTextInstance;
		DWORD dwCustomTitleID;
		WORD wTitleSpriteResourceIndex;
		BYTE bTitleSpriteFrameCount;
		BYTE bTitleSpriteFrameIndex;
		int iTitleSpriteWidth;
		int iTitleSpriteHeight;
		DWORD dwTitleSpriteNextFrameTime;
#endif

// Find this line:
void DetachTitle(DWORD dwVID);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	void UpdateCustomTitle(TTextTail* pTextTail, DWORD dwTitleID);
	void ClearCustomTitleVisuals(TTextTail* pTextTail);
	void UpdateCustomTitleSpriteFrame(TTextTail* pTextTail, DWORD dwCurrentTime);
#endif
