#ifndef UTG4082WIDGET_H
#define UTG4082WIDGET_H
#include "Def.h"
#include <QWidget>
#include "Include/uci.h"
#include "Include/UTG4162.h"
#include "Include/ucidef.h"
#include "Include/unit.h"
#include "Include/dso_base.h"
#include "Include/Utg4000Lib.h"
#include <utility>
using namespace Utg4000;
typedef enum paramValueType{_MY_INT, _MY_DOUBLE} ValueType;
class Utg4082ParamItem;
class Utg4082widget;
class Utg4082CH;

class Utg4082widget : public QWidget
{
    Q_OBJECT
public:
    explicit Utg4082widget(QWidget *parent = nullptr);
    ~Utg4082widget();
signals:
protected slots:
    void RefreshParam(int baseWaveType);
    void SetParam();
    void GetParam();
private:
    void InitParam(int paramSelect);
private:
    QLabel* m_labCh1;
    QLabel* m_labCh2;
    QLabel* m_labMode;
    QLabel* m_labBaseWaveChoose;
    QLabel* m_labModeWaveChoose;
    QRadioButton* m_radioCh1;
    QRadioButton* m_radioCh2;
    QRadioButton* m_radioMode;
    QComboBox* m_cmbBaseWaveChoose;
    QComboBox* m_cmbModeWaveChoose;
    QPushButton* m_btnGetParam;
    QPushButton* m_btnSetParam;
    QGroupBox* m_ledStatus;
    QGroupBox* m_grpChChoose;
    QGroupBox* m_grpWaveChoose;
    QGroupBox* m_grpCh1Params;
    QGroupBox* m_grpCh2Params;
    QHBoxLayout m_hlayout_Choose, m_hlayout_Chchoose, m_hlayout_WvChoose;
    QHBoxLayout* m_hlayout_ch1Params;
    QHBoxLayout* m_hlayout_ch2Params;
    QVBoxLayout m_vlayoutFinal;
    std::vector<std::pair<int, int>> m_vPrmSelect;
public:
    Utg4082CH* m_utg4082_ch1;
    Utg4082CH* m_utg4082_ch2;
};

class Utg4082CH : public QWidget
{
    Q_OBJECT
public:
    const int m_paramEnable;
    explicit Utg4082CH(int paramSelect, QWidget *parent = nullptr);
    ~Utg4082CH();
    void Delete();
    void GetWaveParam(int ch);
    void SetWaveParam(int ch);
signals:
    void AddMsg(QString msg);
private:
    Utg4000Lib* m_utg4082;
    Utg4082ParamItem* m_pFreq;              //频率
    Utg4082ParamItem* m_pAmp;               //幅度
    Utg4082ParamItem* m_pPhase;             //相位
    Utg4082ParamItem* m_pOffset;            //偏移
    Utg4082ParamItem* m_pDuty;              //占空比
    Utg4082ParamItem* m_pRisetime;          //上升沿时间
    Utg4082ParamItem* m_pFalltime;          //下降沿时间
    Utg4082ParamItem* m_pSymmetry;          //对称度
    QVBoxLayout m_vlayout;
};

class Utg4082ParamItem :public QWidget
{
    Q_OBJECT
public:
    explicit Utg4082ParamItem(QString name, ValueType type, QString unit, QWidget *parent = nullptr);
private:
    const QString m_name;
    const ValueType m_valueType;
    const QString m_unit;
public:
    QLabel* m_labTitle;
    QLabel* m_labUnit;
    QSpinBox* m_spinValue;
    QDoubleSpinBox* m_spinDoubValue;
    QHBoxLayout m_hlayout;
    void Delete();
};
#endif // UTG4082WIDGET_H
