/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 13/01/2023 13:34:48
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyLongPeriodicAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyLongPeriodicAnalyze";
CREATE TABLE "Future_DailyLongPeriodicAnalyze" (
  "TradeDay" text NOT NULL,
  "FutureContractName" TEXT,
  "MainForceChange" TEXT,
  "CITIC_GoldmanSachs" TEXT,
  "ExternalMarket" TEXT,
  "MonthlyTrend" TEXT,
  "WeeklyTrend" TEXT,
  "DailyTrend" TEXT,
  "4HTrend" TEXT,
  "EmotionalFeeling" TEXT,
  PRIMARY KEY ("TradeDay")
);

PRAGMA foreign_keys = true;
