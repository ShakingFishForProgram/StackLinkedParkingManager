// 停车场管理系统 - 主窗口实现
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QStyle>
#include <QScreen>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "ParkingFileManager.h"

// ============ 自定义对话框 ============

/*
 * 通用成功/错误/提示弹窗，固定大小，灰色 OK 按钮
 * @param parent  父窗口指针
 * @param title   弹窗标题
 * @param text    弹窗显示文本内容
 */
static void showSuccessDialog(QWidget* parent, const QString& title, const QString& text) {
    QDialog* dialog = new QDialog(parent);                      // 创建自定义弹窗
    dialog->setWindowTitle(title);
    dialog->setMinimumSize(400, 90);                            // 固定弹窗大小
    dialog->setMaximumSize(400, 90);
    dialog->setStyleSheet("QDialog { font-family: 'YouYuan'; font-size: 20px; }");
    dialog->setSizeGripEnabled(false);                          // 禁止拖动调整大小
    dialog->setWindowFlags(dialog->windowFlags() & ~Qt::WindowMaximizeButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QVBoxLayout* layout = new QVBoxLayout(dialog);              // 垂直布局
    layout->setContentsMargins(15, 10, 15, 10);

    QLabel* label = new QLabel(text, dialog);                   // 显示文本内容
    label->setWordWrap(true);                                   // 自动换行
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setStyleSheet("font-size: 18px; font-family: 'YouYuan';");
    layout->addWidget(label);

    layout->addStretch();                                       // 弹性空间，将按钮推到底部

    QPushButton* okBtn = new QPushButton("是", dialog);         // 灰色确认按钮
    okBtn->setFixedSize(80, 32);
    okBtn->setStyleSheet("QPushButton { font-size: 12px; background-color: #808080; color: white; border-radius: 6px; }");
    QObject::connect(okBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    QHBoxLayout* btnLayout = new QHBoxLayout();                 // 按钮水平居中
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    layout->addLayout(btnLayout);

    dialog->exec();                                             // 模态显示
    delete dialog;
}

/*
 * 通用输入弹窗，返回用户输入的车牌号，取消返回空字符串
 * @param parent  父窗口指针
 * @param title   弹窗标题
 * @param label   输入框上方的提示文字
 * @return        用户输入的车牌号，取消返回空字符串
 */
static QString showInputDialog(QWidget* parent, const QString& title, const QString& label) {
    QDialog* dialog = new QDialog(parent);                      // 创建自定义输入弹窗
    dialog->setWindowTitle(title);
    dialog->setFixedSize(300, 160);                             // 固定大小
    dialog->setStyleSheet("QDialog { font-family: 'YouYuan'; font-size: 16px; }");
    dialog->setSizeGripEnabled(false);
    dialog->setWindowFlags(dialog->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(10, 10, 10, 10);

    QLabel* lbl = new QLabel(label, dialog);                    // 提示文字
    lbl->setStyleSheet("font-size: 20px; font-family: 'YouYuan';");
    layout->addWidget(lbl);

    layout->addSpacing(10);

    QLineEdit* edit = new QLineEdit(dialog);                    // 输入框
    edit->setMinimumHeight(40);
    edit->setStyleSheet("font-size: 20px; padding: 8px; border: 1px solid #bbb; border-radius: 5px;");
    layout->addWidget(edit);

    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout();                 // 按钮水平布局
    btnLayout->addStretch();

    QPushButton* okBtn = new QPushButton("是", dialog);         // 确认按钮
    okBtn->setFixedSize(100, 32);
    okBtn->setStyleSheet("QPushButton { font-size: 12px; background-color: #808080; color: white; border-radius: 6px; }");
    QObject::connect(okBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    QPushButton* cancelBtn = new QPushButton("取消", dialog); // 取消按钮
    cancelBtn->setFixedSize(100, 32);
    cancelBtn->setStyleSheet("QPushButton { font-size: 12px; background-color: #808080; color: white; border-radius: 6px; }");
    QObject::connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);

    btnLayout->addWidget(okBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    QString result;
    if (dialog->exec() == QDialog::Accepted) {                  // 用户点击 OK 时获取输入内容
        result = edit->text();
    }
    delete dialog;
    return result;
}

// ============ 构造函数与析构函数 ============

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , park(nullptr)                                             // 初始无停车场
    , parkCreated(false)
{
    ui->setupUi(this);                                          // 加载 UI 布局
    this->resize(1000, 770);                                    // 设置窗口大小
    this->setGeometry(                                          // 窗口居中显示
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            QGuiApplication::primaryScreen()->availableGeometry()
        )
    );

    // ===== 全局样式表 =====
    this->setStyleSheet(
        "* { font-family: 'YouYuan'; }"                         // 全局字体：幼圆
        "QPushButton {"                                         // 按钮样式
        "  font-size: 22px;"
        "  padding: 10px 25px;"
        "  border-radius: 8px;"
        "  background-color: #808080;"
        "  color: white;"
        "}"
        "QPushButton:hover {"                                   // 悬停变深灰
        "  background-color: #6B6B6B;"
        "}"
        "QPushButton:pressed {"                                 // 按下变更深灰
        "  background-color: #5A5A5A;"
        "}"
        "QLineEdit, QSpinBox, QDoubleSpinBox {"                         // 输入框样式
        "  font-size: 18px;"
        "  padding: 8px 12px;"
        "  border: 1px solid #bbb;"
        "  border-radius: 5px;"
        "}"
        "QGroupBox {"                                           // 分组框样式
        "  font-size: 20px;"
        "  font-weight: bold;"
        "}"
        "QTableWidget {"                                        // 表格样式：交替行颜色
        "  font-size: 18px;"
        "  background-color: #F5F5F5;"
        "  alternate-background-color: #EEEEEE;"
        "}"
        "QHeaderView::section {"                                // 表头样式
        "  font-size: 20px;"
        "  font-weight: bold;"
        "}"
        "QTableWidget::item:selected {"                         // 选中行样式
        "  background-color: #E0E0E0;"
        "  color: black;"
        "}"
        "QTableWidget::item:hover {"                            // 悬停行样式
        "  background-color: #D0D0D0;"
        "}"
        "QListWidget {"                                         // 列表大字号
        "  font-size: 36px;"
        "}"
        "QListWidget::item {"
        "  padding: 10px;"
        "}"
        "QTextEdit {"                                           // 系统说明区域样式
        "  background-color: #F5F5F5;"
        "  border: 1px solid #ddd;"
        "  border-radius: 10px;"
        "  font-size: 16px;"
        "}"
        "QMessageBox {"                                         // 消息弹窗字号
        "  font-size: 18px;"
        "}"
        "QMessageBox QPushButton {"
        "  min-width: 60px;"
        "  min-height: 25px;"
        "  font-size: 14px;"
        "}"
        "QInputDialog {"                                        // 输入弹窗宽度
        "  min-width: 480px;"
        "}"
        "QInputDialog QLabel {"
        "  font-size: 18px;"
        "}"
        "QInputDialog QPushButton {"
        "  min-width: 50px;"
        "  min-height: 20px;"
        "  font-size: 14px;"
        "}"
    );

    ui->stackedWidget->setCurrentIndex(0);                      // 默认显示首页

    // 系统说明页：取消滚动条
    ui->textAbout->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textAbout->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 车辆信息表格：交替行颜色、无选中、行高
    ui->tableCarInfo->setAlternatingRowColors(true);
    ui->tableCarInfo->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableCarInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableCarInfo->verticalHeader()->setDefaultSectionSize(50);   // 行高 50px
    ui->tableCarInfo->horizontalHeader()->setDefaultSectionSize(250); // 列宽 250px

    // 单独设置特定按钮的样式（覆盖全局样式）
    ui->btnAboutBack->setStyleSheet("QPushButton { margin-top: 40px; background-color: #808080; color: white; font-size: 18px; }");
    ui->btnConfirmAddTime->setStyleSheet("QPushButton { background-color: #808080; color: white; font-size: 18px; }");

    // 隐藏数字输入框的上下箭头按钮
    ui->spinAddHour->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinAddMinute->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinAddSecond->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinSetPrice->setButtonSymbols(QAbstractSpinBox::NoButtons);

    ui->btnSetPrice->setStyleSheet("QPushButton { background-color: #808080; color: white; font-size: 18px; }");

    // 返回按钮：浅灰色、无圆角
    ui->btnCarInfoBack->setStyleSheet("QPushButton { background-color: #D0D0D0; color: white; font-size: 11px; border-radius: 0px; }");

    // 定时器：每秒刷新车辆详情页表格
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshCarInfoPage);
}

MainWindow::~MainWindow() {
    delete park;                                                // 释放停车场对象
    delete ui;                                                  // 释放 UI 对象
}

// ============ 首页 ============

/*
 * 点击"创建停车场"按钮：切换到创建停车场页面
 */
void MainWindow::on_btnCreatePark_clicked() {
    ui->editParkName->clear();                                  // 清空输入框
    ui->editCapacity->clear();
    ui->editPrice->clear();
    ui->stackedWidget->setCurrentIndex(1);                      // 切换到创建页面
}

/*
 * 点击"系统说明"按钮：切换到系统说明页面
 */
void MainWindow::on_btnAbout_clicked() {
    ui->stackedWidget->setCurrentIndex(2);                      // 切换到系统说明页
}

/*
 * 点击"退出程序"按钮：关闭主窗口
 */
void MainWindow::on_btnExit_clicked() {
    this->close();                                              // 关闭窗口
}

/*
 * 点击"导入停车场"按钮：选择 JSON 文件并导入
 */
void MainWindow::on_btnImportPark_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "导入停车场", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    std::string name;
    int capacity;
    double unitPrice;
    std::vector<ImportCarData> cars;

    if (ParkingFileManager::importFromJson(filePath.toStdString(), name, capacity, unitPrice, cars)) {
        if (park) delete park;                                  // 销毁旧停车场
        park = new ParkingStructure(capacity, unitPrice);       // 创建新停车场
        parkCreated = true;
        parkName = name;

        // 遍历导入车辆，恢复历史总时间并按顺序入场
        for (const auto& carData : cars) {
            Car car(carData.plate);
            car.setExtraTime(carData.totalTime);                // 恢复历史总时间
            park->insertPark(car);                              // 入场
        }

        showSuccessDialog(this, "成功", QString("停车场导入成功！\n名称: %1\n容量: %2\n单价: %3 元/小时\n已导入 %4 辆车")
            .arg(QString::fromStdString(name))
            .arg(capacity)
            .arg(unitPrice)
            .arg((int)cars.size()));

        refreshManagePage();                                    // 刷新管理页面
        ui->stackedWidget->setCurrentIndex(3);                  // 切换到管理页面
    } else {
        showSuccessDialog(this, "错误", "文件格式错误或无法读取！");
    }
}

// ============ 创建停车场 ============

/*
 * 点击"确认创建"按钮：校验输入并创建 ParkingStructure 对象
 * 校验规则：容量必须为正整数，单价必须为非负数
 */
void MainWindow::on_btnCreateConfirm_clicked() {
    QString capText = ui->editCapacity->text();
    QString priceText = ui->editPrice->text();

    // 输入检查
    if (capText.isEmpty() || priceText.isEmpty()) {
        showSuccessDialog(this, "错误", "请输入完整信息！");
        return;
    }

    bool capOk, priceOk;
    int capacity = capText.toInt(&capOk);                       // 将字符串转为整数
    double price = priceText.toDouble(&priceOk);                // 将字符串转为浮点数

    if (!capOk || capacity <= 0) {                              // 容量检查
        showSuccessDialog(this, "错误", "停车场容量必须为正整数！");
        return;
    }
    if (!priceOk || price < 0) {                                // 价格检查
        showSuccessDialog(this, "错误", "单价必须为非负数！");
        return;
    }

    if (park) delete park;                                      // 销毁旧的停车场对象
    park = new ParkingStructure(capacity, price);               // 根据输入创建新停车场
    parkCreated = true;
    parkName = ui->editParkName->text().trimmed().toStdString();
    if (parkName.empty()) parkName = "我的停车场";               // 名称为空时使用默认名

    {
        showSuccessDialog(this, "成功", "停车场创建成功！\n容量: " + QString::number(capacity) + "\n单价: " + QString::number(price) + " 元/小时");
    }

    refreshManagePage();                                        // 刷新管理页面
    ui->stackedWidget->setCurrentIndex(3);                      // 切换到管理页面
}

/*
 * 点击"返回首页"按钮：回到首页
 */
void MainWindow::on_btnCreateBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);                      // 切换到首页
}

// ============ 系统说明 ============

void MainWindow::on_btnAboutBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);                      // 切换到首页
}

// ============ 管理页面 ============

/*
 * 刷新管理页面：更新栈内车辆、便道车辆列表和停车场名称
 */
void MainWindow::refreshManagePage() {
    if (!park) return;

    ui->listStack->clear();                                     // 清空列表
    ui->listWaiting->clear();

    // 更新停车场名称显示
    ui->labelParkNameDisplay->setText(QString::fromStdString(parkName));

    // 刷新栈内车辆列表
    ParkingDualStackInterface& stackMsg = park->getStackMessage();
    for (int i = 0; i < stackMsg.size; i++) {
        ui->listStack->addItem(QString::fromStdString(stackMsg.container[i].id));
    }
    ui->labelStackStatus->setText(QString("容量: %1/%2").arg(stackMsg.size).arg(park->getSize()));

    // 刷新便道车辆列表
    ParkingDoublyLinkedListInterface& listMsg = park->getLinkedListMessage();
    for (int i = 0; i < listMsg.size; i++) {
        ui->listWaiting->addItem(QString::fromStdString(listMsg.container[i].id));
    }
    ui->labelWaitingStatus->setText(QString("等待: %1").arg(listMsg.size));
}

void MainWindow::on_btnManageBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);                      // 切换到首页
}

/*
 * 添加车辆：弹出输入框获取车牌号，然后调用 insertPark 入场
 */
void MainWindow::on_btnAddCar_clicked() {
    if (!park) {
        showSuccessDialog(this, "错误", "请先创建停车场！");
        return;
    }

    QString plate = showInputDialog(this, "添加车辆", "请输入车牌号：");  // 弹出输入框
    if (plate.isEmpty()) return;
    string newCarId = plate.toStdString();
    if (park->isExist(newCarId)) {                              // 检查车牌号是否已存在
        showSuccessDialog(this, "警告", "该汽车已经存在！");
        return;
    }

    Car newCar(newCarId);
    park->insertPark(newCar);                                   // 执行入场
    refreshManagePage();                                        // 刷新管理页面
}

/*
 * 删除车辆：弹出输入框获取车牌号，调用 leavePark 尝试离场
 * 离场成功后显示停车时间和费用
 */
void MainWindow::on_btnRemoveCar_clicked() {
    if (!park) {
        showSuccessDialog(this, "错误", "请先创建停车场！");
        return;
    }

    QString plate = showInputDialog(this, "删除车辆", "请输入要删除的车牌号：");
    if (plate.isEmpty()) return;

    Car leftCar("");
    if (park->leavePark(plate.toStdString(), leftCar)) {        // 执行离场
        time_t parkingTime = leftCar.getPakingTime();           // 获取停车时间
        double fee = parkingTime * park->getUnitPrice() / 3600.0;  // 计算费用
        // 格式化时间显示：<1分钟显示秒，<1小时显示分秒，≥1小时显示时分秒
        int pHours = parkingTime / 3600;
        int pMins = (parkingTime % 3600) / 60;
        int pSecs = parkingTime % 60;
        QString pTimeStr;
        if (pHours > 0)
            pTimeStr = QString("%1时%2分%3秒").arg(pHours).arg(pMins).arg(pSecs);
        else if (pMins > 0)
            pTimeStr = QString("%1分%2秒").arg(pMins).arg(pSecs);
        else
            pTimeStr = QString("%1秒").arg(pSecs);
        {
            showSuccessDialog(this, "车辆离场", QString("车辆 %1 已离场\n停车时间: %2\n费用: %3 元")
                                                        .arg(QString::fromStdString(leftCar.getId()))
                                                        .arg(pTimeStr)
                                                        .arg(fee, 0, 'f', 2));
        }
        refreshManagePage();                                    // 刷新管理页面
    } else {
        showSuccessDialog(this, "错误", "未找到该车辆！");
    }
}

/*
 * 点击"车辆信息"按钮：切换到车辆信息页面并刷新表格
 */
void MainWindow::on_btnCarInfo_clicked() {
    if (!park) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("请先创建停车场！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }
    // 进入详情页时重置所有输入框
    ui->spinAddHour->setValue(0);
    ui->spinAddMinute->setValue(0);
    ui->spinAddSecond->setValue(0);
    ui->spinSetPrice->setValue(park->getUnitPrice());           // 单价输入框显示当前价格
    refreshCarInfoPage();                                       // 立即刷新表格
    refreshTimer->start(1000);                                  // 启动定时器，每秒刷新
    ui->stackedWidget->setCurrentIndex(4);                      // 切换到车辆详情页
}

/*
 * 点击"导出信息"按钮：将停车场数据导出为 JSON 文件
 */
void MainWindow::on_btnExportInfo_clicked() {
    if (!park) {
        showSuccessDialog(this, "错误", "请先创建停车场！");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "导出信息", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) return;

    if (ParkingFileManager::exportToJson(filePath.toStdString(), *park, parkName)) {
        showSuccessDialog(this, "成功", "停车场信息已导出！");
    } else {
        showSuccessDialog(this, "错误", "文件写入失败！");
    }
}

// ============ 车辆信息页面 ============

/*
 * 刷新车辆信息表格：从栈中获取最新数据并显示
 * 显示内容包括：车牌号、停车总时长、费用
 * 时间格式化：不足1分钟显示秒，不足1小时显示分秒，超过1小时显示时分秒
 */
void MainWindow::refreshCarInfoPage() {
    if (!park) return;

    ParkingDualStackInterface& msg = park->getStackMessage();   // 获取车辆信息
    ui->tableCarInfo->setRowCount(msg.size);                    // 设置表格行数
    ui->tableCarInfo->verticalHeader()->setDefaultSectionSize(50);

    for (int i = 0; i < msg.size; i++) {
        // 第1列：车牌号
        QTableWidgetItem* item0 = new QTableWidgetItem(QString::fromStdString(msg.container[i].id));
        item0->setTextAlignment(Qt::AlignCenter);

        // 第2列：停车总时长（格式化显示）
        int totalSec = msg.container[i].totalTime;              // 总秒数
        int hours = totalSec / 3600;                            // 取小时
        int mins = (totalSec % 3600) / 60;                      // 取分钟
        int secs = totalSec % 60;                               // 取秒
        QString timeStr;
        if (hours > 0)                                          // ≥1小时：X时Y分Z秒
            timeStr = QString("%1时%2分%3秒").arg(hours).arg(mins).arg(secs);
        else if (mins > 0)                                      // ≥1分钟：Y分Z秒
            timeStr = QString("%1分%2秒").arg(mins).arg(secs);
        else                                                    // <1分钟：Z秒
            timeStr = QString("%1秒").arg(secs);
        QTableWidgetItem* item1 = new QTableWidgetItem(timeStr);
        item1->setTextAlignment(Qt::AlignCenter);

        // 第3列：费用
        QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(msg.container[i].price, 'f', 2) + "元");
        item2->setTextAlignment(Qt::AlignCenter);

        ui->tableCarInfo->setItem(i, 0, item0);
        ui->tableCarInfo->setItem(i, 1, item1);
        ui->tableCarInfo->setItem(i, 2, item2);
    }

    // 显示停车场名称
    ui->labelParkNameFooter->setText(QString("停车场: %1").arg(QString::fromStdString(parkName)));
}

/*
 * 点击"返回管理"按钮：回到管理页面
 */
void MainWindow::on_btnCarInfoBack_clicked() {
    refreshTimer->stop();                                       // 停止定时器
    ui->stackedWidget->setCurrentIndex(3);                      // 切换到管理页面
}

/*
 * 点击"确认增加"按钮：给所有在场车辆追加调试时间
 * @note 时间单位由 addExtraTime 内部处理：hours=小时, minutes=分钟, seconds=秒
 */
void MainWindow::on_btnConfirmAddTime_clicked() {
    if (!park) {
        showSuccessDialog(this, "错误", "请先创建停车场！");
        return;
    }

    int hour = ui->spinAddHour->value();
    int minute = ui->spinAddMinute->value();
    int second = ui->spinAddSecond->value();

    if (hour == 0 && minute == 0 && second == 0) {              // 时间为0则不处理
        showSuccessDialog(this, "提示", "未增加任何时间。");
        return;
    }

    park->addExtraTime(second, minute, hour);                   // 执行调试加时
    {
        showSuccessDialog(this, "成功", QString("已增加 %1 时 %2 分 %3 秒").arg(hour).arg(minute).arg(second));
    }
    refreshCarInfoPage();                                       // 刷新表格
    ui->spinAddHour->setValue(0);                               // 重置加时输入框
    ui->spinAddMinute->setValue(0);
    ui->spinAddSecond->setValue(0);
}

/*
 * 点击"确认修改"按钮：修改单价
 */
void MainWindow::on_btnSetPrice_clicked() {
    if (!park) {
        showSuccessDialog(this, "错误", "请先创建停车场！");
        return;
    }

    double newPrice = ui->spinSetPrice->value();
    park->setUnitPrice(newPrice);                               // 设置新单价
    showSuccessDialog(this, "成功", QString("单价已修改为 %1 元/小时").arg(newPrice));
    refreshCarInfoPage();                                       // 刷新表格（费用重新计算）
    ui->spinSetPrice->setValue(park->getUnitPrice());           // 输入框显示新价格
}
