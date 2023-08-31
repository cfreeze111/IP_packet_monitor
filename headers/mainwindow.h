#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "uiselectnetworkcard.h"
#include <QDebug>
#include "mynpacp.h"
#include "moniterthread.h"
#include "graphthread.h"
#include "barchartsbuilder.h"
#include "uiparsetable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QAction;
class QMenu;
class QPlainTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startMoniter(int idx);

    void lockMoniter();

    void unlockMoniter();

    void sendNetWorkUI(QString);


private slots:
    void card();

    void protocol();

    void reshow(QString);

    void sltTooltip(bool status, int index, QBarSet *barset);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_pressed();

    void on_pushButton_3_pressed();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);


private:

    void createActions();

    void createMenus();

    bool getCardNum();

    void table(QString);

    void netWorkShow(QString);

    void textbrowser(QString);

    void ResetLabel();

    QPlainTextEdit *textEdit; /* 文本框 */
    QMenu *fileMenu; /* file菜单 */
   /* 定义了file的下拉菜单为new、open、save、saveAS、exit */
    QAction *newAct;
    QAction *open;
    QAction *Save;
    QAction *saveAs;
    QAction *exit;
    //计时定时器
    QTimer *timer_;
    int time_;
    //子线程对象
    graphThread *graphthread_;
    moniterThread *moniter_;
    //绘图对象
    BarChartBuilder *chart_;
    std::vector<QLabel*>    m_vecToolTips;

    Ui::MainWindow *ui;
    //网卡UI对象
    UISelectNetworkCard* networkCard_;

    uiParseTable* parseTable_;

};
#endif // MAINWINDOW_H
