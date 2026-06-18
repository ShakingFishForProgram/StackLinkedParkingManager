# 停车场管理系统

数据结构课程设计 — 共享双栈与双向链表的综合应用

## 项目简介

模拟停车场管理。使用 **共享双栈（DualStack）** 管理停车场内车辆和临时让路车辆，使用 **双向链表（DoublyLinkedList）** 管理便道（等待区）车辆。

提供 **Qt6 GUI 程序** 和 **控制台程序** 两套交互方式。

## 数据结构

| 数据结构 | 文件 | 说明 |
|---------|------|------|
| 共享双栈 | `DualStack.h` | 两个栈共用同一数组：左栈（停车场）+ 右栈（让路） |
| 双向链表 | `DoublyLinkedList.h` | 便道车辆管理，支持按索引增删查 |
| 车辆类 | `Car.h` | 车牌号、入场/出场时间戳、调试追加时间 |

## 业务逻辑层

| 类 | 文件 | 说明 |
|---|------|------|
| `ParkingDualStack` | `ParkingDualStack.h` | 继承 DualStack，封装入场/离场时间记录、追加调试时间、信息查询 |
| `ParkingDoublyLinkedList` | `ParkingDoublyLinkedList.h` | 继承 DoublyLinkedList，按车牌查找、信息查询 |
| `ParkingStructure` | `ParkingStructure.h` | 顶层管理类，组合双栈 + 链表，提供统一接口 |
| `ParkingInterfaces` | `ParkingInterfaces.h` | 信息查询接口：CarInfo（车牌、时间、价格） |
| `ParkingFileManager` | `ParkingFileManager.h` | JSON 文件导入/导出停车场配置及车辆数据 |

## 核心功能

### 创建停车场
- 设置停车场名称、总容量、每小时单价
- 可从 JSON 文件导入现有停车场配置

### 导入/导出
- 导出为 JSON：停车场名称、容量、单价、栈内车辆（含总时间）、便道车辆
- 导入 JSON：恢复停车场配置，按顺序插入车辆并保留历史总时长

### 车辆入场
- 停车场不满 → 入栈，自动记录入场时间
- 停车场已满 → 入便道等待

### 车辆离场
- 在栈中：后面车辆让路（移入右栈）→ 目标车离场 → 恢复 → 便道第一辆车补入
- 在便道：直接删除，不收停车费

### 计费
- 按实际停车时间 + 调试追加时间计算
- 单价可随时修改，公式：`price = totalTime × unitPrice ÷ 3600`

### 调试加时
- 可对全场所有车辆手动追加若干小时、分钟、秒的停车时间

### 信息查询
- 栈内车辆列表：车牌号（管理页）
- 便道车辆列表：车牌号（管理页）
- 详细表格：车牌、停车总时长（自动格式化）、费用（车辆详情页，每秒自动刷新）

## Qt GUI 程序

主窗口使用 `QStackedWidget` 实现多页面切换，共 5 个页面。

### 首页
- **创建停车场**：跳转到创建页面
- **导入停车场**：选择 JSON 文件导入
- **系统说明**：查看功能介绍
- **退出程序**：关闭窗口

### 创建停车场
- 输入：停车场名称、容量（正整数）、初始单价（非负数）
- 确认创建后自动进入管理页面

### 系统说明
- 只读文本，介绍数据结构和核心功能

### 管理页面
- 显示停车场名称、栈内车辆列表（容量状态）、便道车辆列表
- 操作按钮：返回首页、添加车辆、删除车辆、车辆信息、导出信息
- 每个列表项显示车牌号，36px 大字号

### 车辆详情页面
- **标题行**：左侧「◀ 返回」按钮 + 居中「车辆详细信息」标题
- **表格**：车牌、停车总时长（不足1分钟显示秒，不足1小时显示分秒，超过1小时显示时分秒）、费用
- **增加额外时间**：时/分/秒三个输入框 + 确认按钮
- **修改单价**：单价输入框（默认显示当前价格）+ 确认按钮
- **自动刷新**：进入页面时启动 1 秒定时器，自动刷新表格数据；离开页面时停止
- 鼠标悬停表格行时高亮为浅灰色

### 自定义弹窗
- 使用自定义 `QDialog` 替代 `QMessageBox`/`QInputDialog`
- 精确控制弹窗大小、字体（幼圆）、按钮样式（灰色圆角）

### 全局样式
- 全局字体：幼圆（YouYuan）
- 按钮：灰色背景 `#808080`，白色文字，hover/pressed 变色
- 输入框：18px 字号，68px 高度，圆角边框
- 表格：交替行颜色、20px 表头加粗、18px 内容
- 列表：36px 大字号

## 控制台程序

`ConsoleParking` 是独立的控制台可执行文件，不依赖 Qt 库，提供菜单式交互界面。

### 菜单选项
1. **Car Entry** — 车辆入场，输入车牌号
2. **Car Leave** — 车辆离场，输入车牌号，显示停车时间和费用
3. **Add Time** — 追加调试时间（时/分/秒）
4. **Show Parking Info** — 显示停车场和便道状态
5. **Set Unit Price** — 设置单价（元/小时）
6. **Exit** — 退出程序

### 输入处理
- 所有整数输入使用 `getline` + `stringstream` 进行类型检查
- 输入非数字时提示重新输入，不会残留字符在缓冲区
- 车牌号使用 `getline` 读取，避免残留问题

### 运行方式
```cmd
cd build\Desktop_Qt_6_10_3_MinGW_64_bit_Debug
ConsoleParking.exe
```

## 文件结构

```
StackLinkedParkingManager/
├── include/                  # 头文件
│   ├── Car.h
│   ├── DualStack.h
│   ├── DoublyLinkedList.h
│   ├── ParkingDualStack.h
│   ├── ParkingDoublyLinkedList.h
│   ├── ParkingInterfaces.h
│   ├── ParkingStructure.h
│   └── ParkingFileManager.h
├── src/                      # 源文件
│   ├── console_main.cpp      # 控制台程序入口
│   ├── hello_qt.cpp          # Qt 测试程序
│   ├── main.cpp              # Qt GUI 程序入口
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   └── mainwindow.ui
├── test/                     # 测试文件
│   ├── test_cin.cpp
│   ├── testCar.cpp
│   ├── testDoublyLinkedList.cpp
│   ├── testDoublyLinkedListCar.cpp
│   ├── testDualStack.cpp
│   ├── testParkingDualStack.cpp
│   ├── testParkingMessage.cpp
│   └── testParkingStructure.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## 测试覆盖

| 测试文件 | 说明 |
|---------|------|
| test_cin.cpp | 控制台输入验证 |
| testCar.cpp | Car 类基本功能 |
| testDualStack.cpp | 共享双栈操作（string） |
| testParkingDualStack.cpp | 停车场双栈（Car） |
| testDoublyLinkedList.cpp | 双向链表操作（string） |
| testDoublyLinkedListCar.cpp | 双向链表操作（Car） |
| testParkingStructure.cpp | 停车场管理完整流程 |
| testParkingMessage.cpp | 信息查询 + 计费 + 车辆移除 |

## 构建

需要 **Qt6** 和 **MinGW**。使用 CMake 配置和构建：

```cmd
cd StackLinkedParkingManager
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

Qt GUI 程序使用 `qt_add_executable`，控制台程序 `ConsoleParking` 为独立目标（不链接 Qt 库）。

运行 Qt GUI：
```cmd
build\Desktop_Qt_6_10_3_MinGW_64_bit_Debug\StackLinkedParkingManager.exe
```
