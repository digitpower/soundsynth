#include <stdlib.h>
#include <algorithm>
#include "audiofilemanager.h"

AudioFileManager::AudioFileManager(char **fileNames, int fileCount)
    : m_fileCount(fileCount)
{
    m_fileStructs =(FileStruct**)malloc(sizeof(FileStruct*) * fileCount);
    for (int i = 0;i < fileCount;i++) {
        FileStruct* f_st = (FileStruct*)malloc(sizeof(FileStruct));
        f_st->fsize = 0;
        f_st->m_file = nullptr;
        strcpy(f_st->m_fileName, fileNames[i]);
        m_fileStructs[i] = f_st;
    }
}

AudioFileManager::~AudioFileManager()
{

}

void AudioFileManager::open()
{
    for (int i = 0; i < m_fileCount; i++) {
        FILE* fp = fopen(m_fileStructs[i]->m_fileName, "rb");
        m_fileStructs[i]->m_file = fp;
        if(fp)
        {
            fseek(fp, 0L, SEEK_END);
            long fsize = ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            m_fileStructs[i]->fsize = fsize;
        }
    }
}

int AudioFileManager::readData(SAMPLE *data, EngineAudioChunk &engineAudioChunk)
{
    return -1;
}

int AudioFileManager::seek(double seekRatio)
{
    return -1;
}

long AudioFileManager::getFrameCount()
{
    return -1;
}

int *AudioFileManager::close()
{
    int* results = (int*)malloc(m_fileCount*sizeof(int));
    for (int i = 0;i < m_fileCount;i++) {
        FILE* fp = m_fileStructs[i]->m_file;
        if(fp == nullptr)
            results[i] = 1;
        else {
            int res = fclose(fp);
            results[i] = res;
            if(!res)
                m_fileStructs[i]->m_file = nullptr;
        }
    }
    return results;
}

