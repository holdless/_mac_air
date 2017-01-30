//
//  TCPServer.m
//  iOS_FSK_TCP_Server
//
//  Created by Hiroshi on 10/19/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import "TCPServer.h"
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 6677
#define MAXDATASIZE 100
#define LENGTH_OF_LISTEN_QUEUE  20
#define THREAD_MAX    5
NSLock *lock;

@implementation TCPServer
/*
- (id)init
{
    if (self = [super init])
    {
        [self initServer];
    }
    return self;
}
*/
- (void)initServer
{
    // 1. declare a socket-address-struct "server_addr" to present server's address & port
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    
    // 2. create TCP socket, use "server_socket" as server's socket
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if( server_socket < 0)
    {
        printf("TCP:Create Socket Failed!");
        exit(1);
    }
    
    // 3. bind socket with socket-address-struct
    if( bind(server_socket,
             (struct sockaddr*)&server_addr,
             sizeof(server_addr)))
    {
        printf("TCP:Server Bind Port : %d Failed!", PORT);
        exit(1);
    }
    
    // 4. server_socket for "listening"
    if ( listen(server_socket,
                LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("TCP:Server Listen Failed!");
        exit(1);
    }
    
    isClosed = NO;
    
    Byte clientNo = 0;
    while(!isClosed) // Server must keep running
    {
        clientNo++;
        
        printf("TCP:Server start......\n");
        // 5.1 define client's socket-address-struct "client_addr"
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        
        //接受一个到server_socket代表的socket的一个连接
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性 (block mode)
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
        //new_server_socket代表了服务器和客户端之间的一个通信通道
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中
        int new_client_socket;
        new_client_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_client_socket < 0)
        {
            printf("TCP:Server Accept Failed!/n");
            break;
        }
        
        printf("TCP:one client connted..\n");
        
        // send back "connection OK" message to client
        write(new_client_socket, "TCP:connection OK\n", strlen("TCP:connection OK\n"));
        // read message from client
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            
            [self readData:[NSNumber numberWithInt:new_client_socket]];
            
        });

        // refresh UI (back to main thread)
        dispatch_async(dispatch_get_main_queue(), ^{
            

            
        });
        
        //        [NSThread detachNewThreadSelector:@selector(readData:)
        //                                 toTarget:self
        //                               withObject:[NSNumber numberWithInt:new_client_socket]];
    }
    //关闭监听用的socket
    close(server_socket);
    NSLog(@"%@",@"TCP:server closed....");
}

// read client message
-(void) readData:(NSNumber*) clientSocket // clientSocket為一與client之間的通道代號
{
//    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    int intSocket = [clientSocket intValue];
    
    while(buffer[0] != '-')
    {
        bzero(buffer, BUFFER_SIZE);
        // receive msg from client and put into buffer (also a block action...)
        long readSize = recv(intSocket,
                             buffer,
                             BUFFER_SIZE,
                             0);
        
        
        if (readSize > 0)
        {
            // 20141105, for multi-client
            self.currentSocket = intSocket;
            
            printf("TCP:client:%s\n",buffer);
            
            // 20141019, hiroshi: add KVO key here for view controller to observe.
            self.infoCome = !self.infoCome;

            //Send the message back to client
//            write(intSocket , buffer , strlen(buffer));
            memset(buffer, 0x0, sizeof(buffer));
        }
        else
            break;
    }
    
    // close connection from client
    printf("TCP:client:close\n");
    close(intSocket);
}

- (char *)returnChar
{
    return buffer;
}

- (void)writeToClient:(int)socket :(const void *)msg :(size_t)size
{
    write(socket, msg, size);
}

-(void) closeServer{
    isClosed = YES;
}


@end
