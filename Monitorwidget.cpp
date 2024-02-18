#define _CRT_SECURE_NO_WARNINGS
#include "Monitorwidget.h"
#include <QPushButton>
#include <forward_list>
Monitorwidget::Monitorwidget(QWidget *parent) : QWidget(parent)
{   for(int i = 0; i < DEV_MAX_CNT; ++i)
       g_utl8512Com[i] = nullptr;
    m_tabMonitor = new QTabWidget(this);
    m_statusDevs = new StatusDevs(this);
    m_statusDgIO = new StatusIO(this);
    m_tabMonitor->addTab(m_statusDevs, "Devs");
    m_tabMonitor->addTab(m_statusDgIO, "IO");
    m_tabMonitor->setCurrentWidget(m_statusDevs);
    m_layout.addWidget(m_tabMonitor);
    this->setLayout(&m_layout);
}

Monitorwidget::~Monitorwidget()
{
    delete m_statusDgIO;
    delete m_statusDevs;
    delete m_tabMonitor;
    for(int i = 0; i < DEV_MAX_CNT; ++i)
        delete g_utl8512Com[i];
}
StatusDevs::StatusDevs(QWidget *parent) : QWidget(parent)
{
    //for debug:
    for(int i = 0; i < 10; ++i)
        memset(m_udp6730AdressList[i], 0, 256);
    char nullAdressList[256];
    memset(nullAdressList, 0, 256);
    memcpy(m_udp6730AdressList[0], "UNI-T Technologies, UTS3036B, 000000001, 00.00.01", 256);

    m_treeViewDevs = new QTreeView(this);
    m_standardItemModelDevs = new QStandardItemModel(this);
    m_standardItemModelDevs->setHorizontalHeaderLabels(QStringList(QStringLiteral("设备资源")));
    m_itemRoot = m_standardItemModelDevs->invisibleRootItem();
    m_itemUdp6730 = new QStandardItem(QStringLiteral("UDP6730"));
    m_itemRoot->appendRow(m_itemUdp6730);

    QList<QStandardItem*> udp6730List;
    for(int i = 0; i < 10; ++i)
        if(strcmp(m_udp6730AdressList[i], nullAdressList)) {
            udp6730List.append(new QStandardItem(m_udp6730AdressList[i]));
        }
    m_itemUdp6730->appendRows(udp6730List);

    m_itemUtg4000 = new QStandardItem(QStringLiteral("UTG4000"));
    m_itemRoot->appendRow(m_itemUtg4000);
    QList<QStandardItem*> utg4000List;
    utg4000List.append(new QStandardItem("4000list1"));
    m_itemUtg4000->appendRows(utg4000List);
    m_itemSds6000 = new QStandardItem("SDS6000");
    m_itemRoot->appendRow(m_itemSds6000);

    m_itemUil8512 = new QStandardItem("UTL8512");
    m_itemRoot->appendRow(m_itemUil8512);
    m_treeViewDevs->setModel(m_standardItemModelDevs);
    m_treeViewDevs->expandAll();
    m_treeViewDevs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_layout.addWidget(m_treeViewDevs);
    this->setLayout(&m_layout);
    Init();
}

StatusDevs::~StatusDevs()
{
    delete m_itemUil8512;
    delete m_itemSds6000;
    delete m_itemUtg4000;
    delete m_itemUdp6730;
    delete m_standardItemModelDevs;
    delete m_treeViewDevs;
    delete m_itemRoot;
}
void StatusDevs::Init()
{
    InitUtl8512();
    InitSds6000();
}

void StatusDevs::InitUtl8512()
{
    if(g_vUtl8512DevComs.empty())
        return;
    else{
        int size = (int)g_vUtl8512DevComs.size();
        for(int i = 0; i < size; ++i) {
            g_utl8512Com[i] = new COM();
            if(g_utl8512Com[i]->OpenCom(g_vUtl8512DevComs[i].c_str())) {
                g_utl8512Com[i]->ConfigCom(16, 2048, 4800, 8, 0, 1);
                char cmd[10] = {0};
                unsigned char readBuf[4096];
                memset(readBuf, 0, 4096);
                strcat(cmd, "*IDN?");
                g_utl8512Com[i]->ComWrite((unsigned char*)cmd, 10);
                Sleep(300);
                g_utl8512Com[i]->ReadCom(readBuf, READ_LENGTH);

                QStandardItem* itemIdn = new QStandardItem(QString::fromStdString(std::string((char*)readBuf)));
                m_itemUil8512->appendRow(itemIdn);
            }
            else {
                emit AddMsg(QString("Open ").append(QString(g_vUtl8512DevComs[i].c_str())).append(" ").append("Failed!"));
            }
        }
    }
}

void StatusDevs::InitSds6000()
{
    for(int i = 0; i < 4; ++i) {
        memset(g_Sds6000AdressList[i], 0, 256);
        memset(&g_SdsHandle[i], 0, sizeof(unsigned long));
    }
    g_Sds6000 = new Sds6000Lib();
    unsigned long  retNumInstrs = 0;
    if(g_Sds6000->FindDev((char*)"?*", g_Sds6000AdressList, 10, &retNumInstrs) >= 0)
        for(int i = 0; i < 10; ++i)
            if(nullptr != strstr(g_Sds6000AdressList[i], "SDS"))
                if(g_Sds6000->OpenDev(g_Sds6000AdressList[i], g_SdsHandle[i]) >= 0) {
                    const char* cmd = "*IDN?";
                    char readBuf[1024];
                    memset(readBuf, 0, 1024);
                    if(g_Sds6000->Read(g_SdsHandle[i], cmd, readBuf, 1024, 2000) > 0) {
                        QStandardItem* itemIdn = new QStandardItem(QString::fromStdString(std::string((char*)readBuf)));
                        m_itemSds6000->appendRow(itemIdn);
                    }
                }
}
StatusIO::StatusIO(QWidget *parent) : QWidget(parent)
{
    m_ioCfgWid = new IOConfigWidget(this);
    m_treeViewIO = new QTreeView(this);
    m_model = new QStandardItemModel(this);
    QStringList strlist;
    strlist << QStringLiteral("资源类型") << QStringLiteral("资源名称") <<QStringLiteral("物理通道") << QStringLiteral("硬件接口") << QStringLiteral("电平") << QStringLiteral("输入输出");
    m_model->setHorizontalHeaderLabels(strlist);
    m_model->setColumnCount(6);

    m_treeViewIO->setModel(m_model);
    m_treeViewIO->expandAll();
    m_treeViewIO->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_treeViewIO->setAnimated(true);
    m_hlayoutFinal.addWidget(m_treeViewIO);

    strlist.clear();
    strlist << "NULL";
    for(int i = 1; i <= 15; ++i)
        strlist << QString("IN/OUT_CH_%1").arg(i);
    for(int i = 0; i < 8; ++i)
        strlist << QString("DPS%1").arg(i);
    for(int i = 0; i < 8; ++i)
        strlist << QString("SMA%1").arg(i);
    m_treeViewIO->setItemDelegateForColumn(3, new ComboBoxDeleget(strlist));
    strlist.clear();
    strlist << "NULL" <<"3.3v" << "5v";
    m_treeViewIO->setItemDelegateForColumn(4, new ComboBoxDeleget(strlist));
    strlist.clear();
    strlist << "NULL" << "IN" << "OUT";
    m_treeViewIO->setItemDelegateForColumn(5, new ComboBoxDeleget(strlist));

    LoadCfg();
    m_ioCfgWid->InitModelView(m_model);
    this->setLayout(&m_hlayoutFinal);
}

void StatusIO::LoadCfg()
{
    if(nullptr == m_ioCfgWid->m_pFile) {
        m_ioCfgWid->m_pFile = fopen(g_ioCfgFileName.c_str(), "r");
        if(nullptr == m_ioCfgWid->m_pFile) {
            QMessageBox::critical(this, "File Error!", "Open ConfigFile Error", QMessageBox::Ok);
            return;
        }
    }
    char buf[READ_LENGTH] = {0};
    char* str, * substr;
    str = buf;
    int line = 0;
    int col = 0;
    while(fgets(buf, READ_LENGTH, m_ioCfgWid->m_pFile)) {
        if(0 == line) {
            ++line;
            continue;
        }
        if(line > 0) {
            if(nullptr != strstr(str, ",")) {
                while(nullptr != (substr = strtok(str, ","))) {
                    if(col > 5)
                        break;
                    str = nullptr;
                    sscanf(substr, "%s", buf);
                    m_item = new QStandardItem(QString::fromLatin1(buf));
                    if(0 == col) {
                        m_item->setCheckable(true);
                        m_model->appendRow(m_item);
                        m_item->setCheckState(Qt::Checked);
                    }
                    else {
                        m_model->setItem(line - 1, col, m_item);
                        m_item->setCheckable(false);
                    }
                    ++col;
                }
                col = 0;
            }
        }

        ++line;
        memset(buf, 0, READ_LENGTH);
        str = buf;
    }
    fclose(m_ioCfgWid->m_pFile);
}

StatusIO::~StatusIO()
{
    delete m_ioCfgWid;
    delete m_treeViewIO;
    delete m_model;
}

IOConfigWidget::IOConfigWidget(QWidget *parent) : QDialog(parent)
{
   m_pFile = nullptr;
   m_thread = nullptr;
   m_treeView = new QTableView(this);
   m_vlayoutFinal.addWidget(m_treeView);

   m_txtFileSelect = new QLineEdit(this);
   std::string stdStr = string_TO_UTF8(g_ioCfgFileName);
   m_txtFileSelect->setText(QString::fromStdString(stdStr));
   m_btnFileSelect = new QPushButton(QStringLiteral("配置文件选择"), this);
   m_hlayoutFileSelect.addWidget(m_txtFileSelect);
   m_hlayoutFileSelect.addWidget(m_btnFileSelect);
   m_vlayoutFinal.addLayout(&m_hlayoutFileSelect);
   connect(m_btnFileSelect, SIGNAL(clicked()), this, SLOT(SlotOnBtnFileSelectClicked()));

   m_labProgress = new QLabel(QStringLiteral("进度:   "));
   m_labProgressInfo = new QLabel();
   m_barProgress = new QProgressBar(this);
   m_hlayoutProgress.addWidget(m_labProgress);
   m_hlayoutProgress.addWidget(m_barProgress);
   m_vlayoutFinal.addLayout(&m_hlayoutProgress);
   m_vlayoutFinal.addWidget(m_labProgressInfo);

   m_btnRun = new QPushButton(QStringLiteral("开始配置"), this);
   m_vlayoutFinal.addWidget(m_btnRun);
   connect(m_btnRun, SIGNAL(clicked()), this, SLOT(SlotRun()));
   m_thread = new QThread();
   m_btnRun->moveToThread(m_thread);
   connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));

   this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
   this->setLayout(&m_vlayoutFinal);
}

void IOConfigWidget::SlotRun()
{
    QList<QString> listCh;
    QStandardItem* item = new QStandardItem();
    int sizeRows = m_model->rowCount();
    int sizeCols = m_model->columnCount();
    m_barProgress->setRange(0, sizeRows);
    char* cstrName = new char[NAME_MAX];
    char* cstrWrite = new char[WRITE_MAX];
    memset(cstrName, 0, NAME_MAX);
    memset(cstrWrite, 0, WRITE_MAX);
    std::string stdStr = UTF8_TO_string(m_txtFileSelect->text().toStdString());
    g_ioCfgFileName = stdStr;
    m_pFile = fopen(g_ioCfgFileName.c_str(), "w+");
    QMessageBox msgBox;
    if(nullptr == m_pFile) {
        msgBox.setText("Open File:" + m_txtFileSelect->text() + "Failed!");
        msgBox.exec();
        return;
    }
    strcat(cstrWrite,", Type, Name, Chanel, Interface, Level, In/Out\n");
    fwrite(cstrWrite, sizeof(char), strlen(cstrWrite), m_pFile);
    QString errTitle, errMsg, qstrType, qstrName, qstrCh, qstrIf, qstrLev, qstrInout;
    errTitle = "ConfigError";
    static bool enable = false;
    for(int row = 1; row < sizeRows; ++row) {
        for(int col = 0; col < sizeCols; ++col) {
            item = m_model->item(row, col);
            if(0 == col) {
                if(Qt::Checked == item->checkState())
                   enable = true;
                else
                    enable = false;
            }
            if(0 == item) {
                errMsg = QString("Row:%1,Col:%2 is NULL, Please Enter a Value").arg(row).arg(col);
                errTitle = QString("ConfigDataError");
                ShowErrorMsg(errTitle, errMsg);
                break;
            }
            else {
                if(0 == col)
                    qstrType = item->text();
                else if(1 == col)
                    qstrName = item->text();
                else if(2 == col)
                    qstrCh = item->text();
                else if(3 == col)
                    qstrIf = item->text();
                else if(4 == col)
                    qstrLev = item->text();
                else if(5 == col)
                    qstrInout = item->text();
            }
        }

        if(listCh.contains(qstrCh)) {
            errTitle = "ConfigConflict";
            errMsg = qstrName + " Chanel: " + qstrCh + " Conflict!";
            ShowErrorMsg(errTitle, errMsg);
            break;
        }
        else
            listCh.push_back(qstrCh);

        std::string str = qstrIf.toStdString();
        if("DEV_POW" == qstrType) {
            if(str.npos != str.find("SMA")) {
                errTitle = "Device Config Error";
                errMsg = "DEV_POW:" + qstrName + ": Interface:" + qstrIf + " Configured Error!\n";
                errMsg += "DEV_POW Can Not Choose Sma Chanel!";
                ShowErrorMsg(errTitle, errMsg);
                break;
            }
        }
        if("DEV_SIG" == qstrType) {
            if(str.npos != str.find("DPS") || str.npos != str.find("IN/OUT")) {
                errTitle = "Device Config Error";
                errMsg = "DEV_SIG:" + qstrName + ": Interface:" + qstrIf + " Configured Error\n";
                errMsg += "DEV_SIG Can Not Choose DPS Or IN/OUT Chanel!";
                ShowErrorMsg(errTitle, errMsg);
                break;
            }
        }
        if(enable) {
            m_vIoCfgs.push_back(new IOConfigItem(qstrType, qstrName, qstrCh, qstrIf, qstrLev, qstrInout));
            Sleep(100);
            m_barProgress->setRange(1, (int)m_vIoCfgs.size());
            memset(cstrWrite, 0, WRITE_MAX);
            QString qstrWrite = qstrType + "," + qstrName + "," +qstrCh +"," + qstrIf + "," +qstrLev + ","+ qstrInout + "," + "\n";
            strcat(cstrWrite, qstrWrite.toStdString().c_str());
            fwrite(cstrWrite, 1, strlen(cstrWrite), m_pFile);
            m_barProgress->setValue((int)m_vIoCfgs.size());
        }
    }
    fclose(m_pFile);
}

void IOConfigWidget::SlotOnBtnFileSelectClicked()
{
    m_barProgress->setValue(m_barProgress->minimum());
    QString exePath = QApplication::applicationDirPath() + QString("/Config/");
    QString qStrName = QFileDialog::getOpenFileName(this, QStringLiteral("打开.csv文件"), exePath, "files(*.csv)");
    m_txtFileSelect->setText(qStrName);
    std::string stdStr = UTF8_TO_string(qStrName.toStdString());
    g_ioCfgFileName = stdStr;
}

void IOConfigWidget::CheckConflict()
{

}

void IOConfigWidget::ShowErrorMsg(QString &title, QString &msg)
{
    m_barProgress->setValue(m_barProgress->maximum());
    QMessageBox::warning(this, title, msg, QMessageBox::Ok);
    fclose(m_pFile);
}

void IOConfigWidget::closeEvent(QCloseEvent *)
{
    if(nullptr != m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
    m_thread = nullptr;
    m_barProgress->setValue(m_barProgress->minimum());
}

void IOConfigWidget::InitModelView(QStandardItemModel *model)
{
    QStringList strlist;
    strlist << "NULL";
    for(int i = 1; i <= 15; ++i)
        strlist << QString("IN/OUT_CH_%1").arg(i);
    for(int i = 0; i < 8; ++i)
        strlist << QString("DPS%1").arg(i);
    for(int i = 0; i < 8; ++i)
        strlist << QString("SMA%1").arg(i);
    m_treeView->setItemDelegateForColumn(3, new ComboBoxDeleget(strlist));
    strlist.clear();
    strlist << "NULL" <<"3.3v" << "5v";
    m_treeView->setItemDelegateForColumn(4, new ComboBoxDeleget(strlist));
    strlist.clear();
    strlist << "NULL" << "IN" << "OUT";
    m_treeView->setItemDelegateForColumn(5, new ComboBoxDeleget(strlist));
    this->m_model = model;
    this->m_treeView->setModel(model);
}

IOConfigWidget::~IOConfigWidget()
{
    if(nullptr != m_pFile) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
    if(nullptr != m_thread)
        m_thread->terminate();
    delete m_thread;
    delete m_btnFileSelect;
    delete m_txtFileSelect;
    delete m_btnRun;
    delete m_barProgress;
    delete m_labProgress;
    delete m_labProgressInfo;
    delete m_treeView;
}
std::string g_ioCfgFileName;


IOConfigItem::~IOConfigItem()
{
    m_type = "NULL";
    m_name = "NULL";
    m_chanel = "NULL";
    m_interface = "NULL";
    m_level = "NULL";
    m_inout = "NULL";
}

IOConfigItem::IOConfigItem(QString &type, QString &name, QString &chanel, QString &intfc, QString &level, QString &inOut) :
                           m_type(type), m_name(name), m_chanel(chanel), m_interface(intfc), m_level(level), m_inout(inOut)
{

}
