#include <headers/barchartsbuilder.h>


BarChartBuilder::BarChartBuilder():
    m_Chartview(nullptr),
    m_Series(nullptr),
    m_SeriesNew(nullptr),
    m_Barset(nullptr)
{
    m_Layout = new QHBoxLayout;
    m_uiCnt = 0;
}

BarChartBuilder::~BarChartBuilder()
{
    delete m_Layout;
}

void BarChartBuilder::ClearData()
{
    if(nullptr != m_Series)
    {
        m_Series->clear();
    }
}

//不改变当前柱状图，仅更新Y轴数据
void BarChartBuilder::UpdateDate(std::vector<int> vecDate)
{
    if(m_uiCnt == vecDate.size())
    {
        //已有图标条柱个数与新数据一致，不需要重置
        for(unsigned int idx = 0; idx < vecDate.size(); idx++)
            m_Barset->replace(idx, vecDate[idx]);
    }
}

//改变当前柱状图重新创建
void BarChartBuilder::UpdateDate(std::map<QString, int> mapData)
{
    if(0 == mapData.size())
        return;

    if(m_uiCnt == mapData.size())
    {
        std::vector<int> vecDate;
        for(std::map<QString, int>::iterator it = mapData.begin(); it != mapData.end(); it++)
        {
            vecDate.push_back(it->second);
        }
        UpdateDate(vecDate);
        return;
    }


    QChartView *pchartView = CreaterNewChartView(mapData);
    m_Layout->removeWidget(m_Chartview);
    m_Layout->addWidget(pchartView);
    m_Chartview = pchartView;
    m_Series = m_SeriesNew;

    m_uiCnt = mapData.size();
}

QChartView * BarChartBuilder::CreaterNewChartView(std::map<QString, int> mapData)
{
    m_Barset = new QBarSet("");
    m_SeriesNew = new QBarSeries();
    m_SeriesNew->append(m_Barset);

    QChart *pChart = new QChart();
    pChart->addSeries(m_SeriesNew);
    pChart->setTitle("IP Packets Moniter");                // 设置图表的标题
    pChart->setAnimationOptions(QChart::SeriesAnimations);      // 动画效果

    //pseries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);//单条数据数值
    //pseries->setLabelsVisible(true);                                //数值可视化

    QStringList pcategories;
    int iMax = 0;
    for(std::map<QString, int>::iterator it = mapData.begin(); it != mapData.end(); it++)
    {
        m_Barset->append(it->second);
        pcategories.append(it->first);

        if(iMax < it->second)
            iMax = it->second;
    }

    QBarCategoryAxis *paxis = new QBarCategoryAxis(); //X轴
    paxis->append(pcategories);
    paxis->setLabelsAngle(90);
    pChart->setAxisX(paxis, m_SeriesNew);


    QValueAxis *mAyis= new QValueAxis;      //Y轴
    pChart->addAxis(mAyis, Qt::AlignLeft);
    m_SeriesNew->attachAxis(mAyis);
    mAyis->setRange(0, iMax + 1);            //Y轴大小值
    mAyis->setLabelFormat("%u");            //Y轴数据格式

    pChart->legend()->setVisible(false);            //数据说明不可见
    //pChart->legend()->setAlignment(Qt::AlignBottom);//数值说明放底部

    QChartView *pchartView = new QChartView(pChart);
    pchartView->setRenderHint(QPainter::Antialiasing);

    return pchartView;
}
