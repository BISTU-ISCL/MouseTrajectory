#include "trajectorywidget.h"

#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 主窗口容器
    QWidget window;
    window.setWindowTitle(QStringLiteral("鼠标轨迹实时预览"));
    window.resize(800, 600);

    // 提示文本，告诉用户如何体验效果
    auto *hintLabel = new QLabel(QStringLiteral(
        "将鼠标在白色区域内移动，系统会实时用渐变线条展示最近 3 秒的移动路径。"
        "颜色会从亮蓝逐渐过渡到橙色并逐渐透明，以便观察操作习惯。"));
    hintLabel->setWordWrap(true);

    auto *trailWidget = new TrajectoryWidget;
    trailWidget->setStyleSheet("background: white;");

    auto *layout = new QVBoxLayout;
    layout->addWidget(hintLabel);
    layout->addWidget(trailWidget, 1);

    window.setLayout(layout);
    window.show();

    return app.exec();
}
