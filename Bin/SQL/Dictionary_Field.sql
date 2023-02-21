/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 21/02/2023 11:24:34
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for Dictionary_Field
-- ----------------------------
DROP TABLE IF EXISTS "Dictionary_Field";
CREATE TABLE "Dictionary_Field" (
  "Value" integer,
  "Subject" TEXT,
  "Meaning" TEXT,
  "Translation" TEXT
);

-- ----------------------------
-- Records of Dictionary_Field
-- ----------------------------
INSERT INTO "Dictionary_Field" VALUES (0, 'Festival', 'NewYear', '元旦');
INSERT INTO "Dictionary_Field" VALUES (1, 'Festival', 'SpringFestival', '春节');
INSERT INTO "Dictionary_Field" VALUES (2, 'Festival', 'Qingming', '清明节');
INSERT INTO "Dictionary_Field" VALUES (3, 'Festival', 'LaborDay', '五一');
INSERT INTO "Dictionary_Field" VALUES (4, 'Festival', 'DragonBoatFestival', '端午节');
INSERT INTO "Dictionary_Field" VALUES (5, 'Festival', 'Mid-AutumnFestival', '中秋节');
INSERT INTO "Dictionary_Field" VALUES (6, 'Festival', 'NationalDay', '国庆节');
INSERT INTO "Dictionary_Field" VALUES (0, 'Judgement', 'Absolutely right', '完全正确');
INSERT INTO "Dictionary_Field" VALUES (1, 'Judgement', 'roughly right', '基本正确');
INSERT INTO "Dictionary_Field" VALUES (2, 'Judgement', 'Something Right,Something Wrong', '对错各半');
INSERT INTO "Dictionary_Field" VALUES (3, 'Judgement', 'Wrong', '完全错误');
INSERT INTO "Dictionary_Field" VALUES (0, 'EmotionFeeling', 'Relaxed', '心态放松');
INSERT INTO "Dictionary_Field" VALUES (1, 'EmotionFeeling', 'Hopeful', '期待');
INSERT INTO "Dictionary_Field" VALUES (2, 'EmotionFeeling', 'Cheerful', '愉悦');
INSERT INTO "Dictionary_Field" VALUES (3, 'EmotionFeeling', 'Excited', '兴奋');
INSERT INTO "Dictionary_Field" VALUES (4, 'EmotionFeeling', 'Doubtful', '疑虑');
INSERT INTO "Dictionary_Field" VALUES (5, 'EmotionFeeling', 'Afraid', '紧张，担心害怕，纠结');
INSERT INTO "Dictionary_Field" VALUES (6, 'EmotionFeeling', 'Painful', '感到痛苦');
INSERT INTO "Dictionary_Field" VALUES (7, 'EmotionFeeling', 'BitterlyPainful ', '极度痛苦，不堪回首');
INSERT INTO "Dictionary_Field" VALUES (1, 'TradeType', 'Buy', '买入');
INSERT INTO "Dictionary_Field" VALUES (0, 'TradeType', 'Sell', '卖出');
INSERT INTO "Dictionary_Field" VALUES (0, 'TimePeriod', '5Min', '5分钟');
INSERT INTO "Dictionary_Field" VALUES (1, 'TimePeriod', '15Min', '15分钟');
INSERT INTO "Dictionary_Field" VALUES (2, 'TimePeriod', '1H', '1小时');
INSERT INTO "Dictionary_Field" VALUES (3, 'TimePeriod', '2H', '2小时');
INSERT INTO "Dictionary_Field" VALUES (4, 'TimePeriod', '4H', '4小时');
INSERT INTO "Dictionary_Field" VALUES (5, 'TimePeriod', '1Day', '日');
INSERT INTO "Dictionary_Field" VALUES (6, 'TimePeriod', 'Week', '周');
INSERT INTO "Dictionary_Field" VALUES (7, 'TimePeriod', 'Month', '月');
INSERT INTO "Dictionary_Field" VALUES (0, 'TechnicalIndex', 'MACD', 'MACD');
INSERT INTO "Dictionary_Field" VALUES (1, 'TechnicalIndex', 'MACDPillar', 'MACD柱');
INSERT INTO "Dictionary_Field" VALUES (2, 'TechnicalIndex', 'Trix', 'Trix');
INSERT INTO "Dictionary_Field" VALUES (3, 'TechnicalIndex', 'SKDJ', 'SKDJ');
INSERT INTO "Dictionary_Field" VALUES (4, 'TechnicalIndex', 'StrengthAngle', '角度和能量');
INSERT INTO "Dictionary_Field" VALUES (5, 'TechnicalIndex', 'PriceFluctuating', '价格波动');
INSERT INTO "Dictionary_Field" VALUES (6, 'TechnicalIndex', 'Deviation', '背离');
INSERT INTO "Dictionary_Field" VALUES (7, 'TechnicalIndex', 'RedGreenKRatio', '红绿K线比');
INSERT INTO "Dictionary_Field" VALUES (8, 'TechnicalIndex', 'RiseFallExtent', '涨跌幅度');
INSERT INTO "Dictionary_Field" VALUES (9, 'TechnicalIndex', 'TradeVolume', '成交量');
INSERT INTO "Dictionary_Field" VALUES (0, 'MACD', 'InclineUpward_Weak', '倾斜弱向上');
INSERT INTO "Dictionary_Field" VALUES (1, 'MACD', 'InclineUpward_Normal', '倾斜中等角度向上');
INSERT INTO "Dictionary_Field" VALUES (2, 'MACD', 'InclineUpward_Powerful', '倾斜大角度向上');
INSERT INTO "Dictionary_Field" VALUES (3, 'MACD', 'InclineDown_Weak', '倾斜弱向下');
INSERT INTO "Dictionary_Field" VALUES (4, 'MACD', 'InclineDown_Normal', '倾斜中等角度向下');
INSERT INTO "Dictionary_Field" VALUES (5, 'MACD', 'InclineDown_Powerful', '倾斜大角度向下');
INSERT INTO "Dictionary_Field" VALUES (0, 'MACDPillar', '1Wave_Pillar_Red', '1波红柱进行中');
INSERT INTO "Dictionary_Field" VALUES (1, 'MACDPillar', '2Wave_Pillar_Red', '2波红柱进行中');
INSERT INTO "Dictionary_Field" VALUES (2, 'MACDPillar', '3Wave_40Pillar_Red', '3波40根红柱，可能要走绿柱');
INSERT INTO "Dictionary_Field" VALUES (3, 'MACDPillar', '3Wave_40Pillar_Red', '3波40根红柱后，开始走绿柱行情');
INSERT INTO "Dictionary_Field" VALUES (4, 'MACDPillar', '3Wave_40Pillar_Red', '3波40根红柱后，绿柱行情进行中');
INSERT INTO "Dictionary_Field" VALUES (5, 'MACDPillar', '4Wave_48Pillar_Red', '4波48根红柱，大概率要走绿柱行情');
INSERT INTO "Dictionary_Field" VALUES (6, 'MACDPillar', '4Wave_48Pillar_Red', '4波48根红柱，开始走绿柱行情');
INSERT INTO "Dictionary_Field" VALUES (7, 'MACDPillar', '4Wave_48Pillar_Red', '4波48根红柱，绿柱行情进行中');
INSERT INTO "Dictionary_Field" VALUES (8, 'MACDPillar', '5Wave_56Pillar_Red', '5波56根红柱，即将走绿柱行情');
INSERT INTO "Dictionary_Field" VALUES (9, 'MACDPillar', '5Wave_56Pillar_Red', '5波56根红柱，开始走绿柱行情');
INSERT INTO "Dictionary_Field" VALUES (10, 'MACDPillar', '5Wave_56Pillar_Red', '5波56根红柱，绿柱行情进行中');
INSERT INTO "Dictionary_Field" VALUES (11, 'MACDPillar', '1Wave_Pillar_Green', '1波绿柱进行中');
INSERT INTO "Dictionary_Field" VALUES (12, 'MACDPillar', '2Wave_Pillar_Green', '2波绿柱进行中');
INSERT INTO "Dictionary_Field" VALUES (13, 'MACDPillar', '3Wave_40Pillar_Green', '3波40根绿柱，可能要走红柱');
INSERT INTO "Dictionary_Field" VALUES (14, 'MACDPillar', '3Wave_40Pillar_Green', '3波40根绿柱，开始走红柱行情');
INSERT INTO "Dictionary_Field" VALUES (15, 'MACDPillar', '3Wave_40Pillar_Green', '3波40根绿柱，绿柱行情进行中');
INSERT INTO "Dictionary_Field" VALUES (16, 'MACDPillar', '4Wave_48Pillar_Green', '4波48根绿柱，大概率要走红柱行情');
INSERT INTO "Dictionary_Field" VALUES (17, 'MACDPillar', '4Wave_48Pillar_Green', '4波48根绿柱，开始走红柱行情');
INSERT INTO "Dictionary_Field" VALUES (18, 'MACDPillar', '4Wave_48Pillar_Green', '4波48根绿柱，绿柱行情进行中');
INSERT INTO "Dictionary_Field" VALUES (19, 'MACDPillar', '5Wave_56Pillar_Green', '5波56根绿柱，即将走红柱行情');
INSERT INTO "Dictionary_Field" VALUES (20, 'MACDPillar', '5Wave_56Pillar_Green', '5波56根绿柱，开始走红柱行情');
INSERT INTO "Dictionary_Field" VALUES (21, 'MACDPillar', '5Wave_56Pillar_Green', '5波56根绿柱，红柱行情进行中');
INSERT INTO "Dictionary_Field" VALUES (0, 'Trix', 'CrossedUpward_Weak', '已小角度向上交叉');
INSERT INTO "Dictionary_Field" VALUES (1, 'Trix', 'CrossedUpward_Noraml', '已中等角度向上交叉');
INSERT INTO "Dictionary_Field" VALUES (2, 'Trix', 'CrossedUpward_Powerful', '已大角度向上交叉');
INSERT INTO "Dictionary_Field" VALUES (3, 'Trix', 'CrossedDown_Weak', '已小角度向下交叉');
INSERT INTO "Dictionary_Field" VALUES (4, 'Trix', 'CrossedDown_Noraml', '已中等角度向下交叉');
INSERT INTO "Dictionary_Field" VALUES (5, 'Trix', 'CrossedDown_Powerful', '已大角度向下交叉');
INSERT INTO "Dictionary_Field" VALUES (6, 'Trix', 'GapIncreasing', '敞开变大');
INSERT INTO "Dictionary_Field" VALUES (7, 'Trix', 'GapDecreasing', '敞口减小');
INSERT INTO "Dictionary_Field" VALUES (8, 'Trix', 'GapCrossing', '即将交叉');
INSERT INTO "Dictionary_Field" VALUES (9, 'Trix', 'GapMaximizing', '敞口最大');
INSERT INTO "Dictionary_Field" VALUES (0, 'SKDJ', '2Bottom', '2次探底');
INSERT INTO "Dictionary_Field" VALUES (1, 'SKDJ', '3Bottom', '3次探底');
INSERT INTO "Dictionary_Field" VALUES (2, 'SKDJ', '2Top', '2次冲顶');
INSERT INTO "Dictionary_Field" VALUES (3, 'SKDJ', '3Top', '3次冲顶');
INSERT INTO "Dictionary_Field" VALUES (4, 'SKDJ', 'WanderingInTop', '顶部徘徊');
INSERT INTO "Dictionary_Field" VALUES (5, 'SKDJ', 'WanderingInBottom', '底部徘徊');
INSERT INTO "Dictionary_Field" VALUES (0, 'StrengthAngle', 'K_Rise_Small', 'K线小角度上涨');
INSERT INTO "Dictionary_Field" VALUES (1, 'StrengthAngle', 'K_Rise_Medium', 'K线中等角度上涨');
INSERT INTO "Dictionary_Field" VALUES (2, 'StrengthAngle', 'K_Rise_Large', 'K线大角度上涨');
INSERT INTO "Dictionary_Field" VALUES (3, 'StrengthAngle', 'K_Fall_Small', 'K线小角度下跌');
INSERT INTO "Dictionary_Field" VALUES (4, 'StrengthAngle', 'K_Fall_Medium', 'K线中等角度下跌');
INSERT INTO "Dictionary_Field" VALUES (5, 'StrengthAngle', 'K_Fall_Large', 'K线大角度下跌');
INSERT INTO "Dictionary_Field" VALUES (0, 'PriceFluctuating', 'K_Rising_Small', '小角度上涨');
INSERT INTO "Dictionary_Field" VALUES (1, 'PriceFluctuating', 'K_Rising_Medium', '中等角度上涨');
INSERT INTO "Dictionary_Field" VALUES (2, 'PriceFluctuating', 'K_Rising_Large', '大角度上涨');
INSERT INTO "Dictionary_Field" VALUES (3, 'PriceFluctuating', 'K_Falling_Small', '小角度下跌');
INSERT INTO "Dictionary_Field" VALUES (4, 'PriceFluctuating', 'K_Falling_Medium', '中等角度下跌');
INSERT INTO "Dictionary_Field" VALUES (5, 'PriceFluctuating', 'K_Falling_Large', '大角度下跌');
INSERT INTO "Dictionary_Field" VALUES (6, 'PriceFluctuating', 'OscillatingAfterRise', '上涨后震荡');
INSERT INTO "Dictionary_Field" VALUES (7, 'PriceFluctuating', 'OscillatingAfterFall', '下跌后震荡');
INSERT INTO "Dictionary_Field" VALUES (8, 'PriceFluctuating', 'SlightFallAfterRiseDramatically', '大角度上涨后小幅度下跌');
INSERT INTO "Dictionary_Field" VALUES (9, 'PriceFluctuating', 'SlightRiseAfterFallSharply', '大角度下跌后小幅度上涨');
INSERT INTO "Dictionary_Field" VALUES (0, 'Deviation', '3GreenMACDPillar,RecordLowPriceButIndexValueNot', '3波MACD绿柱，指标未创新低而价格创新低');
INSERT INTO "Dictionary_Field" VALUES (1, 'Deviation', '4GreenMACDPillar,RecordLowPriceButIndexValueNot', '4波MACD绿柱，指标未创新低而价格创新低');
INSERT INTO "Dictionary_Field" VALUES (2, 'Deviation', '5GreenMACDPillar,RecordLowPriceButIndexValueNot', '5波MACD绿柱，指标未创新低而价格创新低');
INSERT INTO "Dictionary_Field" VALUES (3, 'Deviation', '3RedMACDPillar,RecordHighPriceButIndexValueNot', '3波MACD红柱，指标未创新高而价格创新高');
INSERT INTO "Dictionary_Field" VALUES (4, 'Deviation', '4RedMACDPillar,RecordHighPriceButIndexValueNot', '4波MACD红柱，指标未创新高而价格创新高');
INSERT INTO "Dictionary_Field" VALUES (5, 'Deviation', '5RedMACDPillar,RecordHighPriceButIndexValueNot', '5波MACD红柱，指标未创新高而价格创新高');
INSERT INTO "Dictionary_Field" VALUES (0, 'RedGreenKRatio', '2RedVS1Green', '红绿K数2:1,上涨行情');
INSERT INTO "Dictionary_Field" VALUES (1, 'RedGreenKRatio', '3RedVS1Green', '红绿K数3:1,上涨行情');
INSERT INTO "Dictionary_Field" VALUES (2, 'RedGreenKRatio', '4RedVS1Green', '红绿K数4:1,强上涨行情');
INSERT INTO "Dictionary_Field" VALUES (3, 'RedGreenKRatio', '5RedVS1Green', '红绿K数5:1,极强上涨行情');
INSERT INTO "Dictionary_Field" VALUES (4, 'RedGreenKRatio', '6RedVS1Green', '红绿K数6:1,极强上涨行情');
INSERT INTO "Dictionary_Field" VALUES (5, 'RedGreenKRatio', '1RedVS2Green', '红绿K数1:2,下跌行情');
INSERT INTO "Dictionary_Field" VALUES (6, 'RedGreenKRatio', '1RedVS3Green', '红绿K数1:3,下跌行情');
INSERT INTO "Dictionary_Field" VALUES (7, 'RedGreenKRatio', '1RedVS4Green', '红绿K数1:4,强下跌行情');
INSERT INTO "Dictionary_Field" VALUES (8, 'RedGreenKRatio', '1RedVS5Green', '红绿K数1:5,极强下跌行情');
INSERT INTO "Dictionary_Field" VALUES (9, 'RedGreenKRatio', '1RedVS6Green', '红绿K数1:6,极强下跌行情');
INSERT INTO "Dictionary_Field" VALUES (0, 'RiseFallExtent', 'DecreaseBy6%', '下跌6%');
INSERT INTO "Dictionary_Field" VALUES (1, 'RiseFallExtent', 'DecreaseBy9%', '下跌9%');
INSERT INTO "Dictionary_Field" VALUES (2, 'RiseFallExtent', 'DecreaseBy12%', '下跌12%');
INSERT INTO "Dictionary_Field" VALUES (3, 'RiseFallExtent', 'IncreaseBy6%', '上涨6%');
INSERT INTO "Dictionary_Field" VALUES (4, 'RiseFallExtent', 'IncreaseBy9%', '上涨9%');
INSERT INTO "Dictionary_Field" VALUES (5, 'RiseFallExtent', 'IncreaseBy12%', '上涨12%');
INSERT INTO "Dictionary_Field" VALUES (0, 'TradeVolume', 'Average*2', '2倍');
INSERT INTO "Dictionary_Field" VALUES (1, 'TradeVolume', 'Average*3', '3倍');
INSERT INTO "Dictionary_Field" VALUES (2, 'TradeVolume', 'Average*4', '4倍');
INSERT INTO "Dictionary_Field" VALUES (0, 'Conflict', 'PositionDirectionvsMonetaryTighteningOrMonetaryLoosening', '持仓方向与货币宽松/紧缩环境(预期)');
INSERT INTO "Dictionary_Field" VALUES (1, 'Conflict', 'PositionDirectionvsMarketTemperature', '持仓方向与市场温度');
INSERT INTO "Dictionary_Field" VALUES (2, 'Conflict', 'ShortPeriodvcLongPeriod', '短周期与长周期');
INSERT INTO "Dictionary_Field" VALUES (3, 'Conflict', 'PricevsTechnicalIndex', '价格与技术指标');
INSERT INTO "Dictionary_Field" VALUES (4, 'Conflict', 'TechnicalIndexvsEnergy', '技术指标与能量');
INSERT INTO "Dictionary_Field" VALUES (0, 'Inclination', 'Easy2RiseHard2Fall', '易涨难跌');
INSERT INTO "Dictionary_Field" VALUES (1, 'Inclination', 'Easy2FallHard2Rise', '易跌难涨');
INSERT INTO "Dictionary_Field" VALUES (2, 'Inclination', 'RiseSteadily', '持续稳步上涨');
INSERT INTO "Dictionary_Field" VALUES (3, 'Inclination', 'RiseSharply', '快速大幅上涨');
INSERT INTO "Dictionary_Field" VALUES (4, 'Inclination', 'FallSteadily', '暴跌后持续阴跌');
INSERT INTO "Dictionary_Field" VALUES (5, 'Inclination', 'FallSharply', '暴跌进行中');

PRAGMA foreign_keys = true;
