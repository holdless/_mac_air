//
//  ServerViewController.m
//  iOSSocketServer
//
//  Created by Hiroshi on 8/22/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import "ServerViewController.h"
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
#define BUFFER_SIZE 1024
#define THREAD_MAX    5
NSLock *lock;

@interface ServerViewController ()

@end

@implementation ServerViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        [self initServer];

    });

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

///////
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
        printf("Create Socket Failed!");
        exit(1);
    }
    
    // 3. bind socket with socket-address-struct
    if( bind(server_socket,
             (struct sockaddr*)&server_addr,
             sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", PORT);
        exit(1);
    }
    
    // 4. server_socket for "listening"
    if ( listen(server_socket,
                LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!");
        exit(1);
    }
    
    isClosed = NO;
    
    while(!isClosed) // while loop for different clients
    {
        printf("Server start......\n");
        // 5.1 define client's socket-address-struct "client_addr"
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        
        //接受一个到server_socket代表的socket的一个连接
        //如果没有连接请求,就等待到有连接请求--这是accept函数的特性 (block mode)
        //accept函数返回一个新的socket,这个socket(new_server_socket)用于同连接到的客户的通信
        //new_server_socket代表了服务器和客户端之间的一个通信通道
        //accept函数把连接到的客户端信息填写到客户端的socket地址结构client_addr中
        int new_client_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_client_socket < 0)
        {
            printf("Server Accept Failed!/n");
            break;
        }
        
        printf(" one client connted..\n");
        
        
        // refresh UI (back to main thread)
        dispatch_async(dispatch_get_main_queue(), ^{
            
            write(new_client_socket, "connection OK", strlen("connection OK"));
            [self readData:[NSNumber numberWithInt:new_client_socket]];
            
        });

//        [NSThread detachNewThreadSelector:@selector(readData:)
//                                 toTarget:self
//                               withObject:[NSNumber numberWithInt:new_client_socket]];
    }
    //关闭监听用的socket
    close(server_socket);
    NSLog(@"%@",@"server closed....");
}

// read client message
-(void) readData:(NSNumber*) clientSocket // clientSocket為一與client之間的通道代號
{
    char buffer[BUFFER_SIZE];
    int intSocket = [clientSocket intValue];
    
    while(buffer[0] != '-')
    {
        bzero(buffer, BUFFER_SIZE);
        //接收客户端发送来的信息到buffer中
        long readSize = recv(intSocket,
                            buffer,
                            BUFFER_SIZE,
                            0);
        
        
        if (readSize > 0)
        {
            printf("client:%s\n",buffer);
            
            //Send the message back to client
            
            for (int i = 0; i < 5; i++)
                NSLog(@"byte %d: %d", i, buffer[i]);
            
            write(intSocket , buffer , strlen(buffer));
            memset(buffer, 0x0, sizeof(buffer));
        }
        else
            break;
    }

    // close connection from client
    printf("client:close\n");
    close(intSocket);
}


// 在新线程中监听客户端
-(void) startListenAndNewThread
{
    [NSThread detachNewThreadSelector:@selector(initServer)
                             toTarget:self withObject:nil];
}

-(void) closeServer{
    isClosed = YES;
}

#pragma mark - @transmission protocol


@end
