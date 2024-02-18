#ifndef DEVCONFIGWIDGET_H
#define DEVCONFIGWIDGET_H
#define _CRT_SECURE_NO_WARNINGS
#include <QWidget>
#include "Def.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
extern std::vector<std::string>       g_vUtl8512DevComs;
extern std::string                    g_ioCfgFileName;
extern std::string UTF8_TO_string(const std::string& str);
extern std::string string_TO_UTF8(const std::string& str);
class DevConfigwidget : public QDialog
{
    Q_OBJECT
public:
    explicit DevConfigwidget(QWidget *parent = nullptr);

private:
    QLabel*          m_labIOCfg;
    QLabel*          m_labAvaliRs232;
    QLabel*          m_labUDP6730;
    QLabel*          m_labUTG4082;
    QLabel*          m_labUTL8512;
    QLabel*          m_labSDS6000;
    QLineEdit*       m_txtIoCfg;
    QLineEdit*       m_txtAvaliRs232Coms;
    QLineEdit*       m_txtUDP6730Coms;
    QLineEdit*       m_txtUTG4082Coms;
    QLineEdit*       m_txtUTL8512Coms;
    QLineEdit*       m_txtSDS6000Coms;
    QHBoxLayout      m_hlayoutIoCfg;
    QHBoxLayout      m_hlayoutAvaliRs232;
    QHBoxLayout      m_hlayoutUDP6730;
    QHBoxLayout      m_hlayoutUTG4082;
    QHBoxLayout      m_hlayoutUTL8512;
    QHBoxLayout      m_hlayoutSDS6000;
    QVBoxLayout      m_vlayoutAll;
    QStringList      GetAvalibPort() noexcept;
public:
    QPushButton* m_btnDone;
    QStringList m_listSerialPorts;
public slots:
    void ConfigDone();
};

#endif // DEVCONFIGWIDGET_H
