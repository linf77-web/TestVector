#include "Sds6034widget.h"
#include <QPushButton>
QMutex* g_qMutex;
Sds6000Lib* g_Sds6000;
char g_Sds6000AdressList [4][256];
unsigned long g_SdsHandle[4];
ImgShowThread::ImgShowThread(QLabel* lab, std::deque<std::string>* deque) : m_lab(lab), m_deque(deque)
{
    m_img = new QImage;
    m_path = QCoreApplication::applicationDirPath();
    m_path = "D:\\Work\\Sds6000\\Share\\";
}
void ImgShowThread::run(){
    while(1) {
        while(m_deque->size()>= DEQUE_MAX_SIZE) {
            g_qMutex->lock();
            m_deque->pop_front();
            g_qMutex->unlock();
        }
        if(!m_deque->empty() && !m_suspend) {
            QString fullName = m_path +  QString::fromStdString(m_deque->back());
            if(m_img->load(m_path + QString::fromStdString(m_deque->back())))
                m_lab->setPixmap(QPixmap::fromImage(*m_img));
        }
        else
            continue;
        sleep(1);
    }

}

void ImgShowThread::SlotShowImg(int index)
{
    std::deque<std::string>::iterator iter;
    iter = m_deque->end() - 1;
    iter += index;
    if(iter < m_deque->begin())
        iter = m_deque->begin();
    if(iter >= m_deque->end())
        iter = m_deque->end() - 1;
    QString fullName = m_path +  QString::fromStdString(*iter);
    if(m_img->load(fullName))
        m_lab->setPixmap(QPixmap::fromImage(*m_img));
}
Sds6034widget::Sds6034widget(QWidget *parent) : QDialog(parent)
{
    //m_cmbCH = new QComboBox(this);
    //QStringList strList;
    //for(int i = 1; i <= 4; ++i)
    //    strList << QString("CH%1").arg(i);
    //m_cmbCH->addItems(strList);
    m_sds6000Qt = new Sds6000Qt();
    m_thread = new QThread();
    m_sds6000Qt->moveToThread(m_thread);
	m_curDevIndex = 0;
	for (int i = 1; i <= 4; ++i)
        if(g_SdsHandle[i] > 0)
			m_vSeqSdsdev.push_back(std::pair<int, unsigned short>(i, g_SdsHandle[i]));
    m_labImgShow = new QLabel(QStringLiteral("ImgShow"));
    m_labImgShow->setMinimumSize(1280, 800);
    m_labImgShow->setMaximumSize(1200, 800);
    m_timer = new QTimer();
    m_timer->setInterval(5000);
    m_dequeImgName = new std::deque<std::string>(0);
    m_imgShowThrd = new ImgShowThread(m_labImgShow, m_dequeImgName);
    g_qMutex = new QMutex();
    m_sds6000 = new Sds6000Lib();
    m_labDevChoose = new QLabel(QStringLiteral("设备选择"), this);
	QStringList strlist;
	strlist << "1" << "2" << "3" << "4";
	m_cmbDevChoose = new QComboBox(this);
	m_cmbDevChoose->addItems(strlist);
    m_btnScreenShot = new QPushButton(QStringLiteral("截图"));
    m_btnForward = new QPushButton(QStringLiteral("上一张"));
    m_btnBackward = new QPushButton(QStringLiteral("下一张"));
	m_hlayoutOper.addWidget(m_labDevChoose, 0);
	m_hlayoutOper.addWidget(m_cmbDevChoose, 1);
    m_hlayoutOper.addWidget(m_btnScreenShot, 1);
    m_hlayoutOper.addWidget(m_btnForward, 2);
    m_hlayoutOper.addWidget(m_btnBackward, 2);
    m_hlayouDis.addWidget(m_labImgShow);
    m_vlayoutFinal.addLayout(&m_hlayoutOper);
    m_vlayoutFinal.addLayout(&m_hlayouDis);
    InitCfgWids();
    InitMenu();
    connect(m_cmbDevChoose, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotDevSelectChanged(int)));
    connect(m_btnScreenShot, SIGNAL(clicked()), this, SLOT(SlotScreenShot()));
    this->setLayout(&m_vlayoutFinal);
}

Sds6034widget::~Sds6034widget()
{
    delete m_menuBar;
    for(int i = 0; i < MENU_QTY; ++i)
        delete m_menus[i];
    delete m_labImgShow;
    delete m_timer;
    m_imgShowThrd->terminate();
    delete m_imgShowThrd;
    m_sds6000->Close();
    delete m_sds6000;
    delete g_qMutex;
    m_dequeImgName->clear();
    m_vMenueTitles.clear();
    delete m_sds6000Qt;
    delete m_thread;
    delete m_labDevChoose;
    delete m_cmbDevChoose;
    delete m_btnScreenShot;
    delete m_btnForward;
    delete m_btnBackward;
    delete m_lanWid;
    delete m_netStoreWid;
    delete m_disWid;
    delete m_acqWid;
    delete m_trigWid;
    delete m_cursorWid;
    delete m_measureWid;
}

void Sds6034widget::closeEvent(QCloseEvent *)
{
    if(nullptr != m_lanWid)
        m_lanWid->close();
    if(nullptr != m_netStoreWid)
        m_netStoreWid->close();
    if(nullptr != m_disWid)
        m_disWid->close();
    if(nullptr != m_acqWid)
        m_acqWid->close();
    if(nullptr != m_trigWid)
        m_trigWid->close();
    if(nullptr != m_cursorWid)
        m_cursorWid->close();
    if(nullptr != m_measureWid)
        m_measureWid->close();
}

void Sds6034widget::InitCfgWids()
{
    m_lanWid = nullptr;
    m_netStoreWid = nullptr;
    m_disWid = nullptr;
    m_acqWid = nullptr;
    m_trigWid = nullptr;
    m_cursorWid = nullptr;
    m_measureWid = nullptr;
}

void Sds6034widget::InitMenu()
{
    m_menuBar = new QMenuBar(this);
    m_vlayoutFinal.setMenuBar(m_menuBar);

    m_vMenueTitles.push_back(QStringLiteral("功能"));
    m_vMenueTitles.push_back(QStringLiteral("显示"));
    m_vMenueTitles.push_back(QStringLiteral("采样"));
    m_vMenueTitles.push_back(QStringLiteral("触发"));
    m_vMenueTitles.push_back(QStringLiteral("光标"));
    m_vMenueTitles.push_back(QStringLiteral("测量"));
    for(int i = 0; i < MENU_QTY; ++i) {
        m_menus[i] = new QMenu(m_menuBar);
        m_menuBar->addAction(m_menus[i]->menuAction());
        m_menus[i]->setTitle(m_vMenueTitles[i]);
    }
    QList<QAction*> actions;
    actions.push_back(new QAction(QStringLiteral("菜单")));
    actions.push_back(new QAction(QStringLiteral("存储/调出")));
    actions.push_back(new QAction(QStringLiteral("函数发生器")));
    actions.push_back(new QAction(QStringLiteral("系统设置")));
    actions.push_back(new QAction(QStringLiteral("局域网设置")));
    actions.push_back(new QAction(QStringLiteral("网络存储设置")));
    actions.push_back(new QAction(QStringLiteral("截图")));
    actions.push_back(new QAction(QStringLiteral("重启")));
    actions.push_back(new QAction(QStringLiteral("关机")));
    m_menus[0]->addActions(actions);
    connect(actions[4], SIGNAL(triggered()), this, SLOT(SlotLanWidShow()));
    connect(actions[5], SIGNAL(triggered()), this, SLOT(SlotNetStoreWidShow()));
    connect(actions[6], SIGNAL(triggered()), this, SLOT(SlotScreenShot()));

    actions.clear();
    actions.push_back(new QAction(QStringLiteral("菜单")));
    actions.push_back(new QAction(QStringLiteral("余晖")));
    actions.push_back(new QAction(QStringLiteral("清楚显示")));
    actions.push_back(new QAction(QStringLiteral("完整网格")));
    actions.push_back(new QAction(QStringLiteral("轻网格")));
    actions.push_back(new QAction(QStringLiteral("无网格")));
    m_menus[1]->addActions(actions);
    connect(actions[0], SIGNAL(triggered()), this, SLOT(SlotDisWidShow()));

    actions.clear();
    actions.push_back(new QAction(QStringLiteral("菜单")));
    actions.push_back(new QAction(QStringLiteral("分段采样")));
    actions.push_back(new QAction(QStringLiteral("XY模式")));
    actions.push_back(new QAction(QStringLiteral("Zoom模式")));
    actions.push_back(new QAction(QStringLiteral("滚动模式")));
    actions.push_back(new QAction(QStringLiteral("清除扫描")));
    actions.push_back(new QAction(QStringLiteral("运行/停止")));
    actions.push_back(new QAction(QStringLiteral("自动设置")));
    actions.push_back(new QAction(QStringLiteral("默认设置")));
    m_menus[2]->addActions(actions);
    connect(actions[0], SIGNAL(triggered()), this, SLOT(SlotAcqWidShow()));

    actions.clear();
    actions.push_back(new QAction(QStringLiteral("菜单")));
    actions.push_back(new QAction(QStringLiteral("区域")));
    actions.push_back(new QAction(QStringLiteral("自动")));
    actions.push_back(new QAction(QStringLiteral("单次")));
    actions.push_back(new QAction(QStringLiteral("正常")));
    m_menus[3]->addActions(actions);
    connect(actions[0], SIGNAL(triggered()), this, SLOT(SlotTrigWidShow()));

    actions.clear();
    actions.push_back(new QAction(QStringLiteral("菜单")));
    actions.push_back(new QAction(QStringLiteral("X")));
    actions.push_back(new QAction(QStringLiteral("Y")));
    actions.push_back(new QAction(QStringLiteral("X-Y")));
    m_menus[4]->addActions(actions);
    connect(actions[0], SIGNAL(triggered()), this, SLOT(SlotCursorWidShow()));

    actions.clear();
    actions.push_back(new QAction(QStringLiteral("菜单")));
    actions.push_back(new QAction(QStringLiteral("基本测量")));
    actions.push_back(new QAction(QStringLiteral("高级测量")));
    m_menus[5]->addActions(actions);
    connect(actions[0], SIGNAL(triggered()), this, SLOT(SlotMeasureWidShow()));
    actions.clear();
}

void Sds6034widget::SlotScreenShot()
{
    std::string time = currTime();
    std::string name = time + ".png";
    g_qMutex->lock();
    m_dequeImgName->push_back(name);
    g_qMutex->unlock();
    m_sds6000Qt->m_sds6000->Save(g_SdsHandle[m_curDevIndex], SV_IMAGE, SV_NET, name.c_str());
}

void Sds6034widget::SlotDevSelectChanged(int index)
{
	m_curDevIndex = index;
}

void Sds6034widget::SlotLanWidShow()
{
    if(nullptr == m_lanWid)
        m_lanWid = new CfgLanWid(this);
    m_lanWid->setWindowTitle(QStringLiteral("局域网设置"));
    m_lanWid->adjustSize();
    m_lanWid->show();
}

void Sds6034widget::SlotNetStoreWidShow()
{
    if(nullptr == m_netStoreWid)
        m_netStoreWid = new CfgNetStoreWid(this);
    m_netStoreWid->setWindowTitle(QStringLiteral("网络存储"));
    m_netStoreWid->adjustSize();
    m_netStoreWid->show();
}

void Sds6034widget::SlotDisWidShow()
{
    if(nullptr == m_disWid)
        m_disWid = new CfgDisWid(this);
    m_disWid->setWindowTitle(QStringLiteral("显示设置"));
    m_disWid->adjustSize();
    m_disWid->show();
}

void Sds6034widget::SlotAcqWidShow()
{
    if(nullptr == m_acqWid)
        m_acqWid = new CfgAcquireWid(this);
    m_acqWid->setWindowTitle(QStringLiteral("采样设置"));
    m_acqWid->adjustSize();
    m_acqWid->show();
}

void Sds6034widget::SlotTrigWidShow()
{
    if(nullptr == m_trigWid)
        m_trigWid = new CfgTrigWid(this);
    m_trigWid->setWindowTitle(QStringLiteral("触发设置"));
    m_trigWid->adjustSize();
    m_trigWid->show();
}

void Sds6034widget::SlotCursorWidShow()
{
    if(nullptr == m_cursorWid)
        m_cursorWid = new CfgCursorWid(this);
    m_cursorWid->setWindowTitle(QStringLiteral("光标设置"));
    m_cursorWid->adjustSize();
    m_cursorWid->show();
}

void Sds6034widget::SlotMeasureWidShow()
{
    if(nullptr == m_measureWid)
        m_measureWid = new CfgMeasureWid(this);
    m_measureWid->setWindowTitle(QStringLiteral("测量设置"));
    m_measureWid->adjustSize();
    m_measureWid->show();
}


CfgLanWid::CfgLanWid(QWidget *parent) : QDialog(parent)
{
    m_labLanType = new QLabel(QStringLiteral("自动/手动"));
    m_radioBtnLanType = new QRadioButton(QStringLiteral("手动"));
    m_hlayoutLanType.addWidget(m_labLanType);
    m_hlayoutLanType.addWidget(m_radioBtnLanType);
    m_vlayoutFinal.addLayout(&m_hlayoutLanType);

    m_labIpAddr = new QLabel(QStringLiteral("IP地址"));
    m_txtIpAddr = new QLineEdit(this);
    m_hlayoutIpAddr.addWidget(m_labIpAddr);
    m_hlayoutIpAddr.addWidget(m_txtIpAddr);
    m_vlayoutFinal.addLayout(&m_hlayoutIpAddr);

    m_labSmask = new QLabel(QStringLiteral("子网掩码"));
    m_txtSmask = new QLineEdit(this);
    m_hlayoutSmask.addWidget(m_labSmask);
    m_hlayoutSmask.addWidget(m_txtSmask);
    m_vlayoutFinal.addLayout(&m_hlayoutSmask);

    m_labGateWay = new QLabel(QStringLiteral("网关"));
    m_txtGateWay = new QLineEdit(this);
    m_hlayoutGateWay.addWidget(m_labGateWay);
    m_hlayoutGateWay.addWidget(m_txtGateWay);
    m_vlayoutFinal.addLayout(&m_hlayoutGateWay);

    m_labMac = new QLabel(QStringLiteral("MAC地址"));
    m_txtMac = new QLineEdit(this);
    m_hlayoutMac.addWidget(m_labMac);
    m_hlayoutMac.addWidget(m_txtMac);
    m_vlayoutFinal.addLayout(&m_hlayoutMac);

    m_labVncPort = new QLabel(QStringLiteral("VNC端口"));
    m_txtVncPort = new QLineEdit(this);
    m_hlayoutVncPort.addWidget(m_labVncPort);
    m_hlayoutVncPort.addWidget(m_txtVncPort);
    m_vlayoutFinal.addLayout(&m_hlayoutVncPort);

    m_btnApply = new QPushButton(QStringLiteral("确认"));
    m_btnCancel = new QPushButton(QStringLiteral("取消"));
    m_hlayoutApply.addWidget(m_btnApply);
    m_hlayoutApply.addWidget(m_btnCancel);
    m_vlayoutFinal.addLayout(&m_hlayoutApply);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgLanWid::~CfgLanWid()
{
    delete m_labIpAddr;
    delete m_txtIpAddr;
    delete m_labGateWay;
    delete m_txtGateWay;
    delete m_labMac;
    delete m_txtMac;
    delete m_labSmask;
    delete m_txtSmask;
    delete m_labLanType;
    delete m_radioBtnLanType;
    delete m_labVncPort;
    delete m_txtVncPort;
    delete m_btnApply;
    delete m_btnCancel;
}

CfgNetStoreWid::CfgNetStoreWid(QWidget *parent) : QDialog(parent)
{
    m_labDrv = new QLabel(QStringLiteral("驱动器"));
    m_cmbDrv = new QComboBox(this);
    QStringList strList;
    strList << "E:" <<"F:" <<"G:" << "H:" << "I";
    m_cmbDrv->addItems(strList);
    m_hlayoutDrv.addWidget(m_labDrv);
    m_hlayoutDrv.addWidget(m_cmbDrv);
    m_vlayoutFinal.addLayout(&m_hlayoutDrv);

    m_labPath = new QLabel(QStringLiteral("路径"));
    m_txtPath = new QLineEdit(this);
    m_ckbRmemPath = new QCheckBox(QStringLiteral("记住"), this);
    m_ckbRmemPath->setChecked(true);
    m_hlayoutPath.addWidget(m_labPath);
    m_hlayoutPath.addWidget(m_txtPath);
    m_hlayoutPath.addWidget(m_ckbRmemPath);
    m_vlayoutFinal.addLayout(&m_hlayoutPath);

    m_ckbAnonymous = new QCheckBox(QStringLiteral("匿名"), this);
    m_ckbAnonymous->setChecked(false);
    m_vlayoutFinal.addWidget(m_ckbAnonymous);

    m_labUsrName = new QLabel(QStringLiteral("用户名"));
    m_txtUsrName = new QLineEdit(this);
    m_ckbRmemName = new QCheckBox(QStringLiteral("记住"), this);
    m_ckbRmemName->setChecked(true);
    m_hlayoutName.addWidget(m_labUsrName);
    m_hlayoutName.addWidget(m_txtUsrName);
    m_hlayoutName.addWidget(m_ckbRmemName);
    m_vlayoutFinal.addLayout(&m_hlayoutName);

    m_labPasswd = new QLabel(QStringLiteral("密码"));
    m_txtPasswd = new QLineEdit(this);
    m_ckbRmemPasswd = new QCheckBox(QStringLiteral("记住"), this);
    m_ckbRmemPasswd->setChecked(true);
    m_hlayoutPasswd.addWidget(m_labPasswd);
    m_hlayoutPasswd.addWidget(m_txtPasswd);
    m_hlayoutPasswd.addWidget(m_ckbRmemPasswd);
    m_vlayoutFinal.addLayout(&m_hlayoutPasswd);

    m_ckbAutoConnc = new QCheckBox(QStringLiteral("自动链接"), this);
    m_ckbAutoConnc->setChecked(true);
    m_vlayoutFinal.addWidget(m_ckbAutoConnc);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgNetStoreWid::~CfgNetStoreWid()
{
    delete m_labDrv;
    delete m_cmbDrv;
    delete m_labPath;
    delete m_txtPath;
    delete m_ckbRmemPath;
    delete m_ckbAnonymous;
    delete m_labUsrName;
    delete m_txtUsrName;
    delete m_ckbRmemName;
    delete m_labPasswd;
    delete m_txtPasswd;
    delete m_ckbRmemPasswd;
    delete m_ckbAutoConnc;
}

CfgDisWid::CfgDisWid(QWidget *parent) : QDialog(parent)
{
    m_labType = new QLabel(QStringLiteral("类型"));
    m_cmbType = new QComboBox(this);
    QStringList strlist;
    strlist << QStringLiteral("矢量") << QStringLiteral("点");
    m_cmbType->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    m_labColorTemper = new QLabel(QStringLiteral("色温"));
    m_radiobtnColorTemper = new QRadioButton(QStringLiteral("打开"), this);
    m_hlayoutColorTemper.addWidget(m_labColorTemper);
    m_hlayoutColorTemper.addWidget(m_radiobtnColorTemper);
    m_vlayoutFinal.addLayout(&m_hlayoutColorTemper);

    m_labAfterGlow = new QLabel(QStringLiteral("余晖"));
    m_cmbAfterGlow = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("余晖1") << QStringLiteral("余晖2") << QStringLiteral("关闭");
    m_cmbAfterGlow->addItems(strlist);
    m_hlayoutAfterGlow.addWidget(m_labAfterGlow);
    m_hlayoutAfterGlow.addWidget(m_cmbAfterGlow);
    m_vlayoutFinal.addLayout(&m_hlayoutAfterGlow);

    m_btnClear = new QPushButton(QStringLiteral("清除显示"), this);
    m_vlayoutFinal.addWidget(m_btnClear);

    m_labHidenMenu = new QLabel(QStringLiteral("隐藏菜单"));
    m_cmbHidenMenu = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("3秒") << QStringLiteral("5秒") << QStringLiteral("10秒");
    m_cmbHidenMenu->addItems(strlist);
    m_hlayoutHidenMenu.addWidget(m_labHidenMenu);
    m_hlayoutHidenMenu.addWidget(m_cmbHidenMenu);
    m_vlayoutFinal.addLayout(&m_hlayoutHidenMenu);

    m_labMenuStyle = new QLabel(QStringLiteral("菜单样式"));
    m_cmbMenuStyle = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("内嵌") << QStringLiteral("悬浮");
    m_cmbMenuStyle->addItems(strlist);
    m_hlayoutMenuStyle.addWidget(m_labMenuStyle);
    m_hlayoutMenuStyle.addWidget(m_cmbMenuStyle);
    m_vlayoutFinal.addLayout(&m_hlayoutMenuStyle);

    m_labGrid = new QLabel(QStringLiteral("网格"));
    m_cmbGrid = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("完整网格") << QStringLiteral("高亮") << QStringLiteral("无");
    m_cmbGrid->addItems(strlist);
    m_hlayoutGrid.addWidget(m_labGrid);
    m_hlayoutGrid.addWidget(m_cmbGrid);
    m_vlayoutFinal.addLayout(&m_hlayoutGrid);

    m_labWaveIntensity = new QLabel(QStringLiteral("波形亮度"));
    m_txtWaveIntensity = new QLineEdit(this);
    m_hlayoutWaveIntensity.addWidget(m_labWaveIntensity);
    m_hlayoutWaveIntensity.addWidget(m_txtWaveIntensity);
    m_vlayoutFinal.addLayout(&m_hlayoutWaveIntensity);

    m_labBackLight = new QLabel(QStringLiteral("背光亮度"));
    m_txtBackLight = new QLineEdit(this);
    m_hlayoutBackLight.addWidget(m_labBackLight);
    m_hlayoutBackLight.addWidget(m_txtBackLight);
    m_vlayoutFinal.addLayout(&m_hlayoutBackLight);

    m_labGridLight = new QLabel(QStringLiteral("网格亮度"));
    m_txtGridLight = new QLineEdit(this);
    m_hlayoutGridLight.addWidget(m_labGridLight);
    m_hlayoutGridLight.addWidget(m_txtGridLight);
    m_vlayoutFinal.addLayout(&m_hlayoutGridLight);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgDisWid::~CfgDisWid()
{
    delete m_labType;
    delete m_cmbType;
    delete m_labColorTemper;
    delete m_radiobtnColorTemper;
    delete m_labAfterGlow;
    delete m_cmbAfterGlow;
    delete m_btnClear;
    delete m_labHidenMenu;
    delete m_cmbHidenMenu;
    delete m_labMenuStyle;
    delete m_cmbMenuStyle;
    delete m_labGrid;
    delete m_cmbGrid;
    delete m_labWaveIntensity;
    delete m_txtWaveIntensity;
    delete m_labBackLight;
    delete m_txtBackLight;
    delete m_labGridLight;
    delete m_txtGridLight;
}

CfgAcquireWid::CfgAcquireWid(QWidget *parent) : QDialog(parent)
{
    m_labInterp = new QLabel(QStringLiteral("内插方式"));
    m_cmbInterp = new QComboBox(this);
    QStringList strlist;
    strlist << QStringLiteral("X") << QStringLiteral("Sinc");
    m_cmbInterp->addItems(strlist);
    m_hlayoutInterp.addWidget(m_labInterp);
    m_hlayoutInterp.addWidget(m_cmbInterp);
    m_vlayoutFinal.addLayout(&m_hlayoutInterp);

    m_labAcqMode = new QLabel(QStringLiteral("采集模式"));
    m_cmbAcqMode = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("快采") << QStringLiteral("慢采");
    m_cmbAcqMode->addItems(strlist);
    m_hlayoutMode.addWidget(m_labAcqMode);
    m_hlayoutMode.addWidget(m_cmbAcqMode);
    m_vlayoutFinal.addLayout(&m_hlayoutMode);

    m_labAcqType = new QLabel(QStringLiteral("采集方式"));
    m_cmbAcqType = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("普通") << QStringLiteral("峰值检测") << QStringLiteral("平均值") << QStringLiteral("增强分辨率");
    m_cmbAcqType->addItems(strlist);
    m_hlayoutType.addWidget(m_labAcqType);
    m_hlayoutType.addWidget(m_cmbAcqType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    m_labMemMode = new QLabel(QStringLiteral("存储模式"));
    m_cmbMemMode = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("自动") << QStringLiteral("固定采样率") << QStringLiteral("固定存储深度");
    m_cmbMemMode->addItems(strlist);
    m_hlayoutMemMode.addWidget(m_labMemMode);
    m_hlayoutMemMode.addWidget(m_cmbMemMode);
    m_vlayoutFinal.addLayout(&m_hlayoutMemMode);

    m_labMemDepth = new QLabel(QStringLiteral("最大存储深度"));
    m_cmbMemDepth = new QComboBox(this);
    strlist.clear();
    strlist << "1.25k" << "2.5k" << "12.5k" << "25k" << "125k" << "250k" << "1.25M" << "2.5M" << "12.5M" << "62.5M" << "125M";
    m_cmbMemDepth->addItems(strlist);
    m_hlayoutMemDepth.addWidget(m_labMemDepth);
    m_hlayoutMemDepth.addWidget(m_cmbMemDepth);
    m_vlayoutFinal.addLayout(&m_hlayoutMemDepth);

    m_labSequence = new QLabel(QStringLiteral("分段采样"));
    m_radioBtnSequence = new QRadioButton(QStringLiteral("打开"), this);
    m_txtSequenceCnt = new QLineEdit(this);
    m_hlayoutSequence.addWidget(m_labSequence);
    m_hlayoutSequence.addWidget(m_radioBtnSequence);
    m_hlayoutSequence.addWidget(m_txtSequenceCnt);
    m_vlayoutFinal.addLayout(&m_hlayoutSequence);

    m_labXYMode = new QLabel(QStringLiteral("XY模式"));
    m_radioBtnXYMode = new QRadioButton(QStringLiteral("打开"));
    m_hlayoutXYMode.addWidget(m_labXYMode);
    m_hlayoutXYMode.addWidget(m_radioBtnXYMode);
    m_vlayoutFinal.addLayout(&m_hlayoutXYMode);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgAcquireWid::~CfgAcquireWid()
{
    delete m_labInterp;
    delete m_cmbInterp;
    delete m_labAcqMode;
    delete m_cmbAcqMode;
    delete m_labAcqType;
    delete m_cmbAcqType;
    delete m_labMemMode;
    delete m_cmbMemMode;
    delete m_labMemDepth;
    delete m_cmbMemDepth;
    delete m_labSequence;
    delete m_txtSequenceCnt;
    delete m_labXYMode;
    delete m_radioBtnXYMode;
}

CfgTrigWid::CfgTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labType = new QLabel(QStringLiteral("类型"));
    m_cmbtype = new QComboBox(this);
    QStringList strlist;
    strlist << QStringLiteral("无")
            << QStringLiteral("边沿")     << QStringLiteral("斜率")        << QStringLiteral("脉宽")    << QStringLiteral("视频")
            << QStringLiteral("窗口")     << QStringLiteral("间隔")        << QStringLiteral("超时")    << QStringLiteral("欠幅")
            << QStringLiteral("码型")     << QStringLiteral("QUALified")  << QStringLiteral("延迟")    << QStringLiteral("第N边沿")
            << QStringLiteral("建立/保持") << QStringLiteral("IIC总线")     << QStringLiteral("SPI总线") << QStringLiteral("UART总线")
            << QStringLiteral("CAN总线")  << QStringLiteral("LIN总线");
    m_cmbtype->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbtype);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    m_labFreq = new QLabel(QStringLiteral("频率"));
    m_txtFreq = new QLineEdit(this);
    m_hlayoutFreq.addWidget(m_labFreq);
    m_hlayoutFreq.addWidget(m_txtFreq);
    m_vlayoutFinal.addLayout(&m_hlayoutFreq);

    m_labMode = new QLabel(QStringLiteral("模式"));
    m_cmbMode = new QComboBox(this);
    strlist.clear();
    strlist << QStringLiteral("Single") << QStringLiteral("Normal") << QStringLiteral("Auto") << QStringLiteral("Ftrig");
    m_cmbMode->addItems(strlist);
    m_hlayoutMode.addWidget(m_labMode);
    m_hlayoutMode.addWidget(m_cmbMode);
    m_vlayoutFinal.addLayout(&m_hlayoutMode);

    m_btnRunStop = new QPushButton("Run/Stop");
    m_vlayoutFinal.addWidget(m_btnRunStop);

    InitChildWid();
    m_curWid = m_lastWid = nullptr;

    connect(m_cmbtype, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotSelectChildWid(int)));
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgTrigWid::~CfgTrigWid()
{
    delete m_labType;
    delete m_cmbtype;
    delete m_labFreq;
    delete m_txtFreq;
    delete m_labMode;
    delete m_cmbMode;
    delete m_btnRunStop;
    delete m_edgeTrigWid;
    delete m_slopTrigWid;
    delete m_pulseTrigWid;
    delete m_videoTrigWid;
    delete m_wndTrigWid;
    delete m_INTTrigWid;
    delete m_dropOutTrigWid;
    delete m_runtTrigWid;
    delete m_patternTrigWid;
    delete m_qualTrigWid;
    delete m_delayTrigWid;
    delete m_nEdgeTrigWid;
    delete m_sHoldTrigWid;
    delete m_IIcTrigWid;
    delete m_SPITrigWid;
    delete m_UARTTrigWid;
    delete m_CANTrigWid;
    delete m_LINTrigWid;
}

void CfgTrigWid::closeEvent(QCloseEvent *)
{
    if(nullptr != m_curWid)
        m_curWid->close();
    if(nullptr != m_lastWid)
        m_lastWid->close();
    this->close();
}

void CfgTrigWid::InitChildWid()
{
    m_edgeTrigWid = nullptr;
    m_slopTrigWid = nullptr;
    m_pulseTrigWid = nullptr;
    m_videoTrigWid = nullptr;
    m_wndTrigWid = nullptr;
    m_INTTrigWid = nullptr;
    m_dropOutTrigWid = nullptr;
    m_runtTrigWid = nullptr;
    m_patternTrigWid = nullptr;
    m_qualTrigWid = nullptr;
    m_delayTrigWid = nullptr;
    m_nEdgeTrigWid = nullptr;
    m_sHoldTrigWid = nullptr;
    m_IIcTrigWid = nullptr;
    m_SPITrigWid = nullptr;
    m_UARTTrigWid = nullptr;
    m_CANTrigWid = nullptr;
    m_LINTrigWid = nullptr;
}

void CfgTrigWid::SlotSelectChildWid(int item)
{
    if(nullptr != m_lastWid)
        m_lastWid->close();
    switch (item) {
    case 0:
        m_curWid = nullptr;
        break;
    case 1:
        if(nullptr == m_edgeTrigWid)
            m_edgeTrigWid = new EdgeTrigWid(this);
        m_curWid = (QWidget*)m_edgeTrigWid;
        break;
    case 2:
        if(nullptr == m_slopTrigWid)
            m_slopTrigWid = new SlopTrigWid(this);
        m_curWid = (QWidget*)m_slopTrigWid;
        break;
    case 3:
        if(nullptr == m_pulseTrigWid)
            m_pulseTrigWid = new PulseTrigWid(this);
        m_curWid = (QWidget*)m_pulseTrigWid;
        break;
    case 4:
        if(nullptr == m_videoTrigWid)
             m_videoTrigWid = new VideoTrigWid(this);
        m_curWid = (QWidget*)m_videoTrigWid;
        break;
    case 5:
        if(nullptr == m_wndTrigWid)
            m_wndTrigWid = new WindowTrigWid(this);
        m_curWid = (QWidget*)m_wndTrigWid;
        break;
    case 6:
        if(nullptr == m_INTTrigWid)
            m_INTTrigWid = new INTTrigWid(this);
        m_curWid = (QWidget*)m_INTTrigWid;
        break;
    case 7:
        if(nullptr == m_dropOutTrigWid)
            m_dropOutTrigWid = new DropOutTrigWid(this);
        m_curWid = (QWidget*)m_dropOutTrigWid;
        break;
    case 8:
        if(nullptr == m_runtTrigWid)
            m_runtTrigWid = new RuntTrigWid(this);
        m_curWid = (QWidget*)m_runtTrigWid;
        break;
    case 9:
        if(nullptr == m_patternTrigWid)
            m_patternTrigWid = new PatternTrigWid(this);
        m_curWid = (QWidget*)m_patternTrigWid;
        break;
    case 10:
        if(nullptr == m_qualTrigWid)
            m_qualTrigWid = new QUALTrigWid(this);
        m_curWid = (QWidget*)m_qualTrigWid;
        break;
    case 11:
        if(nullptr == m_delayTrigWid)
            m_delayTrigWid = new DelayTrigWid(this);
        m_curWid = (QWidget*)m_delayTrigWid;
        break;
    case 12:
        if(nullptr == m_nEdgeTrigWid)
            m_nEdgeTrigWid = new NedgeTrigWid(this);
        m_curWid = (QWidget*)m_nEdgeTrigWid;
        break;
    case 13:
        if(nullptr == m_sHoldTrigWid)
            m_sHoldTrigWid = new SHoldTrigWid(this);
        m_curWid = (QWidget*)m_sHoldTrigWid;
        break;
    case 14:
        if(nullptr == m_IIcTrigWid)
            m_IIcTrigWid = new IICTrigWid(this);
        m_curWid = (QWidget*)m_IIcTrigWid;
        break;
    case 15:
        if(nullptr == m_SPITrigWid)
            m_SPITrigWid = new SPITrigWid(this);
        m_curWid = (QWidget*)m_SPITrigWid;
        break;
    case 16:
        if(nullptr == m_UARTTrigWid)
            m_UARTTrigWid = new UARTTrigWid(this);
        m_curWid = (QWidget*)m_UARTTrigWid;
        break;
    case 17:
        if(nullptr == m_CANTrigWid)
            m_CANTrigWid = new CANTrigWid(this);
        m_curWid = (QWidget*)m_CANTrigWid;
        break;
    case 18:
        if(nullptr == m_LINTrigWid)
            m_LINTrigWid = new LINTrigWid(this);
        m_curWid = (QWidget*)m_LINTrigWid;
        break;
    default:
        break;
    }
    m_lastWid = m_curWid;
    if(nullptr != m_curWid) {
        m_curWid->adjustSize();
        m_curWid->move(this->x() - 300, this->y());
        m_curWid->show();
    }
}

EdgeTrigWid::EdgeTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labIMP = new QLabel(QStringLiteral("IMPedance"));
    m_cmbIMP = new QComboBox(this);
    strlist.clear();
    strlist << "ONEMeg" << "FIFTy";
    m_cmbIMP->addItems(strlist);
    m_hlayoutIMP.addWidget(m_labIMP);
    m_hlayoutIMP.addWidget(m_cmbIMP);
    m_vlayoutFinal.addLayout(&m_hlayoutIMP);

    m_labLevel = new QLabel(QStringLiteral("Level"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing" << "ALTernate";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("边沿触发"));
    this->setLayout(&m_vlayoutFinal);
}

EdgeTrigWid::~EdgeTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labIMP;
    delete m_cmbIMP;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labSource;
    delete m_cmbSource;
}

SlopTrigWid::SlopTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHLevel = new QLabel(QStringLiteral("HLevel"));
    m_txtHLevel = new QLineEdit(this);
    m_hlayoutHLevel.addWidget(m_labHLevel);
    m_hlayoutHLevel.addWidget(m_txtHLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutHLevel);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labLLevel = new QLabel(QStringLiteral("LLevel"));
    m_txtLLevel = new QLineEdit(this);
    m_hlayoutLLevel.addWidget(m_labLLevel);
    m_hlayoutLLevel.addWidget(m_txtLLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLLevel);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing" << "ALTernate";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("斜率触发"));
    this->setLayout(&m_vlayoutFinal);
}

SlopTrigWid::~SlopTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHLevel;
    delete m_txtHLevel;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labLLevel;
    delete m_txtLLevel;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labSource;
    delete m_cmbSource;
}

PulseTrigWid::PulseTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labLevel = new QLabel(QStringLiteral("Level"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labPol = new QLabel(QStringLiteral("Pol"));
    m_cmbPol = new QComboBox(this);
    strlist.clear();
    strlist << "POSitive" << "NEGative";
    m_cmbPol->addItems(strlist);
    m_hlayoutPol.addWidget(m_labPol);
    m_hlayoutPol.addWidget(m_cmbPol);
    m_vlayoutFinal.addLayout(&m_hlayoutPol);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("脉宽触发"));
    this->setLayout(&m_vlayoutFinal);
}

PulseTrigWid::~PulseTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labPol;
    delete m_cmbPol;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
}

VideoTrigWid::VideoTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labFcnt = new QLabel(QStringLiteral("FCNT"));
    m_cmbFcnt = new QComboBox(this);
    QStringList strlist;
    strlist << "1" << "2" << "4" << "8";
    m_cmbFcnt->addItems(strlist);
    m_hlayoutFcnt.addWidget(m_labFcnt);
    m_hlayoutFcnt.addWidget(m_cmbFcnt);
    m_vlayoutFinal.addLayout(&m_hlayoutFcnt);

    m_labField = new QLabel(QStringLiteral("FIELd"));
    m_cmbField = new QComboBox(this);
    strlist.clear();
    strlist << "1" << "2";
    m_cmbField->addItems(strlist);
    m_hlayoutField.addWidget(m_labField);
    m_hlayoutField.addWidget(m_cmbField);
    m_vlayoutFinal.addLayout(&m_hlayoutField);

    m_labFrate = new QLabel(QStringLiteral("FRATe"));
    m_cmbFrate = new QComboBox(this);
    strlist.clear();
    strlist << "25Hz" << "30Hz" << "50Hz" << "60Hz";
    m_cmbFrate->addItems(strlist);
    m_hlayoutFrate.addWidget(m_labFrate);
    m_hlayoutFrate.addWidget(m_cmbFrate);
    m_vlayoutFinal.addLayout(&m_hlayoutFrate);

    m_labInterl = new QLabel(QStringLiteral("INTerlace"));
    m_cmbInterl = new QComboBox(this);
    strlist.clear();
    strlist << "1" << "2" << "4" << "8";
    m_cmbInterl->addItems(strlist);
    m_hlayoutInterl.addWidget(m_labInterl);
    m_hlayoutInterl.addWidget(m_cmbInterl);
    m_vlayoutFinal.addLayout(&m_hlayoutInterl);

    m_labLcnt = new QLabel(QStringLiteral("LCNT"));
    m_txtLcnt = new QLineEdit(this);
    m_hlayoutLcnt.addWidget(m_labLcnt);
    m_hlayoutLcnt.addWidget(m_txtLcnt);
    m_vlayoutFinal.addLayout(&m_hlayoutLcnt);

    m_labLevel = new QLabel(QStringLiteral("Level"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labLine = new QLabel(QStringLiteral("Line"));
    m_txtLine = new QLineEdit(this);
    m_hlayoutLine.addWidget(m_labLine);
    m_hlayoutLine.addWidget(m_txtLine);
    m_vlayoutFinal.addLayout(&m_hlayoutLine);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labStand = new QLabel(QStringLiteral("STANdard"));
    m_cmbStand = new QComboBox(this);
    strlist.clear();
    strlist << "NTSC" << "PAL" << "P720L50" << "P720L60" << "P1080L50" << "P1080L60" << "I1080L50" << "I1080L60" << "CUSTom";
    m_cmbStand->addItems(strlist);
    m_hlayoutStand.addWidget(m_labStand);
    m_hlayoutStand.addWidget(m_cmbStand);
    m_vlayoutFinal.addLayout(&m_hlayoutStand);

    m_labSync = new QLabel(QStringLiteral("SYNC"));
    m_cmbSync = new QComboBox(this);
    strlist.clear();
    strlist << "SELec" << "ANY";
    m_cmbSync->addItems(strlist);
    m_hlayoutSync.addWidget(m_labSync);
    m_hlayoutSync.addWidget(m_cmbSync);
    m_vlayoutFinal.addLayout(&m_hlayoutSync);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("视频触发"));
    this->setLayout(&m_vlayoutFinal);
}

VideoTrigWid::~VideoTrigWid()
{
    delete m_labFcnt;
    delete m_cmbFcnt;
    delete m_labField;
    delete m_cmbField;
    delete m_labFrate;
    delete m_cmbFrate;
    delete m_labInterl;
    delete m_cmbInterl;
    delete m_labLcnt;
    delete m_txtLcnt;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labLine;
    delete m_txtLine;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labStand;
    delete m_cmbStand;
    delete m_labSync;
    delete m_cmbSync;
}

WindowTrigWid::WindowTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCLevel = new QLabel(QStringLiteral("CLEVel"));
    m_txtCLevel = new QLineEdit(this);
    m_hlayoutCLevel.addWidget(m_labCLevel);
    m_hlayoutCLevel.addWidget(m_txtCLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutCLevel);

    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labDLevel = new QLabel(QStringLiteral("DLEVel"));
    m_txtDLevel = new QLineEdit(this);
    m_hlayoutDLevel.addWidget(m_labDLevel);
    m_hlayoutDLevel.addWidget(m_txtDLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutDLevel);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHLevel = new QLabel(QStringLiteral("HLEVel"));
    m_txtHLevel = new QLineEdit(this);
    m_hlayoutHLevel.addWidget(m_labHLevel);
    m_hlayoutHLevel.addWidget(m_txtHLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutHLevel);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labLLevel = new QLabel(QStringLiteral("LLEVel"));
    m_txtLLevel = new QLineEdit(this);
    m_hlayoutLLevel.addWidget(m_labLLevel);
    m_hlayoutLLevel.addWidget(m_txtLLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLLevel);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labType = new QLabel(QStringLiteral("TYPE"));
    m_cmbType = new QComboBox(this);
    strlist.clear();
    strlist << "ABSolute" << "RELative";
    m_cmbType->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("窗口触发"));
    this->setLayout(&m_vlayoutFinal);
}

WindowTrigWid::~WindowTrigWid()
{
    delete m_labCLevel;
    delete m_txtCLevel;
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labDLevel;
    delete m_txtDLevel;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHLevel;
    delete m_txtHLevel;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labLLevel;
    delete m_txtLLevel;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labType;
    delete m_cmbType;
}



INTTrigWid::INTTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labLevel = new QLabel(QStringLiteral("LEVel"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("间隔触发"));
    this->setLayout(&m_vlayoutFinal);
}

INTTrigWid::~INTTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
}

DropOutTrigWid::DropOutTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labLevel = new QLabel(QStringLiteral("LEVel"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labTime = new QLabel(QStringLiteral("TIME"));
    m_txtTime = new QLineEdit(this);
    m_hlayoutTime.addWidget(m_labTime);
    m_hlayoutTime.addWidget(m_txtTime);
    m_vlayoutFinal.addLayout(&m_hlayoutTime);

    m_labType = new QLabel(QStringLiteral("TYPE"));
    m_cmbType = new QComboBox(this);
    strlist.clear();
    strlist << "EDGE" << "STATe";
    m_cmbType->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("超时触发"));
    this->setLayout(&m_vlayoutFinal);
}

DropOutTrigWid::~DropOutTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labTime;
    delete m_txtTime;
    delete m_labType;
    delete m_cmbType;
}

RuntTrigWid::RuntTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHLevel = new QLabel(QStringLiteral("HLEVel"));
    m_txtHLevel = new QLineEdit(this);
    m_hlayoutHLevel.addWidget(m_labHLevel);
    m_hlayoutHLevel.addWidget(m_txtHLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutHLevel);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labLLevel = new QLabel(QStringLiteral("LLEVel"));
    m_txtLLevel = new QLineEdit(this);
    m_hlayoutLLevel.addWidget(m_labLLevel);
    m_hlayoutLLevel.addWidget(m_txtLLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLLevel);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    m_labPol = new QLabel(QStringLiteral("POLarity"));
    m_cmbPol = new QComboBox(this);
    strlist.clear();
    strlist << "POSitive" << "NEGative";
    m_cmbPol->addItems(strlist);
    m_hlayoutPol.addWidget(m_labPol);
    m_hlayoutPol.addWidget(m_cmbPol);
    m_vlayoutFinal.addLayout(&m_hlayoutPol);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("欠幅触发"));
    this->setLayout(&m_vlayoutFinal);
}

RuntTrigWid::~RuntTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHLevel;
    delete m_txtHLevel;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labLLevel;
    delete m_txtLLevel;
    delete m_labNREJ;
    delete m_cmbNREJ;
    delete m_labPol;
    delete m_cmbPol;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
}

PatternTrigWid::PatternTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    QStringList strlist;
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labInput = new QLabel(QStringLiteral("INPut"));
    m_txtInput = new QLineEdit(this);
    m_hlayoutInput.addWidget(m_labInput);
    m_hlayoutInput.addWidget(m_txtInput);
    m_vlayoutFinal.addLayout(&m_hlayoutInput);

    m_labLevel = new QLabel(QStringLiteral("LEVel"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labLogic = new QLabel(QStringLiteral("Logic"));
    m_cmbLogic = new QComboBox(this);
    strlist.clear();
    strlist << "AND" << "OR" << "NAND" << "NOR";
    m_cmbLogic->addItems(strlist);
    m_hlayoutLogic.addWidget(m_labLogic);
    m_hlayoutLogic.addWidget(m_cmbLogic);
    m_vlayoutFinal.addLayout(&m_hlayoutLogic);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("码型触发"));
    this->setLayout(&m_vlayoutFinal);
}

PatternTrigWid::~PatternTrigWid()
{
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labInput;
    delete m_txtInput;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labLogic;
    delete m_cmbLogic;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
}

QUALTrigWid::QUALTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labELevel = new QLabel(QStringLiteral("ELEVel"));
    m_txtELevel = new QLineEdit(this);
    m_hlayoutELevel.addWidget(m_labELevel);
    m_hlayoutELevel.addWidget(m_txtELevel);
    m_vlayoutFinal.addLayout(&m_hlayoutELevel);

    m_labESlop = new QLabel(QStringLiteral("ESlop"));
    m_cmbESlop = new QComboBox(this);
    QStringList strlist;
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbESlop->addItems(strlist);
    m_hlayoutESlop.addWidget(m_labESlop);
    m_hlayoutESlop.addWidget(m_cmbESlop);
    m_vlayoutFinal.addLayout(&m_hlayoutESlop);

    m_labESource = new QLabel(QStringLiteral("ESource"));
    m_cmbESource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbESource->addItems(strlist);
    m_hlayoutESource.addWidget(m_labESource);
    m_hlayoutESource.addWidget(m_cmbESource);
    m_vlayoutFinal.addLayout(&m_hlayoutESource);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labQLevel = new QLabel(QStringLiteral("QLEVel"));
    m_txtQLevel = new QLineEdit(this);
    m_hlayoutQLevel.addWidget(m_labQLevel);
    m_hlayoutQLevel.addWidget(m_txtQLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutQLevel);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    m_labType = new QLabel(QStringLiteral("TYPE"));
    m_cmbType = new QComboBox(this);
    strlist.clear();
    strlist << "STATe" << "STATE_DLY" << "EDGE" << "EDGE_DLY";
    m_cmbType->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("QUALified触发"));
    this->setLayout(&m_vlayoutFinal);

}

QUALTrigWid::~QUALTrigWid()
{
    delete m_labELevel;
    delete m_txtELevel;
    delete m_labESlop;
    delete m_cmbESlop;
    delete m_labESource;
    delete m_cmbESource;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labQLevel;
    delete m_txtQLevel;
    delete m_labQSource;
    delete m_cmbQSource;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
    delete m_labType;
    delete m_cmbType;
}

DelayTrigWid::DelayTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labCoup = new QLabel(QStringLiteral("耦合"));
    m_cmbCoup = new QComboBox(this);
    QStringList strlist;
    strlist << "DC" << "AC" << "LFREJect" << "HFREJect";
    m_cmbCoup->addItems(strlist);
    m_hlayoutCoup.addWidget(m_labCoup);
    m_hlayoutCoup.addWidget(m_cmbCoup);
    m_vlayoutFinal.addLayout(&m_hlayoutCoup);

    m_labSource = new QLabel(QStringLiteral("Source"));
    m_txtSource = new QLineEdit(this);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_txtSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labSource2 = new QLabel(QStringLiteral("Source2"));
    m_cmbSource2 = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource2->addItems(strlist);
    m_hlayoutSource2.addWidget(m_labSource2);
    m_hlayoutSource2.addWidget(m_cmbSource2);
    m_vlayoutFinal.addLayout(&m_hlayoutSource2);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labSlop2 = new QLabel(QStringLiteral("Slop2"));
    m_cmbSlop2 = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbSlop2->addItems(strlist);
    m_hlayoutSlop2.addWidget(m_labSlop2);
    m_hlayoutSlop2.addWidget(m_cmbSlop2);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop2);

    m_labLevel = new QLabel(QStringLiteral("LEVel"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labLevel2 = new QLabel(QStringLiteral("LEVel2"));
    m_txtLevel2 = new QLineEdit(this);
    m_hlayoutLevel2.addWidget(m_labLevel2);
    m_hlayoutLevel2.addWidget(m_txtLevel2);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel2);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("延迟触发"));
    this->setLayout(&m_vlayoutFinal);
}

DelayTrigWid::~DelayTrigWid()
{
    delete m_labCoup;
    delete m_cmbCoup;
    delete m_labSource;
    delete m_txtSource;
    delete m_labSource2;
    delete m_cmbSource2;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labSlop2;
    delete m_cmbSlop2;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labLevel2;
    delete m_txtLevel2;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
}

NedgeTrigWid::NedgeTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labSource = new QLabel(QStringLiteral("Source"));
    m_cmbSource = new QComboBox(this);
    QStringList strlist;
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labIdle = new QLabel(QStringLiteral("Idle"));
    m_txtIdle = new QLineEdit(this);
    m_hlayoutIdle.addWidget(m_labIdle);
    m_hlayoutIdle.addWidget(m_txtIdle);
    m_vlayoutFinal.addLayout(&m_hlayoutIdle);

    m_labEdge = new QLabel(QStringLiteral("Edge"));
    m_txtEdge = new QLineEdit(this);
    m_hlayoutEdge.addWidget(m_labEdge);
    m_hlayoutEdge.addWidget(m_txtEdge);
    m_vlayoutFinal.addLayout(&m_hlayoutEdge);

    m_labLevel = new QLabel(QStringLiteral("LEVel"));
    m_txtLevel = new QLineEdit(this);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_txtLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labHldOff = new QLabel(QStringLiteral("HOLDoff"));
    m_cmbHldOff = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "EVENts" << "TIME";
    m_cmbHldOff->addItems(strlist);
    m_hlayoutHldOff.addWidget(m_labHldOff);
    m_hlayoutHldOff.addWidget(m_cmbHldOff);
    m_vlayoutFinal.addLayout(&m_hlayoutHldOff);

    m_labHldTime = new QLabel(QStringLiteral("HLDTime"));
    m_txtHldTime = new QLineEdit(this);
    m_hlayoutHldTime.addWidget(m_labHldTime);
    m_hlayoutHldTime.addWidget(m_txtHldTime);
    m_vlayoutFinal.addLayout(&m_hlayoutHldTime);

    m_labHldEvt = new QLabel(QStringLiteral("HLDEVent"));
    m_txtHldEvt = new QLineEdit(this);
    m_hlayoutHldEvt.addWidget(m_labHldEvt);
    m_hlayoutHldEvt.addWidget(m_txtHldEvt);
    m_vlayoutFinal.addLayout(&m_hlayoutHldEvt);

    m_labHstart = new QLabel(QStringLiteral("HSTart"));
    m_cmbHstart = new QComboBox(this);
    strlist.clear();
    strlist << "LAST_TRIG" << "ACQ_START";
    m_cmbHstart->addItems(strlist);
    m_hlayoutHstart.addWidget(m_labHstart);
    m_hlayoutHstart.addWidget(m_cmbHstart);
    m_vlayoutFinal.addLayout(&m_hlayoutHstart);

    m_labNREJ = new QLabel(QStringLiteral("NREJect"));
    m_cmbNREJ = new QComboBox(this);
    strlist.clear();
    strlist << "OFF" << "ON";
    m_cmbNREJ->addItems(strlist);
    m_hlayoutNREJ.addWidget(m_labNREJ);
    m_hlayoutNREJ.addWidget(m_cmbNREJ);
    m_vlayoutFinal.addLayout(&m_hlayoutNREJ);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("第N边沿触发"));
    this->setLayout(&m_vlayoutFinal);
}

NedgeTrigWid::~NedgeTrigWid()
{
    delete m_labSource;
    delete m_cmbSource;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labIdle;
    delete m_txtIdle;
    delete m_labEdge;
    delete m_txtEdge;
    delete m_labLevel;
    delete m_txtLevel;
    delete m_labHldOff;
    delete m_cmbHldOff;
    delete m_labHldTime;
    delete m_txtHldTime;
    delete m_labHldEvt;
    delete m_txtHldEvt;
    delete m_labHstart;
    delete m_cmbHstart;
    delete m_labNREJ;
    delete m_cmbNREJ;
}

SHoldTrigWid::SHoldTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labType = new QLabel(QStringLiteral("TYPE"));
    m_cmbType = new QComboBox(this);
    QStringList strlist;
    strlist.clear();
    strlist << "SETup" << "HOLD";
    m_cmbType->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    m_labCSource = new QLabel(QStringLiteral("CSource"));
    m_cmbCSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbCSource->addItems(strlist);
    m_hlayoutCSource.addWidget(m_labCSource);
    m_hlayoutCSource.addWidget(m_cmbCSource);
    m_vlayoutFinal.addLayout(&m_hlayoutCSource);

    m_labCThr = new QLabel(QStringLiteral("CTHReshold"));
    m_txtCThr = new QLineEdit(this);
    m_hlayoutCThr.addWidget(m_labCThr);
    m_hlayoutCThr.addWidget(m_txtCThr);
    m_vlayoutFinal.addLayout(&m_hlayoutCThr);

    m_labSlop = new QLabel(QStringLiteral("Slop"));
    m_cmbSlop = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbSlop->addItems(strlist);
    m_hlayoutSlop.addWidget(m_labSlop);
    m_hlayoutSlop.addWidget(m_cmbSlop);
    m_vlayoutFinal.addLayout(&m_hlayoutSlop);

    m_labDSource = new QLabel(QStringLiteral("DSource"));
    m_cmbDSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbDSource->addItems(strlist);
    m_hlayoutDSource.addWidget(m_labDSource);
    m_hlayoutDSource.addWidget(m_cmbDSource);
    m_vlayoutFinal.addLayout(&m_hlayoutDSource);

    m_labDThr = new QLabel(QStringLiteral("DTHReshold"));
    m_txtDThr = new QLineEdit(this);
    m_hlayoutDThr.addWidget(m_labDThr);
    m_hlayoutDThr.addWidget(m_txtDThr);
    m_vlayoutFinal.addLayout(&m_hlayoutDThr);

    m_labLevel = new QLabel(QStringLiteral("Level"));
    m_cmbLevel = new QComboBox(this);
    strlist.clear();
    strlist << "LOW" << "HIGH";
    m_cmbLevel->addItems(strlist);
    m_hlayoutLevel.addWidget(m_labLevel);
    m_hlayoutLevel.addWidget(m_cmbLevel);
    m_vlayoutFinal.addLayout(&m_hlayoutLevel);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "LESSthan" << "GREATerthan" << "INNer" << "OUTer";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labTlow = new QLabel(QStringLiteral("TLow"));
    m_txtTlow = new QLineEdit(this);
    m_hlayoutTlow.addWidget(m_labTlow);
    m_hlayoutTlow.addWidget(m_txtTlow);
    m_vlayoutFinal.addLayout(&m_hlayoutTlow);

    m_labTupp = new QLabel(QStringLiteral("TUpp"));
    m_txtTupp = new QLineEdit(this);
    m_hlayoutTupp.addWidget(m_labTupp);
    m_hlayoutTupp.addWidget(m_txtTupp);
    m_vlayoutFinal.addLayout(&m_hlayoutTupp);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("建立/保持触发"));
    this->setLayout(&m_vlayoutFinal);
}

SHoldTrigWid::~SHoldTrigWid()
{
    delete m_labType;
    delete m_cmbType;
    delete m_labCSource;
    delete m_cmbCSource;
    delete m_labCThr;
    delete m_txtCThr;
    delete m_labSlop;
    delete m_cmbSlop;
    delete m_labDSource;
    delete m_cmbDSource;
    delete m_labDThr;
    delete m_txtDThr;
    delete m_labLevel;
    delete m_cmbLevel;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labTlow;
    delete m_txtTlow;
    delete m_labTupp;
    delete m_txtTupp;
}

IICTrigWid::IICTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labAddr = new QLabel(QStringLiteral("ADDRess"));
    m_txtAddr = new QLineEdit(this);
    m_hlayoutAddr.addWidget(m_labAddr);
    m_hlayoutAddr.addWidget(m_txtAddr);
    m_vlayoutFinal.addLayout(&m_hlayoutAddr);

    m_labAlen = new QLabel(QStringLiteral("ALENgth"));
    m_cmbAlen = new QComboBox(this);
    QStringList strlist;
    strlist << "7BIT" << "10BIT";
    m_cmbAlen->addItems(strlist);
    m_hlayoutAlen.addWidget(m_labAlen);
    m_hlayoutAlen.addWidget(m_cmbAlen);
    m_vlayoutFinal.addLayout(&m_hlayoutAlen);

    m_labCond = new QLabel(QStringLiteral("CONDition"));
    m_cmbCond = new QComboBox(this);
    strlist.clear();
    strlist << "STARt" << "STOP" << "RESTart" << "NACK" << "EEPRom" << "7ADDRess" << "10ADDRess" << "DLENgth";
    m_cmbCond->addItems(strlist);
    m_hlayoutCond.addWidget(m_labCond);
    m_hlayoutCond.addWidget(m_cmbCond);
    m_vlayoutFinal.addLayout(&m_hlayoutCond);

    m_labData2 = new QLabel(QStringLiteral("DATA2"));
    m_txtData2 = new QLineEdit(this);
    m_hlayoutData2.addWidget(m_labData2);
    m_hlayoutData2.addWidget(m_txtData2);
    m_vlayoutFinal.addLayout(&m_hlayoutData2);

    m_labData = new QLabel(QStringLiteral("DATA"));
    m_txtData = new QLineEdit(this);
    m_hlayoutData.addWidget(m_labData);
    m_hlayoutData.addWidget(m_txtData);
    m_vlayoutFinal.addLayout(&m_hlayoutData);

    m_labDlen = new QLabel(QStringLiteral("DLENgth"));
    m_txtDlen = new QLineEdit(this);
    m_hlayoutDlen.addWidget(m_labDlen);
    m_hlayoutDlen.addWidget(m_txtDlen);
    m_vlayoutFinal.addLayout(&m_hlayoutDlen);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "EQUal" << "GREaterthan" << "LESSthan";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labRwbit = new QLabel(QStringLiteral("RWBit"));
    m_cmbRwbit = new QComboBox(this);
    strlist.clear();
    strlist << "WRITe" << "READ" << "ANY";
    m_cmbRwbit->addItems(strlist);
    m_hlayoutRwbit.addWidget(m_labRwbit);
    m_hlayoutRwbit.addWidget(m_cmbRwbit);
    m_vlayoutFinal.addLayout(&m_hlayoutRwbit);

    m_labSclSource = new QLabel(QStringLiteral("SCLSource"));
    m_cmbSclSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSclSource->addItems(strlist);
    m_hlayoutSclSource.addWidget(m_labSclSource);
    m_hlayoutSclSource.addWidget(m_cmbSclSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSclSource);

    m_labSclThr = new QLabel(QStringLiteral("SCLThreshold"));
    m_txtSclThr = new QLineEdit(this);
    m_hlayoutSclThr.addWidget(m_labSclThr);
    m_hlayoutSclThr.addWidget(m_txtSclThr);
    m_vlayoutFinal.addLayout(&m_hlayoutSclThr);

    m_labSdaSource = new QLabel(QStringLiteral("SDASource"));
    m_cmbSdaSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSdaSource->addItems(strlist);
    m_hlayoutSdaSource.addWidget(m_labSdaSource);
    m_hlayoutSdaSource.addWidget(m_cmbSdaSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSdaSource);

    m_labSdaThr = new QLabel(QStringLiteral("SDAThreshold"));
    m_txtSdaThr = new QLineEdit(this);
    m_hlayoutSdaThr.addWidget(m_labSdaThr);
    m_hlayoutSdaThr.addWidget(m_txtSdaThr);
    m_vlayoutFinal.addLayout(&m_hlayoutSdaThr);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("IIC总线触发"));
    this->setLayout(&m_vlayoutFinal);
}

IICTrigWid::~IICTrigWid()
{
    delete m_labAddr;
    delete m_txtAddr;
    delete m_labAlen;
    delete m_cmbAlen;
    delete m_labCond;
    delete m_cmbCond;
    delete m_labData2;
    delete m_txtData2;
    delete m_labData;
    delete m_txtData;
    delete m_labDlen;
    delete m_txtDlen;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labRwbit;
    delete m_cmbRwbit;
    delete m_labSclSource;
    delete m_cmbSclSource;
    delete m_labSclThr;
    delete m_txtSclThr;
    delete m_labSdaSource;
    delete m_cmbSdaSource;
    delete m_labSdaThr;
    delete m_txtSdaThr;
}

SPITrigWid::SPITrigWid(QWidget *parent) : QDialog(parent)
{
    m_labBitOrder = new QLabel(QStringLiteral("BITorder"));
    m_cmbBitOrder = new QComboBox(this);
    QStringList strlist;
    strlist << "LSB" << "MSB";
    m_cmbBitOrder->addItems(strlist);
    m_hlayoutBitOrder.addWidget(m_labBitOrder);
    m_hlayoutBitOrder.addWidget(m_cmbBitOrder);
    m_vlayoutFinal.addLayout(&m_hlayoutBitOrder);

    m_labClkSource = new QLabel(QStringLiteral("CLKSource"));
    m_cmbClkSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbClkSource->addItems(strlist);
    m_hlayoutClkSource.addWidget(m_labClkSource);
    m_hlayoutClkSource.addWidget(m_cmbClkSource);
    m_vlayoutFinal.addLayout(&m_hlayoutClkSource);

    m_labClkThr = new QLabel(QStringLiteral("CLKThreshold"));
    m_txtClkThr = new QLineEdit(this);
    m_hlayoutClkThr.addWidget(m_labClkThr);
    m_hlayoutClkThr.addWidget(m_txtClkThr);
    m_vlayoutFinal.addLayout(&m_hlayoutClkThr);

    m_labCSSource = new QLabel(QStringLiteral("CSSource"));
    m_cmbCSSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_hlayoutCSSource.addWidget(m_labCSSource);
    m_hlayoutCSSource.addWidget(m_cmbCSSource);
    m_vlayoutFinal.addLayout(&m_hlayoutCSSource);

    m_labCSThr = new QLabel(QStringLiteral("CSThreshold"));
    m_txtCSThr = new QLineEdit(this);
    m_hlayoutCSThr.addWidget(m_labCSThr);
    m_hlayoutCSThr.addWidget(m_txtCSThr);
    m_vlayoutFinal.addLayout(&m_hlayoutCSThr);

    m_labCSType = new QLabel(QStringLiteral("CSTYpe"));
    m_cmbCSType = new QComboBox(this);
    strlist.clear();
    strlist << "NCS" << "CS" << "TIMeout";
    m_cmbCSType->addItems(strlist);
    m_hlayoutCSType.addWidget(m_labCSType);
    m_hlayoutCSType.addWidget(m_cmbCSType);
    m_vlayoutFinal.addLayout(&m_hlayoutCSType);

    m_labData = new QLabel(QStringLiteral("DATA"));
    m_txtData = new QLineEdit(this);
    m_hlayoutData.addWidget(m_labData);
    m_hlayoutData.addWidget(m_txtData);
    m_vlayoutFinal.addLayout(&m_hlayoutData);

    m_labDlen = new QLabel(QStringLiteral("DLENgth"));
    m_txtDlen = new QLineEdit(this);
    m_hlayoutDlen.addWidget(m_labDlen);
    m_hlayoutDlen.addWidget(m_txtDlen);
    m_vlayoutFinal.addLayout(&m_hlayoutDlen);

    m_labLATC = new QLabel(QStringLiteral("LATChedge"));
    m_cmbLATC = new QComboBox(this);
    strlist.clear();
    strlist << "RISing" << "FALLing";
    m_cmbLATC->addItems(strlist);
    m_hlayoutLATC.addWidget(m_labLATC);
    m_hlayoutLATC.addWidget(m_cmbLATC);
    m_vlayoutFinal.addLayout(&m_hlayoutLATC);

    m_labMISOSource = new QLabel(QStringLiteral("MISOSource"));
    m_cmbMISOSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbMISOSource->addItems(strlist);
    m_hlayoutMISOSource.addWidget(m_labMISOSource);
    m_hlayoutMISOSource.addWidget(m_cmbMISOSource);
    m_vlayoutFinal.addLayout(&m_hlayoutMISOSource);

    m_labMISOThr = new QLabel(QStringLiteral("MISOThreshold"));
    m_txtMISOThr = new QLineEdit(this);
    m_hlayoutMISOThr.addWidget(m_labMISOThr);
    m_hlayoutMISOThr.addWidget(m_txtMISOThr);
    m_vlayoutFinal.addLayout(&m_hlayoutMISOThr);

    m_labMOSISource = new QLabel(QStringLiteral("MOSISource"));
    m_cmbMOSISource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbMOSISource->addItems(strlist);
    m_hlayoutMOSISource.addWidget(m_labMOSISource);
    m_hlayoutMOSISource.addWidget(m_cmbMOSISource);
    m_vlayoutFinal.addLayout(&m_hlayoutMOSISource);

    m_labMOSIThr = new QLabel(QStringLiteral("MOSIThreshold"));
    m_txtMOSIThr = new QLineEdit(this);
    m_hlayoutMOSIThr.addWidget(m_labMOSIThr);
    m_hlayoutMOSIThr.addWidget(m_txtMOSIThr);
    m_vlayoutFinal.addLayout(&m_hlayoutMOSIThr);

    m_labNCSSource = new QLabel(QStringLiteral("NCSSource"));
    m_cmbNCSSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbNCSSource->addItems(strlist);
    m_hlayoutNCSSource.addWidget(m_labNCSSource);
    m_hlayoutNCSSource.addWidget(m_cmbNCSSource);
    m_vlayoutFinal.addLayout(&m_hlayoutNCSSource);

    m_labNCSThr = new QLabel(QStringLiteral("NCSThreshold"));
    m_txtNCSThr = new QLineEdit(this);
    m_hlayoutNCSThr.addWidget(m_labNCSThr);
    m_hlayoutNCSThr.addWidget(m_txtNCSThr);
    m_vlayoutFinal.addLayout(&m_hlayoutNCSThr);

    m_labTType = new QLabel(QStringLiteral("TTYPe"));
    m_cmbTType = new QComboBox(this);
    strlist.clear();
    strlist << "MISO" << "MOSI";
    m_cmbTType->addItems(strlist);
    m_hlayoutTType.addWidget(m_labTType);
    m_hlayoutTType.addWidget(m_cmbTType);
    m_vlayoutFinal.addLayout(&m_hlayoutTType);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("SPI总线触发"));
    this->setLayout(&m_vlayoutFinal);
}

SPITrigWid::~SPITrigWid()
{
    delete m_labBitOrder;
    delete m_cmbBitOrder;
    delete m_labClkSource;
    delete m_cmbClkSource;
    delete m_labClkThr;
    delete m_txtClkThr;
    delete m_labCSSource;
    delete m_cmbCSSource;
    delete m_labCSThr;
    delete m_txtCSThr;
    delete m_labCSType;
    delete m_cmbCSType;
    delete m_labData;
    delete m_txtData;
    delete m_labDlen;
    delete m_txtDlen;
    delete m_labLATC;
    delete m_cmbLATC;
    delete m_labMISOSource;
    delete m_cmbMISOSource;
    delete m_labMISOThr;
    delete m_txtMISOThr;
    delete m_labMOSISource;
    delete m_cmbMOSISource;
    delete m_labMOSIThr;
    delete m_txtMOSIThr;
    delete m_labNCSSource;
    delete m_cmbNCSSource;
    delete m_labNCSThr;
    delete m_txtNCSThr;
    delete m_labTType;
    delete m_cmbTType;
}

UARTTrigWid::UARTTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labBAUD = new QLabel(QStringLiteral("波特率"));
    m_cmbBAUD = new QComboBox(this);
    QStringList strlist;
    strlist << "600bps" << "1200bps" << "2400bps" << "4800bps" << "9600bps" << "19200bps" << "38400bps"
            << "57600bps" << "115200bps";
    m_cmbBAUD->addItems(strlist);
    m_hlayoutBAUD.addWidget(m_labBAUD);
    m_hlayoutBAUD.addWidget(m_cmbBAUD);
    m_vlayoutFinal.addLayout(&m_hlayoutBAUD);

    m_labBitOrder = new QLabel(QStringLiteral("BITorder"));
    m_cmbBitOrder = new QComboBox(this);
    strlist.clear();
    strlist << "LSB" << "MSB";
    m_hlayoutBitOrder.addWidget(m_labBitOrder);
    m_hlayoutBitOrder.addWidget(m_cmbBitOrder);
    m_vlayoutFinal.addLayout(&m_hlayoutBitOrder);

    m_labCond = new QLabel(QStringLiteral("CONDition"));
    m_cmbCond = new QComboBox(this);
    strlist.clear();
    strlist << "STARt" << "STOP" << "DATA" << "ERRor";
    m_cmbCond->addItems(strlist);
    m_hlayoutCond.addWidget(m_labCond);
    m_hlayoutCond.addWidget(m_cmbCond);
    m_vlayoutFinal.addLayout(&m_hlayoutCond);

    m_labData = new QLabel(QStringLiteral("DATA"));
    m_txtData = new QLineEdit(this);
    m_hlayoutData.addWidget(m_labData);
    m_hlayoutData.addWidget(m_txtData);
    m_vlayoutFinal.addLayout(&m_hlayoutData);

    m_labDlen = new QLabel(QStringLiteral("DLENgth"));
    m_txtDlen = new QLineEdit(this);
    m_hlayoutDlen.addWidget(m_labDlen);
    m_hlayoutDlen.addWidget(m_txtDlen);
    m_vlayoutFinal.addLayout(&m_hlayoutDlen);

    m_labIDLE = new QLabel(QStringLiteral("IDLE"));
    m_cmbIDLE = new QComboBox(this);
    strlist.clear();
    strlist << "LOW" << "HIGH";
    m_cmbIDLE->addItems(strlist);
    m_hlayoutIDLE.addWidget(m_labIDLE);
    m_hlayoutIDLE.addWidget(m_cmbIDLE);
    m_vlayoutFinal.addLayout(&m_hlayoutIDLE);

    m_labLimit = new QLabel(QStringLiteral("Limit"));
    m_cmbLimit = new QComboBox(this);
    strlist.clear();
    strlist << "EQUal" << "GREaterthan" << "LESSthan";
    m_cmbLimit->addItems(strlist);
    m_hlayoutLimit.addWidget(m_labLimit);
    m_hlayoutLimit.addWidget(m_cmbLimit);
    m_vlayoutFinal.addLayout(&m_hlayoutLimit);

    m_labParity = new QLabel(QStringLiteral("PARity"));
    m_cmbParity = new QComboBox(this);
    strlist.clear();
    strlist << "NONE" << "ODD" << "EVEN" << "MARK" << "SPACe";
    m_cmbParity->addItems(strlist);
    m_hlayoutParity.addWidget(m_labParity);
    m_hlayoutParity.addWidget(m_cmbParity);
    m_vlayoutFinal.addLayout(&m_hlayoutParity);

    m_labRXSource = new QLabel(QStringLiteral("RXSource"));
    m_cmbRXSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbRXSource->addItems(strlist);
    m_hlayoutRXSource.addWidget(m_labRXSource);
    m_hlayoutRXSource.addWidget(m_cmbRXSource);
    m_vlayoutFinal.addLayout(&m_hlayoutRXSource);

    m_labRXThr = new QLabel(QStringLiteral("RXThreshold"));
    m_txtRXThr = new QLineEdit(this);
    m_hlayoutRXThr.addWidget(m_labRXThr);
    m_hlayoutRXThr.addWidget(m_txtRXThr);
    m_vlayoutFinal.addLayout(&m_hlayoutRXThr);

    m_labStop = new QLabel(QStringLiteral("STOP"));
    m_cmbStop = new QComboBox(this);
    strlist.clear();
    strlist << "1" << "1.5" << "2";
    m_cmbStop->addItems(strlist);
    m_hlayoutStop.addWidget(m_labStop);
    m_hlayoutStop.addWidget(m_cmbStop);
    m_vlayoutFinal.addLayout(&m_hlayoutStop);

    m_labTType = new QLabel(QStringLiteral("TTYPe"));
    m_cmbTTYpe = new QComboBox(this);
    strlist.clear();
    strlist << "RX" << "TXT";
    m_cmbTTYpe->addItems(strlist);
    m_hlayoutTType.addWidget(m_labTType);
    m_hlayoutTType.addWidget(m_cmbTTYpe);
    m_vlayoutFinal.addLayout(&m_hlayoutTType);

    m_labTXSource = new QLabel(QStringLiteral("TXSource"));
    m_cmbTXSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbTXSource->addItems(strlist);
    m_hlayoutTXSource.addWidget(m_labTXSource);
    m_hlayoutTXSource.addWidget(m_cmbTXSource);
    m_vlayoutFinal.addLayout(&m_hlayoutTXSource);

    m_labTXThr = new QLabel(QStringLiteral("TXThreshold"));
    m_txtTXThr = new QLineEdit(this);
    m_hlayoutTXThr.addWidget(m_labTXThr);
    m_hlayoutTXThr.addWidget(m_txtTXThr);
    m_vlayoutFinal.addLayout(&m_hlayoutTXThr);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("UART总线触发"));
    this->setLayout(&m_vlayoutFinal);
}

UARTTrigWid::~UARTTrigWid()
{
    delete m_labBAUD;
    delete m_cmbBAUD;
    delete m_labBitOrder;
    delete m_cmbBitOrder;
    delete m_labCond;
    delete m_cmbCond;
    delete m_labData;
    delete m_txtData;
    delete m_labDlen;
    delete m_txtDlen;
    delete m_labIDLE;
    delete m_cmbIDLE;
    delete m_labLimit;
    delete m_cmbLimit;
    delete m_labParity;
    delete m_cmbParity;
    delete m_labRXSource;
    delete m_cmbRXSource;
    delete m_labRXThr;
    delete m_txtRXThr;
    delete m_labStop;
    delete m_cmbStop;
    delete m_labTType;
    delete m_cmbTTYpe;
    delete m_labTXSource;
    delete m_cmbTXSource;
    delete m_labTXThr;
    delete m_txtTXThr;
}

CANTrigWid::CANTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labBAUD = new QLabel(QStringLiteral("波特率"));
    m_cmbBAUD = new QComboBox(this);
    QStringList strlist;
    strlist << "5kbps" << "10kbps" << "50kbps" << "100kbps" << "125kbps" << "250kbps" << "500kbps"
            << "800kbps" << "1Mbps";
    m_cmbBAUD->addItems(strlist);
    m_hlayoutBAUD.addWidget(m_labBAUD);
    m_hlayoutBAUD.addWidget(m_cmbBAUD);
    m_vlayoutFinal.addLayout(&m_hlayoutBAUD);

    m_labCond = new QLabel(QStringLiteral("CONDition"));
    m_cmbCond = new QComboBox(this);
    strlist.clear();
    strlist << "STARt" << "REMote" << "ID" << "ID_AND_DATA" <<"ERRor";
    m_cmbCond->addItems(strlist);
    m_hlayoutCond.addWidget(m_labCond);
    m_hlayoutCond.addWidget(m_cmbCond);
    m_vlayoutFinal.addLayout(&m_hlayoutCond);

    m_labData2 = new QLabel(QStringLiteral("DATA2"));
    m_txtData2 = new QLineEdit(this);
    m_hlayoutData2.addWidget(m_labData2);
    m_hlayoutData2.addWidget(m_txtData2);
    m_vlayoutFinal.addLayout(&m_hlayoutData2);

    m_labData = new QLabel(QStringLiteral("DATA"));
    m_txtData = new QLineEdit(this);
    m_hlayoutData.addWidget(m_labData);
    m_hlayoutData.addWidget(m_txtData);
    m_vlayoutFinal.addLayout(&m_hlayoutData);

    m_labID = new QLabel(QStringLiteral("ID"));
    m_txtID = new QLineEdit(this);
    m_hlayoutID.addWidget(m_labID);
    m_hlayoutID.addWidget(m_txtID);
    m_vlayoutFinal.addLayout(&m_hlayoutID);

    m_labIDLen = new QLabel(QStringLiteral("IDLength"));
    m_cmbIDLen = new QComboBox(this);
    strlist.clear();
    strlist << "11BITS" << "29BITS";
    m_cmbIDLen->addItems(strlist);
    m_hlayoutIDLen.addWidget(m_labIDLen);
    m_hlayoutIDLen.addWidget(m_cmbIDLen);
    m_vlayoutFinal.addLayout(&m_hlayoutIDLen);

    m_labSource = new QLabel(QStringLiteral("SOURce"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labThr = new QLabel(QStringLiteral("THReshold"));
    m_txtThr = new QLineEdit(this);
    m_hlayoutThr.addWidget(m_labThr);
    m_hlayoutThr.addWidget(m_txtThr);
    m_vlayoutFinal.addLayout(&m_hlayoutThr);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("CAN总线触发"));
    this->setLayout(&m_vlayoutFinal);
}

CANTrigWid::~CANTrigWid()
{
    delete m_labBAUD;
    delete m_cmbBAUD;
    delete m_labCond;
    delete m_cmbCond;
    delete m_labData2;
    delete m_txtData2;
    delete m_labData;
    delete m_txtData;
    delete m_labID;
    delete m_txtID;
    delete m_labIDLen;
    delete m_cmbIDLen;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labThr;
    delete m_txtThr;
}

LINTrigWid::LINTrigWid(QWidget *parent) : QDialog(parent)
{
    m_labBAUD = new QLabel(QStringLiteral("波特率"));
    m_cmbBAUD = new QComboBox(this);
    QStringList strlist;
    strlist << "600bps" << "1200bps" << "2400bps" << "4800bps" << "9600bps" << "19200bps" << "38400bps";
    m_cmbBAUD->addItems(strlist);
    m_hlayoutBAUD.addWidget(m_labBAUD);
    m_hlayoutBAUD.addWidget(m_cmbBAUD);
    m_vlayoutFinal.addLayout(&m_hlayoutBAUD);

    m_labCond = new QLabel(QStringLiteral("CONDition"));
    m_cmbCond = new QComboBox(this);
    strlist.clear();
    strlist << "BReak" << "ID" << "ID_AND_DATA" << "DATA_ERROR";
    m_cmbCond->addItems(strlist);
    m_hlayoutCond.addWidget(m_labCond);
    m_hlayoutCond.addWidget(m_cmbCond);
    m_vlayoutFinal.addLayout(&m_hlayoutCond);

    m_labData2 = new QLabel(QStringLiteral("DATA2"));
    m_txtData2 = new QLineEdit(this);
    m_hlayoutData2.addWidget(m_labData2);
    m_hlayoutData2.addWidget(m_txtData2);
    m_vlayoutFinal.addLayout(&m_hlayoutData2);

    m_labData = new QLabel(QStringLiteral("DATA"));
    m_txtData = new QLineEdit(this);
    m_hlayoutData.addWidget(m_labData);
    m_hlayoutData.addWidget(m_txtData);
    m_vlayoutFinal.addLayout(&m_hlayoutData);

    m_labErrCheckSum = new QLabel(QStringLiteral("ERRor:CHECksum"));
    m_cmbErrCheckSum = new QComboBox(this);
    strlist.clear();
    strlist << "0" << "1";
    m_cmbErrCheckSum->addItems(strlist);
    m_hlayoutErrCheckSum.addWidget(m_labErrCheckSum);
    m_hlayoutErrCheckSum.addWidget(m_cmbErrCheckSum);
    m_vlayoutFinal.addLayout(&m_hlayoutErrCheckSum);

    m_labErrDlen = new QLabel(QStringLiteral("ERRor:DLENgth"));
    m_txtErrDlen = new QLineEdit(this);
    m_hlayoutErrDlen.addWidget(m_labErrDlen);
    m_hlayoutErrDlen.addWidget(m_txtErrDlen);
    m_vlayoutFinal.addLayout(&m_hlayoutErrDlen);

    m_labErrID = new QLabel(QStringLiteral("ERRor:ID"));
    m_txtErrID = new QLineEdit(this);
    m_hlayoutErrID.addWidget(m_labErrID);
    m_hlayoutErrID.addWidget(m_txtErrID);
    m_vlayoutFinal.addLayout(&m_hlayoutErrID);

    m_labErrParity = new QLabel(QStringLiteral("ERRor:PARity"));
    m_cmbErrParity = new QComboBox(this);
    strlist.clear();
    strlist << "0" << "1";
    m_cmbErrParity->addItems(strlist);
    m_hlayoutErrParity.addWidget(m_labErrParity);
    m_hlayoutErrParity.addWidget(m_cmbErrParity);
    m_vlayoutFinal.addLayout(&m_hlayoutErrParity);

    m_labErrSYNC = new QLabel(QStringLiteral("ERRor:SYNC"));
    m_cmbErrSYNC = new QComboBox(this);
    strlist.clear();
    strlist << "0" << "1";
    m_cmbErrSYNC->addItems(strlist);
    m_hlayoutErrSYNC.addWidget(m_labErrSYNC);
    m_hlayoutErrSYNC.addWidget(m_cmbErrSYNC);
    m_vlayoutFinal.addLayout(&m_hlayoutErrSYNC);

    m_labID = new QLabel(QStringLiteral("ID"));
    m_txtID = new QLineEdit(this);
    m_hlayoutID.addWidget(m_labID);
    m_hlayoutID.addWidget(m_txtID);
    m_vlayoutFinal.addLayout(&m_hlayoutID);

    m_labSource = new QLabel(QStringLiteral("SOURce"));
    m_cmbSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource->addItems(strlist);
    m_hlayoutSource.addWidget(m_labSource);
    m_hlayoutSource.addWidget(m_cmbSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSource);

    m_labSTAN = new QLabel(QStringLiteral("STANdard"));
    m_cmbSTAN = new QComboBox(this);
    strlist.clear();
    strlist << "0" << "1";
    m_cmbSTAN->addItems(strlist);
    m_hlayoutSTAN.addWidget(m_labSTAN);
    m_hlayoutSTAN.addWidget(m_cmbSTAN);
    m_vlayoutFinal.addLayout(&m_hlayoutSTAN);

    m_labThr = new QLabel(QStringLiteral("THReshold"));
    m_txtThr = new QLineEdit(this);
    m_hlayoutThr.addWidget(m_labThr);
    m_hlayoutThr.addWidget(m_txtThr);
    m_vlayoutFinal.addLayout(&m_hlayoutThr);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("LIN总线触发"));
    this->setLayout(&m_vlayoutFinal);
}

LINTrigWid::~LINTrigWid()
{
    delete m_labBAUD;
    delete m_cmbBAUD;
    delete m_labCond;
    delete m_cmbCond;
    delete m_labData2;
    delete m_txtData2;
    delete m_labData;
    delete m_txtData;
    delete m_labErrCheckSum;
    delete m_cmbErrCheckSum;
    delete m_labErrDlen;
    delete m_txtErrDlen;
    delete m_labErrID;
    delete m_txtErrID;
    delete m_labErrParity;
    delete m_cmbErrParity;
    delete m_labErrSYNC;
    delete m_cmbErrSYNC;
    delete m_labID;
    delete m_txtID;
    delete m_labSource;
    delete m_cmbSource;
    delete m_labSTAN;
    delete m_cmbSTAN;
    delete m_labThr;
    delete m_txtThr;
}

CfgCursorWid::CfgCursorWid(QWidget *parent) : QDialog(parent)
{
    m_labEnable = new QLabel(QStringLiteral("打开/关闭"));
    m_btnEnable = new QRadioButton(this);
    m_btnEnable->setChecked(false);
    m_hlayoutEnable.addWidget(m_labEnable);
    m_hlayoutEnable.addWidget(m_btnEnable);
    m_vlayoutFinal.addLayout(&m_hlayoutEnable);

    m_labTagStyle = new QLabel(QStringLiteral("光标样式"));
    m_cmbTagStyle = new QComboBox(this);
    QStringList strlist;
    strlist << "FIXed" << "FOLLowing";
    m_cmbTagStyle->addItems(strlist);
    m_hlayoutTagStyle.addWidget(m_labTagStyle);
    m_hlayoutTagStyle.addWidget(m_cmbTagStyle);
    m_vlayoutFinal.addLayout(&m_hlayoutTagStyle);

    m_labIXD = new QLabel(QStringLiteral("光标值"));
    m_txtIXD = new QLineEdit(this);
    m_hlayoutIXD.addWidget(m_labIXD);
    m_hlayoutIXD.addWidget(m_txtIXD);
    m_vlayoutFinal.addLayout(&m_hlayoutIXD);

    m_labMitem = new QLabel(QStringLiteral("MITem"));
    m_cmbMitem = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2";
    m_cmbMitem->addItems(strlist);
    m_hlayoutMitem.addWidget(m_labMitem);
    m_hlayoutMitem.addWidget(m_cmbMitem);
    m_vlayoutFinal.addLayout(&m_hlayoutMitem);

    m_labMode = new QLabel(QStringLiteral("光标模式"));
    m_cmbMode = new QComboBox(this);
    strlist.clear();
    strlist << "TRACk" << "MANua";
    m_cmbMode->addItems(strlist);
    m_hlayoutMode.addWidget(m_labMode);
    m_hlayoutMode.addWidget(m_cmbMode);
    m_vlayoutFinal.addLayout(&m_hlayoutMode);

    m_labType = new QLabel(QStringLiteral("光标类型"));
    m_cmbType = new QComboBox(this);
    strlist.clear();
    strlist << "X" << "Y" <<"XY";
    m_cmbType->addItems(strlist);
    m_hlayoutType.addWidget(m_labType);
    m_hlayoutType.addWidget(m_cmbType);
    m_vlayoutFinal.addLayout(&m_hlayoutType);

    m_labSource1 = new QLabel(QStringLiteral("信号源1"));
    m_cmbSource1 = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource1->addItems(strlist);
    m_hlayoutSource1.addWidget(m_labSource1);
    m_hlayoutSource1.addWidget(m_cmbSource1);
    m_vlayoutFinal.addLayout(&m_hlayoutSource1);

    m_labSource2 = new QLabel(QStringLiteral("信号源2"));
    m_cmbSource2 = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4";
    m_cmbSource2->addItems(strlist);
    m_hlayoutSource2.addWidget(m_labSource2);
    m_hlayoutSource2.addWidget(m_cmbSource2);
    m_vlayoutFinal.addLayout(&m_hlayoutSource2);

    m_labX1 = new QLabel(QStringLiteral("X1"));
    m_txtX1 = new QLineEdit(this);
    m_hlayoutX1.addWidget(m_labX1);
    m_hlayoutX1.addWidget(m_txtX1);
    m_vlayoutFinal.addLayout(&m_hlayoutX1);

    m_labX2 = new QLabel(QStringLiteral("X2"));
    m_txtX2 = new QLineEdit(this);
    m_hlayoutX2.addWidget(m_labX2);
    m_hlayoutX2.addWidget(m_txtX2);
    m_vlayoutFinal.addLayout(&m_hlayoutX2);

    m_labXDEL = new QLabel(QStringLiteral("XDELta"));
    m_txtXDEL = new QLineEdit(this);
    m_hlayoutXDEL.addWidget(m_labXDEL);
    m_hlayoutXDEL.addWidget(m_txtXDEL);
    m_vlayoutFinal.addLayout(&m_hlayoutXDEL);

    m_labXREF = new QLabel(QStringLiteral("XREFerence"));
    m_cmbXREF = new QComboBox(this);
    strlist.clear();
    strlist << "DELay" << "POSition";
    m_cmbXREF->addItems(strlist);
    m_hlayoutXREF.addWidget(m_labXREF);
    m_hlayoutXREF.addWidget(m_cmbXREF);
    m_vlayoutFinal.addLayout(&m_hlayoutXREF);

    m_labY1 = new QLabel(QStringLiteral("Y1"));
    m_txtY1 = new QLineEdit(this);
    m_hlayoutY1.addWidget(m_labY1);
    m_hlayoutY1.addWidget(m_txtY1);
    m_vlayoutFinal.addLayout(&m_hlayoutY1);

    m_labY2 = new QLabel(QStringLiteral("Y2"));
    m_txtY2 = new QLineEdit(this);
    m_hlayoutY2.addWidget(m_labY2);
    m_hlayoutY2.addWidget(m_txtY2);
    m_vlayoutFinal.addLayout(&m_hlayoutY2);

    m_labYDEL = new QLabel(QStringLiteral("YDELta"));
    m_txtYDEL = new QLineEdit(this);
    m_hlayoutYDEL.addWidget(m_labYDEL);
    m_hlayoutYDEL.addWidget(m_txtYDEL);
    m_vlayoutFinal.addLayout(&m_hlayoutYDEL);

    m_labYREF = new QLabel(QStringLiteral("YREFerence"));
    m_cmbYREF = new QComboBox(this);
    strlist.clear();
    strlist << "DELay" << "POSition";
    m_cmbYREF->addItems(strlist);
    m_hlayoutYREF.addWidget(m_labYREF);
    m_hlayoutYREF.addWidget(m_cmbYREF);
    m_vlayoutFinal.addLayout(&m_hlayoutYREF);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgCursorWid::~CfgCursorWid()
{
    delete m_labEnable;
    delete m_btnEnable;
    delete m_labTagStyle;
    delete m_cmbTagStyle;
    delete m_labIXD;
    delete m_txtIXD;
    delete m_labMitem;
    delete m_cmbMitem;
    delete m_labMode;
    delete m_cmbMode;
    delete m_labType;
    delete m_cmbType;
    delete m_labSource1;
    delete m_cmbSource1;
    delete m_labSource2;
    delete m_cmbSource2;
    delete m_labX1;
    delete m_txtX1;
    delete m_labX2;
    delete m_txtX2;
    delete m_labXDEL;
    delete m_txtXDEL;
    delete m_labXREF;
    delete m_cmbXREF;
    delete m_labY1;
    delete m_txtY1;
    delete m_labY2;
    delete m_txtY2;
    delete m_labYDEL;
    delete m_txtYDEL;
    delete m_labYREF;
    delete m_cmbYREF;
}

CfgMeasureThresholdWid::CfgMeasureThresholdWid(QWidget *parent) : QDialog(parent)
{
    m_labThrABSHigh = new QLabel(QStringLiteral("ABSolute:High"));
    m_txtThrABSHigh = new QLineEdit(this);
    m_hlayoutThrABSHigh.addWidget(m_labThrABSHigh);
    m_hlayoutThrABSHigh.addWidget(m_txtThrABSHigh);
    m_vlayoutFinal.addLayout(&m_hlayoutThrABSHigh);

    m_labThrABSMid = new QLabel(QStringLiteral("ABSolute:Mid"));
    m_txtThrABSMid = new QLineEdit(this);
    m_hlayoutThrABSMid.addWidget(m_labThrABSMid);
    m_hlayoutThrABSMid.addWidget(m_txtThrABSMid);
    m_vlayoutFinal.addLayout(&m_hlayoutThrABSMid);

    m_labThrABSLow = new QLabel(QStringLiteral("ABSolute:Low"));
    m_txtThrABSLow = new QLineEdit(this);
    m_hlayoutThrABSLow.addWidget(m_labThrABSLow);
    m_hlayoutThrABSLow.addWidget(m_txtThrABSLow);
    m_vlayoutFinal.addLayout(&m_hlayoutThrABSLow);

    m_labThrPerHigh = new QLabel(QStringLiteral("PERCent:High"));
    m_txtThrPerHigh = new QLineEdit(this);
    m_hlayoutThrPerHigh.addWidget(m_labThrPerHigh);
    m_hlayoutThrPerHigh.addWidget(m_txtThrPerHigh);
    m_vlayoutFinal.addLayout(&m_hlayoutThrPerHigh);

    m_labThrPerMid = new QLabel(QStringLiteral("PERCent:Mid"));
    m_txtThrPerMid = new QLineEdit(this);
    m_hlayoutThrPerMid.addWidget(m_labThrPerMid);
    m_hlayoutThrPerMid.addWidget(m_txtThrPerMid);
    m_vlayoutFinal.addLayout(&m_hlayoutThrPerMid);

    m_labThrPerLow = new QLabel(QStringLiteral("PERCent:Low"));
    m_txtThrPerLow = new QLineEdit(this);
    m_hlayoutThrPerLow.addWidget(m_labThrPerLow);
    m_hlayoutThrPerLow.addWidget(m_txtThrPerLow);
    m_vlayoutFinal.addLayout(&m_hlayoutThrPerLow);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("MeasureThresholdWid"));
    this->setLayout(&m_vlayoutFinal);
}

CfgMeasureThresholdWid::~CfgMeasureThresholdWid()
{
    delete m_labThrABSHigh;
    delete m_txtThrABSHigh;
    delete m_labThrABSMid;
    delete m_txtThrABSMid;
    delete m_labThrABSLow;
    delete m_txtThrABSLow;;
    delete m_labThrPerHigh;
    delete m_txtThrPerHigh;
    delete m_labThrPerMid;
    delete m_txtThrPerMid;
    delete m_labThrPerLow;
    delete m_txtThrPerLow;
}

CfgMeasureAdvancedWid::CfgMeasureAdvancedWid(QWidget *parent) : QDialog(parent)
{
    m_labAdvNumber = new QLabel(QStringLiteral("LINenumber"));
    m_txtAdvNumber = new QLineEdit(this);
    m_hlayoutAdvNumber.addWidget(m_labAdvNumber);
    m_hlayoutAdvNumber.addWidget(m_txtAdvNumber);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvNumber);

    m_labAdvP = new QLabel(QStringLiteral("P<n>"));
    m_cmbAdvP = new QComboBox(this);
    QStringList strlistP, strlistPrm, strlist;
    for(int i = 1; i <= 12; ++i)
        strlistP << QString("P%1").arg(i);
    m_cmbAdvP->addItems(strlistP);
    m_btnAdvPOn = new QRadioButton(QStringLiteral("ON/OFF"), this);
    m_btnAdvPOn->setAutoExclusive(false);
    m_btnAdvPOn->setChecked(true);
    m_btnAdvPSet = new QPushButton(this);
    m_btnAdvPSet->setText(QStringLiteral("set"));
    m_hlayoutAdvP.addWidget(m_labAdvP);
    m_hlayoutAdvP.addWidget(m_cmbAdvP);
    m_hlayoutAdvP.addWidget(m_btnAdvPOn);
    m_hlayoutAdvP.addWidget(m_btnAdvPSet);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvP);

    m_labAdvPSource1 = new QLabel(QStringLiteral("P<n>:SOURce1"));
    m_cmbAdvPSource1P = new QComboBox(this);
    m_cmbAdvPSource1P->addItems(strlistP);
    m_cmbAdvPSource1Source = new QComboBox(this);
    strlist << "C1" << "C2" << "C3" << "C4" << "REFA" << "REFB" << "REFC" << "REFD";
    m_cmbAdvPSource1Source->addItems(strlist);
    m_btnAdvPSource1Set = new QPushButton(this);
    m_btnAdvPSource1Set->setText("set");
    m_hlayoutAdvPSource1.addWidget(m_labAdvPSource1);
    m_hlayoutAdvPSource1.addWidget(m_cmbAdvPSource1P);
    m_hlayoutAdvPSource1.addWidget(m_cmbAdvPSource1Source);
    m_hlayoutAdvPSource1.addWidget(m_btnAdvPSource1Set);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvPSource1);

    m_labAdvPSource2 = new QLabel(QStringLiteral("P<n>:SOURce2"));
    m_cmbAdvPSource2P = new QComboBox(this);
    m_cmbAdvPSource2P->addItems(strlistP);
    m_cmbAdvPSource2Source = new QComboBox(this);
    strlist << "C1" << "C2" << "C3" << "C4" << "REFA" << "REFB" << "REFC" << "REFD";
    m_cmbAdvPSource2Source->addItems(strlist);
    m_btnAdvPSource2Set = new QPushButton(this);
    m_btnAdvPSource2Set->setText("set");
    m_hlayoutAdvPSource2.addWidget(m_labAdvPSource2);
    m_hlayoutAdvPSource2.addWidget(m_cmbAdvPSource2P);
    m_hlayoutAdvPSource2.addWidget(m_cmbAdvPSource2Source);
    m_hlayoutAdvPSource2.addWidget(m_btnAdvPSource2Set);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvPSource2);

    m_labAdvPType = new QLabel(QStringLiteral("P<n>:TYPE"));
    m_cmbAdvPTypeP = new QComboBox(this);
    m_cmbAdvPTypeP->addItems(strlistP);
    m_cmbAdvPTypeParam = new QComboBox(this);
    strlistPrm << "PKPK" << "MAX" << "MIN" << "AMPL" << "TOP" << "BASE" << "LEVELX" << "CMEAN" << "MEAN"
               << "STDEV" << "VSTD" << "RMS" << "CRMS" << "MEDIAN" << "CMEDIAN" << "OVSN" << "FPRE"
               << "OVSP" << "RPRE" << "PER" << "FREQ" << "TMAX" << "TMIN" << "PWID" << "NWID" << "DUTY"
               << "NDUTY" << "WID" << "NBWID" << "DELAY" << "TIMEL" << "RISE" << "FALL" << "RISE10T90"
               << "FALL90T10" << "CCJ" << "PAREA" << "NAREA" << "AREA" << "ABSAREA" << "CYCLES" << "REDGES"
               << "FEDGES" << "EDGES" << "PPULSES" << "NPULSES" << "PHA" << "SKEW" << "FRR" << "FRF"
               << "FFR" << "FFF" << "LRR" << "LRF" << "LFR" << "LFF" << "PACArea" << "NACArea" << "ACArea" << "ABSACArea"
               << "PSLOPE" << "NSLOPE" << "TSR" << "TSF" << "THR" << "THF";
    m_cmbAdvPTypeParam->addItems(strlistPrm);
    m_btnAdvPTypeSet = new QPushButton(this);
    m_btnAdvPTypeSet->setText("set");
    m_hlayoutPType.addWidget(m_labAdvPType);
    m_hlayoutPType.addWidget(m_cmbAdvPTypeP);
    m_hlayoutPType.addWidget(m_cmbAdvPTypeParam);
    m_hlayoutPType.addWidget(m_btnAdvPTypeSet);
    m_vlayoutFinal.addLayout(&m_hlayoutPType);

    m_labAdvPVal = new QLabel(QStringLiteral("P<n>:VALue"));
    m_cmbAdvPValP = new QComboBox(this);
    m_cmbAdvPValP->addItems(strlistP);
    m_txtAdvPVal = new QLineEdit(this);
    m_hlayoutAdvPVal.addWidget(m_labAdvPVal);
    m_hlayoutAdvPVal.addWidget(m_cmbAdvPValP);
    m_hlayoutAdvPVal.addWidget(m_txtAdvPVal);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvPVal);

    m_labAdvStatsOn = new QLabel(QStringLiteral("STATisticsOn"));
    m_btnAdvStatsOn = new QRadioButton("ON/OFF", this);
    m_btnAdvStatsOn->setAutoExclusive(false);
    m_hlayoutAdvStatsOn.addWidget(m_labAdvStatsOn);
    m_hlayoutAdvStatsOn.addWidget(m_btnAdvStatsOn);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvStatsOn);

    m_labAdvPStats = new QLabel(QStringLiteral("P<n>:STATistics"));
    m_cmbAdvPStatsP = new QComboBox(this);
    m_cmbAdvPStatsP->addItems(strlistP);
    m_cmbAdvPStatsType = new QComboBox(this);
    strlist.clear();
    strlist << "ALL" << "CURRent" << "MEAN" << "MAXimum" << "MINimum" << "STDev" << "COUNt";
    m_cmbAdvPStatsType->addItems(strlist);
    m_txtAdvPStats = new QLineEdit(this);
    m_hlayoutAdvPStats.addWidget(m_labAdvPStats);
    m_hlayoutAdvPStats.addWidget(m_cmbAdvPStatsP);
    m_hlayoutAdvPStats.addWidget(m_cmbAdvPStatsType);
    m_hlayoutAdvPStats.addWidget(m_txtAdvPStats);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvPStats);

    m_labAdvPStatsType = new QLabel(QStringLiteral("P<n>:TYPE"));
    m_cmbAdvPStatsTypeP = new QComboBox(this);
    m_cmbAdvPStatsTypeP->addItems(strlistP);
    m_cmbAdvPStatsTypeParam = new QComboBox(this);
    m_cmbAdvPStatsTypeParam->addItems(strlist);
    m_hlayoutAdvPstatsType.addWidget(m_labAdvPStatsType);
    m_hlayoutAdvPstatsType.addWidget(m_cmbAdvPStatsTypeP);
    m_hlayoutAdvPstatsType.addWidget(m_cmbAdvPStatsTypeParam);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvPstatsType);

    m_labAdvStatsHistog = new QLabel(QStringLiteral("QStringLiteral"));
    m_btnAdvStatsHistog = new QRadioButton("ON/OFF", this);
    m_btnAdvStatsHistog->setAutoExclusive(false);
    m_hlayoutAdvStatsHistog.addWidget(m_labAdvStatsHistog);
    m_hlayoutAdvStatsHistog.addWidget(m_btnAdvStatsHistog);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvStatsHistog);

    m_labAdvStatsMaxcnt = new QLabel(QStringLiteral("STATistics:MAXCount"));
    m_txtAdvStatsMaxcnt = new QLineEdit(this);
    m_hlayoutAdvStatsMaxcnt.addWidget(m_labAdvStatsMaxcnt);
    m_hlayoutAdvStatsMaxcnt.addWidget(m_txtAdvStatsMaxcnt);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvStatsMaxcnt);

    m_labAdvStatsReset = new QLabel(QStringLiteral("reset"));
    m_btnAdvStatsReset = new QPushButton(this);
    m_hlayoutAdvStatsReset.addWidget(m_labAdvStatsReset);
    m_hlayoutAdvStatsReset.addWidget(m_btnAdvStatsReset);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvStatsReset);

    m_labAdvStyle = new QLabel(QStringLiteral("STYLe"));
    m_cmbAdvStyle = new QComboBox(this);
    m_hlayoutAdvStyle.addWidget(m_labAdvStyle);
    m_hlayoutAdvStyle.addWidget(m_cmbAdvStyle);
    m_vlayoutFinal.addLayout(&m_hlayoutAdvStyle);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("MeasureAdvancedWid"));
    this->setLayout(&m_vlayoutFinal);
}

CfgMeasureAdvancedWid::~CfgMeasureAdvancedWid()
{
    delete m_labAdvNumber;
    delete m_txtAdvNumber;
    delete m_labAdvP;
    delete m_cmbAdvP;
    delete m_btnAdvPOn;
    delete m_btnAdvPSet;
    delete m_labAdvPSource1;
    delete m_cmbAdvPSource1P;
    delete m_cmbAdvPSource1Source;
    delete m_btnAdvPSource1Set;
    delete m_labAdvPSource2;
    delete m_cmbAdvPSource2P;
    delete m_cmbAdvPSource2Source;
    delete m_btnAdvPSource2Set;
    delete m_labAdvPType;
    delete m_cmbAdvPTypeP;
    delete m_cmbAdvPTypeParam;
    delete m_btnAdvPTypeSet;
    delete m_labAdvPVal;
    delete m_cmbAdvPValP;
    delete m_txtAdvPVal;
    delete m_labAdvStatsOn;
    delete m_btnAdvStatsOn;
    delete m_labAdvPStats;
    delete m_cmbAdvPStatsP;
    delete m_cmbAdvPStatsType;
    delete m_txtAdvPStats;
    delete m_labAdvPStatsType;
    delete m_cmbAdvPStatsTypeP;
    delete m_cmbAdvPStatsTypeParam;
    delete m_labAdvStatsHistog;
    delete m_btnAdvStatsHistog;
    delete m_labAdvStatsMaxcnt;
    delete m_txtAdvStatsMaxcnt;
    delete m_labAdvStatsReset;
    delete m_btnAdvStatsReset;
    delete m_labAdvStyle;
    delete m_cmbAdvStyle;
}

CfgMeasureSimpleWid::CfgMeasureSimpleWid(QWidget *parent) : QDialog(parent)
{
    m_labSimpItem = new QLabel(QStringLiteral("ITEM"));
    m_cmbSimpItemParam = new QComboBox(this);
    QStringList strlist, strlistP;
    strlistP << "PKPK" << "MAX" << "MIN" << "AMPL" << "TOP" << "BASE" << "LEVELX" << "CMEAN" << "MEAN"
            << "STDEV" << "VSTD" << "RMS" << "CRMS" << "MEDIAN" << "CMEDIAN" << "OVSN" << "FPRE"
            << "OVSP" << "RPRE" << "PER" << "FREQ" << "TMAX" << "TMIN" << "PWID" << "NWID" << "DUTY"
            << "NDUTY" << "WID" << "NBWID" << "DELAY" << "TIMEL" << "RISE" << "FALL" << "RISE20T80"
            << "FALL80T20" << "CCJ" << "PAREA" << "NAREA" << "AREA" << "ABSAREA" << "CYCLES" << "REDGES"
            << "FEDGES" << "EDGES" << "PPULSES" << "NPULSES" << "PACArea" << "NACArea" << "ACArea" << "ABSACArea";
    m_cmbSimpItemParam->addItems(strlistP);
    m_cmbSimpItemState = new QComboBox(this);
    strlist << "ON" << "OFF";
    m_cmbSimpItemState->addItems(strlist);
    m_hlayoutSimpItem.addWidget(m_labSimpItem);
    m_hlayoutSimpItem.addWidget(m_cmbSimpItemParam);
    m_hlayoutSimpItem.addWidget(m_cmbSimpItemState);
    m_vlayoutFinal.addLayout(&m_hlayoutSimpItem);

    m_labSimpSource = new QLabel(QStringLiteral("SOURce"));
    m_cmbSimpSource = new QComboBox(this);
    strlist.clear();
    strlist << "C1" << "C2" << "C3" << "C4" << "REFA" << "REFB" << "REFC" << "REFD";
    m_cmbSimpSource->addItems(strlist);
    m_hlayoutSimpSource.addWidget(m_labSimpSource);
    m_hlayoutSimpSource.addWidget(m_cmbSimpSource);
    m_vlayoutFinal.addLayout(&m_hlayoutSimpSource);

    m_labSimpValue = new QLabel(QStringLiteral("VALue"));
    m_cmbSimpValueType = new QComboBox(this);
    strlistP << "ALL";
    m_cmbSimpValueType->addItems(strlistP);
    m_hlayoutSimpValue.addWidget(m_labSimpValue);
    m_hlayoutSimpValue.addWidget(m_cmbSimpValueType);
    m_vlayoutFinal.addLayout(&m_hlayoutSimpValue);

    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setWindowTitle(QStringLiteral("MeasureSimpleWid"));
    this->setLayout(&m_vlayoutFinal);
}

CfgMeasureSimpleWid::~CfgMeasureSimpleWid()
{
    delete m_labSimpItem;
    delete m_cmbSimpItemParam;
    delete m_cmbSimpItemState;
    delete m_labSimpSource;
    delete m_cmbSimpSource;
    delete m_labSimpValue;
    delete m_cmbSimpValueType;
    delete m_txtSiimpValue;
}

CfgMeasureWid::CfgMeasureWid(QWidget *parent) : QDialog(parent)
{
    m_labEnable = new QLabel(QStringLiteral("Measure"));
    m_btnEnable = new QRadioButton(QStringLiteral("ON"), this);
    m_btnEnable->setAutoExclusive(false);
    m_hlayoutEnable.addWidget(m_labEnable);
    m_hlayoutEnable.addWidget(m_btnEnable);
    m_vlayoutFinal.addLayout(&m_hlayoutEnable);

    m_labGateOn = new QLabel(QStringLiteral("Gate"));
    m_btnGateOn = new QRadioButton(QStringLiteral("ON"), this);
    m_btnGateOn->setAutoExclusive(false);
    m_hlayoutGateOn.addWidget(m_labGateOn);
    m_hlayoutGateOn.addWidget(m_btnGateOn);
    m_vlayoutFinal.addLayout(&m_hlayoutGateOn);

    m_labGateGa = new QLabel(QStringLiteral("GATE:GA"));
    m_txtGateGa = new QLineEdit(this);
    m_hlayoutGateGa.addWidget(m_labGateGa);
    m_hlayoutGateGa.addWidget(m_txtGateGa);
    m_vlayoutFinal.addLayout(&m_hlayoutGateGa);

    m_labGateGb = new QLabel(QStringLiteral("GATE:GB"));
    m_txtGateGb = new QLineEdit(this);
    m_hlayoutGateGb.addWidget(m_labGateGb);
    m_hlayoutGateGb.addWidget(m_txtGateGb);
    m_vlayoutFinal.addLayout(&m_hlayoutGateGb);

    m_labMeasureType = new QLabel(QStringLiteral("MeasureType"));
    m_cmbMeasureType = new QComboBox(this);
    QStringList  strlist;
    strlist << "NULL" <<"Simple" << "Advanced" << "Threshold";
    m_cmbMeasureType->addItems(strlist);
    m_hlayoutMeasureType.addWidget(m_labMeasureType);
    m_hlayoutMeasureType.addWidget(m_cmbMeasureType);
    m_vlayoutFinal.addLayout(&m_hlayoutMeasureType);
    InitChildWid();
    m_lastWid = nullptr;
    m_curWid = nullptr;
    connect(m_cmbMeasureType, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotSelectChildWid(int)));
    this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    this->setLayout(&m_vlayoutFinal);
}

CfgMeasureWid::~CfgMeasureWid()
{
    delete m_labEnable;
    delete m_btnEnable;
    delete m_labGateOn;
    delete m_btnGateOn;
    delete m_labGateGa;
    delete m_txtGateGa;
    delete m_labGateGb;
    delete m_txtGateGb;
    delete m_labMeasureType;
    delete m_cmbMeasureType;
    delete m_simpWid;
    delete m_advWid;
    delete m_thrWid;
}

void CfgMeasureWid::InitChildWid()
{
    m_simpWid = nullptr;
    m_advWid = nullptr;
    m_thrWid = nullptr;
}

void CfgMeasureWid::closeEvent(QCloseEvent *)
{
    if(nullptr != m_curWid)
        m_curWid->close();
    if(nullptr != m_lastWid)
        m_lastWid->close();
    this->close();
}

void CfgMeasureWid::SlotSelectChildWid(int type)
{
    if(nullptr != m_lastWid)
        m_lastWid->close();
    switch (type) {
    case 0:
        m_curWid = nullptr;
        break;
    case 1:
        if(nullptr == m_simpWid)
            m_simpWid = new CfgMeasureSimpleWid(this);
        m_curWid = (QWidget*)m_simpWid;
        break;
    case 2:
        if(nullptr == m_advWid)
            m_advWid = new CfgMeasureAdvancedWid(this);
        m_curWid = (QWidget*)m_advWid;
        break;
    case 3:
        if(nullptr == m_thrWid)
            m_thrWid = new CfgMeasureThresholdWid(this);
        m_curWid = (QWidget*)m_thrWid;
        break;
    default:
        break;
    }
    if(nullptr != m_curWid) {
        m_curWid->adjustSize();
        m_curWid->move(this->x() - 300, this->y());
        m_curWid->show();
        m_lastWid = m_curWid;
    }
}

Sds6000Qt::Sds6000Qt()
{
    this->m_sds6000 = g_Sds6000;
}

Sds6000Qt::~Sds6000Qt()
{
    this->m_sds6000 = nullptr;
}
