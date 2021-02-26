#include "traveling.h"
#include "ui_traveling.h"
#include <QMessageBox>
#include <QDebug>

traveling::traveling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::traveling)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timefunc()));
    timer->start(1000);
}

traveling::~traveling()
{
    delete ui;
}

void traveling::timefunc()
{
    if(startup == 1)
    {
        if (time_limit >= 0 && distance_limit >= 0)
        {

            time_limit = time_limit - 1;
            distance_limit = distance_limit - 11;
            if(time_limit < 0 || distance_limit < 0)
            {
                    QMessageBox::about(this,"警告","您已超出约定的时间或距离限制！您的信誉积分将被扣除一部分");
                    qDebug() << "1";
            }
            else
            {
                QString remaining_time_sec, remaining_time_min, remaining_time_hour;
                remaining_time_sec = QString("%1").arg((time_limit%60), 2, 10, QLatin1Char('0'));
                remaining_time_min = QString("%1").arg((((time_limit - time_limit%60)/60)%60), 2, 10, QLatin1Char('0'));
                remaining_time_hour = QString("%1").arg((((time_limit - time_limit%60)/60 - ((time_limit - time_limit%60)/60)%60)/60), 2, 10, QLatin1Char('0'));
                ui->remaining_time->display(remaining_time_hour + ":" + remaining_time_min + ":" + remaining_time_sec);
                ui->remaining_distance->display(QString::number(distance_limit) + "m");
            }
        }
    }
}

