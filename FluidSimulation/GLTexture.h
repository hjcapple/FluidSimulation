#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <UIKit/UIKit.h>

@interface GLTexture : NSObject

@property (atomic) unsigned int textureID;
@property (atomic) int target;
@property (atomic) int width;
@property (atomic) int height;
@property (atomic) int format;

- (id)initWithTarget:(int)target;

- (void)createWithWidth:(int)width height:(int)height format:(int)format;
- (void)createWithWidth:(int)width height:(int)height format:(int)format data:(void *)data;
- (void)createWithImage:(UIImage *)image;

- (void)bindFBO:(int)fbo;

@end
