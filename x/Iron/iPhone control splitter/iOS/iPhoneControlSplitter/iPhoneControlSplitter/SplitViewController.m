//
//  SplitViewController.m
//  iPhoneControlSplitter
//
//  Created by Hiroshi on 9/12/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import "SplitViewController.h"

@interface SplitViewController ()

@end

@implementation SplitViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    // no lock screen
    [[UIApplication sharedApplication] setIdleTimerDisabled: YES];
    
    port = 8888;
    myTimer = nil;
    
    myWebsite = @"192.168.1.109";
    
    mySocket = [[AsyncUdpSocket alloc]initWithDelegate:self];

    
    // UI Init
    self.loudnessLed.layer.cornerRadius = 15;

}

- (void)viewDidUnload
{
    [super viewDidUnload];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


/// UDP
- (void)sendUdpMessage
{
    int myValue;
    if (self.motorSwitch.on)
        myValue = 1;
    else
        myValue = 0;
        
    NSString *myString = [NSString stringWithFormat:@"%d", myValue];
    myData = [myString dataUsingEncoding:NSUTF8StringEncoding];
    
    // to Arduino WiFi
    if (self.wifiOnOff.on)
        myWebsite = @"holdless.no-ip.biz";
    else
        myWebsite = @"192.168.1.109";

    [mySocket sendData:myData toHost:myWebsite port:port withTimeout:-1 tag:1];
    [mySocket receiveWithTimeout:-1 tag:3];
}

/// UDP delegate
// did send
- (void)onUdpSocket:(AsyncUdpSocket *)sock didSendDataWithTag:(long)tag
{
    if (tag == 1)
    {
        
        NSLog(@"i'm messaging to the host");
    }
    
    if (tag == 2)
    {
    }
    
    
    if (tag == 5)
    {
        
    }
    
}

// did not send error handling
- (void)onUdpSocket:(AsyncUdpSocket *)sock didNotSendDataWithTag:(long)tag dueToError:(NSError *)error
{
    if (![mySocket connectToHost:myWebsite onPort:port error:nil])
    {
        exit(0);
    }
}


// did receive
- (BOOL)onUdpSocket:(AsyncUdpSocket *)sock didReceiveData:(NSData *)data withTag:(long)tag fromHost:(NSString *)host port:(UInt16)port
{
    
    if (tag == 6)
    {
        
    }
    
    if (tag == 3)
    {
        NSString *result = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
        
        // decript information from UDP
        NSArray *infoArray = [result componentsSeparatedByString:@","];
        
        // info#1: MIC on/off
        if ([infoArray[0] intValue] == 1)
            self.loudnessLed.backgroundColor = [UIColor greenColor];
        else
            self.loudnessLed.backgroundColor = [UIColor blackColor];

        // info#2: Moisture info
        self.drynessMeter.value = [infoArray[1] intValue];
        self.drynessValue.text = infoArray[1];
        
        
        NSLog(@"\n\nResponse from Ardino:\nMIC  : %@\nDryness: %@ received. \n\n", infoArray[0], infoArray[1]);
    }
    return YES;
     
}

// hiroshi, 2014-Mar-7: NSTimer related
- (IBAction)startStopTimer:(UISwitch *)sender
{
    if (sender.isOn)
    {
        if (myTimer == nil)
        {
            myTimer = [NSTimer scheduledTimerWithTimeInterval:_Time target:self selector:@selector(sendUdpMessage) userInfo:nil repeats:YES];
            
            [[NSRunLoop mainRunLoop] addTimer:myTimer forMode:NSRunLoopCommonModes];
        }
    }
    else
    {
        if (myTimer != nil)
        {
            [myTimer invalidate];
            myTimer = nil;
        }
    }
}

@end
