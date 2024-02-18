/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStil;
    QAction *actionEVCD;
    QAction *actionVCD;
    QAction *actionPinMap;
    QAction *actionTiming;
    QAction *actionPattern;
    QAction *actionDigitalBoard;
    QAction *actionSDS6034;
    QAction *actionUDP6730;
    QAction *actionUTG4082;
    QAction *actionUTL8512;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuPatternFile;
    QMenu *menuDevices;
    QMenu *menuDOscilloscope;
    QMenu *menuDCPower;
    QMenu *menuSigSource;
    QMenu *menuELoad;
    QMenu *menuTools;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1420, 847);
        actionStil = new QAction(MainWindow);
        actionStil->setObjectName(QString::fromUtf8("actionStil"));
        actionEVCD = new QAction(MainWindow);
        actionEVCD->setObjectName(QString::fromUtf8("actionEVCD"));
        actionVCD = new QAction(MainWindow);
        actionVCD->setObjectName(QString::fromUtf8("actionVCD"));
        actionPinMap = new QAction(MainWindow);
        actionPinMap->setObjectName(QString::fromUtf8("actionPinMap"));
        actionTiming = new QAction(MainWindow);
        actionTiming->setObjectName(QString::fromUtf8("actionTiming"));
        actionPattern = new QAction(MainWindow);
        actionPattern->setObjectName(QString::fromUtf8("actionPattern"));
        actionDigitalBoard = new QAction(MainWindow);
        actionDigitalBoard->setObjectName(QString::fromUtf8("actionDigitalBoard"));
        actionSDS6034 = new QAction(MainWindow);
        actionSDS6034->setObjectName(QString::fromUtf8("actionSDS6034"));
        actionUDP6730 = new QAction(MainWindow);
        actionUDP6730->setObjectName(QString::fromUtf8("actionUDP6730"));
        actionUTG4082 = new QAction(MainWindow);
        actionUTG4082->setObjectName(QString::fromUtf8("actionUTG4082"));
        actionUTL8512 = new QAction(MainWindow);
        actionUTL8512->setObjectName(QString::fromUtf8("actionUTL8512"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1420, 23));
        menuPatternFile = new QMenu(menuBar);
        menuPatternFile->setObjectName(QString::fromUtf8("menuPatternFile"));
        menuDevices = new QMenu(menuBar);
        menuDevices->setObjectName(QString::fromUtf8("menuDevices"));
        menuDOscilloscope = new QMenu(menuDevices);
        menuDOscilloscope->setObjectName(QString::fromUtf8("menuDOscilloscope"));
        menuDCPower = new QMenu(menuDevices);
        menuDCPower->setObjectName(QString::fromUtf8("menuDCPower"));
        menuSigSource = new QMenu(menuDevices);
        menuSigSource->setObjectName(QString::fromUtf8("menuSigSource"));
        menuELoad = new QMenu(menuDevices);
        menuELoad->setObjectName(QString::fromUtf8("menuELoad"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuPatternFile->menuAction());
        menuBar->addAction(menuDevices->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuPatternFile->addAction(actionStil);
        menuPatternFile->addAction(actionEVCD);
        menuPatternFile->addAction(actionVCD);
        menuDevices->addAction(actionDigitalBoard);
        menuDevices->addAction(menuSigSource->menuAction());
        menuDevices->addAction(menuDOscilloscope->menuAction());
        menuDevices->addAction(menuDCPower->menuAction());
        menuDevices->addAction(menuELoad->menuAction());
        menuDOscilloscope->addAction(actionSDS6034);
        menuDCPower->addAction(actionUDP6730);
        menuSigSource->addAction(actionUTG4082);
        menuELoad->addAction(actionUTL8512);
        menuTools->addAction(actionPinMap);
        menuTools->addAction(actionTiming);
        menuTools->addAction(actionPattern);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionStil->setText(QCoreApplication::translate("MainWindow", "STIL", nullptr));
        actionEVCD->setText(QCoreApplication::translate("MainWindow", "EVCD", nullptr));
        actionVCD->setText(QCoreApplication::translate("MainWindow", "VCD", nullptr));
        actionPinMap->setText(QCoreApplication::translate("MainWindow", "PinMap", nullptr));
        actionTiming->setText(QCoreApplication::translate("MainWindow", "Timing", nullptr));
        actionPattern->setText(QCoreApplication::translate("MainWindow", "Pattern", nullptr));
        actionDigitalBoard->setText(QCoreApplication::translate("MainWindow", "\346\225\260\345\255\227\346\235\277\345\215\241", nullptr));
        actionSDS6034->setText(QCoreApplication::translate("MainWindow", "SDS6034", nullptr));
        actionUDP6730->setText(QCoreApplication::translate("MainWindow", "UDP6730", nullptr));
        actionUTG4082->setText(QCoreApplication::translate("MainWindow", "UTG4082", nullptr));
        actionUTL8512->setText(QCoreApplication::translate("MainWindow", "UTL8512", nullptr));
        menuPatternFile->setTitle(QCoreApplication::translate("MainWindow", "\345\220\221\351\207\217\346\226\207\344\273\266", nullptr));
        menuDevices->setTitle(QCoreApplication::translate("MainWindow", "\350\256\276\345\244\207", nullptr));
        menuDOscilloscope->setTitle(QCoreApplication::translate("MainWindow", "\346\225\260\345\255\227\347\244\272\346\263\242\345\231\250", nullptr));
        menuDCPower->setTitle(QCoreApplication::translate("MainWindow", "\347\233\264\346\265\201\347\224\265\346\272\220", nullptr));
        menuSigSource->setTitle(QCoreApplication::translate("MainWindow", "\344\277\241\345\217\267\346\272\220", nullptr));
        menuELoad->setTitle(QCoreApplication::translate("MainWindow", "\347\224\265\345\255\220\350\264\237\350\275\275", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
