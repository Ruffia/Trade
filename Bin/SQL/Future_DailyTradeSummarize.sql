/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 12/08/2023 11:56:00
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyTradeSummarize
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyTradeSummarize";
CREATE TABLE "Future_DailyTradeSummarize" (
  "TradeDay" date NOT NULL,
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
