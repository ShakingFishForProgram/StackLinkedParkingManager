# 队列栈的综合应用 - 停车场管理系统

## 项目简介

模拟停车场管理。使用**共享双栈**（同一线性空间）管理停车场内车辆和临时让路车辆，使用**双向链表**管理便道（等待区）车辆。

## 数据结构设计

| 数据结构 | 文件 | 说明 |
|---------|------|------|
| 共享双栈 | `DualStack.h` | 两个栈共用同一数组，左栈（停车场）+ 右栈（让路） |
| 双向链表 | `DoublyLinkedList.h` | 便道车辆管理，支持按索引增删查 |
| 车辆类 | `Car.h` | 车牌、入场/出场时间戳、调试追加时间 |

## 业务逻辑层

| 类 | 文件 | 功能 |
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

## 文件结构

```
StackLinkedParkingManager/
├── include/              # 头文件
│   ├── Car.h
│   ├── DualStack.h
│   ├── DoublyLinkedList.h
│   ├── ParkingDualStack.h
│   ├── ParkingDoublyLinkedList.h
│   ├── ParkingInterfaces.h
│   └── ParkingStructure.h
├── src/                  # Qt 主程序（待开发）
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   └── mainwindow.ui
├── test/                 # 测试文件
│   ├── testCar.cpp
│   ├── testDualStack.cpp
│   ├── testParkingDualStack.cpp
│   ├── testDoublyLinkedList.cpp
│   ├── testDoublyLinkedListCar.cpp
│   ├── testParkingStructure.cpp
│   └── testParkingMessage.cpp
├── CMakeLists.txt
└── README.md
```

## 测试覆盖

| 测试文件 | 测试内容 |
|---------|---------|
| testCar.cpp | Car 类基本功能 |
| testDualStack.cpp | 共享双栈操作（string） |
| testParkingDualStack.cpp | 停车场双栈（Car） |
| testDoublyLinkedList.cpp | 双向链表操作（string） |
| testDoublyLinkedListCar.cpp | 双向链表操作（Car） |
| testParkingStructure.cpp | 停车场管理完整流程 |
| testParkingMessage.cpp | 信息查询 + 计费 + 车辆移除（20个测试） |
