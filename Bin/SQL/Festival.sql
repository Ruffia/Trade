/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 03/02/2023 13:10:21
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Festival
-- ----------------------------
DROP TABLE IF EXISTS "Festival";
CREATE TABLE "Festival" (
  "Date" Date NOT NULL,
  "Festival" integer
);

-- ----------------------------
-- Records of Festival
-- ----------------------------
INSERT INTO "Festival" VALUES ('2023-01-01', 1);
INSERT INTO "Festival" VALUES ('2023-01-02', 1);
INSERT INTO "Festival" VALUES ('2023-01-21', 2);
INSERT INTO "Festival" VALUES ('2023-01-22', 2);
INSERT INTO "Festival" VALUES ('2023-01-23', 2);
INSERT INTO "Festival" VALUES ('2023-01-24', 2);
INSERT INTO "Festival" VALUES ('2023-01-25', 2);
INSERT INTO "Festival" VALUES ('2023-01-26', 2);
INSERT INTO "Festival" VALUES ('2023-01-27', 2);
INSERT INTO "Festival" VALUES ('2023-04-05', 3);
INSERT INTO "Festival" VALUES ('2023-04-29', 4);
INSERT INTO "Festival" VALUES ('2023-04-30', 4);
INSERT INTO "Festival" VALUES ('2023-05-01', 4);
INSERT INTO "Festival" VALUES ('2023-05-02', 4);
INSERT INTO "Festival" VALUES ('2023-05-03', 4);
INSERT INTO "Festival" VALUES ('2023-06-22', 5);
INSERT INTO "Festival" VALUES ('2023-06-23', 5);
INSERT INTO "Festival" VALUES ('2023-06-24', 5);
INSERT INTO "Festival" VALUES ('2023-09-29', 6);
INSERT INTO "Festival" VALUES ('2023-09-30', 6);
INSERT INTO "Festival" VALUES ('2023-10-01', 7);
INSERT INTO "Festival" VALUES ('2023-10-02', 7);
INSERT INTO "Festival" VALUES ('2023-10-03', 7);
INSERT INTO "Festival" VALUES ('2023-10-04', 7);
INSERT INTO "Festival" VALUES ('2023-10-05', 7);
INSERT INTO "Festival" VALUES ('2023-10-06', 7);

PRAGMA foreign_keys = true;
