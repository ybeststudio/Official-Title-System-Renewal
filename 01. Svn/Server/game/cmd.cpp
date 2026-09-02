// Find this line:
ACMD(do_stun);

// Add after it:
#if defined(__TITLE_SYSTEM__)
ACMD(do_title_system);
#endif

// Find this line:
{ "slow", do_slow, 0, POS_DEAD, GM_LOW_WIZARD },

// Add after it:
#if defined(__TITLE_SYSTEM__)
	{ "title_system", do_title_system, 0, POS_DEAD, GM_LOW_WIZARD },
#endif
