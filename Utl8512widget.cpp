#include "Utl8512widget.h"
#include <QtGlobal>

MinSetWidget::MinSetWidget(QString name, double value, QString unit, QWidget* parent) : QWidget(parent), m_name(name), m_value(value), m_unit(unit)
{
    m_labUnit = new QLabel(m_unit, this);
    m_labTitle = new QLabel(m_name, this);
    m_dSpinValue = new QDoubleSpinBox(this);
    m_dSpinValue->setValue(m_value);
    hlayout.addWidget(m_labTitle);
    hlayout.addWidget(m_dSpinValue);
    hlayout.addWidget(m_labUnit);
    this->setLayout(&hlayout); 
    this->setStyleSheet("QLabel{font-family: 'Microsoft YaHei';font-size: 20px;border-radius: 12px;font-weight: bold;text-align: center;min-width: 60px; min-height:30px;background-color: #6FAD7F;}"
                        "QPushButton{font-family: 'Microsoft YaHei';font-size: 20px;border-radius: 12px;font-weight: bold;text-align: center;min-width: 60px; min-height:30px;background-color: #6FAD7F;}"
                        "QPushButton:hover{background-color: #2EAD51;}"
                        "QPushButton:pressed{background-color: #257FE6;}");
}
MinSetWidget::~MinSetWidget()
{
//    delete m_labUnit;
//    delete m_labTitle;
//    delete m_dSpinValue;
//    m_labUnit = nullptr;
//    m_labTitle = nullptr;
//    m_dSpinValue = nullptr;
}

Utl8512widget::Utl8512widget(QWidget *parent) : QWidget(parent)
{
    m_lcdVoltageDis = new QLCDNumber(this);
    m_lcdVoltageDis->setMode(QLCDNumber::Dec);
    m_lcdVoltageDis->setSegmentStyle(QLCDNumber::Flat);
    m_lcdVoltageDis->setDigitCount(5);
    m_hlayOutDisItemV.addWidget(m_lcdVoltageDis, 2);
    m_labDisVoltage = new QLabel("V", this);
    m_hlayOutDisItemV.addWidget(m_labDisVoltage);

    m_lcdCurrentDis = new QLCDNumber(this);
    m_lcdCurrentDis->setMode(QLCDNumber::Dec);
    m_lcdCurrentDis->setSegmentStyle(QLCDNumber::Flat);
    m_lcdCurrentDis->setDigitCount(5);
    m_hlayOutDisItemC.addWidget(m_lcdCurrentDis, 2);
    m_labDisCurrent = new QLabel("I", this);
    m_hlayOutDisItemC.addWidget(m_labDisCurrent);

    m_lcdPowDis = new QLCDNumber(this);
    m_lcdPowDis->setMode(QLCDNumber::Dec);
    m_lcdPowDis->setSegmentStyle(QLCDNumber::Flat);
    m_lcdPowDis->setDigitCount(5);
    m_hlayOutDisItemP.addWidget(m_lcdPowDis, 2);
    m_labDisPower = new QLabel("P", this);
    m_hlayOutDisItemP.addWidget(m_labDisPower);
    m_hlayOutDis.addLayout(&m_hlayOutDisItemV);
    m_hlayOutDis.addLayout(&m_hlayOutDisItemC);
    m_hlayOutDis.addLayout(&m_hlayOutDisItemP);
    m_grpModeDis = new QGroupBox();
    m_grpModeDis->setLayout(&m_hlayOutDis);

    MinSetWidget* m_Vpp;
    MinSetWidget* m_Vp_plus;
    MinSetWidget* m_Vp_minus;
    MinSetWidget* m_Ipp;
    MinSetWidget* m_Ip_plus;
    MinSetWidget* m_Ip_minus;
    m_grpParamSetVpp = new QGroupBox();
    m_Vpp = new MinSetWidget(QString("Vpp"), 0.010f, QString("V"));
    m_Vp_plus = new MinSetWidget(QString("Vp+"), 0.010f, QString("V"));
    m_Vp_minus = new MinSetWidget(QString("Vp-"), 0.000f, QString("V"));
    m_vlayOutParamMinV.addWidget(m_Vpp);
    m_vlayOutParamMinV.addWidget(m_Vp_plus);
    m_vlayOutParamMinV.addWidget(m_Vp_minus);
    m_grpParamSetVpp->setLayout(&m_vlayOutParamMinV);
    m_hlayOutParamSetVppIpp.addWidget(m_grpParamSetVpp);
    m_grpParamSetIpp = new QGroupBox();
    m_Ipp = new MinSetWidget(QString("Ipp"), 0.000f, QString("A"));
    m_Ip_plus = new MinSetWidget(QString("Ip+"), 0.001f, QString("A"));
    m_Ip_minus = new MinSetWidget(QString("Ip-"), 0.000f, QString("A"));
    m_vlayOutParamMinI.addWidget(m_Ipp);
    m_vlayOutParamMinI.addWidget(m_Ip_plus);
    m_vlayOutParamMinI.addWidget(m_Ip_minus);
    m_grpParamSetIpp->setLayout(&m_vlayOutParamMinI);
    m_hlayOutParamSetVppIpp.addWidget(m_grpParamSetIpp);
    m_hlayOutAll.addWidget(m_grpModeDis);
    m_utl8512TabView = new Utl8512TabView();
    m_vlayOutParamSetAll.addWidget(m_utl8512TabView);
    m_vlayOutParamSetAll.addLayout(&m_hlayOutParamSetVppIpp, 1);
    m_hlayOutAll.addLayout(&m_vlayOutParamSetAll);
    this->setLayout(&m_hlayOutAll);
    this->setStyleSheet("QLCDNumber {"
                        "font-size: 10px;"
                        "font-weight: bold;"
                        "color: yellow;"
                        "border: 1px solid green;"
                        "background-color: black;"
                        "background-position: center;}");
}

Utl8512widget::~Utl8512widget()
{
    delete m_labDisVoltage;
    delete m_labDisCurrent;
    delete m_labDisPower;
    delete m_lcdVoltageDis;
    delete m_lcdCurrentDis;
    delete m_lcdPowDis;
    delete m_grpModeDis;
}

void Utl8512widget::Init()
{
}

void Utl8512TabView::InitMode()
{
    QStringList titles, units;
    m_mod_CC = new QStandardItemModel(4, 2);
    titles << QStringLiteral("电流设置") << QStringLiteral("带载电压") << QStringLiteral("过压保护") << QStringLiteral("过流保护");
    units << QStringLiteral("A") << QStringLiteral("V") << QStringLiteral("V") << QStringLiteral("A");
    SetViewHeader(m_mod_CC, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CV = new QStandardItemModel(4, 2);
    titles << QStringLiteral("电压设置") << QStringLiteral("带载电压") << QStringLiteral("过压保护") << QStringLiteral("过流保护");
    units << QStringLiteral("V") << QStringLiteral("V") << QStringLiteral("V") << QStringLiteral("A");
    SetViewHeader(m_mod_CV, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CR = new QStandardItemModel(4, 2, this);
    titles << QStringLiteral("电阻设置") << QStringLiteral("带载电压") << QStringLiteral("过压保护") << QStringLiteral("过流保护");
    units << QStringLiteral("Ω") << QStringLiteral("V") << QStringLiteral("V") << QStringLiteral("A");
    SetViewHeader(m_mod_CR, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CP = new QStandardItemModel(4, 2);
    titles << QStringLiteral("功率设置") << QStringLiteral("带载电压") << QStringLiteral("过压保护") << QStringLiteral("过流保护");
    units << QStringLiteral("W") << QStringLiteral("V") << QStringLiteral("V") << QStringLiteral("A");
    SetViewHeader(m_mod_CP, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_DYNC = new QStandardItemModel(8, 2);
    titles << QStringLiteral("高位拉载电流") << QStringLiteral("高位拉载电流时间") << QStringLiteral("低位拉载电流") << QStringLiteral("低位拉载电流时间")
           << QStringLiteral("电流上升率") << QStringLiteral("电流下降率") << QStringLiteral("工作模式") << QStringLiteral("重复次数");
    units << QStringLiteral("A") << QStringLiteral("ms") << QStringLiteral("A") << QStringLiteral("ms")
           << QStringLiteral("A/us") << QStringLiteral("A/us") << QStringLiteral(" ") << QStringLiteral(" ");
    SetViewHeader(m_mod_DYNC, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_DYNV = new QStandardItemModel(8, 2);
    titles << QStringLiteral("高位拉载电压") << QStringLiteral("高位拉载电压时间") << QStringLiteral("低位拉载电压") << QStringLiteral("低位拉载电压时间")
            << QStringLiteral("电压上升率") << QStringLiteral("电压下降率") << QStringLiteral("工作模式") << QStringLiteral("重复次数");
    units  << QStringLiteral("V") << QStringLiteral("ms") << QStringLiteral("V") << QStringLiteral("ms")
           << QStringLiteral("V/us") << QStringLiteral("V/us") << QStringLiteral(" ") << QStringLiteral(" ");
    SetViewHeader(m_mod_DYNV, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_List = new QStandardItemModel(7, 7);
    titles << QStringLiteral("序号") << QStringLiteral("模式") << QStringLiteral("定值") << QStringLiteral("定时/ms")
           << QStringLiteral("检查") << QStringLiteral("下限") << QStringLiteral("上限");
    titles << QStringLiteral("1") << QStringLiteral("2") << QStringLiteral("2") << QStringLiteral("4")
           << QStringLiteral("5") << QStringLiteral("6") << QStringLiteral("7");
    SetViewHeader(m_mod_List, Qt::Horizontal, titles, units);
    titles.clear();
    units.clear();

    m_mod_OCP = new QStandardItemModel(4, 2);
    titles << QStringLiteral("起始电流") << QStringLiteral("步进电流") << QStringLiteral("截止电流")
           << QStringLiteral("步进时间");
    units << QStringLiteral("A") << QStringLiteral("A") << QStringLiteral("A")
           << QStringLiteral("ms");
    SetViewHeader(m_mod_OCP, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_OPP = new QStandardItemModel(4, 2);
    titles << QStringLiteral("起始功率") << QStringLiteral("步进功率") << QStringLiteral("截止功率")
           << QStringLiteral("步进时间");
    units << QStringLiteral("W") << QStringLiteral("W") << QStringLiteral("W")
           << QStringLiteral("ms");
    SetViewHeader(m_mod_OPP, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CR_LED = new QStandardItemModel(3, 2);
    titles << QStringLiteral("LED-Vo") << QStringLiteral("LED-Io") << QStringLiteral("Rd系数");
    units << QStringLiteral("V") << QStringLiteral("A") << QStringLiteral(" ");
    SetViewHeader(m_mod_CR_LED, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CCBATTERY  = new QStandardItemModel(2, 2);
    titles << QStringLiteral("放电电流值") << QStringLiteral("截止电压");
    units << QStringLiteral("A") << QStringLiteral("V");
    SetViewHeader(m_mod_CCBATTERY, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CRBATTERY  = new QStandardItemModel(2, 2);
    titles << QStringLiteral("放电电阻值") << QStringLiteral("截止电压");
    units << QStringLiteral("Ω") << QStringLiteral("V");
    SetViewHeader(m_mod_CRBATTERY, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_CPBATTERY  = new QStandardItemModel(2, 2);
    titles << QStringLiteral("放电功率值") << QStringLiteral("截止电压");
    units << QStringLiteral("W") << QStringLiteral("V");
    SetViewHeader(m_mod_CPBATTERY, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_OVP = new QStandardItemModel(1, 2);
    titles << QStringLiteral("触发电压");
    units << QStringLiteral("V");
    SetViewHeader(m_mod_OVP, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_mod_TIMING = new QStandardItemModel(9, 1);
    titles << QStringLiteral("带载模式") << QStringLiteral("带载值") << QStringLiteral("带载参数") << QStringLiteral("起始触发源")
           << QStringLiteral("起始触发沿") << QStringLiteral("起始触发电平") << QStringLiteral("结束触发源") << QStringLiteral("结束触发沿") << QStringLiteral("结束触发电平");
    units << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral(" ")
          << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral(" ") << QStringLiteral(" ");
    SetViewHeader(m_mod_TIMING, Qt::Vertical, titles, units);
    titles.clear();
    units.clear();

    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CC, m_mod_CC));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CV, m_mod_CV));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CR, m_mod_CR));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CP, m_mod_CP));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::DYNC, m_mod_DYNC));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::DYNV, m_mod_DYNV));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::MOD_LIST, m_mod_List));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::OCP, m_mod_OCP));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::OPP, m_mod_OPP));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::LED, m_mod_CR_LED));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CCBATTERY, m_mod_CCBATTERY));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CRBATTERY, m_mod_CRBATTERY));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::CPBATTERY, m_mod_CPBATTERY));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::OVP, m_mod_OVP));
    m_vItemPair.push_back(std::pair<int, QStandardItemModel*>(CMD_SOURCE_MODE::TIMING, m_mod_TIMING));
}

void Utl8512TabView::SetViewHeader(QStandardItemModel* itemModel, Qt::Orientation direction, QStringList& strTitles, QStringList& strUnits)
{
    int size = strTitles.size();
    for(int i = 0; i < size; ++i) {
        int k = 1;
        itemModel->setHeaderData(i, direction, strTitles[i]);
        if(m_mod_List == itemModel) {
            int sizeCol = strUnits.size();
            for(int j = 0; j < sizeCol; ++j)
                itemModel->setHeaderData(j, Qt::Vertical, strUnits[j]);
        }
        else {
            while(k <= 1) {
                itemModel->setHeaderData(0, Qt::Horizontal, QStringLiteral("设定值"));
                itemModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("单位"));
                ++k;
            }
            QStandardItem* item = new QStandardItem(strUnits[i]);
            itemModel->setItem(i, 1, item);
            //Can not delete item, will occurs bug;
        }
    }
}

void Utl8512TabView::RefrsParmView(int modeIndex)
{
    this->m_tabViewParamSetMain->setModel(m_vItemPair[modeIndex].second);
}
Utl8512TabView::Utl8512TabView(QWidget *parent) : QWidget(parent)
{
    m_tabViewParamSetMain = new QTableView(this);
    InitMode();
    m_cmbModChoose = new QComboBox(this);
    m_labModChoose = new QLabel(QStringLiteral("模式选择:"));
    QStringList strListMode;
    strListMode << QStringLiteral("恒流") << QStringLiteral("恒压") << QStringLiteral("恒阻") <<QStringLiteral("恒功率") << QStringLiteral("动态电流测试")
                << QStringLiteral("动态电压测试")<< QStringLiteral("列表测试") << QStringLiteral("OCP") << QStringLiteral("OPP") << QStringLiteral("LED")
                << QStringLiteral("电池恒流放电") << QStringLiteral("电池恒阻放电") << QStringLiteral("电池恒功率放电") << QStringLiteral("OVP") << QStringLiteral("时间测试");
    m_cmbModChoose->addItems(strListMode);
    m_tabViewParamSetMain->setModel(m_vItemPair[m_cmbModChoose->currentIndex()].second);
    DoubleSpinDeleget* dSpinBoxdeleget = new DoubleSpinDeleget();
    m_tabViewParamSetMain->setItemDelegateForColumn(0, dSpinBoxdeleget);
    m_hlayOutModChoose.addWidget(m_labModChoose);
    m_hlayOutModChoose.addWidget(m_cmbModChoose, 2);
    m_vlayOutParamSetAll.addLayout(&m_hlayOutModChoose);
    m_vlayOutParamSetAll.addWidget(m_tabViewParamSetMain, 2);
    this->setLayout(&m_vlayOutParamSetAll);
    connect(m_cmbModChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(RefrsParmView(int)));
    QHeaderView* hHeaderView = m_tabViewParamSetMain->horizontalHeader();
    hHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    QHeaderView* vHeaderView = m_tabViewParamSetMain->verticalHeader();
    vHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    hHeaderView->setStyleSheet("QHeaderView::section {font-size: 14px;"
                               "background-color: #2EAD51;}");
    vHeaderView->setStyleSheet("QHeaderView::section {font-size: 14px;"
                               "background-color: #2EAD51;}");
}

Utl8512TabView::~Utl8512TabView()
{
    delete m_cmbModChoose;
    delete m_labModChoose;
    delete m_tabViewParamSetMain;
    delete m_mod_CC;
    delete m_mod_CV;
    delete m_mod_CR;
    delete m_mod_CP;
    delete m_mod_DYNC;
    delete m_mod_DYNV;
    delete m_mod_List;
    delete m_mod_OCP;
    delete m_mod_OPP;
    delete m_mod_CR_LED;
    delete m_mod_CCBATTERY;
    delete m_mod_CRBATTERY;
    delete m_mod_CPBATTERY;
    delete m_mod_OVP;
    delete m_mod_TIMING;
}
COM* g_utl8512Com[DEV_MAX_CNT];
std::vector<std::string> g_vUtl8512DevComs;
