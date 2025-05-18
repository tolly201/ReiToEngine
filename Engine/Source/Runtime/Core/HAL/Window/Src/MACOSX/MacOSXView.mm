#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_APPLE
#include "../../Include/MACOSX/MacOSXView.h"
#import <AppKit/AppKit.h>
#include "Core/HAL/Window/Include/WindowEnums.h"
#include "../../Include/MACOSX/MacOSXWindow.h"

@implementation OSXView {

}
- (void)displayBuffer:(const void*)buffer width:(uint32_t)width height:(uint32_t)height channel:(uint8_t)channel {
    if (buffer == nullptr || width == 0 || height == 0 || channel == 0) {
        return;
    }
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast; // or kCGImageAlphaNoneSkipLast if no alpha
    if(channel == 3) bitmapInfo = kCGImageAlphaNoneSkipLast;

    size_t bytesPerPixel = channel;
    size_t bytesPerRow = bytesPerPixel * width;
    CGContextRef context = CGBitmapContextCreate((void*)buffer, width, height, 8, bytesPerRow, colorSpace, bitmapInfo);
    if (context == nullptr) {
        NSLog(@"Error creating CGContext");
        CGColorSpaceRelease(colorSpace);
        return;
    }

    CGImageRef cgImage = CGBitmapContextCreateImage(context);

    if (cgImage == nullptr) {
        NSLog(@"Error creating CGImage");
        CGContextRelease(context);
        CGColorSpaceRelease(colorSpace);
        return;
    }
    self->currentImage = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
    [self setNeedsDisplay:YES]; // 标记视图需要重绘，触发 drawRect:

    //  Release resources
    CGImageRelease(cgImage);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
}

- (void)drawRect:(NSRect)dirtyRect {
    NSRect imageRect = NSMakeRect(0, 0, self->currentImage.size.width, self->currentImage.size.height);
    [self->currentImage drawInRect:self.bounds fromRect:imageRect operation:NSCompositingOperationSourceOver fraction:1.0]; // 绘制 NSImage 缩放到视图 bounds
    [super drawRect:dirtyRect]; // 调用父类的 drawRect: 实现
}


- (void)bindWindow:(MacOSXWindow*) window_ptr{
    self->ownerWindow = window_ptr;
}

// NSTextInputClient 协议必需的方法
- (NSArray<NSAttributedStringKey> *)validAttributesForMarkedText {
    return @[];
}

- (NSAttributedString *)attributedSubstringForProposedRange:(NSRange)range
                                               actualRange:(NSRangePointer)actualRange {
    return nil;
}

- (void)insertText:(id)string replacementRange:(NSRange)replacementRange {
    // 处理文本输入
}

- (void)setMarkedText:(id)string selectedRange:(NSRange)selectedRange
                               replacementRange:(NSRange)replacementRange {
    // 处理输入法编辑
}

- (void)unmarkText {
    // 清除输入法编辑标记
}

- (NSRange)selectedRange {
    return NSMakeRange(0, 0);
}

- (NSRange)markedRange {
    return NSMakeRange(NSNotFound, 0);
}

- (BOOL)hasMarkedText {
    return NO;
}

- (NSRect)firstRectForCharacterRange:(NSRange)range
                         actualRange:(NSRangePointer)actualRange {
    return NSZeroRect;
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point {
    return 0;
}
@end

MacOSXView::MacOSXView() = default;
MacOSXView::~MacOSXView() = default;

// 设置和获取 Buffer 相关信息
void MacOSXView::SetBuffer(uint8_t* _buffer)
{
    buffer = _buffer;
}
uint8_t* MacOSXView::GetBuffer() const
{
    return buffer;
}

void MacOSXView::SetWidth(uint32_t _width)
{
    width = _width;
}
uint32_t MacOSXView::GetWidth() const
{
    return width;
}

void MacOSXView::SetHeight(uint32_t _height)
{
    height = _height;
}
uint32_t MacOSXView::GetHeight() const
{
    return height;
}

void MacOSXView::SetChannel(uint8_t _channel)
{
    channel = _channel;
}
uint8_t MacOSXView::GetChannel() const
{
    return channel;
}


void MacOSXView::SetViewInstance(OSXView* view)
{
    osxView = view;
}
OSXView* MacOSXView::GetViewInstance()
{
    return osxView;
}

void MacOSXView::Draw()
{
    [osxView displayBuffer:buffer width:width height:height channel:channel]; // 调用 OSXView 的 displayBuffer 更新 image
}
#endif
