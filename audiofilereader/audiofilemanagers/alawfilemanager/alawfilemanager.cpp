#include <cstring>
#include <cassert>
#include <strings.h>  //for bzero
#include "alawfilemanager.h"

AlawFileManager::AlawFileManager(char **fileNames, int fileCount)
    :
      AudioFileManager(fileNames, fileCount)
{
}

AlawFileManager::~AlawFileManager()
{
}

int AlawFileManager::readData(SAMPLE* data, EngineAudioChunk& engineAudioChunk)
{
    bool mixData = m_fileCount > 1;
    if(m_rawDataFromChannels == nullptr)
    {
        m_rawDataFromChannels = new RawDataFromChannel*[m_fileCount];
        for(int i = 0;i < m_fileCount;i++)
            m_rawDataFromChannels[i] = new RawDataFromChannel;
    }

    int ENGINE_SAMPLES_FOR_DURATION = SAMPLE_RATE * engineAudioChunk.durationNeeded / 1000;

    int oneMsecsFrames = ALAW_SAMPLE_RATE/1000;
    const int FRAMES_FOR_READ = oneMsecsFrames*engineAudioChunk.durationNeeded;

    ENGINE_SAMPLE** resampledChannels = new ENGINE_SAMPLE*[m_fileCount];
    for (int fileCounter = 0;fileCounter< m_fileCount; fileCounter++) {
        FILE* fp = m_fileStructs[fileCounter]->m_file;
        if(fp == nullptr) {

        }
        else {
            RawDataFromChannel* channelData = m_rawDataFromChannels[fileCounter];
            if(FRAMES_FOR_READ > channelData->dataLength)
            {
                if(channelData->data != nullptr)
                    delete[] channelData->data;
                channelData->data = new char[FRAMES_FOR_READ];
                channelData->dataLength = FRAMES_FOR_READ;
            }
            int readCount = fread(channelData->data,
                  1,
                  FRAMES_FOR_READ,
                  fp);


            ENGINE_SAMPLE* decodedData = new ENGINE_SAMPLE[FRAMES_FOR_READ];
//            bzero((void*)decodedData, FRAMES_FOR_READ*sizeof(ENGINE_SAMPLE));
            decodeAlaw(decodedData, channelData->data, FRAMES_FOR_READ);


            resampledChannels[fileCounter] = new ENGINE_SAMPLE[FRAMES_PER_BUFFER];
            bzero((void*)resampledChannels[fileCounter], FRAMES_PER_BUFFER*sizeof(ENGINE_SAMPLE));
            channelData->m_resampler.resample(decodedData, FRAMES_FOR_READ,
                                              resampledChannels[fileCounter], FRAMES_PER_BUFFER,
                                              double(ENGINE_SAMPLES_FOR_DURATION)/FRAMES_FOR_READ);
        }
    }
    mergeData(data, resampledChannels, FRAMES_PER_BUFFER);
    return 0;
}

int AlawFileManager::seek(double seekRatio)
{
    return -1;
}

long AlawFileManager::getFrameCount()
{
    long framesInFile = 0;
    for (int i = 0;i < m_fileCount;i++) {
        FILE* fp = m_fileStructs[i]->m_file;
        if(fp != nullptr)
            framesInFile = std::max(m_fileStructs[i]->fsize, framesInFile);
    }
    return framesInFile;
}

void AlawFileManager::decodeAlaw(ENGINE_SAMPLE *decodedData, char *sourceData, int srcLength)
{
    for(int i = 0;i < srcLength; i++)
    {
        int16_t decodedSample = decodeAlawByte(sourceData[i]);
        ENGINE_SAMPLE f = ((float) decodedSample) / (float) 32768;
        if( f > 1 ) f = 1;
        if( f < -1 ) f = -1;
        decodedData[i] = f;
    }
}

int16_t AlawFileManager::decodeAlawByte(int8_t a_val)
{
    //https://github.com/GStreamer/gst-plugins-good/blob/master/gst/law/alaw-decode.c
    int16_t t;
    int16_t seg;

    a_val ^= 0x55;
    t = a_val & 0x7f;
    if (t < 16)
      t = (t << 4) + 8;
    else {
      seg = (t >> 4) & 0x07;
      t = ((t & 0x0f) << 4) + 0x108;
      t <<= seg - 1;
    }
    return ((a_val & 0x80) ? t : -t);
}

void AlawFileManager::mergeData(SAMPLE *decodedMerged, ENGINE_SAMPLE **decodedChannels, int frameCount)
{
    for (int i = 0; i < frameCount; i++) {
        decodedMerged[2*i]   = floatToShort(decodedChannels[0][i]);
        decodedMerged[2*i+1] = floatToShort(decodedChannels[1][i]);
    }
}

SAMPLE AlawFileManager::floatToShort(float f)
{
    f = f * 32768 ;
    if( f > 32767 ) f = 32767;
    if( f < -32768 ) f = -32768;
    return (SAMPLE) f;
}
