# -*- coding: UTF-8 -*-

import time
import queue
import socket
import threading

class build_client(object):
    # 连接服务器
    def __init__(self, addr="127.0.0.1", port=9999):
        self.addr = addr
        self.port = port
        self.username = None
        self.queue = queue.Queue()
        self.status = True
        self.loginStatus = False
        self.loginBack = None
        self.registerBack = None
        self.userlist = []
        self.usermsg = []
        self.sysmsg = []

        #客户端TCP套接字
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           
        
        #客户端的套接字连接到服务器的ip/port
        try:
            self.s.connect((self.addr, self.port)) 
            self.s.settimeout(0.000001)
        except socket.error as err:
            # 连接被拒绝
            if err.errno == 10061:
                print("Connection with {addr}:{port} was refused".format(addr=self.addr, port=self.port))
                return
            else:
                raise
        # 成功连接
        else: 
            print("Successfully connect to Server {addr}:{port}...".format(addr=self.addr, port=self.port))

    # 向服务器注册账号
    def register(self, name, password):
        self.s.send(str({"type": "register",
                                "name": name,
                                "password": password,
                                "time": time.time()}).encode()) 

    # 向服务器请求登录
    def login(self, name, password):
        self.username = name 
        self.s.send(str({"type": "login",
                                "name": name,
                                "password": password,
                                "time": time.time()}).encode())

    # 发送消息
    def send_msg(self, msg_send, destname, mtype = "msg", fname = ""):
        # 字典消息编码成str，send参数str
        msg_info = str({"type": "usermsg",
                        "mtype": mtype,
                        "destname": destname,#接受方
                        "fname": fname,
                        "name": self.username, #发送方
                        "time": time.time(),
                        "msg": msg_send}).encode()
        msg_length = len(msg_info)

        msg_length_info = str({"type": "msglen",
                                "destname": destname,
                                "name": self.username,
                                "len": msg_length}).encode()

        self.s.send( msg_length_info )

        print("send a message to {username}. ".format(username=destname))
        #发送完长度信息后，停0.01s，发送消息a
        time.sleep(0.01)
        self.s.send(msg_info)

    # 接收消息
    def receive_msg(self):
        #有连接
        while self.status:
            #从服务器接收消息，长度msg/真正msg
            try:
                msg_recv = eval(self.s.recv(1024))
            except socket.timeout:
                pass
            except SyntaxError:
                continue
            except socket.error as err:
                #软件导致连接退出
                if err.errno == 10053:
                    print("Software caused connection abort. ")
                    self.status = False
            # 正常接收
            else:
                # 长度msg
                if msg_recv["type"] == "msglen": 
                    self.queue.put(msg_recv)
                    print("receive a message from {username}.".format(username=msg_recv["name"]))
                    length = msg_recv["len"]
                    received_length = 0 #已接受长度
                    while msg_recv["type"] != "usermsg":
                        try:
                            msg_recv = "".encode() 

                            # 已收信息小于对方发来的长度
                            while received_length < length: 
                                try:
                                    # 收指定长度真正数据
                                    msg_recv_ = self.s.recv(length) 
                                    msg_recv = msg_recv + msg_recv_
                                    received_length = received_length + len(msg_recv_)
                                    msg_recv = eval(msg_recv)
                                    time.sleep(length * 0.00000001)
                                except socket.timeout:
                                    continue
                                except SyntaxError:
                                    #继续循环，received_length=真正成功收到的长度，总的收到的不要大于length
                                    continue 
                                else:
                                    break #终止循环
                        except socket.timeout:
                            continue
                        except socket.error as err:
                            if err.errno == 10053:
                                print("Software caused connection abort ")
                                self.status = False
                    self.queue.put(msg_recv)
                    print("receive a message from {username}.".format(username=msg_recv["name"]))
                # 真正消息
                else:
                    self.queue.put(msg_recv)
                    print("receive a message from {username}.".format(username=msg_recv["name"]))

    # 客户端从服务器接收消息，待用户界面处理
    def handle_msg(self):
        while True:
            msg = self.queue.get()
            print("handle a message in queue.")

            if msg["type"] == "loginBack":
                self.loginBack = msg
                # 服务器发消息，登录成功
                if msg["info"] == "loginSucc":
                    # 用户名加进客户端的用户列表
                    self.userlist = msg["userlist"]
            # 注册信息
            elif msg["type"] == "rgtrBack": 
                self.registerBack = msg
            # 其他用户发的信息
            elif msg["type"] == "usermsg": 
                self.usermsg.append(msg)
            # 系统信息
            elif msg["type"] == "sysmsg":
                self.sysmsg.append(msg)

    # 客户端子线程
    def main(self):
        func1 = threading.Thread(target=self.receive_msg)
        func2 = threading.Thread(target=self.handle_msg)
        # 启动新线程，调用线程的 run()
        func1.start() 
        func2.start()

    # 客户端删除时，自动断开连接
    def __del__(self): 
        self.s.close()



