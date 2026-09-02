// Add to includes:
#if defined(__TITLE_SYSTEM__)
#	include "title_system.h"
#endif

// Before
		if (!m_map_pkPCChr.empty())
		{
			CHARACTER_VECTOR v;
			v.reserve(m_map_pkPCChr.size());
			std::transform(m_map_pkPCChr.begin(), m_map_pkPCChr.end(), std::back_inserter(v), [](NAME_MAP::value_type map) { return map.second; });
			if (0 == (iPulse % PASSES_PER_SEC(5)))
			{
				FuncUpdateAndResetChatCounter f;
				for_each(v.begin(), v.end(), f);
			}
			std::for_each(v.begin(), v.end(), [iPulse](LPCHARACTER ch) {
				if (ch != nullptr) ch->UpdateCharacter(iPulse); });
		}

// After
		if (!m_map_pkPCChr.empty())
		{
			CHARACTER_VECTOR v;
			v.reserve(m_map_pkPCChr.size());
			std::transform(m_map_pkPCChr.begin(), m_map_pkPCChr.end(), std::back_inserter(v), [](NAME_MAP::value_type map) { return map.second; });
			if (0 == (iPulse % PASSES_PER_SEC(5)))
			{
				FuncUpdateAndResetChatCounter f;
				for_each(v.begin(), v.end(), f);
			}
#if defined(__TITLE_SYSTEM__)
			if (0 == (iPulse % PASSES_PER_SEC(60)))
			{
				for (CHARACTER_VECTOR::iterator it = v.begin(); it != v.end(); ++it)
				{
					if (*it)
						CTitleSystemManager::instance().Process(*it);
				}
			}
#endif
			std::for_each(v.begin(), v.end(), [iPulse](LPCHARACTER ch) {
				if (ch != nullptr) ch->UpdateCharacter(iPulse); });
		}
