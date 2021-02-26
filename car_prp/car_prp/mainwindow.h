#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <traveling.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    traveling travel;

private slots:
    void on_read_file_clicked();



    void on_set_config_clicked();

    void on_success_result_db_clicked();

    void on_fail_result_db_clicked();

    void on_car_rental_record_clicked();

    void on_result_db_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
