#include "Udp6730widget.h"
#include <QPushButton>
#include <QString>
Udp6730Widget::Udp6730Widget(QWidget *parent) : QWidget(parent)
{
    for(int i = 0; i < 10; ++i)
        memset(m_findAdressList[i], 0, 256 * sizeof(char));
    m_devDescriptor = new char[VAL_MAX];
    m_outputStatu = -1;
    m_udp6730 = new Udp6730();
    m_lcdDisVol = new QLCDNumber(this);
    m_lcdDisCur = new QLCDNumber(this);
    m_lcdDisVol->setMode(QLCDNumber::Dec);
    m_lcdDisCur->setMode(QLCDNumber::Dec);
    m_lcdDisVol->display("05.34");
    m_lcdDisCur->display("2.565");
    m_lcdDisVol->setSegmentStyle(QLCDNumber::Flat);
    m_lcdDisCur->setSegmentStyle(QLCDNumber::Flat);
    m_labMeasuredV = new QLabel("MeasuredV:");
    m_labMeasuredC = new QLabel("MeasuredC:");
    m_labDisV = new QLabel("V", this);
    m_labDisC = new QLabel("A", this);
    m_labVolCtrl = new QLabel("Voltage:", this);
    m_labCurCtrl = new QLabel("Current:", this);
    m_btnSetVProt = new QPushButton("Set OVP", this);
    m_btnSetCProt = new QPushButton("Set OCP", this);
    m_editSetVProt = new QDoubleSpinBox(this);
    m_editSetCProt = new QDoubleSpinBox(this);
    m_editSetCProt->setRange(1.0f, 100.1f);
    m_editSetVProt->setRange(1.0f, 100.1f);
    m_editVolCtrl = new QDoubleSpinBox(this);
    m_editVolCtrl->setSingleStep(0.1);
    m_editCurCtrl = new QDoubleSpinBox(this);
    m_editCurCtrl->setSingleStep(0.1);
    m_editVolCtrl->setRange(0.01f, 100.1f);
    m_editCurCtrl->setRange(0.01f, 100.1f);
    m_hsliderVolCtrl = new QSlider(Qt::Horizontal, this);
    m_hsliderCurCtrl = new QSlider(Qt::Horizontal, this);
    m_hsliderVolCtrl->setRange(0, 100);
    m_hsliderCurCtrl->setRange(0, 100);
    m_btnSetVoltage = new QPushButton("SetV", this);
    m_btnSetCurrent = new QPushButton("SetC", this);
    m_btnOutPut = new QPushButton("OUTPUT:", this);
    m_lcdDisVol->setDigitCount(5);
    m_lcdDisCur->setDigitCount(5);
    m_hlayoutDisV.addWidget(m_labMeasuredV);
    m_hlayoutDisV.addWidget(m_lcdDisVol, 2);
    m_hlayoutDisV.addWidget(m_labDisV);
    m_hlayoutDisC.addWidget(m_labMeasuredC);
    m_hlayoutDisC.addWidget(m_lcdDisCur, 2);
    m_hlayoutDisC.addWidget(m_labDisC);
    m_hlayoutSetProt.addWidget(m_btnSetVProt);
    m_hlayoutSetProt.addWidget(m_editSetVProt, 2);
    m_hlayoutSetProt.addWidget(m_editSetCProt, 2);
    m_hlayoutSetProt.addWidget(m_btnSetCProt);
    m_hlayoutVolCtrl.addWidget(m_labVolCtrl);
    m_hlayoutVolCtrl.addWidget(m_editVolCtrl);
    m_hlayoutVolCtrl.addWidget(m_hsliderVolCtrl);
    m_hlayoutVolCtrl.addWidget(m_btnSetVoltage);
    m_hlayoutCurCtrl.addWidget(m_labCurCtrl);
    m_hlayoutCurCtrl.addWidget(m_editCurCtrl);
    m_hlayoutCurCtrl.addWidget(m_hsliderCurCtrl);
    m_hlayoutCurCtrl.addWidget(m_btnSetCurrent);
    m_layoutFinal.addLayout(&m_hlayoutDisV);
    m_layoutFinal.addLayout(&m_hlayoutDisC);
    m_layoutFinal.addLayout(&m_hlayoutSetProt);
    //m_layoutFinal.addLayout(&m_vLayoutDis);
    m_layoutFinal.addLayout(&m_hlayoutVolCtrl);
    m_layoutFinal.addLayout(&m_hlayoutCurCtrl);
    m_layoutFinal.addWidget(m_btnOutPut);
    this->setStyleSheet("QLCDNumber {font-size: 10px;font-weight: bold;color: yellow;border: 1px solid green;background-color: black;background-position: center;}"
                        "QLabel{font-family: 'Microsoft YaHei';font-size: 20px;border-radius: 12px;font-weight: bold;text-align: center;min-width: 60px; min-height:30px;background-color: #6FAD7F;}"
                        "QPushButton{font-family: 'Microsoft YaHei';font-size: 20px;border-radius: 12px;font-weight: bold;text-align: center;min-width: 60px; min-height:30px;background-color: #6FAD7F;}"
                        "QPushButton:hover{background-color: #2EAD51;}"
                        "QPushButton:pressed{background-color: #257FE6;}");
    this->setLayout(&m_layoutFinal);
    connect(m_btnSetVProt, SIGNAL(clicked()), this, SLOT(SetOvp()));
    connect(m_btnSetCProt, SIGNAL(clicked()), this, SLOT(SetOcp()));
    connect(m_btnOutPut, SIGNAL(clicked()), this, SLOT(SetOutPut()));
    connect(m_hsliderVolCtrl, SIGNAL(valueChanged(int)), SLOT(OnEditVolChanged(int)));
    connect(m_hsliderCurCtrl, SIGNAL(valueChanged(int)), SLOT(OnEditCurChanged(int)));
    connect(m_btnSetVoltage, SIGNAL(clicked()), this, SLOT(SetVoltage()));
    connect(m_btnSetCurrent, SIGNAL(clicked()), this, SLOT(SetCurrent()));
}
Udp6730Widget::~Udp6730Widget()
{   

}
void Udp6730Widget::OnEditVolChanged(int value)
{
    m_editVolCtrl->setValue(double(value));
}
void Udp6730Widget::OnEditCurChanged(int value)
{
    m_editCurCtrl->setValue(double(value));
}
int Udp6730Widget::GetOutPut()
{
    int ret = -1;
    char buf[VAL_MAX];
    if(m_udp6730->OutputGet(TIMEOUT_DEFAULT, false, buf))
        emit AddMsg(QString("Error: Get OutPut Statu failed!\n"));
    else {
        emit AddMsg(QString("Get OutPut Statu Succeed!\n"));
        const char* statu = buf;
        if(0 == strcmp("ON", statu))
            ret = 1;
        else if(0 == strcmp("OFF", statu))
            ret = 0;
    }
    return ret;
}
void Udp6730Widget::SetOutPut()
{
    if(-1 == m_outputStatu || 0 == m_outputStatu) {
        if(m_udp6730->OutputSet(TIMEOUT_DEFAULT, true) < 0) {
            m_btnOutPut->setText("OUTPUT:   UNABLE");
            m_btnOutPut->setStyleSheet("QPushButton{background-color: #F08650;}");
            emit AddMsg(QString("Error: Set OutPut ON Failed!\n"));
        }
        else if(1 == GetOutPut()) {
                emit AddMsg(QString("Set OutPut ON Succeed!\n"));
                m_outputStatu = 1;
                m_btnOutPut->setText("OUTPUT:   ON");
                m_btnOutPut->setStyleSheet("QPushButton{background-color: #377D22;}");
            }
    }
    else if(1 == m_outputStatu) {
        if(m_udp6730->OutputSet(TIMEOUT_DEFAULT, false) < 0) {
            m_btnOutPut->setText("OUTPUT:   UNABLE");
            m_btnOutPut->setStyleSheet("QPushButton{background-color: #F08650;}");
            emit AddMsg(QString("Error: Set OutPut OFF Failed\n"));
        }
        else if(0 == GetOutPut()){
            emit AddMsg(QString("Set OutPut OFF Succeed!\n"));
            m_outputStatu = 0;
            m_btnOutPut->setText("OUTPUT:   OFF");
            m_btnOutPut->setStyleSheet("QPushButton{background-color: #F09B59;}");
        }
    }
}
void Udp6730Widget::SetOvp()
{
    char buf[VAL_MAX];
    sprintf(buf, "%2.2f", m_editSetVProt->value());
    if(m_udp6730->SrcSet(TIMEOUT_DEFAULT, buf, true, true) < 0)
        emit AddMsg(QString("Error: SetOvp Failed!\n"));
    else
        emit AddMsg(QString("SetOvp Succeed!\n"));
}
void Udp6730Widget::SetOcp()
{
    char buf[VAL_MAX];
    sprintf(buf, "%2.2f", m_editSetCProt->value());
    if(m_udp6730->SrcSet(TIMEOUT_DEFAULT, buf, false, true) < 0)
       emit AddMsg(QString("Error: SetOcp Failed!\n"));
    else
       emit AddMsg(QString("SetOcp Succeed!\n"));
}
void Udp6730Widget::SetVoltage()
{
    char buf[VAL_MAX];
    sprintf(buf, "%2.2f", m_editVolCtrl->value());
    if(m_udp6730->SrcSet(TIMEOUT_DEFAULT, buf, true, false) < 0)
        emit AddMsg(QString("Error: Set Voltage Failed!\n"));
    else
        emit AddMsg(QString("Set Voltage Succeed!\n"));
}
void Udp6730Widget::SetCurrent()
{
    char buf[VAL_MAX];
    sprintf(buf, "%2.2f", m_editCurCtrl->value());
    if(m_udp6730->SrcSet(TIMEOUT_DEFAULT, buf, false, false) < 0)
        emit AddMsg(QString("Error: Set Current Failed!\n"));
    else
        emit AddMsg(QString("Set Current Succeed!\n"));
}
void Udp6730Widget::GetVersion()
{
    emit AddMsg(QString(m_udp6730->GetVersion()));
}
void Udp6730Widget::Init(char findAdressList[10][256])
{
    char cmd[CMD_MAX] = {0};
    strcat_s(cmd, "?*");
    visa::GetInstance()->FindDev(cmd, findAdressList, 10, &m_devNums);
    for(int i =0; i < 10; ++i)
        memcpy(m_findAdressList[i], findAdressList[i], 256 * sizeof(char));
    visa::GetInstance()->OpenDev(m_findAdressList[0], 2000);
}
