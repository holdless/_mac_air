//
//  ClientViewController.m
//  iOSSocketClient
//
//  Created by Hiroshi on 10/15/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import "ClientViewController.h"

@interface ClientViewController ()
{

}
@end

@implementation ClientViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
	//[self initNetworkCommunication];
	[self connectToServer:nil];
    
//	self.inputNameField.text = @"jack";
	self.messages = [[NSMutableArray alloc] init];
}

- (void) initNetworkCommunication {
	
	CFReadStreamRef readStream;
	CFWriteStreamRef writeStream;
	CFStreamCreatePairWithSocketToHost(NULL,
                                       (__bridge CFStringRef)self.text_serverIP.text,
                                       self.text_port.text.integerValue,//NSInteger not int
                                       &readStream,
                                       &writeStream);
	
	self.inputStream = (__bridge NSInputStream *)readStream;
	self.outputStream = (__bridge NSOutputStream *)writeStream;
	[self.inputStream setDelegate:self];
	[self.outputStream setDelegate:self];
	[self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop]
                                forMode:NSDefaultRunLoopMode];
	[self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop]
                                 forMode:NSDefaultRunLoopMode];
	[self.inputStream open];
	[self.outputStream open];
	
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)connectToServer:(UIButton *)sender
{
    [self initNetworkCommunication];
}

/*
- (IBAction)join:(UIButton *)sender
{
//	[self.view bringSubviewToFront:chatView];
	NSString *response  = [NSString stringWithFormat:@"iam:%@", self.inputNameField.text];
	NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    // write data to server
	[self.outputStream write:[data bytes] maxLength:[data length]];
}
*/

- (IBAction)sendMessage:(UIButton *)sender
{
	NSString *msg  = [NSString stringWithFormat:@"%@%@%@", self.inputNameField.text, self.inputDeviceNameField.text, self.inputMessageField.text];
	NSData *msgData = [[NSData alloc] initWithData:[msg dataUsingEncoding:NSASCIIStringEncoding]];
    
    // write data to server
	[self.outputStream write:[msgData bytes] maxLength:[msgData length]];
/*
    NSNumber *serialNo = [NSNumber numberWithInt:1];
    NSNumber *userNo = [NSNumber numberWithInt:1];
    NSNumber *deviceNo = [NSNumber numberWithInt:1];
    NSNumber *control = [NSNumber numberWithInt:1];
    NSNumber *value = [NSNumber numberWithInt:1000];
    
    NSArray *inputArray = [NSArray arrayWithObjects:serialNo, userNo, deviceNo, control, value, nil];
    NSArray *outputArray = [NSArray new];
    
//    outputArray = [self commandToByte:inputArray :outputArray];
    
    unsigned char encodedMsg[5] = {0};
    for (int i = 0; i < 5; i++)
        encodedMsg[i] = (unsigned char)[[outputArray objectAtIndex:i] intValue];
    
    
    [self.outputStream write:encodedMsg maxLength:sizeof(encodedMsg)];
*/    
	self.inputMessageField.text = @"";
}

- (void)stream:(NSStream *)theStream handleEvent:(NSStreamEvent)streamEvent
{
    
	NSLog(@"stream event %i", streamEvent);
	
	switch (streamEvent)
    {
		case NSStreamEventOpenCompleted:
			NSLog(@"Stream opened");
            self.label_status.text = @"connected!";
			break;
		case NSStreamEventHasBytesAvailable:
            
			if (theStream == self.inputStream)
            {
				uint8_t buffer[1024];
				int len;
				
				while ([self.inputStream hasBytesAvailable]) {
					len = [self.inputStream read:buffer maxLength:sizeof(buffer)];
					if (len > 0)
                    {
						NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
						
						if (nil != output)
                        {
							NSLog(@"server said: %@", output);
                            self.label_returnedMsg.text = output;
							[self messageReceived:output];
							
						}
					}
				}
			}
			break;
            
			
		case NSStreamEventErrorOccurred:
			
			NSLog(@"Can not connect to the host!");
            self.label_status.text = @"fail!";
			break;
			
		case NSStreamEventEndEncountered:
            
            [theStream close];
            [theStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
//            [theStream release];
            theStream = nil;
			
			break;
		default:
			NSLog(@"Unknown event");
            self.label_status.text = @"connected!";
	}
    
}

- (void) messageReceived:(NSString *)message {
	
	[self.messages addObject:message];
//	[self.tView reloadData];
	NSIndexPath *topIndexPath = [NSIndexPath indexPathForRow:self.messages.count-1
												   inSection:0];
//	[self.tView scrollToRowAtIndexPath:topIndexPath
//					  atScrollPosition:UITableViewScrollPositionMiddle
//							  animated:YES];
    
}

/* 20141020, hiroshi: marked, since encoding/decoding on FSK may cause data fidelity problem...
#pragma mark - @transmission protocol
- (NSArray*)commandToByte:(NSArray *)inputArray :(NSArray *)outputArray
{
    int serial, user, device, control, value;
    serial = [[inputArray objectAtIndex:0] intValue];
    user = [[inputArray objectAtIndex:1] intValue];
    device = [[inputArray objectAtIndex:2] intValue];
    control = [[inputArray objectAtIndex:3] intValue];
    value = [[inputArray objectAtIndex:4] intValue];
    
    NSNumber *byte1 = [NSNumber numberWithInt:(serial/16)];
    NSNumber *byte2 = [NSNumber numberWithInt:(serial%16*16 + user)];
    NSNumber *byte3 = [NSNumber numberWithInt:device];
    NSNumber *byte4 = [NSNumber numberWithInt:control*16 + value/256];
    NSNumber *byte5 = [NSNumber numberWithInt:value%256];

    outputArray = [outputArray arrayByAddingObject:byte1];
    outputArray = [outputArray arrayByAddingObject:byte2];
    outputArray = [outputArray arrayByAddingObject:byte3];
    outputArray = [outputArray arrayByAddingObject:byte4];
    outputArray = [outputArray arrayByAddingObject:byte5];
    
    return outputArray;
//    msg[0] = (serial/16);
//    msg[1] = serial%16*16 + user;
//    msg[2] = deivce;
//    msg[3] = control*16 + value/256;
//    msg[4] = value%256;
}
*/
@end
