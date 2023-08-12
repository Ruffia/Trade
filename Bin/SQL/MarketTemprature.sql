/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 12/08/2023 11:56:13
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for MarketTemprature
-- ----------------------------
DROP TABLE IF EXISTS "MarketTemprature";
CREATE TABLE "MarketTemprature" (
  "Day" date NOT NULL,
  "Number_Rise" integer,
  "Number_Drop" integer,
  "Number_Rise2UpperLimit" integer,
  "Number_Drop2LowerLimit" integer,
  "Number_TradeVolume" integer,
  "Number_MoneyInOut" integer,
  "Number_ProfitLoss" integer,
  PRIMARY KEY ("Day")
);

PRAGMA foreign_keys = true;
