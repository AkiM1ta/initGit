/*********************************************
 * function:根据套接字建立TCP 连接的过程，创建客户端程序，向服务器发送连接请求，连接后向服务器发送字符串并保持连接
 * time:2016-5-27
 ***********************************************/
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<sys/un.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<pthread.h>
#include<signal.h>
#include<arpa/inet.h>
int main(int argc,char *argv[])
{
    int connect_fd;
    int ret;
    char snd_buf[1024];
    int i;
    int port;
    int len;
    static struct sockaddr_in srv_addr;
    if(argc!=3)
    {
        printf("Usage: %s server_ip_address port\n",argv[0]);
        return 1;
    }    
    port=atoi(argv[2]);
    connect_fd=socket(PF_INET,SOCK_STREAM,0);
    if(connect_fd<0)
    {
        perror("cannot create communication socket");
        return 1;
    }
    memset(&srv_addr,0,sizeof(srv_addr));
    srv_addr.sin_family=AF_INET;
    srv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    srv_addr.sin_port=htons(port);
    //连接指定服务器
    ret=connect(connect_fd,(struct sockaddr *)&srv_addr,sizeof(srv_addr));
    if(ret==-1)
    {
        perror("cannot connect to the server");
        close(connect_fd);
        return 1;
    }
    memset(snd_buf,0,1024);
    //当用户输入信息后，程序将输入的信息通过套接字发送给服务器
    //然后调用read 函数从服务器中读取发送过来的信息
    //当输入'@' 时，程序退出
    while(1)
    {
        write(STDOUT_FILENO,"input message:",14);
        len=read(STDIN_FILENO,snd_buf,1024);
        if(len>0)
            write(connect_fd,snd_buf,len);
        len=read(connect_fd,snd_buf,len);
        if(len>0)
            printf("Message from server: %s\n",snd_buf);
        if(snd_buf[0]=='@')
            break;
    }
    close(connect_fd);
    return 0;
}
