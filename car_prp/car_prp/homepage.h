#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include "mainwindow.h"
#include "systempage.h"
namespace Ui {
class homepage;
}

class homepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit homepage(QWidget *parent = nullptr);
    ~homepage();
    MainWindow fota_window;
    systempage system_window;

private slots:
    void on_fota_pb_clicked();

    void on_key_pb_clicked();

    void on_system_pb_clicked();

private:
    Ui::homepage *ui;
};

#endif // HOMEPAGE_H
