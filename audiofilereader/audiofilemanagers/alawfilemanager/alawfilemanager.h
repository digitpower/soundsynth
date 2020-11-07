#ifndef ALAW_FILE_MANAGER_H
#define ALAW_FILE_MANAGER_H

#include <string>
#include <stdio.h>           //for FILE*
#include <samplerate.h>
#include "../audiofilemanager/audiofilemanager.h"
#include "../filestructs.h"
#include "audioengineparams.h"
#include "resampler.h"

#define ENGINE_SAMPLE float

class AlawFileManager : public AudioFileManager{
public:
    AlawFileManager(char **fileNames, int fileCount);
    ~AlawFileManager();
    int  readData(SAMPLE* data, EngineAudioChunk &engineAudioChunk);
    int  seek(double seekRatio);
    long getFrameCount();
private:
    void mergeData(SAMPLE *decodedMerged, ENGINE_SAMPLE** decodedChannels, int frameCount);
    SAMPLE floatToShort(ENGINE_SAMPLE engineSample);
public:
    struct RawDataFromChannel {
        char* data = nullptr;
        int dataLength = 0;
        Resampler m_resampler;
    };
    RawDataFromChannel** m_rawDataFromChannels = nullptr;

    const int ALAW_SAMPLE_RATE = 8000;
};

#endif
