#ifndef AUDIOFILEMANAGERPROXY_H
#define AUDIOFILEMANAGERPROXY_H

#include "../filestructs.h"
#include "../audiofileinterface.h"
#include "audiofilemanager_global.h"


class AudioFileManager;

class AUDIOFILEMANAGER_EXPORT AudioFileManagerProxy : public AudioFileInterface
{
public:
    AudioFileManagerProxy(char** fileNames, int fileCount);
    ~AudioFileManagerProxy();
    void open();
    int  readData(SAMPLE* data, EngineAudioChunk &engineAudioChunk);
    int  seek(double seekRatio);
    long getFrameCount();
    int *close();
private:
    AudioFileManager* m_audioFileManager = nullptr;
};


#endif // AUDIOFILEMANAGERPROXY_H
