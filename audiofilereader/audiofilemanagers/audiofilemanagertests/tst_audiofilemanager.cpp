#include <QtTest>
#include "pcmfilemanager.h"

// add necessary includes here

class AudioFileManagerTest : public QObject
{
    Q_OBJECT

public:
    AudioFileManagerTest();
    ~AudioFileManagerTest();

private slots:
    void test_case1();

};

AudioFileManagerTest::AudioFileManagerTest()
{

}

AudioFileManagerTest::~AudioFileManagerTest()
{

}

void AudioFileManagerTest::test_case1()
{
    char** files = (char**)malloc(sizeof(char*)*1);
    files[0] = (char*)malloc(sizeof(char)*500);

    strcpy(files[0], "/home/user/projects/soundsynth/audiofilereader/audiofilemanagers/pcmfilemanager/pcmfilemanagertests/coronavirus.wav");
    AudioFileManager* fileManager = new PcmFileManager(files, 1);

    fileManager->open();
    fileManager->getFrameCount();
    QVERIFY(fileManager->m_fileStructs != nullptr);
}

QTEST_APPLESS_MAIN(AudioFileManagerTest)

#include "tst_audiofilemanager.moc"
