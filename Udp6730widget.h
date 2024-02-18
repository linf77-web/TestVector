#ifndef UDP6730WIDGET_H
#define UDP6730WIDGET_H

#include "Def.h"
#include "Include/Udp6730Lib.h"
#include "Msglogwidget.h"

class Udp6730Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Udp6730Widget(QWidget *parent = nullptr);
    ~Udp6730Widget();
    int GetOutPut();
    void GetVersion();
    char* GetDevDescrip(){return m_devDescriptor;}
signals:
    void AddMsg(QString msg);

protected slots:
    void SetVoltage();
    void SetCurrent();
    void SetOvp();
    void SetOcp();
    void SetOutPut();
    void OnEditVolChanged(int);
    void OnEditCurChanged(int);
    void Init(char findAdressList[10][256]);

private:
    char m_findAdressList[10][VAL_MAX];         //find 10 devs.
    unsigned long m_devNums = 0;
    char* m_devDescriptor;
    int m_outputStatu;
    Udp6730* m_udp6730;
    QLCDNumber* m_lcdDisVol;
    QLabel* m_labMeasuredV;
    QLabel* m_labDisV;
    QLabel* m_labDisC;
    QLabel* m_labMeasuredC;
    QLabel* m_labVolCtrl;
    QLabel* m_labCurCtrl;
    QLCDNumber* m_lcdDisCur;
    QPushButton* m_btnSetVProt;
    QPushButton* m_btnSetCProt;
    QPushButton* m_btnOutPut;
    QDoubleSpinBox* m_editSetVProt;
    QDoubleSpinBox* m_editSetCProt;
    QDoubleSpinBox* m_editVolCtrl;
    QDoubleSpinBox* m_editCurCtrl;
    QSlider* m_hsliderVolCtrl;
    QSlider* m_hsliderCurCtrl;
    QPushButton* m_btnSetVoltage;
    QPushButton* m_btnSetCurrent;
    QHBoxLayout m_hlayoutDisV, m_hlayoutDisC, m_hlayoutSetProt, m_hlayoutVolCtrl, m_hlayoutCurCtrl;
    QVBoxLayout m_vLayoutDis;
    QVBoxLayout m_layoutFinal;

};
#endif // UDP6730WIDGET_H
