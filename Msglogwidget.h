#ifndef MSGLOGWIDGET_H
#define MSGLOGWIDGET_H
#define _CRT_SECURE_NO_WARNINGS
#include <QWidget>
#include "Def.h"
#include <QFile>
class MsgLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MsgLogWidget(QWidget *parent = nullptr);
    ~MsgLogWidget();

signals:
private:
    QDateTime m_dateTime;
    FILE* m_logFile;
    char* m_fileName;
    char* m_strWrite;
    QTextEdit* m_editmMsg;
    QHBoxLayout m_hlayout;
    QVBoxLayout m_vlayout;
    QLineEdit* m_editLogDir;
    QPushButton* m_btnOpenDir;
    QCheckBox* m_cbAutoSave;
    QLabel* m_labAutoSave;
public:
    void AddMsg(QString Msg);
};
#endif // MSGLOGWIDGET_H
