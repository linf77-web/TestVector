#define _CRT_SECURE_NO_WARNINGS
#ifndef TESTDEMO_H
#define TESTDEMO_H

#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QCoreApplication>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QVector>
#include <QPair>
#include "Def.h"
#include "TestVector.h"
#include "type.h"
#include "WaveformView.h"
#include "StilParserTool.h"
inline bool isExprValid(const int&, const QString&);
class WFCWnd : public QDialog
{
    Q_OBJECT
public:
    explicit WFCWnd(const QString& title, QWidget *parent = nullptr);
    ~WFCWnd();
public slots:
    void SlotAddWfcDone();
    void SlotAddNewWfc();
    void SlotDelete();
    void createContextMenu(const QPoint&);
signals:
    void SendWfc(QVariant&);
private:
    int                                             m_rowCntInView;
    QVBoxLayout*                                    m_vlayoutFinal;
    QString                                         m_title;
    QTableView*                                     m_wfcView;
    QMenu*                                          m_menue;
    QStandardItemModel*                             m_wfcModel;
    QModelIndexList                                 m_selectedIndexs;
    QStandardItem*                                  m_root;
    QPushButton*                                    m_btnAddNewWfc;
    QPushButton*                                    m_btnAddDone;
    WFC*                                            m_curWfc[SIGNAL_WFC_CNTS_MAX];
public:
    int                                             m_period;
    friend class                                    SignalWnd;
};

class SignalWnd : public QDialog
{
    Q_OBJECT
public:
    explicit SignalWnd(const QString& title, QWidget *parent = nullptr);
    ~SignalWnd();
public slots:
    void SlotAddNewSig();
    void SlotAddSigDone();
    void SlotDelete();
    void createContextMenu(const QPoint&);
signals:
    void SendSig(QVariant&);
private:
    int                                             m_rowCntInView;
    QVBoxLayout*                                    m_vlayoutFinal;
    QString                                         m_title;
    QModelIndexList                                 m_selectedIndexs;
    QStandardItem*                                  m_root;
    QTableView*                                     m_sigView;
    QMenu*                                          m_menue;
    QStandardItemModel*                             m_sigModel;
    QPushButton*                                    m_btnAddDone;
    QPushButton*                                    m_btnAddWFC;
    QPushButton*                                    m_btnAddNewSig;
    WFC*                                            m_curWfc[SIGNAL_WFC_CNTS_MAX];
    SignalEx*                                       m_curSig[SIGNAL_WFC_CNTS_MAX];
public:
    int                                             m_period;
    friend class                                    TimingWnd;
};
class TimingWnd : public QDialog
{
    Q_OBJECT
public:
    explicit TimingWnd(const QString& title, QWidget *parent = nullptr);
    ~TimingWnd();
    void InitModel();
    void deleteTim();
    void deleteSignal();
    void deleteWfc();
    int getCurrentTimIndex();
    int getTimIndexByName(const QString&) const;
    int getSigIndexByName(const int&, const QString&) const;
    QStandardItem* getItemParent() const;
    QStandardItem* getTopParent() const;
    bool hasChildren() const;
    void display();
signals:
    void SendVtims(const QVariant&);
public slots:
    void SlotReciveWfc(const QVariant&);
    void SlotReciveSig(const QVariant&);
    void createContextMenu(const QPoint&);
    void SlotSave();
    void SlotDelete();
    void SlotAddSignal();
    void SlotAddWFC();
    void SlotAddNewTim();
    void SlotDone();
private:
    QString                                         m_title;
    SignalWnd*                                      m_sigWnd;
    WFCWnd*                                         m_wfcWnd;
    QHBoxLayout*                                    m_hlayoutBtn;
    QVBoxLayout*                                    m_vlayoutFinal;
    QModelIndexList                                 m_selectedIndexs;
    QStandardItemModel*                             m_model;
    QTreeView*                                      m_treeView;
    QStandardItem*                                  m_root;
    QPushButton*                                    m_btnSave;
    QPushButton*                                    m_btnLoad;
    QPushButton*                                    m_btnUnload;
    QPushButton*                                    m_btnAddTim;
    QPushButton*                                    m_btnDone;
    QMenu*                                          m_menue;
    QSpacerItem*                                    m_spacer;
    std::vector<QString>                            m_vaddSigErrs;
    std::vector<QString>                            m_vaddWFCErrs;
    std::vector<Timing*>                            m_vtims;
    Event*                                          m_curEvent;
    SignalEx*                                       m_curSig[SIGNAL_WFC_CNTS_MAX];
    WFC*                                            m_curWfc[SIGNAL_WFC_CNTS_MAX];
    int                                             m_timCnt;
public:
    friend class                                    VectorTool;
};

class VectorTool : public QMainWindow
{
    Q_OBJECT
public:
    VectorTool(QWidget *parent = nullptr);
    ~VectorTool();
    void closeEvent(QCloseEvent *);
    void InitWnd();
    void InitStyle();
    void GenerateTestVector();
    void GenerateTestVector(int begin, int end, int cnts);
    void GetWfcArry();
    void ReverseArry();
    void ClearArry();
    void SetEdgeTableToWaveWnd();
    char getEvtType(const char*);
    int getEvtOffset(const char*);
public slots:
    void ShowSigWnd();
    void ShowTimWnd();
    void ShowWaveWnd();
    void SlotAppendRow();
    void SlotInsertRow();
    void SlotDeleteRows();
    void SlotRecivevTims(const QVariant&);
    void createContextMenu(const QPoint&);
    void SlotTimNameChanged(QStandardItem*);
private:
    QWidget*                                        m_widget;
    QMenuBar*                                       m_menueBar;
    QMenu*                                          m_menueTool;
    QMenu*                                          m_menueCreate;
    QMenu*                                          m_menueRightClick;
    QAction*                                        m_actWave;
    QAction*                                        m_actCreateTim;
    QAction*                                        m_actCreateVector;
    TimingWnd*                                      m_timingWnd;
    SignalWnd*                                      m_sigWnd;
    QTableView*                                     m_vectorView;
    StilParserTool*                                 m_waveWnd;
    QStandardItemModel*                             m_model;
    QModelIndexList                                 m_selectedIndexs;
    QFile                                           m_xmlFile;
    QString                                         m_fileName;
    QVBoxLayout                                     m_vlayoutFinal;
    std::vector<PatternItem*>                       m_vectorFromUI;
    std::vector<PatternItem*>                       m_vectorFinal;
    std::vector<Timing*>                            m_vtims;
    QStringList                                     m_listTimNames;
    PatternItem*                                    m_curPattItem[SIGNAL_WFC_CNTS_MAX];
    PatternItem                                     m_firstPattenItem;
    WFC**                                           m_wfcArry;
    WFC**                                           m_wfcArryReverse;
    Event*                                          m_tempEvent[EVT_CNTS_MAX];
    WFC*                                            m_tempWfc[SIGNAL_WFC_CNTS_MAX];
    int                                             m_signalCnts;
    int                                             m_vectorSize;
    int                                             m_pattCnt;
    unsigned int                                    m_row;
};
#endif // TESTDEMO_H
