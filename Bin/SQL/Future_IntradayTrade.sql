/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 13/01/2023 14:19:01
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_IntradayTrade
-- ----------------------------
DROP TABLE IF EXISTS "Future_IntradayTrade";
CREATE TABLE "Future_IntradayTrade" (
  "TradeDay" text NOT NULL,
  "FutureContractName" text,
  "TradeTime" text,
  "ExpectOrderTime" integer,
  "TradeType" integer,
  "OpenQuantity" integer,
  "OpenPrice" float,
  "ExpectMaxLoss" float,
  "ExpectIncomde" float,
  "BuySellStrength" text,
  "Reason4Transaction" text,
  "Reason4Prevention" text,
  "China_HKStockMarket01" text,
  "FutureMarket01" text,
  "FutureMACD01" text,
  "FutureStrenthAngle01" text,
  "FutureTRIX01" text,
  "China_HKStockMarket02" text,
  "FutureMarket02" text,
  "FutureMACD02" text,
  "FutureStrenthAngle02" text,
  "FutureTRIX02" text,
  "ClosePositionReason" text,
  "PreventClosePositionReason" text,
  "ClosePositionTime" text,
  "CloseQuantity" float,
  "ClosePrice" float,
  "GainLoss" float,
  "HighestPrice" float,
  "LowestPricePrice" float,
  "TransactionQuality" float,
  PRIMARY KEY ("TradeDay")
);

PRAGMA foreign_keys = true;
