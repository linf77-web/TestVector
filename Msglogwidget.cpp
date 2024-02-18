#include "Msglogwidget.h"
#include <QPushButton>
#include <QApplication>
MsgLogWidget::MsgLogWidget(QWidget *parent) : QWidget(parent)
{
    m_logFile = nullptr;
    m_fileName = new char[LOG_MAX];
    m_strWrite = new char[LOG_MAX];
    memset(m_fileName, 0, LOG_MAX);
    memset(m_strWrite, 0 , LOG_MAX);
    m_btnOpenDir = new QPushButton("DirChoose", this);
    m_editLogDir = new QLineEdit(this);
    m_cbAutoSave = new QCheckBox(this);
    m_labAutoSave = new QLabel("AutoSave", this);
    m_editmMsg = new QTextEdit(this);
    m_editmMsg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_cbAutoSave->setChecked(true);
    m_hlayout.addWidget(m_btnOpenDir);
    m_hlayout.addWidget(m_editLogDir);
    m_hlayout.addWidget(m_cbAutoSave);
    m_hlayout.addWidget(m_labAutoSave);
    m_vlayout.addLayout(&m_hlayout);
    m_vlayout.addWidget(m_editmMsg);
    this->setLayout(&m_vlayout);

    QString curDir = QApplication::applicationDirPath() + "/";
    m_editLogDir->setText(curDir);
    m_dateTime = QDateTime::currentDateTime();
    QString date = m_dateTime.toString("yyyyMMdd");
    strcat(m_fileName, (curDir.toStdString() + date.toStdString()).c_str());
    strcat(m_fileName, ".log");
    m_logFile = fopen(m_fileName, "a+");
    QString msg;
    if(nullptr == m_logFile) {
        msg.append("Error: Create Or Open LogFile Error!\n");
        AddMsg(msg);
    }
    else {
        fclose(m_logFile);
        msg.append("Create Or Open LogFile Succeed!\n");
        AddMsg(msg);
    }
}
MsgLogWidget::~MsgLogWidget()
{
    delete[] m_strWrite;
    delete[] m_fileName;
    delete m_btnOpenDir;
    delete m_editLogDir;
    delete m_cbAutoSave;
    delete m_labAutoSave;
    delete m_editmMsg;
}
void MsgLogWidget::AddMsg(QString message)
{
    m_dateTime = QDateTime::currentDateTime();
    QString msg = m_dateTime.toString("[yyyy-MM-dd]hh:mm:ss ");
    msg += message;
    m_editmMsg->append(msg);
    if(nullptr != m_logFile && m_cbAutoSave->isChecked())
        if(fopen(m_fileName, "a+")) {
            memset(m_strWrite, 0, LOG_MAX);
            strcat(m_strWrite, msg.toStdString().c_str());
            fwrite(m_strWrite, sizeof(char), strlen(m_strWrite), m_logFile);
            fclose(m_logFile);
        }
}
