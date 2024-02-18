/********************************************************************************
** Form generated from reading UI file 'WaveformView.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAVEFORMVIEW_H
#define UI_WAVEFORMVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaveformView
{
public:
    QFrame *line_1;
    QLabel *scale_1;
    QFrame *line_2;
    QLabel *scale_2;
    QLabel *differ;
    QLabel *label;

    void setupUi(QWidget *WaveformView)
    {
        if (WaveformView->objectName().isEmpty())
            WaveformView->setObjectName(QString::fromUtf8("WaveformView"));
        WaveformView->resize(1641, 900);
        WaveformView->setMinimumSize(QSize(1601, 0));
        WaveformView->setMaximumSize(QSize(2560, 16777215));
        WaveformView->setStyleSheet(QString::fromUtf8(""));
        line_1 = new QFrame(WaveformView);
        line_1->setObjectName(QString::fromUtf8("line_1"));
        line_1->setGeometry(QRect(500, 30, 16, 830));
        line_1->setLineWidth(1);
        line_1->setFrameShape(QFrame::VLine);
        line_1->setFrameShadow(QFrame::Sunken);
        scale_1 = new QLabel(WaveformView);
        scale_1->setObjectName(QString::fromUtf8("scale_1"));
        scale_1->setGeometry(QRect(610, 30, 54, 12));
        line_2 = new QFrame(WaveformView);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(1010, 30, 16, 830));
        line_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
        line_2->setLineWidth(1);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        scale_2 = new QLabel(WaveformView);
        scale_2->setObjectName(QString::fromUtf8("scale_2"));
        scale_2->setGeometry(QRect(800, 210, 54, 12));
        differ = new QLabel(WaveformView);
        differ->setObjectName(QString::fromUtf8("differ"));
        differ->setGeometry(QRect(1100, 250, 54, 12));
        label = new QLabel(WaveformView);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(340, 300, 54, 12));

        retranslateUi(WaveformView);

        QMetaObject::connectSlotsByName(WaveformView);
    } // setupUi

    void retranslateUi(QWidget *WaveformView)
    {
        WaveformView->setWindowTitle(QCoreApplication::translate("WaveformView", "Form", nullptr));
        scale_1->setText(QString());
        scale_2->setText(QString());
        differ->setText(QString());
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WaveformView: public Ui_WaveformView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAVEFORMVIEW_H
