#include "homepage.h"
#include "ui_homepage.h"

homepage::homepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::homepage)
{
    ui->setupUi(this);
}

homepage::~homepage()
{
    delete ui;
}

void homepage::on_fota_pb_clicked()
{
    fota_window.show();
}

void homepage::on_system_pb_clicked()
{
    system_window.show();
}

void homepage::on_key_pb_clicked()
{

}
