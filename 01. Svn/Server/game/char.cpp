// Add to includes:
#if defined(__TITLE_SYSTEM__)
#	include "title_system.h"
#endif

// Before
			else
				addPacket.bGuildLeaderGrade = 1;
#endif
		}

		addPacket.sAlignment = m_iAlignment / 10;

		d->Packet(&addPacket, sizeof(TPacketGCCharacterAdditionalInfo));
	}


	if (iDur)
	{

// After
			else
				addPacket.bGuildLeaderGrade = 1;
#endif
		}

		addPacket.sAlignment = m_iAlignment / 10;

		d->Packet(&addPacket, sizeof(TPacketGCCharacterAdditionalInfo));
	}

#if defined(__TITLE_SYSTEM__)
	if (IsPC())
		CTitleSystemManager::instance().BroadcastEquippedTitle(this);
#endif

	if (iDur)
	{

// In `bool CHARACTER::Show(long lMapIndex, long x, long y, long z, bool bShowSpawnMotion/* = false */ , bool bAggressive)`, find this block:
SetValidComboInterval(0);

// Add after it:
#if defined(__TITLE_SYSTEM__)
	if (IsPC())
	{
		CTitleSystemManager::instance().BroadcastEquippedTitle(this);
	}
#endif
