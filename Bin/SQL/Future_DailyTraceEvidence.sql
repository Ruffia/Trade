/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 15/02/2023 18:13:37
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyTraceEvidence
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyTraceEvidence";
CREATE TABLE "Future_DailyTraceEvidence" (
  "TradeDay" TEXT NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "TimePeriod" integer,
  "TechnicalIndex" integer,
  "IndexValue" integer,
  PRIMARY KEY ("TradeDay", "FutureContractName")
);

PRAGMA foreign_keys = true;
