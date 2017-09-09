//
//  ViewController.m
//  PureMusicCreater
//
//  Created by CIA on 2017/9/6.
//  Copyright © 2017年 CIA. All rights reserved.
//

//固定参数
// 采样频率 44100HZ
// 采样精度 16位 单通道
// 采样时间 5s的声音

//纯音频率变化范围20 ~ 20000

#define KRecordFrequency 44100
#define KSoudeTime 5 //5s的音频数据


#import "ViewController.h"
#import "WAVFilePakage.h"
@import AVFoundation;

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UILabel *phaseLabel;
@property (weak, nonatomic) IBOutlet UILabel *frequenceLabel;
@property (weak, nonatomic) IBOutlet UILabel *amplitudeLabel;

@property (weak, nonatomic) IBOutlet UISlider *phaseSlider;
@property (weak, nonatomic) IBOutlet UISlider *frequenceSlider;
@property (weak, nonatomic) IBOutlet UISlider *amplitudeSlider;
@property (weak, nonatomic) IBOutlet UISwitch *sinOrCosSwitch;

@property (nonatomic, strong) AVAudioPlayer *audioPlayer;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.phaseLabel.text = [NSString stringWithFormat:@"%.1f",self.phaseSlider.value];
    self.frequenceLabel.text = [NSString stringWithFormat:@"%d",(int)self.frequenceSlider.value];
    self.amplitudeLabel.text = [NSString stringWithFormat:@"%d",(int)self.amplitudeSlider.value];
}



#pragma mark - slider
- (IBAction)phaseSliderChange:(UISlider *)sender {
    self.phaseLabel.text = [NSString stringWithFormat:@"%.1f",[sender value]];
}
- (IBAction)frequencyChange:(UISlider *)sender {
    self.frequenceLabel.text = [NSString stringWithFormat:@"%d",(int)[sender value]];
}
- (IBAction)maxAmplitudeChange:(UISlider *)sender {
    self.amplitudeLabel.text = [NSString stringWithFormat:@"%d",(int)[sender value]];
}


#pragma mark - button pressed
- (IBAction)playButtonPressed:(id)sender {
    NSData *waveData = [self crateWaveData];
    if (self.audioPlayer) {
        [self.audioPlayer pause];
    }
    self.audioPlayer = [[AVAudioPlayer alloc] initWithData:waveData error:nil];
    [self.audioPlayer play];
}

#pragma mark - 生成wav文件
-(NSData *)crateWaveData{
    int frequency = self.frequenceSlider.value;
    int maxAmplitude = self.amplitudeSlider.value;
    
    //每次采样的时间长度
    double timeLengthForRecord = 1.0 / KRecordFrequency;
    
    //正弦周期长度
    double timeLengthForCycle = 1.0 / frequency;
    
    //总采样次数
    int totalRecordLength = KSoudeTime / timeLengthForCycle;
    
    int16_t *pcmData = malloc(sizeof(int16_t) * totalRecordLength);
    
    BOOL isSin = self.sinOrCosSwitch.isOn;
    for (int i = 0; i < totalRecordLength; i ++) {
        int16_t value = 0;
        if (isSin) {
            value = maxAmplitude * sin(M_PI * 2 * frequency * i * timeLengthForRecord);
        } else {
            value = maxAmplitude * cos(M_PI * 2 * frequency * i * timeLengthForRecord);
        }
        pcmData[i] = value;
    }
    
    unsigned char *waveBuffer = NULL;
    int waveBufferLength = 0;
    generateWaveBuffer((unsigned char *)pcmData, totalRecordLength * 2, frequency, 1, 16, &waveBuffer, &waveBufferLength);
    NSData *waveData = [NSData dataWithBytes:waveBuffer length:waveBufferLength];
    [waveData writeToFile:@"/Users/CIA/Desktop/测试内容.wav" atomically:YES];
    return waveData;
}

@end
