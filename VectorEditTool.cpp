#include "VectorEditTool.h"

inline bool isExprValid(const int& period, const QString& qstr)
{
    int expr = 0;
    std::string str = std::string(qstr.toStdString());
    unsigned long long pos = str.find("ns");
    if (str.npos != pos) {
        std::string substr = str.substr(0, pos);
        sscanf(substr.c_str(), "%d", &expr);
    }
    else {
        pos = str.find("us");
        if (str.npos != pos) {
            std::string substr = str.substr(0, pos);
            sscanf(substr.c_str(), "%d", &expr);
        }
    }
    sscanf(str.c_str(), "%d", &expr);
    return expr <= period ? true : false;
}

VectorTool::VectorTool(QWidget *parent)
    : QMainWindow(parent)
{
    m_timingWnd = nullptr;
    m_sigWnd = nullptr;
    m_waveWnd = nullptr;
    m_model = nullptr;
    m_signalCnts = 0;
    m_vectorSize = 0;
    m_pattCnt = 0;
    m_row = 0;
    InitWnd();
    int cnt = 0;
    while (cnt < SIGNAL_WFC_CNTS_MAX) {
        m_curPattItem[cnt] = nullptr;
        m_tempWfc[cnt] = nullptr;
        m_curPattItem[cnt++] = nullptr;
        m_tempEvent[cnt++] = nullptr;
    }
    cnt = SIGNAL_WFC_CNTS_MAX;
    while (cnt < EVT_CNTS_MAX)
        m_tempEvent[cnt++] = nullptr;
    
}

VectorTool::~VectorTool()
{
    int cnt = 0;
    while(cnt < SIGNAL_WFC_CNTS_MAX) {
        if (nullptr != m_curPattItem[cnt]) 
            delete m_curPattItem[cnt++];
        else
            break;
    }
    cnt = 0;
    while (cnt < SIGNAL_WFC_CNTS_MAX) {
        if (nullptr != m_tempWfc[cnt])
            delete m_tempWfc[cnt++];
        else
            break;
    }
    cnt = 0;
    while (cnt < EVT_CNTS_MAX)
        if (nullptr != m_tempEvent[cnt])
            delete m_tempEvent[cnt++];
        else
            break;

}

void VectorTool::ClearArry()
{
    if (0 != m_vectorSize)
        for (int i = 0; i < m_vectorSize; ++i)
            delete[] m_wfcArry[i];
    if (0 != m_signalCnts)
        for (int i = 0; i < m_signalCnts; ++i)
            delete[] m_wfcArryReverse[i];
}
void VectorTool::closeEvent(QCloseEvent *)
{
    if(nullptr != m_timingWnd)
        m_timingWnd->deleteLater();
    if(nullptr != m_sigWnd)
        m_sigWnd->deleteLater();
    if(nullptr != m_waveWnd)
        m_waveWnd->deleteLater();
}

void VectorTool::InitWnd()
{
    m_menueBar = new QMenuBar(this);
    m_menueTool = new QMenu("Tool", m_menueBar);
    m_menueBar->addMenu(m_menueTool);
    m_menueCreate= new QMenu(QStringLiteral("Create"), m_menueTool);
    m_actWave = new QAction(QStringLiteral("WaveWindow"));
    m_menueTool->addMenu(m_menueCreate);
    m_menueTool->addAction(m_actWave);
    m_actCreateTim = new QAction(QStringLiteral("Create Timing"));
    m_actCreateVector = new QAction(QStringLiteral("Create new Vector"));
    QList<QAction*> listActs;
    listActs << m_actCreateTim << m_actCreateVector;
    m_menueCreate->addActions(listActs);

    m_sigWnd = new SignalWnd("SignalWindow", this);
    m_timingWnd = new TimingWnd("TimingWindow", this);
    m_waveWnd = new StilParserTool(this);
    m_waveWnd->move(100, 100);

    m_model = new QStandardItemModel(this);
    QStringList strlist;
    strlist << "TimingName" << "Signal" << "WFC" << "Command" << "Label";
    m_model->setColumnCount(5);
    m_model->setHorizontalHeaderLabels(strlist);
    m_vectorView = new QTableView(this);
    m_vectorView->setModel(m_model);
    m_vectorView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_vectorView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_vlayoutFinal.addWidget(m_vectorView);
    //strlist.clear();
    //strlist << "Loop" << "Insert" << "Jump";
    //m_vectorView->setItemDelegateForColumn(3, new ComboBoxDeleget(strlist));
    this->setMenuBar(m_menueBar);
    m_widget = new QWidget(this);
    m_widget->setLayout(&m_vlayoutFinal);
    this->setCentralWidget(m_widget);
    m_menueRightClick = new QMenu(m_vectorView);
    m_menueRightClick->addAction(QStringLiteral("Append"), this, SLOT(SlotAppendRow()));
    m_menueRightClick->addAction(QStringLiteral("Insert"), this, SLOT(SlotInsertRow()));
    m_menueRightClick->addAction(QStringLiteral("Delete"), this, SLOT(SlotDeleteRows()));
    InitStyle();
    connect(m_actCreateTim, SIGNAL(triggered()), this, SLOT(ShowTimWnd()));
    connect(m_actCreateVector, SIGNAL(triggered()), this, SLOT(SlotAppendRow()));
    connect(m_actWave, SIGNAL(triggered()), this, SLOT(ShowWaveWnd()));
    connect(m_timingWnd, &TimingWnd::SendVtims, this, &VectorTool::SlotRecivevTims);
    connect(m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(SlotTimNameChanged(QStandardItem*)));
    connect(m_vectorView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createContextMenu(const QPoint&)));
}

void VectorTool::ShowSigWnd()
{
    if(nullptr != m_sigWnd)
        m_sigWnd->show();
}

void VectorTool::ShowTimWnd()
{
    if(nullptr != m_timingWnd)
        m_timingWnd->show();
}


void VectorTool::ShowWaveWnd()
{
    GenerateTestVector();
    int end = (int)m_vectorFromUI.size() - 1;
    int begin = 0;
    GenerateTestVector(begin, end, 1);
    GetWfcArry();
    ReverseArry();
    SetEdgeTableToWaveWnd();
    if(nullptr != m_waveWnd)
        m_waveWnd->show();
    if (m_waveWnd->isVisible())
        m_waveWnd->UpdateWaveView();
}

void VectorTool::SlotAppendRow()
{
    if(m_vtims.empty()) {
         QMessageBox::critical(this, QStringLiteral("添加Vector 错误"), QStringLiteral("Timing为空: 请先创建Timing"), QMessageBox::Ok);
         return;
    }
    m_model->appendRow(new QStandardItem(""));
    m_model->setItem(m_row, 4, new QStandardItem(QString::number(m_row)));
    m_model->setItem(m_row++, 3, new QStandardItem(QString("Nop")));
}

void VectorTool::SlotInsertRow()
{
    m_model->insertRow(m_selectedIndexs.first().row());
    m_model->setItem(m_row++, 3, new QStandardItem(QString("Nop")));
    m_model->setItem(m_selectedIndexs.first().row(), 4, new QStandardItem(QString::number(m_row++)));
}

void VectorTool::SlotDeleteRows()
{
    m_model->removeRows(m_selectedIndexs.first().row(), m_selectedIndexs.count());
    m_row -= m_selectedIndexs.count();
}

void VectorTool::SlotRecivevTims(const QVariant& vtims)
{
    m_listTimNames.clear();
    m_vtims = vtims.value<std::vector<Timing*>>();
    int size = (int)m_vtims.size();
    if(size <= 0)
        return;
    for(int index = 0; index < size; ++index)
        m_listTimNames << m_vtims[index]->timname();
    m_vectorView->setItemDelegateForColumn(0, new ComboBoxDeleget(m_listTimNames, this));
}

void VectorTool::createContextMenu(const QPoint & p)
{
   QModelIndex index = m_vectorView->indexAt(p);
   if(index.isValid()) {
          m_selectedIndexs = m_vectorView->selectionModel()->selectedRows();
          m_menueRightClick->exec(QCursor::pos());
   }
}

void VectorTool::SlotTimNameChanged(QStandardItem * item)
{
    if(m_vtims.empty())
        return;
    int row = item->row();
    int col = item->column();
    QString sigNames = "";
    QString wfcNames = "";
    if(0 == col) {
       int tim_size = (int)m_vtims.size();
       for(int tim_index = 0; tim_index < tim_size; ++tim_index) {
           if(0 == strcmp(item->text().toStdString().c_str(), m_vtims[tim_index]->timname())) {
               int sig_size = (int)m_vtims[tim_index]->vsignals().size();
               for(int sig_index = 0; sig_index < sig_size; ++sig_index) {
                   sigNames += m_vtims[tim_index]->signal(sig_index)->signame();
                   sigNames += ",";
                   if(m_vtims[tim_index]->signal(sig_index)->vwfc().empty())
                       continue;
                   wfcNames += m_vtims[tim_index]->signal(sig_index)->wfc(0)->wfcname();
                   wfcNames += ",";
               }
           }
       }
       m_model->setItem(row, col + 1, new QStandardItem(sigNames));
       m_model->setItem(row, col + 2, new QStandardItem(wfcNames));

       if("" == m_model->item(0, 0)->text())
           QMessageBox::warning(this, QStringLiteral("添加Vector 错误"), QStringLiteral("第一条Vector timing不能为空"), QMessageBox::Ok);
           return;
    }
    else
        return;
}

void VectorTool::InitStyle()
{
    //Load StyleSheet;
    //QFile file(":/qss/psblack.css");
//    QFile file(":/qss/flatwhite.css");
      QFile file(":/qss/lightblue.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        this->setPalette(QPalette(QColor(paletteColor)));
        this->setStyleSheet(qss);
        file.close();
    }
}

void VectorTool::GenerateTestVector()
{
    std::vector<std::string> stdStrVsigNames;
    std::vector<const char*> vWfcNames;
    std::vector<WFC*> vwfcs;
    char pNames[STR_NAME_MIN * MAX_SIGNAL_NUM] = { 0 };
    int rowCnts = m_model->rowCount();
    m_vectorFromUI.clear();
    for(int row = 0; row < rowCnts; ++row) {
        stdStrVsigNames.clear();
        vWfcNames.clear();
        vwfcs.clear();
        Timing* timing;
        WFC* wfc;
        for (auto tim : m_vtims)
            if (0 == strcmp(tim->timname(), m_model->item(row, 0)->text().toStdString().c_str())) {
                timing = tim;
                break;
            }
        size_t len = strlen(m_model->item(row, 1)->text().toStdString().c_str());
        if (0 == len)
            return;
        memset(pNames, 0, STR_NAME_MIN * MAX_SIGNAL_NUM);
        memcpy(pNames, m_model->item(row, 1)->text().toStdString().c_str(), len);
        char* p = pNames;
        char* subStr = { 0 };
        if (nullptr != strstr(p, ",")) {
            while (nullptr != (subStr = strtok(p, ","))) {
                p = nullptr;
                stdStrVsigNames.push_back(std::string(subStr));
            }
        }

        len = strlen(m_model->item(row, 1)->text().toStdString().c_str());
        if (0 == len)
            return;
        memset(pNames, 0, STR_NAME_MIN * MAX_SIGNAL_NUM);
        memcpy(pNames, m_model->item(row, 2)->text().toStdString().c_str(), len);
        
        p = pNames;
        subStr = { 0 };
        if (nullptr != strstr(p, ",")) {
            while (nullptr != (subStr = strtok(p, ","))) {
                p = nullptr;
                vWfcNames.push_back(subStr);
            }
        }
        size_t sizeSig = stdStrVsigNames.size();
        size_t sizeWfc = vWfcNames.size();
        if(sizeSig != sizeWfc) {
            QMessageBox::critical(this, QStringLiteral("向量生成错误"), QStringLiteral("SigName， wfc 数量不一致"), QMessageBox::Ok);
            return;
        }
        for(size_t num = 0; num < sizeSig; ++num) {
            wfc = timing->getSignalByName(stdStrVsigNames[num].c_str())->getWfcByName(vWfcNames[num]);
            vwfcs.push_back(wfc);
        }
        m_curPattItem[m_pattCnt] = new PatternItem(timing->period(), vwfcs, stdStrVsigNames, m_model->item(row, 3)->text().toStdString().c_str(), m_model->item(row, 4)->text().toStdString().c_str());
        m_vectorFromUI.push_back(m_curPattItem[m_pattCnt++]);
    }
    if(!m_vectorFromUI.empty())
        m_firstPattenItem = m_vectorFromUI[0];
}

void VectorTool::GenerateTestVector(int begin, int end, int cnts)
{
    char cmd[STR_NAME_MIN] = { 0 };
    char label[STR_NAME_MIN] = { 0 };
    char opCode[STR_NAME_MIN] = { 0 };
    int opNumber = 0;
    int lab = 0;
    
    std::vector<const char*> vcmd;
    if (m_vectorFromUI.empty())
        return;

    for (int cnt = 0; cnt < cnts; ++cnt) {
        for (int index = begin; index <= end; ++index) {
            memset(cmd, 0, STR_NAME_MIN);
            strcat(cmd, m_vectorFromUI[index]->cmd());
            memset(label, 0, STR_NAME_MIN);
            strcat(label, m_vectorFromUI[index]->label());

            char* p = cmd;
            char* substr = { 0 };
            if (nullptr != strstr(p, " ")) {
                while (nullptr != (substr = strtok(p, " "))) {
                    p = nullptr;
                    vcmd.push_back(substr);
                }
            }
            p = cmd;
            if (nullptr != strstr(p, "Nop") || nullptr != strstr(p, "NOP") || nullptr != strstr(p, "nop")) {
                m_curPattItem[m_pattCnt] = new PatternItem(m_vectorFromUI[index]);
                m_vectorFinal.push_back(m_curPattItem[m_pattCnt++]);
                continue;
            }
            sscanf(label, "%d", &lab);
            if (nullptr != strstr(p, "JUMP") || nullptr != strstr(p, "jump") || nullptr != strstr(p, "Jump")) {
                if (0 == lab) {
                    QMessageBox::critical(this, QStringLiteral("向量生成错误"), QStringLiteral("lab不能为0或者字符"), QMessageBox::Ok);
                    return;
                }
                index = lab - 2;
                continue;
            }
            if (vcmd.size() > 2) {
                QMessageBox::critical(this, QStringLiteral("向量生成错误"), QStringLiteral("操作码操作数格式不正确"), QMessageBox::Ok);
                return;
            }
            memcpy(opCode, vcmd.front(), STR_NAME_MIN);
            size_t vcmdSize = vcmd.size();
            if (2 == vcmdSize) {
                sscanf(vcmd[1], "%d", &opNumber);
                if (0 == opNumber) {
                    QMessageBox::critical(this, QStringLiteral("向量生成错误"), QStringLiteral("操作码不能为0"), QMessageBox::Ok);
                    return;
                }
                if (0 == strcmp("Repeate", opCode) || 0 == strcmp("REPEATE", opCode) || 0 == strcmp("repeate", opCode)) {
                    m_curPattItem[m_pattCnt] = new PatternItem(m_vectorFromUI[index]->period(), m_vectorFromUI[index]->vwfcs(), m_vectorFromUI[index]->vsigNames(), "Nop", m_vectorFromUI[index]->label());
                    m_vectorFinal.push_back(m_curPattItem[m_pattCnt++]);
                    for (int repeatCnt = 0; repeatCnt < opNumber; ++repeatCnt) {
                        m_curPattItem[m_pattCnt] = new PatternItem(m_vectorFromUI[index]->period(), m_vectorFromUI[index]->vwfcs(), m_vectorFromUI[index]->vsigNames(), "Nop", m_vectorFromUI[index]->label());
                        m_vectorFinal.push_back(m_curPattItem[m_pattCnt++]);
                    }
                    vcmd.clear();
                    continue;
                }
                if (0 == strcmp("Loop", opCode) || 0 == strcmp("LOOP", opCode) || 0 == strcmp("loop", opCode)) {
                    if (0 == lab) {
                        QMessageBox::critical(this, QStringLiteral("向量生成错误"), QStringLiteral("lab不能为0或者字符"), QMessageBox::Ok);
                        return;
                    }
                    if ((lab - 1) > end) {
                        QMessageBox::critical(this, QStringLiteral("向量生成错误"), QStringLiteral("循环嵌套越界"), QMessageBox::Ok);
                        return;
                    }
                    m_curPattItem[m_pattCnt] = new PatternItem(m_vectorFromUI[index]->period(), m_vectorFromUI[index]->vwfcs(), m_vectorFromUI[index]->vsigNames(), "Nop", m_vectorFromUI[index]->label());
                    m_vectorFinal.push_back(m_curPattItem[m_pattCnt++]);
                    GenerateTestVector(index + 1, lab - 1, opNumber);
                    index = lab - 1;
                }
            }
            vcmd.clear();
        }
    }
}

char VectorTool::getEvtType(const char* evt_event)
{
    if (0 == strcmp("D", evt_event))
        return EventType_ForceDown;
    if (0 == strcmp("U", evt_event))
        return EventType_ForceUp;
    if (0 == strcmp("H", evt_event))
        return EventType_ExpectHigh;
    if (0 == strcmp("L", evt_event))
        return EventType_ExpectLow;
    if (0 == strcmp("X", evt_event))
        return EventType_CompareUnknown;
    if (0 == strcmp("Z", evt_event))
        return EventType_ForceOff;
    if (0 == strcmp("P", evt_event))
        return EventType_ForcePrior;
    if (0 == strcmp("M", evt_event))
        return EventType_CompareValid;
    return -1;
}
int VectorTool::getEvtOffset(const char* evt_expr)
{
    int offset = 0;
    std::string str = std::string(evt_expr);
    unsigned long long pos = str.find("ns");
    if (str.npos != pos) {
        std::string substr = str.substr(0, pos);
        sscanf(substr.c_str(), "%d", &offset);
        return offset;
    }
    else {
        pos = str.find("us");
        if (str.npos != pos) {
            std::string substr = str.substr(0, pos);
            sscanf(substr.c_str(), "%d", &offset);
            return offset * 1000;
        }
    }
    sscanf(str.c_str(), "%d", &offset);
    return offset;
}

void VectorTool::GetWfcArry()
{
    static int evtCnt = 0;
    static int wfcCnt = 0;
    m_signalCnts = (int)m_firstPattenItem.vsigNames().size();
    m_vectorSize = (int)m_vectorFinal.size();
    if (0 == m_vectorSize) {
        QMessageBox::critical(this, QStringLiteral("向量错误"), QStringLiteral("生成向量表为空"), QMessageBox::Ok);
        return;
    }
    std::vector<WFC*> vwfcDebug;
    m_wfcArry = new WFC* [m_vectorSize];
    std::vector<Event*> vEvents;
    for (int i = 0; i < m_vectorSize; ++i)
        m_wfcArry[i] = new WFC[m_signalCnts];

    //for (int col = 0; col < m_signalCnts; ++col) {                                                           
    //    m_wfcArry[0][col] = m_firstPattenItem.wfc(col);  
    //    vwfcDebug.push_back(&m_wfcArry[0][col]);
    //    for (int i = 0; i < 4; ++i) {                                                                        
    //        m_tempEvent[evtCnt] = new Event(m_firstPattenItem.wfc(col)->vevt().back());
    //        vEvents.push_back(m_tempEvent[evtCnt++]);
    //    }                                                                                                    
    //    m_tempWfc[wfcCnt] = new WFC(m_firstPattenItem.wfc(col)->wfcname(), vEvents);
    //} 
    //vEvents.clear();

    //int lastPeriod = m_firstPattenItem.period();
    int lastPeriod = 0;
    for (int row = 0; row < m_vectorSize; ++row) {
        int sig_size = (int)m_vectorFinal[row]->vsigNames().size();
        for (int cur_col = 0; cur_col < sig_size; ++cur_col) {
            //assignment 
            for (int index = 0; index < 4; ++index) {
                m_tempEvent[evtCnt] = new Event(m_vectorFinal[row]->wfc(cur_col)->evt(index)->evt(), m_vectorFinal[row]->wfc(cur_col)->evt(index)->expr());
                vEvents.push_back(m_tempEvent[evtCnt++]);
            }
            int offset = getEvtOffset(vEvents.back()->expr());
            if (offset < m_vectorFinal[row]->period()) {
                char exprBuf[STR_NAME_MIN] = { 0 };
                sprintf(exprBuf, "%d", m_vectorFinal[row]->period());
                m_tempEvent[evtCnt] = new Event(vEvents.back()->evt(), exprBuf);
            }
            vEvents.push_back(m_tempEvent[evtCnt++]);
            m_tempWfc[wfcCnt] = new WFC(m_vectorFinal[row]->wfc(cur_col)->wfcname(), vEvents);
            vEvents.clear();
                
            if (sig_size == m_signalCnts) {
                if (m_vectorFinal[row]->sigName(cur_col) == m_firstPattenItem.sigName(cur_col)) {
                    m_wfcArry[row][cur_col] = m_tempWfc[wfcCnt++];
                    m_wfcArry[row][cur_col].setEventOffset(lastPeriod);
                }
            }
            else {
                QMessageBox::critical(this, QStringLiteral("向量错误"), QStringLiteral("当前第%1条向量通道数量和初始通道数量不一致").arg(row), QMessageBox::Ok);
                return;
            }
        }
        lastPeriod += m_vectorFinal[row]->period();
    }
}

void VectorTool::ReverseArry()
{
    m_wfcArryReverse = new WFC * [m_signalCnts];
    for (int i = 0; i < m_signalCnts; ++i)
        m_wfcArryReverse[i] = new WFC[m_vectorSize];
    for (int i = 0; i < m_signalCnts; ++i)
        for (int j = 0; j < m_vectorSize; ++j)
            m_wfcArryReverse[i][j] = m_wfcArry[j][i];
}

void VectorTool::SetEdgeTableToWaveWnd()
{   
    SignalTruthTable table;
    EdgeList edgeList;
    Edge edge;
    for (int i = 0; i < m_signalCnts; ++i) {
        edgeList.clear();
        for (int j = 0; j < m_vectorSize; ++j) {
            int evtSize = (int)m_wfcArryReverse[i][j].vevt().size();
            for (int index = 0; index < evtSize; ++index) {
                edge.event = getEvtType(m_wfcArryReverse[i][j].evt(index)->evt());
                edge.offset = getEvtOffset(m_wfcArryReverse[i][j].evt(index)->expr());
                edgeList.push_back(edge);
            }
        }
        table.push_back(edgeList);
    }
    m_waveWnd->GetTableFromTestVector(table);
    m_waveWnd->SetViewSource(false);
    QStringList sigNameList;
    for(auto sigName : m_firstPattenItem.vsigNames())
        sigNameList << QString::fromStdString(sigName);
    m_waveWnd->SetSignals(sigNameList);
    table.clear();
    ClearArry();
    m_vectorFromUI.clear();
    m_vectorFinal.clear();
}

TimingWnd::TimingWnd(const QString &title, QWidget *parent) : QDialog(parent), m_title(title)
{
    m_timCnt = 0;
    int cnt = 0;
    while (cnt < SIGNAL_WFC_CNTS_MAX) {
        m_curSig[cnt] = nullptr;
        m_curWfc[cnt++] = nullptr;
    }
    this->setWindowTitle(m_title);
    m_vlayoutFinal = new QVBoxLayout(this);
    m_btnSave = new QPushButton(QStringLiteral("Save"));
    m_btnLoad = new QPushButton(QStringLiteral("Load"));
    m_btnUnload = new QPushButton(QStringLiteral("Unload"));
    m_btnAddTim = new QPushButton(QStringLiteral("Add Timing"));
    m_btnDone = new QPushButton(QStringLiteral("Done"));
    m_spacer = new QSpacerItem(400, 20);
    m_hlayoutBtn = new QHBoxLayout();
    m_hlayoutBtn->addWidget(m_btnSave);
    m_hlayoutBtn->addWidget(m_btnLoad);
    m_hlayoutBtn->addWidget(m_btnUnload);
    m_hlayoutBtn->addWidget(m_btnAddTim);
    m_hlayoutBtn->addSpacerItem(m_spacer);
    m_vlayoutFinal->addLayout(m_hlayoutBtn);
    m_sigWnd = new SignalWnd("Add Signal", this);
    m_wfcWnd = new WFCWnd("Add WFC", this);
    connect(m_btnAddTim, SIGNAL(clicked()), this, SLOT(SlotAddNewTim()));

    m_treeView = new QTreeView(this);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_vlayoutFinal->addWidget(m_treeView);
    m_vlayoutFinal->addWidget(m_btnDone, 1);

	InitModel();
    connect(m_treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createContextMenu(const QPoint&)));
    m_menue = new QMenu(m_treeView);
    m_menue->addAction(QStringLiteral("AddSignal"), this, SLOT(SlotAddSignal()));
    m_menue->addAction(QStringLiteral("AddWFC"), this, SLOT(SlotAddWFC()));
    m_menue->addAction(QStringLiteral("Delete"), this, SLOT(SlotDelete()));

    connect(m_btnDone, SIGNAL(clicked()), this, SLOT(SlotDone()));
    connect(m_wfcWnd, &WFCWnd::SendWfc, this, &TimingWnd::SlotReciveWfc);
    connect(m_sigWnd, &SignalWnd::SendSig, this, &TimingWnd::SlotReciveSig);
    this->setLayout(m_vlayoutFinal);
    this->adjustSize(); 
}
TimingWnd::~TimingWnd()
{
    int cnt = 0;
    while (cnt < SIGNAL_WFC_CNTS_MAX) {
        if (nullptr == m_curSig[cnt] && nullptr == m_curWfc[cnt])
            break;
        else {
            if (nullptr != m_curSig[cnt])
                delete m_curSig[cnt];
            if (nullptr != m_curWfc[cnt])
                delete m_curWfc[cnt];
            cnt++;
        }
    }
}
void TimingWnd::InitModel()
{
	QStringList strlist;
    strlist << "TimingName" << "WFC" << "Evt0 Event" << "Evt0 Expr" << "Evt1 Event" << "Evt1 Expr"
        << "Evt2 Event" << "Evt2 Expr" << "Evt3 Event" << "Evt3 Expr";
    m_model = new QStandardItemModel();
    m_model->setColumnCount(10);
    m_model->setHorizontalHeaderLabels(strlist);
	m_treeView->setModel(m_model);
    m_root = m_model->invisibleRootItem();
}

void TimingWnd::deleteTim()
{
    QString text = m_model->item(m_selectedIndexs.first().row(), 0)->text();
    m_vtims.erase(m_vtims.begin() + getTimIndexByName(m_model->item(m_selectedIndexs.first().row(), 0)->text()));
    m_timCnt--;
    display();
}

void TimingWnd::deleteSignal()
{
    QString txt = m_selectedIndexs.first().data().toString();
    int tim_index = getTimIndexByName(getItemParent()->text());
    m_vtims[tim_index]->deleteSignalByName(txt.toLocal8Bit().constData());
    display();
}

void TimingWnd::deleteWfc()
{
    QString txt = m_treeView->currentIndex().data().toString();
    int tim_index = getTimIndexByName(getItemParent()->parent()->text());
    int sig_index = getSigIndexByName(tim_index, getItemParent()->text());
    m_vtims[tim_index]->signal(sig_index)->deleteWfcByName(txt.toStdString().c_str());
    display();
}

int TimingWnd::getCurrentTimIndex()
{
    QString qtimName;
    int tim_index;
    QStandardItem* item = getTopParent();
    qtimName = item->text();
    tim_index = getTimIndexByName(qtimName);
    return tim_index;
}

int TimingWnd::getTimIndexByName(const QString& name) const
{
    if(m_vtims.empty())
        return -1;
    int size = (int)m_vtims.size();
    for(int index = 0; index < size; ++index)
        if(name == QString(m_vtims[index]->timname()))
            return index;
    return -1;
}

int TimingWnd::getSigIndexByName(const int& tim_index, const QString& signame) const
{
    if(m_vtims.empty())
        return -1;
    if(m_vtims[tim_index]->vsignals().empty())
        return -1;
    int sig_size = (int)m_vtims[tim_index]->vsignals().size();
    for(int sig_index = 0; sig_index < sig_size; ++sig_index)
        if(signame == QString(m_vtims[tim_index]->signal(sig_index)->signame()))
            return sig_index;
    return -1;
}

QStandardItem* TimingWnd::getItemParent() const
{
    return m_model->itemFromIndex(m_selectedIndexs.first())->parent();
}

QStandardItem* TimingWnd::getTopParent() const
{
    QStandardItem* item = m_model->itemFromIndex(m_treeView->currentIndex());
    while (nullptr != item->parent()) {
        if (getTimIndexByName(item->text()) < 0)
            item = item->parent();
        else
            return item;
    }
    return item;
}
bool TimingWnd::hasChildren() const
{
    return m_model->itemFromIndex(m_selectedIndexs.first())->hasChildren();
}

void TimingWnd::SlotReciveWfc(const QVariant& vVariant)
{
    QString qsigName, qtimName;
    int sig_index, tim_index;
    QStandardItem* itemParent = getItemParent();
    if (nullptr == itemParent)
        return;
    if (nullptr == itemParent->parent()) {
        qsigName = m_treeView->currentIndex().data().toString();
        qtimName = itemParent->text();
        tim_index = getTimIndexByName(qtimName);
        if (m_vtims[tim_index]->hasSignalByName(qsigName.toStdString().c_str()) < 0)
            return;
    } else {
        qtimName = itemParent->parent()->text();
        qsigName = itemParent->text();
    }
    tim_index = getTimIndexByName(qtimName);
    sig_index = getSigIndexByName(tim_index, qsigName);
    std::vector<WFC*> vwfc = vVariant.value<std::vector<WFC*>>();
    int size = (int)vVariant.value<std::vector<WFC*>>().size();
    for(int index = 0; index < size; ++index) {
        if(m_vtims[tim_index]->signal(sig_index)->hasWfcByName(vwfc[index]->wfcname()))
            QMessageBox::critical(this, QStringLiteral("添加wfc错误"), QStringLiteral("WFCName：%1 重复").arg(QString(vwfc[index]->wfcname())), QMessageBox::Ok);
        else
            m_vtims[tim_index]->signal(sig_index)->addWFC(vwfc[index]);
    }
    display();
}

void TimingWnd::SlotReciveSig(const QVariant& vVariant)
{
    int tim_index = getCurrentTimIndex();
    if (tim_index < 0)
        return;
    std::vector<SignalEx*> vsignal = vVariant.value<std::vector<SignalEx*>>();
    int size = (int)vVariant.value<std::vector<SignalEx*>>().size();
    for(int index = 0; index < size; ++index) {
        if(m_vtims[tim_index]->hasSignalByName(vsignal[index]->signame()) >= 0)
            QMessageBox::critical(this, QStringLiteral("添加wfc错误"), QStringLiteral("SignalName：%1 重复").arg(QString(vsignal[index]->signame())), QMessageBox::Ok);
        else
            m_vtims[tim_index]->addSignal(vsignal[index]);
    }
    display();
}

void TimingWnd::createContextMenu(const QPoint& p)
{
    m_treeView->expandAll();
   QModelIndex index = m_treeView->indexAt(p);
   if(index.isValid()) {
          m_selectedIndexs = m_treeView->selectionModel()->selectedRows();
          m_menue->exec(QCursor::pos());
   }
}
void TimingWnd::SlotAddNewTim()
{
    static int m_wfc_cnt = 0;
    static int m_sig_cnt = 0;
    std::vector<Event*> vevents;
    vevents.push_back(new Event("U", "0ns"));
    vevents.push_back(new Event("D", "50ns"));
    vevents.push_back(new Event("U", "200ns"));
    vevents.push_back(new Event("D", "400ns"));

    m_curWfc[m_wfc_cnt] = new WFC("Z", vevents);
    m_curSig[m_sig_cnt] = new SignalEx("PA0", m_curWfc[m_wfc_cnt++]);
    m_vtims.push_back(new Timing(QString("new timing %1").arg(m_timCnt++).toStdString().c_str(), 500, m_curSig[m_sig_cnt++]));
    QStandardItem* itemTim = new QStandardItem(m_vtims.back()->timname());
    m_root->appendRow(itemTim);
    itemTim->appendRow(new QStandardItem(QString::number(m_vtims.back()->period())));
    QStandardItem* itemSig = new QStandardItem(m_vtims.back()->signal(0)->signame());
    itemTim->appendRow(itemSig);
    QStandardItem* itemWfc = new QStandardItem("");
    itemSig->appendRow(itemWfc);
    itemSig->setChild(0, 1, new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->wfcname()));
    itemSig->setChild(0, 2, new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(0)->evt()));
    itemSig->setChild(0, 3, new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(0)->expr()));
    itemSig->setChild(0, 4,  new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(1)->evt()));
    itemSig->setChild(0, 5,  new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(1)->expr()));
    itemSig->setChild(0, 6,  new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(2)->evt()));
    itemSig->setChild(0, 7,  new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(2)->expr()));
    itemSig->setChild(0, 8,  new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(3)->evt()));
    itemSig->setChild(0, 9,  new QStandardItem(m_vtims.back()->signal(0)->wfc(0)->evt(3)->expr()));
}

void TimingWnd::display()
{
    int timSize = (int)m_vtims.size();
    if (timSize <= 0) {
        m_model->removeRows(0, m_model->rowCount());
        return;
    }
    int rowCnt = m_model->rowCount();
    m_model->removeRows(0, rowCnt);
    for(int tim_index = 0; tim_index < timSize; ++tim_index) {
        QStandardItem* itemTim = new QStandardItem(m_vtims[tim_index]->timname());
        m_root->appendRow(itemTim);                                                                                                         //add timName
        itemTim->appendRow(new QStandardItem(QString::number(m_vtims[tim_index]->period())));                                               //add timPeriod
        int sigSize = (int)m_vtims[tim_index]->vsignals().size();
        for(int sig_index = 0; sig_index < sigSize; ++sig_index) {
            QStandardItem* itemSig = new QStandardItem(m_vtims[tim_index]->signal(sig_index)->signame());
            itemTim->appendRow(itemSig);                                                                                                     //add signalName
            int wfcSize = (int)m_vtims[tim_index]->signal(sig_index)->vwfc().size();
            for(int wfc_index = 0; wfc_index < wfcSize; ++wfc_index) {
                itemSig->appendRow(new QStandardItem(""));
                itemSig->setChild(wfc_index, 1, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->wfcname()));             //add wfcName
                itemSig->setChild(wfc_index, 2, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(0)->evt()));
                itemSig->setChild(wfc_index, 3, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(0)->expr()));
                itemSig->setChild(wfc_index, 4, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(1)->evt()));
                itemSig->setChild(wfc_index, 5, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(1)->expr()));
                itemSig->setChild(wfc_index, 6, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(2)->evt()));
                itemSig->setChild(wfc_index, 7, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(2)->expr()));
                itemSig->setChild(wfc_index, 8, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(3)->evt()));
                itemSig->setChild(wfc_index, 9, new QStandardItem(m_vtims[tim_index]->signal(sig_index)->wfc(wfc_index)->evt(3)->expr()));
            }
        }
    }
    m_treeView->expandAll();
}

void TimingWnd::SlotDone()
{
    QVariant var_vtims;
    var_vtims.setValue(m_vtims);
    emit SendVtims(var_vtims);
    this->close();
}
void TimingWnd::SlotSave()
{

}

void TimingWnd::SlotDelete()
{
    QStandardItem* itemParent = getItemParent();
    if (nullptr == itemParent)
        return deleteTim();
    else {
        if(nullptr == itemParent->parent()) {
            return deleteSignal();
        }
        else if(nullptr == itemParent->parent()->parent())
            deleteWfc();
    }
}


void TimingWnd::SlotAddSignal()
{
    int tim_index = getCurrentTimIndex();
    if (tim_index < 0)
        return;
    m_sigWnd->m_period = m_vtims[tim_index]->period();
    m_sigWnd->show();
}

void TimingWnd::SlotAddWFC()
{
    int tim_index = getCurrentTimIndex();
    if (tim_index < 0)
        return;
    m_wfcWnd->m_period = m_vtims[tim_index]->period();
    m_wfcWnd->show();
}

SignalWnd::SignalWnd(const QString &title, QWidget *parent) : QDialog(parent), m_title(title)
{
    int cnt = 0;
    while (cnt < SIGNAL_WFC_CNTS_MAX) {
        m_curSig[cnt] = nullptr;
        m_curWfc[cnt++] = nullptr;
    }
    m_rowCntInView = 0;
    m_btnAddNewSig = new QPushButton(QStringLiteral("AddNewSignal"), this);
    m_btnAddDone = new QPushButton(QStringLiteral("AddDone"), this);
    m_sigModel = new QStandardItemModel(this);
    m_root = m_sigModel->invisibleRootItem();
    QStringList strlist;
    strlist << "Signal" << "WFC" << "Evt0 Event" << "Evt0 Expr" << "Evt1 Event" << "Evt1 Expr"
            << "Evt2 Event" << "Evt2 Expr" << "Evt3 Event" << "Evt3 Expr";
    m_sigModel->setColumnCount(10);
    m_sigModel->setHorizontalHeaderLabels(strlist);
    m_vlayoutFinal = new QVBoxLayout(this);
    m_sigView = new QTableView(this);
    m_sigView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sigView->setContextMenuPolicy(Qt::CustomContextMenu);
    strlist.clear();
    strlist << "D" << "U" << "H" << "L" << "X" << "Z" << "P" << "M";
    for (int col = 2; col < 9; col += 2)
        m_sigView->setItemDelegateForColumn(col, new ComboBoxDeleget(strlist, this));
    m_menue = new QMenu(m_sigView);
    m_menue->addAction(QStringLiteral("Delete"), this, SLOT(SlotDelete()));
    connect(m_btnAddNewSig, SIGNAL(clicked()), this, SLOT(SlotAddNewSig()));
    connect(m_btnAddDone, SIGNAL(clicked()), this, SLOT(SlotAddSigDone()));
    connect(m_sigView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createContextMenu(const QPoint&)));
    m_sigView->setModel(m_sigModel);
    m_vlayoutFinal->addWidget(m_btnAddNewSig);
    m_vlayoutFinal->addWidget(m_sigView);
    m_vlayoutFinal->addWidget(m_btnAddDone);
    this->adjustSize();
    this->setWindowTitle(title);
}

SignalWnd::~SignalWnd()
{
   int cnt = 0;
   while (cnt < SIGNAL_WFC_CNTS_MAX) {
       if (nullptr == m_curSig[cnt] && nullptr == m_curWfc[cnt])
           break;
       else {
           if(nullptr != m_curSig[cnt])
               delete m_curSig[cnt];
           if(nullptr != m_curWfc[cnt])
               delete m_curWfc[cnt];
               cnt++;
       }
   }
}

void SignalWnd::SlotAddNewSig()
{
    std::vector<Event*> vevents;
    vevents.push_back(new Event("U", "0"));
    vevents.push_back(new Event("D", "50ns"));
    vevents.push_back(new Event("U", "200ns"));
    vevents.push_back(new Event("D", "400ns"));

    WFC wfc;
    wfc.setvEvent(vevents);
    wfc.setWfcName("Z");

    QStandardItem* itemSig = new QStandardItem(QString("PA%1").arg(QString::number(m_rowCntInView + 1)));
    m_root->appendRow(itemSig);
    m_sigModel->setItem(m_rowCntInView, 1,  new QStandardItem(wfc.wfcname()));
    m_sigModel->setItem(m_rowCntInView, 2,  new QStandardItem(wfc.evt(0)->evt()));
    m_sigModel->setItem(m_rowCntInView, 3,  new QStandardItem(wfc.evt(0)->expr()));
    m_sigModel->setItem(m_rowCntInView, 4,  new QStandardItem(wfc.evt(1)->evt()));
    m_sigModel->setItem(m_rowCntInView, 5,  new QStandardItem(wfc.evt(1)->expr()));
    m_sigModel->setItem(m_rowCntInView, 6,  new QStandardItem(wfc.evt(2)->evt()));
    m_sigModel->setItem(m_rowCntInView, 7,  new QStandardItem(wfc.evt(2)->expr()));
    m_sigModel->setItem(m_rowCntInView, 8,  new QStandardItem(wfc.evt(3)->evt()));
    m_sigModel->setItem(m_rowCntInView, 9,  new QStandardItem(wfc.evt(3)->expr()));
    m_rowCntInView++;
}

void SignalWnd::SlotAddSigDone()
{
    static int sig_cnt = 0;
    int rowMax = m_sigModel->rowCount();
    std::vector<Event*> vevents;
    std::vector<SignalEx*> vsignal;
    for(int row = 0; row < rowMax; ++row) {
        if (isExprValid(m_period, m_sigModel->item(row, 3)->text()) && isExprValid(m_period, m_sigModel->item(row, 3)->text()) && isExprValid(m_period, m_sigModel->item(row, 3)->text())
            && isExprValid(m_period, m_sigModel->item(row, 3)->text())) {
            vevents.push_back(new Event(m_sigModel->item(row, 2)->text().toStdString().c_str(), m_sigModel->item(row, 3)->text().toStdString().c_str()));
            vevents.push_back(new Event(m_sigModel->item(row, 4)->text().toStdString().c_str(), m_sigModel->item(row, 5)->text().toStdString().c_str()));
            vevents.push_back(new Event(m_sigModel->item(row, 6)->text().toStdString().c_str(), m_sigModel->item(row, 7)->text().toStdString().c_str()));
            vevents.push_back(new Event(m_sigModel->item(row, 8)->text().toStdString().c_str(), m_sigModel->item(row, 9)->text().toStdString().c_str()));
        }
        else {
            QMessageBox::critical(this, QStringLiteral("添加Signal错误"), QStringLiteral("沿时间大于timing 周期（%1）").arg(QString::number(m_period)), QMessageBox::Ok);
            return;
        }
        m_curWfc[sig_cnt] = new WFC(m_sigModel->item(row, 1)->text().toStdString().c_str(), vevents);
        m_curSig[sig_cnt] = new SignalEx(m_sigModel->item(row, 0)->text().toStdString().c_str(), m_curWfc[sig_cnt]);
        vsignal.push_back(m_curSig[sig_cnt++]);
        vevents.clear();
    }
    QVariant var_signal;
    var_signal.setValue(vsignal);
    emit SendSig(var_signal);
    this->close();
}

void SignalWnd::SlotDelete()
{
    int rowFirst = m_selectedIndexs.first().row();
    int rowLast = m_selectedIndexs.last().row();
    m_sigModel->removeRows(rowFirst, rowLast - rowFirst + 1);
    m_rowCntInView -= rowLast - rowFirst + 1;
}

void SignalWnd::createContextMenu(const QPoint& p)
{
   QModelIndex index = m_sigView->indexAt(p);
   if(index.isValid()) {
          m_selectedIndexs = m_sigView->selectionModel()->selectedRows();
          m_menue->exec(QCursor::pos());
   }
}

WFCWnd::WFCWnd(const QString &title, QWidget *parent) : QDialog(parent), m_title(title)
{
    int wfc_cnt = 0;
    while(wfc_cnt < SIGNAL_WFC_CNTS_MAX)
        m_curWfc[wfc_cnt++] = nullptr;
    m_rowCntInView = 0;
    m_btnAddNewWfc = new QPushButton(QStringLiteral("AddNewWFC"), this);
    m_btnAddDone = new QPushButton(QStringLiteral("添加完成"), this);
    m_wfcModel = new QStandardItemModel();
    QStringList strlist;
    strlist << "WFC" << "Evt0 Event" << "Evt0 Expr" << "Evt1 Event" << "Evt1 Expr"
            << "Evt2 Event" << "Evt2 Expr" << "Evt3 Event" << "Evt3 Expr";
    m_wfcModel->setColumnCount(9);
    m_wfcModel->setHorizontalHeaderLabels(strlist);
    m_root = m_wfcModel->invisibleRootItem();
    m_menue= new QMenu(this);
    m_wfcView = new QTableView(this);
    m_wfcView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_wfcView->setContextMenuPolicy(Qt::CustomContextMenu);
    strlist.clear();
    strlist << "D" << "U" << "H" << "L" << "X" << "Z" << "P" << "M";
    for(int col = 1; col < 9; col += 2)
        m_wfcView->setItemDelegateForColumn(col, new ComboBoxDeleget(strlist));
    m_vlayoutFinal = new QVBoxLayout(this);
    m_vlayoutFinal->addWidget(m_btnAddNewWfc);
    m_vlayoutFinal->addWidget(m_wfcView);
    m_wfcView->setModel(m_wfcModel);
    m_vlayoutFinal->addWidget(m_btnAddDone);
    this->setLayout(m_vlayoutFinal);
    this->adjustSize();
    m_menue->addAction(QStringLiteral("Delete"), this, SLOT(SlotDelete()));
    connect(m_wfcView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createContextMenu(const QPoint&)));
    connect(m_btnAddNewWfc, SIGNAL(clicked()), this, SLOT(SlotAddNewWfc()));
    connect(m_btnAddDone, SIGNAL(clicked()), this, SLOT(SlotAddWfcDone()));
}

WFCWnd::~WFCWnd()
{
    int wfc_cnt = 0;
    while(wfc_cnt < SIGNAL_WFC_CNTS_MAX) {
        if(nullptr != m_curWfc[wfc_cnt])
            delete m_curWfc[wfc_cnt++];
        else
            return;
    }
}

void WFCWnd::SlotAddWfcDone()
{
    static int wfc_cnt = 0;
    int rowMax = m_wfcModel->rowCount();
    std::vector<Event*> vevents;
    std::vector<WFC*> vwfc;
    for(int row = 0; row < rowMax; ++row) {
        if (isExprValid(m_period, m_wfcModel->item(row, 2)->text()) && isExprValid(m_period, m_wfcModel->item(row, 4)->text()) && isExprValid(m_period, m_wfcModel->item(row, 6)->text())
            && isExprValid(m_period, m_wfcModel->item(row, 8)->text())) {
            vevents.push_back(new Event(m_wfcModel->item(row, 1)->text().toStdString().c_str(), m_wfcModel->item(row, 2)->text().toStdString().c_str()));
            vevents.push_back(new Event(m_wfcModel->item(row, 3)->text().toStdString().c_str(), m_wfcModel->item(row, 4)->text().toStdString().c_str()));
            vevents.push_back(new Event(m_wfcModel->item(row, 5)->text().toStdString().c_str(), m_wfcModel->item(row, 6)->text().toStdString().c_str()));
            vevents.push_back(new Event(m_wfcModel->item(row, 7)->text().toStdString().c_str(), m_wfcModel->item(row, 8)->text().toStdString().c_str()));
        }
        else {
            QMessageBox::critical(this, QStringLiteral("添加Wfc错误"), QStringLiteral("沿时间大于timing 周期（%1）").arg(QString::number(m_period)), QMessageBox::Ok);
            return;
        }
        m_curWfc[wfc_cnt] = new WFC(m_wfcModel->item(row, 0)->text().toStdString().c_str(), vevents);
        vwfc.push_back(m_curWfc[wfc_cnt++]);
        vevents.clear();
    }
    QVariant var_wfc;
    var_wfc.setValue(vwfc);
    emit SendWfc(var_wfc);
    this->close();
}

void WFCWnd::SlotAddNewWfc()
{
    std::vector<Event*> vevents;
    vevents.push_back(new Event("D", "0"));
    vevents.push_back(new Event("U", "50ns"));
    vevents.push_back(new Event("D", "200ns"));
    vevents.push_back(new Event("U", "400ns"));

    WFC wfc;
    wfc.setvEvent(vevents);
    wfc.setWfcName("Z");

    m_root->appendRow(new QStandardItem(wfc.wfcname()));
    m_wfcModel->setItem(m_rowCntInView, 1,  new QStandardItem(wfc.evt(0)->evt()));
    m_wfcModel->setItem(m_rowCntInView, 2,  new QStandardItem(wfc.evt(0)->expr()));
    m_wfcModel->setItem(m_rowCntInView, 3,  new QStandardItem(wfc.evt(1)->evt()));
    m_wfcModel->setItem(m_rowCntInView, 4,  new QStandardItem(wfc.evt(1)->expr()));
    m_wfcModel->setItem(m_rowCntInView, 5,  new QStandardItem(wfc.evt(2)->evt()));
    m_wfcModel->setItem(m_rowCntInView, 6,  new QStandardItem(wfc.evt(2)->expr()));
    m_wfcModel->setItem(m_rowCntInView, 7,  new QStandardItem(wfc.evt(3)->evt()));
    m_wfcModel->setItem(m_rowCntInView, 8,  new QStandardItem(wfc.evt(3)->expr()));
    m_rowCntInView++;
}

void WFCWnd::SlotDelete()
{
    int rowFirst = m_selectedIndexs.first().row();
    int rowLast = m_selectedIndexs.last().row();
    m_wfcModel->removeRows(rowFirst, rowLast - rowFirst + 1);
    m_rowCntInView -= rowLast - rowFirst + 1;
}

void WFCWnd::createContextMenu(const QPoint& p)
{
   QModelIndex index = m_wfcView->indexAt(p);
   if(index.isValid()) {
          m_selectedIndexs = m_wfcView->selectionModel()->selectedRows();
          m_menue->exec(QCursor::pos());
   }
}
