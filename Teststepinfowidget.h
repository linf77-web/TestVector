#ifndef TESTSTEPINFOWIDGET_H
#define TESTSTEPINFOWIDGET_H

#include "Def.h"
#include "Include/UTG4162.h"
#include "Include/dso_base.h"
#include "Include/uci.h"
#include "Include/ucidef.h"
#include "Include/unit.h"
#include "Include/Udp6730Lib.h"
#include "Include/Utg4000Lib.h"
#include "Include/Utl8500.h"
#include "Utl8512widget.h"
typedef enum : int {DEV_UDP6730, DEV_UTG4082, DEV_UTL8512} DEVS;
struct Udp6730Info{
    int id = 01;
    double volProt;
    double curProt;
    double voltage;
    double current;
};
struct Utg4000Info{
    int id;
    int sg_ch;
    int waveType;
    double freq;
    double amp;
    double offset;
    double phase;
    double duty;
    double riseTime;
    double fallTime;
    double symmetry;
};
struct Utl8500Info{
    int id = 21;
    int loadMode;
    //cc/cv/cr/cp
    double current;         //0
    double voltage;
    double resistance;
    double power;
    double loadVol;         //4
    double volProt;
    double curProt;
    // list


    //dync/dynv
    double srcHVal;         //7
    double srcHTime;
    double srcLVal;
    double srcLTime;
    double riseSlop;
    double fallSlop;
    double repeatTimes;
    //ocp/opp
    double beginVal;        //14
    double stepVal;
    double endVal;
    double stepTime;
    //led
    double led_vo;          //18
    double led_io;
    double rd_factor;
    //battery
    double disChrgVal;      //21
    double endVol;
    //ovp
    double ovpTrigVol;      //23
    //timing
    double onloadMod;       //24
    double onloadVal;
    double onloadParm;
    double beginTrigSrc;
    double beginTrigEdge;
    double beginTrigLevel;
    double endTrigSrc;
    double endTrigEdge;
    double endTrigLevel;    //32
};

struct StepInfo {
    int id;
    int devType;
    int ch;
    int delay;
    int fuc;
    Udp6730Info udp6730Info;
    Utg4000Info utg4000Info;
    Utl8500Info utl8500Info;
};
Q_DECLARE_METATYPE(StepInfo)
class StepCommonMinWidget: public QWidget
{
    Q_OBJECT
public:
    explicit StepCommonMinWidget(QString name, QWidget* parent = nullptr);
    ~StepCommonMinWidget();
private:
    QLabel* m_lab;
    QDoubleSpinBox* m_dspinBox;
    QHBoxLayout m_hlayout;
    QString m_name;
public:
    double GetVal() {return m_dspinBox->value();}
};

class StepUdp6730Widget : public QWidget
{
    Q_OBJECT
public:
    explicit StepUdp6730Widget(QWidget* parent = nullptr);
    ~StepUdp6730Widget();
private:
    StepCommonMinWidget* m_widgetVol;
    StepCommonMinWidget* m_widgetCur;
    StepCommonMinWidget* m_widgetVolProt;
    StepCommonMinWidget* m_widgetCurProt;
    QVBoxLayout m_vlayoutAll;
public:
    friend class TestStepInfowidget;
};
class StepUtg4082Widget : public QWidget
{
    Q_OBJECT
public:
    explicit StepUtg4082Widget(QWidget* parent = nullptr);
    ~StepUtg4082Widget();
private:
    QLabel* m_labChChoose;
    QLabel* m_labWaveChoose;
    QComboBox* m_cmbChChoose;
    QComboBox* m_cmbWaveChoose;
    QStandardItemModel* m_itemModSine;
    QStandardItemModel* m_itemModSquare;
    QStandardItemModel* m_itemModPulse;
    QStandardItemModel* m_itemModDC;
    QStandardItemModel* m_itemModRamp;
    QStandardItemModel* m_itemModNoise;
    QTableView* m_tabView;
    std::vector<std::pair<int, QStandardItemModel*>> m_vWaveChoose;
    QHBoxLayout m_hlayoutChChoose, m_hlayoutWaveChoose;
    QVBoxLayout m_vlayoutAll;
    void SetViewHeader(QStandardItemModel* itemMod, QStringList& titles, QStringList& units);
public slots:
    void RefreshModView(int);
public:
    friend class TestStepInfowidget;
};
class StepUtl8512Widget : public QWidget
{
    Q_OBJECT
public:
    explicit StepUtl8512Widget(QWidget* parent = nullptr);
    ~StepUtl8512Widget();
private:
    Utl8512TabView* m_utl8512TabView;
    QVBoxLayout m_vlayoutAll;
public:
    friend class TestStepInfowidget;
};
class StepSds6000Widet : public QWidget
{
    Q_OBJECT
public:
    explicit StepSds6000Widet(QWidget* parent = nullptr);
    ~StepSds6000Widet();
};

class TestStepInfowidget : public QDialog
{
    Q_OBJECT
public:
    explicit TestStepInfowidget(QWidget *parent = nullptr);
    ~TestStepInfowidget();

signals:
    void SendStepInfo(QVariant&);
private:
    StepUdp6730Widget* m_udp6730;
    StepUtg4082Widget* m_utg4082;
    StepUtl8512Widget* m_utl8512;
    QLabel* m_labDevChoose;
    QComboBox* m_cmbDevChoose;
    QVBoxLayout m_vlayoutUdp6730;
    QVBoxLayout m_vlayoutUtg4082;
    QVBoxLayout m_vlayoutUtl8512;
    QGroupBox* m_grpUdp6730;
    QGroupBox* m_grpUtg4082;
    QGroupBox* m_grpUtl8512;
    QGroupBox* m_grpSds6000;
    QPushButton* m_btnDone;
    QHBoxLayout m_hlayoutDevChoose;
    QVBoxLayout m_vlayoutAll;
public:
    StepInfo m_stepInfo;
public slots:
    void AddDone();
    void DevChanged(int);
};

#endif // TESTSTEPINFOWIDGET_H
