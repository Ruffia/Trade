/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 18/11/2022 16:23:21
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for DailyFutureContractAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "DailyFutureContractAnalyze";
CREATE TABLE "DailyFutureContractAnalyze" (
  "TradeDay" int NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "MainForceChange" TEXT,
  "OuterDisc" TEXT,
  "MonthlyTrend" TEXT,
  "WeeklyTrend" TEXT,
  "DailyTrend" TEXT,
  "4HTrend" TEXT,
  "EmotionalFeeling" TEXT,
  PRIMARY KEY ("TradeDay", "FutureContractName")
);

PRAGMA foreign_keys = true;
