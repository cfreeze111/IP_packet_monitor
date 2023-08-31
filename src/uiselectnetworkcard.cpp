#include "headers/uiselectnetworkcard.h"
#include "ui_uiselectnetworkcard.h"

UISelectNetworkCard::UISelectNetworkCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UISelectNetworkCard)
{
    ui->setupUi(this);
    setWindowTitle("网卡信息");
    showInformation();

}

void UISelectNetworkCard::showInformation(){
    pcap_if_t* list = selectnetworkcard_->getInformation();

    //show网卡列表
    //初始化tableview表头
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->horizontalHeader()->setMinimumHeight(30);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置表格
    ui->table->setColumnCount(2);       // 2 列  网卡名称，网卡描述
    ui->table->setHorizontalHeaderLabels(QStringList() << "网卡名称" << "网卡描述");
    ui->table->setRowCount(::size);     // size 行

    pcap_if_t* cur = ::allNetwork;
    int idx = 0;
    while(cur) {
        ui->table->setItem(idx,0,new QTableWidgetItem(cur->name));
        ui->table->setItem(idx,1,new QTableWidgetItem(cur->description));

        cur = cur->next;
        idx++;
    }
    //show网卡列表
}

UISelectNetworkCard::~UISelectNetworkCard()
{
    delete ui;
}

void UISelectNetworkCard::on_table_itemDoubleClicked(QTableWidgetItem *item)
{
    this->idx_ = item->row();
    this->buf = getID();
    //将选择的网卡数据返回给mainwidow
    //传递信号
    //emit signalSend(QString::number(idx_ + 1));
    //this->close();
    on_closeButton_clicked();
}

QString UISelectNetworkCard::getID(){
    int num = idx_ + 1;
    return QString::number(num);
}

void UISelectNetworkCard::on_closeButton_clicked()
{
    //qDebug() << buf;
    emit signalSend(buf);
    this->close();
}
