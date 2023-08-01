/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 01/08/2023 11:07:17
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Business
-- ----------------------------
DROP TABLE IF EXISTS "Business";
CREATE TABLE "Business" (
  "BusinessName" TEXT NOT NULL,
  "UI" TEXT,
  "Description" TEXT,
  PRIMARY KEY ("BusinessName")
);

-- ----------------------------
-- Records of Business
-- ----------------------------
INSERT INTO "Business" VALUES ('Trade_DailyMacroEnvironmentAnalyze', 'Trade_DailyMacroEnvironmentAnalyze.xml', '交易日宏观环境分析记录表');
INSERT INTO "Business" VALUES ('Future_IntradayTrade', 'Future_IntradayTrade.xml', '期货日内交易记录表');
INSERT INTO "Business" VALUES ('Future_DailyTradeSummarize', 'Future_DailyTradeSummarize.xml', '期货日交易总结记录表');
INSERT INTO "Business" VALUES ('Future_DailyMinorCycleAnalyze', 'FutureContract_TrackPhraseIn1Day.xml', '期货日内短周期分析记录表');
INSERT INTO "Business" VALUES ('Future_DailyLongPeriodicAnalyze', 'FutureContract_LongPeriodicAnalyze.xml', '期货日内长周期分析记录表');
INSERT INTO "Business" VALUES ('Future_DailyTraceEvidence', 'FutureContract_DailyTraceEvidence.xml', '期货日内日内行情中的线索');
INSERT INTO "Business" VALUES ('Future_DailyTraceConflict', 'FutureContract_DailyTraceConflict.xml', '期货日内日内行情中的冲突点');
INSERT INTO "Business" VALUES ('DocumentTemplate', NULL, '应用程序所用模板');

PRAGMA foreign_keys = true;
