/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: 网卡选择UI类

*/

#ifndef UISELECTNETWORKCARD_H
#define UISELECTNETWORKCARD_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDebug>
#include "selectnetworkcard.h"

namespace Ui {
class UISelectNetworkCard;
}

class UISelectNetworkCard : public QDialog
{
    Q_OBJECT

public:
    explicit UISelectNetworkCard(QWidget *parent = nullptr);
    //将数据转化为列表展示出来
    void showInformation();
    ~UISelectNetworkCard();
    QString buf;
    //int转qstring
    QString getID();
    void on_closeButton_clicked();

private slots:
    //将用户选择的网卡号转为string格式返回给mainwindow
    void on_table_itemDoubleClicked(QTableWidgetItem *item);

signals:
    void signalSend(QString);

private:
    selectnetworkcard* selectnetworkcard_ = new selectnetworkcard();
    Ui::UISelectNetworkCard *ui;
    int idx_;
};

#endif // UISELECTNETWORKCARD_H
