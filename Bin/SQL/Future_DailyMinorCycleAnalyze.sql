/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 12/08/2023 11:55:40
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyMinorCycleAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyMinorCycleAnalyze";
CREATE TABLE "Future_DailyMinorCycleAnalyze" (
  "TradeDay" date NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "RecordTime" TEXT NOT NULL,
  "MACD_5MIN" integer,
  "MACD_15MIN" integer,
  "MACD_2H" integer,
  "MACDPillar_5MIN" integer,
  "MACDPillar_15MIN" integer,
  "MACDPillar_2H" integer,
  "Trix_5MIN" integer,
  "Trix_15MIN" integer,
  "Trix_2H" integer,
  "SKDJ_5MIN" integer,
  "SKDJ_15MIN" integer,
  "SKDJ_2H" integer,
  "StrengthAngle_5MIN" integer,
  "StrengthAngle_15MIN" integer,
  "StrengthAngle_2H" integer,
  "FutureMarketTemperature" integer,
  "StockMarketTemperature" integer,
  "Inclination" integer,
  "MainSignal" text,
  "Forecast" TEXT,
  "Judgement" integer,
  "EmotionFeelingAfterCheck" integer,
  "HighCertainty" integer DEFAULT (0),
  "InkeepingWithMainForce" integer DEFAULT (0),
  "InkeepingWithMacroEconomicPolicy" integer DEFAULT (0),
  "FeelRelaxedButNotAfraid" integer DEFAULT (0),
  "BeHunterNotBePrey" integer DEFAULT (0),
  PRIMARY KEY ("TradeDay", "FutureContractName", "RecordTime")
);

PRAGMA foreign_keys = true;
