/*
 * Official Title System - player.player_title
 *
 * TR: Oyuncunun sahip oldugu unvanlari ve bitis zamanlarini tutar.
 *     `title_index`  -> title_system.cpp icindeki STitleProto.dwTitleIndex
 *     `end_time`     -> unix timestamp; 0 kayit yuklenirken otomatik doldurulur.
 *     Kusanili unvan bu tabloda DEGIL, quest flag `title_system.active` icinde tutulur.
 *
 * EN: Stores the titles a player owns plus their expiration time.
 *     `title_index`  -> STitleProto.dwTitleIndex in title_system.cpp
 *     `end_time`     -> unix timestamp; 0 is normalized on load.
 *     The equipped title is NOT stored here, it lives in the quest flag
 *     `title_system.active`.
 *
 * Multi-table (get_table_postfix) forks must also create player_title<postfix>.
 */

START TRANSACTION;

CREATE TABLE IF NOT EXISTS `player_title` (
  `pid`         INT(10) UNSIGNED NOT NULL,
  `title_index` INT(10) UNSIGNED NOT NULL,
  `end_time`    INT(10) UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`pid`, `title_index`),
  KEY `idx_player_title_pid` (`pid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin5;

COMMIT;
