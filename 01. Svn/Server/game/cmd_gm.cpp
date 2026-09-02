// Add to includes, then add the GM command implementation at the bottom of this file.
// The include and the ACMD body share one #if block here:
#if defined(__TITLE_SYSTEM__)
#include "title_system.h"

ACMD(do_title_system)
{
	if (!argument || !*argument)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Kullanim: /title_system <unvan_no/unvan_adi> veya /title_system <oyuncu_adi> <unvan_no/unvan_adi>");
		return;
	}

	const char* p = argument;
	while (*p && isspace(*p))
		p++;

	if (!*p)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Kullanim: /title_system <unvan_no/unvan_adi> veya /title_system <oyuncu_adi> <unvan_no/unvan_adi>");
		return;
	}

	std::string firstWord;
	while (*p && !isspace(*p))
	{
		firstWord.push_back(*p);
		p++;
	}

	while (*p && isspace(*p))
		p++;

	LPCHARACTER tch = nullptr;
	std::string titleArg;

	if (*p)
	{
		tch = CHARACTER_MANAGER::instance().FindPC(firstWord.c_str());
		if (tch)
		{
			titleArg = p;
		}
		else
		{
			tch = ch;
			titleArg = firstWord + " " + p;
		}
	}
	else
	{
		tch = ch;
		titleArg = firstWord;
	}

	if (!tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "%s isimli oyuncu oyunda veya ayni haritada degil.", firstWord.c_str());
		return;
	}

	titleArg.erase(0, titleArg.find_first_not_of(" \t\r\n"));
	titleArg.erase(titleArg.find_last_not_of(" \t\r\n") + 1);

	if (titleArg.empty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Gecersiz unvan parametresi.");
		return;
	}

	const CTitleSystemManager::STitleProto* proto = nullptr;

	bool isNumeric = true;
	for (char c : titleArg)
	{
		if (!isdigit(c))
		{
			isNumeric = false;
			break;
		}
	}

	if (isNumeric)
	{
		DWORD dwIdx = static_cast<DWORD>(strtoul(titleArg.c_str(), nullptr, 10));
		proto = CTitleSystemManager::instance().FindProto(dwIdx);
		if (!proto)
			proto = CTitleSystemManager::instance().FindProtoByResource(static_cast<WORD>(dwIdx));
	}
	else
	{
		proto = CTitleSystemManager::instance().FindProtoByName(titleArg);
	}

	if (!proto)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Unvan bulunamadi: '%s'", titleArg.c_str());
		return;
	}

	DWORD dwTitleIndex = proto->dwTitleIndex;

	if (CTitleSystemManager::instance().IsTitleOwned(tch, dwTitleIndex))
	{
		CTitleSystemManager::instance().DeletePlayerTitle(tch, dwTitleIndex);
	}

	if (CTitleSystemManager::instance().GrantTitle(tch, dwTitleIndex, 0, true))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Basariyla '%s' (%u) unvani %s isimli oyuncuya verildi.", proto->szName, dwTitleIndex, tch->GetName());
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Unvan verme basarisiz: '%s'", proto->szName);
	}
}
#endif
