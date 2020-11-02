#ifndef PCM_FILE_MANAGER_H
#define PCM_FILE_MANAGER_H

#include <string>
#include <stdio.h>           //for FILE*
#include "../audiofilemanager/audiofilemanager.h"
#include "../filestructs.h"
#include "audioengineparams.h"

class PcmFileManager : public AudioFileManager{
public:
    PcmFileManager(char **fileNames, int fileCount);
    ~PcmFileManager();
    int  readData(SAMPLE* data, EngineAudioChunk &engineAudioChunk);
    int  seek(double seekRatio);
    long getFrameCount();
private:
public:
    int m_channelCount = 0;
    int numChannels = 2;
    int sampleSize = 2;
};

#endif
