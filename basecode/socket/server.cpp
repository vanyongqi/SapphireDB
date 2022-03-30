#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>//sockaddr_in
#include<arpa/inet.h>//inet_addr
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<thread>
#include<iostream>
/*
sys/types.h：数据类型定义
sys/socket.h：提供socket函数及数据结构
netinet/in.h：定义数据结构sockaddr_in
arpa/inet.h：提供IP地址转换函数
netdb.h：提供设置及获取域名的函数
sys/ioctl.h：提供对I/O控制的函数
sys/poll.h：提供socket等待测试机制的函数
其他在网络程序中常见的头文件
unistd.h：提供通用的文件、目录、程序及进程操作的函数
errno.h：提供错误号errno的定义，用于错误处理
fcntl.h：提供对文件控制的函数
time.h：提供有关时间的函数
crypt.h：提供使用DES加密算法的加密函数
pwd.h：提供对/etc/passwd文件访问的函数
shadow.h：提供对/etc/shadow文件访问的函数
pthread.h：提供多线程操作的函数
signal.h：提供对信号操作的函数
sys/wait.h、sys/ipc.h、sys/shm.h：提供进程等待、进程间通讯(IPC)及共享内存的函数
*/
#define PORT 7777
int conn;
#define QUEUE 20//连接请求队列
void thread_task(){

}


int main(){
    printf("%d\n",SOCK_STREAM);//TCP
    //int socket(int domain, int type, int protocol);
    int fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd,(struct sockaddr*)& server_sockaddr,sizeof(server_sockaddr))== -1){

    }
    if(listen(fd,QUEUE)== -1){

    }
    struct sockaddr_in client_addr;
    socklen_t length =sizeof(client_addr);

    conn = accept(fd,(struct sockaddr*)&client_addr,&length);

    if(conn <0){

    }

    char buffer[1024];
    memset(buffer, 0 ,sizeof(buffer));
    int len = recv(conn, buffer, sizeof(buffer), 0);
    printf("%s",buffer);
    send(conn, buffer, len , 0);//向TCP连接的另一端发送数据。

    
    close(conn);//因为accpet函数连接成功后还会生成一个新的套接字描述符，结束后也需要关闭
    close(fd);//关闭socket套接字描述符
    return 0;
}
/*

domain:
    AF_INET      IPv4 Internet protocols 
    AF_INET6     IPv6 Internet protocols                    ipv6(7)
type:
        SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based 
byte streams.  An out-of-band data transmission mechanism may be  sup‐
                       ported.

       SOCK_DGRAM      Supports datagrams (connectionless, unreliable messages 
of a fixed maximum length).
*/