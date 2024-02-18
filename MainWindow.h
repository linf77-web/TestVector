#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Def.h"
#include "Monitorwidget.h"
#include "Msglogwidget.h"
#include "Progressctrlwidget.h"
#include "DevConfigwidget.h"
#include "Udp6730widget.h"
#include "Utg4082widget.h"
#include "Utl8512widget.h"
#include "VectorEditTool.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionStil_triggered();

    void on_actionEVCD_triggered();

    void on_actionVCD_triggered();

    void on_actionPinMap_triggered();

    void on_actionTiming_triggered();

    void on_actionPattern_triggered();

    void on_actionDigitalBoard_triggered();

    void on_actionSDS6034_triggered();

    void on_actionUDP6730_triggered();

    void on_actionUTG4082_triggered();

    void on_actionUTL8512_triggered();

private:
    Ui::MainWindow *ui;

    QTabWidget*         m_consoleWid;
    Monitorwidget*      m_monitorWid;
    MsgLogWidget*       m_msgLogWid;

    ProgressCtrlwidget* m_progressWid;
    DevConfigwidget*    m_devConfigWid;
    QVBoxLayout         m_hLayout;

    Sds6034widget* m_sds6000Widget;
    Udp6730Widget* m_udp6730Widget;
    Utg4082widget* m_utg4082Widget;
    Utl8512widget* m_utl8512Widget;

    VectorTool*    m_vectorToolWnd;
};

#endif // MAINWINDOW_H
