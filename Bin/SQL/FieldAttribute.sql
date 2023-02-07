/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 07/02/2023 10:34:09
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
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MoneyBegin', 'Future_DailyTradeSummarize', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MoneyEnd', 'Future_DailyTradeSummarize', 'string', '结束时资金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MoneyEnd', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MoneyEnd', 'Future_DailyTradeSummarize', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Money2Bank', 'Future_DailyTradeSummarize', 'string', '出金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Money2Bank', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Money2Bank', 'Future_DailyTradeSummarize', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MoneyFromBank', 'Future_DailyTradeSummarize', 'string', '入金数额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MoneyFromBank', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MoneyFromBank', 'Future_DailyTradeSummarize', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'GainLoss', 'Future_DailyTradeSummarize', 'string', '盈利(亏损)金额');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'GainLoss', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'GainLoss', 'Future_DailyTradeSummarize', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'GainLossPercent', 'Future_DailyTradeSummarize', 'string', '盈亏百分比');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'GainLossPercent', 'Future_DailyTradeSummarize', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'GainLossPercent', 'Future_DailyTradeSummarize', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeDay', 'Future_IntradayTrade', 'string', '交易日');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeDay', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeDay', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureContractName', 'Future_IntradayTrade', 'string', '合约名称');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureContractName', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureContractName', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeTime', 'Future_IntradayTrade', 'string', '下单时间');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeTime', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeTime', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ExpectOrderTime', 'Future_IntradayTrade', 'string', '预期交易时长');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ExpectOrderTime', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ExpectOrderTime', 'Future_IntradayTrade', 'string', '%d');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeType', 'Future_IntradayTrade', 'string', '交易类型');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeType', 'Future_IntradayTrade', 'string', 'CComBox');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeType', 'Future_IntradayTrade', 'string', '%d');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'OpenQuantity', 'Future_IntradayTrade', 'string', '开仓仓位');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'OpenQuantity', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'OpenQuantity', 'Future_IntradayTrade', 'string', '%d');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'OpenPrice', 'Future_IntradayTrade', 'string', '开仓价格');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'OpenPrice', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'OpenPrice', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ExpectMaxLoss', 'Future_IntradayTrade', 'string', '预期最大损失');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ExpectMaxLoss', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ExpectMaxLoss', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ExpectIncomde', 'Future_IntradayTrade', 'string', '预期收益');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ExpectIncomde', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ExpectIncomde', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'BuySellStrength', 'Future_IntradayTrade', 'string', '多空强度');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'BuySellStrength', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'BuySellStrength', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Reason4Transaction', 'Future_IntradayTrade', 'string', '交易理由');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Reason4Transaction', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Reason4Transaction', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Reason4Prevention', 'Future_IntradayTrade', 'string', '阻止理由');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Reason4Prevention', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Reason4Prevention', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'China_HKStockMarket01', 'Future_IntradayTrade', 'string', 'A股/香港股市行情');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'China_HKStockMarket01', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'China_HKStockMarket01', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureMarket01', 'Future_IntradayTrade', 'string', '期货市场整体行情');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureMarket01', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureMarket01', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureMACD01', 'Future_IntradayTrade', 'string', 'MACD');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureMACD01', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureMACD01', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureStrenthAngle01', 'Future_IntradayTrade', 'string', '角度和力量');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureStrenthAngle01', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureStrenthAngle01', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureTRIX01', 'Future_IntradayTrade', 'string', 'Trix');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureTRIX01', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureTRIX01', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'China_HKStockMarket02', 'Future_IntradayTrade', 'string', 'A股/香港股市行情');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'China_HKStockMarket02', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'China_HKStockMarket02', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureMarket02', 'Future_IntradayTrade', 'string', '期货市场整体行情');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureMarket02', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureMarket02', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureMACD02', 'Future_IntradayTrade', 'string', 'MACD');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureMACD02', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureMACD02', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureStrenthAngle02', 'Future_IntradayTrade', 'string', '角度和力量');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureStrenthAngle02', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureStrenthAngle02', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureTRIX02', 'Future_IntradayTrade', 'string', 'Trix');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureTRIX02', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureTRIX02', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ClosePositionReason', 'Future_IntradayTrade', 'string', '平仓理由');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ClosePositionReason', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ClosePositionReason', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'PreventClosePositionReason', 'Future_IntradayTrade', 'string', '阻止平仓理由');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'PreventClosePositionReason', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'PreventClosePositionReason', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ClosePositionTime', 'Future_IntradayTrade', 'string', '平仓时间');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ClosePositionTime', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ClosePositionTime', 'Future_IntradayTrade', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'CloseQuantity', 'Future_IntradayTrade', 'string', '平仓仓位');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'CloseQuantity', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'CloseQuantity', 'Future_IntradayTrade', 'string', '%d');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ClosePrice', 'Future_IntradayTrade', 'string', '平仓价格');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ClosePrice', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ClosePrice', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'GainLoss', 'Future_IntradayTrade', 'string', '盈亏');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'GainLoss', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'GainLoss', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'HighestPrice', 'Future_IntradayTrade', 'string', '区间最高价格');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'HighestPrice', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'HighestPrice', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'LowestPricePrice', 'Future_IntradayTrade', 'string', '区间最低价格');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'LowestPricePrice', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'LowestPricePrice', 'Future_IntradayTrade', 'string', '%.1f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TransactionQuality', 'Future_IntradayTrade', 'string', '交易质量');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TransactionQuality', 'Future_IntradayTrade', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TransactionQuality', 'Future_IntradayTrade', 'string', '%.2f');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeDay', 'Future_DailyMinorCycleAnalyze', 'string', '交易日');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeDay', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeDay', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureContractName', 'Future_DailyMinorCycleAnalyze', 'string', '合约名称');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureContractName', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureContractName', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'RecordTime', 'Future_DailyMinorCycleAnalyze', 'string', '记录时间');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'RecordTime', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'RecordTime', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '15MIN_MACD', 'Future_DailyMinorCycleAnalyze', 'string', '15分钟MACD');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '15MIN_MACD', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '15MIN_MACD', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '1H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', '1小时MACD');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '1H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '1H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '4H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', '4小时MACD');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '4H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '4H_MACD', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '15MIN_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', '15分钟MACD柱');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '15MIN_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '15MIN_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '1H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', '1小时MACD柱');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '1H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '1H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '4H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', '4小时MACD柱');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '4H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '4H_MACDPillar', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '15MIN_Trix', 'Future_DailyMinorCycleAnalyze', 'string', '15分钟Trix');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '15MIN_Trix', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '15MIN_Trix', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '1H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', '1小时Trix');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '1H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '1H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '4H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', '4小时Trix');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '4H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '4H_Trix', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '15MIN_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', '15分钟SKDJ');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '15MIN_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '15MIN_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '1H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', '1小时SKDJ');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '1H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '1H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '4H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', '4小时SKDJ');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '4H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '4H_SKDJ', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '15MIN_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', '15分钟趋势角度');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '15MIN_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '15MIN_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '1H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', '1小时趋势角度');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '1H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '1H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '4H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', '4小时趋势角度');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '4H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '4H_StrengthAngle', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MainSignal', 'Future_DailyMinorCycleAnalyze', 'string', '主要信号');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MainSignal', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MainSignal', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Inclination', 'Future_DailyMinorCycleAnalyze', 'string', '倾向和意愿');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Inclination', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Inclination', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'EmotionFeeling', 'Future_DailyMinorCycleAnalyze', 'string', '情绪感受');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'EmotionFeeling', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'EmotionFeeling', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Forecast', 'Future_DailyMinorCycleAnalyze', 'string', '预测');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Forecast', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Forecast', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'Reality', 'Future_DailyMinorCycleAnalyze', 'string', '实际情况');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'Reality', 'Future_DailyMinorCycleAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'Reality', 'Future_DailyMinorCycleAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'TradeDay', 'Future_DailyLongPeriodicAnalyze', 'string', '交易日');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'TradeDay', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'TradeDay', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'FutureContractName', 'Future_DailyLongPeriodicAnalyze', 'string', '合约名称');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'FutureContractName', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'FutureContractName', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MainForceChange', 'Future_DailyLongPeriodicAnalyze', 'string', '主力增减仓');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MainForceChange', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MainForceChange', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'CITIC_GoldmanSachs', 'Future_DailyLongPeriodicAnalyze', 'string', '中信/乾坤/国泰');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'CITIC_GoldmanSachs', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'CITIC_GoldmanSachs', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'ExternalMarket', 'Future_DailyLongPeriodicAnalyze', 'string', '外围市场');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'ExternalMarket', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'ExternalMarket', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'MonthlyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '月线趋势');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'MonthlyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'MonthlyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'WeeklyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '周线趋势');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'WeeklyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'WeeklyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'DailyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '日线趋势');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'DailyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'DailyTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', '4HTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '4H线趋势');
INSERT INTO "FieldAttribute" VALUES ('ControlType', '4HTrend', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', '4HTrend', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');
INSERT INTO "FieldAttribute" VALUES ('Meaning', 'EmotionalFeeling', 'Future_DailyLongPeriodicAnalyze', 'string', '情绪感受');
INSERT INTO "FieldAttribute" VALUES ('ControlType', 'EmotionalFeeling', 'Future_DailyLongPeriodicAnalyze', 'string', 'CEdit');
INSERT INTO "FieldAttribute" VALUES ('DisplayType', 'EmotionalFeeling', 'Future_DailyLongPeriodicAnalyze', 'string', '%s');

PRAGMA foreign_keys = true;
