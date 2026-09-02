// Add to includes:
#if defined(ENABLE_TITLE_SYSTEM)
#include "PythonTitleSystem.h"
#endif

// Add this anonymous namespace above CPythonNetworkStream::RecvSpecificEffect:
#if defined(ENABLE_TITLE_SYSTEM)
namespace
{
	bool IsScreenSpaceTitleNameplate(DWORD dwTitleIndex)
	{
		switch (dwTitleIndex)
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

// In `bool CPythonNetworkStream::RecvSpecificEffect()`, the instance branch has to
// understand the "<effect path>|<title index>" payload the server sends.

// Before
	if (pInstance)
	{
#if defined(ENABLE_GRAPHIC_ON_OFF)
		if (!pInstance->CanRenderActor() || !pInstance->CanRenderEffect())
			return true;
#endif
		CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_TEMP, "", kSpecificEffect.effect_file, false);
		pInstance->AttachSpecialEffect(CInstanceBase::EFFECT_TEMP);
	}

// After
#if defined(ENABLE_TITLE_SYSTEM)
	if (pInstance)
	{
#if defined(ENABLE_GRAPHIC_ON_OFF)
		if (!pInstance->CanRenderActor() || !pInstance->CanRenderEffect())
			return true;
#endif
		DWORD dwTitleIndex = 0;
		char szCleanPath[128];
		strncpy(szCleanPath, kSpecificEffect.effect_file, sizeof(szCleanPath));
		szCleanPath[sizeof(szCleanPath) - 1] = '\0';

		char* pPipe = strchr(szCleanPath, '|');
		if (pPipe)
		{
			*pPipe = '\0';
			dwTitleIndex = strtoul(pPipe + 1, nullptr, 10);
		}

		const bool bClearTitleEffect =
			(strcmp(szCleanPath, "__TITLE_EFFECT_CLEAR__") == 0);
		const bool bIsTitleEffect = (dwTitleIndex > 0);

		if (bClearTitleEffect || bIsTitleEffect)
		{
			pInstance->ClearSpecialEffectOverHead();
			pInstance->SetEquippedTitle(0);
		}

		if (!bClearTitleEffect)
		{
			if (bIsTitleEffect)
			{
				pInstance->SetEquippedTitle(dwTitleIndex);
				if (!IsScreenSpaceTitleNameplate(dwTitleIndex))
				{
					CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_TEMP, "", szCleanPath, false);
					pInstance->AttachSpecialEffectOverHead(CInstanceBase::EFFECT_TEMP);
				}
			}
			else
			{
				CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_TEMP, "", szCleanPath, false);
				pInstance->AttachSpecialEffect(CInstanceBase::EFFECT_TEMP);
			}
		}
	}
#else
	if (pInstance)
	{
#if defined(ENABLE_GRAPHIC_ON_OFF)
		if (!pInstance->CanRenderActor() || !pInstance->CanRenderEffect())
			return true;
#endif
		CInstanceBase::RegisterEffect(CInstanceBase::EFFECT_TEMP, "", kSpecificEffect.effect_file, false);
		pInstance->AttachSpecialEffect(CInstanceBase::EFFECT_TEMP);
	}
#endif
