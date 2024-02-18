#include "WaveformView.h"
#include "ui_WaveformView.h"

WaveformView::WaveformView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WaveformView),
    m_multiple(1),
    m_startTime(0),
    m_table(0)
{
    ui->setupUi(this);
    this->setStyleSheet("QLabel{font-family: 'Microsoft YaHei';font-size:14px;font-weight: bold;background-color: #ffffff;}"
        "#scale_1{font-family: 'Microsoft YaHei';font-size:14px;font-weight: bold;background-color: red;}"
        "#scale_2{font-family: 'Microsoft YaHei';font-size:14px;font-weight: bold;background-color: yellow;}"
        "#label{border: 1px solid white;background-color: transparent;}"
        "#line_1{border-top: 1px solid red; background-color: red;}"
        "#line_2{border-top: 1px solid yellow; background-color: yellow;}"
        "QComboBox{font-family: 'Microsoft YaHei';font-size:18px;min-width:200px;min-height:30px;}"
        "QLineEdit{font-family: 'Microsoft YaHei';font-size:18px;min-height:30px;}"
        "QSpinBox{font-family: 'Microsoft YaHei';font-size:18px;min-height:30px;}"
        "QPushButton {font-family: 'Microsoft YaHei';font-size: 18px;font-weight: bold;background-color: #ffffff;}");
    this->setMouseTracking(true);
    ui->scale_1->setGeometry(150, 50, 50, 15);
    ui->scale_1->setText("50ns");
    ui->scale_1->adjustSize();
    ui->scale_1->show();
    ui->line_1->setGeometry(150, 50, 1, this->height());
    ui->line_1->show();
    ui->scale_2->setGeometry(200, 50, 50, 15);
    ui->scale_2->setText("100ns");
    ui->scale_2->adjustSize();
    ui->scale_2->show();
    ui->line_2->setGeometry(200, 50, 1, this->height());
    ui->line_2->show();
    ui->label->hide();

    int differ = ui->line_2->x() - ui->line_1->x();
    double freq = 1.0 / differ * 1000000;
    if (freq < 0) {
        freq = -freq;
    }
    ui->differ->setText(QString::number(differ * m_multiple) + "ns  " + QString::number(freq) + "kHz");
    ui->differ->adjustSize();
    ui->differ->move((ui->line_2->x() + ui->line_1->x()) / 2 - ui->differ->width() / 2, 0);
}

WaveformView::~WaveformView()
{
    delete ui;
}

void WaveformView::SetSignals(QStringList sig_list)
{
    foreach(auto itor, m_vectorLab) {
        itor->hide();
    }

    int row = 1;
    for (int i = 0; i < sig_list.size(); i++) {
        QLabel* lab;
        if (m_vectorLab.size() < row)
        {
            lab = new QLabel(sig_list[i], this);
            lab->setAlignment(Qt::AlignRight | Qt::AlignCenter);
            m_vectorLab.push_back(lab);
        }
        else {
            lab = m_vectorLab[row - 1];
            lab->setText(sig_list[i]);
        }
        lab->setGeometry(0, row * 50, 100, 50);
        lab->show();
        row++;
    }
    this->setGeometry(0, 0, this->width(), (row + 1) * 50);
}

void WaveformView::SetShowMultiple(int multiple)
{
    m_multiple = multiple;
    ui->scale_1->setText(QString::number(m_startTime + (ui->line_1->x() - 100) * m_multiple) + "ns");
    ui->scale_1->adjustSize();
    ui->scale_2->setText(QString::number(m_startTime + (ui->line_2->x() - 100) * m_multiple) + "ns");
    ui->scale_2->adjustSize();
    int differ = ui->line_2->x() - ui->line_1->x();
    double freq = 1.0 / (differ * m_multiple) * 1000000;
    if (freq < 0) {
        freq = -freq;
    }
    ui->differ->setText(QString::number(differ * m_multiple) + "ns  " + QString::number(freq) + "kHz");
    ui->differ->adjustSize();
    ui->differ->move((ui->line_2->x() + ui->line_1->x()) / 2 - ui->differ->width() / 2, 0);
}

void WaveformView::SetShowPeriodGrid(bool enable)
{
    m_isShowPeriodGrid = enable;
}

void WaveformView::SetWaveformViewStartTime(int time)
{
    m_startTime = time;
    ui->scale_1->setText(QString::number(m_startTime + (ui->line_1->x() - 100) * m_multiple) + "ns");
    ui->scale_1->adjustSize();
    ui->scale_2->setText(QString::number(m_startTime + (ui->line_2->x() - 100) * m_multiple) + "ns");
    ui->scale_2->adjustSize();
}

void WaveformView::SetSignalTruthTable(SignalTruthTable* table)
{
    m_table = *table;
}

void WaveformView::paintEvent(QPaintEvent* pe) {
    Q_UNUSED(pe);
    QPainter painter(this);
    QPen greenPen(QBrush(Qt::green), 1);//Force pen
    QPen bluePen(QBrush(Qt::blue), 1);//Compare pen
    QPen bluePen2(QBrush(Qt::blue), 2);//Compare pen
    QPen whitePen(QBrush(Qt::white), 1);
    QPen grayPen(QBrush(Qt::gray), 1);
    painter.setViewport(100, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.fillRect(0, 0, width(), height(), Qt::black);

    //表头
    painter.setPen(QPen(QBrush(Qt::green), 2));
    painter.drawLine(0, 50, width(), 50);

    painter.setPen(greenPen);
    for (int i = 0; i <= width() / 100; i++) {
        painter.drawText(100 * i, 35, QString("%1ns").arg(i * 100 * m_multiple + m_startTime));
        painter.drawLine(100 * i, 40, 100 * i, 50);
        painter.drawLine(100 * i + 50, 45, 100 * i + 50, 50);
    }

    //网格
    QVector<qreal>dashes;
    qreal space = 8;
    dashes << 2 << space;
    whitePen.setDashPattern(dashes);
    painter.setPen(whitePen);
    if (!m_isShowPeriodGrid) {
        for (int i = 1; i <= width() / 100; i++) {
            painter.drawLine(100 * i, 50, 100 * i, height());
        }
    }
    for (int i = 2; i <= height() / 50; i++) {
        painter.drawLine(0, 50 * i, width(), 50 * i);
    }

    if (m_table.empty()) {
        return;
    }
    int row = 2;
    for (int i = 0; i < m_table.size(); i++) {

        EdgeList elist = m_table.at(i);
        int lastEvent = 0;
        int lastX = 0;
        int lastY = 50 * row;
        for (int i = 0; i < elist.size(); i++) {
            Edge edge = elist[i];
            int offset = edge.offset;
            int x = (offset - m_startTime) / m_multiple;
            int y;
            switch (edge.event) {
            case EventType_Unknown:
                y = 50 * row - 20;
                break;
            case EventType_ForceDown:
                y = 50 * row;
                break;
            case EventType_ForceUp:
                y = 50 * row - 40;
                break;
            case EventType_ForceOff:
                y = 50 * row - 20;
                break;
            case EventType_ForcePrior:
                y = lastY;
                break;
            case EventType_CompareLow:
                y = lastY;
                painter.setPen(bluePen2);
                painter.drawLine(x, 50 * row - 40, x, 50 * row);
                painter.drawLine(x - 5, 50 * row - 5, x, 50 * row);
                painter.drawLine(x, 50 * row, x + 5, 50 * row - 5);
                break;
            case EventType_CompareHigh:
                y = lastY;
                painter.setPen(bluePen2);
                painter.drawLine(x, 50 * row - 40, x, 50 * row);
                painter.drawLine(x - 5, 50 * row - 35, x, 50 * row - 40);
                painter.drawLine(x, 50 * row - 40, x + 5, 50 * row - 35);
                break;
            case EventType_CompareUnknown:
                y = 50 * row - 20;
                break;
            case EventType_CompareOff:
                y = lastY;
                painter.setPen(bluePen2);
                painter.drawLine(x, 50 * row - 40, x, 50 * row);
                painter.drawLine(x - 5, 50 * row - 15, x + 5, 50 * row - 25);
                painter.drawLine(x - 5, 50 * row - 25, x + 5, 50 * row - 15);
                break;
            case EventType_CompareValid:
                y = lastY;
                painter.setPen(bluePen2);
                painter.drawLine(x, 50 * row - 40, x, 50 * row);
                painter.drawLine(x - 5, 50 * row - 5, x, 50 * row);
                painter.drawLine(x, 50 * row, x + 5, 50 * row - 5);
                painter.drawLine(x - 5, 50 * row - 35, x, 50 * row - 40);
                painter.drawLine(x, 50 * row - 40, x + 5, 50 * row - 35);
                break;
            case EventType_CompareLowWindow:
                y = 50 * row;
                painter.setPen(bluePen);
                painter.drawLine(x, lastY, x, y);
                break;
            case EventType_CompareHighWindow:
                y = 50 * row - 40;
                painter.setPen(bluePen);
                painter.drawLine(x, 50 * row, x, y);
                break;
            case EventType_CompareOffWindow:
                y = 50 * row - 20;
                painter.setPen(bluePen);
                painter.drawLine(x, 50 * row, x, y);
                break;
            case EventType_CompareVaildWindow:
                y = 50 * row - 40;
                break;
            default:
                y = lastY;
                if (m_isShowPeriodGrid) {
                    painter.setPen(whitePen);
                    painter.drawLine(x, 50 * row - 50, x, 50 * row);
                }
                break;
            }

            switch (lastEvent) {
            case EventType_Unknown:
                painter.setPen(grayPen);
                painter.drawLine(lastX, 50 * row, x, 50 * row);
                painter.drawLine(lastX, 50 * row - 20, x, 50 * row - 20);
                painter.drawLine(lastX, 50 * row - 20, x, 50 * row);
                painter.drawLine(lastX, 50 * row, x, 50 * row - 20);
                break;
            case EventType_ForceDown:
                painter.setPen(greenPen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, y);
                break;
            case EventType_ForceUp:
                painter.setPen(greenPen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, y);
                break;
            case EventType_ForceOff:
                painter.setPen(greenPen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, y);
                break;
            case EventType_ForcePrior:
                painter.setPen(greenPen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(lastX, 50 * row, x, 50 * row);
                painter.drawLine(x, lastY, x, y);
                break;
            case EventType_CompareLow:
                break;
            case EventType_CompareHigh:
                break;
            case EventType_CompareUnknown:
                painter.setPen(grayPen);
                painter.drawLine(lastX, 50 * row, x, 50 * row);
                painter.drawLine(lastX, 50 * row - 20, x, 50 * row - 20);
                painter.drawLine(lastX, 50 * row - 20, x, 50 * row);
                painter.drawLine(lastX, 50 * row, x, 50 * row - 20);
                break;
            case EventType_CompareOff:
                break;
            case EventType_CompareValid:
                break;
            case EventType_CompareLowWindow:
                painter.setPen(bluePen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, y);
                break;
            case EventType_CompareHighWindow:
                painter.setPen(bluePen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, 50 * row);
                break;
            case EventType_CompareOffWindow:
                painter.setPen(bluePen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, 50 * row);
                break;
            case EventType_CompareVaildWindow:
                painter.setPen(greenPen);
                painter.drawLine(lastX, lastY, x, lastY);
                painter.drawLine(x, lastY, x, y);
                break;
            default:
                break;
            }
            lastX = x;
            lastY = y;
            if (edge.event != 0xff) {
                lastEvent = edge.event;
            }
        }

        row++;
    }
}

void WaveformView::mousePressEvent(QMouseEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QPoint p = event->pos();

        if (100 > p.x()) {
            return;
        }
        qDebug("mousePressEvent Point(%d,%d)", p.x(), p.y());
        if (ui->line_1->x() - 2 <= p.x() && p.x() <= ui->line_1->x() + 2) {
            m_resolutionDiffer = ui->line_1->x() - p.x();
            m_line1IsPress = true;
        }
        else if (ui->line_2->x() - 2 <= p.x() && p.x() <= ui->line_2->x() + 2) {
            m_resolutionDiffer = ui->line_2->x() - p.x();
            m_line2IsPress = true;
        }
        else {
            m_mouseIsPress = true;
            m_mousePressPoint = p;
        }

    }
}

void WaveformView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QPoint p = event->pos();

        if (100 > p.x()) {
            return;
        }
        qDebug("mouseReleaseEvent Point(%d,%d)", p.x(), p.y());
        m_line1IsPress = false;
        m_line2IsPress = false;
        if (m_mouseIsPress)
        {
            if (p.x() - m_mousePressPoint.x() > 200) {
                emit sig_EnlargeWaveformView(m_mousePressPoint);
            }
            m_mouseIsPress = false;
            ui->label->hide();
        }

    }
}

void WaveformView::mouseMoveEvent(QMouseEvent* event) {
    if (event->type() == QEvent::MouseMove) {
        QPoint p = event->pos();

        if (100 > p.x() || 1600 < p.x()) {
            m_line1IsPress = false;
            m_line2IsPress = false;
            return;
        }
        if (m_line1IsPress) {
            qDebug("mouseMoveEvent Point(%d,%d)", p.x(), p.y());
            ui->scale_1->setText(QString::number((p.x() + m_resolutionDiffer - 100) * m_multiple + m_startTime) + "ns");
            ui->scale_1->adjustSize();
            if (1500 < p.x()) {
                ui->scale_1->move(p.x() + m_resolutionDiffer - ui->scale_1->width(), 50);
            }
            else {
                ui->scale_1->move(p.x() + m_resolutionDiffer, 50);
            }
            ui->line_1->move(p.x() + m_resolutionDiffer, 50);
            int differ = ui->line_2->x() - ui->line_1->x();
            double freq = 1.0 / differ * 1000000;
            if (freq < 0) {
                freq = -freq;
            }
            ui->differ->setText(QString::number(differ * m_multiple) + "ns  " + QString::number(freq) + "kHz");
            ui->differ->adjustSize();
            ui->differ->move((ui->line_2->x() + ui->line_1->x()) / 2 - ui->differ->width() / 2, 0);
        }
        else if (m_line2IsPress) {
            qDebug("mouseMoveEvent Point(%d,%d)", p.x(), p.y());
            ui->scale_2->setText(QString::number((p.x() + m_resolutionDiffer - 100) * m_multiple + m_startTime) + "ns");
            ui->scale_2->adjustSize();
            if (1500 < p.x()) {
                ui->scale_2->move(p.x() + m_resolutionDiffer - ui->scale_1->width(), 50);
            }
            else {
                ui->scale_2->move(p.x() + m_resolutionDiffer, 50);
            }
            ui->line_2->move(p.x() + m_resolutionDiffer, 50);
            int differ = ui->line_2->x() - ui->line_1->x();
            double freq = 1.0 / differ * 1000000;
            if (freq < 0) {
                freq = -freq;
            }
            ui->differ->setText(QString::number(differ * m_multiple) + "ns  " + QString::number(freq) + "kHz");
            ui->differ->adjustSize();
            ui->differ->move((ui->line_2->x() + ui->line_1->x()) / 2 - ui->differ->width() / 2, 0);
        }
        else if (m_mouseIsPress) {
            ui->label->setGeometry(m_mousePressPoint.x(), m_mousePressPoint.y(), p.x() - m_mousePressPoint.x(), p.y() - m_mousePressPoint.y());
            ui->label->show();
        }

    }
}

void WaveformView::on_WaveformView_customContextMenuRequested(const QPoint& pos)
{
    //    qDebug()<<"Point(x, y):"<<pos.x()<<","<<pos.y();
    m_tabMenuBar->move(cursor().pos());
    int row = pos.y() / 50;
    qDebug() << "row:" << row;
    if (m_vectorLab.size() < row)
        return;
    m_clickedSignalName = m_vectorLab[row - 1]->text();
    m_tabMenuBar->show();
}

