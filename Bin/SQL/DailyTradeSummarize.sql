/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 18/11/2022 16:07:32
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for DailyTradeSummarize
-- ----------------------------
DROP TABLE IF EXISTS "DailyTradeSummarize";
CREATE TABLE "DailyTradeSummarize" (
  "TradeDay" int NOT NULL,
  "RightDeeds" TEXT,
  "WrongDeeds" TEXT,
  "MoneyBegin" real,
  "MoneyEnd" real,
  "Money2Bank" real,
  "MoneyFromBank" real,
  "GainLoss" real,
  "GainLossPercent" real,
  PRIMARY KEY ("TradeDay")
);

PRAGMA foreign_keys = true;
