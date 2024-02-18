#include "Teststepinfowidget.h"
#include <QPushButton>
StepCommonMinWidget::StepCommonMinWidget(QString name, QWidget* parent) : QWidget(parent), m_name(name)
{
    m_dspinBox = new QDoubleSpinBox();
    m_lab = new QLabel(m_name);
    m_hlayout.addWidget(m_lab, 1);
    m_hlayout.addWidget(m_dspinBox, 2);
    this->setLayout(&m_hlayout);
}
StepCommonMinWidget::~StepCommonMinWidget()
{
    delete m_dspinBox;
    delete m_lab;
}
StepUdp6730Widget::StepUdp6730Widget(QWidget *parent) : QWidget(parent)
{
    m_widgetVol = new StepCommonMinWidget(QStringLiteral("电压："));
    m_widgetCur = new StepCommonMinWidget(QStringLiteral("电流"));
    m_widgetVolProt = new StepCommonMinWidget(QStringLiteral("保护电压："));
    m_widgetCurProt = new StepCommonMinWidget(QStringLiteral("保护电流："));
    m_vlayoutAll.addWidget(m_widgetVol);
    m_vlayoutAll.addWidget(m_widgetCur);
    m_vlayoutAll.addWidget(m_widgetVolProt);
    m_vlayoutAll.addWidget(m_widgetCurProt);
    this->setLayout(&m_vlayoutAll);
}

StepUdp6730Widget::~StepUdp6730Widget()
{
    delete m_widgetVol;
    delete m_widgetCur;
    delete m_widgetVolProt;
    delete m_widgetCurProt;
}

StepUtg4082Widget::StepUtg4082Widget(QWidget *parent) : QWidget(parent)
{
   QStringList titles, units;
   m_itemModSine = new QStandardItemModel(4 , 2);
   titles << QStringLiteral("频率") << QStringLiteral("幅度") << QStringLiteral("直流偏移") << QStringLiteral("相位");
   units << QStringLiteral("kHz") << QStringLiteral("Vpp") << QStringLiteral("mv") << QStringLiteral("°");
   SetViewHeader(m_itemModSine, titles, units);
   titles.clear();
   units.clear();
   m_itemModSquare = new QStandardItemModel(4, 2);
   titles << QStringLiteral("频率") << QStringLiteral("幅度") << QStringLiteral("直流偏移") << QStringLiteral("占空比");
   units << QStringLiteral("kHz") << QStringLiteral("Vpp") << QStringLiteral("mv") << QStringLiteral("%");
   SetViewHeader(m_itemModSquare, titles, units);
   titles.clear();
   units.clear();
   m_itemModPulse = new QStandardItemModel(7, 2);
   titles << QStringLiteral("频率") << QStringLiteral("幅度") << QStringLiteral("直流偏移") << QStringLiteral("相位") << QStringLiteral("占空比")
          <<QStringLiteral("下降沿") << QStringLiteral("上升沿");
   units << QStringLiteral("kHz") << QStringLiteral("Vpp") << QStringLiteral("mv") << QStringLiteral("°") << QStringLiteral("%")
         <<QStringLiteral("ns") << QStringLiteral("ns");
   SetViewHeader(m_itemModPulse, titles, units);
   titles.clear();
   units.clear();
   m_itemModDC = new QStandardItemModel(1, 2);
   titles << QStringLiteral("直流偏移");
   units << QStringLiteral("mv");
   SetViewHeader(m_itemModDC, titles, units);
   titles.clear();
   units.clear();
   m_itemModRamp = new QStandardItemModel(5, 2);
   titles << QStringLiteral("频率") << QStringLiteral("幅度") << QStringLiteral("直流偏移") << QStringLiteral("相位") << QStringLiteral("对称度");
   units << QStringLiteral("kHz") << QStringLiteral("Vpp") << QStringLiteral("mv") << QStringLiteral("°") << QStringLiteral("%");
   SetViewHeader(m_itemModRamp, titles, units);
   titles.clear();
   units.clear();
   m_itemModNoise = new QStandardItemModel(2, 2);
   titles << QStringLiteral("幅度") << QStringLiteral("直流偏移");
   units << QStringLiteral("Vpp") << QStringLiteral("mv");
   SetViewHeader(m_itemModNoise, titles, units);
   titles.clear();
   units.clear();
   m_vWaveChoose.push_back(std::pair<int, QStandardItemModel*>(uci::utg4162::EBaseWave::BASE_SINE, m_itemModSine));
   m_vWaveChoose.push_back(std::pair<int, QStandardItemModel*>(uci::utg4162::EBaseWave::BASE_SQUARE, m_itemModSquare));
   m_vWaveChoose.push_back(std::pair<int, QStandardItemModel*>(uci::utg4162::EBaseWave::BASE_PULSE, m_itemModPulse));
   m_vWaveChoose.push_back(std::pair<int, QStandardItemModel*>(uci::utg4162::EBaseWave::BASE_DC, m_itemModDC));
   m_vWaveChoose.push_back(std::pair<int, QStandardItemModel*>(uci::utg4162::EBaseWave::BASE_RAMP, m_itemModRamp));
   m_vWaveChoose.push_back(std::pair<int, QStandardItemModel*>(uci::utg4162::EBaseWave::BASE_NOISE, m_itemModNoise));

   m_tabView = new QTableView();
   DoubleSpinDeleget* dDeleget = new DoubleSpinDeleget();
   m_tabView->setItemDelegateForColumn(0, dDeleget);
   m_labChChoose = new QLabel(QStringLiteral("通道选择："));
   m_labWaveChoose = new QLabel(QStringLiteral("波形选择："));
   m_cmbChChoose = new QComboBox();
   titles << QStringLiteral("CH1") << QStringLiteral("CH2");
   m_cmbChChoose->addItems(titles);
   titles.clear();
   m_cmbWaveChoose = new QComboBox();
   titles << QStringLiteral("正弦波") << QStringLiteral("方波") << QStringLiteral("脉冲波") << QStringLiteral("直流电压") << QStringLiteral("斜波") << QStringLiteral("噪声波");
   m_cmbWaveChoose->addItems(titles);
   titles.clear();
   m_tabView->setModel(m_vWaveChoose[m_cmbWaveChoose->currentIndex()].second);
   connect(m_cmbWaveChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(RefreshModView(int)));
   m_hlayoutChChoose.addWidget(m_labChChoose);
   m_hlayoutChChoose.addWidget(m_cmbChChoose, 2);
   m_hlayoutWaveChoose.addWidget(m_labWaveChoose);
   m_hlayoutWaveChoose.addWidget(m_cmbWaveChoose, 2);
   m_vlayoutAll.addLayout(&m_hlayoutChChoose);
   m_vlayoutAll.addLayout(&m_hlayoutWaveChoose);
   m_vlayoutAll.addWidget(m_tabView);
   this->setLayout(&m_vlayoutAll);
   QHeaderView* hHeaderView = m_tabView->horizontalHeader();
   hHeaderView->setSectionResizeMode(QHeaderView::Stretch);
   QHeaderView* vHeaderView = m_tabView->verticalHeader();
   vHeaderView->setSectionResizeMode(QHeaderView::Stretch);
   hHeaderView->setStyleSheet("QHeaderView::section {font-size: 14px;"
                              "background-color: #2EAD51;}");
   vHeaderView->setStyleSheet("QHeaderView::section {font-size: 14px;"
                              "background-color: #2EAD51;}");
}

StepUtg4082Widget::~StepUtg4082Widget()
{
    delete m_labChChoose;
    delete m_labWaveChoose;
    delete m_cmbChChoose;
    delete m_cmbWaveChoose;
    delete m_itemModSine;
    delete m_itemModSquare;
    delete m_itemModPulse;
    delete m_itemModDC;
    delete m_itemModRamp;
    delete m_itemModNoise;
    delete m_tabView;
}
void StepUtg4082Widget::SetViewHeader(QStandardItemModel* itemMod, QStringList& titles, QStringList& units)
{
    int size = titles.size();
    for(int i = 0; i < size; ++i) {
        itemMod->setHeaderData(i, Qt::Vertical, titles[i]);
        QStandardItem* item = new QStandardItem(units[i]);
        itemMod->setItem(i, 1, item);
    }
    QStringList hHeader;
    hHeader << QStringLiteral("设定值") << QStringLiteral("单位");
    itemMod->setHorizontalHeaderLabels(hHeader);
}
void StepUtg4082Widget::RefreshModView(int index)
{
     m_tabView->setModel(m_vWaveChoose[index].second);
}
StepUtl8512Widget::StepUtl8512Widget(QWidget *parent) : QWidget(parent)
{
    m_utl8512TabView = new Utl8512TabView();
    m_vlayoutAll.addWidget(m_utl8512TabView);
    this->setLayout(&m_vlayoutAll);
}

StepUtl8512Widget::~StepUtl8512Widget()
{
    delete m_utl8512TabView;
}

TestStepInfowidget::TestStepInfowidget(QWidget *parent) : QDialog(parent)
{
    memset(&m_stepInfo, 0, sizeof(m_stepInfo));
    m_labDevChoose = new QLabel(QStringLiteral("设备选择"));
    m_cmbDevChoose = new QComboBox();
    QStringList devNameList;
    devNameList << QStringLiteral("电源") << QStringLiteral("信号发生器") << QStringLiteral("负载");
    m_cmbDevChoose->addItems(devNameList);
    m_hlayoutDevChoose.addWidget(m_labDevChoose);
    m_hlayoutDevChoose.addWidget(m_cmbDevChoose, 2);
    m_udp6730 = new StepUdp6730Widget();
    m_vlayoutUdp6730.addWidget(m_udp6730);
    m_grpUdp6730 = new QGroupBox(QStringLiteral("电源"));
    m_grpUdp6730->setLayout(&m_vlayoutUdp6730);
    m_utg4082 = new StepUtg4082Widget();
    m_vlayoutUtg4082.addWidget(m_utg4082);
    m_grpUtg4082 = new QGroupBox(QStringLiteral("信号发生器"));
    m_grpUtg4082->setLayout(&m_vlayoutUtg4082);
    m_utl8512 = new StepUtl8512Widget();
    m_vlayoutUtl8512.addWidget(m_utl8512);
    m_grpUtl8512 = new QGroupBox(QStringLiteral("负载"));
    m_grpUtl8512->setLayout(&m_vlayoutUtl8512);
    m_vlayoutAll.addLayout(&m_hlayoutDevChoose);
    m_vlayoutAll.addWidget(m_grpUdp6730);
    m_vlayoutAll.addWidget(m_grpUtg4082);
    m_vlayoutAll.addWidget(m_grpUtl8512);
    m_btnDone = new QPushButton(QStringLiteral("完成"));
    m_vlayoutAll.addWidget(m_btnDone);
    DevChanged(m_cmbDevChoose->currentIndex());
    connect(m_cmbDevChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(DevChanged(int)));
    connect(m_btnDone, SIGNAL(clicked()), this, SLOT(AddDone()));
    this->setLayout(&m_vlayoutAll);
}

TestStepInfowidget::~TestStepInfowidget()
{
    delete m_udp6730;
    delete m_utg4082;
    delete m_utl8512;
    delete m_cmbDevChoose;
    delete m_labDevChoose;
    delete m_btnDone;
    delete m_grpUdp6730;
    delete m_grpUtg4082;
    delete m_grpUtl8512;
    delete m_grpSds6000;
}

void TestStepInfowidget::AddDone()
{
    memset(&m_stepInfo, 0, sizeof(m_stepInfo));
    m_stepInfo.devType = this->m_cmbDevChoose->currentIndex();
    m_stepInfo.fuc = m_stepInfo.devType;
    int devIndex = m_cmbDevChoose->currentIndex();
    int chIndex = m_utg4082->m_cmbChChoose->currentIndex();
    m_stepInfo.utg4000Info.sg_ch = chIndex;
    int waveIndex = m_utg4082->m_cmbWaveChoose->currentIndex();
    int modIndex = m_utl8512->m_utl8512TabView->m_cmbModChoose->currentIndex();
    if(1 == m_stepInfo.devType) {
        m_stepInfo.utg4000Info.sg_ch = this->m_utg4082->m_cmbChChoose->currentIndex();
        m_stepInfo.utg4000Info.waveType = this->m_utg4082->m_cmbWaveChoose->currentIndex();
    }
    if(2 == m_stepInfo.devType)
        m_stepInfo.utl8500Info.loadMode = this->m_utl8512->m_utl8512TabView->m_cmbModChoose->currentIndex();
    QStandardItemModel* itemMod = m_utl8512->m_utl8512TabView->m_vItemPair[m_utl8512->m_utl8512TabView->m_cmbModChoose->currentIndex()].second;
    //volProt
    if(DEV_UDP6730 == devIndex) {
        m_stepInfo.udp6730Info.id = 01;
        m_stepInfo.udp6730Info.volProt = m_udp6730->m_widgetVolProt->GetVal();
        m_stepInfo.udp6730Info.curProt = m_udp6730->m_widgetCurProt->GetVal();
        m_stepInfo.udp6730Info.voltage = m_udp6730->m_widgetVol->GetVal();
        m_stepInfo.udp6730Info.current = m_udp6730->m_widgetCur->GetVal();
    }
    QModelIndex modelIndex;
    if(DEV_UTG4082== devIndex) {
        m_stepInfo.utg4000Info.sg_ch = chIndex;
        m_stepInfo.utg4000Info.waveType = waveIndex;
        char* p = (char*)&m_stepInfo.utg4000Info;
        p += 2 * sizeof(double);                                       //memory alignment 8  : the first two int alignmented to one double, the third int alignmented to one double
        double* pdbegin = (double*)p;
        double* pd = pdbegin;
        switch (waveIndex) {
        case 0:
            pd = pdbegin;
            itemMod = m_utg4082->m_itemModSine;
            for(int i = 0; i < 4; ++i, ++pd) {
               modelIndex = itemMod->index(i, 0);
               *pd = (itemMod->data(modelIndex)).toDouble();
            }
            break;
        case 1:
            pd = pdbegin;
            itemMod = m_utg4082->m_itemModSquare;
            for(int i = 0; i < 4; ++i, ++pd) {
                if(3 == i)
                    ++pd;
                modelIndex = itemMod->index(i, 0);
                *pd = (itemMod->data(modelIndex)).toDouble();
            }
            break;
        case 2:
            pd = pdbegin;
            itemMod = m_utg4082->m_itemModPulse;
            for(int i = 0; i < 7; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = (itemMod->data(modelIndex)).toDouble();
            }
            break;
        case 3:
            itemMod = m_utg4082->m_itemModDC;
            m_stepInfo.utg4000Info.offset = (itemMod->data(modelIndex)).toDouble();
            break;
        case 4:
            pd = pdbegin;
            itemMod = m_utg4082->m_itemModRamp;
            for(int i = 0; i < 4; ++i, ++pd) {
                if(4 == i)
                    pd += 3;
                modelIndex = itemMod->index(i, 0);
                *pd = (itemMod->data(modelIndex).toDouble());
            }
            break;
        case 5:
            m_stepInfo.utl8500Info.loadMode = modIndex;
            modelIndex = itemMod->index(0, 0);
            itemMod = m_utg4082->m_itemModNoise;
            m_stepInfo.utg4000Info.amp = itemMod->data(modelIndex).toDouble();
            modelIndex = itemMod->index(1, 0);
            m_stepInfo.utg4000Info.offset = itemMod->data(modelIndex).toDouble();
            break;
        }
    }
    if(DEV_UTL8512 == devIndex) {
        char* p = (char*)&m_stepInfo.utl8500Info;
        p += 1 * sizeof(double);
        double* pdbegin = (double*)p;
        double* pd = pdbegin;
        switch (modIndex) {
        case 0:
            pd = pdbegin;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CC;
            modelIndex = itemMod->index(0, 0);
            m_stepInfo.utl8500Info.current = itemMod->data(modelIndex).toDouble();
            pd += 4;
            for(int i = 1; i < 4; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 1:
            pd = pdbegin;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CV;
            modelIndex = itemMod->index(0, 0);
            m_stepInfo.utl8500Info.voltage = itemMod->data(modelIndex).toDouble();
            pd += 4;
            for(int i = 1; i < 4; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 2:
            pd = pdbegin;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CR;
            modelIndex = itemMod->index(0, 0);
            m_stepInfo.utl8500Info.resistance = itemMod->data(modelIndex).toDouble();
            pd += 4;
            for(int i = 1; i < 4; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 3:
            pd = pdbegin;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CP;
            modelIndex = itemMod->index(0, 0);
            m_stepInfo.utl8500Info.power = itemMod->data(modelIndex).toDouble();
            pd += 4;
            for(int i = 1; i < 4; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 4:
            pd = pdbegin;
            pd += 7;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_DYNC;
            for(int i = 0; i < 8; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 5:
            pd = pdbegin;
            pd += 7;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_DYNV;
            for(int i = 0; i < 8; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 6:
            break;
        case 7:
            pd = pdbegin;
            pd += 14;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_OCP;
            for(int i = 0; i < 4; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 8:
            pd = pdbegin;
            pd += 14;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_OPP;
            for(int i = 0; i < 4; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 9:
            pd = pdbegin;
            pd += 18;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CR_LED;
            for(int i = 0; i < 3; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 10:
            pd = pdbegin;
            pd += 21;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CCBATTERY;
            for(int i = 0; i < 2; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 11:
            pd = pdbegin;
            pd += 21;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CRBATTERY;
            for(int i = 0; i < 2; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 12:
            pd = pdbegin;
            pd += 21;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_CPBATTERY;
            for(int i = 0; i < 2; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        case 13:
            pd = pdbegin;
            pd += 23;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_OVP;
            modelIndex = itemMod->index(0, 0);
            *pd = itemMod->data(modelIndex).toDouble();
            break;
        case 14:
            pd = pdbegin;
            pd += 24;
            itemMod = m_utl8512->m_utl8512TabView->m_mod_TIMING;
            for(int i = 0; i < 10; ++i, ++pd) {
                modelIndex = itemMod->index(i, 0);
                *pd = itemMod->data(modelIndex).toDouble();
            }
            break;
        }
    }
    QVariant var_stepInfo;
    var_stepInfo.setValue(m_stepInfo);
    emit SendStepInfo(var_stepInfo);
    this->close();
}

void TestStepInfowidget::DevChanged(int selectedIndex)
{
    switch (selectedIndex) {
    case DEV_UDP6730:
        m_grpUdp6730->show();
        m_grpUtg4082->hide();
        m_grpUtl8512->hide();
        break;
    case DEV_UTG4082:
        m_grpUtg4082->show();
        m_grpUdp6730->hide();
        m_grpUtl8512->hide();
        break;
    case DEV_UTL8512:
        m_grpUtl8512->show();
        m_grpUdp6730->hide();
        m_grpUtg4082->hide();
        break;
    default:
        break;
    }
}

