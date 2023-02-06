/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 06/02/2023 16:58:57
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for FieldMeta
-- ----------------------------
DROP TABLE IF EXISTS "FieldMeta";
CREATE TABLE "FieldMeta" (
  "FieldName" TEXT NOT NULL,
  "TableName" TEXT NOT NULL,
  "DataType" TEXT,
  "PrimaryKey" integer,
  "ShowOnUI" integer,
  "SequenceID" integer,
  PRIMARY KEY ("FieldName", "TableName")
);

-- ----------------------------
-- Records of FieldMeta
-- ----------------------------
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 1, 0, 0);
INSERT INTO "FieldMeta" VALUES ('USStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 1);
INSERT INTO "FieldMeta" VALUES ('USDollarIndex', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 2);
INSERT INTO "FieldMeta" VALUES ('JapanKoreaStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 3);
INSERT INTO "FieldMeta" VALUES ('EropeStock', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 4);
INSERT INTO "FieldMeta" VALUES ('OilPrice', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 5);
INSERT INTO "FieldMeta" VALUES ('VIX', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 6);
INSERT INTO "FieldMeta" VALUES ('FEDMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 7);
INSERT INTO "FieldMeta" VALUES ('BuySellAndStrength', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 8);
INSERT INTO "FieldMeta" VALUES ('DomesticMessage', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 9);
INSERT INTO "FieldMeta" VALUES ('MarketSentiment', 'Trade_DailyMacroEnvironmentAnalyze', 'string', 0, 1, 10);
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Future_DailyTradeSummarize', 'string', 1, 0, 0);
INSERT INTO "FieldMeta" VALUES ('RightDeeds', 'Future_DailyTradeSummarize', 'string', 0, 1, 1);
INSERT INTO "FieldMeta" VALUES ('WrongDeeds', 'Future_DailyTradeSummarize', 'string', 0, 1, 2);
INSERT INTO "FieldMeta" VALUES ('MoneyBegin', 'Future_DailyTradeSummarize', 'float', 0, 1, 3);
INSERT INTO "FieldMeta" VALUES ('MoneyEnd', 'Future_DailyTradeSummarize', 'float', 0, 1, 4);
INSERT INTO "FieldMeta" VALUES ('Money2Bank', 'Future_DailyTradeSummarize', 'float', 0, 1, 5);
INSERT INTO "FieldMeta" VALUES ('MoneyFromBank', 'Future_DailyTradeSummarize', 'float', 0, 1, 6);
INSERT INTO "FieldMeta" VALUES ('GainLoss', 'Future_DailyTradeSummarize', 'float', 0, 1, 7);
INSERT INTO "FieldMeta" VALUES ('GainLossPercent', 'Future_DailyTradeSummarize', 'float', 0, 1, 8);
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Future_IntradayTrade', 'string', 1, 0, 0);
INSERT INTO "FieldMeta" VALUES ('FutureContractName', 'Future_IntradayTrade', 'string', 0, 1, 1);
INSERT INTO "FieldMeta" VALUES ('TradeTime', 'Future_IntradayTrade', 'string', 0, 1, 2);
INSERT INTO "FieldMeta" VALUES ('ExpectOrderTime', 'Future_IntradayTrade', 'int', 0, 1, 3);
INSERT INTO "FieldMeta" VALUES ('TradeType', 'Future_IntradayTrade', 'int', 0, 1, 4);
INSERT INTO "FieldMeta" VALUES ('OpenQuantity', 'Future_IntradayTrade', 'int', 0, 1, 5);
INSERT INTO "FieldMeta" VALUES ('OpenPrice', 'Future_IntradayTrade', 'float', 0, 1, 6);
INSERT INTO "FieldMeta" VALUES ('ExpectMaxLoss', 'Future_IntradayTrade', 'float', 0, 1, 7);
INSERT INTO "FieldMeta" VALUES ('ExpectIncomde', 'Future_IntradayTrade', 'float', 0, 1, 8);
INSERT INTO "FieldMeta" VALUES ('BuySellStrength', 'Future_IntradayTrade', 'string', 0, 1, 9);
INSERT INTO "FieldMeta" VALUES ('Reason4Transaction', 'Future_IntradayTrade', 'string', 0, 1, 10);
INSERT INTO "FieldMeta" VALUES ('Reason4Prevention', 'Future_IntradayTrade', 'string', 0, 1, 11);
INSERT INTO "FieldMeta" VALUES ('China_HKStockMarket01', 'Future_IntradayTrade', 'string', 0, 1, 12);
INSERT INTO "FieldMeta" VALUES ('FutureMarket01', 'Future_IntradayTrade', 'string', 0, 1, 13);
INSERT INTO "FieldMeta" VALUES ('FutureMACD01', 'Future_IntradayTrade', 'string', 0, 1, 14);
INSERT INTO "FieldMeta" VALUES ('FutureStrenthAngle01', 'Future_IntradayTrade', 'string', 0, 1, 15);
INSERT INTO "FieldMeta" VALUES ('FutureTRIX01', 'Future_IntradayTrade', 'string', 0, 1, 16);
INSERT INTO "FieldMeta" VALUES ('China_HKStockMarket02', 'Future_IntradayTrade', 'string', 0, 1, 17);
INSERT INTO "FieldMeta" VALUES ('FutureMarket02', 'Future_IntradayTrade', 'string', 0, 1, 18);
INSERT INTO "FieldMeta" VALUES ('FutureMACD02', 'Future_IntradayTrade', 'string', 0, 1, 19);
INSERT INTO "FieldMeta" VALUES ('FutureStrenthAngle02', 'Future_IntradayTrade', 'string', 0, 1, 20);
INSERT INTO "FieldMeta" VALUES ('FutureTRIX02', 'Future_IntradayTrade', 'string', 0, 1, 21);
INSERT INTO "FieldMeta" VALUES ('ClosePositionReason', 'Future_IntradayTrade', 'string', 0, 1, 22);
INSERT INTO "FieldMeta" VALUES ('PreventClosePositionReason', 'Future_IntradayTrade', 'string', 0, 1, 23);
INSERT INTO "FieldMeta" VALUES ('ClosePositionTime', 'Future_IntradayTrade', 'string', 0, 1, 24);
INSERT INTO "FieldMeta" VALUES ('CloseQuantity', 'Future_IntradayTrade', 'int', 0, 1, 25);
INSERT INTO "FieldMeta" VALUES ('ClosePrice', 'Future_IntradayTrade', 'float', 0, 1, 26);
INSERT INTO "FieldMeta" VALUES ('GainLoss', 'Future_IntradayTrade', 'float', 0, 1, 27);
INSERT INTO "FieldMeta" VALUES ('HighestPrice', 'Future_IntradayTrade', 'float', 0, 1, 28);
INSERT INTO "FieldMeta" VALUES ('LowestPricePrice', 'Future_IntradayTrade', 'float', 0, 1, 29);
INSERT INTO "FieldMeta" VALUES ('TransactionQuality', 'Future_IntradayTrade', 'float', 0, 1, 30);
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Future_DailyLongPeriodicAnalyze', 'string', 1, 0, 0);
INSERT INTO "FieldMeta" VALUES ('FutureContractName', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 1);
INSERT INTO "FieldMeta" VALUES ('MainForceChange', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 2);
INSERT INTO "FieldMeta" VALUES ('CITIC_GoldmanSachs', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 3);
INSERT INTO "FieldMeta" VALUES ('ExternalMarket', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 4);
INSERT INTO "FieldMeta" VALUES ('MonthlyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 5);
INSERT INTO "FieldMeta" VALUES ('WeeklyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 6);
INSERT INTO "FieldMeta" VALUES ('DailyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 7);
INSERT INTO "FieldMeta" VALUES ('4HTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 8);
INSERT INTO "FieldMeta" VALUES ('EmotionalFeeling', 'Future_DailyLongPeriodicAnalyze', 'string', 0, 1, 9);
INSERT INTO "FieldMeta" VALUES ('TradeDay', 'Future_DailyMinorCycleAnalyze', 'string', 1, 0, 0);
INSERT INTO "FieldMeta" VALUES ('FutureContractName', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 1);
INSERT INTO "FieldMeta" VALUES ('RecordTime', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 2);
INSERT INTO "FieldMeta" VALUES ('15MIN_MACD', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 3);
INSERT INTO "FieldMeta" VALUES ('1H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 4);
INSERT INTO "FieldMeta" VALUES ('4H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 5);
INSERT INTO "FieldMeta" VALUES ('15MIN_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 6);
INSERT INTO "FieldMeta" VALUES ('1H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 7);
INSERT INTO "FieldMeta" VALUES ('4H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 8);
INSERT INTO "FieldMeta" VALUES ('15MIN_Trix', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 9);
INSERT INTO "FieldMeta" VALUES ('1H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 10);
INSERT INTO "FieldMeta" VALUES ('4H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 11);
INSERT INTO "FieldMeta" VALUES ('15MIN_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 12);
INSERT INTO "FieldMeta" VALUES ('1H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 13);
INSERT INTO "FieldMeta" VALUES ('4H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 14);
INSERT INTO "FieldMeta" VALUES ('15MIN_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 15);
INSERT INTO "FieldMeta" VALUES ('1H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 16);
INSERT INTO "FieldMeta" VALUES ('4H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 17);
INSERT INTO "FieldMeta" VALUES ('MainSignal', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 18);
INSERT INTO "FieldMeta" VALUES ('Inclination', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 19);
INSERT INTO "FieldMeta" VALUES ('EmotionFeeling', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 20);
INSERT INTO "FieldMeta" VALUES ('Forecast', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 21);
INSERT INTO "FieldMeta" VALUES ('Reality', 'Future_DailyMinorCycleAnalyze', 'string', 0, 1, 22);

PRAGMA foreign_keys = true;
