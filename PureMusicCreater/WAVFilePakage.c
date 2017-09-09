//
//  WAVFilePakage.c
//  WAVFilePakage
//
//  Created by CIA on 2017/9/6.
//  Copyright © 2017年 CIA. All rights reserved.
//

#include "WAVFilePakage.h"
#include <stdlib.h>

int generateFile(char *pcmFilePath, uint32_t frequency, uint16_t channleCount, uint16_t bitPerChannle, char *outputPath){
    FILE  *inputFp = NULL;
    if((inputFp = fopen(pcmFilePath, "rb")) == NULL){
        printf("Input file not found");
        return -1;
    }
    fseek(inputFp,0,SEEK_END);
    uint32_t dataLength = (uint32_t)ftell(inputFp);
    fseek(inputFp,0,0);

    
    FILE *outputFile = NULL;
    if((outputFile = fopen(outputPath, "wb")) == NULL){
        printf("can't crate output file");
        return -2;
    }
    
    //RIFF Chunck,length will write later
    fwrite("RIFF0000WAVE", 12, 1, outputFile);
    
    //fmt Chunck
    unsigned char contentTemp[10] = {0x66,0x6d,0x74,0x20,0x10,0x0,0x0,0x0,0x01,0x00};
    fwrite(contentTemp, 10, 1, outputFile);
    fwrite(&channleCount, 2, 1, outputFile);
    fwrite(&frequency, 4, 1, outputFile);
    uint16_t bytePerSample = channleCount * bitPerChannle / 8;
    uint32_t dataCount = frequency * bytePerSample;
    fwrite(&dataCount, 4, 1, outputFile);
    fwrite(&bytePerSample, 2, 1, outputFile);
    fwrite(&bitPerChannle, 2, 1, outputFile);

    
    //data Chunck
    fwrite("data", 4, 1, outputFile);
    fwrite(&dataLength, 4, 1, outputFile);
    
    //data filed
    unsigned char temp = 0;
    //使用feof会导致多执行一次操作
    for (int i = 0; i < dataLength; i += 1) {
        fread(&temp, 1, 1, inputFp);
        fwrite(&temp, 1, 1, outputFile);
    }
    
    //reset RIFF length
    fseek(outputFile,0,SEEK_END);
    uint32_t fileLength = (uint32_t)ftell(outputFile) - 8;
    fseek(outputFile,4,0);
    fwrite(&fileLength, 4, 1, outputFile);
    fflush(outputFile);
    fclose(inputFp);
    fclose(outputFile);
    return 0;
}

int generateWaveBuffer(unsigned char *inputPcmBuffer,int pcmBufferByteLength, uint32_t frequency, uint16_t channleCount, uint16_t bitPerChannle, unsigned char **outputWavBuffer, int *waveBufferByteLength){
    uint32_t waveLength = pcmBufferByteLength + 0x2c;
    unsigned char *waveBuffer = malloc(waveLength);
    
    int index = 0;
    char *riffInfo = "RIFF0000WAVE";
    for (int i = 0; i < 12; i++) {
        waveBuffer[i + index] = riffInfo[i];
    }
    *(uint32_t *)(&waveBuffer[4]) = waveLength - 8;
    index += 12;
    
    unsigned char contentTemp[10] = {0x66,0x6d,0x74,0x20,0x10,0x0,0x0,0x0,0x01,0x00};
    for (int i = 0; i < 10; i++) {
        waveBuffer[i + index] = contentTemp[i];
    }
    index += 10;
    
    *(uint16_t *)(&waveBuffer[index]) = channleCount;
    index += 2;
    
    *(uint32_t *)(&waveBuffer[index]) = frequency;
    index += 4;
    
    uint16_t bytePerSample = channleCount * bitPerChannle / 8;
    uint32_t dataCount = frequency * bytePerSample;
    
    *(uint32_t *)(&waveBuffer[index]) = dataCount;
    index += 4;
    
    *(uint16_t *)(&waveBuffer[index]) = bytePerSample;
    index += 2;
    
    *(uint16_t *)(&waveBuffer[index]) = bitPerChannle;
    index += 2;
    
    char *dataInfo = "data";
    for (int i = 0; i < 4; i++) {
        waveBuffer[i + index] = dataInfo[i];
    }
    index += 4;
    
    *(uint32_t *)(&waveBuffer[index]) = pcmBufferByteLength;
    index += 4;
    
    //data filed
    for (int i = 0; i < pcmBufferByteLength; i += 1) {
        waveBuffer[i + index] = inputPcmBuffer[i];
    }
    index += pcmBufferByteLength;
    
    *waveBufferByteLength = waveLength;
    *outputWavBuffer = waveBuffer;
    
    return 0;
}
