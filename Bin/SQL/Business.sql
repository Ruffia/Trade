/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 13/01/2023 11:00:39
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
INSERT INTO "Business" VALUES ('Future_DailyMinorCycleAnalyze', NULL, '期货日内短周期分析记录表');
INSERT INTO "Business" VALUES ('Future_DailyLongPeriodicAnalyze', NULL, '期货日内长周期分析记录表');

PRAGMA foreign_keys = true;
