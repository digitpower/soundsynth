#include <cstring>
#include <cassert>
#include <stdlib.h>
#include "pcmfilemanager.h"

PcmFileManager::PcmFileManager(char **fileNames, int fileCount)
    :
      AudioFileManager(fileNames, fileCount)
{
}

PcmFileManager::~PcmFileManager()
{
    for (int i = 0; i < m_fileCount; i++)
    {
        FILE* f = m_fileStructs[i]->m_file;
        if(f != nullptr)
            f = nullptr;
    }
}

int PcmFileManager::readData(SAMPLE* data, EngineAudioChunk& engineAudioChunk)
{
    int framesNeeded = engineAudioChunk.sampleRateNeeded * engineAudioChunk.durationNeeded / 1000;
    int readCount = -1;
    if(m_fileCount == 1) {
        FILE* fp = m_fileStructs[0]->m_file;
        if(fp != nullptr)
        {
            readCount = fread(data, 1, framesNeeded*numChannels*sampleSize, fp);
            if(readCount < framesNeeded || readCount == 0)
            {
                if(ferror(fp))
                    return -2;
                if(feof(fp))
                    return -3;
            }
        }
        else {
            return -4;
        }
    }
    else {
        return -1;
    }
    return readCount;
}

int PcmFileManager::seek(double seekRatio)
{
    int res = -1;
    for (int i = 0; i < m_fileCount; i++)
    {
        FILE* fp = m_fileStructs[0]->m_file;
        if(fp == nullptr)
            continue;
        else {
            long fileSize = m_fileStructs[i]->fsize;
            res = fseek(fp, seekRatio*fileSize - static_cast<u_int64_t>(seekRatio*fileSize) % (numChannels*sampleSize), SEEK_SET);
        }
    }
    return res;
}

long PcmFileManager::getFrameCount()
{
    long framesInFile = 0;
    for (int i = 0;i < m_fileCount;i++) {
        FILE* fp = m_fileStructs[i]->m_file;
        if(fp != nullptr)
            framesInFile = std::max(m_fileStructs[i]->fsize / numChannels / sampleSize, framesInFile);
    }
    return framesInFile;
}
