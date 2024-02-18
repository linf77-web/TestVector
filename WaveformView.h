#ifndef WaveformView_H
#define WaveformView_H

#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QDebug>
#include <QDialog>
#include <QMouseEvent>
#include "type.h"

namespace Ui {
class WaveformView;
}

class WaveformView : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformView(QWidget *parent = nullptr);
    ~WaveformView();
    void SetSignals(QStringList sig_list);
    void SetShowMultiple(int multiple);
    void SetShowPeriodGrid(bool enable);
    void SetWaveformViewStartTime(int time);
    void SetSignalTruthTable(SignalTruthTable* table);
signals:
    void sig_EnlargeWaveformView(QPoint p);
    void sig_edit_clicked(QString);
    void sig_delete_clicked(QString);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_WaveformView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::WaveformView                                *ui;
    bool                                            m_line1IsPress = false;
    bool                                            m_line2IsPress = false;
    int                                             m_resolutionDiffer = 0;
    bool                                            m_isShowPeriodGrid = false;
    QMenu                                           *m_tabMenuBar;
    QVector<QLabel*>                                m_vectorLab;
    int                                             m_multiple;
    int                                             m_startTime;
    SignalTruthTable                                m_table;
    QString                                         m_clickedSignalName;
    QPoint                                          m_mousePressPoint;
    bool                                            m_mouseIsPress = false;
};

#endif // WaveformView_H
