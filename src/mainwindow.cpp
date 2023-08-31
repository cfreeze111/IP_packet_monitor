#include <sstream>

#include <QMessageBox>
#include <QTimer>
#include <QTableWidgetItem>
#include "ui_mainwindow.h"
#include "headers/mainwindow.h"


#define WARNWIND(ERR)     QMessageBox::warning(NULL, "有点问题", ERR, QMessageBox::Yes, QMessageBox::Yes)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("IPPacketsMoniterAndParse");
    setWindowIcon(QIcon(":/images/design.ico"));        //图标
    createActions(); /* 首先创建file下拉的菜单栏 */
    createMenus(); /* 再来创建菜单目录，把下拉菜单的内容addaction加入到menu中 */
    time_ = 0;

    //时间QTimer
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, [=]()
    {
        time_++;
        ui->label_3->setText(QString::number(time_));
    });

    //接收子线程数据
    moniter_ = new moniterThread();
    connect(moniter_,&moniterThread::sendMessage,this,&MainWindow::table);
    connect(moniter_,&moniterThread::sendNetWork,this,&MainWindow::netWorkShow);

    graphthread_ = new graphThread();
    connect(graphthread_,&graphThread::sendNum,this,&MainWindow::textbrowser);

    //子窗口
    networkCard_ = new UISelectNetworkCard();
    connect(networkCard_,&UISelectNetworkCard::signalSend,this,&MainWindow::reshow);
    parseTable_ = new uiParseTable();
    connect(this,&MainWindow::sendNetWorkUI,parseTable_,&uiParseTable::netWorkShow);

    //charts
    chart_ = new BarChartBuilder();
    ui->widget->setLayout(chart_->m_Layout);

    m_vecToolTips.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::textbrowser(QString s){
    if( s.isEmpty() == false){
        // 将字符串包装为一个 istringstream 对象
        std::istringstream iss(s.toStdString());
        // 将字符串按照空格划分为一个字符串向量,并放入MAP中方便绘制直方图
        std::string word;
        QStringList sourceIP;//IP源地址
        std::vector<int> numIP;//IP数目
        std::map<QString,int> mapIP;//map《IP源地址，IP数目》
        int i = 0;
        //将s按空格分开，按照奇偶数分为IP：sourceIP和该IP包总数：numIP
        while (iss >> word) {
            //ui->textBrowser->append(QString::fromStdString(word));
            if(i%2==0){
                sourceIP.append(QString::fromStdString(word));
            }
            else{
                numIP.push_back(atoi(word.c_str()));
            }
            i++;
        }
        //给map赋值
        if(sourceIP.size() == numIP.size()){
            //qDebug() << "大小相同";
            int i = 0;
            for(std::vector<int>::iterator it = numIP.begin(); it != numIP.end(); it++){
                QString key = sourceIP[i];
                mapIP[key] = *it;
                i++;
            }
            //打印map内容
//            for(std::map<QString,int>::iterator it = mapIP.begin(); it != mapIP.end(); it++){
//                qDebug() << it->first << " " << it->second;
//            }
        }

        if( chart_ == nullptr){
            chart_ = new BarChartBuilder();
        }

        if( mapIP.begin() != mapIP.end()){
            chart_->UpdateDate(mapIP);
            ResetLabel();
        }
    }

}

void MainWindow::table(QString qmessage){
    //qDebug() << "table run !!!!";
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(30);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 设置最后一列的宽度自适应剩余的空间
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // 设置表格
    ui->tableWidget->setColumnCount(4);       // 数据包编号, 源地址，目标地址，协议类型，TTL
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "源地址" << "目标地址" << "协议类型" << "TTL");
    //string message = s + " " + s1 + " " + protocol + " " + s2;

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // 将字符串包装为一个 istringstream 对象
    std::istringstream iss(qmessage.toStdString());

    // 将字符串按照空格划分为一个字符串向量
    std::string word;
    int col = 0;
    while (iss >> word) {
        ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(word)));
        col++;
    }
    ui->tableWidget->scrollToBottom();
}


void MainWindow::reshow(QString y)
{
    this->show();
    ui->idx->setText(y);
}

void MainWindow::card()
{
    if(networkCard_ == nullptr){
        networkCard_ = new UISelectNetworkCard();
    }
    networkCard_->show();
}

void MainWindow::netWorkShow(QString s){
    if(parseTable_ == nullptr){
        parseTable_ = new uiParseTable();
    }
    emit sendNetWorkUI(s);
    parseTable_->show();
}


void MainWindow::protocol() {

}

/* file下拉菜单，名字为new、open、Save、saveAs和exit，并且添加了快捷键和图标，以及相应的槽函数 */
void MainWindow::createActions() {
    newAct = new QAction ( QIcon ( ":/image/new.png" ), tr ( "&网卡设置" ), this );
    newAct->setShortcuts ( QKeySequence::New );
    newAct->setStatusTip ( tr ( "Create a new file" ) );
    connect ( newAct, SIGNAL ( triggered() ), this, SLOT ( card() ) ); /* New点击事件调用网卡设置 */

    open = new QAction ( QIcon ( ":/image/open.png" ), tr ( "&协议设置" ), this );
    open->setShortcuts ( QKeySequence::Open );
    open->setStatusTip ( tr ( "open a new file" ) );
    connect ( open, SIGNAL ( triggered() ), this, SLOT ( protocol() ) ); /* 点击Open事件调用协议设置 */

    exit = new QAction ( tr ( "&exit" ), this );
    exit->setShortcuts ( QKeySequence::Close );
    exit->setStatusTip ( tr ( "exit a new file" ) );
    connect ( exit, SIGNAL ( triggered() ), this, SLOT ( close() ) ); /* 点击exit事件调用test */
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu ( tr ( "&设置" ) ); /* 创建一个name为设置的菜单栏 */
    /* 在这个菜单栏添加action即下拉菜单 */
    fileMenu->addAction ( newAct );
    fileMenu->addAction ( open );
    fileMenu->addSeparator(); /* 添加一个分割器 */
    fileMenu->addAction ( exit ); /* 推出下拉菜单 */
}



void MainWindow::on_pushButton_clicked()
{  
    //获取网卡号
   if( getCardNum() == false ){
        return;
   }
   //打开网卡获取包
   //指向用户选择网卡
   pcap_if_t* ptr;
   int i;
   int idx = ui->idx->text().toInt();
   for (ptr = ::allNetwork, i = 0; i < idx - 1; ptr = ptr->next, i++);
   //获取网卡信息
   QString label = ptr->description;
   QString label2 = "正在监听";
   ui->label_2->setText(label2+label);

   //子线程操作
   if(graphthread_ == nullptr){
       graphthread_ = new graphThread();
   }

   //传递给子线程idx
   if(moniter_ == nullptr){
       moniter_ = new moniterThread();
   }

   connect(this,&MainWindow::startMoniter,moniter_,&moniterThread::revIdx);
   emit startMoniter(idx);
   //启动子线程
   //开始计时=======================================================================此处需要修改，以及当计时结束后需要将time修改为0；
   timer_->start(1000);
   moniter_->start();
   graphthread_->start();
}


bool MainWindow::getCardNum(){
    // 网卡信息不可以为空
    if(ui->idx->text() == "") {
        WARNWIND("input network card not empty");
        return false;
    }
    int idx = ui->idx->text().toInt();      // 获得输入网卡编号
    if(idx <= 0 || idx > ::size) {          // 判断是否合理
        WARNWIND("network card idx error");
        return false;
    }

    //进行网络嗅探
    int i;
    //指向用户选择网卡信息
    pcap_if_t* ptr;
    for (ptr = ::allNetwork, i = 0; i < idx - 1; ptr = ptr->next, i++);
    //嗅探

    //打开选择的网卡要求回应
    if ((::sniff = pcap_open_live(ptr->name,

        65536,                        //包长度最大值)

        PCAP_OPENFLAG_PROMISCUOUS,    //混杂模式

        1000,                        //读超时为1秒

        errbuf                      //错误缓冲池

    )) == NULL)
    {
        WARNWIND("Can't Open!Npacp Don't Support It!");
        return false;
    }
    return true;
}


void MainWindow::on_pushButton_2_clicked()
{
    moniter_->lock();
}

void MainWindow::on_pushButton_3_clicked()
{

    moniter_->unlock();
}

void MainWindow::on_pushButton_2_pressed()
{
    timer_->stop();
}

void MainWindow::on_pushButton_3_pressed()
{
    timer_->start(1000);
}

//重置鼠标悬停时的标签对象，连接新柱状图的信号槽函数
void MainWindow::ResetLabel()
{
    connect(chart_->m_Series, SIGNAL(hovered(bool,int,QBarSet*)), this, SLOT(sltTooltip(bool,int,QBarSet*)));
    for(unsigned int index = 0; index < m_vecToolTips.size(); index++)
    {
        delete m_vecToolTips[index];
    }
    m_vecToolTips.clear();
    for(unsigned int index = 0; index < chart_->m_uiCnt; index++)
    {
        QLabel *label = nullptr;
        m_vecToolTips.push_back(label);
    }
}

//鼠标悬停时标签创建和显示
void MainWindow::sltTooltip(bool status, int index, QBarSet *barset)
{
    if(nullptr != chart_->m_Series && nullptr != chart_->m_Chartview)
    {
        QChart* pchart = chart_->m_Chartview->chart();
        QLabel *m_tooltip = nullptr;

        if(nullptr != m_vecToolTips[index])
        {
            m_tooltip = m_vecToolTips[index];
        }
        else
        {
            m_tooltip = new QLabel(chart_->m_Chartview);    //头文件中的定义 QLabel*   m_tooltip = nullptr;
            m_tooltip->setStyleSheet("background: rgba(240, 128, 128,185);color: rgb(248, 248, 255);"
                                     "border:0px groove gray;border-radius:10px;padding:2px 4px;"
                                     "border:2px groove gray;border-radius:10px;padding:2px 4px");
            m_tooltip->setVisible(false);
            m_vecToolTips[index] = m_tooltip;
        }

        if (status)
        {
            double val = barset->at(index);
            QPointF point(index, barset->at(index));
            QPointF pointLabel = pchart->mapToPosition(point);

            m_tooltip->setText(QString::number(val));
            m_tooltip->move(pointLabel.x(), pointLabel.y() - m_tooltip->height()*1.5);
            m_tooltip->show();
        }
        else
        {
            m_tooltip->hide();
        }
    }
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    int idx = item->row();

}
