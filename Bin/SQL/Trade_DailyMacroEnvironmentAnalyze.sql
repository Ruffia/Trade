/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 12/08/2023 11:56:21
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Trade_DailyMacroEnvironmentAnalyze
-- ----------------------------
DROP TABLE IF EXISTS "Trade_DailyMacroEnvironmentAnalyze";
CREATE TABLE "Trade_DailyMacroEnvironmentAnalyze" (
  "TradeDay" date NOT NULL,
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
