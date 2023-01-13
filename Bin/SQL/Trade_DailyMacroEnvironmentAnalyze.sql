/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 13/01/2023 13:34:27
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Trade_DailyMacroEnvironmentAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "Trade_DailyMacroEnvironmentAnalyze";
CREATE TABLE "Trade_DailyMacroEnvironmentAnalyze" (
  "TradeDay" text NOT NULL,
  "USStock" text,
  "USDollarIndex" text,
  "JapanKoreaStock" text,
  "EropeStock" text,
  "OilPrice" text,
  "VIX" text,
  "FEDMessage" text,
  "BuySellAndStrength" text,
  "DomesticMessage" text,
  "MarketSentiment" text,
  PRIMARY KEY ("TradeDay")
);

PRAGMA foreign_keys = true;
