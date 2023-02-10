/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 10/02/2023 10:02:00
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyLongPeriodicAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyLongPeriodicAnalyze";
CREATE TABLE "Future_DailyLongPeriodicAnalyze" (
  "TradeDay" text NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "MainForceChange" TEXT,
  "CITIC_GoldmanSachs" TEXT,
  "ExternalMarket" TEXT,
  "Trend_Month" TEXT,
  "Trend_Week" TEXT,
  "Trend_Day" TEXT,
  "Trend_4H" TEXT,
  "EmotionalFeeling" TEXT,
  PRIMARY KEY ("TradeDay", "FutureContractName")
);

PRAGMA foreign_keys = true;
