#if !defined(__INC_LOCALE_INC_H__)
#define __INC_LOCALE_INC_H__

#include "Version.h"

#define LOCALE_SERVICE_EUROPE
#define LOCALE_SERVICE_OWSAP

////////////////////////////////////////////////////////////////////////////////
// Oyunla ilgili
#define ENABLE_MAILBOX										// Posta Kutusu Sistemi
#define ENABLE_QUEST_RENEWAL								// Görev Sayfası Yenilemesi
#define ENABLE_CHATTING_WINDOW_RENEWAL						// Sohbet Penceresi Yenilemesi (Mini Versiyon)
#define ENABLE_CUBE_RENEWAL									// Küp (Craft) Sistemi Yenilemesi
#define ENABLE_RANKING_SYSTEM								// Sıralama Sistemi
#define ENABLE_ELEMENT_ADD									// Canavar Element Sistemi
#define ENABLE_SEND_TARGET_INFO								// Canavar Bilgisi ve Düşen Eşyalar
#define ENABLE_REFINE_MSG_ADD								// Geliştirme Başarısızlık Mesajı Genişletme
#define ENABLE_BALANCE_IMPROVING							// PvP Denge İyileştirmeleri
#define ENABLE_QUEST_REQUEST_EVENT							// Görev Talep Etkinliği
#define ENABLE_MOUNT_UPGRADE_SYSTEM							// Resmi At Gelistirme (Binicilik) Sistemi

////////////////////////////////////////////////////////////////////////////////
// Harita ve Zindan ile İlgili
#define ENABLE_DEFENSE_WAVE									// Savunma Dalgası (Hydra Zindanı)
#define ENABLE_CLIENT_TIMER									// İstemci Zamanlayıcı (Instance'lar için kullanılır)
#define ENABLE_ELEMENTAL_DUNGEON							// Elemental Zindan
#define ENABLE_LABYRINTH_DUNGEON							// Labirent Zindani (Secret Dungeon)
#define ENABLE_DAWNMIST_DUNGEON								// Erebus / Other World Zindani
#define ENABLE_GUILD_DRAGONLAIR_SYSTEM						// Lonca Ejderha İni Sistemi
#define ENABLE_NPC_LOCATION_HELPER							// Geliştirilmiş Harita Sistemi
#if defined(ENABLE_GUILD_DRAGONLAIR_SYSTEM)
	#define ENABLE_GUILD_DRAGONLAIR_PARTY_SYSTEM			// Lonca Ejderha İni Parti Sistemi
#endif

////////////////////////////////////////////////////////////////////////////////
// Mini Oyunlar ile İlgili
#define ENABLE_MINI_GAME_RUMI								// Mini Oyun Rumi (Okey Kartı)
#if defined(ENABLE_MINI_GAME_RUMI)
	#define ENABLE_OKEY_EVENT_FLAG_RENEWAL					// Okey Etkinlik Bayrağı Yenilemesi
#endif
#define ENABLE_MINI_GAME_YUTNORI							// Mini Oyun Yutnori
#if defined(ENABLE_MINI_GAME_YUTNORI)
	#define ENABLE_YUTNORI_EVENT_FLAG_RENEWAL				// Yutnori Etkinlik Bayrağı Yenilemesi
#endif
#define ENABLE_MINI_GAME_CATCH_KING							// Mini Oyun Catch King
#if defined(ENABLE_MINI_GAME_CATCH_KING)
	#define ENABLE_CATCH_KING_EVENT_FLAG_RENEWAL			// Catch King Etkinlik Bayrağı Yenilemesi
#endif
#define ENABLE_FISHING_GAME									// Balık Tutma Mini Oyunu
#define ENABLE_SUMMER_EVENT_ROULETTE						// Yaz Etkinliği Ruleti
#if defined(ENABLE_SUMMER_EVENT_ROULETTE)
	//#define ENABLE_LATE_SUMMER_EVENT_SKYBOX				// Yaz Sonu Etkinliği Skybox
#endif

////////////////////////////////////////////////////////////////////////////////
// Etkinlikler ile İlgili
#define ENABLE_OX_RENEWAL									// OX Etkinliği Yenilemesi
#define ENABLE_EVENT_BANNER_FLAG							// Etkinlik Banner Bayrakları
#define ENABLE_METINSTONE_SWAP								// Metin Taşı Görünüm Değiştirme
#define ENABLE_RACE_SWAP									// Irk (Karakter) Görünüm Değiştirme
#define ENABLE_SNOWFLAKE_STICK_EVENT						// Kar Tanesi Çubuğu Etkinliği
#define ENABLE_EVENT_BANNER									// Etkinlik Banner Sistemi
#define ENABLE_INGAME_EVENT_MANAGER							// Oyun Ici Etkinlik Yoneticisi Affect'leri
#define ENABLE_EVENT_BANNER_REWARD_LIST_RENEWAL				// Etkinlik Banner Ödül Listesi Yenilemesi
#define ENABLE_FLOWER_EVENT									// Çiçek Etkinliği
#define ENABLE_TREASURE_HUNT								// Hazine Adasi Etkinligi
#define ENABLE_SPORTS_MATCH_EVENT							// Sampiyonluk Etkinligi
#if defined(ENABLE_SPORTS_MATCH_EVENT)						
	#define ENABLE_FIX_SPORTS_MATCH_DESC					// Alkış Etkinliği Tooltip Desch
#endif

////////////////////////////////////////////////////////////////////////////////
// Para Birimleri ile İlgili
#define ENABLE_CHEQUE_SYSTEM								// Çek (Won) Sistemi
#define ENABLE_GEM_SYSTEM									// Mücevher (Gaya) Sistemi

////////////////////////////////////////////////////////////////////////////////
// Mağaza ile İlgili
#define ENABLE_SHOPEX_RENEWAL								// ShopEx Yenilemesi
#if defined(ENABLE_SHOPEX_RENEWAL)
	#define ENABLE_SHOPEX_TAB4								// ShopEx 4 Sekme
#endif
#define ENABLE_MYSHOP_DECO									// Kişisel Pazar Dekorasyonu
#define ENABLE_MYSHOP_EXPANSION								// Ekstra Kişisel Pazar Sekmesi
#define ENABLE_PRIVATESHOP_SEARCH_SYSTEM					// Kişisel Pazar Arama Sistemi

////////////////////////////////////////////////////////////////////////////////
// Ejderha Taşı Simya ile İlgili
#define ENABLE_DRAGON_SOUL_SYSTEM							// Ejderha Taşı Simya Sistemi
#if defined(ENABLE_DRAGON_SOUL_SYSTEM)
	#define ENABLE_DS_GRADE_MYTH							// Ejderha Taşı Simya Efsanevi Seviye
	#define ENABLE_DS_SET									// Ejderha Taşı Simya Set (Tablo) Yönetimi
	#define ENABLE_DS_CHANGE_ATTR							// Ejderha Taşı Simya Özellik Değiştirme
	#define ENABLE_DS_7_SLOT								// Ejderha Taşı Simya 7. Slot
#endif

////////////////////////////////////////////////////////////////////////////////
// Karakter ile İlgili
#define ENABLE_PLAYER_PER_ACCOUNT5							// Hesap Başına Karakter Sayısı (5)
#define ENABLE_WOLFMAN_CHARACTER							// Lycan Karakteri
#if defined(ENABLE_WOLFMAN_CHARACTER)
	//#define DISABLE_WOLFMAN_CREATION						// Lycan Oluşturmayı Devre Dışı Bırak
#endif
#define ENABLE_VIEW_TARGET_DECIMAL_HP						// Hedef HP Görüntüleme (Ondalıklı)
#if defined(ENABLE_VIEW_TARGET_DECIMAL_HP)
	#define ENABLE_VIEW_TARGET_PLAYER_HP					// Oyuncu Hedef HP Görüntüleme
#endif
#define ENABLE_IMPROVED_LOGOUT_POINTS						// Geliştirilmiş Çıkış Noktaları
#define ENABLE_MULTI_LANGUAGE_SYSTEM						// Çoklu Dil Sistemi
#define ENABLE_EXPRESSING_EMOTION							// Özel Hareketler (Emote Sistemi)
#define ENABLE_CONQUEROR_LEVEL								// Fatih (Şampiyon) Seviyesi
#define ENABLE_DELETE_FAILURE_TYPE							// Karakter Silme Hata Türleri
#if defined(ENABLE_DELETE_FAILURE_TYPE)
	#define ENABLE_DELETE_FAILURE_TYPE_ADD					// Ek Karakter Silme Hata Türleri
#endif
#define ENABLE_LEFT_SEAT									// Oturma (AFK) Sistemi
#define ENABLE_AFFECT_RENEWAL								// Affect (Durum Etkisi) Yenilemesi

////////////////////////////////////////////////////////////////////////////////
// Yetenekler ile İlgili
#define ENABLE_678TH_SKILL									// 6. ve 7. Yetenekler
#define ENABLE_SKILL_COOLTIME_UPDATE						// Ölüm Sonrası Yetenek Bekleme Süresi Güncellemesi
#define ENABLE_PARTY_PROFICY								// Parti Ustalığı Pasif Yeteneği
#define ENABLE_PARTY_INSIGHT								// Parti İçgörü (Insight) Pasif Yeteneği

////////////////////////////////////////////////////////////////////////////////
// Parti ve Lonca ile İlgili
#define ENABLE_DICE_SYSTEM									// Yeni Zar Sistemi (Parti)
#define WJ_SHOW_PARTY_ON_MINIMAP							// Mini Haritada Parti Üyelerini Göster
#define ENABLE_PARTY_CHANNEL_FIX							// Parti Kanal Hatası Düzeltmesi
#define ENABLE_GUILD_LEADER_GRADE_NAME						// Lonca Lideri Ünvan İsmi (TextTail)
#define ENABLE_GUILD_BUILDING_OBJ_PLACEMENT_EFFECT			// Lonca Bina Nesnesi Yerleştirme Efekti

////////////////////////////////////////////////////////////////////////////////
// Arkadaş Listesi (Messenger) ile İlgili
#define ENABLE_MESSENGER_LOCK_FIX							// Arkadaş Listesi Kilitlenme Hatası Düzeltmesi
#define ENABLE_MESSENGER_BLOCK								// Arkadaş Engelleme Sistemi
#define ENABLE_MESSENGER_GM									// GM Arkadaş Listesi
#define ENABLE_MESSENGER_DETAILS							// Arkadaş Listesi Detayları
#define ENABLE_MESSENGER_RENEWAL							// Resmi Gelişmiş Mesaj Sistemi
#if defined(ENABLE_MESSENGER_RENEWAL)
#define ENABLE_COMMUNITY_GUILD_RENEWAL						// Community lonca sekmesi uye listesi / konum
#endif

////////////////////////////////////////////////////////////////////////////////
// Envanter ile İlgili
#define ENABLE_NEW_EQUIPMENT_SYSTEM							// Yeni Ekipman Sistemi (Kemer)
#define ENABLE_EXTEND_INVEN_SYSTEM							// Genişletilmiş Envanter Sayfaları (4)
#define ENABLE_EXTEND_MALLBOX								// Genişletilmiş Mağaza Kutusu (Mallbox)
#define ENABLE_SAFEBOX_IMPROVING							// Depo (Safebox) İyileştirmeleri

////////////////////////////////////////////////////////////////////////////////
// Ekipman ile İlgili
#define ENABLE_QUIVER_SYSTEM								// Ok Torbası (Quiver) Ekipmanı
#define ENABLE_PENDANT										// Tılsım (Element) Sistemi
#define ENABLE_GLOVE_SYSTEM									// Eldiven Ekipmanı
#define ENABLE_ADDITIONAL_EQUIPMENT_PAGE					// Ek Ekipman Sayfası

/////////////////////////////////////////////////////////////////////////////////////////////////
// Gelişim Peti ile İlgili
#define ENABLE_GROWTH_PET_SYSTEM							// Gelişim Peti Sistemi
#if defined(ENABLE_GROWTH_PET_SYSTEM)
	#define ENABLE_PET_ATTR_DETERMINE						// Pet Özellik Belirleme (Büyüleme)
	#define ENABLE_PET_PRIMIUM_FEEDSTUFF					// Pet Premium Besleme Sistemi
	#define ENABLE_GROWTH_PET_SKILL_DEL						// Pet Becerilerini Silme
	#define ENABLE_GROWTH_PET_HATCHING_MONEY_CHANGE			// Pet Kuluçka Ücreti
	#define ENABLE_UPBRINGING_PET_CONQUEROR_LEVEL			// Pet Şampiyon (Fatih) Seviyesi Sistemi
#endif

////////////////////////////////////////////////////////////////////////////////
// Kostüm ile İlgili
#define ENABLE_COSTUME_SYSTEM								// Kostüm Sistemi
#define ENABLE_MOUNT_COSTUME_SYSTEM							// Binek Kostüm Sistemi
#define ENABLE_ACCE_COSTUME_SYSTEM							// Kuşak (Acce) Kostüm Sistemi
#define ENABLE_ACCE_SECOND_COSTUME_SYSTEM					// İkinci Kuşak (Acce) Kostüm Sistemi
#define ENABLE_AURA_COSTUME_SYSTEM							// Aura Kostüm Sistemi
#define ENABLE_WEAPON_COSTUME_SYSTEM						// Silah Kostüm Sistemi
#define ENABLE_MOVE_COSTUME_ATTR							// Kostüm Özelliği Aktarma (Item Birleştirme)
#define ENABLE_HIDE_COSTUME_SYSTEM							// Kostüm Gizleme Sistemi

////////////////////////////////////////////////////////////////////////////////
// Eşyalar ile İlgili
#define ENABLE_ENERGY_SYSTEM								// Enerji Sistemi
#define ENABLE_MAGIC_REDUCTION								// Büyü Azaltma Eşyası
#define ENABLE_STONE_OF_BLESS								// Kutsama Taşı (Geliştirme Eşyası)
#define ENABLE_SOULBIND_SYSTEM								// Ruh Bağlama Sistemi (Eşyaları Korur)
#if defined(ENABLE_SOULBIND_SYSTEM)
	#define ENABLE_DRAGON_SOUL_SEAL							// Ejderha Ruhu Mührü
	#define ENABLE_UN_SEAL_SCROLL_PLUS						// Mühür Kaldırma Parşömeni+
#endif
//#define ENABLE_SOUL_SYSTEM								// Ruh Sistemi
#define ENABLE_ITEM_APPLY4									// Genişletilmiş Apply Bonus (4)
#define ENABLE_ITEM_SOCKET6									// Genişletilmiş Eşya Yuvası (6)
#define ENABLE_ITEM_VALUE10									// Genişletilmiş Eşya Değerleri
#define ENABLE_APPLY_RANDOM									// Rastgele Bonus (Temel Bonus)
#define ENABLE_CHANGED_ATTR									// Özellik Değiştirme / Seçme
#define ENABLE_ATTR_6TH_7TH									// 6. ve 7. Özellik
#define ENABLE_SKILLBOOK_COMB_SYSTEM						// Beceri Kitabı Birleştirme Sistemi
#define ENABLE_CHANGE_LOOK_SYSTEM							// Görünüm Değiştirme Sistemi (Eşya Görünümü)
#define ENABLE_LOOTING_SYSTEM								// Yağma (Loot) Sistemi (Filtre)
#if defined(ENABLE_LOOTING_SYSTEM)
	#define ENABLE_PREMIUM_LOOTING							// Loot Filtre Sistemi Premium Kullanımı
#endif
#define ENABLE_GACHA										// Kutu Açma Sistemi (x Defa Aç)
#define ENABLE_LUCKY_BOX									// Şans Kutusu
#define ENABLE_SET_ITEM										// Set Eşya Bonusu
#define ENABLE_GEM_CONVERTER								// Mücevher Dönüştürücü
#define ENABLE_REFINE_ELEMENT_SYSTEM						// Element Geliştirme Sistemi
#define ENABLE_TITLE_SYSTEM									// Resmi Ünvan Başlık Sistemi
#define ENABLE_PASSIVE_ATTR									// Resmi Kalıntı Bonusu Sistemi
#if defined(ENABLE_PASSIVE_ATTR)
	#define ENABLE_PASSIVE_ATTR_TOOLTIP						// Resmi Kalıntı Bonusu Efsun Tooltipi
#endif
#define ENABLE_AUTO_SYSTEM									// Resmi Otomatik Av Sistemi
	#if defined(ENABLE_AUTO_SYSTEM)
		#define ENABLE_AUTO_RESTART_EVENT					// Otomatik yeniden başlatma etkinliği
		#define ENABLE_IMPROVED_AUTOMATIC_HUNTING_SYSTEM	// Geliştirilmiş Otomatik Avlanma Sistemi
		#define ENABLE_AUTO_AFFECT_DISPLAY					// Otomatik etki göstergesi
		#define EVENT_HANDLER_MASTER						// Olay yönetici ana modülü
	#endif
#define ENABLE_PARTY_MATCH									// Grup Parti Sistemi
#define ENABLE_BATTLE_FIELD									// Savaş Bölgesi Sistemi

////////////////////////////////////////////////////////////////////////////////
// Arayüz (UI) ile İlgili
#define ENABLE_SELECT_RENEWAL								// Yeni Karakter Seçim Arayüzü
#define ENABLE_CREATE_RENEWAL								// Yeni Karakter Oluşturma Arayüzü
#define ENABLE_SERVER_SELECT_RENEWAL						// Sunucu Seçim Listesi Yenilemesi
#define ENABLE_CHANNEL_LIST									// Kanal Listesi Yenilemesi
#define LOGIN_COUNT_DOWN_UI_MODIFY							// Giriş Geri Sayım Arayüzü
#define ENABLE_NEW_USER_CARE								// Yeni Oyuncu Destek Sistemi (Kontrol)
#define WJ_SHOW_MOB_INFO									// Canavar Seviye ve Agresiflik Bilgisi
#define WJ_ENABLE_PICKUP_ITEM_EFFECT						// Eşya Toplama Efekti
#define WJ_ENABLE_TRADABLE_ICON								// Takas Edilemeyen Eşya İkonu
#define WJ_MULTI_TEXTLINE									// Çok Satırlı Metin (TextLine)
#define ENABLE_ATLAS_MARK_INFO								// Atlas İşaret Bilgisi Yükleme
#define ENABLE_POPUP_NOTICE									// Açılır Bildirim (Pop-up)
#define ENABLE_NEW_DROP_DIALOG								// Yeni Drop Penceresi (Eşya Silme Seçenekli)
#define ENABLE_ITEM_DROP_RENEWAL							// Eşya Düşme Yenilemesi (Renk Efektli)
#define ENABLE_TEXT_IMAGE_LINE								// Görselli Metin Satırı (Emoji)
#define ENABLE_HIGH_LIGHT_IMAGE								// Vurgulu Görsel (Highlight Image)
#define ENABLE_SLOT_SCALE									// Slot Ölçeklendirme
#define ENABLE_SLOT_COOLTIME_REFRESH_FIX					// Slot Bekleme Süresi Yenileme Düzeltmesi
#define ENABLE_CLIP_MASK									// Pencere Kırpma Maskesi (Clip Mask)
#define ENABLE_MOUSE_WHEEL_TOP_WINDOW						// Fare Tekerleği ile Üst Pencere Kontrolü
#define RENDER_TARGET										// Model Render Hedef Sistemi
#define ENABLE_MONSTER_CARD									// Model Render Hedef Sistemi
#if defined(ENABLE_MONSTER_CARD)
	#define ENABLE_MONSTER_CARD_ACHIEV						//
#endif
#define ENABLE_KEYCHANGE_SYSTEM								// Klavye Özelleştirme Sistemi
#if defined(ENABLE_KEYCHANGE_SYSTEM)
	#define ENABLE_DSS_KEY_SELECT							// DSS Tuş Seçimi
#endif
#define ENABLE_DETAILS_UI									// Karakter Detayları (Bonus Sayfası)
#define ENABLE_CHAT_LOG_FIX									// Sohbet Kayıt Hatası Düzeltmesi
#define ENABLE_INPUT_CANCEL									// Görev Girdi İptali
#define ENABLE_HELP_RENEWAL									// Yardım Menüsü Yenilemesi
#define ENABLE_MOVE_WND_SCREEN_LIMIT						// Pencere Taşıma Ekran Sınırı
#define ENABLE_QUEST_WIDTH_EXPANSION						// Görev Penceresi Genişletme
#define ENABLE_MINIMAP_TEXTURE_PATTERN						// Mini Harita Doku Deseni
#define ENABLE_HIDE_ATTACHING_EFFECT_ON_ACTING				// Aksiyon Sırasında Bağlı Efektleri Gizleme
#define ENABLE_LOADING_TIP									// Yükleme Ekranı İpuçları
#define ENABLE_TAB_NEXT_TARGET								// TAB ile Sonraki Hedef Seçimi
#define ENABLE_GAME_OPTION_ESCAPE							// ESC ile Oyun Seçenekleri
#define ENABLE_ATLAS_GM_GOTO								// Atlas Üzerinden GM Işınlanma
#define MINIMAP_SMOOTH_ZOOM									// Yumuşak Mini Harita Yakınlaştırma
#define __FONT_TEXTURE_FIX__								// Font Dokularında Nokta Hatasını Önlemek İçin Padding Ekler
#define ENABLE_DRAGON_GATE 									// Ejderha Geçidi Sistemi
#define ENABLE_SUNGMAHEE_GATE 								// Sungmahee Geçidi Sistemi
#define ENABLE_GAME_OPTION_RENEWAL 							// Oyun Ayarları Yenileme
#define ENABLE_DAMAGE_PRINT_IMPROVE 						// Resmi Damage Efekt Güncellemesi

// Çevre ve Render ile İlgili
#define REMOVE_SAFEZONE_COLLISION							// Güvenli Bölge NPC Çarpışmasını Kaldır
#define REMOVE_SHOP_COLLISION								// Pazar (Shop) Çarpışmasını Kaldır
#define REMOVE_HORSE_COLLISION								// At Çarpışmasını Kaldır
#define REMOVE_PET_COLLISION								// Pet Çarpışmasını Kaldır
#define REMOVE_OX_PLAYER_COLLISION							// OX Haritası Oyuncu Çarpışmasını Kaldır
#define REMOVE_BAZAR_COLLISION								// Pazar Alanı Çarpışmasını Kaldır
#define ENABLE_FOG_FIX										// Sis (Fog) Düzeltmesi
#define ENABLE_ENVIRONMENT_EFFECT_OPTION					// Ortam Efekt Seçenekleri
#define ENABLE_FOV_OPTION									// Genişletilmiş Görüş Açısı (FoV) Ayarı
#define ENABLE_DISABLE_SOFTWARE_TILING						// Yazılımsal Tiling Devre Dışı
#define ENABLE_GRAPHIC_ON_OFF								// Grafik Sistemi Aç/Kapat Seçeneği
#define ENABLE_SHADOW_RENDER_QUALITY_OPTION					// Gölge Render Kalitesi Ayarı
#define ENABLE_OPTIMIZATION									// Resmi Animasyon Optimizasyonu [REVERSED]
#define ENABLE_OX_RENDER_AREA								// OX Etkinliği Render Alanı
#define ENVIRONMENT_SYSTEM									// Ortam Sistemi
#define ENABLE_FLY_TARGET_POSITION							// Resmi Uçuş Hedef Pozisyonu [REVERSED]
#define ENABLE_SAVE_CAMERA_MODE								// Resmi Kamera Modu Kaydetme [REVERSED]
#define __ENABLE_STEALTH_FIX__								// Gizlilik (Stealth) Düzeltmesi

////////////////////////////////////////////////////////////////////////////////
// Diğer (Misc)
#define ENABLE_LOCALE_CLIENT								// Dil (Locale) İstemci Metinleri
#define ENABLE_EMOTION_HIDE_WEAPON							// Hareket Sırasında Silah Gizleme
#define ENABLE_NO_MOUNT_CHECK								// Yuva Kontrolü Yoksay

////////////////////////////////////////////////////////////////////////////////
// Uygulama ile İlgili
#define ENABLE_SHARED_PACK_CHANGE_LOCALE_PATH				// UIScript Reverse
#define DISABLE_INDEX_FILE									// Dahili Pack Index Kullanımı (Index Dosyasını Devre Dışı Bırak)
#define ENABLE_ETER_PACK_OBSCURING							// EterPack Anahtar Gizleme
#define ENABLE_DISCORD_RPC									// Discord Rich Presence (RPC)
#define ENABLE_CEF_BROWSER									// CEF Tarayıcı Entegrasyonu
#define ENABLE_CHAR_CONFIG									// Karakter Yapılandırma Sistemi
#define ENABLE_RENDER_LOGIN_EFFECTS							// Giriş Ekranı Efektlerini Etkinleştir (introSelect)
#define ENABLE_LOADING_PERFORMANCE							// Yükleme Performansı İyileştirmeleri
#define IMPROVE_ENTITY_FREEZE								// Entity Donma İyileştirmesi
#define ENABLE_ENTITY_PRELOADING							// Haritadaki Tüm NPC/Mob Ön Yükleme (Mikro Donmaları Önler)
#define REMOVING_COLLECTORS									// Garbage Collector Kaldırma (Harita Verilerini Bellekte Tutma)
#define BLACK_COLOR											// Oyundaki Siyah Renk Düzeltmesi
#if defined(_DEBUG)
	#define ENABLE_CONSOLE_WINDOW							// Konsol Penceresi
	#else
	#define ENABLE_CHECK_DLL_MD5							// Dosya MD5 Kontrolü
#endif
#define ENABLE_DEV_FILE_MODE								// Pack kapatmadan direk root'u okumayı etkinleştirir.
#define ENABLE_UI_DEBUG_WINDOW								// Oyun içi UI debug penceresi etkinleştirir;

////////////////////////////////////////////////////////////////////////////////
// Ağ (Network) ile İlgili
#define ENABLE_MOVE_CHANNEL									// Oyun Kanalı Değiştirme Sistemi

////////////////////////////////////////////////////////////////////////////////
// ImGui Geliştirici Araçları
// Locale_inc, ServiceDefs'ten önce dahil edilir; araçların Cython build'lerinde de aktif kalması sağlanır.
#if !defined(__IMGUI__)
	#define __IMGUI__										// ImGui Geliştirici Araçlarını Etkinleştir
#endif

#include "Disclaimer.h"										// Uyarı / Sorumluluk Reddi Başlık Dosyası
#endif // __INC_LOCALE_INC_H__
