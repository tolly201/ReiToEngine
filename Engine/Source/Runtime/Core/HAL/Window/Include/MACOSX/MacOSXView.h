#import <Cocoa/Cocoa.h>
#include "../WindowData.h"
#include "../IWindowView.h"

@interface OSXView : NSView<NSTextInputClient>
{
    // @public SWindowData     *window_data;
    @private NSTrackingArea *tracking_area;
    @private NSImage *currentImage; // 用于存储要显示的 NSImage
}
- (void)displayBuffer:(const void*)buffer width:(uint32_t)width height:(uint32_t)height channel:(uint8_t)channel;

- (void)drawRect:(NSRect)dirtyRect; // 声明 drawRect: 方法
@end

class RTENGINE_API MacOSXView: public IWindowView
{
public:
    MacOSXView();
    ~MacOSXView();

    // 设置和获取 Buffer 相关信息
    void SetBuffer(uint8_t* buffer) override;
    uint8_t* GetBuffer() const override;

    void SetWidth(uint32_t width) override;
    uint32_t GetWidth() const override;

    void SetHeight(uint32_t height) override;
    uint32_t GetHeight() const override;

    void SetChannel(uint8_t channel) override;
    uint8_t GetChannel() const override;

    void SetViewInstance(OSXView*);
    OSXView* GetViewInstance();
    void Draw();
private:
    OSXView* osxView;
};
