#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ParkingStructure.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnCreatePark_clicked();
    void on_btnAbout_clicked();
    void on_btnExit_clicked();

    void on_btnCreateConfirm_clicked();
    void on_btnCreateBack_clicked();

    void on_btnAboutBack_clicked();

    void on_btnManageBack_clicked();
    void on_btnAddCar_clicked();
    void on_btnRemoveCar_clicked();
    void on_btnCarInfo_clicked();

    void on_btnCarInfoBack_clicked();
    void on_btnRefreshCarInfo_clicked();
    void on_btnConfirmAddTime_clicked();

private:
    void refreshManagePage();
    void refreshCarInfoPage();

    Ui::MainWindow *ui;
    ParkingStructure *park;
    bool parkCreated;
};

#endif // MAINWINDOW_H
