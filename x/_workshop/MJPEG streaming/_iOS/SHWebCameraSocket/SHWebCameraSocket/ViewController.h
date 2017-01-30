//
//  ViewController.h
//  SHWebCameraSocket
//
//  Created by Hiroshi on 8/13/14.
//  Copyright (c) 2014 x. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController <NSStreamDelegate>
{
    NSInputStream *iStream;
    NSOutputStream *oStream;
    
    int jpegHead, jpegEnd;
    UILabel *connectionLabel;
}

@property (nonatomic, strong) UIImageView *imgView;

@end
