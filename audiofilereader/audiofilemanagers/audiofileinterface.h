#ifndef AUDIO_FILE_INTERFACE
#define AUDIO_FILE_INTERFACE
#include "../../lib/audioengineparams.h"

class AudioFileInterface
{
public:
//    virtual ~AudioFileInterface() = 0;
    virtual void open() = 0;
    virtual int  readData(SAMPLE* data, EngineAudioChunk &engineAudioChunk) = 0;
    virtual int seek(double seekRatio) = 0;
    virtual long getFrameCount() = 0;
    virtual int *close() = 0;
};
#endif
