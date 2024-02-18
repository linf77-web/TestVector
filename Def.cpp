#include "Def.h"
std::string UTF8_TO_string(const std::string& str)
{
    //utf8 to wide_char
    int wlen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[wlen + 1];
    memset(pwBuf, 0, wlen * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), pwBuf, wlen);
    //wide_char to std::string
    int len = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, 0, NULL, NULL);
    char* pBuf = new char[len + 1];
    memset(pBuf, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, pwBuf, wlen, pBuf, len, NULL, NULL);

    std::string strRet(pBuf);
    delete []pBuf;
    delete []pwBuf;
    pBuf = NULL;
    pwBuf = NULL;
    return strRet;
}
std::string string_TO_UTF8(const std::string& str)
{
    //std::string to wide_char
    int wlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[wlen + 1];
    memset(pwBuf, 0, wlen * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), pwBuf, wlen);
    //wide_cahr to utf8
    int len = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, 0, NULL, NULL);
    char* pBuf = new char[len + 1];
    memset(pBuf, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, pwBuf, wlen, pBuf, len, NULL, NULL);

    std::string strRet(pBuf);
    delete []pBuf;
    delete []pwBuf;
    pBuf = NULL;
    pwBuf = NULL;
    return strRet;
}

LabelDeleget::LabelDeleget(QObject* parent) : QItemDelegate(parent)
{

}
QWidget* LabelDeleget::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLabel* lab = new QLabel(parent);
    lab->installEventFilter(const_cast<LabelDeleget*>(this));
    return lab;
}
void LabelDeleget::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLabel* lab = static_cast<QLabel*>(editor);
    QString str = index.model()->data(index).toString();
    lab->setText(str);
}
void LabelDeleget::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLabel* lab = static_cast<QLabel*>(editor);
    QString str = lab->text();
    model->setData(index, str);
}
void LabelDeleget::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
DoubleSpinDeleget::DoubleSpinDeleget(QObject* parent) : QItemDelegate(parent)
{

}
QWidget* DoubleSpinDeleget::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QDoubleSpinBox* dSpinBox = new QDoubleSpinBox(parent);
    dSpinBox->setRange(1.00f, 100.0f);
    dSpinBox->installEventFilter(const_cast<DoubleSpinDeleget*>(this));
    return dSpinBox;
}
void DoubleSpinDeleget::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox* dSpinBox = static_cast<QDoubleSpinBox*>(editor);
    double val = index.model()->data(index).toDouble();
    dSpinBox->setValue(val);
}
void DoubleSpinDeleget::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox* dSpinBox = static_cast<QDoubleSpinBox*>(editor);
    double val = dSpinBox->value();
    model->setData(index, val);
}
void DoubleSpinDeleget::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

ComboBoxDeleget::ComboBoxDeleget(QStringList& cmblist, QObject *parent) : QItemDelegate(parent), m_cmbList(cmblist)
{

}

QWidget *ComboBoxDeleget::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox* dcmbBox = new QComboBox(parent);
    QStringList list;
    dcmbBox->addItems(m_cmbList);
    dcmbBox->setCurrentIndex(0);
    dcmbBox->installEventFilter(const_cast<ComboBoxDeleget*>(this));
    return dcmbBox;
}

void ComboBoxDeleget::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox* dcmbBox = static_cast<QComboBox*>(editor);
    QString str = index.model()->data(index).toString();
    dcmbBox->setCurrentText(str);
}

void ComboBoxDeleget::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* dcmbBox = static_cast<QComboBox*>(editor);
    QString str = dcmbBox->currentText();
    model->setData(index, str);
}

void ComboBoxDeleget::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

