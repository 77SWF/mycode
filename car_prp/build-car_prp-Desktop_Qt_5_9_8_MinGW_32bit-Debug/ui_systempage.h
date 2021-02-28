/********************************************************************************
** Form generated from reading UI file 'systempage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMPAGE_H
#define UI_SYSTEMPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_systempage
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *systempage)
    {
        if (systempage->objectName().isEmpty())
            systempage->setObjectName(QStringLiteral("systempage"));
        systempage->resize(800, 600);
        menubar = new QMenuBar(systempage);
        menubar->setObjectName(QStringLiteral("menubar"));
        systempage->setMenuBar(menubar);
        centralwidget = new QWidget(systempage);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        systempage->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(systempage);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        systempage->setStatusBar(statusbar);

        retranslateUi(systempage);

        QMetaObject::connectSlotsByName(systempage);
    } // setupUi

    void retranslateUi(QMainWindow *systempage)
    {
        systempage->setWindowTitle(QApplication::translate("systempage", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class systempage: public Ui_systempage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMPAGE_H
