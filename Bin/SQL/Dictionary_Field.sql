/*
 Navicat SQLite Data Transfer

 Source Server         : TradeTrack
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 16/02/2023 10:02:23
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
INSERT INTO "Dictionary_Field" VALUES (7, 'TechnicalIndex', 'RiseFallExtent', '涨跌幅度');
INSERT INTO "Dictionary_Field" VALUES (8, 'TechnicalIndex', 'TradeVolume', '成交量');
INSERT INTO "Dictionary_Field" VALUES (0, 'MACD', 'InclineUpward', '倾斜向上');
INSERT INTO "Dictionary_Field" VALUES (1, 'MACD', 'InclineDown', '倾斜向下');
INSERT INTO "Dictionary_Field" VALUES (0, 'MACDPillar', '3Wave_40Pillar_Red', '3波40根红柱');
INSERT INTO "Dictionary_Field" VALUES (1, 'MACDPillar', '4Wave_45Pillar_Red', '4波48根红柱');
INSERT INTO "Dictionary_Field" VALUES (2, 'MACDPillar', '5Wave_50Pillar_Red', '5波58根红柱');
INSERT INTO "Dictionary_Field" VALUES (3, 'MACDPillar', '3Wave_40Pillar_Green', '3波40根绿柱');
INSERT INTO "Dictionary_Field" VALUES (4, 'MACDPillar', '4Wave_45Pillar_Green', '4波48根绿柱');
INSERT INTO "Dictionary_Field" VALUES (5, 'MACDPillar', '5Wave_50Pillar_Green', '5波58根绿柱');
INSERT INTO "Dictionary_Field" VALUES (0, 'Trix', 'CrossedUpward', '已向上交叉');
INSERT INTO "Dictionary_Field" VALUES (1, 'Trix', 'CrossedDown', '已向下交叉');
INSERT INTO "Dictionary_Field" VALUES (2, 'Trix', 'GapIncreasing', '敞开变大');
INSERT INTO "Dictionary_Field" VALUES (3, 'Trix', 'GapDecreasing', '敞口减小');
INSERT INTO "Dictionary_Field" VALUES (4, 'Trix', 'GapCrossing', '即将交叉');
INSERT INTO "Dictionary_Field" VALUES (5, 'Trix', 'GapMaximizing', '敞口最大');
INSERT INTO "Dictionary_Field" VALUES (0, 'SKDJ', '2Bottom', '2次探底');
INSERT INTO "Dictionary_Field" VALUES (1, 'SKDJ', '3Bottom', '3次探底');
INSERT INTO "Dictionary_Field" VALUES (2, 'SKDJ', '2Top', '2次冲顶');
INSERT INTO "Dictionary_Field" VALUES (3, 'SKDJ', '3Top', '3次冲顶');
INSERT INTO "Dictionary_Field" VALUES (4, 'SKDJ', 'WanderingInTop', '顶部徘徊');
INSERT INTO "Dictionary_Field" VALUES (5, 'SKDJ', 'WanderingInBottom', '底部徘徊');
INSERT INTO "Dictionary_Field" VALUES (0, 'StrengthAngle', 'Small', '小角度');
INSERT INTO "Dictionary_Field" VALUES (1, 'StrengthAngle', 'Medium', '中等角度');
INSERT INTO "Dictionary_Field" VALUES (2, 'StrengthAngle', 'Large', '大角度');
INSERT INTO "Dictionary_Field" VALUES (0, 'PriceFluctuating', 'Rising', '上涨');
INSERT INTO "Dictionary_Field" VALUES (1, 'PriceFluctuating', 'Falling', '下跌');
INSERT INTO "Dictionary_Field" VALUES (2, 'PriceFluctuating', 'OscillatingAfterRise', '上涨后震荡');
INSERT INTO "Dictionary_Field" VALUES (3, 'PriceFluctuating', 'OscillatingAfterFall', '下跌后震荡');
INSERT INTO "Dictionary_Field" VALUES (4, 'PriceFluctuating', 'SlightFallAfterRiseDramatically', '大角度上涨后小幅度下跌');
INSERT INTO "Dictionary_Field" VALUES (5, 'PriceFluctuating', 'SlightRiseAfterFallSharply', '大角度下跌后小幅度上涨');
INSERT INTO "Dictionary_Field" VALUES (0, 'Deviation', '3GreenMACDPillar,RecordLowPriceButIndexValueNot', '3波MACD绿柱，指标未创新低而价格创新低');
INSERT INTO "Dictionary_Field" VALUES (1, 'Deviation', '4GreenMACDPillar,RecordLowPriceButIndexValueNot', '4波MACD绿柱，指标未创新低而价格创新低');
INSERT INTO "Dictionary_Field" VALUES (2, 'Deviation', '3RedMACDPillar,RecordHighPriceButIndexValueNot', '3波MACD红柱，指标未创新高而价格创新高');
INSERT INTO "Dictionary_Field" VALUES (3, 'Deviation', '4RedMACDPillar,RecordHighPriceButIndexValueNot', '4波MACD红柱，指标未创新高而价格创新高');
INSERT INTO "Dictionary_Field" VALUES (0, 'RiseFallExtent', 'DecreaseBy6%', '下跌6%');
INSERT INTO "Dictionary_Field" VALUES (1, 'RiseFallExtent', 'DecreaseBy9%', '下跌9%');
INSERT INTO "Dictionary_Field" VALUES (2, 'RiseFallExtent', 'DecreaseBy12%', '下跌12%');
INSERT INTO "Dictionary_Field" VALUES (3, 'RiseFallExtent', 'IncreaseBy6%', '上涨6%');
INSERT INTO "Dictionary_Field" VALUES (4, 'RiseFallExtent', 'IncreaseBy9%', '上涨9%');
INSERT INTO "Dictionary_Field" VALUES (5, 'RiseFallExtent', 'IncreaseBy12%', '上涨12%');

PRAGMA foreign_keys = true;
