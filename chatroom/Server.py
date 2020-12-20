# -*- coding: UTF-8 -*-
# 文件名：Server.py

import time
import queue
import socket
import sqlite3
import threading

class initial_server(object):
    def __init__(self, addr="localhost", port=12345):
        self.addr = addr
        self.port = port

        self.connections = []#已建立的连接
        self.name = {}#已建立连接的客户端套接字，ip:port字符串
        self.nametoconn = {}
        self.userlist = []#已登录用户
        self.queue = queue.Queue()

        self.dbconn = sqlite3.connect('UserInfo.db')#用户信息数据库
        self.dbcursor = self.dbconn.cursor()#创建数据库连接的游标
        #游标.执行sql语句
        self.dbcursor.execute('''CREATE TABLE IF NOT EXISTS USERINFO
               (USERNAME    VARCHAR(20) PRIMARY KEY     NOT NULL,
                PASSWORD    VARCHAR(20)                 NOT NULL,
                LASTLOGIN   VARCHAR(50)                 NOT NULL,
                STATUS      INT(1)                      NOT NULL
                               );''')
        self.dbcursor.execute("UPDATE USERINFO set STATUS = 0")
        self.dbconn.commit()

        # 服务器TCP套接字
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           
        self.s.bind((self.addr, self.port))                                 
        print("Server is listening on " + str(port) + "...")

    def listen_to_port(self):
        # 建立的连接s监听TCP传入连接
        self.s.listen(10)                           
        while True:
            # 新连接是真正的连接，s还在监听
            connection_new, address = self.s.accept()        
            connection_new.settimeout(0.000001) #建立的这个链接的超时时间
            add = address[0] + ":" + str(address[1]) #IP套接字，地址信息是(ip, port)
            # 已建立的连接
            self.connections.append(connection_new)           
            # key=客户端套接字，value=用户名
            self.name[add] = add

    def msg_queue(self):
        while True:
            # 遍历已建立的连接，
            for c in self.connections:
                try:
                    # 从这个连接接受msg
                    msg_recv = eval(c.recv(1024))
                # 客户端接受数据产生超时异常
                except socket.timeout: 
                    continue
                except SyntaxError:
                    pass
                except socket.error as err:
                    # 软件导致连接退出/连接被重置
                    if err.errno == 10053 or err.errno == 10054:
                        self.remove_connection(c) 
                except ValueError:
                    pass
                # 正常收到信息
                else:
                    # 这个连接的(ip,port)
                    addr = c.getpeername() 
                    # 消息入队：(发方套接字,消息,发方的连接)
                    self.queue.put((addr, msg_recv, c)) 
                    if msg_recv["type"] == "msglen":
                        length = msg_recv["len"]
                        time.sleep(length * 0.0000001) 
                        received_length = 0
                        while msg_recv["type"] != "usermsg":
                            try:
                                msg_recv = "".encode()
                                # 直至收到事先给予的长度
                                while received_length < length:       
                                    try:
                                        msg_recv_ = c.recv(length) #收给定长度的
                                        msg_recv = msg_recv + msg_recv_ #收到的总消息
                                        received_length = received_length + len(msg_recv_) #已收到的长度
                                        msg_recv = eval(msg_recv) #收到的总消息
                                        time.sleep(length * 0.00000001)
                                    #超时，继续收
                                    except socket.timeout:
                                        continue
                                    except SyntaxError:
                                        continue
                                    # 收到给定长度
                                    else: 
                                        break
                            except socket.timeout:
                                continue
                            except socket.error as err:
                                if err.errno == 10053 or err.errno == 10054:
                                    self.remove_connection(c)
                            except ValueError:
                                pass
                        # length长的消息是用户消息，入队
                        self.queue.put((addr, msg_recv, c))

    def login_request(self, msg_recv, socket_tuple):
        username = msg_recv["name"]
        self.dbcursor.execute("SELECT * from USERINFO where USERNAME = \"{Uname}\"".format(Uname = username))      #通过用户名检索出对应的用户实体
        # 查询结果的下一整行
        Userinfo = self.dbcursor.fetchone()
        
        # 判断是否登录成功，给客户端反馈
        # 用户不存在/密码错误
        if Userinfo == None or Userinfo[1] != msg_recv["password"]:        
            flag = False
            back = {"type": "loginBack",
                    "name":"Server",
                    "info": "loginFail"}
        # 用户状态=1
        elif Userinfo[3] == 1:
            flag = False
            back = {"type": "loginBack",
                    "name":"Server",
                    "info": "loginAlready"}
        # 登录成功
        else:
            flag = True
            address = socket_tuple[0] + ":" + str(socket_tuple[1])
            # key=登录用户的套接字，value=用户名
            self.name[address] = username
            self.userlist.append(username)
            # 这个用户的LASTLOGIN字段(数据库里)
            self.lastlogintime = Userinfo[2]
            # 更新LASTLOGIN=这次登录的时间
            self.dbcursor.execute("UPDATE USERINFO set LASTLOGIN = {logintime}, STATUS = 1 where USERNAME=\"{Uname}\"".format(
                logintime = time.time(),           #time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(msg_recv["time"])),
                Uname = username))
            self.dbconn.commit()
            # 成功登录,反馈给客户端
            back = {"type": "loginBack",
                    "name":"Server",
                    "info": "loginSucc",
                    "userlist": self.userlist}
            forward = {"type": "sysmsg",
                       "info": "userlogin",
                       "name": username,
                       "time": time.time(),
                       "msg": ">> {name} enters Chatting Room".format(name=username)}

        for connection in self.connections:
            # 这个连接的客户端套接字
            connection_socket = connection.getpeername()
            # 这次登录的用户的套接字，和已连接的某个套接字一样
            if connection_socket == socket_tuple:
                # 登录成功
                if flag:
                    # dict:用户名——>连接
                    self.nametoconn[self.name[address]] = connection #key=用户名，value=连接
                connection.send(str(back).encode())
            # 这次登录的用户的套接字是新的
            elif flag:
                connection.send(str(forward).encode())

    def register_request(self, msg_recv, register_socket):
        # 要注册的用户名
        username = msg_recv["name"]
        # 查已有用户名
        self.dbcursor.execute("SELECT * from USERINFO where USERNAME=\"{Uname}\"".format(Uname = username))  # 通过用户名检索出对应的用户实体
        Userinfo = self.dbcursor.fetchone()

        # 用户名不存在
        if Userinfo == None:        
            # 用户信息存储
            self.dbcursor.execute("INSERT INTO USERINFO (USERNAME, PASSWORD, LASTLOGIN, STATUS) \
            VALUES (\"{Uname}\", \"{Passwd}\", \"Never\", 0)".format(Uname = username, Passwd = msg_recv["password"]))
            self.dbconn.commit()
            # key=套接字，value=用户名
            self.name[register_socket] = username
            self.lastlogintime = "Never"
            back = {"type": "rgtrBack",
                    "name":"Server",
                    "info": "rgtrSucc"}
        # 用户已存在
        else:
            back = {"type": "rgtrBack",
                    "name":"Server",
                    "info": "rgtrFail"}
        
        # 查找转发对象连接
        for connection in self.connections:
            connection_socket = connection.getpeername()
            if connection_socket == register_socket:
                connection.send(str(back).encode())

    # 删除连接、用户名-套接字列表name、已登录用户名列表userlist、修改数据库的用户状态、通知其他连接的套接字
    def remove_connection(self, removed_connection):
        try:
            self.connections.remove(removed_connection) 
        except ValueError:
            pass
        address = removed_connection.getpeername() 
        socket_str = address[0] + ":" + str(address[1]) 
        username = self.name[socket_str]
        self.name.pop(socket_str)
        # 若用户已登录
        if username in self.userlist:
            self.userlist.remove(username)
        # 用户信息数据库，修改用户状态
        dbconn1 = sqlite3.connect('userinfo.db') 
        dbcursor1 = dbconn1.cursor()
        dbcursor1.execute("UPDATE USERINFO set STATUS=0 where USERNAME=\"{Uname}\"".format(Uname=username))
        dbconn1.commit()
        back = {"type": "sysmsg",
                "info": "userexit",
                "name": username,
                "time": time.time(),
                "msg": "<< {name} exits Chatting Room.".format(name=username)}
        # 通知其他用户
        for c in self.connections:
            c.send(str(back).encode())

    def msg_forward(self, msg_forward, addr):
        socket_str = addr[0] + ":" + str(addr[1]) 

        if msg_forward["destname"] == "all":
            #给所有连接了的客户端转发msg
            for c in self.connections:
                print("forward     ")
                c.send(str(msg_forward).encode())
        else:
            #要转发的用户名的连接
            self.nametoconn[msg_forward["destname"]].send(str(msg_forward).encode())
            self.nametoconn[msg_forward["name"]].send(str(msg_forward).encode())
            print("forward     ")

    # 监听、消息处理线程
    def run(self):
        func1 = threading.Thread(target=self.listen_to_port)
        func2 = threading.Thread(target=self.msg_queue)
        func1.start()
        func2.start()
        while True:
            # 消息队列空
            if self.queue.empty():
                continue
            # 非空，取一条
            addr, msg, conn = self.queue.get()

            # 3种消息类型，使用3种处理函数
            # 登录：账号密码
            if msg["type"] == "login":
                self.login_request(msg, addr)
            # 转发信息
            elif msg["type"] in ("usermsg", "msglen"):
                self.msg_forward(msg, addr)
            # 注册：用户名、密码
            elif msg["type"] == "register":
                self.register_request(msg, addr)

if __name__ == '__main__':
    server = initial_server(addr="127.0.0.1", port=15000)
    server.run()

