/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 18/11/2022 16:00:07
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for DailyMacroEnvironment
-- ----------------------------
DROP TABLE IF EXISTS "DailyMacroEnvironment";
CREATE TABLE "DailyMacroEnvironment" (
  "TradeDay" int NOT NULL,
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

-- ----------------------------
-- Records of DailyMacroEnvironment
-- ----------------------------

PRAGMA foreign_keys = true;
