//
//  ClientViewController.h
//  iOSSocketClient
//
//  Created by Hiroshi on 10/15/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ClientViewController : UIViewController <NSStreamDelegate, UITableViewDelegate>

@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;

@property (nonatomic, retain) NSMutableArray *messages;

@property (weak, nonatomic) IBOutlet UILabel *label_status;
@property (weak, nonatomic) IBOutlet UITextField *text_serverIP;
@property (weak, nonatomic) IBOutlet UITextField *text_port;
@property (weak, nonatomic) IBOutlet UITextField *inputNameField;
@property (weak, nonatomic) IBOutlet UITextField *inputDeviceNameField;
@property (weak, nonatomic) IBOutlet UITextField *inputMessageField;
@property (weak, nonatomic) IBOutlet UILabel *label_returnedMsg;


@end
