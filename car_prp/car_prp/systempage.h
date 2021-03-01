#ifndef SYSTEMPAGE_H
#define SYSTEMPAGE_H

#include <QWidget>

namespace Ui {
class systempage;
}

class systempage : public QWidget
{
    Q_OBJECT

public:
    explicit systempage(QWidget *parent = nullptr);
    ~systempage();

private slots:
    void on_read_file_clicked();

    void on_set_config_2_clicked();

private:
    Ui::systempage *ui;
};

#endif // SYSTEMPAGE_H
