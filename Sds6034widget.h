#ifndef SDS6034WIDGET_H
#define SDS6034WIDGET_H

#include <QtCore>
#include <QWidget>
#include "Def.h"
#include "Include/Sds6000Lib.h"
#include <deque>
#include <QMutex>
#include <QTimer>
#include <QList>
#ifndef DEQUE_MAX_SIZE
#define DEQUE_MAX_SIZE (200)
#endif
#ifndef DEQUE_MIN_SIZE
#define DEQUE_MIN_SIZE (20)
#endif
#ifndef MENU_QTY
#define MENU_QTY (6)
#endif
using namespace  Sds6000;
extern Sds6000Lib* g_Sds6000;
extern char g_Sds6000AdressList [4][256];
extern unsigned long g_SdsHandle[4];
class Sds6000Qt:  public QObject
{
    Q_OBJECT
public:
    Sds6000Qt();
    ~Sds6000Qt();
public:
    Sds6000Lib* m_sds6000;
};

class ImgShowThread : public QThread
{
    Q_OBJECT
public:
    ImgShowThread(QLabel*, std::deque<std::string>*);
    void run() override;
private:
    QImage*                  m_img;
    QLabel*                  m_lab;
    std::deque<std::string>* m_deque;
    QString                  m_path;
    bool                     m_suspend = false;
public slots:
    void Suspend(bool suspend) { m_suspend = suspend; }
    void SlotShowImg(int index);
};


class CfgLanWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgLanWid(QWidget* parent = nullptr);
    ~CfgLanWid();
private:
    QLabel*             m_labIpAddr;
    QLineEdit*          m_txtIpAddr;
    QHBoxLayout         m_hlayoutIpAddr;
    QLabel*             m_labGateWay;
    QLineEdit*          m_txtGateWay;
    QHBoxLayout         m_hlayoutGateWay;
    QLabel*             m_labMac;
    QLineEdit*          m_txtMac;
    QHBoxLayout         m_hlayoutMac;
    QLabel*             m_labSmask;
    QLineEdit*          m_txtSmask;
    QHBoxLayout         m_hlayoutSmask;
    QLabel*             m_labLanType;
    QRadioButton*       m_radioBtnLanType;
    QHBoxLayout         m_hlayoutLanType;
    QLabel*             m_labVncPort;
    QLineEdit*          m_txtVncPort;
    QHBoxLayout         m_hlayoutVncPort;
    QPushButton*        m_btnApply;
    QPushButton*        m_btnCancel;
    QHBoxLayout         m_hlayoutApply;
    QVBoxLayout         m_vlayoutFinal;
};
class CfgNetStoreWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgNetStoreWid(QWidget* parent = nullptr);
    ~CfgNetStoreWid();
private:
    QLabel*             m_labDrv;
    QComboBox*          m_cmbDrv;
    QHBoxLayout         m_hlayoutDrv;
    QLabel*             m_labPath;
    QLineEdit*          m_txtPath;
    QCheckBox*          m_ckbRmemPath;
    QHBoxLayout         m_hlayoutPath;
    QCheckBox*          m_ckbAnonymous;
    QLabel*             m_labUsrName;
    QLineEdit*          m_txtUsrName;
    QCheckBox*          m_ckbRmemName;
    QHBoxLayout         m_hlayoutName;
    QLabel*             m_labPasswd;
    QLineEdit*          m_txtPasswd;
    QCheckBox*          m_ckbRmemPasswd;
    QHBoxLayout         m_hlayoutPasswd;
    QCheckBox*          m_ckbAutoConnc;
    QVBoxLayout         m_vlayoutFinal;
};

class CfgAcquireWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgAcquireWid(QWidget* parent = nullptr);
    ~CfgAcquireWid();
private:
    QLabel*             m_labInterp;
    QComboBox*          m_cmbInterp;
    QHBoxLayout         m_hlayoutInterp;
    QLabel*             m_labAcqMode;
    QComboBox*          m_cmbAcqMode;
    QHBoxLayout         m_hlayoutMode;
    QLabel*             m_labAcqType;
    QComboBox*          m_cmbAcqType;
    QHBoxLayout         m_hlayoutType;
    QLabel*             m_labMemMode;
    QComboBox*          m_cmbMemMode;
    QHBoxLayout         m_hlayoutMemMode;
    QLabel*             m_labMemDepth;
    QComboBox*          m_cmbMemDepth;
    QHBoxLayout         m_hlayoutMemDepth;
    QLabel*             m_labSequence;
    QRadioButton*       m_radioBtnSequence;
    QLineEdit*          m_txtSequenceCnt;
    QHBoxLayout         m_hlayoutSequence;
    QLabel*             m_labXYMode;
    QRadioButton*       m_radioBtnXYMode;
    QHBoxLayout         m_hlayoutXYMode;
    QVBoxLayout         m_vlayoutFinal;
};

class CfgDisWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgDisWid(QWidget* parent = nullptr);
    ~CfgDisWid();
private:
    QLabel*             m_labType;
    QComboBox*          m_cmbType;
    QHBoxLayout         m_hlayoutType;
    QLabel*             m_labColorTemper;
    QRadioButton*       m_radiobtnColorTemper;
    QHBoxLayout         m_hlayoutColorTemper;
    QLabel*             m_labAfterGlow;
    QComboBox*          m_cmbAfterGlow;
    QHBoxLayout         m_hlayoutAfterGlow;
    QPushButton*        m_btnClear;
    QLabel*             m_labHidenMenu;
    QComboBox*          m_cmbHidenMenu;
    QHBoxLayout         m_hlayoutHidenMenu;
    QLabel*             m_labMenuStyle;
    QComboBox*          m_cmbMenuStyle;
    QHBoxLayout         m_hlayoutMenuStyle;
    QLabel*             m_labGrid;
    QComboBox*          m_cmbGrid;
    QHBoxLayout         m_hlayoutGrid;
    QLabel*             m_labWaveIntensity;
    QLineEdit*          m_txtWaveIntensity;
    QHBoxLayout         m_hlayoutWaveIntensity;
    QLabel*             m_labBackLight;
    QLineEdit*          m_txtBackLight;
    QHBoxLayout         m_hlayoutBackLight;
    QLabel*             m_labGridLight;
    QLineEdit*          m_txtGridLight;
    QHBoxLayout         m_hlayoutGridLight;
    QVBoxLayout         m_vlayoutFinal;
};

class EdgeTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit EdgeTrigWid(QWidget* parent = nullptr);
    ~EdgeTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labIMP;
    QComboBox*          m_cmbIMP;
    QHBoxLayout         m_hlayoutIMP;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QVBoxLayout         m_vlayoutFinal;
};

class SlopTrigWid : QDialog
{
    Q_OBJECT
public:
    explicit SlopTrigWid(QWidget* parent = nullptr);
    ~SlopTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHLevel;
    QLineEdit*          m_txtHLevel;
    QHBoxLayout         m_hlayoutHLevel;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labLLevel;
    QLineEdit*          m_txtLLevel;
    QHBoxLayout         m_hlayoutLLevel;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};

class PulseTrigWid : QDialog
{
    Q_OBJECT
public:
    explicit PulseTrigWid(QWidget* parent = nullptr);
    ~PulseTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labPol;
    QComboBox*          m_cmbPol;
    QHBoxLayout         m_hlayoutPol;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};
class VideoTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit VideoTrigWid(QWidget* parent = nullptr);
    ~VideoTrigWid();
private:
    QLabel*             m_labFcnt;
    QComboBox*          m_cmbFcnt;
    QHBoxLayout         m_hlayoutFcnt;
    QLabel*             m_labField;
    QComboBox*          m_cmbField;
    QHBoxLayout         m_hlayoutField;
    QLabel*             m_labFrate;
    QComboBox*          m_cmbFrate;
    QHBoxLayout         m_hlayoutFrate;
    QLabel*             m_labInterl;
    QComboBox*          m_cmbInterl;
    QHBoxLayout         m_hlayoutInterl;
    QLabel*             m_labLcnt;
    QLineEdit*          m_txtLcnt;
    QHBoxLayout         m_hlayoutLcnt;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labLine;
    QLineEdit*          m_txtLine;
    QHBoxLayout         m_hlayoutLine;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labStand;
    QComboBox*          m_cmbStand;
    QHBoxLayout         m_hlayoutStand;
    QLabel*             m_labSync;
    QComboBox*          m_cmbSync;
    QHBoxLayout         m_hlayoutSync;
    QVBoxLayout         m_vlayoutFinal;
};

class WindowTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit WindowTrigWid(QWidget* parent = nullptr);
    ~WindowTrigWid();
private:
    QLabel*             m_labCLevel;
    QLineEdit*          m_txtCLevel;
    QHBoxLayout         m_hlayoutCLevel;
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labDLevel;
    QLineEdit*          m_txtDLevel;
    QHBoxLayout         m_hlayoutDLevel;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHLevel;
    QLineEdit*          m_txtHLevel;
    QHBoxLayout         m_hlayoutHLevel;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labLLevel;
    QLineEdit*          m_txtLLevel;
    QHBoxLayout         m_hlayoutLLevel;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labType;
    QComboBox*          m_cmbType;
    QHBoxLayout         m_hlayoutType;
    QVBoxLayout         m_vlayoutFinal;
};

class INTTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit INTTrigWid(QWidget* parent = nullptr);
    ~INTTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};

class DropOutTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit DropOutTrigWid(QWidget* parent = nullptr);
    ~DropOutTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labTime;
    QLineEdit*          m_txtTime;
    QHBoxLayout         m_hlayoutTime;
    QLabel*             m_labType;
    QComboBox*          m_cmbType;
    QHBoxLayout         m_hlayoutType;
    QVBoxLayout         m_vlayoutFinal;
};

class RuntTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit RuntTrigWid(QWidget* parent = nullptr);
    ~RuntTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHLevel;
    QLineEdit*          m_txtHLevel;
    QHBoxLayout         m_hlayoutHLevel;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labLLevel;
    QLineEdit*          m_txtLLevel;
    QHBoxLayout         m_hlayoutLLevel;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QLabel*             m_labPol;
    QComboBox*          m_cmbPol;
    QHBoxLayout         m_hlayoutPol;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};

class PatternTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit PatternTrigWid(QWidget* parent = nullptr);
    ~PatternTrigWid();
private:
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labInput;
    QLineEdit*          m_txtInput;
    QHBoxLayout         m_hlayoutInput;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labLogic;
    QComboBox*          m_cmbLogic;
    QHBoxLayout         m_hlayoutLogic;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};

class QUALTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit QUALTrigWid(QWidget* parent = nullptr);
    ~QUALTrigWid();
private:
    QLabel*             m_labELevel;
    QLineEdit*          m_txtELevel;
    QHBoxLayout         m_hlayoutELevel;
    QLabel*             m_labESlop;
    QComboBox*          m_cmbESlop;
    QHBoxLayout         m_hlayoutESlop;
    QLabel*             m_labESource;
    QComboBox*          m_cmbESource;
    QHBoxLayout         m_hlayoutESource;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labQLevel;
    QLineEdit*          m_txtQLevel;
    QHBoxLayout         m_hlayoutQLevel;
    QLabel*             m_labQSource;
    QComboBox*          m_cmbQSource;
    QHBoxLayout         m_hlayoutQSource;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QLabel*             m_labType;
    QComboBox*          m_cmbType;
    QHBoxLayout         m_hlayoutType;
    QVBoxLayout         m_vlayoutFinal;
};
class DelayTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit DelayTrigWid(QWidget* parent = nullptr);
    ~DelayTrigWid();
private:
    QLabel*             m_labCoup;
    QComboBox*          m_cmbCoup;
    QHBoxLayout         m_hlayoutCoup;
    QLabel*             m_labSource;
    QLineEdit*          m_txtSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labSource2;
    QComboBox*          m_cmbSource2;
    QHBoxLayout         m_hlayoutSource2;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labSlop2;
    QComboBox*          m_cmbSlop2;
    QHBoxLayout         m_hlayoutSlop2;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labLevel2;
    QLineEdit*          m_txtLevel2;
    QHBoxLayout         m_hlayoutLevel2;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};
class NedgeTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit NedgeTrigWid(QWidget* parent = nullptr);
    ~NedgeTrigWid();
private:
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labIdle;
    QLineEdit*          m_txtIdle;
    QHBoxLayout         m_hlayoutIdle;
    QLabel*             m_labEdge;
    QLineEdit*          m_txtEdge;
    QHBoxLayout         m_hlayoutEdge;
    QLabel*             m_labLevel;
    QLineEdit*          m_txtLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labHldOff;
    QComboBox*          m_cmbHldOff;
    QHBoxLayout         m_hlayoutHldOff;
    QLabel*             m_labHldTime;
    QLineEdit*          m_txtHldTime;
    QHBoxLayout         m_hlayoutHldTime;
    QLabel*             m_labHldEvt;
    QLineEdit*          m_txtHldEvt;
    QHBoxLayout         m_hlayoutHldEvt;
    QLabel*             m_labHstart;
    QComboBox*          m_cmbHstart;
    QHBoxLayout         m_hlayoutHstart;
    QLabel*             m_labNREJ;
    QComboBox*          m_cmbNREJ;
    QHBoxLayout         m_hlayoutNREJ;
    QVBoxLayout         m_vlayoutFinal;
};

class SHoldTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit SHoldTrigWid(QWidget* parent = nullptr);
    ~SHoldTrigWid();
private:
    QLabel*             m_labType;
    QComboBox*          m_cmbType;
    QHBoxLayout         m_hlayoutType;
    QLabel*             m_labCSource;
    QComboBox*          m_cmbCSource;
    QHBoxLayout         m_hlayoutCSource;
    QLabel*             m_labCThr;
    QLineEdit*          m_txtCThr;
    QHBoxLayout         m_hlayoutCThr;
    QLabel*             m_labSlop;
    QComboBox*          m_cmbSlop;
    QHBoxLayout         m_hlayoutSlop;
    QLabel*             m_labDSource;
    QComboBox*          m_cmbDSource;
    QHBoxLayout         m_hlayoutDSource;
    QLabel*             m_labDThr;
    QLineEdit*          m_txtDThr;
    QHBoxLayout         m_hlayoutDThr;
    QLabel*             m_labLevel;
    QComboBox*          m_cmbLevel;
    QHBoxLayout         m_hlayoutLevel;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labTlow;
    QLineEdit*          m_txtTlow;
    QHBoxLayout         m_hlayoutTlow;
    QLabel*             m_labTupp;
    QLineEdit*          m_txtTupp;
    QHBoxLayout         m_hlayoutTupp;
    QVBoxLayout         m_vlayoutFinal;
};

class IICTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit IICTrigWid(QWidget* parent = nullptr);
    ~IICTrigWid();
private:
    QLabel*             m_labAddr;
    QLineEdit*          m_txtAddr;
    QHBoxLayout         m_hlayoutAddr;
    QLabel*             m_labAlen;
    QComboBox*          m_cmbAlen;
    QHBoxLayout         m_hlayoutAlen;
    QLabel*             m_labCond;
    QComboBox*          m_cmbCond;
    QHBoxLayout         m_hlayoutCond;
    QLabel*             m_labData2;
    QLineEdit*          m_txtData2;
    QHBoxLayout         m_hlayoutData2;
    QLabel*             m_labData;
    QLineEdit*          m_txtData;
    QHBoxLayout         m_hlayoutData;
    QLabel*             m_labDlen;
    QLineEdit*          m_txtDlen;
    QHBoxLayout         m_hlayoutDlen;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labRwbit;
    QComboBox*          m_cmbRwbit;
    QHBoxLayout         m_hlayoutRwbit;
    QLabel*             m_labSclSource;
    QComboBox*          m_cmbSclSource;
    QHBoxLayout         m_hlayoutSclSource;
    QLabel*             m_labSclThr;
    QLineEdit*          m_txtSclThr;
    QHBoxLayout         m_hlayoutSclThr;
    QLabel*             m_labSdaSource;
    QComboBox*          m_cmbSdaSource;
    QHBoxLayout         m_hlayoutSdaSource;
    QLabel*             m_labSdaThr;
    QLineEdit*          m_txtSdaThr;
    QHBoxLayout         m_hlayoutSdaThr;
    QVBoxLayout         m_vlayoutFinal;
};

class SPITrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit SPITrigWid(QWidget* parent = nullptr);
    ~SPITrigWid();
private:
    QLabel*             m_labBitOrder;
    QComboBox*          m_cmbBitOrder;
    QHBoxLayout         m_hlayoutBitOrder;
    QLabel*             m_labClkSource;
    QComboBox*          m_cmbClkSource;
    QHBoxLayout         m_hlayoutClkSource;
    QLabel*             m_labClkThr;
    QLineEdit*          m_txtClkThr;
    QHBoxLayout         m_hlayoutClkThr;
    QLabel*             m_labCSSource;
    QComboBox*          m_cmbCSSource;
    QHBoxLayout         m_hlayoutCSSource;
    QLabel*             m_labCSThr;
    QLineEdit*          m_txtCSThr;
    QHBoxLayout         m_hlayoutCSThr;
    QLabel*             m_labCSType;
    QComboBox*          m_cmbCSType;
    QHBoxLayout         m_hlayoutCSType;
    QLabel*             m_labData;
    QLineEdit*          m_txtData;
    QHBoxLayout         m_hlayoutData;
    QLabel*             m_labDlen;
    QLineEdit*          m_txtDlen;
    QHBoxLayout         m_hlayoutDlen;
    QLabel*             m_labLATC;
    QComboBox*          m_cmbLATC;
    QHBoxLayout         m_hlayoutLATC;
    QLabel*             m_labMISOSource;
    QComboBox*          m_cmbMISOSource;
    QHBoxLayout         m_hlayoutMISOSource;
    QLabel*             m_labMISOThr;
    QLineEdit*          m_txtMISOThr;
    QHBoxLayout         m_hlayoutMISOThr;
    QLabel*             m_labMOSISource;
    QComboBox*          m_cmbMOSISource;
    QHBoxLayout         m_hlayoutMOSISource;
    QLabel*             m_labMOSIThr;
    QLineEdit*          m_txtMOSIThr;
    QHBoxLayout         m_hlayoutMOSIThr;
    QLabel*             m_labNCSSource;
    QComboBox*          m_cmbNCSSource;
    QHBoxLayout         m_hlayoutNCSSource;
    QLabel*             m_labNCSThr;
    QLineEdit*          m_txtNCSThr;
    QHBoxLayout         m_hlayoutNCSThr;
    QLabel*             m_labTType;
    QComboBox*          m_cmbTType;
    QHBoxLayout         m_hlayoutTType;
    QVBoxLayout         m_vlayoutFinal;
};

class UARTTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit UARTTrigWid(QWidget* parent = nullptr);
    ~UARTTrigWid();
private:
    QLabel*             m_labBAUD;
    QComboBox*          m_cmbBAUD;
    QHBoxLayout         m_hlayoutBAUD;
    QLabel*             m_labBitOrder;
    QComboBox*          m_cmbBitOrder;
    QHBoxLayout         m_hlayoutBitOrder;
    QLabel*             m_labCond;
    QComboBox*          m_cmbCond;
    QHBoxLayout         m_hlayoutCond;
    QLabel*             m_labData;
    QLineEdit*          m_txtData;
    QHBoxLayout         m_hlayoutData;
    QLabel*             m_labDlen;
    QLineEdit*          m_txtDlen;
    QHBoxLayout         m_hlayoutDlen;
    QLabel*             m_labIDLE;
    QComboBox*          m_cmbIDLE;
    QHBoxLayout         m_hlayoutIDLE;
    QLabel*             m_labLimit;
    QComboBox*          m_cmbLimit;
    QHBoxLayout         m_hlayoutLimit;
    QLabel*             m_labParity;
    QComboBox*          m_cmbParity;
    QHBoxLayout         m_hlayoutParity;
    QLabel*             m_labRXSource;
    QComboBox*          m_cmbRXSource;
    QHBoxLayout         m_hlayoutRXSource;
    QLabel*             m_labRXThr;
    QLineEdit*          m_txtRXThr;
    QHBoxLayout         m_hlayoutRXThr;
    QLabel*             m_labStop;
    QComboBox*          m_cmbStop;
    QHBoxLayout         m_hlayoutStop;
    QLabel*             m_labTType;
    QComboBox*          m_cmbTTYpe;
    QHBoxLayout         m_hlayoutTType;
    QLabel*             m_labTXSource;
    QComboBox*          m_cmbTXSource;
    QHBoxLayout         m_hlayoutTXSource;
    QLabel*             m_labTXThr;
    QLineEdit*          m_txtTXThr;
    QHBoxLayout         m_hlayoutTXThr;
    QVBoxLayout         m_vlayoutFinal;
};

class CANTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit CANTrigWid(QWidget* parent = nullptr);
    ~CANTrigWid();
private:
    QLabel*             m_labBAUD;
    QComboBox*          m_cmbBAUD;
    QHBoxLayout         m_hlayoutBAUD;
    QLabel*             m_labCond;
    QComboBox*          m_cmbCond;
    QHBoxLayout         m_hlayoutCond;
    QLabel*             m_labData2;
    QLineEdit*          m_txtData2;
    QHBoxLayout         m_hlayoutData2;
    QLabel*             m_labData;
    QLineEdit*          m_txtData;
    QHBoxLayout         m_hlayoutData;
    QLabel*             m_labID;
    QLineEdit*          m_txtID;
    QHBoxLayout         m_hlayoutID;
    QLabel*             m_labIDLen;
    QComboBox*          m_cmbIDLen;
    QHBoxLayout         m_hlayoutIDLen;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labThr;
    QLineEdit*          m_txtThr;
    QHBoxLayout         m_hlayoutThr;
    QVBoxLayout         m_vlayoutFinal;
};

class LINTrigWid: public QDialog
{
    Q_OBJECT
public:
    explicit LINTrigWid(QWidget* parent = nullptr);
    ~LINTrigWid();
private:
    QLabel*             m_labBAUD;
    QComboBox*          m_cmbBAUD;
    QHBoxLayout         m_hlayoutBAUD;
    QLabel*             m_labCond;
    QComboBox*          m_cmbCond;
    QHBoxLayout         m_hlayoutCond;
    QLabel*             m_labData2;
    QLineEdit*          m_txtData2;
    QHBoxLayout         m_hlayoutData2;
    QLabel*             m_labData;
    QLineEdit*          m_txtData;
    QHBoxLayout         m_hlayoutData;
    QLabel*             m_labErrCheckSum;
    QComboBox*          m_cmbErrCheckSum;
    QHBoxLayout         m_hlayoutErrCheckSum;
    QLabel*             m_labErrDlen;
    QLineEdit*          m_txtErrDlen;
    QHBoxLayout         m_hlayoutErrDlen;
    QLabel*             m_labErrID;
    QLineEdit*          m_txtErrID;
    QHBoxLayout         m_hlayoutErrID;
    QLabel*             m_labErrParity;
    QComboBox*          m_cmbErrParity;
    QHBoxLayout         m_hlayoutErrParity;
    QLabel*             m_labErrSYNC;
    QComboBox*          m_cmbErrSYNC;
    QHBoxLayout         m_hlayoutErrSYNC;
    QLabel*             m_labID;
    QLineEdit*          m_txtID;
    QHBoxLayout         m_hlayoutID;
    QLabel*             m_labSource;
    QComboBox*          m_cmbSource;
    QHBoxLayout         m_hlayoutSource;
    QLabel*             m_labSTAN;
    QComboBox*          m_cmbSTAN;
    QHBoxLayout         m_hlayoutSTAN;
    QLabel*             m_labThr;
    QLineEdit*          m_txtThr;
    QHBoxLayout         m_hlayoutThr;
    QVBoxLayout         m_vlayoutFinal;
};

class CfgTrigWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgTrigWid(QWidget* parent = nullptr);
    ~CfgTrigWid();
    void closeEvent(QCloseEvent *);
private:
    QLabel*             m_labType;
    QComboBox*          m_cmbtype;
    QHBoxLayout         m_hlayoutType;
    QLabel*             m_labFreq;
    QLineEdit*          m_txtFreq;
    QHBoxLayout         m_hlayoutFreq;
    QLabel*             m_labMode;
    QComboBox*          m_cmbMode;
    QHBoxLayout         m_hlayoutMode;
    QPushButton*        m_btnRunStop;
    QVBoxLayout         m_vlayoutFinal;
    EdgeTrigWid*        m_edgeTrigWid;
    SlopTrigWid*        m_slopTrigWid;
    PulseTrigWid*       m_pulseTrigWid;
    VideoTrigWid*       m_videoTrigWid;
    WindowTrigWid*      m_wndTrigWid;
    INTTrigWid*         m_INTTrigWid;
    DropOutTrigWid*     m_dropOutTrigWid;
    RuntTrigWid*        m_runtTrigWid;
    PatternTrigWid*     m_patternTrigWid;
    QUALTrigWid*        m_qualTrigWid;
    DelayTrigWid*       m_delayTrigWid;
    NedgeTrigWid*       m_nEdgeTrigWid;
    SHoldTrigWid*       m_sHoldTrigWid;
    IICTrigWid*         m_IIcTrigWid;
    SPITrigWid*         m_SPITrigWid;
    UARTTrigWid*        m_UARTTrigWid;
    CANTrigWid*         m_CANTrigWid;
    LINTrigWid*         m_LINTrigWid;
    QWidget*            m_curWid;
    QWidget*            m_lastWid;
    void InitChildWid();
public slots:
    void SlotSelectChildWid(int);
};

class CfgCursorWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgCursorWid(QWidget *parent = nullptr);
    ~CfgCursorWid();
private:
    QLabel*             m_labEnable;
    QRadioButton*       m_btnEnable;
    QHBoxLayout         m_hlayoutEnable;
    QLabel*             m_labTagStyle;
    QComboBox*          m_cmbTagStyle;
    QHBoxLayout         m_hlayoutTagStyle;
    QLabel*             m_labIXD;
    QLineEdit*          m_txtIXD;
    QHBoxLayout         m_hlayoutIXD;
    QLabel*             m_labMitem;
    QComboBox*          m_cmbMitem;
    QHBoxLayout         m_hlayoutMitem;
    QLabel*             m_labMode;
    QComboBox*          m_cmbMode;
    QHBoxLayout         m_hlayoutMode;
    QLabel*             m_labType;
    QComboBox*          m_cmbType;
    QHBoxLayout         m_hlayoutType;
    QLabel*             m_labSource1;
    QComboBox*          m_cmbSource1;
    QHBoxLayout         m_hlayoutSource1;
    QLabel*             m_labSource2;
    QComboBox*          m_cmbSource2;
    QHBoxLayout         m_hlayoutSource2;
    QLabel*             m_labX1;
    QLineEdit*          m_txtX1;
    QHBoxLayout         m_hlayoutX1;
    QLabel*             m_labX2;
    QLineEdit*          m_txtX2;
    QHBoxLayout         m_hlayoutX2;
    QLabel*             m_labXDEL;
    QLineEdit*          m_txtXDEL;
    QHBoxLayout         m_hlayoutXDEL;
    QLabel*             m_labXREF;
    QComboBox*          m_cmbXREF;
    QHBoxLayout         m_hlayoutXREF;
    QLabel*             m_labY1;
    QLineEdit*          m_txtY1;
    QHBoxLayout         m_hlayoutY1;
    QLabel*             m_labY2;
    QLineEdit*          m_txtY2;
    QHBoxLayout         m_hlayoutY2;
    QLabel*             m_labYDEL;
    QLineEdit*          m_txtYDEL;
    QHBoxLayout         m_hlayoutYDEL;
    QLabel*             m_labYREF;
    QComboBox*          m_cmbYREF;
    QHBoxLayout         m_hlayoutYREF;
    QVBoxLayout         m_vlayoutFinal;
};

class CfgMeasureSimpleWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgMeasureSimpleWid(QWidget *parent = nullptr);
    ~CfgMeasureSimpleWid();
private:
    QLabel*             m_labSimpItem;
    QComboBox*          m_cmbSimpItemParam;
    QComboBox*          m_cmbSimpItemState;
    QHBoxLayout         m_hlayoutSimpItem;
    QLabel*             m_labSimpSource;
    QComboBox*          m_cmbSimpSource;
    QHBoxLayout         m_hlayoutSimpSource;
    QLabel*             m_labSimpValue;
    QComboBox*          m_cmbSimpValueType;
    QLineEdit*          m_txtSiimpValue;
    QHBoxLayout         m_hlayoutSimpValue;
    QVBoxLayout         m_vlayoutFinal;
};

class CfgMeasureAdvancedWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgMeasureAdvancedWid(QWidget *parent = nullptr);
    ~CfgMeasureAdvancedWid();
private:
    QLabel*             m_labAdvNumber;
    QLineEdit*          m_txtAdvNumber;
    QHBoxLayout         m_hlayoutAdvNumber;

    QLabel*             m_labAdvP;
    QComboBox*          m_cmbAdvP;
    QRadioButton*       m_btnAdvPOn;
    QPushButton*        m_btnAdvPSet;
    QHBoxLayout         m_hlayoutAdvP;

    QLabel*             m_labAdvPSource1;
    QComboBox*          m_cmbAdvPSource1P;
    QComboBox*          m_cmbAdvPSource1Source;
    QPushButton*        m_btnAdvPSource1Set;
    QHBoxLayout         m_hlayoutAdvPSource1;

    QLabel*             m_labAdvPSource2;
    QComboBox*          m_cmbAdvPSource2P;
    QComboBox*          m_cmbAdvPSource2Source;
    QPushButton*        m_btnAdvPSource2Set;
    QHBoxLayout         m_hlayoutAdvPSource2;


    QLabel*             m_labAdvPType;
    QComboBox*          m_cmbAdvPTypeP;
    QComboBox*          m_cmbAdvPTypeParam;
    QPushButton*        m_btnAdvPTypeSet;
    QHBoxLayout         m_hlayoutPType;

    QLabel*             m_labAdvPVal;
    QComboBox*          m_cmbAdvPValP;
    QLineEdit*          m_txtAdvPVal;
    QHBoxLayout         m_hlayoutAdvPVal;

    QLabel*             m_labAdvStatsOn;
    QRadioButton*       m_btnAdvStatsOn;
    QHBoxLayout         m_hlayoutAdvStatsOn;

    QLabel*             m_labAdvPStats;
    QComboBox*          m_cmbAdvPStatsP;
    QComboBox*          m_cmbAdvPStatsType;
    QLineEdit*          m_txtAdvPStats;
    QHBoxLayout         m_hlayoutAdvPStats;

    QLabel*             m_labAdvPStatsType;
    QComboBox*          m_cmbAdvPStatsTypeP;
    QComboBox*          m_cmbAdvPStatsTypeParam;
    QHBoxLayout         m_hlayoutAdvPstatsType;

    QLabel*             m_labAdvStatsHistog;
    QRadioButton*       m_btnAdvStatsHistog;
    QHBoxLayout         m_hlayoutAdvStatsHistog;

    QLabel*             m_labAdvStatsMaxcnt;
    QLineEdit*          m_txtAdvStatsMaxcnt;
    QHBoxLayout         m_hlayoutAdvStatsMaxcnt;

    QLabel*             m_labAdvStatsReset;
    QPushButton*        m_btnAdvStatsReset;
    QHBoxLayout         m_hlayoutAdvStatsReset;

    QLabel*             m_labAdvStyle;
    QComboBox*          m_cmbAdvStyle;
    QHBoxLayout         m_hlayoutAdvStyle;

    QVBoxLayout         m_vlayoutFinal;
};

class CfgMeasureThresholdWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgMeasureThresholdWid(QWidget *parent = nullptr);
    ~CfgMeasureThresholdWid();
private:
    QLabel*             m_labThrABSHigh;
    QLineEdit*          m_txtThrABSHigh;
    QHBoxLayout         m_hlayoutThrABSHigh;
    QLabel*             m_labThrABSMid;
    QLineEdit*          m_txtThrABSMid;
    QHBoxLayout         m_hlayoutThrABSMid;
    QLabel*             m_labThrABSLow;
    QLineEdit*          m_txtThrABSLow;
    QHBoxLayout         m_hlayoutThrABSLow;
    QLabel*             m_labThrPerHigh;
    QLineEdit*          m_txtThrPerHigh;
    QHBoxLayout         m_hlayoutThrPerHigh;
    QLabel*             m_labThrPerMid;
    QLineEdit*          m_txtThrPerMid;
    QHBoxLayout         m_hlayoutThrPerMid;
    QLabel*             m_labThrPerLow;
    QLineEdit*          m_txtThrPerLow;
    QHBoxLayout         m_hlayoutThrPerLow;
    QVBoxLayout         m_vlayoutFinal;
};

class CfgMeasureWid : public QDialog
{
    Q_OBJECT
public:
    explicit CfgMeasureWid(QWidget *parent = nullptr);
    ~CfgMeasureWid();
    void InitChildWid();
    void closeEvent(QCloseEvent *);
private:
    QLabel*                 m_labEnable;
    QRadioButton*           m_btnEnable;
    QHBoxLayout             m_hlayoutEnable;
    QLabel*                 m_labGateOn;
    QRadioButton*           m_btnGateOn;
    QHBoxLayout             m_hlayoutGateOn;
    QLabel*                 m_labGateGa;
    QLineEdit*              m_txtGateGa;
    QHBoxLayout             m_hlayoutGateGa;
    QLabel*                 m_labGateGb;
    QLineEdit*              m_txtGateGb;
    QHBoxLayout             m_hlayoutGateGb;
    QLabel*                 m_labMeasureType;
    QComboBox*              m_cmbMeasureType;
    QHBoxLayout             m_hlayoutMeasureType;
    QVBoxLayout             m_vlayoutFinal;
    CfgMeasureSimpleWid*    m_simpWid;
    CfgMeasureAdvancedWid*  m_advWid;
    CfgMeasureThresholdWid* m_thrWid;
    QWidget*                m_curWid;
    QWidget*                m_lastWid;
public slots:
    void SlotSelectChildWid(int type);
};

class Sds6034widget : public QDialog
{
    Q_OBJECT
public:
    explicit Sds6034widget(QWidget *parent = nullptr);
    ~Sds6034widget();
    void closeEvent(QCloseEvent *);
signals:
private:
    QThread*                 m_thread;
    Sds6000Qt*               m_sds6000Qt;
	int						 m_curDevIndex;
    QMenuBar*                m_menuBar;
    std::vector<QString>     m_vMenueTitles;
    QMenu*                   m_menus[MENU_QTY];
    QLabel*                  m_labImgShow;
    ImgShowThread*           m_imgShowThrd;
    QTimer*                  m_timer;
    std::deque<std::string>* m_dequeImgName;
    Sds6000Lib*              m_sds6000;
    QHBoxLayout              m_hlayoutOper;
    QHBoxLayout              m_hlayouDis;
    QVBoxLayout              m_vlayoutFinal;
    QLabel*                  m_labDevChoose;
    QComboBox*               m_cmbDevChoose;
    QPushButton*             m_btnScreenShot;
    QPushButton*             m_btnForward;
    QPushButton*             m_btnBackward;
    CfgLanWid*               m_lanWid;
    CfgNetStoreWid*          m_netStoreWid;
    CfgDisWid*               m_disWid;
    CfgAcquireWid*           m_acqWid;
    CfgTrigWid*              m_trigWid;
    CfgCursorWid*            m_cursorWid;
    CfgMeasureWid*           m_measureWid;
    std::vector<std::pair<int, unsigned long>> m_vSeqSdsdev;
private:
    void InitCfgWids();
    void InitMenu();
public slots:
    void SlotScreenShot();
	void SlotDevSelectChanged(int);
    void SlotLanWidShow();
    void SlotNetStoreWidShow();
    void SlotDisWidShow();
    void SlotAcqWidShow();
    void SlotTrigWidShow();
    void SlotCursorWidShow();
    void SlotMeasureWidShow();
};

#endif // SDS6034WIDGET_H
