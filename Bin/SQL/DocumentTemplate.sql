/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 01/08/2023 11:07:34
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for DocumentTemplate
-- ----------------------------
DROP TABLE IF EXISTS "DocumentTemplate";
CREATE TABLE "DocumentTemplate" (
  "DocumentTemplate" TEXT NOT NULL,
  "Desc" TEXT,
  PRIMARY KEY ("DocumentTemplate")
);

-- ----------------------------
-- Records of DocumentTemplate
-- ----------------------------
INSERT INTO "DocumentTemplate" VALUES ('TradeDay', '交易日');
INSERT INTO "DocumentTemplate" VALUES ('TradeRecord', '交易记录');

PRAGMA foreign_keys = true;
