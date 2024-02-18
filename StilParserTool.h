#ifndef STILPARSERTOOL_H
#define STILPARSERTOOL_H

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>
extern "C"{
#include "stil.h"
}
#include "WaveformView.h"
#include "type.h"
namespace Ui {
class StilParserTool;
}

class StilParserTool : public QMainWindow
{
    Q_OBJECT

public:
    explicit StilParserTool(QWidget *parent = nullptr);
    ~StilParserTool();
    void SetSignalTruthTable(SignalTruthTable*);
    void SetSignals(const QStringList&);
    void UpdateWaveView() { m_waveformView->update(); }
    void GetShowTableTestVector();
    void GetTableFromTestVector(const SignalTruthTable& table) { m_tableFromTestVector = table; SetSignalTruthTable(&m_tableFromTestVector); }
    void SetViewSource(const bool& isStil) { m_isStil = isStil; }
private slots:
    void on_Open_triggered();

    void on_Analyse_triggered();

    void on_Previous_triggered();

    void on_Next_triggered();

    void on_Larger_triggered();

    void on_Smaller_triggered();

    void on_Period_triggered();

    void on_Normal_triggered();

    void onEnlargeWaveformViewSlot(QPoint p);

private:
    void GetShowTruthTable();

    EdgeSet *GetSignalEdgeSet(SignalGroupSet* timingSet, QString signal, char wfc);
    QString GetSignalGroup(QString signal);
    Ui::StilParserTool *ui;
    int                                             m_multiple;
    int                                             m_startTime;
    Signal*                                         m_allSignal = nullptr;
    int                                             m_allSignalNum;
    SignalGroup*                                    m_group = nullptr;
    int                                             m_groupNum;
    TimingSet*                                      m_timingSet = nullptr;
    PatternExpression*                              m_pattern = nullptr;
    SignalTruthTable                                m_sigTruthTable;
    SignalTruthTable                                m_tableFromTestVector;
    WaveformView*                                   m_waveformView;
    bool                                            m_isStil;
};

#endif // STILPARSERTOOL_H
