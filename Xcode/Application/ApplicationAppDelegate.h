//
//  ApplicationAppDelegate.h
//  Application
//
//  Created by Mitchell Keith Bloch on 8/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ApplicationAppDelegate : NSObject <NSApplicationDelegate> {
@private
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
