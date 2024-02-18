/********************************************************************************
** Form generated from reading UI file 'StilParserTool.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STILPARSERTOOL_H
#define UI_STILPARSERTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StilParserTool
{
public:
    QAction *Open;
    QAction *Analyse;
    QAction *Previous;
    QAction *Next;
    QAction *Larger;
    QAction *Smaller;
    QAction *Period;
    QAction *Normal;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QToolBar *toolBar;

    void setupUi(QMainWindow *StilParserTool)
    {
        if (StilParserTool->objectName().isEmpty())
            StilParserTool->setObjectName(QString::fromUtf8("StilParserTool"));
        StilParserTool->resize(1640, 900);
        StilParserTool->setMinimumSize(QSize(1640, 0));
        StilParserTool->setMaximumSize(QSize(1640, 16777215));
        Open = new QAction(StilParserTool);
        Open->setObjectName(QString::fromUtf8("Open"));
        Analyse = new QAction(StilParserTool);
        Analyse->setObjectName(QString::fromUtf8("Analyse"));
        Analyse->setEnabled(false);
        Previous = new QAction(StilParserTool);
        Previous->setObjectName(QString::fromUtf8("Previous"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        Previous->setIcon(icon);
        Next = new QAction(StilParserTool);
        Next->setObjectName(QString::fromUtf8("Next"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/image/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        Next->setIcon(icon1);
        Larger = new QAction(StilParserTool);
        Larger->setObjectName(QString::fromUtf8("Larger"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/image/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        Larger->setIcon(icon2);
        Smaller = new QAction(StilParserTool);
        Smaller->setObjectName(QString::fromUtf8("Smaller"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/image/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        Smaller->setIcon(icon3);
        Period = new QAction(StilParserTool);
        Period->setObjectName(QString::fromUtf8("Period"));
        Period->setEnabled(true);
        Normal = new QAction(StilParserTool);
        Normal->setObjectName(QString::fromUtf8("Normal"));
        Normal->setEnabled(false);
        centralwidget = new QWidget(StilParserTool);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setMaximumSize(QSize(16777215, 16777215));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1600, 846));
        scrollAreaWidgetContents->setMinimumSize(QSize(1600, 0));
        scrollAreaWidgetContents->setMaximumSize(QSize(1600, 16777215));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);

        StilParserTool->setCentralWidget(centralwidget);
        toolBar = new QToolBar(StilParserTool);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        StilParserTool->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(Open);
        toolBar->addAction(Analyse);
        toolBar->addSeparator();
        toolBar->addAction(Previous);
        toolBar->addAction(Next);
        toolBar->addSeparator();
        toolBar->addAction(Larger);
        toolBar->addAction(Smaller);
        toolBar->addSeparator();
        toolBar->addAction(Normal);
        toolBar->addAction(Period);

        retranslateUi(StilParserTool);

        QMetaObject::connectSlotsByName(StilParserTool);
    } // setupUi

    void retranslateUi(QMainWindow *StilParserTool)
    {
        StilParserTool->setWindowTitle(QCoreApplication::translate("StilParserTool", "WaveFormView", nullptr));
        Open->setText(QCoreApplication::translate("StilParserTool", "\346\211\223\345\274\200", nullptr));
        Analyse->setText(QCoreApplication::translate("StilParserTool", "\350\247\243\346\236\220", nullptr));
        Previous->setText(QCoreApplication::translate("StilParserTool", "\345\211\215\344\270\200\351\241\265", nullptr));
#if QT_CONFIG(tooltip)
        Previous->setToolTip(QCoreApplication::translate("StilParserTool", "\345\211\215\344\270\200\351\241\265", nullptr));
#endif // QT_CONFIG(tooltip)
        Next->setText(QCoreApplication::translate("StilParserTool", "\345\220\216\344\270\200\351\241\265", nullptr));
#if QT_CONFIG(tooltip)
        Next->setToolTip(QCoreApplication::translate("StilParserTool", "\345\220\216\344\270\200\351\241\265", nullptr));
#endif // QT_CONFIG(tooltip)
        Larger->setText(QCoreApplication::translate("StilParserTool", "\346\224\276\345\244\247", nullptr));
        Smaller->setText(QCoreApplication::translate("StilParserTool", "\347\274\251\345\260\217", nullptr));
        Period->setText(QCoreApplication::translate("StilParserTool", "\345\221\250\346\234\237", nullptr));
        Normal->setText(QCoreApplication::translate("StilParserTool", "\346\240\207\345\207\206", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("StilParserTool", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StilParserTool: public Ui_StilParserTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STILPARSERTOOL_H
