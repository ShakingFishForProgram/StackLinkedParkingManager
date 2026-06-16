# 停车场管理系统

数据结构课程设计 - 队列栈的综合应用

## 项目简介

模拟停车场管理。使用**共享双栈**（同一线性空间）管理停车场内车辆和临时让路车辆，使用**双向链表**管理便道（等待区）车辆。

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

## 核心功能

### 车辆入场
- 停车场不满 → 入栈，自动记录入场时间
- 停车场已满 → 入便道等待

### 车辆离场
- 在栈中：后面车辆让路（移入右栈）→ 目标车离场 → 恢复 → 便道第一辆车补入
- 在便道：直接删除，不收停车费

### 计费
- 按实际停车时间 + 调试追加时间计算
- 单价可设置，公式：`price = totalTime × unitPrice ÷ 3600`

### 调试功能
- 可手动追加若干小时的停车时间（全场同步）

### 信息查询
- 查询栈内车辆：车牌、累计时间、费用
- 查询便道车辆：车牌（时间和费用均为0）

## 控制台程序

`ConsoleParking` 是一个独立的控制台可执行文件，提供菜单式交互界面。

### 菜单选项

1. **Car Entry** - 车辆入场，输入车牌号
2. **Car Leave** - 车辆离场，输入车牌号，显示停车时间和费用
3. **Add Time** - 追加调试时间（时/分/秒）
4. **Show Parking Info** - 显示停车场和便道状态
5. **Set Unit Price** - 设置单价（元/小时）
6. **Exit** - 退出程序

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
│   └── ParkingStructure.h
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

需要 Qt6 和 MinGW。使用 CMake 配置和构建：

```cmd
cmake -B build
cmake --build build
```
