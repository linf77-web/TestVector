#ifndef DEF_H
#define DEF_H

#ifndef CMD_MAX
#define CMD_MAX (64)
#endif
#ifndef READ_LENGTH
#define READ_LENGTH (64)
#endif
#ifndef VAL_MAX
#define VAL_MAX (256)
#endif
#ifndef LOG_MAX
#define LOG_MAX (512)
#endif
//#ifndef TIMEOUT_DEFAULT
//#define TIMEOUT_DEFAULT (3)
//#endif
#ifndef LIST_MIN
#define LIST_MIN (10)
#endif
#ifndef LIST_MAX
#define LIST_MAX (100)
#endif
#ifndef DEV_MAX_CNT
#define DEV_MAX_CNT (8)
#endif
#include "Include/Utl8500.h"
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QDateTime>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QTimer>
#include <QButtonGroup>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QRadioButton>
#include <iostream>
#include <QString>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QLCDNumber>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QTreeView>
#include <QStandardItem>
#include <QList>
#include <QTableView>
#include <QItemDelegate>
#include <QHeaderView>
#include <QThread>
#include <QFileDialog>
#include <QApplication>
#include <QProgressBar>
#include <vector>

struct ComInfo{
    DWORD dwInQueue;
    DWORD dwOutQueue;
    DWORD BaudRate;
    BYTE ByteSize;
    BYTE Parity;
    BYTE StopBits;
};

extern std::string UTF8_TO_string(const std::string& str);
extern std::string string_TO_UTF8(const std::string& str);
class LabelDeleget : public QItemDelegate
{
public:
    LabelDeleget(QObject* parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class DoubleSpinDeleget : public QItemDelegate
{
public:
    DoubleSpinDeleget(QObject* parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class ComboBoxDeleget : public QItemDelegate
{
private:
    QStringList               m_cmbList;
public:
    ComboBoxDeleget(QStringList& cmblist, QObject* parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // DEF_H
//typedef enum {UDP6731, UTG4082A, SDS6034, UIL8512} DevType;


