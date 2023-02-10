/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 10/02/2023 10:02:07
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
  "MACD_15MIN" TEXT,
  "MACD_1H" TEXT,
  "MACD_4H" TEXT,
  "MACDPillar_15MIN" TEXT,
  "MACDPillar_1H" TEXT,
  "MACDPillar_4H" TEXT,
  "Trix_15MIN" TEXT,
  "Trix_1H" TEXT,
  "Trix_4H" TEXT,
  "SKDJ_15MIN" TEXT,
  "SKDJ_1H" TEXT,
  "SKDJ_4H" TEXT,
  "StrengthAngle_15MIN" TEXT,
  "StrengthAngle_1H" TEXT,
  "StrengthAngle_4H" TEXT,
  "MainSignal" TEXT,
  "Inclination" TEXT,
  "EmotionFeeling" TEXT,
  "Forecast" TEXT,
  "Reality" TEXT,
  PRIMARY KEY ("TradeDay", "FutureContractName", "RecordTime")
);

PRAGMA foreign_keys = true;
