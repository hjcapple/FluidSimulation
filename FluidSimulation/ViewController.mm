#import "ViewController.h"
#import "TKTimerManager.h"
#import "YYGLTexture.h"
#import "FluidSimulation.h"
#import <vector>

static glutils::TextureInfo toFlowImageTexture(YYGLTexture *texture) {
    glutils::TextureInfo inTex;
    inTex.textureID = texture.textureID;
    inTex.width = texture.width;
    inTex.height = texture.height;
    inTex.target = texture.target;
    return inTex;
}

@interface ViewController () <TKTimerManagerDelegate>
@end

@implementation ViewController {
    EAGLContext *_context;
    CGFloat _screenWidth;
    CGFloat _screenHeight;

    YYGLTexture *_inputTexture;
    YYGLTexture *_outputTexture;
    glutils::FluidSimulation *_fluidSimulation;
}

- (void)dealloc {
    if (_fluidSimulation) {
        delete _fluidSimulation;
        _fluidSimulation = nullptr;
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];

    _fluidSimulation = new glutils::FluidSimulation();
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!_context) {
        NSLog(@"Failed to create ES context");
    }

    GLKView *view = (GLKView *)self.view;
    view.context = _context;
    view.enableSetNeedsDisplay = FALSE;

    _screenWidth = [UIScreen mainScreen].bounds.size.width;
    _screenHeight = [UIScreen mainScreen].bounds.size.height;
    view.contentScaleFactor = [UIScreen mainScreen].scale;

    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [UIColor blackColor];
    self.view.userInteractionEnabled = YES;

    [[TKTimerManager sharedManager] startFastestTimerDelegate:self];

    [self setupOpenGL];
    _fluidSimulation->initOpenGL(toFlowImageTexture(_inputTexture));

    self.title = @"Demo";
    UIBarButtonItem *resetButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Reset"
                                                                        style:UIBarButtonItemStylePlain
                                                                       target:self
                                                                       action:@selector(resetButtonItemPressed:)];
    self.navigationItem.rightBarButtonItem = resetButtonItem;
}

- (void)resetButtonItemPressed:(id)sender {
    _fluidSimulation->initOpenGL(toFlowImageTexture(_inputTexture));
}

- (void)setupOpenGL {
    [EAGLContext setCurrentContext:_context];
    glClearColor(0.0, 0.0, 0.0, 1.0);

    _inputTexture = [[YYGLTexture alloc] initWithTarget:GL_TEXTURE_2D];
    _outputTexture = [[YYGLTexture alloc] initWithTarget:GL_TEXTURE_2D];
    [_inputTexture createFromImage:[UIImage imageNamed:@"test.jpeg"]];
    [_outputTexture createWithWidth:_inputTexture.width AndHeight:_inputTexture.height AndFormat:GL_RGBA];
}

static CGRect computeViewport(CGRect rect, YYGLTexture *texture) {
    CGRect viewport = rect;
    CGFloat sx = rect.size.width / (CGFloat)texture.width;
    CGFloat sy = rect.size.height / (CGFloat)texture.height;
    CGFloat s = MIN(sx, sy);

    CGFloat width = (CGFloat)texture.width * s;
    CGFloat height = (CGFloat)texture.height * s;

    viewport.origin.x = (rect.size.width - width) * 0.5;
    viewport.origin.y = (rect.size.height - height) * 0.5;
    viewport.size.width = width;
    viewport.size.height = height;
    return viewport;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    glClear(GL_COLOR_BUFFER_BIT);

    CGRect viewport = computeViewport(rect, _outputTexture);

    CGFloat scale = self.view.contentScaleFactor;
    viewport.origin.x *= scale;
    viewport.origin.y *= scale;
    viewport.size.width *= scale;
    viewport.size.height *= scale;

    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

    glutils::TextureInfo inTex = toFlowImageTexture(_outputTexture);
    _fluidSimulation->drawTexture(inTex, true);
}

- (void)timerManager:(TKTimerManager *)manager timerFireWithInfo:(TKTimerStepInfo)info {
    glutils::TextureInfo outTxt = toFlowImageTexture(_outputTexture);
    _fluidSimulation->stepFrameUV(info.stepTime * 0.1, outTxt);
    GLKView *view = (GLKView *)self.view;
    [view display];
}

@end
