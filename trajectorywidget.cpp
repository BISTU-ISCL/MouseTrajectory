#include "trajectorywidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

TrajectoryWidget::TrajectoryWidget(QWidget *parent)
    : QWidget(parent)
{
    // 确保鼠标移动时不需要按键也能捕获事件
    setMouseTracking(true);
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    // 启动计时器，用于记录每个点的时间戳
    m_timer.start();

    // 定时刷新：约每 16ms 更新一次界面，确保轨迹流畅
    m_refreshTimer.setInterval(16);
    connect(&m_refreshTimer, &QTimer::timeout, this, &TrajectoryWidget::refreshTrail);
    m_refreshTimer.start();
}

void TrajectoryWidget::mouseMoveEvent(QMouseEvent *event)
{
    // 记录当前鼠标位置和时间
    m_points.append({event->position(), m_timer.elapsed() / 1000.0});
    pruneExpiredPoints();
    update();
}

void TrajectoryWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    pruneExpiredPoints();

    // 少于两个点无法形成线段，直接返回
    if (m_points.size() < 2)
        return;

    // 当前时间，用于计算每个点的“年龄”占比
    const qreal nowSec = m_timer.elapsed() / 1000.0;

    // 依次绘制相邻点之间的线段，并根据时间衰减调整颜色
    for (int i = 0; i < m_points.size() - 1; ++i) {
        const Sample &a = m_points[i];
        const Sample &b = m_points[i + 1];

        // 计算两个端点的年龄比例（0 为最新，1 为最旧）
        const qreal ageA = qBound<qreal>(0.0, (nowSec - a.timestamp) / m_windowSeconds, 1.0);
        const qreal ageB = qBound<qreal>(0.0, (nowSec - b.timestamp) / m_windowSeconds, 1.0);
        const qreal averageAge = (ageA + ageB) / 2.0;

        // 根据年龄生成渐变颜色：新的点亮度更高、透明度更高
        QColor segmentColor = colorForAge(averageAge);
        QPen pen(segmentColor, 6.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.drawLine(a.pos, b.pos);
    }

    // 额外绘制当前鼠标位置的圆点，更容易分辨起点
    const Sample &latest = m_points.last();
    QColor headColor = colorForAge(0.0);
    painter.setBrush(headColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(latest.pos, 6.0, 6.0);
}

void TrajectoryWidget::refreshTrail()
{
    pruneExpiredPoints();
    update();
}

void TrajectoryWidget::pruneExpiredPoints()
{
    const qreal nowSec = m_timer.elapsed() / 1000.0;
    // 移除超出保留时间窗口的点
    while (!m_points.isEmpty() && nowSec - m_points.first().timestamp > m_windowSeconds) {
        m_points.removeFirst();
    }
}

QColor TrajectoryWidget::colorForAge(qreal ageRatio) const
{
    // 颜色从蓝色过渡到橙色，同时透明度随时间衰减
    const QColor startColor(0, 162, 255);   // 最新点颜色（亮蓝）
    const QColor endColor(255, 140, 0);     // 最旧点颜色（橙色）

    qreal r = startColor.red() + (endColor.red() - startColor.red()) * ageRatio;
    qreal g = startColor.green() + (endColor.green() - startColor.green()) * ageRatio;
    qreal b = startColor.blue() + (endColor.blue() - startColor.blue()) * ageRatio;

    // alpha 从 255 线性衰减到 0，实现尾部淡出
    qreal alpha = 255 * (1.0 - ageRatio);

    return QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(alpha));
}
