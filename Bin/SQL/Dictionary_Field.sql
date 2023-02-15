/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 15/02/2023 14:58:13
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Dictionary_Field
-- ----------------------------
DROP TABLE IF EXISTS "Dictionary_Field";
CREATE TABLE "Dictionary_Field" (
  "Value" integer,
  "Subject" TEXT,
  "Meaning" TEXT,
  "Translation" TEXT
);

-- ----------------------------
-- Records of Dictionary_Field
-- ----------------------------
INSERT INTO "Dictionary_Field" VALUES (0, 'Festival', 'NewYear', '元旦');
INSERT INTO "Dictionary_Field" VALUES (1, 'Festival', 'SpringFestival', '春节');
INSERT INTO "Dictionary_Field" VALUES (2, 'Festival', 'Qingming', '清明节');
INSERT INTO "Dictionary_Field" VALUES (3, 'Festival', 'LaborDay', '五一');
INSERT INTO "Dictionary_Field" VALUES (4, 'Festival', 'DragonBoatFestival', '端午节');
INSERT INTO "Dictionary_Field" VALUES (5, 'Festival', 'Mid-AutumnFestival', '中秋节');
INSERT INTO "Dictionary_Field" VALUES (6, 'Festival', 'NationalDay', '国庆节');
INSERT INTO "Dictionary_Field" VALUES (0, 'Judgement', 'Absolutely right', '完全正确');
INSERT INTO "Dictionary_Field" VALUES (1, 'Judgement', 'roughly right', '基本正确');
INSERT INTO "Dictionary_Field" VALUES (2, 'Judgement', 'Something Right,Something Wrong', '对错各半');
INSERT INTO "Dictionary_Field" VALUES (3, 'Judgement', 'Wrong', '完全错误');
INSERT INTO "Dictionary_Field" VALUES (0, 'EmotionFeeling', 'Relaxed', '心态放松');
INSERT INTO "Dictionary_Field" VALUES (1, 'EmotionFeeling', 'Hopeful', '期待');
INSERT INTO "Dictionary_Field" VALUES (2, 'EmotionFeeling', 'Cheerful', '愉悦');
INSERT INTO "Dictionary_Field" VALUES (3, 'EmotionFeeling', 'Excited', '兴奋');
INSERT INTO "Dictionary_Field" VALUES (4, 'EmotionFeeling', 'Doubtful', '疑虑');
INSERT INTO "Dictionary_Field" VALUES (5, 'EmotionFeeling', 'Afraid', '紧张，担心害怕，纠结');
INSERT INTO "Dictionary_Field" VALUES (6, 'EmotionFeeling', 'Painful', '感到痛苦');
INSERT INTO "Dictionary_Field" VALUES (7, 'EmotionFeeling', 'BitterlyPainful ', '极度痛苦，不堪回首');
INSERT INTO "Dictionary_Field" VALUES (1, 'TradeType', 'Buy', '买入');
INSERT INTO "Dictionary_Field" VALUES (0, 'TradeType', 'Sell', '卖出');

PRAGMA foreign_keys = true;
