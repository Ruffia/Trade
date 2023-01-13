/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 13/01/2023 13:34:35
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyTradeSummarize
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyTradeSummarize";
CREATE TABLE "Future_DailyTradeSummarize" (
  "TradeDay" text NOT NULL,
  "RightDeeds" TEXT,
  "WrongDeeds" TEXT,
  "MoneyBegin" float,
  "MoneyEnd" float,
  "Money2Bank" float,
  "MoneyFromBank" float,
  "GainLoss" float,
  "GainLossPercent" float,
  PRIMARY KEY ("TradeDay")
);

PRAGMA foreign_keys = true;
