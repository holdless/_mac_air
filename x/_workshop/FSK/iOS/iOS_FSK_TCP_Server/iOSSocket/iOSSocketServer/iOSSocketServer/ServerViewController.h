//
//  ServerViewController.h
//  iOSSocketServer
//
//  Created by Hiroshi on 8/22/14.
//  Copyright (c) 2014 x. All rights reserved.
//

/*
 Socket Basic Knowledge
 
 1、 INADDR_ANY 就是指定地址为 0.0.0.0 的地址，这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。 一般来说，在各个系统中均定义成为 0 值。
 2、 INADDR_ANY 指的是所有 ip，即连接本机任何一个 ip 地址都可以连接上
 3、 sin_family 指代协议族，在socket编程中只能是AF_INET
 4、 sin_port 存储端口号（使用网络字节顺序），需要大于1024.
 5、 sin_addr 存储 IP 地址，使用 in_addr 这个数据结构
 6、 sin_zero 是为了让 sockaddr 与 sockaddr_in 两个数据结构保持大小相同而保留的空字节。
 7、 s_addr 按照网络字节顺序存储 IP 地址
 8、 sockaddr_in 和 sockaddr 是并列的结构，指向 sockaddr_in 的结构体的指针也可以指向 sockaddr 的结构体，并代替它。也就是说，你可以使用 sockaddr_in 建立你所需要的信息。
 */

#import <UIKit/UIKit.h>

@interface ServerViewController : UIViewController
{
    BOOL isClosed;
}

// 初始化服务器
-(void) initServer;
// 读客户端数据
-(void) readData:(NSNumber*) clientSocket;
// 向客户端发送数据
//-(void) sendData:(const char*) data;
// 在新线程中监听客户端
-(void) startListenAndNewThread;
-(void) closeServer;

@end
