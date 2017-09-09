//
//  WAVFilePakage.h
//  WAVFilePakage
//
//  Created by CIA on 2017/9/6.
//  Copyright © 2017年 CIA. All rights reserved.
//

#ifndef WAVFilePakage_h
#define WAVFilePakage_h

#include <stdio.h>

// return 0 ,success otherwise failed
int generateFile(char *pcmFilePath, uint32_t frequency, uint16_t channleCount, uint16_t bitPerChannle, char *outputPath);

int generateWaveBuffer(unsigned char *inputPcmBuffer,int pcmBufferByteLength, uint32_t frequency, uint16_t channleCount, uint16_t bitPerChannle, unsigned char **outputWavBuffer, int *waveBufferByteLength);


#endif /* WAVFilePakage_h */
