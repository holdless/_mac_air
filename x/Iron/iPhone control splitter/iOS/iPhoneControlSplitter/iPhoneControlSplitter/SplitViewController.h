//
//  SplitViewController.h
//  iPhoneControlSplitter
//
//  Created by Hiroshi on 9/12/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AsyncUdpSocket.h"

#define _Time 0.05

@interface SplitViewController : UIViewController <AsyncUdpSocketDelegate>
{
    AsyncUdpSocket *mySocket;
    UInt16 port;
    NSTimer *myTimer;
    NSString *myWebsite;
    NSData *myData;

}

@property (weak, nonatomic) IBOutlet UISwitch *motorSwitch;
@property (weak, nonatomic) IBOutlet UISwitch *wifiOnOff;
@property (weak, nonatomic) IBOutlet UISlider *drynessMeter;
@property (weak, nonatomic) IBOutlet UILabel *drynessValue;
@property (weak, nonatomic) IBOutlet UITextField *loudnessLed;

@end
