#include "audiofilemanagerproxy.h"
#include "audiofilemanager.h"
#include "pcmfilemanager.h"
#include "alawfilemanager.h"

AudioFileManagerProxy::AudioFileManagerProxy(char** fileNames, int fileCount)
{
    if(fileCount == 1)
        m_audioFileManager = new PcmFileManager(fileNames, fileCount);
    else if(fileCount == 2)
        m_audioFileManager = new AlawFileManager(fileNames, fileCount);
}

AudioFileManagerProxy::~AudioFileManagerProxy()
{

}

void AudioFileManagerProxy::open()
{
    m_audioFileManager->open();
}

int AudioFileManagerProxy::readData(SAMPLE *data, EngineAudioChunk &engineAudioChunk)
{
    m_audioFileManager->readData(data, engineAudioChunk);
}

int AudioFileManagerProxy::seek(double seekRatio)
{
    m_audioFileManager->seek(seekRatio);
}

long AudioFileManagerProxy::getFrameCount()
{
    m_audioFileManager->getFrameCount();
}

int *AudioFileManagerProxy::close()
{
    m_audioFileManager->close();
}
