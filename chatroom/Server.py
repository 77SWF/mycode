# -*- coding: UTF-8 -*-
# 文件名：Server.py

import time
import queue
import socket
import sqlite3
import threading

class MyChat_Server(object):
    def __init__(self, addr="localhost", port=12345):#addr仍要赋值
        self.addr = addr
        self.port = port

        self.connections = []#已建立的连接
        self.name = {}#已建立的连接的客户端套接字，ip:port字符串
        self.nametoconn = {}
        self.userlist = []#用户列表
        self.queue = queue.Queue()

        self.dbconn = sqlite3.connect('UserInfo.db')#用户信息数据可
        self.dbcursor = self.dbconn.cursor()#创建数据库连接的游标
        self.dbcursor.execute('''CREATE TABLE IF NOT EXISTS USERINFO
               (USERNAME    VARCHAR(20) PRIMARY KEY     NOT NULL,
                PASSWORD    VARCHAR(20)                 NOT NULL,
                LASTLOGIN   VARCHAR(50)                 NOT NULL,
                STATUS      INT(1)                      NOT NULL
                               );''')#游标.执行sql语句
        self.dbcursor.execute("UPDATE USERINFO set STATUS = 0")
        self.dbconn.commit()

        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)           #创建TCP Socket
        self.s.bind((self.addr, self.port))                                 #将套接字绑定到地址
        print("Initial Successfully!")

    def portlisten(self):
        """
        侦听连接
        """
        self.s.listen(10)                           #建立的连接s监听TCP传入连接
        while True:#持续执行
             #接受TCP连接，并返回（conn,address），新连接是真正的连接，s还在监听
            conn, address = self.s.accept()        
            conn.settimeout(0.000001) #建立的这个链接的超时时间
            add = address[0] + ":" + str(address[1]) #IP套接字，地址信息是(ip, port)
            self.connections.append(conn)           #存储已建立的连接
            # 存已建立的连接的客户端套接字对应的用户名
            # key=套接字，value应该是username？这里只是初始化
            self.name[add] = add

    def msg_queue(self):
        """
        消息队列
        """
        #一直不断遍历各个和服务器的连接
        while True:
            #遍历已建立的各个客户端的连接，
            for c in self.connections:
                try:
                    #从这个连接接受msg
                    msg_recv = eval(c.recv(1024))#？？eval计算字符串表达式的值
                except socket.timeout: #客户端接受数据产生超时异常
                    continue
                except SyntaxError:
                    pass
                except socket.error as err:     #10053 – Software caused connection abort //10054 – connection reset by peer
                    # 软件导致连接退出/连接被重置
                    if err.errno == 10053 or err.errno == 10054:
                        #删除连接、用户名-套接字列表name、已登录用户名列表userlist
                        #修改数据库的用户状态、通知其他连接的套接字
                        self.remove_connection(c) 
                except ValueError:
                    pass #不pass会抛出异常
                #正常收到信息，在msg_recv；try无异常，忽略except，执行else
                else:
                    addr = c.getpeername() #这个连接的(ip,port)
                    #消息msg_recv入队，元组(发送方套接字,消息,发送方的连接)
                    self.queue.put((addr, msg_recv, c)) 
                    #如果try收到的msg_recv是消息长度
                    if msg_recv["type"] == "msglen":
                        length = msg_recv["len"] #消息是dict形式的
                        time.sleep(length * 0.0000001) 
                        mlen = 0
                        #如若不是用户传给用户的信息；直到收到的指定长度的msg_recv是用户消息
                        while msg_recv["type"] != "usermsg":
                            try:
                                msg_recv = "".encode() #清空消息长度的这个msg_recv
                                #起初收到的信息总长mlen=0，会执行
                                while mlen < length:        #直至收到事先给予的长度才停下！
                                    try:
                                        msg_recv_ = c.recv(length) #收给定长度的
                                        msg_recv = msg_recv + msg_recv_ #收到的总消息
                                        mlen = mlen + len(msg_recv_) #已收到的长度
                                        msg_recv = eval(msg_recv) #收到的总消息
                                        time.sleep(length * 0.00000001)
                                    #超时，继续收，加到已收信息后
                                    except socket.timeout:
                                        continue
                                    except SyntaxError:
                                        continue
                                    else: #try没有异常，即收到给定长度了
                                        break
                            except socket.timeout:
                                continue
                            except socket.error as err:  # 10053 – Software caused connection abort //10054 – connection reset by peer
                                if err.errno == 10053 or err.errno == 10054:
                                    self.remove_connection(c)
                            except ValueError:
                                pass
                        # 取到的length长的消息是用户消息，就入队
                        self.queue.put((addr, msg_recv, c))

    def loginMychat(self, msg_recv, addr):
        """
        客户端发来login消息msg_recv，要登录到服务器
        :param msg_recv:收到的消息
        :param addr:
        """
        #客户端send来的消息里有用户名
        Username = msg_recv["name"]
        # MyChat_Server(addr="127.0.0.1", port=14396)创建服务器实例时
        # 就自动调用init，创建了类属性dbcursor，查询后在dbcursor有了结果集，不需返回值
        self.dbcursor.execute("SELECT * from USERINFO where USERNAME = \"{Uname}\"".format(Uname = Username))      #通过用户名检索出对应的用户实体
        # 查询结果的下一行，整行
        Userinfo = self.dbcursor.fetchone()
        
        # 下面3个if判断是否登录成功，并给客户端反馈
        # #用户不存在或密码错误
        if Userinfo == None or Userinfo[1] != msg_recv["password"]:        
            flag = False
            back = {"type": "loginBack",
                    "info": "loginFail"}
        # 用户状态=1，
        elif Userinfo[3] == 1:
            flag = False
            back = {"type": "loginBack",
                    "info": "loginAlready"}
        # 登录成功
        else:
            flag = True
            address = addr[0] + ":" + str(addr[1])

            # name：key=登录用户的套接字，value=用户名
            self.name[address] = Username
            # userlist：登录用户list
            self.userlist.append(Username)
            # 这个用户的LASTLOGIN字段(数据库里)
            self.lastlogintime = Userinfo[2]
            # 更新LASTLOGIN=这次登录的时间
            self.dbcursor.execute("UPDATE USERINFO set LASTLOGIN = {logintime}, STATUS = 1 where USERNAME=\"{Uname}\"".format(
                logintime = time.time(),           #time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(msg_recv["time"])),
                Uname = Username))
            # 提交更改
            self.dbconn.commit()
            # 成功登录反馈给客户端
            back = {"type": "loginBack",
                    "info": "loginSucc",
                    # 当前在线的用户list
                    "userlist": self.userlist}
            # 成功登录后，在客户端显示系统提示 "欢迎您来到..."
            forward = {"type": "sysmsg",
                       "info": "userlogin",
                       "name": Username,
                       "time": time.time(),
                       "msg": "Welcome {name} to MyChat~".format(name=Username)}

        # 遍历已有的每个和服务器的连接
        for c in self.connections:
            # 这个连接的客户端套接字
            c_addr = c.getpeername()
            # 这次登录的用户的套接字，和已连接的某个套接字一样
            if c_addr == addr:
                # 如果登录成功
                if flag:
                    # 存到用户名——>连接的dict，nametoconn
                    self.nametoconn[self.name[address]] = c #key=用户名，value=连接
                c.send(str(back).encode())
            # 这次登录的用户的套接字是新的
            elif flag:
                c.send(str(forward).encode())

    def registerMychat(self, msg_recv, addr):
        """
        注册MyChat账号
        :param msg_recv:收到的消息
        :param addr:
        """

        # 要注册的用户名
        Username = msg_recv["name"]
        # 查已有用户名
        self.dbcursor.execute("SELECT * from USERINFO where USERNAME=\"{Uname}\"".format(Uname = Username))  # 通过用户名检索出对应的用户实体
        Userinfo = self.dbcursor.fetchone()

        # 用户名不存在
        # 没有这行
        if Userinfo == None:        
            # 这个用户的信息存进数据库
            self.dbcursor.execute("INSERT INTO USERINFO (USERNAME, PASSWORD, LASTLOGIN, STATUS) \
            VALUES (\"{Uname}\", \"{Passwd}\", \"Never\", 0)".format(Uname = Username, Passwd = msg_recv["password"]))
            self.dbconn.commit()

            # key=套接字，value=用户名
            self.name[addr] = Username
            # ？
            self.lastlogintime = "Never"
            back = {"type": "rgtrBack",
                    "info": "rgtrSucc"}
        # 用户已存在：查询结果行非空
        else:
            back = {"type": "rgtrBack",
                    "info": "rgtrFail"}
        
        # 遍历各个已有连接，否则不知道用哪个连接发信息
        # 客户端初始化时就创建connect连接！只是可能没有用户登录！
        for c in self.connections:
            # 每个连接的套接字
            c_addr = c.getpeername()
            # 已有连接的套接字和这个连接一样
            if c_addr == addr:
                # 用这个连接c发send反馈信息
                c.send(str(back).encode())

    def remove_connection(self, conn):#参数：要删除的连接c
        '''
        删除连接、用户名-套接字列表name、已登录用户名列表userlist、修改数据库的用户状态、通知其他连接的套接字
        用户退出，删除连接并广播
        :param conn: 要从表中删除的连接
        '''
        try:#从已建立连接list删除
            #还是在服务器class里，所以self
            #从list删除元素
            self.connections.remove(conn) 
        except ValueError:
            pass
        address = conn.getpeername() 
        addr = address[0] + ":" + str(address[1]) #ip:端口，字符串
        Username = self.name[addr] #{套接字:套接字}用左边查右边
        self.name.pop(addr)     #从用户列表中移除：删除这个套接字对应的用户
        if Username in self.userlist:#用户登录的时候才append进userlist，连接建立不一定有用户登录
            self.userlist.remove(Username)
        #创建服务器实例就init建立userinfo.db了，直接打开
        dbconn1 = sqlite3.connect('userinfo.db') 
        dbcursor1 = dbconn1.cursor()
        #修改用户状态
        dbcursor1.execute("UPDATE USERINFO set STATUS=0 where USERNAME=\"{Uname}\"".format(Uname=Username))
        dbconn1.commit()
        back = {"type": "sysmsg",
                "info": "userexit",
                "name": Username,
                "time": time.time(),
                "msg": "{name} Exits MyChat~".format(name=Username)}
        #给其他所有连接发送信息：
        for c in self.connections:
            c.send(str(back).encode()) #send的参数是str，encode默认UTF-8，返回编码后的字符串。

    def msg_forward(self, msg_forward, addr):
        """
        消息转发
        """
        address = addr[0] + ":" + str(addr[1]) #地址写成 ip:port
        if msg_forward["destname"] == "all":#转发的信息msg是dict，destname是？
            #给所有连接了的客户端转发msg
            for c in self.connections:
                print("forward     ")
                c.send(str(msg_forward).encode())
        else:
            #要转发的用户名的连接
            self.nametoconn[msg_forward["destname"]].send(str(msg_forward).encode())
            self.nametoconn[msg_forward["name"]].send(str(msg_forward).encode())
            print("forward     ")

    def run(self):
        """"""
        # 启动监听、消息处理线程
        func1 = threading.Thread(target=self.portlisten)
        func2 = threading.Thread(target=self.msg_queue)
        func1.start()
        func2.start()
        while True:
            # 消息队列空
            if self.queue.empty():
                # 下一次循环
                continue
            # 非空，取第一个消息
            addr, msg, conn = self.queue.get()

            # 3种消息类型，使用3种处理函数
            # 客户端用户登录，传来账号密码
            if msg["type"] == "login":
                # 登录函数
                self.loginMychat(msg, addr)
            # 传给另一个用户的信息/要传的长度
            elif msg["type"] in ("usermsg", "msglen"):
                # 转发函数
                self.msg_forward(msg, addr)
            # 客户端用户注册，传来用户名、密码
            elif msg["type"] == "register":
                # 调用注册函数
                self.registerMychat(msg, addr)

    # 当对象被删除时，会自动被调用            
    def __del__(self): 
        self.s.close()
        self.dbconn.close()

if __name__ == '__main__':
    server = MyChat_Server(addr="127.0.0.1", port=14396)
    server.run()

