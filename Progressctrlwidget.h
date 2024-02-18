#ifndef PROGRESSCTRLWIDGET_H
#define PROGRESSCTRLWIDGET_H

#include "Def.h"

#include "Teststepinfowidget.h"
class ProgressCtrlwidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressCtrlwidget(QWidget *parent = nullptr);
    ~ProgressCtrlwidget();
signals:
private:
    QStringList m_hHeader, m_vHeader;
    TestStepInfowidget* m_stepWidget;
    QStandardItemModel* m_standItemMod;
    QModelIndexList m_selectedIndexs;
    QItemSelectionModel* m_selections;
    QTableView* m_tabViewProg;
    QGroupBox* m_grpTxtInfo;
    QGroupBox* m_grpProg;
    QTextEdit* m_txtInfoDetail;
    QHBoxLayout m_hlayoutGrpDetail;
    QHBoxLayout m_hlayoutProg;
    QHBoxLayout m_hlayoutGrpProg;
    QHBoxLayout m_hlayoutBtn;
    QVBoxLayout m_vlayoutProg;
    QPushButton* m_btnAddStep;
    QPushButton* m_btnImportStep;
    QPushButton* m_btnSaveStep;
    StepInfo m_stepInfo;
    QMenu* m_menue;
    bool m_isInsertStep;
    int m_insertRow;
    std::vector<std::pair<int, QString>> m_vStepDevName;
    std::vector<std::pair<int, QString>> m_vStepDevFuc;
    std::vector<std::pair<int, QString>> m_vStepWaveName;
    std::vector<std::pair<int, QString>> m_vUdp6300Info;
    std::vector<std::pair<int, QString>> m_vUtg4082Info;
    std::vector<std::pair<int, QString>> m_vUtl8512Info;
    std::vector<std::pair<int, QString>> m_vStepLoadModeName;
    std::vector<StepInfo> m_listStepInfo;
public slots:
    void SlotStepOperator(QPoint);
    void SlotDeletStep();
    void SlotInsertStep();
    void SlotBtnAddStep();
    void ReciveStepInfo(QVariant&);
    void SlotSelectionChanged(const QItemSelection&, const QItemSelection&);
public:
    void InitName();
    void DisStepInfo();
    void DisStepInfoDetail();
};

#endif // PROGRESSCTRLWIDGET_H
