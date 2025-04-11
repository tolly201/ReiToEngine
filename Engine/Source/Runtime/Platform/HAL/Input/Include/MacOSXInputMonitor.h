#ifndef CORE_PLATFORM_HAL_MACOSX_INPUT_MONITOR_H
#define CORE_PLATFORM_HAL_MACOSX_INPUT_MONITOR_H

#import <Cocoa/Cocoa.h>
#include "Platform/HAL/Input/Include/InputEnums.h"
#include "InputMonitor.h"

namespace ReiToEngine {
class MacOSXInputMonitor;
}

@interface NSInputMonitor : NSObject
{
    ReiToEngine::MacOSXInputMonitor* _monitor;
}
- (void)addMonitor:(ReiToEngine::MacOSXInputMonitor*)Monitor;
- (void)startMonitoring;
@end

namespace ReiToEngine {
class MacOSXInputMonitor : public InputMonitor
{
public:
    MacOSXInputMonitor();
    ~MacOSXInputMonitor();
    void InnerConvertNSEvent(NSEvent* event);
protected:
    NSInputMonitor* cocoaInputMonitor;
};

}
#endif
