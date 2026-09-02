// In `const char* CItemData::GetUseTypeString()`, extend the switch statement with:
#if defined(ENABLE_TITLE_SYSTEM)
		case USE_TITLE:
			return DEF_STR(USE_TITLE);
#endif
