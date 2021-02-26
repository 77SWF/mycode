/********************************************************************************
** Form generated from reading UI file 'traveling.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAVELING_H
#define UI_TRAVELING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_traveling
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label;
    QLCDNumber *remaining_time;
    QLabel *label_2;
    QLCDNumber *remaining_distance;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *traveling)
    {
        if (traveling->objectName().isEmpty())
            traveling->setObjectName(QStringLiteral("traveling"));
        traveling->resize(709, 420);
        traveling->setStyleSheet(QString::fromUtf8("background-image: url(:/new/prefix1/\350\203\214\346\231\257.jpg);"));
        centralwidget = new QWidget(traveling);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 30, 601, 321));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QStringLiteral("font: 20pt \"Arial\";"));
        label_3->setTextFormat(Qt::AutoText);

        gridLayout->addWidget(label_3, 0, 0, 1, 2);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setStyleSheet(QStringLiteral("font: 18pt \"Arial\";"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        remaining_time = new QLCDNumber(widget);
        remaining_time->setObjectName(QStringLiteral("remaining_time"));
        remaining_time->setDigitCount(8);

        gridLayout->addWidget(remaining_time, 1, 1, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setStyleSheet(QStringLiteral("font: 18pt \"Arial\";"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        remaining_distance = new QLCDNumber(widget);
        remaining_distance->setObjectName(QStringLiteral("remaining_distance"));
        remaining_distance->setDigitCount(8);

        gridLayout->addWidget(remaining_distance, 2, 1, 1, 1);

        traveling->setCentralWidget(centralwidget);
        menubar = new QMenuBar(traveling);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 709, 17));
        traveling->setMenuBar(menubar);
        statusbar = new QStatusBar(traveling);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        traveling->setStatusBar(statusbar);

        retranslateUi(traveling);

        QMetaObject::connectSlotsByName(traveling);
    } // setupUi

    void retranslateUi(QMainWindow *traveling)
    {
        traveling->setWindowTitle(QApplication::translate("traveling", "\347\247\237\350\275\246\346\227\266\351\227\264/\350\267\235\347\246\273\350\256\241\346\225\260", Q_NULLPTR));
        label_3->setText(QApplication::translate("traveling", "\346\202\250\346\255\243\345\234\250\344\275\277\347\224\250\346\261\275\350\275\246......", Q_NULLPTR));
        label->setText(QApplication::translate("traveling", "\345\211\251\344\275\231\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("traveling", "\345\211\251\344\275\231\350\267\235\347\246\273(m)\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class traveling: public Ui_traveling {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAVELING_H
