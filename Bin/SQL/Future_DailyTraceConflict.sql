/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 16/02/2023 08:38:30
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyTraceConflict
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyTraceConflict";
CREATE TABLE "Future_DailyTraceConflict" (
  "TradeDay" TEXT NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "number" integer NOT NULL,
  "TechnicalIndex" integer,
  "IndexValue" integer,
  "ConflictTechnicalIndex" integer,
  "ConflictIndexValue" integer,
  PRIMARY KEY ("TradeDay", "FutureContractName", "number")
);

PRAGMA foreign_keys = true;
