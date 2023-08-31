#include "headers/uiparsetable.h"
#include "ui_uiparsetable.h"
#include <QDebug>
#include <QTableWidgetItem>
#include "sstream"

using namespace std;

uiParseTable::uiParseTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uiParseTable)
{
    ui->setupUi(this);


}

uiParseTable::~uiParseTable()
{
    delete ui;
}

void uiParseTable::netWorkShow(QString qmessage){
    //qDebug() << qmessage;
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(30);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 设置最后一列的宽度自适应剩余的空间
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // 设置表格
    ui->tableWidget->setColumnCount(11);       // 数据包编号, 源地址，目标地址，协议类型，TTL
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "版本" << "首部长度" << "服务类型" << "长度"
                                               << "标识" << "标志" << "生存时间" << "协议" << "首部校验" << "源地址" << "目的地址");
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
