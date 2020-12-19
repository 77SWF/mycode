# -*- coding: UTF-8 -*-
# 文件名：MyChat.py

import os
import sys
import time
import base64
import threading
import webbrowser
from PyQt5.QtCore import Qt
from Client import MyChat_Client
from PyQt5 import QtCore, QtGui, QtWidgets

from functools import partial
from PyQt5.QtWidgets import QMessageBox
from login_window import Ui_loginWindow

import images_src.chat_images_src,images_src.login_images_src

class loginWindow(QtWidgets.QDialog):
    def __init__(self):
        #父类的init
        super(loginWindow, self).__init__()
        #这个loginWindow的init增加执行下面这个函数
        self.setupUi()

    def setupUi(self):
        self.setObjectName("LoginWindow")
        self.setStyleSheet("#LoginWindow{border-image:url(./images/style/login/login.png);}")
        # QIcon对象接收图标路径，
        self.setWindowIcon(QtGui.QIcon("./images/style/icon.png"))
        # 控件self的尺寸
        self.resize(432, 300)

        self.loginButton = QtWidgets.QPushButton(self)      #登录按钮
        # 控件的位置与尺寸
        self.loginButton.setGeometry(QtCore.QRect(118, 243, 220, 35))
        self.loginButton.setObjectName("login")
        self.loginButton.setStyleSheet("border-image:url(./images/style/login/loginbutton.png);")
        # 按钮点击后触发函数loginButtonClicked
        self.loginButton.clicked.connect(self.loginButtonClicked)

        self.registerButton = QtWidgets.QPushButton(self)   #注册按钮
        self.registerButton.setGeometry(QtCore.QRect(12, 250, 65, 25))
        self.registerButton.setObjectName("register")
        self.registerButton.setStyleSheet("border:none;")  #无边框
        self.registerButton.setCursor(Qt.PointingHandCursor)
        self.registerButton.clicked.connect(self.registerButtonClicked)

        self.userName = QtWidgets.QLineEdit(self)       #账号
        self.userName.setGeometry(QtCore.QRect(118, 140, 220, 28))
        self.userName.setObjectName("username")
        self.userName.setPlaceholderText("请输入用户名")
        self.userName.setMaxLength(20)

        self.password = QtWidgets.QLineEdit(self)       #密码
        self.password.setGeometry(QtCore.QRect(118, 170, 220, 28))
        self.password.setObjectName("password")
        self.password.setPlaceholderText("请输入密码")
        self.password.setMaxLength(20)
        self.password.setEchoMode(self.password.Password)

        self.constuserName = QtWidgets.QLineEdit(self)      #文本输入框前的提示
        self.constuserName.setGeometry(QtCore.QRect(42, 140, 75, 28))
        self.constuserName.setStyleSheet("border:none;")
        self.constuserName.setReadOnly(True)
        self.constpassword = QtWidgets.QLineEdit(self)
        self.constpassword.setGeometry(QtCore.QRect(42, 170, 75, 28))
        self.constpassword.setStyleSheet("border:none;")
        self.constpassword.setReadOnly(True)

        self.loginError = QtWidgets.QLineEdit(self)         #登录信息提示框
        self.loginError.setGeometry(QtCore.QRect(118, 205, 220, 28))
        self.loginError.setStyleSheet("background-color: rgb(255, 25, 255, 60);border:none;")
        self.loginError.setAlignment(QtCore.Qt.AlignCenter)
        self.loginError.setReadOnly(True)

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("LoginWindow", "MyChat"))
        self.loginButton.setText(_translate("LoginWindow", "登录"))
        self.registerButton.setText(_translate("LoginWindow", "注册账号"))
        self.constuserName.setText(_translate("LoginWindow", "MyChat账号："))
        self.constpassword.setText(_translate("LoginWindow", "MyChat密码："))
        self.loginError.setText(_translate("registerWindow", "欢迎使用MyChat!"))

    def loginButtonClicked(self):
        '''
        点击登录按钮触发事件
        '''
        Username = self.userName.text() #获取输入的内容，然后使用Client模块
        Password = self.password.text()
        if len(Username) == 0 or len(Password) == 0:
            self.loginError.setText("您还没有输入账号或密码！")
        else:
            client.login(Username, Password)
            while client.loginBack == None:
                pass
            flag = False
            if client.loginBack["info"] == "loginSucc":
                self.loginError.setStyleSheet("background-color: rgb(100, 255, 0, 60);border:none;")
                self.loginError.setText("登陆成功")
                self.hide()
                self.chatWindow = chatWindow(Username)      #登录成功，调出聊天界面
                self.chatWindow.show()
                self.chatWindow.main()
            elif client.loginBack["info"] == "loginFail":
                self.loginError.setText("账号或密码错误！请重新输入！")
            else:
                self.loginError.setText("该账号已经登录！")
            client.loginBack = None

    def registerButtonClicked(self):
        '''
        点击注册账号按钮触发事件
        调出注册窗口
        '''
        self.registerWindow = registerWindow()
        self.registerWindow.show()

class registerWindow(QtWidgets.QDialog):
    def __init__(self):
        super(registerWindow, self).__init__()
        self.setupUi()

    def setupUi(self):
        self.setObjectName("registerWindow")
        self.setStyleSheet("#registerWindow{border-image:url(./images/style/register/register.png);}")
        self.setWindowIcon(QtGui.QIcon("./images/style/icon.png"))
        self.resize(360, 330)

        self.userName = QtWidgets.QLineEdit(self)       #用户名
        self.userName.setGeometry(QtCore.QRect(118, 80, 220, 28))
        self.userName.setObjectName("username")
        self.userName.setPlaceholderText("请输入账号")
        self.userName.setMaxLength(20)

        self.password = QtWidgets.QLineEdit(self)       #密码
        self.password.setGeometry(QtCore.QRect(118, 120, 220, 28))
        self.password.setObjectName("password")
        self.password.setPlaceholderText("请输入密码")
        self.password.setMaxLength(20)
        self.password.setEchoMode(self.password.Password)

        self.passwordAgain = QtWidgets.QLineEdit(self)  #密码确认
        self.passwordAgain.setGeometry(QtCore.QRect(118, 160, 220, 28))
        self.passwordAgain.setObjectName("passwordAgain")
        self.passwordAgain.setPlaceholderText("请再次输入密码")
        self.passwordAgain.setMaxLength(20)
        self.passwordAgain.setEchoMode(self.password.Password)

        self.constuserName = QtWidgets.QLineEdit(self)  #文本输入框前的提示
        self.constuserName.setGeometry(QtCore.QRect(30, 80, 87, 28))
        self.constuserName.setStyleSheet("border:none;")
        self.constuserName.setReadOnly(True)
        self.constpassword = QtWidgets.QLineEdit(self)
        self.constpassword.setGeometry(QtCore.QRect(30, 120, 87, 28))
        self.constpassword.setStyleSheet("border:none;")
        self.constpassword.setReadOnly(True)
        self.constpasswordAgain = QtWidgets.QLineEdit(self)
        self.constpasswordAgain.setGeometry(QtCore.QRect(30, 160, 87, 28))
        self.constpasswordAgain.setStyleSheet("border:none;")
        self.constpasswordAgain.setReadOnly(True)

        self.registerButton = QtWidgets.QPushButton(self)   #注册按钮
        self.registerButton.setGeometry(QtCore.QRect(118, 240, 220, 35))
        self.registerButton.setObjectName("register")
        self.registerButton.setStyleSheet("border-image:url(./images/style/register/registerbutton.png);")
        self.registerButton.clicked.connect(self.registerButtonClicked)

        self.registerError = QtWidgets.QLineEdit(self)      #注册信息提示框
        self.registerError.setGeometry(QtCore.QRect(118, 200, 220, 28))
        self.registerError.setStyleSheet("background-color: rgb(255, 25, 255, 60);border:none;")
        self.registerError.setAlignment(QtCore.Qt.AlignCenter)
        self.registerError.setReadOnly(True)

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.setWindowTitle(_translate("registerWindow", "注册账号"))
        self.constuserName.setText(_translate("registerWindow", "MyChat账号："))
        self.constpassword.setText(_translate("registerWindow", "MyChat密码："))
        self.constpasswordAgain.setText(_translate("registerWindow", "再次输入密码："))
        self.registerButton.setText(_translate("registerWindow", "注册"))
        self.registerError.setText(_translate("registerWindow", "欢迎使用MyChat!"))

    def registerButtonClicked(self):
        '''
        点击注册账号按钮触发事件
        '''
        Username = self.userName.text()
        password = self.password.text()
        passwordAgain = self.passwordAgain.text()
        if len(Username) == 0 or len(password) == 0 or len(passwordAgain) == 0:
            self.registerError.setText("您还没有输入账号或密码！")
        elif password != passwordAgain:
            self.registerError.setText("您两次输入的密码不同！")
        else:
            client.register(Username, password)
            while client.registerBack == None:
                pass
            if client.registerBack["info"] == "rgtrSucc":
                self.registerError.setStyleSheet("background-color: rgb(100, 255, 0, 60);border:none;")
                self.registerError.setText("注册成功！请返回登录！")
            else:
                self.registerError.setText("该账号已存在！")
            client.registerBack = None

class Ui_chatWindow(object):
    def __init__(self, name,chatwindow):
        # 这个聊天界面是用户Username的，属于客户端client
        self.Username = name
        super(Ui_chatWindow, self).__init__()
        self.setupUi(chatwindow)
        try:
            os.mkdir(self.Username)         #创建对应的文件夹
        except FileExistsError:
            pass

    def setupUi(self, chatWindow):
        chatWindow.setObjectName("chatWindow")
        chatWindow.resize(1500, 780)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(chatWindow.sizePolicy().hasHeightForWidth())
        chatWindow.setSizePolicy(sizePolicy)
        chatWindow.setMinimumSize(QtCore.QSize(1500, 780))
        chatWindow.setMaximumSize(QtCore.QSize(1500, 780))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/login_window/images/icon.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        chatWindow.setWindowIcon(icon)
        chatWindow.setStyleSheet("border-image: url(:/chat/images/chatwindow.png);")
        self.centralwidget = QtWidgets.QWidget(chatWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.msg_frame_group = QtWidgets.QTextEdit(self.centralwidget)
        self.msg_frame_group.setGeometry(QtCore.QRect(340, -20, 1161, 431))
        self.msg_frame_group.setStyleSheet("border-image: url(:/chat/images/msg.png);")
        self.msg_frame_group.setReadOnly(True)
        self.msg_frame_group.setObjectName("msg_frame_group")
        self.send_to_user = "all"

        self.edit_frame = QtWidgets.QTextEdit(self.centralwidget)
        self.edit_frame.setGeometry(QtCore.QRect(340, 450, 1161, 311))
        self.edit_frame.setStyleSheet("border-image: url(:/chat/images/send_frame.png);")
        self.edit_frame.setObjectName("edit_frame")

        self.send_buttom = QtWidgets.QPushButton(self.centralwidget)
        self.send_buttom.setGeometry(QtCore.QRect(1350, 713, 90, 40))
        self.send_buttom.setStyleSheet("border-image: url(:/chat/images/friend_list.png);")
        self.send_buttom.setObjectName("send_buttom")

        self.emoji_buttom = QtWidgets.QPushButton(self.centralwidget)
        self.emoji_buttom.setGeometry(QtCore.QRect(359, 421, 21, 21))
        self.emoji_buttom.setStyleSheet("border-image: url(:/chat/images/emojibutton.png);")
        self.emoji_buttom.setText("")
        self.emoji_buttom.setObjectName("emoji_buttom")

        self.photo_buttom = QtWidgets.QPushButton(self.centralwidget)
        self.photo_buttom.setGeometry(QtCore.QRect(390, 420, 21, 21))
        self.photo_buttom.setStyleSheet("border-image: url(:/chat/images/imagebutton.png);")
        self.photo_buttom.setText("")
        self.photo_buttom.setObjectName("photo_buttom")

        self.file__buttom = QtWidgets.QPushButton(self.centralwidget)
        self.file__buttom.setGeometry(QtCore.QRect(420, 420, 21, 21))
        self.file__buttom.setStyleSheet("border-image: url(:/chat/images/filebutton.png);")
        self.file__buttom.setText("")
        self.file__buttom.setObjectName("file__buttom")

        self.emoji_table = QtWidgets.QTableWidget(self.centralwidget)
        self.emoji_table.setGeometry(QtCore.QRect(360, 319, 300, 90))
        self.emoji_table.setStyleSheet("border-image: url(:/chat/images/emoji_back.png);")
        self.emoji_table.setFrameShape(QtWidgets.QFrame.Box)
        self.emoji_table.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.emoji_table.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.emoji_table.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.emoji_table.setShowGrid(True)
        self.emoji_table.setGridStyle(QtCore.Qt.SolidLine)
        self.emoji_table.setWordWrap(True)
        self.emoji_table.setCornerButtonEnabled(True)
        self.emoji_table.setRowCount(3)
        self.emoji_table.setColumnCount(3)
        self.emoji_table.setObjectName("emoji_table")
        self.emoji_table.horizontalHeader().setVisible(False)
        self.emoji_table.horizontalHeader().setHighlightSections(True)
        self.emoji_table.verticalHeader().setVisible(False)
        self.emoji_table.verticalHeader().setSortIndicatorShown(False)
        self.emoji_table.verticalHeader().setStretchLastSection(False)

        self.userlist_logined = QtWidgets.QListWidget(self.centralwidget)
        self.userlist_logined.setGeometry(QtCore.QRect(150, 40, 161, 691))
        self.userlist_logined.setStyleSheet("border-image: url(:/chat/images/friend_list.png);")
        self.userlist_logined.setAutoScrollMargin(16)
        self.userlist_logined.setIconSize(QtCore.QSize(0, 0))
        self.userlist_logined.setLayoutMode(QtWidgets.QListView.SinglePass)
        self.userlist_logined.setGridSize(QtCore.QSize(0, 25))
        self.userlist_logined.setViewMode(QtWidgets.QListView.ListMode)
        self.userlist_logined.setBatchSize(100)
        self.userlist_logined.setItemAlignment(QtCore.Qt.AlignLeading)
        self.userlist_logined.setObjectName("userlist_logined")
        self.userlist_logined.addItems(client.userlist)
        self.userlist_logined.doubleClicked.connect(self.username_double_clicked)
        # key=username,value=与各用户的私聊消息框，1个QTextEdit控件
        self.msg_frames_private = {}

        self.graphicsView = QtWidgets.QGraphicsView(self.centralwidget)
        self.graphicsView.setGeometry(QtCore.QRect(340, 410, 1166, 41))
        self.graphicsView.setStyleSheet("border-image: url(:/chat/images/send_bar.png);")
        self.graphicsView.setObjectName("graphicsView")

        self.group_chat_buttom = QtWidgets.QPushButton(self.centralwidget)
        self.group_chat_buttom.setGeometry(QtCore.QRect(150, 10, 161, 16))
        self.group_chat_buttom.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.group_chat_buttom.setStyleSheet("border-image: url(:/chat/images/friend_list.png);")
        self.group_chat_buttom.setObjectName("group_chat_buttom")

        self.graphicsView.raise_()
        self.msg_frame_group.raise_()
        self.edit_frame.raise_()
        self.send_buttom.raise_()
        self.emoji_buttom.raise_()
        self.photo_buttom.raise_()
        self.file__buttom.raise_()
        self.emoji_table.raise_()
        self.userlist_logined.raise_()
        self.group_chat_buttom.raise_()
        chatWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(chatWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1500, 22))
        self.menubar.setObjectName("menubar")
        chatWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(chatWindow)
        self.statusbar.setObjectName("statusbar")
        chatWindow.setStatusBar(self.statusbar)

        self.retranslateUi(chatWindow)
        QtCore.QMetaObject.connectSlotsByName(chatWindow)

    def retranslateUi(self, chatWindow):
        _translate = QtCore.QCoreApplication.translate
        chatWindow.setWindowTitle(_translate("chatWindow", "Chatting"))
        self.msg_frame_group.setHtml(_translate("chatWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n""<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n""p, li { white-space: pre-wrap; }\n""</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n""<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.send_buttom.setText(_translate("chatWindow", "发送(S)"))
        __sortingEnabled = self.userlist_logined.isSortingEnabled()
        self.userlist_logined.setSortingEnabled(False)
        self.userlist_logined.setSortingEnabled(__sortingEnabled)
        self.group_chat_buttom.setText(_translate("chatWindow", "群 聊"))

    def username_double_clicked(self):
        self.msg_frame_group.hide()
        for username in self.msg_frames_private:
            self.msg_frames_private[username].hide()
        user_clicked = self.userlist_logined.currentItem().text()
        if user_clicked == self.Username:
            return
        elif user_clicked in self.msg_frames_private:
            self.msg_frames_private[user_clicked].show()
            self.send_to_user = user_clicked
        elif user_clicked not in self.msg_frames_private:
            self.msg_frames_private[user_clicked] = QtWidgets.QTextEdit(self.centralwidget)
            self.msg_frames_private[user_clicked].setGeometry(QtCore.QRect(340, -20, 1161, 431))
            self.msg_frames_private[user_clicked].setStyleSheet("border-image: url(:/chat/images/msg.png);")
            self.msg_frames_private[user_clicked].setReadOnly(True)
            self.msg_frames_private[user_clicked].setObjectName("msg_frame_private")
            self.send_to_user = user_clicked

    def recv(self):
        '''
        用于将接收到的消息显示出来
        '''
        while True:
            while len(client.usermsg):
                msg_recv = client.usermsg.pop()
                msgtime = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(msg_recv["time"]))
                if msg_recv["mtype"] == "msg":
                    msg_recv["msg"] = msg_recv["msg"].replace("\n","\n  ")
                    if msg_recv["name"] == self.Username:       #从本地发送的消息打印
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(Qt.green)
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            self.msg_frame_group.setTextColor(Qt.black)
                            self.msg_frame_group.insertPlainText(msg_recv["msg"] + "\n")
                        else:
                            for i in self.prtbutton:
                                print(msg_recv["destname"])
                                print(self.destprtbutton[i])
                                if msg_recv["destname"] == self.destprtbutton[i]:
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.green)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    self.buttontotext[i].setTextColor(Qt.black)
                                    self.buttontotext[i].insertPlainText(msg_recv["msg"] + "\n")
                    elif msg_recv["destname"] in (self.Username, "all"):        #本地接收到的消息打印
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(Qt.blue)
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            self.msg_frame_group.setTextColor(Qt.black)
                            self.msg_frame_group.insertPlainText(msg_recv["msg"] + "\n")
                        else:
                            for i in self.prtbutton:
                                if self.destprtbutton[i] == msg_recv["name"]:
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.blue)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    self.buttontotext[i].setTextColor(Qt.black)
                                    self.buttontotext[i].insertPlainText(msg_recv["msg"] + "\n")
                                    break
                                elif self.destprtbutton[i] == None:
                                    self.destprtbutton[i] = msg_recv["name"]
                                    i.setText(msg_recv["name"])
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.blue)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    self.buttontotext[i].setTextColor(Qt.black)
                                    self.buttontotext[i].insertPlainText(msg_recv["msg"] + "\n")
                                    break
                elif msg_recv["mtype"] == "emoji":
                    if msg_recv["name"] == self.Username:  # 从本地发送的消息打印
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(Qt.green)
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./images/emoji/"+ str(msg_recv["msg"]) +".gif"
                            tcursor = self.msg_frame_group.textCursor()
                            img = QtGui.QTextImageFormat()
                            img.setName(path)
                            img.setHeight(28)
                            img.setWidth(28)
                            tcursor.insertImage(img)
                            self.msg_frame_group.insertPlainText("\n")
                        else:
                            for i in self.prtbutton:
                                print(msg_recv["destname"])
                                print(self.destprtbutton[i])
                                if msg_recv["destname"] == self.destprtbutton[i]:
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.green)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    path = "./images/emoji/" + str(msg_recv["msg"]) + ".gif"
                                    tcursor = self.buttontotext[i].textCursor()
                                    img = QtGui.QTextImageFormat()
                                    img.setName(path)
                                    img.setHeight(28)
                                    img.setWidth(28)
                                    tcursor.insertImage(img)
                                    self.buttontotext[i].insertPlainText("\n")
                    elif msg_recv["destname"] in (self.Username, "all"):  # 本地接收到的消息打印
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(Qt.blue)
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./images/emoji/"+ str(msg_recv["msg"]) +".gif"
                            tcursor = self.msg_frame_group.textCursor()
                            img = QtGui.QTextImageFormat()
                            img.setName(path)
                            img.setHeight(28)
                            img.setWidth(28)
                            tcursor.insertImage(img)
                            self.msg_frame_group.insertPlainText("\n")
                        else:
                            for i in self.prtbutton:
                                if self.destprtbutton[i] == msg_recv["name"]:
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.blue)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    path = "./images/emoji/" + str(msg_recv["msg"]) + ".gif"
                                    tcursor = self.buttontotext[i].textCursor()
                                    img = QtGui.QTextImageFormat()
                                    img.setName(path)
                                    img.setHeight(28)
                                    img.setWidth(28)
                                    tcursor.insertImage(img)
                                    self.buttontotext[i].insertPlainText("\n")
                                    break
                                elif self.destprtbutton[i] == None:
                                    self.destprtbutton[i] = msg_recv["name"]
                                    i.setText(msg_recv["name"])
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.blue)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    path = "./images/emoji/" + str(msg_recv["msg"]) + ".gif"
                                    tcursor = self.buttontotext[i].textCursor()
                                    img = QtGui.QTextImageFormat()
                                    img.setName(path)
                                    img.setHeight(28)
                                    img.setWidth(28)
                                    tcursor.insertImage(img)
                                    self.buttontotext[i].insertPlainText("\n")
                                    break
                else:
                    if msg_recv["name"] == self.Username:  # 从本地发送的消息打印
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(Qt.green)
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                            with open(path,"wb") as f:
                                f.write(base64.b64decode(msg_recv["msg"]))
                                f.close()
                            tcursor = self.msg_frame_group.textCursor()
                            img = QtGui.QTextImageFormat()
                            if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                img.setName(path)
                                img.setHeight(100)
                                img.setWidth(100)
                                tcursor.insertImage(img)
                            else:
                                img.setName("./images/style/MyChat/filebutton.png")
                                img.setHeight(30)
                                img.setWidth(30)
                                tcursor.insertImage(img)
                                self.msg_frame_group.insertPlainText("文件已保存在：" + path)
                            self.msg_frame_group.insertPlainText("\n")
                        else:
                            for i in self.prtbutton:
                                print(msg_recv["destname"])
                                print(self.destprtbutton[i])
                                if msg_recv["destname"] == self.destprtbutton[i]:
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.green)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                                    with open(path, "wb") as f:
                                        f.write(base64.b64decode(msg_recv["msg"]))
                                        f.close()
                                    tcursor = self.buttontotext[i].textCursor()
                                    img = QtGui.QTextImageFormat()
                                    if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                        img.setName(path)
                                        img.setHeight(100)
                                        img.setWidth(100)
                                        tcursor.insertImage(img)
                                    else:
                                        img.setName("./images/style/MyChat/filebutton.png")
                                        img.setHeight(30)
                                        img.setWidth(30)
                                        tcursor.insertImage(img)
                                        self.buttontotext[i].insertPlainText("文件已保存在："+path)
                                    self.buttontotext[i].insertPlainText("\n")
                    elif msg_recv["destname"] in (self.Username, "all"):  # 本地接收到的消息打印
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(Qt.blue)
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                            with open(path, "wb") as f:
                                f.write(base64.b64decode(msg_recv["msg"]))
                                f.close()
                            tcursor = self.msg_frame_group.textCursor()
                            img = QtGui.QTextImageFormat()
                            if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                img.setName(path)
                                img.setHeight(100)
                                img.setWidth(100)
                                tcursor.insertImage(img)
                            else:
                                img.setName("./images/style/MyChat/filebutton.png")
                                img.setHeight(30)
                                img.setWidth(30)
                                tcursor.insertImage(img)
                                self.msg_frame_group.insertPlainText("文件已保存在：" + path)
                            self.msg_frame_group.insertPlainText("\n")
                        else:
                            for i in self.prtbutton:
                                if self.destprtbutton[i] == msg_recv["name"]:
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.blue)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                                    with open(path, "wb") as f:
                                        f.write(base64.b64decode(msg_recv["msg"]))
                                        f.close()
                                    tcursor = self.buttontotext[i].textCursor()
                                    img = QtGui.QTextImageFormat()
                                    if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                        img.setName(path)
                                        img.setHeight(100)
                                        img.setWidth(100)
                                        tcursor.insertImage(img)
                                    else:
                                        img.setName("./images/style/MyChat/filebutton.png")
                                        img.setHeight(30)
                                        img.setWidth(30)
                                        tcursor.insertImage(img)
                                        self.buttontotext[i].insertPlainText("文件已保存在："+path)
                                    self.buttontotext[i].insertPlainText("\n")
                                    break
                                elif self.destprtbutton[i] == None:
                                    self.destprtbutton[i] = msg_recv["name"]
                                    i.setText(msg_recv["name"])
                                    self.buttontotext[i].moveCursor(QtGui.QTextCursor.End)
                                    self.buttontotext[i].setTextColor(Qt.blue)
                                    self.buttontotext[i].insertPlainText(
                                        " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                    path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                                    with open(path, "wb") as f:
                                        f.write(base64.b64decode(msg_recv["msg"]))
                                        f.close()
                                    tcursor = self.buttontotext[i].textCursor()
                                    img = QtGui.QTextImageFormat()
                                    if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                        img.setName(path)
                                        img.setHeight(100)
                                        img.setWidth(100)
                                        tcursor.insertImage(img)
                                    else:
                                        img.setName("./images/style/MyChat/filebutton.png")
                                        img.setHeight(30)
                                        img.setWidth(30)
                                        tcursor.insertImage(img)
                                        self.buttontotext[i].insertPlainText("文件已保存在："+path)
                                    self.buttontotext[i].insertPlainText("\n")
                                    break

            while len(client.sysmsg):
                msg_recv = client.sysmsg.pop()
                # msgtime = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(msg_recv["time"]))
                if msg_recv["info"] == "userlogin":
                    if msg_recv["name"] not in client.userlist:
                        client.userlist.append(msg_recv["name"])
                        print("有人登录")
                        self.userlist_logined.clear()
                        self.userlist_logined.addItems(client.userlist)
                elif msg_recv["info"] == "userexit":
                    if msg_recv["name"] in client.userlist:
                        client.userlist.remove(msg_recv["name"])
                        self.userlist_logined.clear()
                        self.userlist_logined.addItems(client.userlist)
                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                self.msg_frame_group.setTextColor(Qt.gray)
                self.msg_frame_group.insertPlainText("      "+msg_recv["msg"]+"\n")

    def main(self):
        func1 = threading.Thread(target=self.recv)
        func1.start()

def login_button_clicked(ui,loginWindow):
        '''
        点击登录按钮后触发
        '''
        username = ui.userName.text()
        password = ui.password.text()
        if len(username) == 0 or len(password) == 0:
            QMessageBox.about(loginWindow,"error","请输入账号或密码再登录！")
        else:
            client.login(username, password)
            while client.loginBack == None:
                pass
            flag = False
            if client.loginBack["info"] == "loginSucc":
                # 隐藏登录窗口
                loginWindow.hide() 
                # 登录成功，调出聊天界面
                loginWindow.chatwindow = QtWidgets.QMainWindow()
                chatwindow_ui = Ui_chatWindow(username,loginWindow.chatwindow)
                chatwindow_ui.main()
                loginWindow.chatwindow.show()
                # loginWindow.chatwindow.show()
                
                # 点击"群聊"
                #chatwindow_ui.group_chat_buttom.clicked.connect(
                #    partial(group_chat_buttom_clicked,chatwindow_ui,loginWindow.chatwindow))

            elif client.loginBack["info"] == "loginFail":
                QMessageBox.about(loginWindow,"error","账号或密码错误！请重新输入！")
            else:
                QMessageBox.about(loginWindow,"error","该账号已经登录！")
            client.loginBack = None


if __name__=='__main__':
    app = QtWidgets.QApplication(sys.argv)  #
    client = MyChat_Client(addr="localhost", port=14396)
    client.main()
    
    Mainwindow = QtWidgets.QMainWindow() #窗口实例
    ui = Ui_loginWindow() #登录窗口的控件实例
    ui.setupUi(Mainwindow) # 按格式建立客户端主窗口，后面的窗口做Mainwindow.的子窗口
    Mainwindow.show() 

    ui.login_buttom.clicked.connect(partial(login_button_clicked,ui,Mainwindow))


    # app.exec_： 让程序进入消息循环。等待可能的菜单,工具条,鼠标等的输入,进行响应。
    sys.exit(app.exec_()) 
    #使用的函数可以干净地退出，最后一个窗口关闭后,程序才停止。
