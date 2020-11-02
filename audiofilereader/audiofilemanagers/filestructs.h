#ifndef FILE_STRUCTS
#define FILE_STRUCTS

#include <stdio.h>
#include <string.h>
const int MAX_SIZE = 500;
struct FileStruct {
    char m_fileName[MAX_SIZE];
    bool m_infoIsPresented;
    FILE* m_file;
    long fsize;
};

struct EngineAudioChunk {
    int durationNeeded = 0;
    int channelsNeeded = 0;
    int sampleRateNeeded = 0;
};

#endif
