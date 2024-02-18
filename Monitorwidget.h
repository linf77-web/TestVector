#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#ifndef NAME_MAX
#define NAME_MAX (128)
#endif

#ifndef WRITE_MAX
#define WRITE_MAX (128)
#endif

#include <QWidget>
#include <QMouseEvent>
#include "Def.h"
#include "Utl8512widget.h"
#include "Sds6034widget.h"
extern COM*                                 g_utl8512Com[DEV_MAX_CNT];
extern std::vector<std::string>             g_vUtl8512DevComs;
extern char                                 g_Sds6000AdressList [4][256];
extern unsigned long                        g_SdsHandle[4];
extern Sds6000Lib*                          g_Sds6000;
extern std::string                          g_ioCfgFileName;
typedef enum {Digital_IO, CHANEL, IN_OUT_CH, SMA} CH_TYPE;
class StatusDevs : public QWidget
{
    Q_OBJECT
public:
    explicit StatusDevs(QWidget *parent = nullptr);
    ~StatusDevs();
    void InitUtl8512();
    void InitSds6000();
signals:
    void AddMsg(QString msg);
private:
    QHBoxLayout                             m_layout;
    QStandardItemModel*                     m_standardItemModelDevs;
    QStandardItem*                          m_itemRoot;
    QStandardItem*                          m_itemUdp6730;
    QStandardItem*                          m_itemUtg4000;
    QStandardItem*                          m_itemUil8512;
    QStandardItem*                          m_itemSds6000;
    QTreeView*                              m_treeViewDevs;
    char                                    m_udp6730AdressList[10][256];
public slots:
    void Init();

    //void FindDev_udp6730(char findListAdress[10][256]);
};
class IOConfigWidget;
class StatusIO : public QWidget
{
    Q_OBJECT
public:
    explicit StatusIO(QWidget *parent = nullptr);
    ~StatusIO();
    void LoadCfg();
signals:
private:
    QTreeView*                          m_treeViewIO;
    QStandardItemModel*                 m_model;
    QStandardItem*                      m_item;
    QHBoxLayout                         m_hlayoutFinal;
public:
    IOConfigWidget*                     m_ioCfgWid;
};

class IOConfigItem
{
public:
    IOConfigItem(QString& type, QString& name, QString& chanel, QString& intfc, QString& level, QString& inOut);
    ~IOConfigItem();
private:
    QString m_type;
    QString m_name;
    QString m_chanel;
    QString m_interface;
    QString m_level;
    QString m_inout;
};

class IOConfigWidget : public QDialog
{
    Q_OBJECT
public:
    friend class StatusIO;
    explicit IOConfigWidget(QWidget *parent = nullptr);
    ~IOConfigWidget();
    void closeEvent(QCloseEvent *);
    void InitModelView(QStandardItemModel* model);
private:
    QStringList                         m_strlistHeaderV;
    QStringList                         m_strlistHeaderH;
    QPushButton*                        m_btnFileSelect;
    QLineEdit*                          m_txtFileSelect;
    QHBoxLayout                         m_hlayoutFileSelect;
    QPushButton*                        m_btnRun;
    QProgressBar*                       m_barProgress;
    QLabel*                             m_labProgress;
    QHBoxLayout                         m_hlayoutProgress;
    QLabel*                             m_labProgressInfo;
    QTableView*                         m_treeView;
    QStandardItemModel*                 m_model;
    FILE*                               m_pFile;
    QThread*                            m_thread;
    QVBoxLayout                         m_vlayoutFinal;
    void CheckConflict();
    void ShowErrorMsg(QString& title, QString& msg);
public:
    std::vector<IOConfigItem*> m_vIoCfgs;
public slots:
    void SlotRun();
    void SlotOnBtnFileSelectClicked();
signals:

};

class Monitorwidget : public QWidget
{
    Q_OBJECT
public:
    explicit Monitorwidget(QWidget *parent = nullptr);
    ~Monitorwidget();
public:
    QTabWidget*                         m_tabMonitor;
    StatusDevs*                         m_statusDevs;
    StatusIO*                           m_statusDgIO;
    QHBoxLayout                         m_layout;
};

#endif // MONITORWIDGET_H
