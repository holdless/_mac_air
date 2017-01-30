//
//  FSKTCPViewController.h
//  iOS_FSK_TCP_Server
//
//  Created by Hiroshi on 10/17/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CharReceiver.h"
#import "AudioSignalAnalyzer.h"
#import "FSKSerialGenerator.h"
#import "FSKRecognizer.h"
#import "AudioDemo.h"
#import "TCPServer.h"

@interface FSKTCPViewController : UIViewController <AVAudioSessionDelegate, UITextFieldDelegate>
{
    int stringOrder;
    char s,d; // socket#,device#
}

// 20141013, hiroshi: AudeoDemo object declaration
@property AudioDemo *myFSK;
@property TCPServer *myServer;
@property (weak, nonatomic) IBOutlet UILabel *tcpIn;
@property (weak, nonatomic) IBOutlet UITextField *tcpOutText;
@property (weak, nonatomic) IBOutlet UILabel *audioIn;
@property (weak, nonatomic) IBOutlet UITextField *audioOutMsg;
@property (weak, nonatomic) IBOutlet UILabel *label_currentSocket;
@property (weak, nonatomic) IBOutlet UILabel *label_currentDevice;

@property (weak, nonatomic) IBOutlet UITextField *samplingInterval;

@end
