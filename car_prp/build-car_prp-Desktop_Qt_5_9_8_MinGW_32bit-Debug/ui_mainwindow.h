/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *set_config;
    QFrame *line;
    QFrame *line_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *config_1;
    QLineEdit *file_name_input;
    QPushButton *read_file;
    QLabel *label_3;
    QLabel *label_8;
    QLineEdit *config_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_4;
    QLineEdit *config_3;
    QLabel *label_7;
    QLineEdit *config_5;
    QLineEdit *config_6;
    QSpinBox *config_2;
    QLineEdit *config_8;
    QLabel *label_12;
    QLabel *label_11;
    QLineEdit *config_7;
    QLineEdit *config_9;
    QLineEdit *config_10;
    QLabel *label_9;
    QLabel *label_13;
    QLabel *label_10;
    QLineEdit *config_12;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *config_13;
    QLineEdit *config_11;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QPushButton *result_db;
    QPushButton *success_result_db;
    QPushButton *fail_result_db;
    QPushButton *car_rental_record;
    QTableView *dbview;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1600, 626);
        MainWindow->setStyleSheet(QString::fromUtf8("background-image: url(:/new/prefix1/\350\203\214\346\231\257.jpg);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        set_config = new QPushButton(centralWidget);
        set_config->setObjectName(QStringLiteral("set_config"));
        set_config->setGeometry(QRect(220, 520, 51, 28));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(550, 520, 961, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(520, 20, 20, 471));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 20, 441, 461));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        config_1 = new QSpinBox(layoutWidget);
        config_1->setObjectName(QStringLiteral("config_1"));

        gridLayout->addWidget(config_1, 1, 1, 1, 1);

        file_name_input = new QLineEdit(layoutWidget);
        file_name_input->setObjectName(QStringLiteral("file_name_input"));

        gridLayout->addWidget(file_name_input, 0, 1, 1, 1);

        read_file = new QPushButton(layoutWidget);
        read_file->setObjectName(QStringLiteral("read_file"));

        gridLayout->addWidget(read_file, 0, 2, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 6, 0, 1, 1);

        config_4 = new QLineEdit(layoutWidget);
        config_4->setObjectName(QStringLiteral("config_4"));

        gridLayout->addWidget(config_4, 4, 1, 1, 2);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        config_3 = new QLineEdit(layoutWidget);
        config_3->setObjectName(QStringLiteral("config_3"));

        gridLayout->addWidget(config_3, 3, 1, 1, 2);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        config_5 = new QLineEdit(layoutWidget);
        config_5->setObjectName(QStringLiteral("config_5"));

        gridLayout->addWidget(config_5, 5, 1, 1, 2);

        config_6 = new QLineEdit(layoutWidget);
        config_6->setObjectName(QStringLiteral("config_6"));

        gridLayout->addWidget(config_6, 6, 1, 1, 2);

        config_2 = new QSpinBox(layoutWidget);
        config_2->setObjectName(QStringLiteral("config_2"));

        gridLayout->addWidget(config_2, 2, 1, 1, 1);

        config_8 = new QLineEdit(layoutWidget);
        config_8->setObjectName(QStringLiteral("config_8"));

        gridLayout->addWidget(config_8, 8, 1, 1, 2);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 10, 0, 1, 1);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 9, 0, 1, 1);

        config_7 = new QLineEdit(layoutWidget);
        config_7->setObjectName(QStringLiteral("config_7"));

        gridLayout->addWidget(config_7, 7, 1, 1, 2);

        config_9 = new QLineEdit(layoutWidget);
        config_9->setObjectName(QStringLiteral("config_9"));

        gridLayout->addWidget(config_9, 9, 1, 1, 2);

        config_10 = new QLineEdit(layoutWidget);
        config_10->setObjectName(QStringLiteral("config_10"));

        gridLayout->addWidget(config_10, 10, 1, 1, 2);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 7, 0, 1, 1);

        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout->addWidget(label_13, 11, 0, 1, 1);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 8, 0, 1, 1);

        config_12 = new QLineEdit(layoutWidget);
        config_12->setObjectName(QStringLiteral("config_12"));

        gridLayout->addWidget(config_12, 12, 1, 1, 2);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout->addWidget(label_14, 12, 0, 1, 1);

        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout->addWidget(label_15, 13, 0, 1, 1);

        config_13 = new QLineEdit(layoutWidget);
        config_13->setObjectName(QStringLiteral("config_13"));

        gridLayout->addWidget(config_13, 13, 1, 1, 2);

        config_11 = new QLineEdit(layoutWidget);
        config_11->setObjectName(QStringLiteral("config_11"));

        gridLayout->addWidget(config_11, 11, 1, 1, 2);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(570, 20, 971, 461));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        result_db = new QPushButton(layoutWidget1);
        result_db->setObjectName(QStringLiteral("result_db"));

        gridLayout_2->addWidget(result_db, 0, 1, 1, 1);

        success_result_db = new QPushButton(layoutWidget1);
        success_result_db->setObjectName(QStringLiteral("success_result_db"));

        gridLayout_2->addWidget(success_result_db, 0, 2, 1, 1);

        fail_result_db = new QPushButton(layoutWidget1);
        fail_result_db->setObjectName(QStringLiteral("fail_result_db"));

        gridLayout_2->addWidget(fail_result_db, 0, 3, 1, 1);

        car_rental_record = new QPushButton(layoutWidget1);
        car_rental_record->setObjectName(QStringLiteral("car_rental_record"));

        gridLayout_2->addWidget(car_rental_record, 0, 4, 1, 1);

        dbview = new QTableView(layoutWidget1);
        dbview->setObjectName(QStringLiteral("dbview"));
        dbview->setStyleSheet(QStringLiteral(""));
        dbview->setAlternatingRowColors(false);
        dbview->horizontalHeader()->setCascadingSectionResizes(false);
        dbview->verticalHeader()->setCascadingSectionResizes(true);

        gridLayout_2->addWidget(dbview, 1, 0, 1, 5);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1600, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\350\275\246\350\275\275\347\275\221\345\205\263\347\256\241\347\220\206\345\271\263\345\217\260", Q_NULLPTR));
        set_config->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\346\212\245\346\226\207\346\226\207\344\273\266\345\220\215\357\274\232", Q_NULLPTR));
        file_name_input->setPlaceholderText(QApplication::translate("MainWindow", "_.txt", Q_NULLPTR));
        read_file->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\346\234\200\344\275\216\344\277\241\350\252\211\347\247\257\345\210\206\351\234\200\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "ECU_1-MPU\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_4->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\345\217\257\346\233\264\346\226\260\347\232\204ECU\347\232\204ID\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\345\217\257\344\277\241\344\273\273\346\234\215\345\212\241\345\231\250ip\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\346\212\245\346\226\207\346\234\200\345\244\247\350\266\205\346\227\266\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
        config_3->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\345\217\257\344\277\241\344\273\273\345\215\207\347\272\247\345\214\205\344\270\213\345\217\221\346\234\215\345\212\241\345\231\250ip\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        config_5->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_6->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_8->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "ECU_2-MPU\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "ECU_1-MCU-APP_2\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_7->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_9->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_10->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "ECU_1\345\217\257\346\233\264\346\226\260\345\272\224\347\224\250id\357\274\232", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "ECU_2\345\217\257\346\233\264\346\226\260\345\272\224\347\224\250id\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "ECU_1-MCU-APP_1\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_12->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "ECU_2-MCU-APP_1\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "ECU_2-MCU-APP_2\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_13->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_11->setPlaceholderText(QApplication::translate("MainWindow", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\345\256\241\350\256\241\344\277\241\346\201\257\357\274\232", Q_NULLPTR));
        result_db->setText(QApplication::translate("MainWindow", "all", Q_NULLPTR));
        success_result_db->setText(QApplication::translate("MainWindow", "success", Q_NULLPTR));
        fail_result_db->setText(QApplication::translate("MainWindow", "fail", Q_NULLPTR));
        car_rental_record->setText(QApplication::translate("MainWindow", "car_rental_record", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
