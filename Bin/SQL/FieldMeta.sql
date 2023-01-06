/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 05/01/2023 20:18:13
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for FieldMeta
-- ----------------------------
DROP TABLE IF EXISTS "FieldMeta";
CREATE TABLE "FieldMeta" (
  "FieldName" TEXT NOT NULL,
  "TableName" TEXT NOT NULL,
  "DataType" TEXT,
  "PrimaryKey" integer,
  "SequenceID" integer,
  PRIMARY KEY ("FieldName", "TableName")
);

-- ----------------------------
-- Records of FieldMeta
-- ----------------------------
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 1, 0);
INSERT INTO "FieldMeta" VALUES ('USStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1);
INSERT INTO "FieldMeta" VALUES ('USDollarIndex', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 2);
INSERT INTO "FieldMeta" VALUES ('JapanKoreaStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 3);
INSERT INTO "FieldMeta" VALUES ('EropeStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 4);
INSERT INTO "FieldMeta" VALUES ('OilPrice', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 5);
INSERT INTO "FieldMeta" VALUES ('VIX', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 6);
INSERT INTO "FieldMeta" VALUES ('FEDMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 7);
INSERT INTO "FieldMeta" VALUES ('BuySellAndStrength', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 8);
INSERT INTO "FieldMeta" VALUES ('DomesticMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 9);
INSERT INTO "FieldMeta" VALUES ('MarketSentiment', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 10);
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Future_DailyTradeSummarize', 'string', 1, 0);
INSERT INTO "FieldMeta" VALUES ('RightDeeds', 'Future_DailyTradeSummarize', 'string', 0, 1);
INSERT INTO "FieldMeta" VALUES ('WrongDeeds', 'Future_DailyTradeSummarize', 'string', 0, 2);
INSERT INTO "FieldMeta" VALUES ('MoneyBegin', 'Future_DailyTradeSummarize', 'float', 0, 3);
INSERT INTO "FieldMeta" VALUES ('MoneyEnd', 'Future_DailyTradeSummarize', 'float', 0, 4);
INSERT INTO "FieldMeta" VALUES ('Money2Bank', 'Future_DailyTradeSummarize', 'float', 0, 5);
INSERT INTO "FieldMeta" VALUES ('MoneyFromBank', 'Future_DailyTradeSummarize', 'float', 0, 6);
INSERT INTO "FieldMeta" VALUES ('GainLoss', 'Future_DailyTradeSummarize', 'float', 0, 7);
INSERT INTO "FieldMeta" VALUES ('GainLossPercent', 'Future_DailyTradeSummarize', 'float', 0, 8);

PRAGMA foreign_keys = true;
