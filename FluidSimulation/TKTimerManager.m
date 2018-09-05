//=============================================================================
//  cocoa-toolkit
//
//  Copyright (C) 2016 HJC. All rights reserved.
//
//  hjcapple@gmail.com
//
//=============================================================================

#import "TKTimerManager.h"

inline static NSTimeInterval getCurrentTime() {
    return [NSProcessInfo processInfo].systemUptime;
}

@interface TKTimerInfo : NSObject {
}
@property (nonatomic, weak) id<TKTimerManagerDelegate> delegate;
@property (nonatomic, assign) NSInteger timerId;             //  定时器id
@property (nonatomic, assign) NSInteger tag;                 //  用户创建定时器时传进来的tag
@property (nonatomic, assign) NSTimeInterval totalTime;      //  当时器开始后，运行了的总时间
@property (nonatomic, assign) NSTimeInterval lastTime;       //  上一次触发定时器的时间点
@property (nonatomic, assign) NSTimeInterval theoryInterval; //  定时器理论上的间隔
@property (nonatomic, assign) NSTimeInterval remainNextTime; //  下一次触发定时器剩下的时间
@property (nonatomic, readonly) BOOL fireEachStep;           //  是否最快的定时器，也就是每次都触发
@end

@implementation TKTimerInfo

- (BOOL)fireEachStep {
    return _theoryInterval < DBL_MIN;
}

@end

//////////////////////////////////////////////////////////////

@implementation TKTimerManager {
@private
    NSMutableArray<TKTimerInfo *> *_timerInfos;

#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
    CADisplayLink *_displayLink;
#else
    NSTimer *_timer;
#endif
}

- (id)copyWithZone:(NSZone *)zone {
    return self;
}

+ (TKTimerManager *)sharedManager {
    static TKTimerManager *s_timerManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        s_timerManager = [[TKTimerManager alloc] init];
    });
    return s_timerManager;
}

- (id)init {
    self = [super init];
    if (self) {
        _timerInfos = [[NSMutableArray alloc] initWithCapacity:8];
        _runLoopMode = NSRunLoopCommonModes;
    }
    return self;
}

- (TKTimerInfo *)_createTimerInfoWithDelegate:(id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag {
    NSInteger timerId = TKInvalide_TimerId + 1;
    if (_timerInfos.count > 0) {
        TKTimerInfo *lastInfo = _timerInfos.lastObject;
        timerId = lastInfo.timerId + 1;
    }

    TKTimerInfo *timerInfo = [[TKTimerInfo alloc] init];
    timerInfo.timerId = timerId;
    timerInfo.delegate = delegate;
    timerInfo.tag = tag;
    timerInfo.totalTime = 0;
    timerInfo.lastTime = getCurrentTime();

    return timerInfo;
}

#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED

- (void)_startPublicTimer {
    if (_displayLink == nil) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(_timerStep)];
        [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:_runLoopMode];
    }
}

- (void)stopPublicTimer {
    if (_displayLink) {
        [_displayLink invalidate];
        _displayLink = nil;
    }
}

#else

- (void)_startPublicTimer {
    if (_timer == nil) {
        _timer =
            [NSTimer timerWithTimeInterval:0.0 target:self selector:@selector(_timerStep) userInfo:nil repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:_timer forMode:_runLoopMode];
    }
}

- (void)stopPublicTimer {
    if (_timer) {
        [_timer invalidate];
        _timer = nil;
    }
}

#endif

- (NSInteger)startTimerDelegate:(id<TKTimerManagerDelegate>)delegate
                       interval:(NSTimeInterval)interval
                            tag:(NSInteger)tag {
    TKTimerInfo *timerInfo = [self _createTimerInfoWithDelegate:delegate tag:tag];
    if (fabs(interval) < DBL_MIN) {
        interval = 0.0;
    }

    timerInfo.theoryInterval = interval;
    timerInfo.remainNextTime = interval;
    [_timerInfos addObject:timerInfo];

    if (!_isPausingAllTimers) {
        [self _startPublicTimer];
    }

    return timerInfo.timerId;
}

- (NSInteger)startTimerDelegate:(id<TKTimerManagerDelegate>)delegate interval:(NSTimeInterval)interval {
    return [self startTimerDelegate:delegate interval:interval tag:0];
}

- (NSInteger)startFastestTimerDelegate:(id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag {
    return [self startTimerDelegate:delegate interval:0.0 tag:tag];
}

- (NSInteger)startFastestTimerDelegate:(id<TKTimerManagerDelegate>)delegate {
    return [self startTimerDelegate:delegate interval:0.0 tag:0];
}

- (void)_fireTimerInfo:(TKTimerInfo *)info currentTime:(NSTimeInterval)currentTime {
    TKTimerStepInfo stepInfo;
    stepInfo.timerId = info.timerId;
    stepInfo.tag = info.tag;
    stepInfo.stepTime = (currentTime - info.lastTime);
    stepInfo.totalTime = info.totalTime + stepInfo.stepTime;
    stepInfo.systemUptime = currentTime;

    info.lastTime = currentTime;
    info.totalTime = stepInfo.totalTime;
    [info.delegate timerManager:self timerFireWithInfo:stepInfo];
}

- (void)_timerStep {
    NSTimeInterval currentTime = getCurrentTime();
    NSArray *tmpTimerInfos = [NSArray arrayWithArray:_timerInfos];
    BOOL hasWeakDelegate = FALSE;

    // 循环时候，用户有可能会使用stopTimerId函数，引起_timerInfos改变，所以这里用tmpTimerInfos遍历
    for (TKTimerInfo *info in tmpTimerInfos) {
        if (info.delegate == nil) {
            hasWeakDelegate = true;
            continue;
        }

        if (info.fireEachStep) {
            [self _fireTimerInfo:info currentTime:currentTime];
            continue;
        }

        NSTimeInterval divTime = currentTime - info.lastTime;
        if (divTime < 0) {
            info.lastTime = currentTime;
            divTime = 0;
        }

        if (divTime >= info.remainNextTime) {
            info.remainNextTime = (info.remainNextTime - divTime) + info.theoryInterval;
            if (info.remainNextTime < 0) {
                info.remainNextTime = info.theoryInterval;
            }
            [self _fireTimerInfo:info currentTime:currentTime];
        }
    }

    // 删除已经释放了的弱引用
    if (hasWeakDelegate) {
        [self stopTimerIf:^BOOL(TKTimerInfo *info) {
            return info.delegate == nil;
        }];
    }
}

- (void)stopTimerIf:(BOOL (^)(TKTimerInfo *info))block {
    // 从后面遍历到前面
    for (NSInteger idx = _timerInfos.count - 1; idx >= 0; idx--) {
        TKTimerInfo *info = _timerInfos[idx];
        if (block(info)) {
            info.delegate = nil;
            [_timerInfos removeObjectAtIndex:idx];
        }
    }

    if (_timerInfos.count == 0) {
        [self stopPublicTimer];
    }
}

- (void)stopTimerId:(NSInteger)timerId {
    [self stopTimerIf:^BOOL(TKTimerInfo *info) {
        return info.timerId == timerId;
    }];
}

- (void)stopTimerDelegate:(id<TKTimerManagerDelegate>)delegate {
    [self stopTimerIf:^BOOL(TKTimerInfo *info) {
        return info.delegate == delegate;
    }];
}

- (void)stopTimerDelegate:(id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag {
    [self stopTimerIf:^BOOL(TKTimerInfo *info) {
        return info.delegate == delegate && info.tag == tag;
    }];
}

- (BOOL)hasTimerId:(NSInteger)timerId {
    for (TKTimerInfo *info in _timerInfos) {
        if (info.timerId == timerId) {
            return YES;
        }
    }
    return NO;
}

- (BOOL)hasTimerDelegate:(id<TKTimerManagerDelegate>)delegate {
    for (TKTimerInfo *info in _timerInfos) {
        if (info.delegate == delegate) {
            return YES;
        }
    }
    return NO;
}

- (BOOL)hasTimerDelegate:(id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag {
    for (TKTimerInfo *info in _timerInfos) {
        if (info.delegate == delegate && info.tag == tag) {
            return YES;
        }
    }
    return NO;
}

- (void)pauseAllTimers {
    _isPausingAllTimers = YES;
    [self stopPublicTimer];
}

- (void)resumeAllTimers {
    if (_isPausingAllTimers) {
        _isPausingAllTimers = NO;
        NSTimeInterval currentTime = getCurrentTime();
        for (TKTimerInfo *info in _timerInfos) {
            info.lastTime = currentTime;
        }

        if (_timerInfos.count > 0) {
            [self _startPublicTimer];
            [self _timerStep];
        }
    }
}

@end
