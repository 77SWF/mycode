#ifndef TRAVELING_H
#define TRAVELING_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

namespace Ui {
class traveling;
}

class traveling : public QMainWindow
{
    Q_OBJECT

public:
    explicit traveling(QWidget *parent = nullptr);
    ~traveling();
    int time_limit;
    int distance_limit;
    bool startup;

private:
    Ui::traveling *ui;
    QTimer *timer;

private slots:
    void timefunc();
};

#endif // TRAVELING_H
