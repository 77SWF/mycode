#include "systempage.h"
#include "ui_systempage.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include<iostream>
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <time.h>
#include <qsqlquerymodel.h>

#define BUFLEN 255

using namespace std;
char config_sys[13][50]=  {"85","60","1","218.66.204.16/4.2.123.2","1.2.123.2/10.22.123.11/202.108.22.5",
                       "1.0/2.0","1","1.0/2.0","1.0/2.0",
                      "1.0/2.0","1","1.0/2.0","1.0/2.0",};

systempage::systempage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::systempage)
{
    ui->setupUi(this);
    ui->config_1->setValue(atoi(config_sys[0]));
    ui->config_2->setValue(atoi(config_sys[1]));
    ui->config_2->setSuffix("s");
    ui->config_3->setText(config_sys[2]);
    ui->config_4->setText(config_sys[3]);
    ui->config_5->setText(config_sys[4]);
    ui->config_6->setText(config_sys[5]);
    ui->config_7->setText(config_sys[6]);
    ui->config_8->setText(config_sys[7]);
    ui->config_9->setText(config_sys[8]);
    ui->config_10->setText(config_sys[9]);
    ui->config_11->setText(config_sys[10]);
    ui->config_12->setText(config_sys[11]);
    ui->config_13->setText(config_sys[12]);
}

systempage::~systempage()
{
    delete ui;
}

//写存进数据库
void systempage::on_read_file_clicked()
{
    //加载数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","system_log");
    qDebug() << db.connectionName();
    db.setDatabaseName("system_log.db");
    qDebug() << db.databaseName();

    QFile file(ui->file_name_input->text());
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        QMessageBox::about(this,"错误","请输入正确的文本文件名!");
    }
    else
    {
        //打开数据库
        if(!db.open())
        {
            qDebug() << "open fail.";
            qDebug() << db.lastError().text();
        }

        //打开成功执行sql
        QString sql =
                "create table if not exists logs("
                "system_log varchar(500),time varchar(30)"
                ");";

        QSqlQuery query(db);//访问数据库
        if(!query.exec(sql))//创建表
        {
           qDebug() << query.lastError().text();
        }

        //txt所有文本的字符串
        QString tem_str(file.readAll());
        QByteArray tem = tem_str.toLatin1();

        //日期时间字符串datetime
        time_t t = time( nullptr );
        char datetime[BUFLEN];
        strftime(datetime, BUFLEN, "%Y-%m-%d %H:%M:%S", localtime(&t));


        query.prepare("INSERT INTO logs(system_log,time)"
                      "VALUES(:system_log,:time)");
        query.bindValue(":system_log",tem_str);
        query.bindValue(":time",datetime);
        query.exec();
    }

    QSqlQueryModel *qmodel;
        qmodel = new QSqlQueryModel();
        qmodel->setQuery("select * from logs", db);
        ui->dbview->setModel(qmodel);
        ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        //ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->dbview->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


//配置存进txt文件
void systempage::on_set_config_2_clicked()
{

    /* 分割数组 */

    //服务器ip地址
    char delims_config[] = "/";
    char tem_config_3[50];
    strcpy(tem_config_3,config_sys[3]);
    char *tem_server_ip = strtok( config_sys[3], delims_config );
    char server_ip_list[10][20]=  {{0}};
    int j_1 = 0;
    while( tem_server_ip != nullptr )
    {
        //printf("ip = %s \n",tem_server_ip);
        strcpy(server_ip_list[j_1],tem_server_ip);
        j_1++;
        tem_server_ip = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[3],tem_config_3);

    //可信任升级包下发服务器ip地址
    //char *tem_config_4 = {nullptr};
    //strcpy(tem_config_4,config_sys[4]);
    char tem_config_4[50];
    strcpy(tem_config_4,config_sys[4]);
    char *tem_up_server_ip = strtok( config_sys[4], delims_config );
    char up_server_ip_list[10][20]=  {{0}};
    int j_2 = 0;
    while( tem_up_server_ip != nullptr )
    {
        //printf("ip = %s \n",tem_up_server_ip);
        strcpy(up_server_ip_list[j_2],tem_up_server_ip);
        j_2++;
        tem_up_server_ip = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[4],tem_config_4);

    //可更新ECU的ID
    char tem_config_2[50];
    strcpy(tem_config_2,config_sys[2]);
    char *tem_updata_ECU_ID = strtok( config_sys[2], delims_config );
    char updata_ECU_ID[4][10]=  {{0}};
    int j_3 = 0;
    while( tem_updata_ECU_ID != nullptr )
    {
        strcpy(updata_ECU_ID[j_3],tem_updata_ECU_ID);
        j_3++;
        tem_updata_ECU_ID = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[2],tem_config_2);

    //ECU_1-MPU可兼容版本
    char tem_config_5[50];
    strcpy(tem_config_5,config_sys[5]);
    char *tem_ECU_1_MPU = strtok( config_sys[5], delims_config );
    char ECU_1_MPU_list[10][20]=  {{0}};
    int j_4 = 0;
    while( tem_ECU_1_MPU != nullptr )
    {
        strcpy(ECU_1_MPU_list[j_4],tem_ECU_1_MPU);
        j_4++;
        tem_ECU_1_MPU = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[5],tem_config_5);


    //ECU_2-MPU可兼容版本
    char tem_config_9[50];
    strcpy(tem_config_9,config_sys[9]);
    char *tem_ECU_2_MPU = strtok( config_sys[9], delims_config );
    char ECU_2_MPU_list[10][20]=  {{0}};
    int j_5 = 0;
    while( tem_ECU_2_MPU != nullptr )
    {
        strcpy(ECU_2_MPU_list[j_5],tem_ECU_2_MPU);
        j_5++;
        tem_ECU_2_MPU = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[9],tem_config_9);


    //ECU_1可更新应用id
    char tem_config_6[50];
    strcpy(tem_config_6,config_sys[6]);
    char *tem_ECU_1_APP_id = strtok( config_sys[6], delims_config );
    char ECU_1_APP_id[2][2]=  {{0}};
    int j_6 = 0;
    while( tem_ECU_1_APP_id != nullptr )
    {
        strcpy(ECU_1_APP_id[j_6],tem_ECU_1_APP_id);
        j_6++;
        tem_ECU_1_APP_id = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[6],tem_config_6);

    //ECU_2可更新应用id
    char tem_config_10[50];
    strcpy(tem_config_10,config_sys[10]);
    char *tem_ECU_2_APP_id = strtok( config_sys[10], delims_config );
    char ECU_2_APP_id[2][2]=  {{0}};
    int j_7 = 0;
    while( tem_ECU_2_APP_id != nullptr )
    {
        strcpy(ECU_2_APP_id[j_7],tem_ECU_2_APP_id);
        j_7++;
        tem_ECU_2_APP_id = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[10],tem_config_10);

    //ECU_1-MCU-APP_1可兼容版本
    char tem_config_7[50];
    strcpy(tem_config_7,config_sys[7]);
    char *tem_ECU1_APP1 = strtok( config_sys[7], delims_config );
    char ECU1_APP1[10][20]=  {{0}};
    int j_8 = 0;
    while( tem_ECU1_APP1 != nullptr )
    {
        strcpy(ECU1_APP1[j_8],tem_ECU1_APP1);
        j_8++;
        tem_ECU1_APP1 = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[7],tem_config_7);

    //ECU_1-MCU-APP_2可兼容版本
    char tem_config_8[50];
    strcpy(tem_config_8,config_sys[8]);
    char *tem_ECU1_APP2 = strtok( config_sys[8], delims_config );
    char ECU1_APP2[10][20]=  {{0}};
    int j_9 = 0;
    while( tem_ECU1_APP2 != nullptr )
    {
        strcpy(ECU1_APP2[j_9],tem_ECU1_APP2);
        j_9++;
        tem_ECU1_APP2 = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[8],tem_config_8);

    //ECU_2-MCU-APP_1可兼容版本
    char tem_config_11[50];
    strcpy(tem_config_11,config_sys[11]);
    char *tem_ECU2_APP1 = strtok( config_sys[11], delims_config );
    char ECU2_APP1[10][20]=  {{0}};
    int j_10 = 0;
    while( tem_ECU2_APP1 != nullptr )
    {
        strcpy(ECU2_APP1[j_10],tem_ECU2_APP1);
        j_10++;
        tem_ECU2_APP1 = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[11],tem_config_11);

    //ECU_2-MCU-APP_2可兼容版本
    char tem_config_12[50];
    strcpy(tem_config_12,config_sys[12]);
    char *tem_ECU2_APP2 = strtok( config_sys[12], delims_config );
    char ECU2_APP2[10][20]=  {{0}};
    int j_11 = 0;
    while( tem_ECU2_APP2 != nullptr )
    {
        strcpy(ECU2_APP2[j_11],tem_ECU2_APP2);
        j_11++;
        tem_ECU2_APP2 = strtok( nullptr, delims_config );
    }
    strcpy(config_sys[12],tem_config_12);


    /* 更改数组 */

    char *contents0, *contents1;
    contents0 = new char;
    contents1 = new char;
    sprintf(contents0, "%d", ui->config_1->value());
    sprintf(contents1, "%d", ui->config_2->value());
    char *contents2 = ui->config_3->text().toUtf8().data();
    char *contents3 = ui->config_4->text().toUtf8().data();
    char *contents4 = ui->config_5->text().toUtf8().data();
    char *contents5 = ui->config_6->text().toUtf8().data();
    char *contents6 = ui->config_7->text().toUtf8().data();
    char *contents7 = ui->config_8->text().toUtf8().data();
    char *contents8 = ui->config_9->text().toUtf8().data();
    char *contents9 = ui->config_10->text().toUtf8().data();
    char *contents10 = ui->config_11->text().toUtf8().data();
    char *contents11 = ui->config_12->text().toUtf8().data();
    char *contents12 = ui->config_13->text().toUtf8().data();
    strcpy(config_sys[0], contents0);
    strcpy(config_sys[1], contents1);
    strcpy(config_sys[2], contents2);
    strcpy(config_sys[3], contents3);
    strcpy(config_sys[4], contents4);
    strcpy(config_sys[5], contents5);
    strcpy(config_sys[6], contents6);
    strcpy(config_sys[7], contents7);
    strcpy(config_sys[8], contents8);
    strcpy(config_sys[9], contents9);
    strcpy(config_sys[10], contents10);
    strcpy(config_sys[11], contents11);
    strcpy(config_sys[12], contents12);
    QMessageBox::about(this,"成功","设置成功");
    qDebug() << config_sys[0];

    /* 创建txt，存储数组 */

    FILE *fspointer;
    fspointer = fopen("student1.txt", "w");
    if(fspointer==nullptr) return;
    for(int i=0;i<20;i++)
        fprintf(fspointer,"%s\n",config_sys[i]);

    fclose(fspointer);

}

