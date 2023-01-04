/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 04/01/2023 21:28:56
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for FieldAttribute
-- ----------------------------
DROP TABLE IF EXISTS "FieldAttribute";
CREATE TABLE "FieldAttribute" (
  "FieldAttributeName" TEXT NOT NULL,
  "FieldName" TEXT NOT NULL,
  "TableName" TEXT NOT NULL,
  "DataType" TEXT(255),
  "AttributeValue" TEXT(255),
  PRIMARY KEY ("FieldAttributeName", "FieldName", "TableName")
);

-- ----------------------------
-- Records of FieldAttribute
-- ----------------------------
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeDay', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '交易日');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeDay', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeDay', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'USStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '美股(涨跌)');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'USStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'USStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'USDollarIndex', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '美元指数(涨跌)');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'USDollarIndex', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'USDollarIndex', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'JapanKoreaStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '日韩股市(涨跌)');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'JapanKoreaStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'JapanKoreaStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'EropeStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '欧洲股市(涨跌)');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'EropeStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'EropeStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'OilPrice', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '原油价格(涨跌)');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'OilPrice', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'OilPrice', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'VIX', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'VIX指数(涨跌)');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'VIX', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'VIX', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FEDMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '美联储消息');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FEDMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FEDMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'BuySellAndStrength', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '多空以及强度');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'BuySellAndStrength', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'BuySellAndStrength', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'DomesticMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '国内消息');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'DomesticMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'DomesticMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MarketSentiment', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '市场情绪');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MarketSentiment', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MarketSentiment', 'Trade_DailyMacroEnvironmentAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeDay', 'Future_DailyTradeSummarize', 'string', '交易日');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeDay', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeDay', 'Future_DailyTradeSummarize', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'RightDeeds', 'Future_DailyTradeSummarize', 'string', '得');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'RightDeeds', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'RightDeeds', 'Future_DailyTradeSummarize', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'WrongDeeds', 'Future_DailyTradeSummarize', 'string', '失');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'WrongDeeds', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'WrongDeeds', 'Future_DailyTradeSummarize', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MoneyBegin', 'Future_DailyTradeSummarize', 'string', '起始时资金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MoneyBegin', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MoneyBegin', 'Future_DailyTradeSummarize', 'float', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MoneyEnd', 'Future_DailyTradeSummarize', 'string', '结束时资金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MoneyEnd', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MoneyEnd', 'Future_DailyTradeSummarize', 'float', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Money2Bank', 'Future_DailyTradeSummarize', 'string', '出金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Money2Bank', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Money2Bank', 'Future_DailyTradeSummarize', 'float', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MoneyFromBank', 'Future_DailyTradeSummarize', 'string', '入金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MoneyFromBank', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MoneyFromBank', 'Future_DailyTradeSummarize', 'float', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'GainLoss', 'Future_DailyTradeSummarize', 'string', '盈利(亏损)金额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'GainLoss', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'GainLoss', 'Future_DailyTradeSummarize', 'float', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'GainLossPercent', 'Future_DailyTradeSummarize', 'string', '盈亏百分比');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'GainLossPercent', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'GainLossPercent', 'Future_DailyTradeSummarize', 'float', '%.1f');

PRAGMA foreign_keys = true;
