/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 20/02/2023 14:38:01
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Future_DailyTraceConflict
-- ----------------------------
DROP TABLE IF EXISTS "Future_DailyTraceConflict";
CREATE TABLE "Future_DailyTraceConflict" (
  "TradeDay" TEXT NOT NULL,
  "FutureContractName" TEXT NOT NULL,
  "ConflictType" integer NOT NULL,
  "DetailedDescription" text,
  PRIMARY KEY ("TradeDay", "FutureContractName", "ConflictType")
);

PRAGMA foreign_keys = true;
