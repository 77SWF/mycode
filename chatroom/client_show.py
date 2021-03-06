# -*- coding: UTF-8 -*-

import os
import sys
import time
import base64
import threading
import webbrowser
from PyQt5.QtCore import Qt
from client_handle_msg import build_client
from PyQt5 import QtCore, QtGui, QtWidgets

from functools import partial
from PyQt5.QtWidgets import QMessageBox
from login_window import Ui_loginWindow
from register_window import Ui_registerWindow
from PyQt5.QtGui import QColor

import images_src.chat_images_src,images_src.login_images_src
'''
无效
class MyWindow(QtWidgets.QMainWindow):
    def closeEvent(self,event):
        result = QMessageBox.question(self,
                      "Confirm Exit...",
                      "Are you sure you want to exit ?",
                      QtGui.QMessageBox.Yes| QtGui.QMessageBox.No)
        event.ignore()

        if result == QtGui.QMessageBox.Yes:
            event.accept()
''' 

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
        self.msg_frame_group.setGeometry(QtCore.QRect(340, 0, 1161, 405))
        self.msg_frame_group.setStyleSheet("border-image: url(:/chat/images/msg.png);")
        self.msg_frame_group.setReadOnly(True)
        self.msg_frame_group.setObjectName("msg_frame_group")
        self.send_to_user = "all"

        self.edit_frame = QtWidgets.QTextEdit(self.centralwidget)
        self.edit_frame.setGeometry(QtCore.QRect(340, 450, 1161, 311))
        self.edit_frame.setStyleSheet("border-image: url(:/chat/images/send_frame.png);")
        self.edit_frame.setObjectName("edit_frame")

        self.send_button = QtWidgets.QPushButton(self.centralwidget)
        self.send_button.setGeometry(QtCore.QRect(1350, 713, 90, 40))
        self.send_button.setStyleSheet("border-image: url(:/chat/images/friend_list.png);")
        self.send_button.setObjectName("send_button")
        self.send_button.clicked.connect(self.send_button_clicked)
        self.send_button.setCursor(Qt.PointingHandCursor)

        self.emoji_button = QtWidgets.QPushButton(self.centralwidget)
        self.emoji_button.setGeometry(QtCore.QRect(359, 416, 32, 32))
        self.emoji_button.setStyleSheet("border-image: url(:/chat/images/emoji.png);")
        self.emoji_button.setText("")
        self.emoji_button.setObjectName("emoji_button")
        self.emoji_button.clicked.connect(self.emoji_button_clicked)
        self.emoji_button.setCursor(Qt.PointingHandCursor)

        self.photo_button = QtWidgets.QPushButton(self.centralwidget)
        self.photo_button.setGeometry(QtCore.QRect(410, 416, 43,30))
        self.photo_button.setStyleSheet("border-image: url(:/chat/images/biaoqing.png);")
        self.photo_button.setText("")
        self.photo_button.setObjectName("photo_button")
        self.photo_button.clicked.connect(self.photo_button_clicked)
        self.photo_button.setToolTip("表情包会被压缩成正方形噢~")
        self.photo_button.setCursor(Qt.PointingHandCursor)

        self.file_button = QtWidgets.QPushButton(self.centralwidget)
        self.file_button.setGeometry(QtCore.QRect(465, 418, 30, 26))
        self.file_button.setStyleSheet("border-image: url(:/chat/images/file.png);")
        self.file_button.setText("")
        self.file_button.setObjectName("file_button")
        self.file_button.clicked.connect(self.file_button_clicked)
        self.file_button.setCursor(Qt.PointingHandCursor)

        self.emoji_table = QtWidgets.QTableWidget(self.centralwidget)
        self.emoji_table.setGeometry(QtCore.QRect(360, 290, 120, 120))
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
        self.emoji_table.hide()
        self.emoji_table.cellClicked.connect(self.emoji_table_clicked)
        self.emoji_table_show = False

        # 放置默认表情图片(参考)
        for i in range(9):
            icon = QtWidgets.QLabel()
            icon.setMargin(4)
            movie = QtGui.QMovie()
            movie.setScaledSize(QtCore.QSize(30, 30))
            movie.setFileName("./images_src/emoji/"+str(i)+".gif")
            movie.start()
            # icon放的表情
            icon.setMovie(movie)
            # 在第几行第几列放icon
            self.emoji_table.setCellWidget(int(i/3), i%3, icon)
            self.emoji_table.setColumnWidth(i%3, 40)          # 设置列的宽度
            self.emoji_table.setRowHeight(int(i/3), 40)       # 设置行的高度

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
        self.msg_frames_private = {}
        for username in client.userlist:
            if username in self.msg_frames_private or username == self.Username:
                continue
            else:
                self.msg_frames_private[username] = QtWidgets.QTextEdit(self.centralwidget)
                self.msg_frames_private[username].setGeometry(QtCore.QRect(340, 0, 1161, 411))
                self.msg_frames_private[username].setStyleSheet("border-image: url(:/chat/images/msg.png);")
                self.msg_frames_private[username].setReadOnly(True)
                self.msg_frames_private[username].setObjectName("msg_frame_private")
                self.msg_frames_private[username].hide()
        self.userlist_logined.doubleClicked.connect(self.username_double_clicked)
        # key=username,value=与各用户的私聊消息框，1个QTextEdit控件
        

        self.graphicsView = QtWidgets.QGraphicsView(self.centralwidget)
        self.graphicsView.setGeometry(QtCore.QRect(340, 410, 1166, 41))
        self.graphicsView.setStyleSheet("border-image: url(:/chat/images/send_bar.png);")
        self.graphicsView.setObjectName("graphicsView")

        self.group_chat_button = QtWidgets.QPushButton(self.centralwidget)
        self.group_chat_button.setGeometry(QtCore.QRect(150, 10, 161, 16))
        self.group_chat_button.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.group_chat_button.setStyleSheet(
            "QPushButton{border-image: url(:/chat/images/friend_list.png);text-align: left}"#按键前景色 
            "QPushButton:hover{border-image: url(:/chat/images/click.png)}" #光标移动到上面后的前景色
            # 无效，未解决
            "QPushButton:pressed{border-image: url(:/chat/images/click.png)}" #按下时的样式
            )
        self.group_chat_button.setObjectName("group_chat_button")
        self.group_chat_button.clicked.connect(self.group_chat_button_clicked)
        self.group_chat_button.setCursor(Qt.PointingHandCursor)

        self.msg_frame_group.raise_() #最下层
        self.graphicsView.raise_()
        self.edit_frame.raise_()
        self.send_button.raise_()
        self.emoji_button.raise_()
        self.photo_button.raise_()
        self.file_button.raise_()
        self.userlist_logined.raise_()
        self.group_chat_button.raise_()
        self.emoji_table.raise_()
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

        self.openfile = QtWidgets.QFileDialog(self.centralwidget)
        self.openfile.setGeometry(QtCore.QRect(150, 20, 1021, 531))

    def retranslateUi(self, chatWindow):
        _translate = QtCore.QCoreApplication.translate
        self.windowtitle = "Chatting(user: " + self.Username + ")"
        chatWindow.setWindowTitle(_translate("chatWindow", self.windowtitle))
        self.msg_frame_group.setHtml(_translate("chatWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n""<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n""p, li { white-space: pre-wrap; }\n""</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n""<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.send_button.setText(_translate("chatWindow", "发送(S)"))
        __sortingEnabled = self.userlist_logined.isSortingEnabled()
        self.userlist_logined.setSortingEnabled(False)
        self.userlist_logined.setSortingEnabled(__sortingEnabled)
        self.group_chat_button.setText(_translate("chatWindow", "群 聊"))

    def username_double_clicked(self):
        username_clicked = self.userlist_logined.currentItem().text()
        # 点击自己
        if username_clicked == self.Username:
            return
        # 点击用户已打开过消息框，显示
        elif username_clicked in self.msg_frames_private:
            # 隐藏群聊消息框
            self.msg_frame_group.hide()
            # 隐藏所有私聊消息框
            for username in self.msg_frames_private:
                self.msg_frames_private[username].hide()
            self.msg_frames_private[username_clicked].setTextColor(Qt.gray)
            self.msg_frames_private[username_clicked].insertPlainText("------you are chatting with {name}~------\n".format(name=username_clicked))
            self.msg_frames_private[username_clicked].show()
            self.send_to_user = username_clicked
        # 点击用户未打开过消息框
        elif username_clicked not in self.msg_frames_private:
            # 新建
            self.msg_frames_private[username_clicked] = QtWidgets.QTextEdit(self.centralwidget)
            self.msg_frames_private[username_clicked].setGeometry(QtCore.QRect(340, 0, 1161, 411))
            self.msg_frames_private[username_clicked].setStyleSheet("border-image: url(:/chat/images/msg.png);")
            self.msg_frames_private[username_clicked].setReadOnly(True)
            self.msg_frames_private[username_clicked].setObjectName("msg_frame_private")
            # 显示
            self.msg_frames_private[username_clicked].show()
            self.send_to_user = username_clicked
            # 隐藏其他
            self.msg_frame_group.hide()
            for username in self.msg_frames_private:
                self.msg_frames_private[username].hide()

    # 显示从服务器接受的消息(参考)
    def display_recv_msgs(self):
        while True:
            while len(client.usermsg):
                msg_recv = client.usermsg.pop()
                msgtime = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(msg_recv["time"]))
                # 文字消息
                if msg_recv["mtype"] == "msg":
                    msg_recv["msg"] = msg_recv["msg"].replace("\n","\n  ")
                    # 自己发送，看发给谁destname：绿色
                    if msg_recv["name"] == self.Username:
                        # 群发
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(QColor(85,170,127))
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            self.msg_frame_group.setTextColor(Qt.black)
                            self.msg_frame_group.insertPlainText(msg_recv["msg"] + "\n")
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                        # 私聊
                        else:
                            curent_private_msg_frame = self.msg_frames_private[msg_recv["destname"]]
                            curent_private_msg_frame.moveCursor(QtGui.QTextCursor.End)
                            curent_private_msg_frame.setTextColor(QColor(85,170,127))
                            curent_private_msg_frame.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            curent_private_msg_frame.setTextColor(Qt.black)
                            curent_private_msg_frame.insertPlainText(msg_recv["msg"] + "\n")
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                    # 接受的,看谁发送的name：蓝色
                    elif msg_recv["destname"] in (self.Username, "all"): 
                        # 群发
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(QColor(62,62,185))
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            self.msg_frame_group.setTextColor(Qt.black)
                            self.msg_frame_group.insertPlainText(msg_recv["msg"] + "\n")
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                        # 私聊
                        else:
                            who_send = msg_recv["name"]
                            if who_send in self.msg_frames_private:
                                curent_private_msg_frame = self.msg_frames_private[who_send]
                                curent_private_msg_frame.moveCursor(QtGui.QTextCursor.End)
                                curent_private_msg_frame.setTextColor(QColor(62,62,185))
                                curent_private_msg_frame.insertPlainText(
                                    " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                curent_private_msg_frame.setTextColor(Qt.black)
                                curent_private_msg_frame.insertPlainText(msg_recv["msg"] + "\n")
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                # 表情
                elif msg_recv["mtype"] == "emoji":
                    # 发送
                    if msg_recv["name"] == self.Username:  # 从本地发送的消息打印
                        # 群发
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(QColor(85,170,127))
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./images_src/emoji/"+ str(msg_recv["msg"]) +".gif"
                            tcursor = self.msg_frame_group.textCursor()
                            image_show = QtGui.QTextImageFormat()
                            image_show.setName(path)
                            image_show.setHeight(28)
                            image_show.setWidth(28)
                            tcursor.insertImage(image_show)
                            self.msg_frame_group.insertPlainText("\n")
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                        # 私聊
                        else:
                            curent_private_msg_frame = self.msg_frames_private[msg_recv["destname"]]
                            curent_private_msg_frame.setTextColor(QColor(85,170,127))
                            curent_private_msg_frame.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./images_src/emoji/"+ str(msg_recv["msg"]) +".gif"
                            tcursor = curent_private_msg_frame.textCursor()
                            image_show = QtGui.QTextImageFormat()
                            image_show.setName(path)
                            image_show.setHeight(28)
                            image_show.setWidth(28)
                            tcursor.insertImage(image_show)
                            curent_private_msg_frame.insertPlainText("\n")
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                    # 接收
                    elif msg_recv["destname"] in (self.Username, "all"):  # 本地接收到的消息打印
                        # 群发
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(QColor(62,62,185))
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./images_src/emoji/"+ str(msg_recv["msg"]) +".gif"
                            tcursor = self.msg_frame_group.textCursor()
                            image_show = QtGui.QTextImageFormat()
                            image_show.setName(path)
                            image_show.setHeight(28)
                            image_show.setWidth(28)
                            tcursor.insertImage(image_show)
                            self.msg_frame_group.insertPlainText("\n")
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                        # 私聊
                        else:
                            who_send = msg_recv["name"]
                            if who_send in self.msg_frames_private:
                                curent_private_msg_frame = self.msg_frames_private[who_send]
                                curent_private_msg_frame.moveCursor(QtGui.QTextCursor.End)
                                curent_private_msg_frame.setTextColor(QColor(62,62,185))
                                curent_private_msg_frame.insertPlainText(
                                    " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                path = "./images_src/emoji/"+ str(msg_recv["msg"]) +".gif"
                                tcursor = curent_private_msg_frame.textCursor()
                                image_show = QtGui.QTextImageFormat()
                                image_show.setName(path)
                                image_show.setHeight(28)
                                image_show.setWidth(28)
                                tcursor.insertImage(image_show)
                                curent_private_msg_frame.insertPlainText("\n")
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                # 表情包/文件
                else:
                    # 发送的
                    if msg_recv["name"] == self.Username: 
                        # 群发
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(QColor(85,170,127))
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            # 本地发送文件保存到用户文件夹
                            path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                            with open(path,"wb") as f:
                                f.write(base64.b64decode(msg_recv["msg"]))
                                f.close()
                            tcursor = self.msg_frame_group.textCursor()
                            image_show = QtGui.QTextImageFormat()
                            # 发送图片
                            if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                image_show.setName(path)
                                image_show.setHeight(100)
                                image_show.setWidth(100)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText("(预览)\n")
                                image_show.setName("./images_src/success.png")
                                image_show.setHeight(30)
                                image_show.setWidth(30)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText(
                                    "图片\"{filename}{filetype}\"已发送!\n".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]))
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            elif msg_recv["mtype"] == "biaoqingbao":
                                image_show.setName(path)
                                image_show.setHeight(60)
                                image_show.setWidth(60)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText("\n")
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            # 发送文件
                            else:
                                image_show.setName("./images_src/success.png")
                                image_show.setHeight(30)
                                image_show.setWidth(30)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText(
                                    "文件\"{filename}{type}\"已发送!\n".format(filename=msg_recv["fname"],type=msg_recv["mtype"]))
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                        # 私聊
                        else:
                            curent_private_msg_frame = self.msg_frames_private[msg_recv["destname"]]
                            curent_private_msg_frame.moveCursor(QtGui.QTextCursor.End)
                            curent_private_msg_frame.setTextColor(QColor(85,170,127))
                            curent_private_msg_frame.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            # 本地发送文件保存到用户文件夹
                            path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                            with open(path,"wb") as f:
                                f.write(base64.b64decode(msg_recv["msg"]))
                                f.close()
                            tcursor = curent_private_msg_frame.textCursor()
                            image_show = QtGui.QTextImageFormat()
                            # 发送图片
                            if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                image_show.setName(path)
                                image_show.setHeight(100)
                                image_show.setWidth(100)
                                tcursor.insertImage(image_show)
                                curent_private_msg_frame.insertPlainText("(预览)\n")
                                image_show.setName("./images_src/success.png")
                                image_show.setHeight(30)
                                image_show.setWidth(30)
                                tcursor.insertImage(image_show)
                                curent_private_msg_frame.insertPlainText(
                                    "图片\"{filename}{filetype}\"已发送!\n".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]))
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            elif msg_recv["mtype"] == "biaoqingbao":
                                image_show.setName(path)
                                image_show.setHeight(60)
                                image_show.setWidth(60)
                                tcursor.insertImage(image_show)
                                curent_private_msg_frame.insertPlainText("\n")
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            # 发送文件
                            else:
                                image_show.setName("./images_src/success.png")
                                image_show.setHeight(30)
                                image_show.setWidth(30)
                                tcursor.insertImage(image_show)
                                curent_private_msg_frame.insertPlainText(
                                    "文件\"{filename}{filetype}\"已发送!\n".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]))
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                    # 接收的
                    elif msg_recv["destname"] in (self.Username, "all"):  # 本地接收到的消息打印
                        # 群发
                        if msg_recv["destname"] == "all":
                            self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            self.msg_frame_group.setTextColor(QColor(62,62,185))
                            self.msg_frame_group.insertPlainText(
                                " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                            path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                            with open(path, "wb") as f:
                                f.write(base64.b64decode(msg_recv["msg"]))
                                f.close()
                            tcursor = self.msg_frame_group.textCursor()
                            image_show = QtGui.QTextImageFormat()
                            # 图片
                            if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                image_show.setName(path)
                                image_show.setHeight(100)
                                image_show.setWidth(100)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText("(预览)\n")
                                image_show.setName("./images_src/success.png")
                                image_show.setHeight(30)
                                image_show.setWidth(30)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText(
                                    "收到文件\"{filename}{filetype}\"\t".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]) 
                                        +"-saved in \""+path+"\".\n"
                                )
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            elif msg_recv["mtype"] == "biaoqingbao":
                                image_show.setName(path)
                                image_show.setHeight(60)
                                image_show.setWidth(60)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText("\n")
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                            # 文件
                            else:
                                image_show.setName("./images_src/success.png")
                                image_show.setHeight(30)
                                image_show.setWidth(30)
                                tcursor.insertImage(image_show)
                                self.msg_frame_group.insertPlainText(
                                    "收到文件\"{filename}{filetype}\"\t".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]) 
                                    +"-saved in \""+path+"\".\n")
                                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                        # 私聊
                        else:
                            who_send = msg_recv["name"]
                            if who_send in self.msg_frames_private:
                                curent_private_msg_frame = self.msg_frames_private[who_send]
                                curent_private_msg_frame.moveCursor(QtGui.QTextCursor.End)
                                curent_private_msg_frame.setTextColor(QColor(62,62,185))
                                curent_private_msg_frame.insertPlainText(
                                    " " + msg_recv["name"] + "  " + msgtime + "\n  ")
                                path = "./" + self.Username + "/" + msg_recv["fname"] + msg_recv["mtype"]
                                with open(path, "wb") as f:
                                    f.write(base64.b64decode(msg_recv["msg"]))
                                    f.close()
                                tcursor = curent_private_msg_frame.textCursor()
                                image_show = QtGui.QTextImageFormat()
                                # 图片
                                if msg_recv["mtype"] in (".png", ".gif", ".jpg"):
                                    image_show.setName(path)
                                    image_show.setHeight(100)
                                    image_show.setWidth(100)
                                    tcursor.insertImage(image_show)
                                    curent_private_msg_frame.insertPlainText("(预览)\n")
                                    image_show.setName("./images_src/success.png")
                                    image_show.setHeight(30)
                                    image_show.setWidth(30)
                                    tcursor.insertImage(image_show)
                                    curent_private_msg_frame.insertPlainText(
                                        "收到图片\"{filename}{filetype}\"\t".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]) 
                                        +"-saved in \""+path+"\".\n"
                                    )
                                    self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                                elif msg_recv["mtype"] == "biaoqingbao":
                                    image_show.setName(path)
                                    image_show.setHeight(60)
                                    image_show.setWidth(60)
                                    tcursor.insertImage(image_show)
                                    curent_private_msg_frame.insertPlainText("\n")
                                    self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                                # 文件
                                else:
                                    image_show.setName("./images_src/success.png")
                                    image_show.setHeight(30)
                                    image_show.setWidth(30)
                                    tcursor.insertImage(image_show)
                                    curent_private_msg_frame.insertPlainText(
                                        "收到文件\"{filename}{filetype}\"\t".format(filename=msg_recv["fname"],filetype=msg_recv["mtype"]) 
                                        +"-saved in \""+path+"\".\n")
                                    self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)

            while len(client.sysmsg):
                msg_recv = client.sysmsg.pop()
                if msg_recv["info"] == "userlogin":
                    if msg_recv["name"] not in client.userlist:
                        client.userlist.append(msg_recv["name"])
                        self.userlist_logined.clear()
                        self.userlist_logined.addItems(client.userlist)
                elif msg_recv["info"] == "userexit":
                    if msg_recv["name"] in client.userlist:
                        client.userlist.remove(msg_recv["name"])
                        self.userlist_logined.clear()
                        self.userlist_logined.addItems(client.userlist)
                        # 注释后，下次登录聊天记录可保持
                        # self.msg_frames_private.pop(msg_recv["name"])
                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
                self.msg_frame_group.setTextColor(Qt.gray)
                self.msg_frame_group.insertPlainText(msg_recv["msg"]+"------\n")
                self.msg_frame_group.moveCursor(QtGui.QTextCursor.End)
    
    def main(self):
        func1 = threading.Thread(target=self.display_recv_msgs)
        func1.start()

    def __del__(self): 
        self.s.close()
        self.dbconn.close()

    # 点击群聊
    def group_chat_button_clicked(self):
        # 隐藏所有私聊消息框
        for username in self.msg_frames_private:
            self.msg_frames_private[username].hide()
        self.msg_frame_group.setTextColor(Qt.gray)
        self.msg_frame_group.insertPlainText("------you are in the group chatting room~------\n")
        # 显示群聊消息框
        self.msg_frame_group.show()
        self.send_to_user = "all"

    # 点击发送
    def send_button_clicked(self):
        text_sent = self.edit_frame.toPlainText()
        # 无效，尚未解决
        if len(text_sent) == 0:
            self.send_button.setToolTip("不能发送空白信息")
        else:
            client.send_msg(text_sent,self.send_to_user)
            self.edit_frame.clear()

    # 显示/隐藏表情面板
    def emoji_button_clicked(self):
        if(self.emoji_table_show == False):
            self.emoji_table.show()
            self.emoji_table_show = True
            self.emoji_table.raise_()
        else:
            self.emoji_table.hide()
            self.emoji_table_show = False
        
    def emoji_table_clicked(self,row,col):
        client.send_msg(row*3+col,self.send_to_user,"emoji")
        self.emoji_table.hide()

    def file_button_clicked(self):
        file_open = self.openfile.getOpenFileName(self.centralwidget,'选取要发送的文件',"C:\ ")
        file_path = file_open[0]
        file_path_,file_type = os.path.splitext(file_path)
        filename = file_path_.split('/')[-1]
        if file_path is None or file_path == '':
            return
        with open(file_path, "rb") as f:
            file_ = f.read()
            file_encode = base64.encodebytes(file_).decode("utf-8")
            f.close()
        client.send_msg(file_encode,self.send_to_user,file_type,filename)

    def photo_button_clicked(self):
        file_open = self.openfile.getOpenFileName(self.centralwidget,'选取表情包(限正方形~)',"C:\ ","Image files (*.jpg *.gif *.png)")
        file_path = file_open[0]
        file_path_,file_type = os.path.splitext(file_path)
        filename = file_path_.split('/')[-1]
        if file_path is None or file_path == '':
            return
        with open(file_path, "rb") as image_file:
            f = image_file.read()
            image = base64.encodebytes(f).decode("utf-8")
            image_file.close()
        client.send_msg(image,self.send_to_user,"biaoqingbao",filename)



# 点击登录按钮，触发
def login_button_clicked(ui,loginWindow):
    username = ui.userName.text()
    password = ui.password.text()
    if len(username) == 0 or len(password) == 0:
        QMessageBox.about(loginWindow,"错误提示","请输入账号或密码再登录！")
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

        elif client.loginBack["info"] == "loginFail":
            QMessageBox.about(loginWindow,"错误提示","账号或密码错误！请重新输入！")
        else:
            QMessageBox.about(loginWindow,"错误提示","该账号已经登录！")
        client.loginBack = None

# 主界面，点注册
def register_button_clicked(loginWindow,client):
    # 调出注册界面
    loginWindow.register_window = QtWidgets.QMainWindow()
    register_window_ui = Ui_registerWindow(loginWindow.register_window) #控件
    loginWindow.register_window.show()
    loginWindow.register_window.raise_()
    register_window_ui.ok_register_button.clicked.connect(partial(ok_register_button_clicked,register_window_ui,loginWindow))

# 确定注册账号
def ok_register_button_clicked(ui,loginWindow):
    username = ui.username_register.text()
    password = ui.password_register.text()
    password_check = ui.password_register_check.text()
    if len(username) == 0 or len(password) == 0 or len(password_check) == 0:
        QMessageBox.about(loginWindow,"错误提示","请输入账号或密码再注册！")
        loginWindow.register_window.raise_()
    elif password != password_check:
        QMessageBox.about(loginWindow,"错误提示","两次输入密码不同，请重新输入！")
        loginWindow.register_window.raise_()
        ui.password_register.clear()
        ui.password_register_check.clear()
    else:
        client.register(username, password)
        while client.registerBack == None:
            pass
        if client.registerBack["info"] == "rgtrSucc":
            QMessageBox.about(loginWindow,"成功注册","成功注册账号\"{name}\",请返回登录！".format(name=username))
            loginWindow.register_window.raise_()
        else:
            QMessageBox.about(loginWindow,"错误提示","用户\"{name}\"已存在，请选择其他用户名！".format(name=username))
            loginWindow.register_window.raise_()
            ui.password_register.clear()
            ui.password_register_check.clear()
        client.registerBack = None


if __name__=='__main__':
    app = QtWidgets.QApplication(sys.argv)  #
    client = build_client(addr="localhost", port=15000)
    client.main()
    
    Mainwindow = QtWidgets.QMainWindow() #窗口实例
    ui = Ui_loginWindow() #登录窗口的控件实例
    ui.setupUi(Mainwindow) # 按格式建立客户端主窗口，后面的窗口做Mainwindow.的子窗口
    Mainwindow.show() 

    ui.login_button.clicked.connect(partial(login_button_clicked,ui,Mainwindow))
    ui.register_button.clicked.connect(partial(register_button_clicked,Mainwindow,client))

    # app.exec_： 让程序进入消息循环。等待可能的菜单,工具条,鼠标等的输入,进行响应。
    sys.exit(app.exec_()) 
    #使用的函数可以干净地退出，最后一个窗口关闭后,程序才停止。
