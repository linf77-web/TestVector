#include "Progressctrlwidget.h"
#include <QPushButton>
#include <QDebug>

ProgressCtrlwidget::ProgressCtrlwidget(QWidget *parent) : QWidget(parent)
{
    m_isInsertStep = false;
    memset(&m_stepInfo, 0, sizeof(m_stepInfo));
    QVariant var_stepInfo;
    var_stepInfo.setValue(m_stepInfo);
    InitName();
    m_hHeader << QStringLiteral("设备类型") << QStringLiteral("ID") << QStringLiteral("通道") << QStringLiteral("延时") << QStringLiteral("功能");
    m_tabViewProg = new QTableView(this);
    m_standItemMod = new QStandardItemModel(this);
    m_standItemMod->setHorizontalHeaderLabels(m_hHeader);
    QHeaderView* hHeaderView = m_tabViewProg->horizontalHeader();
    QHeaderView* vHeaderView = m_tabViewProg->verticalHeader();
    hHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    vHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    hHeaderView->setStyleSheet("QHeaderView::section {font-size: 14px;"
                               "background-color: #2EAD51;}");
    vHeaderView->setStyleSheet("QHeaderView::section {font-size: 14px;"
                               "background-color: #2EAD51;}");
    int listSize = LIST_MIN;
    for(int i = 0; i < listSize; ++i)
        m_vHeader << QString::number(i);
    m_standItemMod->setVerticalHeaderLabels(m_vHeader);
    m_tabViewProg->setModel(m_standItemMod);
    m_tabViewProg->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabViewProg->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabViewProg->setContextMenuPolicy(Qt::CustomContextMenu);

    m_txtInfoDetail = new QTextEdit(this);
    m_grpTxtInfo = new QGroupBox(QStringLiteral("Dev Detail"));
    m_hlayoutGrpDetail.addWidget(m_txtInfoDetail);
    m_grpTxtInfo->setLayout(&m_hlayoutGrpDetail);
    m_grpProg = new QGroupBox(QStringLiteral("流程"));
    m_hlayoutGrpProg.addWidget(m_tabViewProg);
    m_grpProg->setLayout(&m_hlayoutGrpProg);
    m_vlayoutProg.addWidget(m_grpProg);
    m_btnAddStep = new QPushButton(QStringLiteral("添加"), this);
    m_btnImportStep = new QPushButton(QStringLiteral("导入"), this);
    m_btnSaveStep = new QPushButton(QStringLiteral("保存"), this);
    m_hlayoutBtn.addWidget(m_btnImportStep);
    m_hlayoutBtn.addWidget(m_btnSaveStep);
    m_hlayoutBtn.addWidget(m_btnAddStep);
    m_vlayoutProg.addLayout(&m_hlayoutBtn);
    m_hlayoutProg.addLayout(&m_vlayoutProg, 2);
    m_hlayoutProg.addWidget(m_grpTxtInfo);
    this->setLayout(&m_hlayoutProg);
    m_stepWidget = new TestStepInfowidget();

    m_menue = new QMenu(m_tabViewProg);
    m_menue->addAction(QStringLiteral("插入"), this, SLOT(SlotInsertStep()));
    m_menue->addAction(QStringLiteral("删除"), this, SLOT(SlotDeletStep()));
    connect(m_btnAddStep, SIGNAL(clicked()), this, SLOT(SlotBtnAddStep()));

    connect(m_stepWidget, &TestStepInfowidget::SendStepInfo, this, &ProgressCtrlwidget::ReciveStepInfo);
    connect(m_tabViewProg->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ProgressCtrlwidget::SlotSelectionChanged);
    connect(m_tabViewProg, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SlotStepOperator(QPoint)));
    this->setStyleSheet("QPushButton, QLabel,{font-family: 'Microsoft YaHei';"
                        "font-size: 20px;"
                        "border-radius: 12px;"
                        "font-weight: bold;"
                        "font-position: top;"
                        "text-align: center;"
                        "max-width: 2000px;"
                        "min-width: 60px; "
                        "min-height:30px;"
                        "background-color: #41D1B0;"
                        "background-position: center;}"
                        "QLabel {padding-left: 15px;"
                        "padding-top: 2px;"
                        "padding-right: 10px;"
                        "padding-bottom: 2px;}"
                        "QPushButton:hover {background-color: #33A1C9;}"
                        "QPushButton:pressed {bacskground-color: #257FE6;}");
}

ProgressCtrlwidget::~ProgressCtrlwidget()
{
//    delete m_menue;
//    delete m_stepWidget;
//    delete m_btnSaveStep;
//    delete m_btnImportStep;
//    delete m_btnAddStep;
//    delete m_grpProg;
//    delete m_grpTxtInfo;
//    delete m_txtInfoDetail;
//    delete m_standItemMod;
//    delete m_tabViewProg;
}

void ProgressCtrlwidget::SlotStepOperator(QPoint p)
{
   QModelIndex index = m_tabViewProg->indexAt(p);
   if(index.isValid())                //数据项是否有效，空白处点击无菜单
      {
          m_selectedIndexs = m_tabViewProg->selectionModel()->selectedRows();
          //m_selections = m_tabViewProg->selectionModel();//获取当前的选择模型
          //m_selectedIndexs = m_selections->selectedIndexes();//返回当前选择的模型索引
          m_menue->exec(QCursor::pos());//数据项有效才显示菜单
      }
}

void ProgressCtrlwidget::SlotDeletStep()
{
    int listSize = m_selectedIndexs.size();
    if(listSize <= 0 || m_listStepInfo.size() <= 0)
        return ;

    int first = m_selectedIndexs.first().row();
    int last  = m_selectedIndexs.last().row();
    for(int row = last; row >= first; --row) {
        if(row > (int)(m_listStepInfo.size() - 1))
            continue;
        m_listStepInfo.erase(m_listStepInfo.begin() + row);
        m_standItemMod->removeRow(row);
    }
    m_vHeader.clear();
    for(int i = 0; i < LIST_MIN; ++i)
        m_vHeader << QString::number(i);
    m_standItemMod->setVerticalHeaderLabels(m_vHeader);
}

void ProgressCtrlwidget::SlotInsertStep()
{
    m_insertRow = m_selectedIndexs.first().row();
    m_isInsertStep = true;
    m_stepWidget->show();
}

void ProgressCtrlwidget::SlotBtnAddStep()
{
    m_isInsertStep = false;
    m_stepWidget->show();
}
void ProgressCtrlwidget::ReciveStepInfo(QVariant& vStepInfo)
{
    StepInfo stepInfo;
    memset(&stepInfo, 0, sizeof(m_stepInfo));
    stepInfo = vStepInfo.value<StepInfo>();
    if(m_isInsertStep)
        m_listStepInfo.insert(m_listStepInfo.begin() + m_insertRow, stepInfo);
    else
        m_listStepInfo.push_back(stepInfo);
    DisStepInfo();
}

void ProgressCtrlwidget::SlotSelectionChanged(const QItemSelection& selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    m_selectedIndexs = m_tabViewProg->selectionModel()->selectedRows();
    DisStepInfoDetail();
}


void ProgressCtrlwidget::InitName()
{
    m_vStepDevName.push_back(std::pair<int, QString>(0, QStringLiteral("UDP6730           ")));
    m_vStepDevName.push_back(std::pair<int, QString>(1, QStringLiteral("UTG4082           ")));
    m_vStepDevName.push_back(std::pair<int, QString>(2, QStringLiteral("UTL8512           ")));

    m_vStepDevFuc.push_back(std::pair<int, QString>(0, QStringLiteral("电源                ")));
    m_vStepDevFuc.push_back(std::pair<int, QString>(1, QStringLiteral("信号发生器           ")));
    m_vStepDevFuc.push_back(std::pair<int, QString>(2, QStringLiteral("负载                ")));

    m_vUdp6300Info.push_back(std::pair<int ,QString>(0,QStringLiteral("VolProt:            ")));
    m_vUdp6300Info.push_back(std::pair<int ,QString>(1,QStringLiteral("CurProt:            ")));
    m_vUdp6300Info.push_back(std::pair<int ,QString>(2,QStringLiteral("Voltage:            ")));
    m_vUdp6300Info.push_back(std::pair<int ,QString>(3,QStringLiteral("Current:            ")));

    m_vStepWaveName.push_back(std::pair<int, QString>(0, QStringLiteral("SINE:              ")));
    m_vStepWaveName.push_back(std::pair<int, QString>(1, QStringLiteral("SQUARE:            ")));
    m_vStepWaveName.push_back(std::pair<int, QString>(2, QStringLiteral("PULSE:             ")));
    m_vStepWaveName.push_back(std::pair<int, QString>(3, QStringLiteral("DC:                ")));
    m_vStepWaveName.push_back(std::pair<int, QString>(4, QStringLiteral("RAMP:              ")));
    m_vStepWaveName.push_back(std::pair<int, QString>(5, QStringLiteral("NOISE:             ")));

    m_vUtg4082Info.push_back(std::pair<int ,QString>(0,QStringLiteral("Freq:               ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(1,QStringLiteral("Amp:                ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(2,QStringLiteral("Offset:             ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(3,QStringLiteral("Phase:              ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(4,QStringLiteral("Duty:               ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(5,QStringLiteral("RiseTime:           ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(6,QStringLiteral("FallTtime:          ")));
    m_vUtg4082Info.push_back(std::pair<int ,QString>(7,QStringLiteral("Symmetry:           ")));

    m_vUtl8512Info.push_back(std::pair<int ,QString>(0,QStringLiteral("Current:            ")));        //0
    m_vUtl8512Info.push_back(std::pair<int ,QString>(1,QStringLiteral("Voltage:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(2,QStringLiteral("Resistance:         ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(3,QStringLiteral("Power:              ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(4,QStringLiteral("LoadVol:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(5,QStringLiteral("VolProt:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(6,QStringLiteral("CurProt:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(7,QStringLiteral("HighVal:            ")));       //7
    m_vUtl8512Info.push_back(std::pair<int ,QString>(8,QStringLiteral("HighTime:           ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(9,QStringLiteral("LowVal:             ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(10,QStringLiteral("LowTime:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(11,QStringLiteral("RiseRate:           ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(12,QStringLiteral("FallRate:           ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(13,QStringLiteral("WorkMode:           ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(14,QStringLiteral("ReptTime:           ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(15,QStringLiteral("BeginVal:           ")));       //14
    m_vUtl8512Info.push_back(std::pair<int ,QString>(16,QStringLiteral("StepVal:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(17,QStringLiteral("EndVal:             ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(18,QStringLiteral("EndTime:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(19,QStringLiteral("Led-vo:             ")));       //18
    m_vUtl8512Info.push_back(std::pair<int ,QString>(20,QStringLiteral("Led-io:             ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(21,QStringLiteral("RdSlop:             ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(22,QStringLiteral("DischargeVal:       ")));       //21
    m_vUtl8512Info.push_back(std::pair<int ,QString>(23,QStringLiteral("EndVol:             ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(24,QStringLiteral("TrigVol:            ")));      //23
    m_vUtl8512Info.push_back(std::pair<int ,QString>(25,QStringLiteral("LoadMod:            ")));      //24
    m_vUtl8512Info.push_back(std::pair<int ,QString>(26,QStringLiteral("LoadVal:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(27,QStringLiteral("LoadPrm:            ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(28,QStringLiteral("BeginTrigSrc:       ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(29,QStringLiteral("BeginTrigEdge:      ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(30,QStringLiteral("BeginTrigVal:       ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(31,QStringLiteral("EndTrigEdge:        ")));
    m_vUtl8512Info.push_back(std::pair<int ,QString>(32,QStringLiteral("EndTrigVal:         ")));

    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CC, QStringLiteral("恒流")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CV, QStringLiteral("恒压")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CR, QStringLiteral("恒阻")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CP, QStringLiteral("恒功率")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::DYNC, QStringLiteral("动态电流")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::DYNV, QStringLiteral("动态电压")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::MOD_LIST, QStringLiteral("列表")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::OCP, QStringLiteral("OCP")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::OPP, QStringLiteral("OPP")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::LED, QStringLiteral("LED")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CCBATTERY, QStringLiteral("电池恒电流")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CRBATTERY, QStringLiteral("电池恒电阻")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::CPBATTERY, QStringLiteral("电池恒功率")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::OVP, QStringLiteral("OVP")));
    m_vStepLoadModeName.push_back(std::pair<int, QString>(CMD_SOURCE_MODE::TIMING, QStringLiteral("时间测试")));
}

void ProgressCtrlwidget::DisStepInfo()
{
    int size = (int)m_listStepInfo.size();
    for(int row = 0; row < size; ++row) {
        QStandardItem* item = new QStandardItem(QString(m_vStepDevName[m_listStepInfo[row].devType].second));
        m_standItemMod->setItem(row, 0, item);
        item = new QStandardItem(QString::number(m_listStepInfo[row].id));
        m_standItemMod->setItem(row, 1, item);
        item = new QStandardItem(QString::number(m_listStepInfo[row].ch));
        m_standItemMod->setItem(row, 2, item);
        item = new QStandardItem(QString::number(m_listStepInfo[row].delay));
        m_standItemMod->setItem(row, 3, item);
        item = new QStandardItem(QString(m_vStepDevFuc[m_listStepInfo[row].fuc].second));
        m_standItemMod->setItem(row, 4, item);
    }
}

void ProgressCtrlwidget::DisStepInfoDetail()
{
    QString text;
    if(m_selectedIndexs.size() > 0 && m_listStepInfo.size() > 0) {
        int index = m_selectedIndexs.first().row();
        int size = (int)m_listStepInfo.size();
        if((index + 1) > size)
            return;

        text.append(QStringLiteral("设备类型:           ")).append(QString(m_vStepDevName[m_listStepInfo[index].devType].second)).append(QString("\n"));
        int* p = (int*)&m_listStepInfo[index].udp6730Info;
        double* pdbegin;
        double* pd;
        int i = 0;
        int devType = m_listStepInfo[index].devType;
        if(DEV_UDP6730 == devType) {
            p += 1 + 1 ;              //编译器优化内存按8字节对齐，int+int=double
            pdbegin = (double*)p;
            pd = pdbegin;
            for(int i = 0; pd < pdbegin + 4; ++pd, ++i)
                text.append(m_vUdp6300Info[i].second).append(QString::number(*pd)).append(QString("\n"));
        }
        if(DEV_UTG4082 == devType) {
            text.append(QStringLiteral("通道：              ")).append(QString::number(m_listStepInfo[index].utg4000Info.sg_ch)).append("\n");
            text.append(QStringLiteral("波形：              ")).append(m_vStepWaveName[m_listStepInfo[index].utg4000Info.waveType].second).append("\n");
            p = (int*)&m_listStepInfo[index].utg4000Info;
            p += 2 + 2;                //前两个int对齐称一个double，第三个int 对齐成一个double
            pdbegin = (double*)p;
            pd = pdbegin;

            while(pd < (pdbegin + 8)) {
                if( 0 == m_listStepInfo[index].utg4000Info.waveType)
                    if(i > 3)
                        break;
                if(1 == m_listStepInfo[index].utg4000Info.waveType)
                    if(i == 3 || i > 4) {
                        i++;
                        pd++;
                        continue;
                    }
                if(2 == m_listStepInfo[index].utg4000Info.waveType)
                    if(i > 6)
                        break;
                if(3 == m_listStepInfo[index].utg4000Info.waveType)
                    if(i != 2) {
                        i++;
                        pd++;
                        continue;
                    }
                if(4 == m_listStepInfo[index].utg4000Info.waveType)
                    if(i > 3 && i != 7) {
                        i++;
                        pd++;
                        continue;
                    }
                if(5 == m_listStepInfo[index].utg4000Info.waveType)
                    if(i != 1 && i !=2) {
                        i++;
                        pd++;
                        continue;
                    }
                text.append(m_vUtg4082Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                i++;
                pd++;
            }
        }
        if(DEV_UTL8512 == devType) {
            text.append(QStringLiteral("模式：              ")).append(m_vStepLoadModeName[m_listStepInfo[index].utl8500Info.loadMode].second).append("\n");
            p = (int*)&m_listStepInfo[index].utl8500Info;
            p += 1 + 1;                                       //两个int对齐成一个double
            pdbegin = (double*)p;
            pd = pdbegin;
            for(i = 0; pd < pdbegin + 33; ++i, ++pd) {
                if(0 <= m_listStepInfo[index].utl8500Info.loadMode && m_listStepInfo[index].utl8500Info.loadMode <= 3) {  //cc cv cp cr
                    if(i > 6)
                        break;
                text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                }
                if(4 <= m_listStepInfo[index].utl8500Info.loadMode && m_listStepInfo[index].utl8500Info.loadMode <= 5) {
                    pd += 7;
                    i +=7;
                    if(i > 13)
                        break;
                    for(; pd < pdbegin + 33; ++i,++pd) {
                        if(i > 13)
                            break;
                        text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                    }
                }
                if(6 == m_listStepInfo[index].utl8500Info.loadMode)
                    break;
                if(7 <= m_listStepInfo[index].utl8500Info.loadMode && m_listStepInfo[index].utl8500Info.loadMode <= 8) {
                    pd += 14;
                    i += 14;
                    if(i > 17)
                        break;
                    for(; pd < pdbegin + 33; ++i, ++pd) {
                        if(i > 17)
                            break;
                        text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                    }
                }
                if(9 == m_listStepInfo[index].utl8500Info.loadMode) {
                    pd += 18;
                    i += 18;
                    if(i > 20)
                        break;
                    for(; pd < pd + 33; ++i, ++pd) {
                        if(i >20)
                            break;
                        text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                    }
                }
                if(10 <= m_listStepInfo[index].utl8500Info.loadMode && m_listStepInfo[index].utl8500Info.loadMode <= 12) {
                    pd += 21;
                    i += 21;
                    if(i > 22)
                        break;
                    for(; pd < pd + 33; ++i, ++pd) {
                        if(i >22)
                            break;
                        text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                    }
                }
                if(13 == m_listStepInfo[index].utl8500Info.loadMode) {
                    pd += 23;
                    i += 23;
                    if(i > 23)
                        break;
                    for(; pd < pd + 33; ++i, ++pd) {
                        if(i > 23)
                            break;
                        text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                    }
                }
                if(14 == m_listStepInfo[index].utl8500Info.loadMode) {           //time
                    pd += 24;
                    i += 24;
                    if(i > 32)
                        break;
                    for(; pd < pd + 33; ++i, ++pd) {
                        if(i > 32)
                            break;
                        text.append(m_vUtl8512Info[i].second).append(QString::number(*pd)).append(QString("\n"));
                    }
                }
            }
        }
    }
    m_txtInfoDetail->setText(text);
}
