/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 09/02/2023 09:14:43
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Dictionary_Field
-- ----------------------------
DROP TABLE IF EXISTS "Dictionary_Field";
CREATE TABLE "Dictionary_Field" (
  "Value" integer,
  "Subject" TEXT,
  "Meaning" TEXT
);

-- ----------------------------
-- Records of Dictionary_Field
-- ----------------------------
INSERT INTO "Dictionary_Field" VALUES (1, 'Festival', 'NewYear');
INSERT INTO "Dictionary_Field" VALUES (2, 'Festival', 'SpringFestival');
INSERT INTO "Dictionary_Field" VALUES (3, 'Festival', 'Qingming');
INSERT INTO "Dictionary_Field" VALUES (4, 'Festival', 'LaborDay');
INSERT INTO "Dictionary_Field" VALUES (5, 'Festival', 'DragonBoatFestival');
INSERT INTO "Dictionary_Field" VALUES (6, 'Festival', 'Mid-AutumnFestival');
INSERT INTO "Dictionary_Field" VALUES (7, 'Festival', 'NationalDay');
INSERT INTO "Dictionary_Field" VALUES (1, 'Judgement', 'Absolutely right');
INSERT INTO "Dictionary_Field" VALUES (2, 'Judgement', 'roughly right');
INSERT INTO "Dictionary_Field" VALUES (3, 'Judgement', 'Something Right,Something Wrong');
INSERT INTO "Dictionary_Field" VALUES (4, 'Judgement', 'Wrong');

PRAGMA foreign_keys = true;
