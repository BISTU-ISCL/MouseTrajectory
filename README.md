# 鼠标轨迹实时绘制 Demo

## 功能概述
- 实时显示鼠标光标轨迹，保留最近 3 秒的移动路径。
- 轨迹使用从亮蓝到橙色的渐变，尾部逐渐透明，便于观察操作习惯。

## 开发环境
- Qt 6.10.0（Widgets 模块）
- MSVC 2022 64bit（Release 模式）
- Qt Creator 18.0.0
- CMake 构建（项目已启用 `AUTOMOC`/`AUTOUIC`/`AUTORCC`）

## 目录结构
- `CMakeLists.txt`：CMake 配置。
- `main.cpp`：入口程序，创建主窗口并布置控件。
- `trajectorywidget.h / trajectorywidget.cpp`：自定义控件，实现轨迹采样与绘制。

## 编译与运行
1. 在 Qt Creator 中选择 **“打开项目”**，加载本目录下的 `CMakeLists.txt`。
2. 选择 **MSVC 2022 64bit Release** Kit（确保包含 Qt 6.10.0）。
3. 点击 **Configure Project**，待 CMake 配置完成后构建并运行。
4. 运行后在白色区域内移动鼠标，即可看到最近 3 秒的渐变轨迹。

## 关键实现说明
- `TrajectoryWidget` 使用 `QElapsedTimer` 记录时间戳，并在 `mouseMoveEvent` 中采样鼠标位置。
- `QTimer` 每 16ms 触发刷新，移除超过 3 秒的老旧点并重绘。
- 在 `paintEvent` 中逐线段绘制，并依据时间计算颜色和透明度，实现渐变尾迹效果。

## 体验技巧
- 快速移动鼠标可以看到清晰的渐变带；
- 停留不动时，超过 3 秒的轨迹会自动淡出；
- 可修改 `trajectorywidget.cpp` 中的颜色或线宽，以适配不同展示需求。
