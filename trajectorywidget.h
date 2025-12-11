#ifndef TRAJECTORYWIDGET_H
#define TRAJECTORYWIDGET_H

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

// 负责绘制鼠标轨迹的自定义控件
class TrajectoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrajectoryWidget(QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void refreshTrail();

private:
    struct Sample
    {
        QPointF pos;       // 鼠标位置
        qreal timestamp;   // 记录的秒数
    };

    void pruneExpiredPoints();
    QColor colorForAge(qreal ageRatio) const;

    QVector<Sample> m_points;    // 保存最近 3 秒内的轨迹点
    QElapsedTimer m_timer;       // 计时器，用于计算时间戳
    QTimer m_refreshTimer;       // 定时刷新绘制
    const qreal m_windowSeconds = 3.0; // 轨迹保留时长
};

#endif // TRAJECTORYWIDGET_H
