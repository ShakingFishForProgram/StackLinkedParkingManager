#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStyle>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , park(nullptr)
    , parkCreated(false)
{
    ui->setupUi(this);
    this->resize(1000, 700);
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            QGuiApplication::primaryScreen()->availableGeometry()
        )
    );

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
        "QListWidget {"
        "  font-size: 24px;"
        "}"
        "QListWidget::item {"
        "  padding: 10px;"
        "}"
        "QTextEdit {"
        "  background-color: #F5F5F5;"
        "  border: 1px solid #ddd;"
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
        "  min-width: 960px;"
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

    ui->textAbout->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textAbout->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->tableCarInfo->setAlternatingRowColors(true);
    ui->tableCarInfo->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableCarInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableCarInfo->verticalHeader()->setDefaultSectionSize(50);
    ui->tableCarInfo->horizontalHeader()->setDefaultSectionSize(250);

    ui->btnAboutBack->setStyleSheet("QPushButton { margin-top: 40px; background-color: #808080; color: white; font-size: 18px; }");
    ui->btnConfirmAddTime->setStyleSheet("QPushButton { background-color: #808080; color: white; font-size: 18px; }");
}

MainWindow::~MainWindow() {
    delete park;
    delete ui;
}

// ============ 首页 ============

void MainWindow::on_btnCreatePark_clicked() {
    ui->editCapacity->clear();
    ui->editPrice->clear();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btnAbout_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btnExit_clicked() {
    this->close();
}

// ============ 创建停车场 ============

void MainWindow::on_btnCreateConfirm_clicked() {
    QString capText = ui->editCapacity->text();
    QString priceText = ui->editPrice->text();

    if (capText.isEmpty() || priceText.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("请输入完整信息！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }

    bool capOk, priceOk;
    int capacity = capText.toInt(&capOk);
    double price = priceText.toDouble(&priceOk);

    if (!capOk || capacity <= 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("停车场容量必须为正整数！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }
    if (!priceOk || price < 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("单价必须为非负数！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }

    if (park) delete park;
    park = new ParkingStructure(capacity, price);
    parkCreated = true;

    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("成功");
        msgBox.setText("停车场创建成功！\n容量: " + QString::number(capacity) + "\n单价: " + QString::number(price) + " 元/小时");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(400);
        msgBox.setStyleSheet("QMessageBox { font-size: 24px; } QPushButton { min-width: 80px; min-height: 30px; font-size: 16px; background-color: #808080; color: white; }");
        msgBox.exec();
    }

    refreshManagePage();
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnCreateBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// ============ 系统说明 ============

void MainWindow::on_btnAboutBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

// ============ 管理页面 ============

void MainWindow::refreshManagePage() {
    if (!park) return;

    ui->listStack->clear();
    ui->listWaiting->clear();

    ParkingDualStackInterface& stackMsg = park->getStackMessage();
    for (int i = 0; i < stackMsg.size; i++) {
        ui->listStack->addItem(QString::fromStdString(stackMsg.container[i].id));
    }
    ui->labelStackStatus->setText(QString("容量: %1/%2").arg(stackMsg.size).arg(park->getStackMessage().size));

    ParkingDoublyLinkedListInterface& listMsg = park->getLinkedListMessage();
    for (int i = 0; i < listMsg.size; i++) {
        ui->listWaiting->addItem(QString::fromStdString(listMsg.container[i].id));
    }
    ui->labelWaitingStatus->setText(QString("等待: %1").arg(listMsg.size));
}

void MainWindow::on_btnManageBack_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnAddCar_clicked() {
    if (!park) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("请先创建停车场！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }

    bool ok;
    QString plate = QInputDialog::getText(this, "添加车辆", "请输入车牌号：", QLineEdit::Normal, "", &ok);
    if (!ok || plate.isEmpty()) return;

    Car newCar(plate.toStdString());
    park->insertPark(newCar);
    refreshManagePage();
}

void MainWindow::on_btnRemoveCar_clicked() {
    if (!park) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("请先创建停车场！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }

    bool ok;
    QString plate = QInputDialog::getText(this, "删除车辆", "请输入要删除的车牌号：", QLineEdit::Normal, "", &ok);
    if (!ok || plate.isEmpty()) return;

    Car leftCar("");
    if (park->leavePark(plate.toStdString(), leftCar)) {
        time_t parkingTime = leftCar.getPakingTime();
        double fee = parkingTime * park->getUnitPrice() / 3600.0;
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("车辆离场");
            msgBox.setText(QString("车辆 %1 已离场\n停车时间: %2 秒\n费用: %3 元")
                .arg(QString::fromStdString(leftCar.getId()))
                .arg(parkingTime)
                .arg(fee, 0, 'f', 2));
            msgBox.setIcon(QMessageBox::NoIcon);
            msgBox.setMinimumWidth(600);
            msgBox.setStyleSheet("QMessageBox { font-size: 24px; } QPushButton { min-width: 60px; min-height: 25px; font-size: 14px; background-color: #808080; color: white; }");
            msgBox.exec();
        }
        refreshManagePage();
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("未找到该车辆！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
    }
}

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

// ============ 车辆信息页面 ============

void MainWindow::refreshCarInfoPage() {
    if (!park) return;

    ParkingDualStackInterface& msg = park->getStackMessage();
    ui->tableCarInfo->setRowCount(msg.size);
    ui->tableCarInfo->verticalHeader()->setDefaultSectionSize(50);

    for (int i = 0; i < msg.size; i++) {
        QTableWidgetItem* item0 = new QTableWidgetItem(QString::fromStdString(msg.container[i].id));
        item0->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item1 = new QTableWidgetItem(QString::number(msg.container[i].totalTime) + "s");
        item1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(msg.container[i].price, 'f', 2) + "元");
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableCarInfo->setItem(i, 0, item0);
        ui->tableCarInfo->setItem(i, 1, item1);
        ui->tableCarInfo->setItem(i, 2, item2);
    }
}

void MainWindow::on_btnCarInfoBack_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnRefreshCarInfo_clicked() {
    refreshCarInfoPage();
}

void MainWindow::on_btnConfirmAddTime_clicked() {
    if (!park) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误");
        msgBox.setText("请先创建停车场！");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }

    int hour = ui->spinAddHour->value();
    int minute = ui->spinAddMinute->value();
    int second = ui->spinAddSecond->value();

    if (hour == 0 && minute == 0 && second == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("未增加任何时间。");
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        return;
    }

    park->addExtraTime(second, minute, hour);
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("成功");
        msgBox.setText(QString("已增加 %1 时 %2 分 %3 秒").arg(hour).arg(minute).arg(second));
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setMinimumWidth(400);
        msgBox.setStyleSheet("QMessageBox { font-size: 24px; } QPushButton { min-width: 60px; min-height: 25px; font-size: 14px; background-color: #808080; color: white; }");
        msgBox.exec();
    }
    refreshCarInfoPage();
}
