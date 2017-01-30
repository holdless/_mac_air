//
//  FSKViewController.h
//  FSK_Demo
//
//  Created by Hiroshi on 9/20/14.
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

@interface FSKViewController : UIViewController <AVAudioSessionDelegate, UITextFieldDelegate>
// 20141013, hiroshi: AudeoDemo object declaration
@property AudioDemo *myFSK;
@property (strong, nonatomic) IBOutlet UILabel *valueLabel;

- (BOOL)readArduino;
- (BOOL)writeArduino;

@property (weak, nonatomic) IBOutlet UITextField *commandInput;

- (IBAction)sendButton:(id)sender;
- (IBAction)receiveButton:(id)sender;

@end
