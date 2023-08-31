/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: 扩展柱状图类

*/

#ifndef BARCHARTSBUILDER_H
#define BARCHARTSBUILDER_H


#include <QHBoxLayout>
#include <QDebug>
#include <QtCharts>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QMap>
#include <QString>
#include <map>
#include <vector>
QT_CHARTS_USE_NAMESPACE

class BarChartBuilder
{
public:
    BarChartBuilder();
    ~BarChartBuilder();
    //清空当前柱状图Y轴数据
    void ClearData();
    //数据更新接口：内部判断是重置原柱状图还是更新Y轴数据
    void UpdateDate(std::map<QString, int> mapData);
    //数据更新接口：仅更新柱状图Y轴数据，多于X轴条柱个数不做新增处理
    void UpdateDate(std::vector<int> vecDate);

private:
    //根据输入数据创建对应柱状图
    QChartView * CreaterNewChartView(std::map<QString, int> mapData);

public:
    QChartView      *m_Chartview;
    QHBoxLayout     *m_Layout;
    QBarSeries      *m_Series;
    QBarSeries      *m_SeriesNew;
    unsigned int    m_uiCnt;        //当前柱状图X轴条柱个数
    QBarSet         *m_Barset;
};

#endif // BARCHARTSBUILDER_H

