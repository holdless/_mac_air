//
//  FSKViewController.m
//  FSK_Demo
//
//  Created by Hiroshi on 9/20/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import "FSKViewController.h"

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

@interface FSKViewController ()
{
    NSString *srvReturnedMsg;
}
@end

@implementation FSKViewController
- (void)awakeFromNib
{
    // add KVO for observing return info from audioJack
    [self addObserver:self forKeyPath:@"myFSK.infoCome" options:NSKeyValueObservingOptionNew context:nil];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    // 20141013, hiroshi:new a AudioDemo object here
    self.myFSK = [AudioDemo shared];
    srvReturnedMsg = @"";
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
#pragma mark - @ KVO delegate
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"myFSK.infoCome"])
    {
        NSString *c = [NSString stringWithFormat:@"%c",[self.myFSK returnChar]];
        
        if (![c isEqualToString:@""])
        {
            if (![c isEqualToString:@"\r"] && ![c isEqualToString:@"."])
            {
                srvReturnedMsg = [NSString stringWithFormat:@"%@%@", srvReturnedMsg, c];
            }
            else
            {
                self.valueLabel.text = srvReturnedMsg;
                srvReturnedMsg = @"";

            }
        }
    }
}


#pragma mark - read Arduino (send to arduino 0x00 and wait the response)
//For more details see the arduino firmware

- (BOOL)readArduino
{
//    for (int i =0; i < 15; i++) // Just for me dont need do 15 clicks
//        [[[AudioDemo shared] generator] writeByte:0x00];
    
//    NSString *s = [NSString stringWithFormat:@"%d",[[AudioDemo shared] returnChar]];
    NSString *s = [NSString stringWithFormat:@"%d",[self.myFSK returnChar]];
    
    NSString *s2 = [NSString stringWithFormat:@"%c", s]; // A

//    if (![s isEqualToString:@"13"])
        self.valueLabel.text = s;
    return YES;
}

#pragma mark - write Arduino (send to arduino 0xFF)
//For more details see the arduino firmware

- (BOOL)writeArduino{
    int a ='b';
    char b = 'c';
    char* c = [self.commandInput.text UTF8String];
//    [[[AudioDemo shared] generator] writeByte:c[0]];
    for (int i = 0; i < sizeof(c); i++)
    {
        if (c[i] == 0)
            break;
        
        [[self.myFSK generator] writeByte:c[i]];
        usleep(100000);
    }
    return YES;
}

#pragma mark - buttons

- (IBAction)sendButton:(id)sender {
    [self writeArduino];
    
}

- (IBAction)receiveButton:(id)sender {
    [self readArduino];
}


# pragma mark - @ Textfield Delegate Method and Method to handle Button Touch-up Event

// this is for user touching other place to exit
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    if ([self.commandInput isFirstResponder]) {
        [self.commandInput resignFirstResponder];
    }
    
    return YES;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([self.commandInput isFirstResponder]) {
        [self.commandInput resignFirstResponder];
    }
}

const int _offset = 180;
- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:.3];
    [UIView setAnimationDelegate:self];
    
    [UIView setAnimationBeginsFromCurrentState:YES];
    
    self.commandInput.frame = CGRectMake(self.commandInput.frame.origin.x, self.commandInput.frame.origin.y - _offset, self.commandInput.frame.size.width, self.commandInput.frame.size.height);
    
    [UIView commitAnimations];
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:.3];
    [UIView setAnimationDelegate:self];
    
    [UIView setAnimationBeginsFromCurrentState:YES];
    
    self.commandInput.frame = CGRectMake(self.commandInput.frame.origin.x, self.commandInput.frame.origin.y + _offset, self.commandInput.frame.size.width, self.commandInput.frame.size.height);
    
    [UIView commitAnimations];
}


@end
