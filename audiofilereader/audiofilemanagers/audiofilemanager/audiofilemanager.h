#ifndef AUDIOFILEMANAGER_H
#define AUDIOFILEMANAGER_H

#include "audioengineparams.h"
#include "../filestructs.h"
class AudioFileManager {
public:
    AudioFileManager(char** fileNames, int fileCount);
    ~AudioFileManager();
    void open();
    virtual int  readData(SAMPLE* data, EngineAudioChunk &engineAudioChunk);
    virtual int seek(double seekRatio);
    virtual long getFrameCount();
    int *close();
    public:
    int m_fileCount = 0;
    FileStruct** m_fileStructs = nullptr;
};

#endif // AUDIOFILEMANAGER_H
