/********************************************************************************
** Form generated from reading UI file 'homepage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMEPAGE_H
#define UI_HOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_homepage
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *fota_pb;
    QPushButton *key_pb;
    QPushButton *system_pb;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *homepage)
    {
        if (homepage->objectName().isEmpty())
            homepage->setObjectName(QStringLiteral("homepage"));
        homepage->resize(800, 450);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(homepage->sizePolicy().hasHeightForWidth());
        homepage->setSizePolicy(sizePolicy);
        homepage->setMinimumSize(QSize(450, 0));
        homepage->setMaximumSize(QSize(800, 450));
        homepage->setStyleSheet(QStringLiteral("background-image: url(:/new/prefix1/home2.jpg);"));
        centralwidget = new QWidget(homepage);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 40, 461, 61));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(16);
        label->setFont(font);
        label->setLineWidth(0);
        label->setAlignment(Qt::AlignCenter);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(150, 140, 461, 111));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        fota_pb = new QPushButton(layoutWidget);
        fota_pb->setObjectName(QStringLiteral("fota_pb"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fota_pb->sizePolicy().hasHeightForWidth());
        fota_pb->setSizePolicy(sizePolicy1);
        fota_pb->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(fota_pb, 0, 0, 1, 1);

        key_pb = new QPushButton(layoutWidget);
        key_pb->setObjectName(QStringLiteral("key_pb"));
        key_pb->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(key_pb, 0, 1, 1, 1);

        system_pb = new QPushButton(layoutWidget);
        system_pb->setObjectName(QStringLiteral("system_pb"));
        system_pb->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(system_pb, 0, 2, 1, 1);

        homepage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(homepage);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 17));
        homepage->setMenuBar(menubar);
        statusbar = new QStatusBar(homepage);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        homepage->setStatusBar(statusbar);

        retranslateUi(homepage);

        QMetaObject::connectSlotsByName(homepage);
    } // setupUi

    void retranslateUi(QMainWindow *homepage)
    {
        homepage->setWindowTitle(QApplication::translate("homepage", "\350\275\246\350\275\275\347\275\221\345\205\263\347\256\241\347\220\206\345\271\263\345\217\260", Q_NULLPTR));
        label->setText(QApplication::translate("homepage", "\346\254\242\350\277\216\346\235\245\345\210\260\350\275\246\350\275\275\347\275\221\345\205\263\347\256\241\347\220\206\345\271\263\345\217\260\357\274\201", Q_NULLPTR));
        fota_pb->setText(QApplication::translate("homepage", "FOTA\347\256\241\347\220\206", Q_NULLPTR));
        key_pb->setText(QApplication::translate("homepage", "\346\227\240\347\272\277\345\274\200\351\224\201\347\256\241\347\220\206", Q_NULLPTR));
        system_pb->setText(QApplication::translate("homepage", "\347\263\273\347\273\237\347\256\241\347\220\206", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class homepage: public Ui_homepage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMEPAGE_H
