/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 14/02/2023 09:57:35
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyMinorCycleAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyMinorCycleAnalyze";
CREATE TABLE "Future_DailyMinorCycleAnalyze" (
  "TradeDay" text NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "RecordTime" TEXT NOT NULL,
  "MACD_5MIN" TEXT,
  "MACD_15MIN" TEXT,
  "MACD_2H" TEXT,
  "MACDPillar_5MIN" TEXT,
  "MACDPillar_15MIN" TEXT,
  "MACDPillar_2H" TEXT,
  "Trix_5MIN" TEXT,
  "Trix_15MIN" TEXT,
  "Trix_2H" TEXT,
  "SKDJ_5MIN" TEXT,
  "SKDJ_15MIN" TEXT,
  "SKDJ_2H" TEXT,
  "StrengthAngle_5MIN" TEXT,
  "StrengthAngle_15MIN" TEXT,
  "StrengthAngle_2H" TEXT,
  "MainSignal" TEXT,
  "Inclination" TEXT,
  "EmotionFeeling" integer,
  "Forecast" TEXT,
  "Judgement" integer,
  "HighCertainty" integer DEFAULT (0),
  "InkeepingWithMainForce" integer DEFAULT (0),
  "InkeepingWithMacroEconomicPolicy" integer DEFAULT (0),
  "FeelRelaxedButNotAfraid" integer DEFAULT (0),
  "BeHunterNotBePrey" integer DEFAULT (0),
  PRIMARY KEY ("TradeDay", "FutureContractName", "RecordTime")
);

PRAGMA foreign_keys = true;
