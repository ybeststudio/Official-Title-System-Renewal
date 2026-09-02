// Add to includes:
#if defined(ENABLE_TITLE_SYSTEM)
#include "PythonTitleSystem.h"
#endif

// Add this anonymous namespace at file scope, below the existing
// c_fyGuildNamePosition / colour constants:
#if defined(ENABLE_TITLE_SYSTEM)
namespace
{
	// Preview window uses AniImageBox.SetDelay(10), which advances on UI update ticks.
	// The overhead title sprite uses millisecond timing, so keep a slower cadence close to that visual tempo.
	constexpr DWORD TITLE_SPRITE_FRAME_DELAY_MS = 160;
	constexpr float TITLE_CUSTOM_DEFAULT_OFFSET = 28.0f;
	constexpr float TITLE_CUSTOM_STACK_OFFSET = 8.0f;
	constexpr float TITLE_CUSTOM_GUILD_SPACING = c_fyGuildNamePosition;
	constexpr int TITLE_BANNER_TEXT_PADDING_X = 0;
	constexpr float TITLE_SPRITE_BANNER_OFFSET_Y = -3.0f;

	constexpr float GetCustomTitleVisualAnchorY(float fTextTailY, bool bHasGuildName, bool bHasPlayerTitle, float fGuildNamePosition, float fDefaultOffset, float fVisualHalfHeight, float fAdditionalGuildSpacing)
	{
		float fCustomTitleY = fTextTailY - fDefaultOffset;
		if (bHasGuildName)
		{
			fCustomTitleY = fTextTailY - fGuildNamePosition - fAdditionalGuildSpacing - fVisualHalfHeight;
		}
		if (bHasPlayerTitle)
		{
			fCustomTitleY += TITLE_CUSTOM_STACK_OFFSET;
		}

		return fCustomTitleY;
	}

	static_assert(TITLE_SPRITE_FRAME_DELAY_MS == 160, "Overhead title sprite animation should match the slower official preview cadence.");
	static_assert(TITLE_BANNER_TEXT_PADDING_X == 0, "Banner width should use zero extra horizontal padding.");
	static_assert(TITLE_SPRITE_BANNER_OFFSET_Y == -3.0f, "Title sprite should sit slightly lower on the banner.");
	static_assert(GetCustomTitleVisualAnchorY(100.0f, true, false, c_fyGuildNamePosition, TITLE_CUSTOM_DEFAULT_OFFSET, 8.0f, TITLE_CUSTOM_GUILD_SPACING) == 62.0f,
		"Guild titles should keep a guild-sized gap above the banner block.");

	bool IsScreenSpaceBannerTitle(DWORD dwTitleID)
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
}
#endif

// Find this line:
DWORD dwCurrentTime = 0;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	dwCurrentTime = CTimer::Instance().GetCurrentMillisecond();
#endif

// Before
	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;
#if defined(ENABLE_GRAPHIC_ON_OFF)
		CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(pTextTail->dwVirtualID);
		if (!pInstance)
			continue;

		if (!pInstance->CanRenderActor())
			continue;
#endif
		UpdateTextTail(pTextTail);


		TChatTailMap::iterator itor = m_ChatTailMap.find(pTextTail->dwVirtualID);
		if (m_ChatTailMap.end() != itor)
		{
			TTextTail* pChatTail = itor->second;
			if (pChatTail->bNameFlag)
			{
				pTextTail->y = pChatTail->y - 17.0f;
			}
		}
	}

// After
	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;
#if defined(ENABLE_GRAPHIC_ON_OFF)
		CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(pTextTail->dwVirtualID);
		if (!pInstance)
			continue;

		if (!pInstance->CanRenderActor())
			continue;
#endif
		UpdateTextTail(pTextTail);

#if defined(ENABLE_TITLE_SYSTEM)
		if (pInstance)
		{
			DWORD dwTitleID = pInstance->GetEquippedTitle();
			if (dwTitleID != pTextTail->dwCustomTitleID)
			{
				UpdateCustomTitle(pTextTail, dwTitleID);
			}
		}
		UpdateCustomTitleSpriteFrame(pTextTail, dwCurrentTime);
#endif

		TChatTailMap::iterator itor = m_ChatTailMap.find(pTextTail->dwVirtualID);
		if (m_ChatTailMap.end() != itor)
		{
			TTextTail* pChatTail = itor->second;
			if (pChatTail->bNameFlag)
			{
				pTextTail->y = pChatTail->y - 17.0f;
			}
		}
	}

// Find this line:
pCountryFlagImageInstance->SetPosition(pTextTail->x - (iNameWidth / 2) - iLevelWidth - pCountryFlagImageInstance->GetWidth() - 8.0f, pTextTail->y - 10.0f);

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
		if (pTextTail->dwCustomTitleID > 0 && pTextTail->pCustomTitleTextInstance)
		{
			int iTitleWidth = 0, iTitleHeight = 0;
			pTextTail->pCustomTitleTextInstance->GetTextSize(&iTitleWidth, &iTitleHeight);

			float fCustomTitleVisualHalfHeight = iTitleHeight / 2.0f;
			if (pTextTail->pTitleBgLeftInstance)
			{
				fCustomTitleVisualHalfHeight = pTextTail->pTitleBgLeftInstance->GetHeight() / 2.0f;
			}

			const float fCustomTitleY = GetCustomTitleVisualAnchorY(pTextTail->y,
				NULL != pTextTail->pGuildNameTextInstance,
				NULL != pTextTail->pTitleTextInstance,
				c_fyGuildNamePosition,
				TITLE_CUSTOM_DEFAULT_OFFSET,
				fCustomTitleVisualHalfHeight,
				TITLE_CUSTOM_GUILD_SPACING);

			if (pTextTail->pTitleBgLeftInstance && pTextTail->pTitleBgMiddleInstance && pTextTail->pTitleBgRightInstance)
			{
				int lw = pTextTail->pTitleBgLeftInstance->GetWidth();
				int lh = pTextTail->pTitleBgLeftInstance->GetHeight();
				int rw = pTextTail->pTitleBgRightInstance->GetWidth();

				int iMiddleW = iTitleWidth + TITLE_BANNER_TEXT_PADDING_X;

				float fImgY = fCustomTitleY - (lh / 2.0f);

				pTextTail->pTitleBgLeftInstance->SetPosition(pTextTail->x - (iMiddleW / 2.0f) - lw, fImgY);
				pTextTail->pTitleBgMiddleInstance->SetPosition(pTextTail->x - (iMiddleW / 2.0f), fImgY);
				pTextTail->pTitleBgRightInstance->SetPosition(pTextTail->x + (iMiddleW / 2.0f), fImgY);

				float fMiddleScaleX = (float)iMiddleW / (float)pTextTail->pTitleBgMiddleInstance->GetWidth();
				pTextTail->pTitleBgMiddleInstance->SetScale(fMiddleScaleX, 1.0f);

			}

			if (pTextTail->pTitleSpriteInstance)
			{
				int iSpriteWidth = pTextTail->iTitleSpriteWidth > 0 ? pTextTail->iTitleSpriteWidth : pTextTail->pTitleSpriteInstance->GetWidth();
				int iSpriteHeight = pTextTail->iTitleSpriteHeight > 0 ? pTextTail->iTitleSpriteHeight : pTextTail->pTitleSpriteInstance->GetHeight();
				float fSpriteY = fCustomTitleY - iSpriteHeight + 12.0f;
				if (pTextTail->pTitleBgLeftInstance)
				{
					const int iBannerHeight = pTextTail->pTitleBgLeftInstance->GetHeight();
					const float fBannerY = fCustomTitleY - (iBannerHeight / 2.0f);
					fSpriteY = fBannerY + iBannerHeight - iSpriteHeight + TITLE_SPRITE_BANNER_OFFSET_Y;
				}

				pTextTail->pTitleSpriteInstance->SetPosition(pTextTail->x - (iSpriteWidth / 2.0f), fSpriteY);
				pTextTail->pTitleSpriteInstance->SetScale(1.0f, 1.0f);
			}



			pTextTail->pCustomTitleTextInstance->SetPosition(pTextTail->x, fCustomTitleY, pTextTail->z);
			pTextTail->pCustomTitleTextInstance->Update();
		}
#endif

// Before
	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;
#if defined(ENABLE_GRAPHIC_ON_OFF)
		CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(pTextTail->dwVirtualID);
		if (!pInstance)
			continue;

		if (!pInstance->CanRenderActor())
			continue;

		if (pInstance->IsNPC() && !CPythonGraphicOnOff::Instance().CanRenderNPCName())
		{
			DWORD dwPickedActorID;
			if (!CPythonPlayer::Instance().GetPickedActorID(&dwPickedActorID) || dwPickedActorID != pTextTail->dwVirtualID)
				continue;
		}
#endif
		pTextTail->pTextInstance->Render();


		if (pTextTail->pMarkInstance && pTextTail->pGuildNameTextInstance)
		{
			pTextTail->pMarkInstance->Render();
			pTextTail->pGuildNameTextInstance->Render();
		}

		if (pTextTail->pTitleTextInstance)
			pTextTail->pTitleTextInstance->Render();

#if defined(WJ_SHOW_MOB_INFO)
		if (pTextTail->pLevelTextInstance && (pTextTail->bIsPC == TRUE || CPythonSystem::Instance().IsShowMobLevel()))
#else
		if (pTextTail->pLevelTextInstance)
#endif
			pTextTail->pLevelTextInstance->Render();

#if defined(WJ_SHOW_MOB_INFO)
		if (pTextTail->pAIFlagTextInstance && CPythonSystem::Instance().IsShowMobAIFlag())
			pTextTail->pAIFlagTextInstance->Render();
#endif

#if defined(ENABLE_MULTI_LANGUAGE_SYSTEM)
		if (pTextTail->pCountryFlagImageInstance && CPythonSystem::Instance().IsAlwaysShowCountry())
			pTextTail->pCountryFlagImageInstance->Render();
#endif

#if defined(ENABLE_LEFT_SEAT)
		if (pTextTail->pLeftSeatTextInstance)
			pTextTail->pLeftSeatTextInstance->Render();
#endif
	}

// After
	for (itor = m_CharacterTextTailList.begin(); itor != m_CharacterTextTailList.end(); ++itor)
	{
		TTextTail* pTextTail = *itor;
#if defined(ENABLE_GRAPHIC_ON_OFF)
		CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(pTextTail->dwVirtualID);
		if (!pInstance)
			continue;

		if (!pInstance->CanRenderActor())
			continue;

		if (pInstance->IsNPC() && !CPythonGraphicOnOff::Instance().CanRenderNPCName())
		{
			DWORD dwPickedActorID;
			if (!CPythonPlayer::Instance().GetPickedActorID(&dwPickedActorID) || dwPickedActorID != pTextTail->dwVirtualID)
				continue;
		}
#endif
		pTextTail->pTextInstance->Render();

#if defined(ENABLE_TITLE_SYSTEM)
		if (pTextTail->pTitleSpriteInstance)
		{
			pTextTail->pTitleSpriteInstance->Render();
		}

		if (pTextTail->pTitleBgLeftInstance && pTextTail->pTitleBgMiddleInstance && pTextTail->pTitleBgRightInstance)
		{
			pTextTail->pTitleBgLeftInstance->Render();
			pTextTail->pTitleBgMiddleInstance->Render();
			pTextTail->pTitleBgRightInstance->Render();
		}

		if (pTextTail->pCustomTitleTextInstance)
		{
			pTextTail->pCustomTitleTextInstance->Render();
		}
#endif

		if (pTextTail->pMarkInstance && pTextTail->pGuildNameTextInstance)
		{
			pTextTail->pMarkInstance->Render();
			pTextTail->pGuildNameTextInstance->Render();
		}

		if (pTextTail->pTitleTextInstance)
			pTextTail->pTitleTextInstance->Render();

#if defined(WJ_SHOW_MOB_INFO)
		if (pTextTail->pLevelTextInstance && (pTextTail->bIsPC == TRUE || CPythonSystem::Instance().IsShowMobLevel()))
#else
		if (pTextTail->pLevelTextInstance)
#endif
			pTextTail->pLevelTextInstance->Render();

#if defined(WJ_SHOW_MOB_INFO)
		if (pTextTail->pAIFlagTextInstance && CPythonSystem::Instance().IsShowMobAIFlag())
			pTextTail->pAIFlagTextInstance->Render();
#endif

#if defined(ENABLE_MULTI_LANGUAGE_SYSTEM)
		if (pTextTail->pCountryFlagImageInstance && CPythonSystem::Instance().IsAlwaysShowCountry())
			pTextTail->pCountryFlagImageInstance->Render();
#endif

#if defined(ENABLE_LEFT_SEAT)
		if (pTextTail->pLeftSeatTextInstance)
			pTextTail->pLeftSeatTextInstance->Render();
#endif
	}

// Before
	if (!pCharacterInstance)
		return;

	TTextTail* pTextTail = RegisterTextTail(dwVirtualID,
		pCharacterInstance->GetNameString(),
		pCharacterInstance->GetGraphicThingInstancePtr(),
		pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + fAddHeight, c_rColor);

	CGraphicTextInstance* pTextInstance = pTextTail->pTextInstance;
	pTextInstance->SetOutline(true);
	pTextInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);

	pTextTail->pMarkInstance = NULL;
	pTextTail->pGuildNameTextInstance = NULL;
	pTextTail->pTitleTextInstance = NULL;
	pTextTail->pLevelTextInstance = NULL;
#if defined(WJ_SHOW_MOB_INFO)
	pTextTail->pAIFlagTextInstance = NULL;
#endif
#if defined(ENABLE_MULTI_LANGUAGE_SYSTEM)
	pTextTail->pCountryFlagImageInstance = NULL;
#endif
#if defined(ENABLE_LEFT_SEAT)
	pTextTail->pLeftSeatTextInstance = NULL;
#endif

	if (0 != dwGuildID)
	{
		pTextTail->pMarkInstance = CGraphicMarkInstance::New();

		DWORD dwMarkID = CGuildMarkManager::Instance().GetMarkID(dwGuildID);

		if (dwMarkID != CGuildMarkManager::INVALID_MARK_ID)
		{
			std::string markImagePath;

			if (CGuildMarkManager::Instance().GetMarkImageFilename(dwMarkID / CGuildMarkImage::MARK_TOTAL_COUNT, markImagePath))
			{
				pTextTail->pMarkInstance->SetImageFileName(markImagePath.c_str());
				pTextTail->pMarkInstance->Load();
				pTextTail->pMarkInstance->SetIndex(dwMarkID % CGuildMarkImage::MARK_TOTAL_COUNT);
			}
		}

		std::string strGuildName;
		if (!CPythonGuild::Instance().GetGuildName(dwGuildID, &strGuildName))
			strGuildName = "Noname";

#if defined(ENABLE_GUILD_LEADER_GRADE_NAME)
		strGuildName.insert(0, c_szGuildLeaderGradeName);
#endif

		CGraphicTextInstance*& prGuildNameInstance = pTextTail->pGuildNameTextInstance;
		prGuildNameInstance = CGraphicTextInstance::New();
		prGuildNameInstance->SetTextPointer(ms_pFont);
		prGuildNameInstance->SetOutline(true);
		prGuildNameInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
		prGuildNameInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
		prGuildNameInstance->SetValue(strGuildName.c_str());
		prGuildNameInstance->SetColor(c_TextTail_Guild_Name_Color.r, c_TextTail_Guild_Name_Color.g, c_TextTail_Guild_Name_Color.b);
		prGuildNameInstance->Update();
	}

// After
	if (!pCharacterInstance)
		return;

	TTextTail* pTextTail = RegisterTextTail(dwVirtualID,
		pCharacterInstance->GetNameString(),
		pCharacterInstance->GetGraphicThingInstancePtr(),
		pCharacterInstance->GetGraphicThingInstanceRef().GetHeight() + fAddHeight, c_rColor);

	CGraphicTextInstance* pTextInstance = pTextTail->pTextInstance;
	pTextInstance->SetOutline(true);
	pTextInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);

	pTextTail->pMarkInstance = NULL;
	pTextTail->pGuildNameTextInstance = NULL;
	pTextTail->pTitleTextInstance = NULL;
#if defined(ENABLE_TITLE_SYSTEM)
	pTextTail->pTitleBgLeftInstance = NULL;
	pTextTail->pTitleBgMiddleInstance = NULL;
	pTextTail->pTitleBgRightInstance = NULL;
	pTextTail->pTitleSpriteInstance = NULL;
	pTextTail->pCustomTitleTextInstance = NULL;
	pTextTail->dwCustomTitleID = 0;
	pTextTail->wTitleSpriteResourceIndex = 0;
	pTextTail->bTitleSpriteFrameCount = 0;
	pTextTail->bTitleSpriteFrameIndex = 0;
	pTextTail->iTitleSpriteWidth = 0;
	pTextTail->iTitleSpriteHeight = 0;
	pTextTail->dwTitleSpriteNextFrameTime = 0;
#endif
	pTextTail->pLevelTextInstance = NULL;
#if defined(WJ_SHOW_MOB_INFO)
	pTextTail->pAIFlagTextInstance = NULL;
#endif
#if defined(ENABLE_MULTI_LANGUAGE_SYSTEM)
	pTextTail->pCountryFlagImageInstance = NULL;
#endif
#if defined(ENABLE_LEFT_SEAT)
	pTextTail->pLeftSeatTextInstance = NULL;
#endif

	if (0 != dwGuildID)
	{
		pTextTail->pMarkInstance = CGraphicMarkInstance::New();

		DWORD dwMarkID = CGuildMarkManager::Instance().GetMarkID(dwGuildID);

		if (dwMarkID != CGuildMarkManager::INVALID_MARK_ID)
		{
			std::string markImagePath;

			if (CGuildMarkManager::Instance().GetMarkImageFilename(dwMarkID / CGuildMarkImage::MARK_TOTAL_COUNT, markImagePath))
			{
				pTextTail->pMarkInstance->SetImageFileName(markImagePath.c_str());
				pTextTail->pMarkInstance->Load();
				pTextTail->pMarkInstance->SetIndex(dwMarkID % CGuildMarkImage::MARK_TOTAL_COUNT);
			}
		}

		std::string strGuildName;
		if (!CPythonGuild::Instance().GetGuildName(dwGuildID, &strGuildName))
			strGuildName = "Noname";

#if defined(ENABLE_GUILD_LEADER_GRADE_NAME)
		strGuildName.insert(0, c_szGuildLeaderGradeName);
#endif

		CGraphicTextInstance*& prGuildNameInstance = pTextTail->pGuildNameTextInstance;
		prGuildNameInstance = CGraphicTextInstance::New();
		prGuildNameInstance->SetTextPointer(ms_pFont);
		prGuildNameInstance->SetOutline(true);
		prGuildNameInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
		prGuildNameInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_BOTTOM);
		prGuildNameInstance->SetValue(strGuildName.c_str());
		prGuildNameInstance->SetColor(c_TextTail_Guild_Name_Color.r, c_TextTail_Guild_Name_Color.g, c_TextTail_Guild_Name_Color.b);
		prGuildNameInstance->Update();
	}

// In `void CPythonTextTail::InitializeTextTail(TTextTail* pTextTail)`, find this block:
pTextTail->pTitleTextInstance = NULL;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	pTextTail->pTitleBgLeftInstance = NULL;
	pTextTail->pTitleBgMiddleInstance = NULL;
	pTextTail->pTitleBgRightInstance = NULL;
	pTextTail->pTitleSpriteInstance = NULL;
	pTextTail->pCustomTitleTextInstance = NULL;
	pTextTail->dwCustomTitleID = 0;
	pTextTail->wTitleSpriteResourceIndex = 0;
	pTextTail->bTitleSpriteFrameCount = 0;
	pTextTail->bTitleSpriteFrameIndex = 0;
	pTextTail->iTitleSpriteWidth = 0;
	pTextTail->iTitleSpriteHeight = 0;
	pTextTail->dwTitleSpriteNextFrameTime = 0;
#endif

// In `CPythonTextTail::TTextTail* CPythonTextTail::RegisterTextTail(DWORD dwVirtualID, const char* c_szText, CGraphicObjectInstance* pOwner, float fHeight, const D3DXCOLOR& c_rColor)`, find this block:
pTextTail->pTitleTextInstance = NULL;

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	pTextTail->pTitleBgLeftInstance = NULL;
	pTextTail->pTitleBgMiddleInstance = NULL;
	pTextTail->pTitleBgRightInstance = NULL;
	pTextTail->pTitleSpriteInstance = NULL;
	pTextTail->pCustomTitleTextInstance = NULL;
	pTextTail->dwCustomTitleID = 0;
	pTextTail->wTitleSpriteResourceIndex = 0;
	pTextTail->bTitleSpriteFrameCount = 0;
	pTextTail->bTitleSpriteFrameIndex = 0;
	pTextTail->iTitleSpriteWidth = 0;
	pTextTail->iTitleSpriteHeight = 0;
	pTextTail->dwTitleSpriteNextFrameTime = 0;
#endif

// In `void CPythonTextTail::DeleteTextTail(TTextTail* pTextTail)`, find this block:
if (pTextTail->pLeftSeatTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pLeftSeatTextInstance);
		pTextTail->pLeftSeatTextInstance = NULL;
	}

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
	ClearCustomTitleVisuals(pTextTail);
#endif

// Add the following `CPythonTextTail::ClearCustomTitleVisuals` function anywhere in this file:
#if defined(ENABLE_TITLE_SYSTEM)
void CPythonTextTail::ClearCustomTitleVisuals(TTextTail* pTextTail)
{
	if (pTextTail->pTitleBgLeftInstance)
	{
		CGraphicExpandedImageInstance::Delete(pTextTail->pTitleBgLeftInstance);
		pTextTail->pTitleBgLeftInstance = NULL;
	}
	if (pTextTail->pTitleBgMiddleInstance)
	{
		CGraphicExpandedImageInstance::Delete(pTextTail->pTitleBgMiddleInstance);
		pTextTail->pTitleBgMiddleInstance = NULL;
	}
	if (pTextTail->pTitleBgRightInstance)
	{
		CGraphicExpandedImageInstance::Delete(pTextTail->pTitleBgRightInstance);
		pTextTail->pTitleBgRightInstance = NULL;
	}
	if (pTextTail->pTitleSpriteInstance)
	{
		CGraphicExpandedImageInstance::Delete(pTextTail->pTitleSpriteInstance);
		pTextTail->pTitleSpriteInstance = NULL;
	}
	if (pTextTail->pCustomTitleTextInstance)
	{
		CGraphicTextInstance::Delete(pTextTail->pCustomTitleTextInstance);
		pTextTail->pCustomTitleTextInstance = NULL;
	}

	pTextTail->wTitleSpriteResourceIndex = 0;
	pTextTail->bTitleSpriteFrameCount = 0;
	pTextTail->bTitleSpriteFrameIndex = 0;
	pTextTail->iTitleSpriteWidth = 0;
	pTextTail->iTitleSpriteHeight = 0;
	pTextTail->dwTitleSpriteNextFrameTime = 0;
}

void CPythonTextTail::UpdateCustomTitleSpriteFrame(TTextTail* pTextTail, DWORD dwCurrentTime)
{
	if (!pTextTail->pTitleSpriteInstance || pTextTail->wTitleSpriteResourceIndex == 0 || pTextTail->bTitleSpriteFrameCount <= 1)
		return;

	if (pTextTail->dwTitleSpriteNextFrameTime == 0 || dwCurrentTime < pTextTail->dwTitleSpriteNextFrameTime)
		return;

	const BYTE byNextFrame = (pTextTail->bTitleSpriteFrameIndex + 1) % pTextTail->bTitleSpriteFrameCount;
	CResource* pSpriteResource = CPythonTitleSystem::Instance().GetSpriteFrameResource(pTextTail->wTitleSpriteResourceIndex, byNextFrame);
	if (pSpriteResource)
	{
		pTextTail->pTitleSpriteInstance->SetImagePointer((CGraphicImage*)pSpriteResource);
		pTextTail->bTitleSpriteFrameIndex = byNextFrame;
	}

	pTextTail->dwTitleSpriteNextFrameTime = dwCurrentTime + TITLE_SPRITE_FRAME_DELAY_MS;
}

void CPythonTextTail::UpdateCustomTitle(TTextTail* pTextTail, DWORD dwTitleID)
{
	ClearCustomTitleVisuals(pTextTail);

	pTextTail->dwCustomTitleID = dwTitleID;

	if (dwTitleID == 0 || dwTitleID == 1001 || (dwTitleID >= 1 && dwTitleID <= 7))
		return;

	const CPythonTitleSystem::STitleData* pTitleData = CPythonTitleSystem::Instance().GetTitleData(dwTitleID);
	if (!pTitleData)
		return;

	pTextTail->pCustomTitleTextInstance = CGraphicTextInstance::New();
	pTextTail->pCustomTitleTextInstance->SetTextPointer(ms_pFont);
	pTextTail->pCustomTitleTextInstance->SetOutline(true);
	pTextTail->pCustomTitleTextInstance->SetHorizonalAlign(CGraphicTextInstance::HORIZONTAL_ALIGN_CENTER);
	pTextTail->pCustomTitleTextInstance->SetVerticalAlign(CGraphicTextInstance::VERTICAL_ALIGN_CENTER);
	pTextTail->pCustomTitleTextInstance->SetValue(pTitleData->stName.c_str());

	if (pTitleData->iFontColor != 0)
	{
		DWORD dwColor = static_cast<DWORD>(pTitleData->iFontColor);
		float a = ((dwColor >> 24) & 0xFF) / 255.0f;
		float r = ((dwColor >> 16) & 0xFF) / 255.0f;
		float g = ((dwColor >> 8) & 0xFF) / 255.0f;
		float b = (dwColor & 0xFF) / 255.0f;
		if (a == 0.0f) a = 1.0f;
		
		// Force the default gray color (0xFF8E8E8E / -7434610) to ice-blue (0xFFD6E3F3)
		if (dwColor == 0xFF8E8E8E)
		{
			r = 214.0f / 255.0f;
			g = 227.0f / 255.0f;
			b = 243.0f / 255.0f;
		}
		
		pTextTail->pCustomTitleTextInstance->SetColor(r, g, b, a);
	}
	else
	{
		pTextTail->pCustomTitleTextInstance->SetColor(214.0f / 255.0f, 227.0f / 255.0f, 243.0f / 255.0f);
	}
	pTextTail->pCustomTitleTextInstance->Update();

	if (pTitleData->bTitleType == CPythonTitleSystem::TYPE_NAMEPLATE)
	{
		std::string stLeft, stMiddle, stRight;
		if (CPythonTitleSystem::Instance().GetNameplateImages(pTitleData->wResourceIndex, stLeft, stMiddle, stRight))
		{
			CResource* pLeftRes = CResourceManager::Instance().GetResourcePointer(stLeft.c_str());
			CResource* pMiddleRes = CResourceManager::Instance().GetResourcePointer(stMiddle.c_str());
			CResource* pRightRes = CResourceManager::Instance().GetResourcePointer(stRight.c_str());

			if (pLeftRes && pMiddleRes && pRightRes)
			{
				pTextTail->pTitleBgLeftInstance = CGraphicExpandedImageInstance::New();
				pTextTail->pTitleBgLeftInstance->SetImagePointer((CGraphicImage*)pLeftRes);

				pTextTail->pTitleBgMiddleInstance = CGraphicExpandedImageInstance::New();
				pTextTail->pTitleBgMiddleInstance->SetImagePointer((CGraphicImage*)pMiddleRes);

				pTextTail->pTitleBgRightInstance = CGraphicExpandedImageInstance::New();
				pTextTail->pTitleBgRightInstance->SetImagePointer((CGraphicImage*)pRightRes);

				if (IsScreenSpaceBannerTitle(dwTitleID))
				{
					CResource* pSpriteRes = CPythonTitleSystem::Instance().GetSpriteFrameResource(pTitleData->wResourceIndex, 0);
					if (pSpriteRes)
					{
						pTextTail->pTitleSpriteInstance = CGraphicExpandedImageInstance::New();
						pTextTail->pTitleSpriteInstance->SetImagePointer((CGraphicImage*)pSpriteRes);
						pTextTail->wTitleSpriteResourceIndex = pTitleData->wResourceIndex;
						pTextTail->bTitleSpriteFrameCount = 36;
						pTextTail->bTitleSpriteFrameIndex = 0;
						pTextTail->iTitleSpriteWidth = 208;
						pTextTail->iTitleSpriteHeight = 128;
						pTextTail->dwTitleSpriteNextFrameTime = CTimer::Instance().GetCurrentMillisecond() + TITLE_SPRITE_FRAME_DELAY_MS;
					}
				}
			}
		}
	}
}
#endif
