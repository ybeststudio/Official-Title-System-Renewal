// Find this line:
Set(HEADER_GC_PASSIVE_ATTR, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCPassiveAttr), STATIC_SIZE_PACKET));

// Add after it:
#if defined(ENABLE_TITLE_SYSTEM)
		Set(HEADER_GC_TITLE_SYSTEM, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCTitleSystem), DYNAMIC_SIZE_PACKET));
#endif
