//=============================================================================
//  cocoa-toolkit
//
//  Copyright (C) 2016 HJC. All rights reserved.
//
//  hjcapple@gmail.com
//
//=============================================================================

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

// 这个类，简化NSTimer的使用过程
// 调用timerIdAfterAddDelegate的函数，就可以监听回调的
// - (void) timerManager:(TKTimerManager*)manager
// timerFireWithInfo:(ATTimerStepInfo)info;
// 内部实现的时候，最多只生成一个NSTimer，也可以节约资源

#define TKInvalide_TimerId 0
/////////////////////////////////////
typedef struct {
    NSInteger timerId;           // 定时器id
    NSInteger tag;               // 定时器tag
    NSTimeInterval totalTime;    // 定时器开始计算的总时间
    NSTimeInterval stepTime;     // 跟上一次激发之后的时间间隔
    NSTimeInterval systemUptime; // 系统启动时间
} TKTimerStepInfo;

@class TKTimerManager;
@protocol TKTimerManagerDelegate <NSObject>
@required
- (void)timerManager:(nonnull TKTimerManager *)manager timerFireWithInfo:(TKTimerStepInfo)info;
@end

//////////////////////////////////////////
// 计时器管理
@interface TKTimerManager : NSObject {
}
@property (nonatomic, readonly) BOOL isPausingAllTimers; // 是否在暂停所有的定时器
@property (nonatomic, retain, nonnull) NSRunLoopMode runLoopMode;

+ (nonnull TKTimerManager *)sharedManager;

// 添加定时器, 返回ID, 之后可以使用这个ID停止定时器, 可以指定间隔秒数
- (NSInteger)startTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate
                       interval:(NSTimeInterval)interval
                            tag:(NSInteger)tag;

- (NSInteger)startTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate interval:(NSTimeInterval)interval;

// 最短间隔的定时器
- (NSInteger)startFastestTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag;
- (NSInteger)startFastestTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate;

// 判断是否已生成定时器
- (BOOL)hasTimerId:(NSInteger)timerId;
- (BOOL)hasTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate;
- (BOOL)hasTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag;

// 停止定时器
- (void)stopTimerId:(NSInteger)timerId;
- (void)stopTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate;
- (void)stopTimerDelegate:(nonnull id<TKTimerManagerDelegate>)delegate tag:(NSInteger)tag;

// 暂停, 回复所有定时器
- (void)pauseAllTimers;
- (void)resumeAllTimers;

@end
