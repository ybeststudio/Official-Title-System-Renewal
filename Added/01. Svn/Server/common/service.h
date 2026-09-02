#if !defined(__INC_COMMON_SERVICE_H__)
#define __INC_COMMON_SERVICE_H__

#define __LOCALE_SERVICE_EUROPE__
#define __LOCALE_SERVICE_BEST_STUDIO__

////////////////////////////////////////////////////////////////////////////////
// Oyun ile İlgili
#define __MAILBOX__ 											// Posta Kutusu Sistemi
#define __QUEST_RENEWAL__ 										// Görev Sayfası Yenileme
#if defined(__QUEST_RENEWAL__)
	#define __QUEST_EVENT_DAMAGE__ 								// Hasar Görev Etkinliği
	#define __QUEST_EVENT_DEAD__ 								// Ölüm Görev Etkinliği
	#define __QUEST_EVENT_FISH__ 								// Balıkçılık Görev Etkinliği
	#define __QUEST_EVENT_MINE__ 								// Madencilik Görev Etkinliği
	#define __QUEST_EVENT_BUY_SELL__ 							// NPC Alış/Satış Görev Etkinliği
	#define __QUEST_EVENT_CRAFT__ 								// Üretim Görev Etkinliği
	#define __QUEST_EVENT_EMOTION__ 							// Duygu İfadesi Görev Etkinliği
	#define __QUEST_EVENT_RESTART_HERE__ 						// Burada Yeniden Başla Görev Etkinliği
	#define __QUEST_REQUEST_EVENT__ 							// Görev Talep Etkinliği
#endif
#define __CHATTING_WINDOW_RENEWAL__ 							// Sohbet Penceresi Yenileme (Mini Versiyon)
#define __CUBE_RENEWAL__ 										// Küp Sistemi Yenileme
#define __RANKING_SYSTEM__ 										// Sıralama Sistemi
#define __ELEMENT_SYSTEM__ 										// Element Sistemi
#define __SEND_TARGET_INFO__ 									// Canavar Bilgisi & Düşen Eşyalar
#define __PVP_COUNTDOWN__ 										// PvP Düello Geri Sayımı
#define __REFINE_MSG_ADD__ 										// Geliştirme Başarısızlık Mesajı Genişletme
#define __MOUNT_UPGRADE__ 										// Resmi At Gelistirme (Binicilik) Sistemi
#define __PVP_BALANCE_IMPROVING__ 								// PvP Denge İyileştirmesi
#define __NPC_LOCATION_HELPER__ 								// Geliştirilmiş Harita Sistemi

////////////////////////////////////////////////////////////////////////////////
// Harita & Zindan ile İlgili
#define __SNOW_DUNGEON__ 										// Kar Zindanı
#define __DUNGEON_RENEWAL__ 									// Genişletilmiş Zindan Fonksiyonları
#define __BLUE_DRAGON_RENEWAL__ 								// Mavi Ejderha Yenilemesi
#define __MT_THUNDER_DUNGEON__ 									// Ochao Tapınağı
#define __DAWNMIST_DUNGEON__ 									// Erebus Zindanı
#define __SUNGMAHEE_GATE__ 										// Sungmahee Geçidi Sistemi
#define __DEFENSE_WAVE__ 										// Savunma Dalgası
#define __CLIENT_TIMER__ 										// İstemci Zamanlayıcı (Instance'lar için kullanılır)
#define __LABYRINTH_DUNGEON__ 									// Labirent Zindanı (Araçlar)
#define __ELEMENTAL_DUNGEON__ 									// Element Zindanı
#define __GUILD_DRAGONLAIR_SYSTEM__ 							// Lonca Ejderha İni Sistemi
#if defined(__GUILD_DRAGONLAIR_SYSTEM__)
	#define __GUILD_DRAGONLAIR_PARTY_SYSTEM__ 					// Lonca Ejderha İni Parti Sistemi
#endif

////////////////////////////////////////////////////////////////////////////////
// Mini-Oyun ile İlgili
#define __MINI_GAME_RUMI__ 										// Mini Oyun Rumi
#if defined(__MINI_GAME_RUMI__)
	#define __OKEY_EVENT_FLAG_RENEWAL__ 						// Okey Etkinlik Bayrağı Yenileme
#endif
#define __MINI_GAME_YUTNORI__ 									// Mini Oyun Yutnori
#if defined(__MINI_GAME_YUTNORI__)
	#define __YUTNORI_EVENT_FLAG_RENEWAL__ 						// Yutnori Etkinlik Bayrağı Yenileme
#endif
#define __MINI_GAME_CATCH_KING__ 								// Mini Oyun Catch King
#if defined(__MINI_GAME_CATCH_KING__)
	#define __CATCH_KING_EVENT_FLAG_RENEWAL__ 					// Catch King Etkinlik Bayrağı Yenileme
#endif
#define __FISHING_GAME__ 										// Balıkçılık Oyunu
#define __SUMMER_EVENT_ROULETTE__ 								// Mini Oyun Rulet (Yaz Sonu Etkinliği)

////////////////////////////////////////////////////////////////////////////////
// Etkinlik ile İlgili
#define __EASTER_EVENT__ 										// Paskalya Etkinliği 2011
#define __XMAS_EVENT_2008__ 									// Noel Etkinliği 2008
#define __XMAS_EVENT_2012__ 									// Noel Etkinliği 2012
#define __2016_VALENTINE_EVENT__ 								// Sevgililer Günü Etkinliği 2016~2024
#define __HALLOWEEN_EVENT_2014__ 								// Cadılar Bayramı Etkinliği 2011~2014 (Cadılar Bayramı Saçları)
#define __OX_RENEWAL__ 											// OX Etkinliği Yenileme
#define __EVENT_BANNER_FLAG__ 									// Etkinlik Banner Bayrakları
#define __METINSTONE_SWAP__ 									// Metin Taşı Görünüm Değiştirme
#define __RACE_SWAP__ 											// Irk Görünüm Değiştirme
#define __SNOWFLAKE_STICK_EVENT__ 								// Kar Tanesi Çubuğu Etkinliği
#define __INGAME_EVENT_MANAGER__ 								// Oyun İçi Etkinlik Yöneticisi
#define __FLOWER_EVENT__ 										// Çiçek Etkinliği
#define __TREASURE_HUNT__ 										// Hazine Adasi Etkinligi
#define __SPORTS_MATCH_EVENT__ 									// Sampiyonluk Etkinligi

////////////////////////////////////////////////////////////////////////////////
// Para Birimi ile İlgili
#define __CHEQUE_SYSTEM__ 										// Çek (Won) Sistemi
#define __GEM_SYSTEM__ 											// Mücevher (Gaya) Sistemi

////////////////////////////////////////////////////////////////////////////////
// Market & Mağaza ile İlgili
#define __SHOPEX_RENEWAL__ 										// ShopEX Yenileme
#if defined(__SHOPEX_RENEWAL__)
	#define __SHOPEX_TAB4__ 									// ShopEX 4 Sekme
#endif
//#define __SHOPEX_EMPIRE_TAX__ 								// ShopEX Vergi Sistemi (3 Kat Fiyat)
#define __MYSHOP_DECO__ 										// Kişisel Pazar Dekorasyonu
#define __MYSHOP_EXPANSION__ 									// Ekstra Kişisel Pazar Sekmesi
#define __PRIVATESHOP_SEARCH_SYSTEM__ 							// Kişisel Pazar Arama Sistemi
#if defined(__GEM_SYSTEM__)
	#define __GEM_SHOP__ 										// Mücevher (Gaya) Mağazası
#endif

////////////////////////////////////////////////////////////////////////////////
// Ejderha Taşı Simya ile İlgili
#define __DRAGON_SOUL_SYSTEM__ 									// Ejderha Taşı Simya Sistemi
#if defined(__DRAGON_SOUL_SYSTEM__)
	#define __DS_GRADE_MYTH__ 									// Ejderha Taşı Simya Efsanevi Seviye
	#define __DS_SET__ 											// Ejderha Taşı Simya Set (Tablo) Yönetimi
	#define __DS_CHANGE_ATTR__ 									// Ejderha Taşı Simya Özellik Değiştirme
	#define __DS_7_SLOT__ 										// Ejderha Taşı Simya 7. Slot
#endif

////////////////////////////////////////////////////////////////////////////////
// Evcil Hayvan Petlerle ile İlgili
#define __PET_SYSTEM__ 											// Pet Sistemi
#if defined(__PET_SYSTEM__)
	#define __PET_LOOT__ 										// Pet Otomatik Eşya Toplama
	//#define __PET_LOOT_AI__ 									// Pet Loot Yapay Zekası (Gidip eşyaları toplar)
	#define USE_ACTIVE_PET_SEAL_EFFECT 							// Pet Mührü Efektini Kullan
	#define PET_SEAL_ACTIVE_SOCKET_IDX 2 						// Pet Mührü Aktif Soket İndeksi (2)
	#define USE_PET_SEAL_ON_LOGIN 								// Giriş / Işınlanma Sonrası Pet Mührünü Kullan
#endif
#define __GROWTH_PET_SYSTEM__ 									// Resmi Gelişim Pet Sistemi
#if defined(__GROWTH_PET_SYSTEM__)
	#define __PET_ATTR_DETERMINE__ 								// Pet Büyüleme / Özellik Belirleme
	#define __PET_PRIMIUM_FEEDSTUFF__ 							// Premium Pet Besleme Sistemi
#endif
////////////////////////////////////////////////////////////////////////////////
// Karakter ile İlgili
#define __PLAYER_PER_ACCOUNT5__ 								// Hesap Başına 5 Karakter
#define __WOLFMAN_CHARACTER__ 									// Lycan Karakteri
#if defined(__WOLFMAN_CHARACTER__)
	//#define __DISABLE_WOLFMAN_CREATION__ 						// Lycan Karakter Oluşturmayı Devre Dışı Bırak
#endif
#define __VIEW_TARGET_HP__ 										// Hedef Canını Görüntüleme
#if defined(__VIEW_TARGET_HP__)
	#define __VIEW_TARGET_PLAYER_HP__ 							// Oyuncu Hedef Canını Görüntüleme
#endif
#define __IMPROVED_LOGOUT_POINTS__ 								// Geliştirilmiş Çıkış Noktaları
#define __EXPRESSING_EMOTIONS__ 								// Özel Hareketler
#define __CONQUEROR_LEVEL__ 									// Fatih Seviyesi
#define __MULTI_LANGUAGE_SYSTEM__ 								// Çoklu Dil Sistemi
#define __DELETE_FAILURE_TYPE__ 								// Karakter Silme Hata Türü
#define __LEFT_SEAT__ 											// Oturma (AFK)
#define __AFFECT_RENEWAL__ 										// Etki Sistemi Yenileme

////////////////////////////////////////////////////////////////////////////////
// Yetenek ile İlgili
#define __7AND8TH_SKILLS__ 										// 7. ve 8. Pasif Yetenekler
#define __SKILL_COOLTIME_UPDATE__ 								// Ölüm Sonrası Yetenek Bekleme Süresi Yenileme
#define __9TH_SKILL__ 											// 9. Oyuncu Yeteneği
#define __PARTY_PROFICY__ 										// Parti Ustalığı Pasif Yeteneği
#define __PARTY_INSIGHT__ 										// Parti Sezgi Pasif Yeteneği

////////////////////////////////////////////////////////////////////////////////
// Parti & Lonca ile İlgili
#define __DICE_SYSTEM__ 										// Yeni Zar Sistemi (Parti)
#define __WJ_SHOW_PARTY_ON_MINIMAP__ 							// Mini Haritada Parti Üyelerini Göster
#define __PARTY_CHANNEL_FIX__ 									// Parti Kanal Düzeltmesi
#define __GUILD_LEADER_GRADE_NAME__ 							// Lonca Lideri Ünvan İsmi (TextTail)
#define __PARTY_KILL_RENEWAL__ 									// Tüm Öldürmelerin Partiye Sayılması
#define __GUILD_WAR_AUTO_JOIN_LEADER__ 							// Lonca Savaşına Lider Olarak Otomatik Katılım
#define __GUILD_EVENT_FLAG__ 									// Lonca Etkinlik Bayrağı

////////////////////////////////////////////////////////////////////////////////
// Arkadaş Listesi (Messenger) ile İlgili
#define __MESSENGER_BLOCK_SYSTEM__ 								// Messenger Engelleme Sistemi
#define __MESSENGER_GM__ 										// Messenger GM Listesi
#define __MESSENGER_DETAILS__ 									// Messenger Detayları
#define __MESSENGER_RENEWAL__ 									// Gelişmiş Topluluk Sistemi
#if defined(__MESSENGER_RENEWAL__)
	#define __COMMUNITY_GUILD_RENEWAL__							// Topluluk lonca sekmesi üye listesi / konum
#endif

////////////////////////////////////////////////////////////////////////////////
// Envanter ile İlgili
#define __EXTEND_INVEN_SYSTEM__ 								// Genişletilmiş Envanter Sistemi
#define __EXTEND_MALLBOX__ 										// Genişletilmiş Mağaza Kutusu
#define __SAFEBOX_IMPROVING__ 									// Depo Sistemi İyileştirmesi

////////////////////////////////////////////////////////////////////////////////
// Ekipman ile İlgili
#define __QUIVER_SYSTEM__ 										// Ok Çantası Sistemi
#define __PENDANT_SYSTEM__ 										// Tılsım (Element) Sistemi
#define __GLOVE_SYSTEM__ 										// Eldiven Ekipmanı

////////////////////////////////////////////////////////////////////////////////
// Kostüm ile İlgili
#define __COSTUME_SYSTEM__ 										// Kostüm Sistemi
#define __MOUNT_COSTUME_SYSTEM__ 								// Binek Kostüm Sistemi
#define __ACCE_COSTUME_SYSTEM__ 								// Kuşak (Sash) Kostüm Sistemi
#define __AURA_COSTUME_SYSTEM__ 								// Aura Kostüm Sistemi
#define __WEAPON_COSTUME_SYSTEM__ 								// Silah Kostüm Sistemi
#if defined(__WEAPON_COSTUME_SYSTEM__)
	#define __HIDE_WEAPON_COSTUME_WITH_NO_MAIN_WEAPON__ 		// Ana Silah Yoksa Silah Kostümünü Gizle
#endif
#define __MOVE_COSTUME_ATTR__ 									// Kostüm Özelliği Aktarma (Eşya Birleştirme)
#define __HIDE_COSTUME_SYSTEM__ 								// Kostüm Gizleme Sistemi

////////////////////////////////////////////////////////////////////////////////
// Eşya ile İlgili
#define __MAGIC_REDUCTION__ 									// Büyü Azaltma Eşyası
#define __STONE_OF_BLESS__ 										// Kutsama Taşı (Geliştirme Eşyası)
#define __REFINE_PICKAXE_RENEWAL__ 								// Kazma Geliştirme Yenilemesi
#define __REFINE_FISHINGROD_RENEWAL__ 							// Olta Geliştirme Yenilemesi
#define __SOUL_BIND_SYSTEM__ 									// Ruh Bağlama Sistemi (Mühür Parşömeni)
#if defined(__SOUL_BIND_SYSTEM__)
	#define __DRAGON_SOUL_SEAL__ 								// Ejderha Ruhu Bağlama Parşömeni
	#define __UN_SEAL_SCROLL_PLUS__ 							// Gelişmiş Ruh Çözme Parşömeni
#endif
//#define __SOUL_SYSTEM__ 										// Ruh Sistemi
#if defined(__SOUL_SYSTEM__)
	#define __SOUL_SYSTEM_CALC_FINAL_DAMAGE__ 					// Nihai Hasar Hesabında Ruh Sistemi
#endif
#define __ITEM_APPLY4__ 										// Genişletilmiş Bonus (4)
#define __ITEM_SOCKET6__ 										// Genişletilmiş Eşya Soketi (6)
#define __ITEM_VALUE10__ 										// Genişletilmiş Eşya Değerleri
#define __ITEM_APPLY_RANDOM__ 									// Rastgele Bonus Uygulama (Temel Bonus)
#define __CHANGED_ATTR__ 										// Bonus Değiştirme / Seçme
#define __ATTR_6TH_7TH__ 										// 6. ve 7. Bonus Özelliği
#define __SKILLBOOK_COMB_SYSTEM__ 								// Yetenek Kitabı Birleştirme Sistemi
#define __CHANGE_LOOK_SYSTEM__ 									// Görünüm Değiştirme Sistemi (Eşya Görünümü)
#define __LOOT_FILTER_SYSTEM__ 									// Eşya Toplama Filtresi Sistemi
#if defined(__LOOT_FILTER_SYSTEM__)
	#define __PREMIUM_LOOT_FILTER__ 							// Premium Eşya Toplama Filtresi Kullanımı
#endif
#define __GACHA_SYSTEM__ 										// Boss (Gacha) Kutuları (x Defa Açma)
#define __LUCKY_BOX__ 											// Şans Kutusu
#define __SET_ITEM__ 											// Set Eşya Bonusu
#define __GEM_CONVERTER__ 										// Mücevher (Gaya) Dönüştürücü
#define __REFINE_ELEMENT_SYSTEM__ 								// Element Geliştirme Sistemi
#define __USE_NEXT_AUTO_POTION__ 								// Otomatik İksir Bittiğinde Sonrakini Kullan
#define __REFINE_STACK_FIX__ 									// Geliştirme Yığın Hatası Düzeltmesi (Taşlar için)
#define __TITLE_SYSTEM__										// Resmi Unvan Baslik Sistemi
#define __PASSIVE_ATTR__										// Resmi Kalıntı Bonusu (Soul Relic) sistemi
#define ENABLE_AUTO_SYSTEM										// Resmi Otomatik Av Sistemi
#if defined(ENABLE_AUTO_SYSTEM)
	#define ENABLE_AUTO_RESTART_EVENT
#endif
#define ENABLE_PARTY_MATCH										// Parti eşleşme sistemi
#define ENABLE_MONSTER_CARD										// Canavar Kart Sistemi (hesap görevi / koleksiyon; /monstercard)

////////////////////////////////////////////////////////////////////////////////
// Arayüz (UI) ile İlgili
#define __WJ_SHOW_MOB_INFO__ 									// Canavar Seviye & Agresiflik Bilgisi
#define __WJ_PICKUP_ITEM_EFFECT__ 								// Eşya Toplama Efekti
#define __NEW_USER_CARE__ 										// Kullanıcı Kontrol Sistemi
#define __BINARY_ATLAS_MARK_INFO__ 								// Atlas İşaret Bilgisi Yükleme
#define __POPUP_NOTICE__ 										// Açılır Bildirim (Pop-up)
#define __NEW_DROP_DIALOG__ 									// Yeni Düşme Penceresi (Eşya Silme Seçenekli)
#define __ITEM_DROP_RENEWAL__ 									// Eşya Düşme Yenilemesi (Renk Efektli)
#define __GAME_OPTION_ESCAPE__ 									// Oyun Seçenekleri (ESC Menüsü)

////////////////////////////////////////////////////////////////////////////////
// Diğer (Misc) Sistemler
#define __EXTENDED_RELOAD__ 									// Genişletilmiş GM Yenileme Komutları (Drop için)
#define __EXTENDED_ITEM_AWARD__ 								// Genişletilmiş Eşya Ödül Sistemi
#define __ENVIRONMENT_SYSTEM__ 									// Ortam Sistemi
#define __LOCALE_CLIENT__ 										// Yerelleştirme (Locale) İstemcisi
#define __REVERSED_FUNCTIONS__ 									// Resmi olarak kullanılan fonksiyonlar
#define __REDUCED_ENTITY_VIEW__ 								// Varlık (entity) görünümünü azaltır
#define __PORTAL_NAMES__ 										// Portal isimlerinin görüntülenmesini sağlar
#define __ENTITY_PRELOADING__ 									// Haritadaki tüm NPC/Mob'ları önceden yükleyerek yeni varlıklarla karşılaşınca oluşan mikro donmaları engeller
#define __MCP_SERVER_AI__ 										// MCP Yapay Zekâ Sunucu Eklentisi

////////////////////////////////////////////////////////////////////////////////
// Ağ (Network) ile İlgili
#define __IMPROVED_PACKET_ENCRYPTION__ 							// Geliştirilmiş Paket Şifreleme
#if defined(__IMPROVED_PACKET_ENCRYPTION__)
	#define USE_IMPROVED_PACKET_DECRYPTED_BUFFER 				// Geliştirilmiş Paket Çözme (Decrypt) Buffer Kullanımı
#endif
//#define __SEND_SEQUENCE__ 									// Sıralama (Sequence) Eşleştirme
#define __UDP_BLOCK__ 											// UDP Engelleme
//#define __ALLOW_EXTERNAL_PEER__ 								// Harici Peer API'ye İzin Ver
#define __PROXY_IP__ 											// Proxy IP Kullanımı
#define __MOVE_CHANNEL__ 										// Kanal Değiştirme
#define __CHECK_PORT_STATUS__ 									// Port Durumu Kontrolü
#define __AUTO_DETECT_INTERNAL_IP__ 							// Dahili IP Otomatik Tespit
//#define __USE_SERVER_KEY__ 									// Sunucu Anahtarı Kontrolü

#include "disclaimer.h" 										// Yasal Uyarı Başlık Dosyası
#endif // __INC_COMMON_SERVICE_H__
