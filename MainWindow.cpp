#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* widget = new QWidget(this);

    m_msgLogWid = new MsgLogWidget(this);
    //m_manualWid = new ManualWidget(this);

    //m_monitorWid = new Monitorwidget(this);
    m_progressWid = new ProgressCtrlwidget(this);
    m_consoleWid = new QTabWidget(this);
    m_consoleWid->addTab(m_progressWid, "ProgressTool");

    //m_hLayout.addWidget(m_monitorWid);
    m_hLayout.addWidget(m_consoleWid);
    m_hLayout.addWidget(m_msgLogWid);
    widget->setLayout(&m_hLayout);
    this->setCentralWidget(widget);
    this->setGeometry(200,100,1600,900);

    m_udp6730Widget = new Udp6730Widget();
    m_utl8512Widget = new Utl8512widget();
    m_utg4082Widget = new Utg4082widget();
    m_sds6000Widget = new Sds6034widget();
    m_vectorToolWnd = new VectorTool();

    connect(m_udp6730Widget, &Udp6730Widget::AddMsg, m_msgLogWid, &MsgLogWidget::AddMsg);
    //connect(m_manualWid->m_utg4082Widget->m_utg4082_ch1, &Utg4082CH::AddMsg, m_msgLogWid, &MsgLogWidget::AddMsg);
    //connect(m_manualWid->m_utg4082Widget->m_utg4082_ch2, &Utg4082CH::AddMsg, m_msgLogWid, &MsgLogWidget::AddMsg);
    //connect(m_manualWid->m_utl8512Widget, &Utl8512widget::AddMsg, m_msgLogWid, &MsgLogWidget::AddMsg);
  //connect(m_monitorWid->m_statusDevs, &StatusDevs::AddMsg, m_msgLogWid, &MsgLogWidget::AddMsg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionStil_triggered()
{

}

void MainWindow::on_actionEVCD_triggered()
{

}

void MainWindow::on_actionVCD_triggered()
{

}

void MainWindow::on_actionPinMap_triggered()
{

}

void MainWindow::on_actionTiming_triggered()
{

}

void MainWindow::on_actionPattern_triggered()
{
    m_vectorToolWnd->show();
}

void MainWindow::on_actionDigitalBoard_triggered()
{

}


void MainWindow::on_actionSDS6034_triggered()
{
    m_sds6000Widget->show();
}

void MainWindow::on_actionUDP6730_triggered()
{
    m_udp6730Widget->show();
}

void MainWindow::on_actionUTG4082_triggered()
{
    m_utg4082Widget->show();
}

void MainWindow::on_actionUTL8512_triggered()
{
    m_utl8512Widget->show();
}
