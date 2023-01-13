/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 13/01/2023 13:34:42
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
  "15MIN_MACD" TEXT,
  "1H_MACD" TEXT,
  "4H_MACD" TEXT,
  "15MIN_MACDPillar" TEXT,
  "1H_MACDPillar" TEXT,
  "4H_MACDPillar" TEXT,
  "15MIN_Trix" TEXT,
  "1H_Trix" TEXT,
  "4H_Trix" TEXT,
  "15MIN_SKDJ" TEXT,
  "1H_SKDJ" TEXT,
  "4H_SKDJ" TEXT,
  "15MIN_StrengthAngle" TEXT,
  "1H_StrengthAngle" TEXT,
  "4H_StrengthAngle" TEXT,
  "MainSignal" TEXT,
  "Inclination" TEXT,
  "EmotionFeeling" TEXT,
  "Forecast" TEXT,
  "Reality" TEXT
);

PRAGMA foreign_keys = true;
