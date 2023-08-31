/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: IP数据包分析表格UI类

*/

#ifndef UIPARSETABLE_H
#define UIPARSETABLE_H

#include <QDialog>

namespace Ui {
class uiParseTable;
}

class uiParseTable : public QDialog
{
    Q_OBJECT

public:
    explicit uiParseTable(QWidget *parent = nullptr);
    ~uiParseTable();

public slots:
    void netWorkShow(QString s);

private:
    Ui::uiParseTable *ui;
};

#endif // UIPARSETABLE_H
