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
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(title);
    dialog->setMinimumSize(400, 90);
    dialog->setMaximumSize(400, 90);
    dialog->setStyleSheet("QDialog { font-family: 'YouYuan'; font-size: 20px; }");
    dialog->setSizeGripEnabled(false);
    dialog->setWindowFlags(dialog->windowFlags() & ~Qt::WindowMaximizeButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(15, 10, 15, 10);

    QLabel* label = new QLabel(text, dialog);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setStyleSheet("font-size: 18px; font-family: 'YouYuan';");
    layout->addWidget(label);

    layout->addStretch();

    QPushButton* okBtn = new QPushButton("OK", dialog);
    okBtn->setFixedSize(80, 32);
    okBtn->setStyleSheet("QPushButton { font-size: 16px; background-color: #808080; color: white; border-radius: 6px; }");
    QObject::connect(okBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    layout->addLayout(btnLayout);

    dialog->exec();
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
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(title);
    dialog->setFixedSize(300, 160);
    dialog->setStyleSheet("QDialog { font-family: 'YouYuan'; font-size: 16px; }");
    dialog->setSizeGripEnabled(false);
    dialog->setWindowFlags(dialog->windowFlags() & ~Qt::WindowMaximizeButtonHint);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(10, 10, 10, 10);

    QLabel* lbl = new QLabel(label, dialog);
    lbl->setStyleSheet("font-size: 20px; font-family: 'YouYuan';");
    layout->addWidget(lbl);

    layout->addSpacing(10);

    QLineEdit* edit = new QLineEdit(dialog);
    edit->setMinimumHeight(40);
    edit->setStyleSheet("font-size: 20px; padding: 8px; border: 1px solid #bbb; border-radius: 5px;");
    layout->addWidget(edit);

    layout->addStretch();

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* okBtn = new QPushButton("OK", dialog);
    okBtn->setFixedSize(100, 32);
    okBtn->setStyleSheet("QPushButton { font-size: 16px; background-color: #808080; color: white; border-radius: 6px; }");
    QObject::connect(okBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    QPushButton* cancelBtn = new QPushButton("Cancel", dialog);
    cancelBtn->setFixedSize(100, 32);
    cancelBtn->setStyleSheet("QPushButton { font-size: 16px; background-color: #808080; color: white; border-radius: 6px; }");
    QObject::connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);

    btnLayout->addWidget(okBtn);
    btnLayout->addSpacing(20);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    QString result;
    if (dialog->exec() == QDialog::Accepted) {
        result = edit->text();
    }
    delete dialog;
    return result;
}

// ============ 构造函数与析构函数 ============

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , park(nullptr)
    , parkCreated(false)
{
    ui->setupUi(this);
    this->resize(1000, 770);
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            QGuiApplication::primaryScreen()->availableGeometry()
        )
    );

    // ===== 全局样式表 =====
    this->setStyleSheet(
        "* { font-family: 'YouYuan'; }"
        "QPushButton {"
        "  font-size: 22px;"
        "  padding: 10px 25px;"
        "  border-radius: 8px;"
        "  background-color: #808080;"
        "  color: white;"
        "}"
        "QPushButton:hover {"
        "  background-color: #6B6B6B;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #5A5A5A;"
        "}"
        "QLineEdit, QSpinBox {"
        "  font-size: 18px;"
        "  padding: 8px 12px;"
        "  border: 1px solid #bbb;"
        "  border-radius: 5px;"
        "}"
        "QGroupBox {"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "}"
        "QTableWidget {"
        "  font-size: 18px;"
        "  background-color: #F5F5F5;"
        "  alternate-background-color: #EEEEEE;"
        "}"
        "QHeaderView::section {"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #E0E0E0;"
        "  color: black;"
        "}"
        "QTableWidget::item:hover {"
        "  background-color: #D0D0D0;"
        "}"
        "QListWidget {"
        "  font-size: 36px;"
        "}"
        "QListWidget::item {"
        "  padding: 10px;"
        "}"
        "QTextEdit {"
        "  background-color: #F5F5F5;"
        "  border: 1px solid #ddd;"
        "  border-radius: 10px;"
        "  font-size: 16px;"
        "}"
        "QMessageBox {"
        "  font-size: 18px;"
        "}"
        "QMessageBox QPushButton {"
        "  min-width: 60px;"
        "  min-height: 25px;"
        "  font-size: 14px;"
        "}"
        "QInputDialog {"
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

    ui->stackedWidget->setCurrentIndex(0);

    // 系统说明页：取消滚动条
    ui->textAbout->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textAbout->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 车辆信息表格：交替行颜色、无选中、行高
    ui->tableCarInfo->setAlternatingRowColors(true);
    ui->tableCarInfo->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableCarInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableCarInfo->verticalHeader()->setDefaultSectionSize(50);
    ui->tableCarInfo->horizontalHeader()->setDefaultSectionSize(250);

    ui->btnAboutBack->setStyleSheet("QPushButton { margin-top: 40px; background-color: #808080; color: white; font-size: 18px; }");
    ui->btnConfirmAddTime->setStyleSheet("QPushButton { background-color: #808080; color: white; font-size: 18px; }");

    ui->spinAddHour->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinAddMinute->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinAddSecond->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->spinSetPrice->setButtonSymbols(QAbstractSpinBox::NoButtons);

    ui->btnSetPrice->setStyleSheet("QPushButton { background-color: #808080; color: white; font-size: 18px; }");
}

MainWindow::~MainWindow() {
    delete park;
    delete ui;
}

// ============ 首页 ============

/*
 * 点击"创建停车场"按钮：切换到创建停车场页面
 */
void MainWindow::on_btnCreatePark_clicked() {
    ui->editParkName->clear();
    ui->editCapacity->clear();
    ui->editPrice->clear();
    ui->stackedWidget->setCurrentIndex(1);
}

/*
 * 点击"系统说明"按钮：切换到系统说明页面
 */
void MainWindow::on_btnAbout_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

/*
 * 点击"退出程序"按钮：关闭主窗口
 */
void MainWindow::on_btnExit_clicked() {
    this->close();
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
        if (park) delete park;
        park = new ParkingStructure(capacity, unitPrice);
        parkCreated = true;
        parkName = name;

        // 遍历导入车辆
        for (const auto& carData : cars) {
            Car car(carData.plate);
            car.setExtraTime(carData.totalTime);
            park->insertPark(car);
        }

        showSuccessDialog(this, "成功", QString("停车场导入成功！\n名称: %1\n容量: %2\n单价: %3 元/小时\n已导入 %4 辆车")
            .arg(QString::fromStdString(name))
            .arg(capacity)
            .arg(unitPrice)
            .arg((int)cars.size()));

        refreshManagePage();
        ui->stackedWidget->setCurrentIndex(3);
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
    int capacity = capText.toInt(&capOk);
    double price = priceText.toDouble(&priceOk);

    if (!capOk || capacity <= 0) {
        showSuccessDialog(this, "错误", "停车场容量必须为正整数！");
        return;
    }
    if (!priceOk || price < 0) {
        showSuccessDialog(this, "错误", "单价必须为非负数！");
        return;
    }

    if (park) delete park;
    park = new ParkingStructure(capacity, price);
    parkCreated = true;
    parkName = ui->editParkName->text().trimmed().toStdString();
    if (parkName.empty()) parkName = "我的停车场";

    {
        showSuccessDialog(this, "成功", "停车场创建成功！\n容量: " + QString::number(capacity) + "\n单价: " + QString::number(price) + " 元/小时");
    }

    refreshManagePage();
    ui->stackedWidget->setCurrentIndex(3);
}

/*
 * 点击"返回首页"按钮：回到首页
 */
void MainWindow::on_btnCreateBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// ============ 系统说明 ============

void MainWindow::on_btnAboutBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// ============ 管理页面 ============

/*
 * 刷新管理页面：更新栈内车辆、便道车辆列表和停车场名称
 */
void MainWindow::refreshManagePage() {
    if (!park) return;

    ui->listStack->clear();
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
    ui->stackedWidget->setCurrentIndex(0);
}

/*
 * 添加车辆：弹出输入框获取车牌号，然后调用 insertPark 入场
 */
void MainWindow::on_btnAddCar_clicked() {
    if (!park) {
        showSuccessDialog(this, "错误", "请先创建停车场！");
        return;
    }

    QString plate = showInputDialog(this, "添加车辆", "请输入车牌号：");
    if (plate.isEmpty()) return;
    string newCarId = plate.toStdString();
    if (park->isExist(newCarId)) {
        showSuccessDialog(this, "警告", "该汽车已经存在！");
        return;
    }

    Car newCar(newCarId);
    park->insertPark(newCar);
    refreshManagePage();
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
    if (park->leavePark(plate.toStdString(), leftCar)) {
        time_t parkingTime = leftCar.getPakingTime();
        double fee = parkingTime * park->getUnitPrice() / 3600.0;
        {
            showSuccessDialog(this, "车辆离场", QString("车辆 %1 已离场\n停车时间: %2 秒\n费用: %3 元")
                                                        .arg(QString::fromStdString(leftCar.getId()))
                                                        .arg(parkingTime)
                                                        .arg(fee, 0, 'f', 2));
        }
        refreshManagePage();
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
    refreshCarInfoPage();
    ui->stackedWidget->setCurrentIndex(4);
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
 */
void MainWindow::refreshCarInfoPage() {
    if (!park) return;

    ParkingDualStackInterface& msg = park->getStackMessage();
    ui->tableCarInfo->setRowCount(msg.size);
    ui->tableCarInfo->verticalHeader()->setDefaultSectionSize(50);

    for (int i = 0; i < msg.size; i++) {
        QTableWidgetItem* item0 = new QTableWidgetItem(QString::fromStdString(msg.container[i].id));
        item0->setTextAlignment(Qt::AlignCenter);
        int totalSec = msg.container[i].totalTime;
        int hours = totalSec / 3600;
        int mins = (totalSec % 3600) / 60;
        int secs = totalSec % 60;
        QString timeStr;
        if (hours > 0)
            timeStr = QString("%1时%2分%3秒").arg(hours).arg(mins).arg(secs);
        else if (mins > 0)
            timeStr = QString("%1分%2秒").arg(mins).arg(secs);
        else
            timeStr = QString("%1秒").arg(secs);
        QTableWidgetItem* item1 = new QTableWidgetItem(timeStr);
        item1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(msg.container[i].price, 'f', 2) + "元");
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableCarInfo->setItem(i, 0, item0);
        ui->tableCarInfo->setItem(i, 1, item1);
        ui->tableCarInfo->setItem(i, 2, item2);
    }

    // 显示停车场名称
    ui->labelParkNameFooter->setText(QString("停车场: %1").arg(QString::fromStdString(parkName)));

    ui->spinAddHour->setValue(0);
    ui->spinAddMinute->setValue(0);
    ui->spinAddSecond->setValue(0);
    ui->spinSetPrice->setValue(1.00);
}

/*
 * 点击"返回管理"按钮：回到管理页面
 */
void MainWindow::on_btnCarInfoBack_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
}

/*
 * 点击"刷新"按钮：重新加载车辆信息表格数据
 */
void MainWindow::on_btnRefreshCarInfo_clicked() {
    refreshCarInfoPage();
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

    if (hour == 0 && minute == 0 && second == 0) {
        showSuccessDialog(this, "提示", "未增加任何时间。");
        return;
    }

    park->addExtraTime(second, minute, hour);
    {
        showSuccessDialog(this, "成功", QString("已增加 %1 时 %2 分 %3 秒").arg(hour).arg(minute).arg(second));
    }
    refreshCarInfoPage();
    ui->spinAddHour->setValue(0);
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
    park->setUnitPrice(newPrice);
    showSuccessDialog(this, "成功", QString("单价已修改为 %1 元/小时").arg(newPrice));
    refreshCarInfoPage();
    ui->spinSetPrice->setValue(1.00);
}
