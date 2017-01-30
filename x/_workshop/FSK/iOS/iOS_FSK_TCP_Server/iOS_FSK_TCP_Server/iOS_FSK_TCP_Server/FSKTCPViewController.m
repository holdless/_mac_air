//
//  FSKTCPViewController.m
//  iOS_FSK_TCP_Server
//
//  Created by Hiroshi on 10/17/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import "FSKTCPViewController.h"

#define _ENCODE_ASCII 31

@interface NSString (NSStringHexToBytes)
-(NSData*) hexToBytes ;
@end

@implementation NSString (NSStringHexToBytes)
-(NSData*) hexToBytes
{
    NSMutableData* data = [NSMutableData data];
    int idx;
    for (idx = 0; idx+2 <= self.length; idx+=2) {
        NSRange range = NSMakeRange(idx, 2);
        NSString* hexStr = [self substringWithRange:range];
        NSScanner* scanner = [NSScanner scannerWithString:hexStr];
        unsigned int intValue;
        [scanner scanHexInt:&intValue];
        [data appendBytes:&intValue length:1];
    }
    return data;
}
@end



@interface FSKTCPViewController ()
{
    NSString *audioInMsg;
}
@end

@implementation FSKTCPViewController

- (void)awakeFromNib
{
    // add KVO for observing return info from audioJack
    [self addObserver:self forKeyPath:@"myFSK.infoCome" options:NSKeyValueObservingOptionNew context:nil];
    
    // add KVO for observing return info from TCP client
    [self addObserver:self forKeyPath:@"myServer.infoCome" options:NSKeyValueObservingOptionNew context:nil];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    // 20141013, hiroshi:new a AudioDemo object here
    self.myFSK = [AudioDemo shared];
    audioInMsg = @"";
    
    // init TCP Server
    self.myServer = [[TCPServer alloc] init];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        [self.myServer initServer];
        
    });
    
    //delegate
    self.tcpOutText.delegate = self;
    self.audioOutMsg.delegate = self;
    self.samplingInterval.delegate = self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - @ KVO delegate
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    // receive from audio-port
    if ([keyPath isEqualToString:@"myFSK.infoCome"])
    {
        NSString *audioInChar = [NSString stringWithFormat:@"%c",[self.myFSK returnChar]];
        
        if (![audioInChar isEqualToString:@""])
        {
            if (![audioInChar isEqualToString:@"\r"] ) // for Serial monitor "carriage return" action
            {
                if (stringOrder == 0)
                {
                    s = [self.myFSK returnChar] - _ENCODE_ASCII; // decoding the feedback string since we encoded it when we sent it out to master
                    NSLog(@"device#: %d", s);
                }
//                else if (stringOrder == 1)
//                {
//                    d = [self.myFSK returnChar];
//                    NSLog(@"socket#: %d", d);
//                }
                else
                {
                    audioInMsg = [NSString stringWithFormat:@"%@%@", audioInMsg, audioInChar];
                }
                
                stringOrder++;
            }
            else
            {
                // 20141105, set stringOrder back to zero
                stringOrder = 0;
                
                // update audio-In UI
                self.audioIn.text = audioInMsg;
                ///////////
                // processing....
                ///////////
                // Update TCP-out by incoming audio-In
                self.tcpOutText.text = audioInMsg;
                
                // write to TCP client ("socket no" is the first char/byte in msg from master)

                [self writeTcpClient:s :audioInMsg];
                
                audioInMsg = @"";
            }
        }
    }

    // receive from TCP client
    if ([keyPath isEqualToString:@"myServer.infoCome"])
    {
        char* pTcpInChar = [self.myServer returnChar];
        
//        if (![tcpInString isEqualToString:@""])
        if (pTcpInChar[0] != 0 && pTcpInChar[0] != '') // '' for suddenly shut down terminal...
        {
            NSString *tcpInString = [NSString stringWithFormat:@"%s",pTcpInChar];
            // update TCP-IN UI
            [self.tcpIn performSelectorOnMainThread:@selector(setText:) withObject:tcpInString waitUntilDone:YES];

            // update Device ID
            [self.label_currentDevice performSelectorOnMainThread:@selector(setText:) withObject:[tcpInString substringToIndex:1] waitUntilDone:YES];


            ///////////
            // processing....
            
            ///////////

            // Update TCP-out by incoming audio-In
            // update TCP-IN UI
            // update current socket
            [self.label_currentSocket performSelectorOnMainThread:@selector(setText:) withObject:[NSString stringWithFormat:@"%d", self.myServer.currentSocket] waitUntilDone:YES];
            // update audio-Out UI msg from client
            [self.audioOutMsg performSelectorOnMainThread:@selector(setText:) withObject:tcpInString waitUntilDone:YES];
            
            // write to Arduino-master
            //device# + socket# + tcpInString
            [self writeArduino:[NSString stringWithFormat:@"%c%@", self.myServer.currentSocket + _ENCODE_ASCII,  tcpInString]]; // currentSockt must be encoded by adding _ENCODE_ASCII (31) to avoid conflicting Arduino master's logic (\n,\r judgment)
            
            // update audio-Out UI
            self.audioOutMsg.text = tcpInString;
        }
        
    }
}


#pragma mark - write Arduino (send to arduino 0xFF)
//For more details see the arduino firmware

- (BOOL)writeArduino:(NSString *)myString
{
    if ([self.audioOutMsg isFirstResponder])
        [self.audioOutMsg resignFirstResponder];
    
    const char* c = [myString UTF8String];
    //    [[[AudioDemo shared] generator] writeByte:c[0]];
    
    for (int i = 0; i < strlen(c); i++)
    {
        [[self.myFSK generator] writeByte:c[i]];
        usleep(self.samplingInterval.text.intValue);
    }
    
    if (c[strlen(c)-1] != '\r' && c[strlen(c)-1] != '\n')
    {
        [[self.myFSK generator] writeByte:'\r'];
    }
    
    return YES;
}

#pragma mark - @ write to TCP client
- (BOOL)writeTcpClient:(int)socket :(NSString *)myString
{
    if ([self.tcpOutText isFirstResponder])
        [self.tcpOutText resignFirstResponder];

    const char* c = [myString UTF8String];
    [self.myServer writeToClient:socket :c :strlen(c)];
    
    return  YES;
}

#pragma mark - buttons

- (IBAction)sendButton:(id)sender
{
    if ([self.audioOutMsg isFirstResponder])
    {
        [self writeArduino:self.audioOutMsg.text];
        NSLog(@"SRV>MAS:%@", self.audioOutMsg.text);
    }
    else
    {
        [self writeTcpClient:self.myServer.currentSocket :self.tcpOutText.text]; // send button will return msg to the client who sent the "last msg" to server (currentSocket's owner)
        NSLog(@"SRV>CLI:%@", self.tcpOutText.text);
    }

}

# pragma mark - @ Textfield Delegate Method and Method to handle Button Touch-up Event

// this is for user touching other place to exit
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    if ([self.audioOutMsg isFirstResponder])
    {
        [self.audioOutMsg resignFirstResponder];
    }

    if ([self.samplingInterval isFirstResponder])
    {
        [self.samplingInterval resignFirstResponder];
    }

    if ([self.tcpOutText isFirstResponder])
    {
        [self.tcpOutText resignFirstResponder];
    }

    return YES;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([self.audioOutMsg isFirstResponder])
    {
        [self.audioOutMsg resignFirstResponder];
    }

    if ([self.samplingInterval isFirstResponder])
    {
        [self.samplingInterval resignFirstResponder];
    }

    if ([self.tcpOutText isFirstResponder])
    {
        [self.tcpOutText resignFirstResponder];
    }
}

#pragma mark - @ math functions
- (unsigned long)decimalToBinary:(int)a
{
    unsigned long b = 0;
    int k = 0;
    while(a)
    {
        b = b + (a & 1)*pow(10, k);
        a = a >> 1;
        k++;
    }
    return b;
}

/*
const int _offset = 180;
- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:.3];
    [UIView setAnimationDelegate:self];
    
    [UIView setAnimationBeginsFromCurrentState:YES];
    
    self.audioOutMsg.frame = CGRectMake(self.audioOutMsg.frame.origin.x, self.audioOutMsg.frame.origin.y - _offset, self.audioOutMsg.frame.size.width, self.audioOutMsg.frame.size.height);
    
    [UIView commitAnimations];
}

- (void)textFieldDidEndEditing:(UITextFrield *)textField
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:.3];
    [UIView setAnimationDelegate:self];
    
    [UIView setAnimationBeginsFromCurrentState:YES];
    
    self.audioOutMsg.frame = CGRectMake(self.audioOutMsg.frame.origin.x, self.audioOutMsg.frame.origin.y + _offset, self.audioOutMsg.frame.size.width, self.audioOutMsg.frame.size.height);
    
    [UIView commitAnimations];
}
*/

@end
