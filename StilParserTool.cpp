#include "StilParserTool.h"
#include "ui_StilParserTool.h"

StilParserTool::StilParserTool(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::StilParserTool),
    m_multiple(1),
    m_startTime(0),
    m_isStil(false)
{
    ui->setupUi(this);
    InitStilFileParser();
    m_waveformView = new WaveformView(ui->scrollAreaWidgetContents);
    connect(m_waveformView, &WaveformView::sig_EnlargeWaveformView, this, &StilParserTool::onEnlargeWaveformViewSlot);
}

StilParserTool::~StilParserTool()
{
    delete ui;
}

void StilParserTool::on_Open_triggered()
{
    m_isStil = true;
    QString fileName = QFileDialog::getOpenFileName(this, "Stil Tool", QApplication::applicationDirPath(), tr("File (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    SetStilFileName(fileName.toStdString().c_str());
    ui->Open->setEnabled(false);
    ui->Analyse->setEnabled(true);
}

void StilParserTool::on_Analyse_triggered()
{
    m_isStil = true;
    StilStartParsing();
    ui->Open->setEnabled(true);
    ui->Analyse->setEnabled(false);
    m_allSignalNum = 0;
    m_allSignal = GetAllSignalList(&m_allSignalNum);
    m_sigTruthTable = QVector<EdgeList>(m_allSignalNum);
    QStringList signalList;
    signalList.clear();
    for (int i = 0; i < m_allSignalNum; i++) {
        signalList.append(m_allSignal[i].name);
    }
    m_waveformView->SetSignals(signalList);
    m_group = GetSignalGroups(&m_groupNum);
    m_timingSet = GetTimingSet();
    m_pattern = GetPattern();

    GetShowTruthTable();
    m_waveformView->SetSignalTruthTable(&m_sigTruthTable);
    ui->scrollAreaWidgetContents->setFixedSize(m_waveformView->width(), m_waveformView->height());

    m_waveformView->update();
}



void StilParserTool::on_Previous_triggered()
{
    m_startTime -= 1500 * m_multiple;
    if (m_startTime < 0) {
        m_startTime = 0;
    }
    m_waveformView->SetWaveformViewStartTime(m_startTime);
    if (m_isStil)
        GetShowTruthTable();
    else 
        GetShowTableTestVector();
    m_waveformView->SetSignalTruthTable(&m_sigTruthTable);
    m_waveformView->update();
}

void StilParserTool::on_Next_triggered()
{
    m_startTime += 1500 * m_multiple;
    m_waveformView->SetWaveformViewStartTime(m_startTime);
    if (m_isStil)
        GetShowTruthTable();
    else
        GetShowTableTestVector();
    m_waveformView->SetSignalTruthTable(&m_sigTruthTable);
    m_waveformView->update();
}

void StilParserTool::GetShowTruthTable()
{
    for (int i = 0; i < m_allSignalNum; i++) {
        m_sigTruthTable[i].clear();
    }
    PatternExpression* expr = m_pattern;
    QString wft = "";
    TimingSet* timingSet = nullptr;
    int offset = 0;
    while (expr != nullptr) {
        if (strcmp("W", expr->opt_name) == 0) {
            wft = expr->opt_data;
            timingSet = m_timingSet;
            while (timingSet != nullptr) {
                if (wft == timingSet->name) {
                    break;
                }
                timingSet = timingSet->next;
            }
        }
        else if (strcmp("V", expr->opt_name) == 0) {
            if (timingSet == nullptr) {
                offset += timingSet->period;
                expr = expr->next;
                continue;
            }
            if ((offset + (int)timingSet->period) < m_startTime) {
                offset += timingSet->period;
                expr = expr->next;
                continue;
            }
            if (offset > (m_startTime + 1500 * m_multiple)) {
                break;
            }
            for (int i = 0; i < m_allSignalNum; i++) {
                char wfc = expr->opt_data[i];
                QString name = m_allSignal[i].name;
                EdgeSet* edgeSet = GetSignalEdgeSet(timingSet->signalgroupSet, name, wfc);
                if (edgeSet == nullptr) {
                    continue;
                }
                for (int j = 0; j < MAX_EDGE_NUM; j++) {
                    if (0 == edgeSet->edge[j].event) {
                        Edge edge;
                        edge.event = 0xff;
                        edge.offset = offset + timingSet->period;
                        m_sigTruthTable[i].append(edge);
                        break;
                    }
                    Edge edge;
                    edge.event = edgeSet->edge[j].event;
                    edge.offset = edgeSet->edge[j].offset + offset;
                    m_sigTruthTable[i].append(edge);
                }
            }
            offset += timingSet->period;
        }
        if (expr->next == nullptr) {
            for (int i = 0; i < m_allSignalNum; i++) {
                char wfc = expr->opt_data[i];
                QString name = m_allSignal[i].name;
                EdgeSet* edgeSet = GetSignalEdgeSet(timingSet->signalgroupSet, name, wfc);
                if (edgeSet == nullptr) {
                    continue;
                }
                int lastIndex = 0;
                for (int j = 0; j < MAX_EDGE_NUM; j++) {
                    if (0 == edgeSet->edge[j].event) {

                        break;
                    }
                    lastIndex = j;
                }
                Edge edge;
                edge.event = edgeSet->edge[lastIndex].event;
                edge.offset = offset;
                m_sigTruthTable[i].append(edge);
            }
        }
        expr = expr->next;
    }
}

void StilParserTool::GetShowTableTestVector()
{
    int tab_size = m_tableFromTestVector.size();
    int offset = 0;
    m_sigTruthTable.clear();
    EdgeList edgList;
    for (int index_sig = 0; index_sig < tab_size; ++index_sig) {
        int edge_size = (int)m_tableFromTestVector[index_sig].size();
        for (int index_edge = 0; index_edge < edge_size; ++index_edge) {
            offset = m_tableFromTestVector[index_sig][index_edge].offset;
            if (offset < m_startTime)
                continue;
            else if (offset > m_startTime + 1500 * m_multiple) 
                break;
            else {
                Edge edge;
                edge.event = m_tableFromTestVector[index_sig][index_edge].event;
                edge.offset = offset;
                edgList.push_back(edge);
            }
        }
        m_sigTruthTable.append(edgList);
        edgList.clear();
    }
}
EdgeSet* StilParserTool::GetSignalEdgeSet(SignalGroupSet* signalGroupSet, QString sig, char wfc)
{
    SignalGroupSet* sgs = signalGroupSet;
    while (sgs != nullptr) {
        if (sig == sgs->name) {
            break;
        }
        sgs = sgs->next;
    }
    if (sgs == nullptr) {
        QString group = "";
        for (int i = 0; i < m_groupNum; i++) {
            for (int j = 0; j < MAX_SIGNAL_NUM; j++) {
                if (sig == m_group[i].arrSignal[j]) {
                    group = m_group[i].name;
                    break;
                }
            }
        }
        if (group.isEmpty()) {
            return nullptr;
        }
        sgs = signalGroupSet;
        while (sgs != nullptr) {
            if (group == sgs->name) {
                break;
            }
            sgs = sgs->next;
        }
    }
    for (int i = 0; i < MAX_EDGE_SET_NUM; i++) {
        if (wfc == sgs->edgeSet[i].id) {
            return &sgs->edgeSet[i];
        }
    }
    return nullptr;
}


void StilParserTool::on_Larger_triggered()
{
    m_multiple /= 2;
    if (m_multiple < 1) {
        m_multiple = 1;
        ui->Larger->setEnabled(false);
    }
    m_waveformView->SetShowMultiple(m_multiple);
    if (m_isStil)
        GetShowTruthTable();
    else
        GetShowTableTestVector();
    m_waveformView->SetSignalTruthTable(&m_sigTruthTable);
    m_waveformView->update();
}

void StilParserTool::on_Smaller_triggered()
{
    m_multiple *= 2;
    ui->Larger->setEnabled(true);
    m_waveformView->SetShowMultiple(m_multiple);
    if (m_isStil)
        GetShowTruthTable();
    else
        GetShowTableTestVector();
    m_waveformView->SetSignalTruthTable(&m_sigTruthTable);
    m_waveformView->update();
}

void StilParserTool::on_Period_triggered()
{
    m_waveformView->SetShowPeriodGrid(true);
    ui->Period->setEnabled(false);
    ui->Normal->setEnabled(true);
    m_waveformView->update();
}

void StilParserTool::on_Normal_triggered()
{
    m_waveformView->SetShowPeriodGrid(false);
    ui->Period->setEnabled(true);
    ui->Normal->setEnabled(false);
    m_waveformView->update();
}

void StilParserTool::onEnlargeWaveformViewSlot(QPoint p)
{
    m_startTime += ((p.x() - 100) / 100 * 100) * m_multiple;
    m_waveformView->SetWaveformViewStartTime(m_startTime);
    m_multiple /= 2;
    if (m_multiple < 1) {
        m_multiple = 1;
        ui->Larger->setEnabled(false);
    }
    m_waveformView->SetShowMultiple(m_multiple);
    GetShowTruthTable();
    m_waveformView->SetSignalTruthTable(&m_sigTruthTable);
    m_waveformView->update();
}

void StilParserTool::SetSignalTruthTable(SignalTruthTable* table)
{
    m_waveformView->SetSignalTruthTable(table);
}

void StilParserTool::SetSignals(const QStringList& strlist)
{
    m_waveformView->SetSignals(strlist);
}
