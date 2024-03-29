/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 02/09/2023 17:32:59
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Stock_MarketTemperature
-- ----------------------------
DROP TABLE IF EXISTS "Stock_MarketTemperature";
CREATE TABLE "Stock_MarketTemperature" (
  "Day" date NOT NULL,
  "Number_Rise" integer,
  "Number_Drop" integer,
  "Number_Rise2UpperLimit" integer,
  "Number_Drop2LowerLimit" integer,
  "Number_TradeVolume" integer,
  "Number_MoneyInOut" integer,
  "Number_ForeignMoneyInOut" integer,
  PRIMARY KEY ("Day")
);

PRAGMA foreign_keys = true;
