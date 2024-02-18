#include "Utg4082widget.h"
#include "QHBoxLayout"
#include <QPushButton>
Utg4082ParamItem::Utg4082ParamItem(QString name, ValueType type, QString unit, QWidget *parent) : QWidget(parent), m_name(name),
                                   m_valueType(type), m_unit(unit)
{
    m_labTitle = nullptr;
    m_labUnit = nullptr;
    m_spinValue = nullptr;
    m_spinDoubValue = nullptr;
    m_labTitle = new QLabel(m_name, this);
    m_labUnit = new QLabel(m_unit, this);
    m_hlayout.addWidget(m_labTitle);
    if(ValueType::_MY_INT == m_valueType) {
        m_spinValue = new QSpinBox(this);
        m_spinDoubValue = nullptr;
        m_hlayout.addWidget(m_spinValue);
    }
    else {
        m_spinDoubValue = new QDoubleSpinBox(this);
        m_spinValue = nullptr;
        m_hlayout.addWidget(m_spinDoubValue);
    }
    m_hlayout.addWidget(m_labUnit);
    this->setLayout(&m_hlayout);
}
void Utg4082ParamItem::Delete()
{
    if(nullptr != m_labTitle)
        delete m_labTitle;
    if(nullptr != m_labUnit)
        delete m_labUnit;
    if(nullptr != m_spinValue)
        delete m_spinValue;
    if(nullptr != m_spinDoubValue)
        delete m_spinDoubValue;
}
void Utg4082CH::Delete()
{
    if(nullptr != m_pFreq) {
        m_pFreq->Delete();
        delete m_pFreq;
    }
    if(nullptr !=m_pAmp) {
        m_pAmp->Delete();
        delete m_pAmp;
    }
    if(nullptr != m_pPhase) {
        m_pPhase->Delete();
        delete m_pPhase;
    }
    if(nullptr != m_pOffset) {
        m_pOffset->Delete();
        delete m_pOffset;
    }
    if(nullptr != m_pDuty) {
       m_pDuty->Delete();
       delete m_pDuty;
    }
    if(nullptr != m_pRisetime) {
        m_pRisetime->Delete();
        delete m_pRisetime;
    }
    if(nullptr != m_pFalltime) {
        m_pFalltime->Delete();
        delete m_pFalltime;
    }
    if(nullptr != m_pSymmetry) {
        m_pSymmetry->Delete();
         delete m_pSymmetry;
    }
}
Utg4082CH::Utg4082CH(int paramSelect, QWidget *parent) : QWidget(parent), m_paramEnable(paramSelect)
{
    m_pFreq = nullptr;
    m_pAmp = nullptr;
    m_pOffset = nullptr;
    m_pPhase = nullptr;
    m_pDuty = nullptr;
    m_pRisetime = nullptr;
    m_pFalltime = nullptr;
    m_pSymmetry = nullptr;
    m_utg4082 = nullptr;
    m_utg4082 = new Utg4000Lib();
    if(paramSelect & 0x01) {
        m_pFreq = new Utg4082ParamItem(QStringLiteral("频率"), ValueType::_MY_DOUBLE, QString("       kHz"), this);
        m_vlayout.addWidget(m_pFreq);
    }
    if(paramSelect >> 1 & 0x01) {
        m_pAmp = new Utg4082ParamItem(QStringLiteral("幅度"), ValueType::_MY_DOUBLE, QString("       Vpp"), this);
        m_vlayout.addWidget(m_pAmp);
    }
    if(paramSelect >> 2 & 0x01) {
        m_pOffset = new Utg4082ParamItem(QStringLiteral("直流偏移"), ValueType::_MY_DOUBLE, QString("        mv"), this);
        m_vlayout.addWidget(m_pOffset);
    }
    if(paramSelect >> 3 & 0x01) {
        m_pPhase = new Utg4082ParamItem(QStringLiteral("相位"), ValueType::_MY_DOUBLE, QStringLiteral("         °"), this);
        m_vlayout.addWidget(m_pPhase);
    }
    if(paramSelect >> 4 & 0x01) {
        m_pDuty = new Utg4082ParamItem(QStringLiteral("占空比"), ValueType::_MY_DOUBLE, QString("         %"), this);
        m_vlayout.addWidget(m_pDuty);
    }
    if(paramSelect >> 5 & 0x01) {
        m_pRisetime = new Utg4082ParamItem(QStringLiteral("上升沿时间"), ValueType::_MY_DOUBLE, QString("        ns"), this);
        m_vlayout.addWidget(m_pRisetime);
    }
    if(paramSelect >> 6 & 0x01) {
        m_pFalltime = new Utg4082ParamItem(QStringLiteral("下降沿时间"), ValueType::_MY_DOUBLE, QString("         ns"), this);
        m_vlayout.addWidget(m_pFalltime);
    }
    if(paramSelect >> 7 & 0x01) {
        m_pSymmetry = new Utg4082ParamItem(QStringLiteral("对称度"), ValueType::_MY_DOUBLE, QString("         %"), this);
        m_vlayout.addWidget(m_pSymmetry);
    }
    this->setLayout(&m_vlayout);
}
void Utg4082CH::SetWaveParam(int ch)
{
   //if(nullptr != m_utg4082) {
   //    if(nullptr != m_pFreq)
   //         if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_FREQ, long(m_pFreq->m_spinValue->value()))) < 0)
   //            emit AddMsg(QString("Error: Write param_msg  RM_BASE_FREQ Error!\n"));
   //    if(nullptr != m_pAmp)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_AMP_VPP, m_pAmp->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_AMP_VPP Error!\n"));
   //    if(nullptr != m_pOffset)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_OFFSET, m_pOffset->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_OFFSET Error!\n"));
   //    if(nullptr != m_pPhase)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_PHASE, m_pPhase->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_PHASE Error!\n"));
   //    if(nullptr != m_pDuty)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_DUTY, m_pDuty->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_DUTY Error!\n"));
   //    if(nullptr != m_pRisetime)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_RISETIME, m_pRisetime->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_RISETIME Error!\n"));
   //    if(nullptr != m_pFalltime)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_FALLTIME, m_pFalltime->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_FALLTIME Error!\n"));
   //    if(nullptr != m_pSymmetry)
   //        if((m_utg4082->WriteParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_SYMMETRY, m_pSymmetry->m_spinDoubValue->value())) < 0)
   //           emit AddMsg(QString("Error: Write param_msg  RM_BASE_SYMMETRY Error!\n"));
   //}
}
void Utg4082CH::GetWaveParam(int ch)
{
    //unsigned char readBuf[VAL_MAX];
    //if(nullptr != m_pFreq) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_FREQ, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_FREQ Error!\n"));
    //    else
    //        m_pFreq->m_spinValue->setValue((int)readBuf);
    //}
    //if(nullptr != m_pAmp) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_AMP_VPP, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_AMP_VPP Error!\n"));
    //    else
    //        m_pAmp->m_spinDoubValue->setValue(double((int)readBuf));
    //}
    //if(nullptr != m_pOffset) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_OFFSET, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_OFFSET Error!\n"));
    //    else
    //        m_pOffset->m_spinDoubValue->setValue(double((int)readBuf));
    //}
    //if(nullptr != m_pPhase) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_PHASE, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_PHASE Error!\n"));
    //    else
    //        m_pPhase->m_spinDoubValue->setValue(double((int)readBuf));
    //}
    //if(nullptr != m_pDuty) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_DUTY, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_DUTY Error!\n"));
    //    else
    //        m_pDuty->m_spinDoubValue->setValue(double((int)readBuf));
    //}
    //if(nullptr != m_pRisetime) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_RISETIME, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_RISETIME Error!\n"));
    //    else
    //        m_pRisetime->m_spinDoubValue->setValue(double((int)readBuf));
    //}
    //if(nullptr != m_pFalltime) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_FALLTIME, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_FALLTIME Error!\n"));
    //    else
    //        m_pFalltime->m_spinDoubValue->setValue(double((int)readBuf));
    //}
    //if(nullptr != m_pSymmetry) {
    //    if(m_utg4082->ReadParam(ch, uci::utg4162::ERemoteMessage::RM_BASE_SYMMETRY, readBuf) < 0)
    //        AddMsg(QString("Error: Read param RM_BASE_SYMMETRY Error!\n"));
    //    else
    //        m_pSymmetry->m_spinDoubValue->setValue(double((int)readBuf));
    //}
}
Utg4082CH::~Utg4082CH() {
    delete m_utg4082;
    Delete();
}
Utg4082widget::Utg4082widget(QWidget *parent) : QWidget(parent)
{
    m_utg4082_ch1 = nullptr;
    m_utg4082_ch2 = nullptr;
    m_vPrmSelect.push_back(std::pair<int, int>(0, 0x0f));
    m_vPrmSelect.push_back(std::pair<int, int>(1, 0x17));
    m_vPrmSelect.push_back(std::pair<int, int>(2, 0x7f));
    m_vPrmSelect.push_back(std::pair<int, int>(3, 0x04));
    m_vPrmSelect.push_back(std::pair<int, int>(4, 0x8f));
    m_vPrmSelect.push_back(std::pair<int ,int>(5, 0x06));
    m_labCh1 = new QLabel("CH1", this);
    m_labCh2 = new QLabel("CH2", this);
    m_labMode = new QLabel(QStringLiteral("调制模式"), this);
    m_labBaseWaveChoose = new QLabel(QStringLiteral("基波波形"));
    m_labModeWaveChoose = new QLabel(QStringLiteral("调制波波形"));
    m_radioCh1 = new QRadioButton(this);
    m_radioCh2 = new QRadioButton(this);
    m_radioMode = new QRadioButton(this);
    m_cmbBaseWaveChoose = new QComboBox(this);
    m_cmbModeWaveChoose = new QComboBox(this);
    m_btnGetParam = new QPushButton(QStringLiteral("刷新参数"), this);
    m_btnSetParam = new QPushButton(QStringLiteral("设置参数"), this);
    QStringList strListBase, strListMode;
    strListBase << QStringLiteral("正弦波") << QStringLiteral("方波") << QStringLiteral("脉冲波") << QStringLiteral("直流") << QStringLiteral("斜波") << QStringLiteral("噪声");
    strListMode << QStringLiteral("正弦波") << QStringLiteral("方波") << QStringLiteral("上行斜波") << QStringLiteral("下行斜波") << QStringLiteral("噪声") << QStringLiteral("任意波");
    m_cmbBaseWaveChoose->addItems(strListBase);

    m_cmbBaseWaveChoose->setCurrentIndex(2);

    m_cmbModeWaveChoose->addItems(strListMode);
    m_hlayout_Chchoose.addWidget(m_radioCh1);
    m_hlayout_Chchoose.addWidget(m_labCh1);
    m_hlayout_Chchoose.addWidget(m_radioCh2);
    m_hlayout_Chchoose.addWidget(m_labCh2);
    m_hlayout_Chchoose.addWidget(m_btnGetParam);
    m_hlayout_Chchoose.addWidget(m_btnSetParam);
    m_grpChChoose = new QGroupBox(this);
    m_grpChChoose->setLayout(&m_hlayout_Chchoose);
    m_hlayout_WvChoose.addWidget(m_radioMode);
    m_hlayout_WvChoose.addWidget(m_labMode);
    m_hlayout_WvChoose.addWidget(m_cmbBaseWaveChoose);
    m_hlayout_WvChoose.addWidget(m_labBaseWaveChoose);
    m_hlayout_WvChoose.addWidget(m_cmbModeWaveChoose);
    m_hlayout_WvChoose.addWidget(m_labModeWaveChoose);
    m_grpWaveChoose = new QGroupBox(this);
    m_grpWaveChoose->setLayout(&m_hlayout_WvChoose);
    m_hlayout_Choose.addWidget(m_grpChChoose);
    m_hlayout_Choose.addWidget(m_grpWaveChoose);
    InitParam( m_vPrmSelect[m_cmbBaseWaveChoose->currentIndex()].second);
    connect(m_cmbBaseWaveChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(RefreshParam(int)));
    connect(m_cmbModeWaveChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(RefreshParam(int)));
    connect(m_btnSetParam, SIGNAL(clicked()), this, SLOT(SetParam()));
    connect(m_btnGetParam, SIGNAL(clicked()), this, SLOT(GetParam()));
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->addWidget(m_grpCh1Params);
    hlayout->addWidget(m_grpCh2Params);
    m_vlayoutFinal.addLayout(&m_hlayout_Choose);
    m_vlayoutFinal.addLayout(hlayout);
    this->setLayout(&m_vlayoutFinal);
    this->setStyleSheet("QLabel{font-family: 'Microsoft YaHei';font-size: 20px;border-radius: 12px;font-weight: bold;text-align: center;min-width: 60px; min-height:30px;background-color: #6FAD7F;}"
                        "QPushButton{font-family: 'Microsoft YaHei';font-size: 20px;border-radius: 12px;font-weight: bold;text-align: center;min-width: 60px; min-height:30px;background-color: #6FAD7F;}"
                        "QPushButton:hover{background-color: #2EAD51;}"
                        "QPushButton:pressed{background-color: #257FE6;}");

}
Utg4082widget::~Utg4082widget()
{

}
void Utg4082widget::InitParam(int paramSelect)
{
    m_hlayout_ch1Params = nullptr;
    m_hlayout_ch2Params = nullptr;
    m_hlayout_ch1Params = new QHBoxLayout();
    m_hlayout_ch2Params = new QHBoxLayout();
    m_utg4082_ch1 = new Utg4082CH(paramSelect, this);
    m_utg4082_ch1->show();
    m_utg4082_ch2 = new Utg4082CH(paramSelect, this);
    m_utg4082_ch2->show();
    m_grpCh1Params = new QGroupBox("CH1", this);
    m_grpCh2Params = new QGroupBox("CH2", this);
    m_hlayout_ch1Params->addWidget(m_utg4082_ch1);
    m_hlayout_ch2Params->addWidget(m_utg4082_ch2);
    m_grpCh1Params->setLayout(m_hlayout_ch1Params);
    m_grpCh2Params->setLayout(m_hlayout_ch2Params);
}
void Utg4082widget::RefreshParam(int paramSelect)
{
    delete m_hlayout_ch1Params;
    delete m_hlayout_ch2Params;
    m_hlayout_ch1Params = new QHBoxLayout(this);
    m_hlayout_ch2Params = new QHBoxLayout(this);
    m_utg4082_ch1->Delete();
    m_utg4082_ch2->Delete();
    int value  = m_vPrmSelect[paramSelect].second;
    m_utg4082_ch1 = new Utg4082CH(value, this);
    m_utg4082_ch1->show();
    m_utg4082_ch2 = new Utg4082CH(value, this);
    m_utg4082_ch2->show();
    m_hlayout_ch1Params->addWidget(m_utg4082_ch1);
    m_hlayout_ch2Params->addWidget(m_utg4082_ch2);
    m_grpCh1Params->setLayout(m_hlayout_ch1Params);
    m_grpCh2Params->setLayout(m_hlayout_ch2Params);
}
void Utg4082widget::SetParam()
{
    if(m_radioCh1->isChecked())
        m_utg4082_ch1->SetWaveParam(1);
    else if(m_radioCh2->isChecked())
        m_utg4082_ch2->SetWaveParam(2);
}
void Utg4082widget::GetParam()
{
    if(m_radioCh1->isChecked())
        m_utg4082_ch1->GetWaveParam(1);
    else if(m_radioCh2->isChecked())
        m_utg4082_ch2->GetWaveParam(2);
}
