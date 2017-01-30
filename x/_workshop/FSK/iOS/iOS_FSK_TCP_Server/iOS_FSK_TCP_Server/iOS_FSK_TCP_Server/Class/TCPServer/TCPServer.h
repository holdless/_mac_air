//
//  TCPServer.h
//  iOS_FSK_TCP_Server
//
//  Created by Hiroshi on 10/19/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import <Foundation/Foundation.h>

#define BUFFER_SIZE 1024

@interface TCPServer : NSObject
{
    BOOL isClosed;
    char buffer[BUFFER_SIZE];
//    int new_client_socket;
}

- (void)initServer;
- (void)readData:(NSNumber*) clientSocket;
- (char *)returnChar;
- (void)writeToClient:(int)socket :(const void *)msg :(size_t)size;
//-(void) sendData:(const char*) data;
//-(void) closeServer;

@property BOOL infoCome;
@property char currentSocket; // 20141105, for multi-client


@end
