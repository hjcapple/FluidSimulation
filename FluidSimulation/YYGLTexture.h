#import <Foundation/Foundation.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#else
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#endif

#import <UIKit/UIKit.h>

@interface YYGLTexture : NSObject

@property (atomic) unsigned int textureID;
@property (atomic) int target;
@property (atomic) int width;
@property (atomic) int height;
@property (atomic) int format;

- (id)initWithTarget:(int)target;

- (void)tearDown;

- (void)createWithWidth:(int)width AndHeight:(int)height AndFormat:(int)format;

- (void)createWithWidth:(int)width AndHeight:(int)height AndFormat:(int)format AndData:(void *)data;

- (void)createFromImage:(UIImage *)image;

- (void)bindFBO:(int)fbo;

@end
