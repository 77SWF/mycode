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

char const *vin_id="LFPH4ACB411C02008";
char config[13][50]=  {"85","60","1","218.66.204.16/4.2.123.2","1.2.123.2/10.22.123.11/202.108.22.5",
                       "1.0/2.0","1","1.0/2.0","1.0/2.0",
                      "1.0/2.0","1","1.0/2.0","1.0/2.0",};
int t_connect=0,t_response=0,t_update=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->config_1->setValue(atoi(config[0]));
    ui->config_2->setValue(atoi(config[1]));
    ui->config_2->setSuffix("s");
    ui->config_3->setText(config[2]);
    ui->config_4->setText(config[3]);
    ui->config_5->setText(config[4]);
    ui->config_6->setText(config[5]);
    ui->config_7->setText(config[6]);
    ui->config_8->setText(config[7]);
    ui->config_9->setText(config[8]);
    ui->config_10->setText(config[9]);
    ui->config_11->setText(config[10]);
    ui->config_12->setText(config[11]);
    ui->config_13->setText(config[12]);
    travel.startup = 0;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_read_file_clicked()
{
    QFile file(ui->file_name_input->text());
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
        QMessageBox::about(this,"错误","请输入正确的文本文件名!");
    }

    //txt所有文本的字符串
    QString tem_str(file.readAll());
    QByteArray tem = tem_str.toLatin1();
    char *str = tem.data();


    char delims[] = "\n";
    char *result = strtok( str, delims );

    char msg[44][50]=  {{0}};
    int i = 0;

    // 报文信息msg数组
    while( result != nullptr )
    {
        strcpy(msg[i],result);
        i++;
        result = strtok( nullptr, delims );
    }

    //加载数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Record");
    qDebug() << db.connectionName();
    db.setDatabaseName("Record.db");
    qDebug() << db.databaseName();

    //打开数据库
    if(!db.open())
    {
        qDebug() << "open fail.";
        qDebug() << db.lastError().text();
    }

    //打开成功执行sql
    QString sql =
            "create table if not exists records("
            "result varchar(10),type_id varchar(10),content varchar(30),threat varchar(30),time varchar(30)"
            ");";

    QSqlQuery query(db);//访问数据库
    if(!query.exec(sql))//创建表
    {
       qDebug() << query.lastError().text();
    }


    //服务器ip地址
    char delims_config[] = "/";
    //char tem_config_3[20];
    //tem_config_3 =config[3];
    //前面：char config[13][50];
    char tem_config_3[50];
    strcpy(tem_config_3,config[3]);
    char *tem_server_ip = strtok( config[3], delims_config );
    char server_ip_list[10][20]=  {{0}};
    int j_1 = 0;
    while( tem_server_ip != nullptr )
    {
        //printf("ip = %s \n",tem_server_ip);
        strcpy(server_ip_list[j_1],tem_server_ip);
        j_1++;
        tem_server_ip = strtok( nullptr, delims_config );
    }
    strcpy(config[3],tem_config_3);

    //可信任升级包下发服务器ip地址
    //char *tem_config_4 = {nullptr};
    //strcpy(tem_config_4,config[4]);
    char tem_config_4[50];
    strcpy(tem_config_4,config[4]);
    char *tem_up_server_ip = strtok( config[4], delims_config );
    char up_server_ip_list[10][20]=  {{0}};
    int j_2 = 0;
    while( tem_up_server_ip != nullptr )
    {
        //printf("ip = %s \n",tem_up_server_ip);
        strcpy(up_server_ip_list[j_2],tem_up_server_ip);
        j_2++;
        tem_up_server_ip = strtok( nullptr, delims_config );
    }
    strcpy(config[4],tem_config_4);

    //可更新ECU的ID
    //char *tem_config_2 = {nullptr};
    //strcpy(tem_config_2,config[2]);
    char tem_config_2[50];
    strcpy(tem_config_2,config[2]);
    char *tem_updata_ECU_ID = strtok( config[2], delims_config );
    char updata_ECU_ID[4][10]=  {{0}};
    int j_3 = 0;
    while( tem_updata_ECU_ID != nullptr )
    {
        strcpy(updata_ECU_ID[j_3],tem_updata_ECU_ID);
        j_3++;
        tem_updata_ECU_ID = strtok( nullptr, delims_config );
    }
    strcpy(config[2],tem_config_2);

    //日期时间字符串datetime
    time_t t = time( nullptr );
    char datetime[BUFLEN];
    strftime(datetime, BUFLEN, "%Y-%m-%d %H:%M:%S", localtime(&t));

    //连接应答报文
    if(strncmp(msg[7],"0x01",50)==0)
    {
        //HMAC、vin_id
        if(strncmp(msg[21],msg[23],50)==0  && strncmp(msg[5],vin_id,50)==0)
        {
            //服务器ip
            for(int i1=0;i1<10;i1++)
            {
                if(strncmp(server_ip_list[i1],msg[3],20)==0)
                {
                    t_connect = atoi(msg[19]);
                    query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                  "VALUES(:result,:type_id,:content,:threat,:time)");
                    query.bindValue(":result","success");
                    query.bindValue(":type_id","0x01");
                    query.bindValue(":content","-");
                    query.bindValue(":threat","-");
                    query.bindValue(":time",datetime);
                    query.exec();
                    QMessageBox::about(this,"成功","该报文通过验证。");
                    break;
                }
                if(i1==9)
                {
                    t_connect = 0;
                    query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                  "VALUES(:result,:type_id,:content,:threat,:time)");
                    query.bindValue(":result","fail");
                    query.bindValue(":type_id","0x01");
                    query.bindValue(":content","服务器ip不在信任列表");
                    query.bindValue(":threat","恶意服务器威胁");
                    query.bindValue(":time",datetime);
                    query.exec();
                    QMessageBox::about(this,"拦截","该报文可疑！");
                }
            }
        }
        else 
        {
            t_connect = 0;
            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                          "VALUES(:result,:type_id,:content,:threat,:time)");
            query.bindValue(":result","fail");
            query.bindValue(":type_id","0x01");
            query.bindValue(":content","HMAC/vin_id验证不通过。");
            query.bindValue(":threat","身份验证未通过，禁止连接建立。");
            query.bindValue(":time",datetime);
            query.exec();
            QMessageBox::about(this,"拦截","该报文可疑！");
        }
    }

    //通信请求报文
    if(strncmp(msg[7],"0x02",50)==0)
    {
        //HMAC、vin_id
        if(strncmp(msg[21],msg[23],50)==0 && strncmp(msg[5],vin_id,50)==0)
        {
            //服务器ip信任列表
            for(int i2=0;i2<10;i2++)
            {
                //在
                if(strncmp(server_ip_list[i2],msg[3],20)==0)
                {
                    t_response = atoi(msg[19]);
                    //超时
                    if(t_response-t_connect > atoi(config[1]))
                    {
                        t_response = 0;
                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                        query.bindValue(":result","fail");
                        query.bindValue(":type_id","0x02");
                        query.bindValue(":content","连接超时");
                        query.bindValue(":threat","重放攻击");
                        query.bindValue(":time",datetime);
                        query.exec();
                        QMessageBox::about(this,"拦截","该报文可疑！");
                    }
                    //不超时
                    else
                    {
                        //ECU可更新列表
                        for(int b=0;b<4;b++)
                        {
                            //在
                            if(strncmp(updata_ECU_ID[b],msg[13],1)==0)
                            {
                                t_response = atoi(msg[19]);
                                query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                              "VALUES(:result,:type_id,:content,:threat,:time)");
                                query.bindValue(":result","success");
                                query.bindValue(":type_id","0x02");
                                query.bindValue(":content","-");
                                query.bindValue(":threat","-");
                                query.bindValue(":time",datetime);
                                query.exec();
                                QMessageBox::about(this,"成功","该报文通过验证。");
                                break;
                            }
                            //不在
                            if(b==3)
                            {
                                t_response = 0;
                                query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                              "VALUES(:result,:type_id,:content,:threat,:time)");
                                query.bindValue(":result","fail");
                                query.bindValue(":type_id","0x02");
                                query.bindValue(":content","该ECU禁止更新");
                                query.bindValue(":threat","ECU恶意刷新");
                                query.bindValue(":time",datetime);
                                query.exec();
                                QMessageBox::about(this,"拦截","该报文可疑！");
                            }
                        }
                    }
                    break;
                }
                //不在
                else if(i2==9)
                {
                    t_response = 0;
                    query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                  "VALUES(:result,:type_id,:content,:threat,:time)");
                    query.bindValue(":result","fail");
                    query.bindValue(":type_id","0x02");
                    query.bindValue(":content","服务器ip不在信任列表");
                    query.bindValue(":threat","恶意服务器威胁");
                    query.bindValue(":time",datetime);
                    query.exec();
                    QMessageBox::about(this,"拦截","该报文可疑！");
                }
            }
        }
        else
        {
            t_response = 0;
            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                          "VALUES(:result,:type_id,:content,:threat,:time)");
            query.bindValue(":result","fail");
            query.bindValue(":type_id","0x02");
            query.bindValue(":content","HMAC验证不通过。");
            query.bindValue(":threat","身份验证未通过，禁止连接建立。");
            query.bindValue(":time",datetime);
            query.exec();
            QMessageBox::about(this,"拦截","该报文可疑！");
        }
    }

    //升级任务，bug
    if(strncmp(msg[7],"0x82",50)==0)
    {
        //HMAC、vin_id
        if(strncmp(msg[21],msg[23],50)==0  && strncmp(msg[5],vin_id,50)==0)
        {
            //服务器ip
            for(int i3=0;i3<10;i3++)
            {
                //服务器ip可信
                if(strncmp(server_ip_list[i3],msg[3],20)==0)
                {
                    t_update = atoi(msg[19]);
                    //超时，pass
                    if(t_update-t_response>atoi(config[1]))
                    {
                        t_update = 0;
                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                        query.bindValue(":result","fail");
                        query.bindValue(":type_id","0x82");
                        query.bindValue(":content","连接超时");
                        query.bindValue(":threat","重放攻击");
                        query.bindValue(":time",datetime);
                        query.exec();
                        QMessageBox::about(this,"拦截","该报文可疑！");
                    }

                    //不超时
                    else
                    {
                        //下载地址前缀ip
                        char delims_address[] = "/";
                        char tem_msg_17[50];
                        strcpy(tem_msg_17,msg[17]);
                        //分割后首字符串
                        strtok( msg[17], delims_address );
                        //char *address = strtok( msg[17], delims_address );
                        //strcpy(msg[17],tem_msg_17);
                        //printf("ip=%s \n",msg[17]);

                        //升级包ip地址
                        //for里有bug
                        for(int c=0;c<10;c++)
                        {
                            printf("%s \n",up_server_ip_list[c]);
                            //在可信列表
                            if(strncmp(msg[17],up_server_ip_list[c],50)==0)
                            {
                                //printf("c=%d,ip=%s,add=%s",c,up_server_ip_list[c],msg[17]);
                                //ECU可更新列表
                                for(int d=0;d<10;d++)
                                {
                                    if(strncmp(updata_ECU_ID[d],msg[13],1)==0)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","success");
                                        query.bindValue(":type_id","0x82");
                                        query.bindValue(":content","-");
                                        query.bindValue(":threat","-");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"成功","该报文通过验证。");
                                        break;
                                    }
                                    if(d==9)
                                    {
                                        t_update = 0;
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","fail");
                                        query.bindValue(":type_id","0x82");
                                        query.bindValue(":content","该ECU禁止更新");
                                        query.bindValue(":threat","ECU恶意刷新");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"拦截","该报文可疑！");
                                    }
                                }
                                break;
                            }

                            //不在可信列表，bug
                            if(c==9)
                            {
                                t_update = 0;
                                query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                              "VALUES(:result,:type_id,:content,:threat,:time)");
                                query.bindValue(":result","fail");
                                query.bindValue(":type_id","0x82");
                                char *s={nullptr};
                                sprintf(s, "%d", 123);
                                query.bindValue(":content","非授权服务器尝试下发ECU升级包");
                                query.bindValue(":threat","ECU恶意刷新");
                                query.bindValue(":time",datetime);
                                query.exec();
                                QMessageBox::about(this,"拦截","该报文可疑！");
                            }
                        }

                    }

                    break;
                }
                //pass
                if(i3==9)
                {
                    t_update = 0;
                    query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                  "VALUES(:result,:type_id,:content,:threat,:time)");
                    query.bindValue(":result","fail");
                    query.bindValue(":type_id","0x82");
                    query.bindValue(":content","服务器ip不在信任列表");
                    query.bindValue(":threat","恶意服务器威胁");
                    query.bindValue(":time",datetime);
                    query.exec();
                    QMessageBox::about(this,"拦截","该报文可疑！");
                }
            }
        }
        else
        {
            t_update = 0;
            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                          "VALUES(:result,:type_id,:content,:threat,:time)");
            query.bindValue(":result","fail");
            query.bindValue(":type_id","0x82");
            query.bindValue(":content","HMAC/vin_id验证不通过。");
            query.bindValue(":threat","身份验证未通过，禁止连接建立。");
            query.bindValue(":time",datetime);
            query.exec();
            QMessageBox::about(this,"拦截","该报文可疑！");
        }
    }

    //升级包配置文件
    if(strncmp(msg[25],"-",5)!=0)
    {
        //ECU_1-MPU可兼容版本
        char tem_config_5[50];
        strcpy(tem_config_5,config[5]);
        char *tem_ECU_1_MPU = strtok( config[5], delims_config );
        char ECU_1_MPU_list[10][20]=  {{0}};
        int j_4 = 0;
        while( tem_ECU_1_MPU != nullptr )
        {
            strcpy(ECU_1_MPU_list[j_4],tem_ECU_1_MPU);
            j_4++;
            tem_ECU_1_MPU = strtok( nullptr, delims_config );
        }
        strcpy(config[5],tem_config_5);


        //ECU_2-MPU可兼容版本
        char tem_config_9[50];
        strcpy(tem_config_9,config[9]);
        char *tem_ECU_2_MPU = strtok( config[9], delims_config );
        char ECU_2_MPU_list[10][20]=  {{0}};
        int j_5 = 0;
        while( tem_ECU_2_MPU != nullptr )
        {
            strcpy(ECU_2_MPU_list[j_5],tem_ECU_2_MPU);
            j_5++;
            tem_ECU_2_MPU = strtok( nullptr, delims_config );
        }
        strcpy(config[9],tem_config_9);


        //ECU_1可更新应用id
        char tem_config_6[50];
        strcpy(tem_config_6,config[6]);
        char *tem_ECU_1_APP_id = strtok( config[6], delims_config );
        char ECU_1_APP_id[2][2]=  {{0}};
        int j_6 = 0;
        while( tem_ECU_1_APP_id != nullptr )
        {
            strcpy(ECU_1_APP_id[j_6],tem_ECU_1_APP_id);
            j_6++;
            tem_ECU_1_APP_id = strtok( nullptr, delims_config );
        }
        strcpy(config[6],tem_config_6);

        //ECU_2可更新应用id
        char tem_config_10[50];
        strcpy(tem_config_10,config[10]);
        char *tem_ECU_2_APP_id = strtok( config[10], delims_config );
        char ECU_2_APP_id[2][2]=  {{0}};
        int j_7 = 0;
        while( tem_ECU_2_APP_id != nullptr )
        {
            strcpy(ECU_2_APP_id[j_7],tem_ECU_2_APP_id);
            j_7++;
            tem_ECU_2_APP_id = strtok( nullptr, delims_config );
        }
        strcpy(config[10],tem_config_10);

        //ECU_1-MCU-APP_1可兼容版本
        char tem_config_7[50];
        strcpy(tem_config_7,config[7]);
        char *tem_ECU1_APP1 = strtok( config[7], delims_config );
        char ECU1_APP1[10][20]=  {{0}};
        int j_8 = 0;
        while( tem_ECU1_APP1 != nullptr )
        {
            strcpy(ECU1_APP1[j_8],tem_ECU1_APP1);
            j_8++;
            tem_ECU1_APP1 = strtok( nullptr, delims_config );
        }
        strcpy(config[7],tem_config_7);

        //ECU_1-MCU-APP_2可兼容版本
        char tem_config_8[50];
        strcpy(tem_config_8,config[8]);
        char *tem_ECU1_APP2 = strtok( config[8], delims_config );
        char ECU1_APP2[10][20]=  {{0}};
        int j_9 = 0;
        while( tem_ECU1_APP2 != nullptr )
        {
            strcpy(ECU1_APP2[j_9],tem_ECU1_APP2);
            j_9++;
            tem_ECU1_APP2 = strtok( nullptr, delims_config );
        }
        strcpy(config[8],tem_config_8);

        //ECU_2-MCU-APP_1可兼容版本
        char tem_config_11[50];
        strcpy(tem_config_11,config[11]);
        char *tem_ECU2_APP1 = strtok( config[11], delims_config );
        char ECU2_APP1[10][20]=  {{0}};
        int j_10 = 0;
        while( tem_ECU2_APP1 != nullptr )
        {
            strcpy(ECU2_APP1[j_10],tem_ECU2_APP1);
            j_10++;
            tem_ECU2_APP1 = strtok( nullptr, delims_config );
        }
        strcpy(config[11],tem_config_11);

        //ECU_2-MCU-APP_2可兼容版本
        char tem_config_12[50];
        strcpy(tem_config_12,config[12]);
        char *tem_ECU2_APP2 = strtok( config[12], delims_config );
        char ECU2_APP2[10][20]=  {{0}};
        int j_11 = 0;
        while( tem_ECU2_APP2 != nullptr )
        {
            strcpy(ECU2_APP2[j_11],tem_ECU2_APP2);
            j_11++;
            tem_ECU2_APP2 = strtok( nullptr, delims_config );
        }
        strcpy(config[12],tem_config_12);

        for(int e1=0;e1<4;e1++)
        {
            if(strncmp(updata_ECU_ID[e1],msg[25],1)==0)
            {
                //MPU升级包
                //ECU1
                if(strncmp(msg[27],"1",1)==0 && strncmp(msg[25],"1",1)==0 && strncmp(msg[33],"0",1)==0)
                {
                    for(int e2=0;e2<10;e2++)
                    {
                        if(strncmp(ECU_1_MPU_list[e2],msg[31],20)==0)
                        {
                            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                          "VALUES(:result,:type_id,:content,:threat,:time)");
                            query.bindValue(":result","success");
                            query.bindValue(":type_id","升级包");
                            query.bindValue(":content","ECU1-MPU升级包通过验证");
                            query.bindValue(":threat","-");
                            query.bindValue(":time",datetime);
                            query.exec();
                            QMessageBox::about(this,"成功","该报文通过验证。");
                            break;
                        }

                        if(e2==9)
                        {
                            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                          "VALUES(:result,:type_id,:content,:threat,:time)");
                            query.bindValue(":result","fail");
                            query.bindValue(":type_id","升级包");
                            query.bindValue(":content","丢弃升级包:ECU1-MPU版本不兼容");
                            query.bindValue(":threat","MPU版本兼容性攻击");
                            query.bindValue(":time",datetime);
                            query.exec();
                            QMessageBox::about(this,"拦截","该升级包可疑！");
                        }
                    }
                }
                //ECU2
                else if (strncmp(msg[27],"1",1)==0 && strncmp(msg[25],"2",1)==0 && strncmp(msg[33],"0",1)==0)
                {
                    for(int e3=0;e3<10;e3++)
                    {
                        if(strncmp(ECU_2_MPU_list[e3],msg[31],20)==0)
                        {
                            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                          "VALUES(:result,:type_id,:content,:threat,:time)");
                            query.bindValue(":result","success");
                            query.bindValue(":type_id","升级包");
                            query.bindValue(":content","ECU2-MPU升级包通过验证");
                            query.bindValue(":threat","-");
                            query.bindValue(":time",datetime);
                            query.exec();
                            QMessageBox::about(this,"成功","该报文通过验证。");
                            break;
                        }

                        if(e3==9)
                        {
                            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                          "VALUES(:result,:type_id,:content,:threat,:time)");
                            query.bindValue(":result","fail");
                            query.bindValue(":type_id","升级包");
                            query.bindValue(":content","丢弃升级包:ECU2-MPU版本不兼容");
                            query.bindValue(":threat","MPU版本兼容性攻击");
                            query.bindValue(":time",datetime);
                            query.exec();
                            QMessageBox::about(this,"拦截","该升级包可疑！");
                        }
                    }
                }

                //应用(硬件)升级包
                //ECU1
                if(strncmp(msg[27],"0",1)==0 && strncmp(msg[25],"1",1)==0 && strncmp(msg[33],"1",1)==0)
                {
                    //可更新的APP
                    for(int f1=0;f1<2;f1++)
                    {
                        if(strncmp(msg[35],ECU_1_APP_id[f1],1)==0)
                        {
                            //APP1
                            if(atoi(msg[35])==1)
                            {
                                for(int f2=0;f2<10;f2++)
                                {
                                    if(strncmp(msg[37],ECU1_APP1[f2],20)==0)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","success");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","ECU1-MCU-APP1升级包通过验证");
                                        query.bindValue(":threat","-");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"成功","该报文通过验证。");
                                        break;
                                    }

                                    if(f2==9)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","fail");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","丢弃升级包:ECU1-MCU-APP1版本不兼容");
                                        query.bindValue(":threat","MCU版本兼容性攻击");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"拦截","该升级包可疑！");
                                    }
                                }
                            }
                            //APP2
                            if(atoi(msg[35])==2)
                            {
                                for(int f3=0;f3<10;f3++)
                                {
                                    if(strncmp(msg[37],ECU1_APP2[f3],20)==0)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","success");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","ECU1-MCU-APP2升级包通过验证");
                                        query.bindValue(":threat","-");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"成功","该报文通过验证。");
                                        break;
                                    }

                                    if(f3==9)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","fail");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","丢弃升级包:ECU1-MCU-APP2版本不兼容");
                                        query.bindValue(":threat","MCU版本兼容性攻击");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"拦截","该升级包可疑！");
                                    }
                                }
                            }
                            break;
                        }

                        if(f1==1)
                        {
                            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                          "VALUES(:result,:type_id,:content,:threat,:time)");
                            query.bindValue(":result","fail");
                            query.bindValue(":type_id","升级包");
                            query.bindValue(":content","丢弃升级包:ECU1关键应用禁止更新");
                            query.bindValue(":threat","关键应用刷新");
                            query.bindValue(":time",datetime);
                            query.exec();
                            QMessageBox::about(this,"拦截","该升级包可疑！");
                        }
                    }

                }
                //ECU2
                else if (strncmp(msg[27],"0",1)==0 && strncmp(msg[25],"2",1)==0 && strncmp(msg[33],"1",1)==0)
                {
                    //可更新的APP
                    for(int g1=0;g1<2;g1++)
                    {
                        if(strncmp(msg[35],ECU_2_APP_id[g1],1)==0)
                        {
                            //APP1
                            if(atoi(msg[35])==1)
                            {
                                for(int g2=0;g2<10;g2++)
                                {
                                    if(strncmp(msg[37],ECU2_APP1[g2],20)==0)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","success");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","ECU2-MCU-APP1升级包通过验证");
                                        query.bindValue(":threat","-");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"成功","该报文通过验证。");
                                        break;
                                    }

                                    if(g2==9)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","fail");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","丢弃升级包:ECU2-MCU-APP1版本不兼容");
                                        query.bindValue(":threat","MCU版本兼容性攻击");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"拦截","该升级包可疑！");
                                    }
                                }
                            }
                            //APP2
                            if(atoi(msg[35])==2)
                            {
                                for(int g3=0;g3<10;g3++)
                                {
                                    if(strncmp(msg[37],ECU2_APP2[g3],20)==0)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","success");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","ECU2-MCU-APP2升级包通过验证");
                                        query.bindValue(":threat","-");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"成功","该升级包通过验证，准备升级。");
                                        break;
                                    }

                                    if(g3==9)
                                    {
                                        query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                                      "VALUES(:result,:type_id,:content,:threat,:time)");
                                        query.bindValue(":result","fail");
                                        query.bindValue(":type_id","升级包");
                                        query.bindValue(":content","丢弃升级包:ECU2-MCU-APP2版本不兼容");
                                        query.bindValue(":threat","MCU版本兼容性攻击");
                                        query.bindValue(":time",datetime);
                                        query.exec();
                                        QMessageBox::about(this,"拦截","该升级包可疑！");
                                    }
                                }
                            }
                            break;
                        }

                        if(g1==1)
                        {
                            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                                          "VALUES(:result,:type_id,:content,:threat,:time)");
                            query.bindValue(":result","fail");
                            query.bindValue(":type_id","升级包");
                            query.bindValue(":content","丢弃升级包:ECU1关键应用禁止更新");
                            query.bindValue(":threat","关键应用刷新");
                            query.bindValue(":time",datetime);
                            query.exec();
                            QMessageBox::about(this,"拦截","该升级包可疑！");
                        }
                    }

                }
            }
        }



    }

    if(strncmp(msg[7], "0x03", 50) == 0)
    {
        if(travel.startup == 0)
        {
            if(atoi(msg[43]) >= atoi(config[0]))
            {
                travel.show();
                travel.startup = 1;
                travel.time_limit = atoi(msg[39]);
                travel.distance_limit = atoi(msg[41]);
                query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                              "VALUES(:result,:type_id,:content,:threat,:time)");
                query.bindValue(":result", "成功启动汽车");
                query.bindValue(":type_id", "0x03");
                query.bindValue(":content", "用户信誉分达到要求");
                query.bindValue(":threat", "-");
                query.bindValue(":time", datetime);
                query.exec();
                qDebug() << travel.time_limit;
            }
            else QMessageBox::about(this,"失败","用户信誉分不足！");
        }
        else QMessageBox::about(this,"失败","车辆已被启动！");
    }

    if(strncmp(msg[7], "0x04", 50) == 0)
    {
        if(travel.startup == 1)
        {
            travel.close();
            travel.startup = 0;
            query.prepare("INSERT INTO records(result,type_id,content,threat,time)"
                          "VALUES(:result,:type_id,:content,:threat,:time)");
            query.bindValue(":result", "结束使用汽车");
            query.bindValue(":type_id", "0x04");
            query.bindValue(":content", "-");
            query.bindValue(":threat", "-");
            query.bindValue(":time", datetime);
            query.exec();
        }
        else QMessageBox::about(this,"失败","车辆处于空闲中！");
    }

    fflush(stdout);
}

void MainWindow::on_set_config_clicked()
{
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
    strcpy(config[0], contents0);
    strcpy(config[1], contents1);
    strcpy(config[2], contents2);
    strcpy(config[3], contents3);
    strcpy(config[4], contents4);
    strcpy(config[5], contents5);
    strcpy(config[6], contents6);
    strcpy(config[7], contents7);
    strcpy(config[8], contents8);
    strcpy(config[9], contents9);
    strcpy(config[10], contents10);
    strcpy(config[11], contents11);
    strcpy(config[12], contents12);
    QMessageBox::about(this,"成功","设置成功");
    qDebug() << config[0];
}

void MainWindow::on_success_result_db_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Record");
    qDebug() << db.connectionName();
    db.setDatabaseName("Record.db");
    qDebug() << db.databaseName();

    //打开数据库
    if(!db.open())
    {
        qDebug() << "open fail.";
        qDebug() << db.lastError().text();
    }

    //打开成功执行sql
    QString sql =
            "create table if not exists records("
            "result varchar(10),type_id varchar(10),content varchar(30),threat varchar(30),time varchar(30)"
            ");";

    QSqlQuery query(db);//访问数据库
    if(!query.exec(sql))//创建表
    {
       qDebug() << query.lastError().text();
    }
    QSqlQueryModel *qmodel;
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("select result, type_id, content, time from records where result = 'success'", db);
    ui->dbview->setModel(qmodel);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->dbview>horizontalHeader()>setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::on_fail_result_db_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Record");
    qDebug() << db.connectionName();
    db.setDatabaseName("Record.db");
    qDebug() << db.databaseName();

    //打开数据库
    if(!db.open())
    {
        qDebug() << "open fail.";
        qDebug() << db.lastError().text();
    }

    //打开成功执行sql
    QString sql =
            "create table if not exists records("
            "result varchar(10),type_id varchar(10),content varchar(30),threat varchar(30),time varchar(30)"
            ");";

    QSqlQuery query(db);//访问数据库
    if(!query.exec(sql))//创建表
    {
       qDebug() << query.lastError().text();
    }
    QSqlQueryModel *qmodel;
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("select * from records where result = 'fail'", db);
    ui->dbview->setModel(qmodel);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //取消注释会显示不全
}

void MainWindow::on_car_rental_record_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Record");
    qDebug() << db.connectionName();
    db.setDatabaseName("Record.db");
    qDebug() << db.databaseName();

    //打开数据库
    if(!db.open())
    {
        qDebug() << "open fail.";
        qDebug() << db.lastError().text();
    }

    //打开成功执行sql
    QString sql =
            "create table if not exists records("
            "result varchar(10),type_id varchar(10),content varchar(30),threat varchar(30),time varchar(30)"
            ");";

    QSqlQuery query(db);//访问数据库
    if(!query.exec(sql))//创建表
    {
       qDebug() << query.lastError().text();
    }
    QSqlQueryModel *qmodel;
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("select result, type_id, content, time from records where type_id = '0x03' OR type_id = '0x04'", db);
    ui->dbview->setModel(qmodel);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_result_db_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Record");
    qDebug() << db.connectionName();
    db.setDatabaseName("Record.db");
    qDebug() << db.databaseName();

    //打开数据库
    if(!db.open())
    {
        qDebug() << "open fail.";
        qDebug() << db.lastError().text();
    }

    //打开成功执行sql
    QString sql =
            "create table if not exists records("
            "result varchar(10),type_id varchar(10),content varchar(30),threat varchar(30),time varchar(30)"
            ");";

    QSqlQuery query(db);//访问数据库
    if(!query.exec(sql))//创建表
    {
       qDebug() << query.lastError().text();
    }
    QSqlQueryModel *qmodel;
    qmodel = new QSqlQueryModel();
    qmodel->setQuery("select * from records", db);
    ui->dbview->setModel(qmodel);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dbview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
