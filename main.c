//not completed yet.2015-05-30
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<arpa/inet.h>

#define PORT 9090                               //use port 9090
#define MAXCNT 10                               //maximal connections

int main()
{
    int sockfd,newsockfd,is_connected[MAXCNT],fd;
    struct sockaddr_in addr;
    int addr_len=sizeof(struct sockaddr_in);
    fd_set myreadfds;
    char msg[]="<html><body><a href=\"www.pku.edu.cn\"><a></body></html>";
    char msgbuffer[256];
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket:");
        exit(1);
    }
    else
    {
        printf("socket created\n");
        printf("socked id:\n",sockfd);
    }

    memset(&addr,'\0',sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sockfd,(struct sockaddr *)&addr,sizeof(addr))<0)
    {
        perror("bind:");
        exit(1);
    }

    else
    {
        printf("connected.\nlocal port:%d\n",PORT);
    }

    if(listen(sockfd,3)<0)
    {
        perror("listen:");
        exit(1);
    }
    else
    {
        printf("listening....\n");
    }
    for(fd=0;fd<MAXCNT;fd++)
    {
        is_connected[fd]=0;
    }

    while(1)
    {
        FD_ZERO(&myreadfds);
        FD_SET(sockfd,&myreadfds);
        for(fd=0;fd<MAXCNT;fd++)
        {
            if(is_connected[fd])
            {
                FD_SET(fd,&myreadfds);
            }
        }
        if(!select(MAXCNT,&myreadfds,NULL,NULL,NULL))
        {
            continue;
        }
        for(fd=0;fd<MAXCNT;fd++)
        {
            if(FD_ISSET(fd,&myreadfds))
            {
                if(fd==sockfd)
                {
                    if((newsockfd=accept(sockfd,(struct sockaddr *)&addr,&addr_len))<0)
                    {
                        perror("accept:");
                    }
                    write(newsockfd,msg,sizeof(msg));
                    is_connected[newsockfd]=1;
                    printf("connect from %s\n",inet_ntoa(addr.sin_addr));
                }
                else
                {
                    memset(msgbuffer,'\0',sizeof(msgbuffer));
                    if(read(fd,msgbuffer,sizeof(msgbuffer))<=0)
                    {
                        printf("connect closed.\n");
                        is_connected[fd]=0;
                        close(fd);
                    }
                    else
                    {
                        write(fd,msg,sizeof(msg));
                        printf("message:%s\n",msgbuffer);
                    }
                }
            }
        }
    }


    return 0;
}
