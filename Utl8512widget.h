#ifndef UTL8512WIDGET_H
#define UTL8512WIDGET_H

#include <QWidget>
#include "Def.h"
#include "Include/Utl8500.h"

class MinSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MinSetWidget(QString name, double value, QString unit, QWidget* parent = nullptr);
    ~MinSetWidget();
    QLabel* m_labTitle;
    QDoubleSpinBox* m_dSpinValue;
    QLabel* m_labUnit;
    QHBoxLayout hlayout;
private:
    const QString m_name;
    double m_value;
    const QString m_unit;
};
class Utl8512TabView : public QWidget
{
    Q_OBJECT
public :
    explicit Utl8512TabView(QWidget* parent = nullptr);
    ~Utl8512TabView();
private:
    QLabel* m_labModChoose;
    QHBoxLayout m_hlayOutModChoose;
    QVBoxLayout m_vlayOutParamSetAll;
    QTableView* m_tabViewParamSetMain;
public:
    QComboBox* m_cmbModChoose;
    QStandardItemModel* m_mod_CC;
    QStandardItemModel* m_mod_CV;
    QStandardItemModel* m_mod_CR;
    QStandardItemModel* m_mod_CP;
    QStandardItemModel* m_mod_DYNC;
    QStandardItemModel* m_mod_DYNV;
    QStandardItemModel* m_mod_List;
    QStandardItemModel* m_mod_OCP;
    QStandardItemModel* m_mod_OPP;
    QStandardItemModel* m_mod_CR_LED;
    QStandardItemModel* m_mod_CCBATTERY;
    QStandardItemModel* m_mod_CRBATTERY;
    QStandardItemModel* m_mod_CPBATTERY;
    QStandardItemModel* m_mod_OVP;
    QStandardItemModel* m_mod_TIMING;
    std::vector<std::pair<int, QStandardItemModel*>> m_vItemPair;
private:
    void InitMode();
    void SetViewHeader(QStandardItemModel*, Qt::Orientation, QStringList& strTitles, QStringList& strUnit);
public slots:
    void RefrsParmView(int modeIndex);
};

class Utl8512widget : public QWidget
{
    Q_OBJECT
public:
    explicit Utl8512widget(QWidget *parent = nullptr);
    ~Utl8512widget();
    void Init();
signals:
    void AddMsg(QString msg);
private:
    QLabel* m_labDisVoltage;
    QLabel* m_labDisCurrent;
    QLabel* m_labDisPower;
    QLCDNumber* m_lcdVoltageDis;
    QLCDNumber* m_lcdCurrentDis;
    QLCDNumber* m_lcdPowDis;
    Utl8512TabView* m_utl8512TabView;
    QGroupBox* m_grpModeDis;
    QGroupBox* m_grpParamSetVpp;
    QGroupBox* m_grpParamSetIpp;
    QVBoxLayout m_vlayOutParamMinV, m_vlayOutParamMinI;
    QHBoxLayout m_hlayOutParamSetVppIpp;
    QVBoxLayout m_vlayOutParamSetAll;
    QHBoxLayout m_hlayOutAll;
    QHBoxLayout m_hlayOutDisItemV, m_hlayOutDisItemC, m_hlayOutDisItemP;
    QVBoxLayout m_hlayOutDis;
};
extern COM* g_utl8512Com[DEV_MAX_CNT];
extern std::vector<std::string> g_vUtl8512DevComs;
#endif // UTL8512WIDGET_H
