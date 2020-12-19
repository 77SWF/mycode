# -*- coding: UTF-8 -*-
# 文件名：Client.py

import time
import queue
import socket
import threading

class MyChat_Client(object):
    def __init__(self, addr="127.0.0.1", port=9999):
        """
        :param addr:客户端地址
        :param port:客户端端口
        """
        #感觉上行应该错了，是服务器的
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

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           #创建TCP Socket

        try:
            self.s.connect((self.addr, self.port)) #客户端的套接字连接到服务器的ip/port
            self.s.settimeout(0.000001)
        except socket.error as err:
            #连接被拒绝
            if err.errno == 10061:
                print("Connection with {addr}:{port} refused".format(addr=self.addr, port=self.port))
                return
            else:
                raise
        else: #无异常，跳过except
            print("initial successfully!")

    def register(self, name, password):
        """
        注册账号
        :param name:要注册的账号/用户名
        :param password:密码
        """
        # s已经connect到服务器的套接字了，发送给服务器，
        # 之前连接成功accept返回的c是绑定到客户端的(ip,port)的
        # 这个s在connect后也绑定到服务器的(ip,port)
        self.s.send(str({"type": "register",
                                "name": name,
                                "password": password,
                                "time": time.time()}).encode()) 

    def login(self, name, password):
        """
        使用账号密码登录
        :param name: 用户账号
        :param password: 密码
        """
        self.username = name 
        self.s.send(str({"type": "login",
                                "name": name,
                                "password": password,
                                "time": time.time()}).encode())

    def send_Msg(self, msg_send, destname, mtype = "msg", fname = ""):
        """
        发送消息
        :param msg_send: 要发送的消息
        :param destname: 发送对象的用户名
        """
        #消息编码成str
        a = str({"type": "usermsg",
                        "mtype": mtype,
                        "destname": destname,#接受方
                        "fname": fname,
                        "name": self.username, #发送方
                        "time": time.time(),
                        "msg": msg_send}).encode()
        constlen = len(a)

        self.s.send(str({"type": "msglen",
                                "destname": destname,
                                "name": self.username,
                                "len": constlen}).encode())
        print("send     ")
        print(str({"type": "msglen",
                          "destname": destname,
                          "name": self.username,
                          "len": constlen}).encode())
        #发送完长度信息后，停0.01s，发送消息a
        time.sleep(0.01)
        self.s.send(a)

    def receive_msg(self):
        """
        接收消息
        """
        while self.status:#有连接
            try:#从服务器接收消息，可能受长度msg/真正msg
                msg_recv = eval(self.s.recv(1024))
            except socket.timeout:
                pass
            except socket.error as err:
                #软件导致连接退出
                if err.errno == 10053:
                    print("Software caused connection abort ")
                    self.status = False
            else:#没有异常，正常接收，存在msg_recv
                #消息是另一个客户端发给服务器，服务器转发的，所以消息有这些字段
                #消息长度的msg

                #如果收长度msg
                if msg_recv["type"] == "msglen": 
                    self.queue.put(msg_recv)
                    print("recv             ")
                    length = msg_recv["len"]
                    mlen = 0 #已接受长度
                    while msg_recv["type"] != "usermsg":
                        try:
                            msg_recv = "".encode() #？

                            while mlen < length: #小于对方发来的长度
                                try:
                                    msg_recv_ = self.s.recv(length) #收真正数据
                                    msg_recv = msg_recv + msg_recv_
                                    mlen = mlen + len(msg_recv_)
                                    msg_recv = eval(msg_recv)
                                    time.sleep(length * 0.00000001)
                                except socket.timeout:
                                    continue
                                except SyntaxError:
                                    continue #继续循环，mlen=真正成功收到的长度，总的收到的不要大于length
                                else:
                                    break #终止循环
                        except socket.timeout:
                            continue
                        except socket.error as err:
                            if err.errno == 10053:
                                print("Software caused connection abort ")
                                self.status = False
                    self.queue.put(msg_recv)
                    print("recv             ")
                else:
                    self.queue.put(msg_recv)
                    print("recv             ")

    def handle_msg(self):
        """
        处理收到的消息
        """
        while True:
            msg = self.queue.get()
            print("handle              ",end='')

            if msg["type"] == "loginBack":
                self.loginBack = msg
                # 服务器发消息说登录成功
                if msg["info"] == "loginSucc":
                    # 用户名加进客户端的用户列表
                    self.userlist = msg["userlist"]
            elif msg["type"] == "rgtrBack": #注册信息
                self.registerBack = msg
            elif msg["type"] == "usermsg": #其他用户发的信息，存在list
                self.usermsg.append(msg)
            elif msg["type"] == "sysmsg":#系统信息？存在list
                self.sysmsg.append(msg)

    def main(self):
        """"""
        func1 = threading.Thread(target=self.receive_msg)
        func2 = threading.Thread(target=self.handle_msg)
        func1.start() #启动新线程，即它调用了线程的 run() 方法
        func2.start()

    def __del__(self): # 当对象被删除时，会自动被调用
        self.s.close()

if __name__ == '__main__':
    #客户端、服务器都是14396
    client = MyChat_Client(addr="127.0.0.1", port=14396)
    client.main() #打开接受消息、处理接收的消息的线程
    client.login("0", "0")
    # with open("./images/style/a.jpg", mode='rb') as f:
    #     r = f.read()
    #     image_str = base64.encodebytes(r).decode("utf-8")
    #     print(image_str)
    # client.send_Msg(image_str,"all","msg")


