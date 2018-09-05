#import "YYGLTexture.h"
#import <UIKit/UIKit.h>

@interface YYGLTexture () {
    BOOL _isRef;
}
@end

@implementation YYGLTexture

- (id)initWithTarget:(int)target {
    if (!(self = [super init]))
        return nil;

    _isRef = false;
    _target = target;
    GLuint textureHandles[1] = {0};
    glGenTextures(1, textureHandles);
    _textureID = textureHandles[0];

    glBindTexture(_target, _textureID);
    glTexParameterf(_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return self;
}

- (void)dealloc {
    [self tearDown];
}

- (void)tearDown {
    if (!_isRef) {
        if (_textureID) {
            glDeleteTextures(1, &_textureID);
            _textureID = 0;
        }
    }
}

- (void)createWithWidth:(int)width AndHeight:(int)height AndFormat:(int)format {
    glBindTexture(_target, _textureID);
    glTexImage2D(_target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nil);
    _width = width;
    _height = height;
    _format = format;
}

- (void)createWithWidth:(int)width AndHeight:(int)height AndFormat:(int)format AndData:(void *)data {
    glBindTexture(_target, _textureID);
    glTexImage2D(_target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    _width = width;
    _height = height;
    _format = format;
}

- (void)createFromImage:(UIImage *)image {
    _width = (int)CGImageGetWidth(image.CGImage);
    _height = (int)CGImageGetHeight(image.CGImage);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void *imageData = malloc(_height * _width * 4);

    CGContextRef context = CGBitmapContextCreate(imageData,
                                                 _width,
                                                 _height,
                                                 8,
                                                 4 * _width,
                                                 colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGContextClearRect(context, CGRectMake(0, 0, _width, _height));
    CGContextTranslateCTM(context, 0, _height - _height);
    CGContextDrawImage(context, CGRectMake(0, 0, _width, _height), image.CGImage);

    glBindTexture(_target, _textureID);
    glTexImage2D(_target, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    CGContextRelease(context);

    free(imageData);
}

- (void)bindFBO:(int)fbo {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(_target, _textureID);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _target, _textureID, 0);
}

@end
