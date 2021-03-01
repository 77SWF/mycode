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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_systempage
{
public:
    QPushButton *set_config;
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
    QPushButton *set_config_2;
    QFrame *line_2;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QTableView *dbview;

    void setupUi(QWidget *systempage)
    {
        if (systempage->objectName().isEmpty())
            systempage->setObjectName(QStringLiteral("systempage"));
        systempage->resize(1600, 626);
        systempage->setStyleSheet(QString::fromUtf8("background-image: url(:/new/prefix1/\350\203\214\346\231\2572.jpg);"));
        set_config = new QPushButton(systempage);
        set_config->setObjectName(QStringLiteral("set_config"));
        set_config->setGeometry(QRect(310, 640, 51, 28));
        layoutWidget = new QWidget(systempage);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 20, 441, 461));
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

        set_config_2 = new QPushButton(systempage);
        set_config_2->setObjectName(QStringLiteral("set_config_2"));
        set_config_2->setGeometry(QRect(180, 520, 51, 28));
        line_2 = new QFrame(systempage);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(500, 20, 20, 471));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        layoutWidget1 = new QWidget(systempage);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(551, 21, 981, 461));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        dbview = new QTableView(layoutWidget1);
        dbview->setObjectName(QStringLiteral("dbview"));
        dbview->setStyleSheet(QStringLiteral(""));
        dbview->setAlternatingRowColors(false);
        dbview->horizontalHeader()->setCascadingSectionResizes(false);
        dbview->verticalHeader()->setCascadingSectionResizes(true);

        gridLayout_2->addWidget(dbview, 1, 0, 1, 1);


        retranslateUi(systempage);

        QMetaObject::connectSlotsByName(systempage);
    } // setupUi

    void retranslateUi(QWidget *systempage)
    {
        systempage->setWindowTitle(QApplication::translate("systempage", "\347\263\273\347\273\237\347\256\241\347\220\206\345\271\263\345\217\260", Q_NULLPTR));
        set_config->setText(QApplication::translate("systempage", "\350\256\276\347\275\256", Q_NULLPTR));
        label->setText(QApplication::translate("systempage", "\346\227\245\345\277\227\346\226\207\344\273\266\357\274\232", Q_NULLPTR));
        file_name_input->setPlaceholderText(QApplication::translate("systempage", "_.txt", Q_NULLPTR));
        read_file->setText(QApplication::translate("systempage", "\350\257\273\345\217\226", Q_NULLPTR));
        label_3->setText(QApplication::translate("systempage", "\346\234\200\344\275\216\344\277\241\350\252\211\347\247\257\345\210\206\351\234\200\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("systempage", "ECU_1-MPU\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_4->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_5->setText(QApplication::translate("systempage", "\345\217\257\346\233\264\346\226\260\347\232\204ECU\347\232\204ID\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("systempage", "\345\217\257\344\277\241\344\273\273\346\234\215\345\212\241\345\231\250ip\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("systempage", "\346\212\245\346\226\207\346\234\200\345\244\247\350\266\205\346\227\266\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
        config_3->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_7->setText(QApplication::translate("systempage", "\345\217\257\344\277\241\344\273\273\345\215\207\347\272\247\345\214\205\344\270\213\345\217\221\346\234\215\345\212\241\345\231\250ip\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        config_5->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_6->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_8->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_12->setText(QApplication::translate("systempage", "ECU_2-MPU\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("systempage", "ECU_1-MCU-APP_2\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_7->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_9->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_10->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_9->setText(QApplication::translate("systempage", "ECU_1\345\217\257\346\233\264\346\226\260\345\272\224\347\224\250id\357\274\232", Q_NULLPTR));
        label_13->setText(QApplication::translate("systempage", "ECU_2\345\217\257\346\233\264\346\226\260\345\272\224\347\224\250id\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("systempage", "ECU_1-MCU-APP_1\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_12->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        label_14->setText(QApplication::translate("systempage", "ECU_2-MCU-APP_1\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        label_15->setText(QApplication::translate("systempage", "ECU_2-MCU-APP_2\345\217\257\345\205\274\345\256\271\347\211\210\346\234\254\357\274\232", Q_NULLPTR));
        config_13->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        config_11->setPlaceholderText(QApplication::translate("systempage", "\344\273\245\"/\"\345\210\206\351\232\224", Q_NULLPTR));
        set_config_2->setText(QApplication::translate("systempage", "\350\256\276\347\275\256", Q_NULLPTR));
        label_2->setText(QApplication::translate("systempage", "\347\263\273\347\273\237\346\227\245\345\277\227\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class systempage: public Ui_systempage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMPAGE_H
