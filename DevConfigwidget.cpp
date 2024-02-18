#include "DevConfigwidget.h"
#include "QPushButton"
DevConfigwidget::DevConfigwidget(QWidget *parent) : QDialog(parent)
{
    m_labIOCfg = new QLabel(QStringLiteral("IO配置文件:   "), this);
    m_txtIoCfg = new QLineEdit(this);
    m_txtIoCfg->setText(QStringLiteral("D:\\Code\\ChipMedic\\CmInstCtrlDemo\\debug\\Config\\111.csv"));
    m_hlayoutIoCfg.addWidget(m_labIOCfg);
    m_hlayoutIoCfg.addWidget(m_txtIoCfg);
    m_labAvaliRs232 = new QLabel(QStringLiteral("可用串口:     "), this);
    m_txtAvaliRs232Coms = new QLineEdit(this);
    m_hlayoutAvaliRs232.addWidget(m_labAvaliRs232);
    m_hlayoutAvaliRs232.addWidget(m_txtAvaliRs232Coms);
    m_labUDP6730 = new QLabel(QStringLiteral("UDP6730:      "), this);
    m_txtUDP6730Coms = new QLineEdit(QStringLiteral("COM1;COM2;COM3;..."), this);
    m_hlayoutUDP6730.addWidget(m_labUDP6730);
    m_hlayoutUDP6730.addWidget(m_txtUDP6730Coms, 2);
    m_labUTL8512 = new QLabel(QStringLiteral("UTL8512:      "), this);
    m_txtUTL8512Coms = new QLineEdit(QStringLiteral("COM3;"), this);
    m_hlayoutUTL8512.addWidget(m_labUTL8512);
    m_hlayoutUTL8512.addWidget(m_txtUTL8512Coms, 2);
    m_labUTG4082 = new QLabel(QStringLiteral("UTG4082:      "), this);
    m_txtUTG4082Coms = new QLineEdit(QStringLiteral("Adress1:port1;Adress2:port2;..."), this);
    m_hlayoutUTG4082.addWidget(m_labUTG4082);
    m_hlayoutUTG4082.addWidget(m_txtUTG4082Coms, 2);
    m_labSDS6000 = new QLabel(QStringLiteral("SDS6000:      "), this);
    m_txtSDS6000Coms = new QLineEdit(QStringLiteral("Adress1:port1;Adress2:port2;..."), this);
    m_hlayoutSDS6000.addWidget(m_labSDS6000);
    m_hlayoutSDS6000.addWidget(m_txtSDS6000Coms, 2);
    m_vlayoutAll.addLayout(&m_hlayoutIoCfg);
    m_vlayoutAll.addLayout(&m_hlayoutAvaliRs232);
    m_vlayoutAll.addLayout(&m_hlayoutUDP6730);
    m_vlayoutAll.addLayout(&m_hlayoutUTL8512);
    m_vlayoutAll.addLayout(&m_hlayoutUTG4082);
    m_vlayoutAll.addLayout(&m_hlayoutSDS6000);
    m_btnDone = new QPushButton(QStringLiteral("完成"), this);
    m_vlayoutAll.addWidget(m_btnDone);
    m_listSerialPorts = GetAvalibPort();
    QString txt;
    foreach(QString com, m_listSerialPorts)
        txt.append(com).append(";");
    m_txtAvaliRs232Coms->setText(txt);
    m_txtUTL8512Coms->setText(txt);
    m_txtAvaliRs232Coms->setReadOnly(true);
    connect(m_btnDone, SIGNAL(clicked()), this, SLOT(ConfigDone()));
    this->setWindowFlags(Qt::Dialog | Qt::WindowShadeButtonHint | Qt::WindowTitleHint);
    this->setFixedSize(400, 200);
    this->setLayout(&m_vlayoutAll);

}

QStringList DevConfigwidget::GetAvalibPort() noexcept
{
    QStringList list_avail_sp;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            list_avail_sp.append(serial.portName());
            serial.close();
        }
    }
    return list_avail_sp;

}

void DevConfigwidget::ConfigDone()
{
    QMessageBox msgBox;
    char utl8512Coms[100];
    QString qstr = m_txtUTL8512Coms->text();
    std::string stdStr = qstr.toStdString();
    strcpy(utl8512Coms, const_cast<char*>(m_txtUTL8512Coms->text().toStdString().c_str()));
    utl8512Coms[m_txtUTL8512Coms->text().toStdString().length()] = 0;
    char* pComs = utl8512Coms;
    char* substr = { 0 };
    if(0 != strlen(utl8512Coms)) {
        if(nullptr != strstr(pComs, ";")) {
            while(nullptr != (substr = strtok(pComs, ";"))){
                pComs =  nullptr;
                g_vUtl8512DevComs.push_back(std::string(substr));
            }
        }
    }

    if(m_txtIoCfg->text().isEmpty() || -1 == m_txtIoCfg->text().indexOf(".csv")) {
        msgBox.setText(QStringLiteral("io配置文件名为空或者格式错误, 正确文件名为：xxx.csv"));
        msgBox.exec();
        return;
    }
    g_ioCfgFileName = UTF8_TO_string(m_txtIoCfg->text().toStdString());
    this->close();
}

