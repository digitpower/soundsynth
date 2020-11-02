#include <QtTest>
#include "../../filestructs.h"
#include "../pcmfilemanager.h"

const char* nonexistentfile = "nonexistentfile";
const char* existentfile = "coronavirus.wav";

// add necessary includes here

class TestPcmFileManager : public QObject
{
    Q_OBJECT

public:
    TestPcmFileManager();
    ~TestPcmFileManager();

private slots:
    void test_constructor();
    void test_openFiles();
    void test_closeFiles();
    void test_getFrameCount();
    void test_readData();
    void test_seek();
};

TestPcmFileManager::TestPcmFileManager()
{

}

TestPcmFileManager::~TestPcmFileManager()
{

}

void TestPcmFileManager::test_constructor()
{
//    /** PcmFileManager::Constructor*/
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, nonexistentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        QVERIFY(manager.m_fileCount == fileCount);
//        QVERIFY(strcmp(manager.m_fileStructs[0]->m_fileName, nonexistentfile) == 0);
//    }
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, existentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        QVERIFY(manager.m_fileCount == fileCount);
//        QVERIFY(strcmp(manager.m_fileStructs[0]->m_fileName, existentfile) == 0);
//    }
    /** PcmFileManager::Constructor*/
}

void TestPcmFileManager::test_openFiles()
{
//    /** PcmFileManager::openfiles*/
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, nonexistentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();
//        QVERIFY(manager.m_fileStructs[0]->m_file == nullptr);
//    }
//    //two channels, one file. file exists
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, existentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();
//        QVERIFY(manager.m_fileStructs[0]->m_file != nullptr);
//        QVERIFY(manager.m_fileStructs[0]->fsize == 69242296);
//    }
    /** PcmFileManager::openfiles*/
}

void TestPcmFileManager::test_closeFiles()
{
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, nonexistentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();
//        int* closeResult = manager.close();
//        QVERIFY(manager.m_fileStructs[0]->m_file == nullptr);
//        QVERIFY(closeResult[0] = 1);
//        free(closeResult);
//    }
////    //two channels, one file. file does not exist
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, existentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();
//        int* closeResult = manager.close();
//        QVERIFY(manager.m_fileStructs[0]->m_file == nullptr);
//        QVERIFY(closeResult[0] = 1);
//        free(closeResult);
//    }
//    //two channels neither file exist
}

void TestPcmFileManager::test_getFrameCount()
{
//    long testfilesize = 0;
//    FILE* fp = fopen("coronavirus.wav", "rb");
//    if(fp != nullptr)
//    {
//        fseek(fp, 0L, SEEK_END);
//        testfilesize = ftell(fp);
//        fseek(fp, 0L, SEEK_SET);
//    }
//    fclose(fp);

//    //two channels, one file. file exists.
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, nonexistentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        long frameCount = manager.getFrameCount();
//        QVERIFY(frameCount == 0);

//        int* closeResults = manager.close();
//        free(closeResults);
//    }

//    //two channels, one file. file exists.
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, existentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        long frameCount = manager.getFrameCount();
//        QVERIFY(frameCount == testfilesize/2/2);

//        int* closeResults = manager.close();
//        free(closeResults);
//    }
}

void TestPcmFileManager::test_readData()
{
//    EngineAudioChunk engineAudioChunk;
//    engineAudioChunk.channelsNeeded = 2;
//    engineAudioChunk.durationNeeded = 10;
//    engineAudioChunk.sampleRateNeeded = 44100;

//    //one channel file does not exist
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, nonexistentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        const int datasize = 30;
//        SAMPLE data[datasize];
//        int res = manager.readData(data, engineAudioChunk);

//        int* closeResults = manager.close();
//        free(closeResults);

//        QVERIFY(res == -4);
//    }

//    //one channel, file exists
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, existentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        const int framesNeeded = 441;
//        const int samplesNeeded = 441*2;
//        SAMPLE sdata[samplesNeeded];
//        int res = manager.readData(sdata, engineAudioChunk);
//        QVERIFY2(res == framesNeeded*2*2, QString("expected %1. got %2").arg(framesNeeded*2*2).arg(res).toStdString().c_str());

//    }
}

void TestPcmFileManager::test_seek() {
//    //one channel file does not exist
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, nonexistentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        int res = manager.seek(0.75);
//        QVERIFY(res == -1);
//        manager.close();

//    }
////    //one channel file exists
//    {
//        const int fileCount = 1;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str->m_fileName, existentfile);
//        f_str->m_infoIsPresented = true;
//        f_str->m_file = nullptr;

//        fileStructs[0] = f_str;

//        PcmFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        int res = manager.seek(0.75);
//        QVERIFY(res == 0);
//        manager.close();
//    }
}

QTEST_APPLESS_MAIN(TestPcmFileManager)

#include "tst_testpcmfilemanager.moc"
