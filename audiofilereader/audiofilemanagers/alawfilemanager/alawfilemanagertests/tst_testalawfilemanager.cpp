#include <QtTest>
#include <QFile>
// add necessary includes here
#include "../../filestructs.h"
#include "../alawfilemanager.h"
#include <string.h>
#include <assert.h>

const char* nonexistentfile = "nonexistentfile";
#if 0
const char* leftaudio = "3749426199_1";
const char* rightaudio = "3749426199_2";
QVERIFY(manager.m_fileStructs[0]->fsize == 2051896);
QVERIFY(manager.m_fileStructs[1]->fsize == 2051096);
#else
const char* leftaudio = "1.alaw";
const char* rightaudio = "2.alaw";
//QVERIFY(manager.m_fileStructs[0]->fsize == 23916);
//QVERIFY(manager.m_fileStructs[1]->fsize == 19998);
#endif

class TestAlawFileManager : public QObject
{
    Q_OBJECT

public:
    TestAlawFileManager();
    ~TestAlawFileManager();

private slots:
    void test_constructor();
    void test_openFiles();
    void test_closeFiles();
    void test_getFrameCount();
    void test_readData();
    void test_seek();
};

TestAlawFileManager::TestAlawFileManager()
{

}

TestAlawFileManager::~TestAlawFileManager()
{

}

void TestAlawFileManager::test_constructor()
{
//    /** AlawFileManager::Constructor*/
//    {
//        const int fileCount = 2;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str_left = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_left->m_fileName, leftaudio);
//        f_str_left->m_infoIsPresented = true;
//        f_str_left->m_file = nullptr;
//        fileStructs[0] = f_str_left;

//        FileStruct* f_str_right = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_right->m_fileName, rightaudio);
//        f_str_right->m_infoIsPresented = true;
//        f_str_right->m_file = nullptr;
//        fileStructs[1] = f_str_right;

//        AlawFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        QVERIFY(manager.m_fileCount == fileCount);
//        QVERIFY(strcmp(manager.m_fileStructs[0]->m_fileName, leftaudio) == 0);
//        QVERIFY(strcmp(manager.m_fileStructs[1]->m_fileName, rightaudio) == 0);
//    }
//    /** AlawFileManager::Constructor*/
}
void TestAlawFileManager::test_openFiles()
{
    /** PcmFileManager::openfiles*/
//    {
//        const int fileCount = 2;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str_left = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_left->m_fileName, leftaudio);
//        f_str_left->m_infoIsPresented = true;
//        f_str_left->m_file = nullptr;
//        fileStructs[0] = f_str_left;

//        FileStruct* f_str_right = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_right->m_fileName, rightaudio);
//        f_str_right->m_infoIsPresented = true;
//        f_str_right->m_file = nullptr;
//        fileStructs[1] = f_str_right;

//        AlawFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();
//        QVERIFY(manager.m_fileStructs[0]->m_file != nullptr);
//        QVERIFY(manager.m_fileStructs[1]->m_file != nullptr);
//        QVERIFY(manager.m_fileStructs[0]->fsize == 23916);
//        QVERIFY(manager.m_fileStructs[1]->fsize == 19998);
//    }
    /** PcmFileManager::openfiles*/
}

void TestAlawFileManager::test_closeFiles()
{
//    //two channels, one file. file does not exist
//    {
//        const int fileCount = 2;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str_left = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_left->m_fileName, leftaudio);
//        f_str_left->m_infoIsPresented = true;
//        f_str_left->m_file = nullptr;
//        fileStructs[0] = f_str_left;

//        FileStruct* f_str_right = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_right->m_fileName, rightaudio);
//        f_str_right->m_infoIsPresented = true;
//        f_str_right->m_file = nullptr;
//        fileStructs[1] = f_str_right;

//        AlawFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();
//        int* closeResult = manager.close();
//        QVERIFY(manager.m_fileStructs[0]->m_file == nullptr);
//        QVERIFY(manager.m_fileStructs[1]->m_file == nullptr);
//        QVERIFY(closeResult[0] = 1);
//        QVERIFY(closeResult[1] = 1);
//        free(closeResult);
//    }
//    //two channels neither file exist
}

void TestAlawFileManager::test_getFrameCount()
{
//    //two channels, one file. file exists.
//    {
//        const int fileCount = 2;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str_left = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_left->m_fileName, leftaudio);
//        f_str_left->m_infoIsPresented = true;
//        f_str_left->m_file = nullptr;
//        fileStructs[0] = f_str_left;

//        FileStruct* f_str_right = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_right->m_fileName, rightaudio);
//        f_str_right->m_infoIsPresented = true;
//        f_str_right->m_file = nullptr;
//        fileStructs[1] = f_str_right;

//        AlawFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        long frameCount = manager.getFrameCount();
//#if 0
//        QVERIFY(frameCount == std::max(2051096, /** 3749426199_2*/
//                                      2051896/** 3749426199_1*/));
//#endif
//        QVERIFY(frameCount == std::max(23916, /** 1.alaw*/
//                                      19998/** 2.alaw*/));
//        int* closeResults = manager.close();
//        free(closeResults);
//    }
}

void TestAlawFileManager::test_readData()
{
    EngineAudioChunk engineAudioChunk;
    engineAudioChunk.channelsNeeded = 2;
    engineAudioChunk.durationNeeded = 10;
    engineAudioChunk.sampleRateNeeded = 44100;
    //one channel, file exists
//    {
//        const int fileCount = 2;
//        FileStruct** fileStructs = (FileStruct**)malloc(sizeof(FileStruct*) * fileCount);

//        FileStruct* f_str_left = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_left->m_fileName, leftaudio);
//        f_str_left->m_infoIsPresented = true;
//        f_str_left->m_file = nullptr;
//        fileStructs[0] = f_str_left;

//        FileStruct* f_str_right = (FileStruct*)malloc(sizeof(FileStruct));
//        strcpy(f_str_right->m_fileName, rightaudio);
//        f_str_right->m_infoIsPresented = true;
//        f_str_right->m_file = nullptr;
//        fileStructs[1] = f_str_right;

//        AlawFileManager manager(fileStructs, fileCount);
//        free(fileStructs);

//        manager.open();

//        const int frameCount = 441*2;
//        const int sampleCount = frameCount*2;
//        SAMPLE sdata[sampleCount];
//        for(int i = 0;i < 300;i++)
//        {
//            int res = manager.readData(sdata, engineAudioChunk);
//        }
//        QVERIFY(manager.m_rawDataFromChannels != nullptr);

//        QVERIFY(manager.m_rawDataFromChannels[0] != nullptr);
//        QVERIFY(manager.m_rawDataFromChannels[1] != nullptr);

//        QVERIFY(manager.m_rawDataFromChannels[0]->data != nullptr);
//        QVERIFY(manager.m_rawDataFromChannels[1]->data != nullptr);

//        QVERIFY(manager.m_rawDataFromChannels[0]->dataLength != 0);
//        QVERIFY(manager.m_rawDataFromChannels[1]->dataLength != 0);

//////        QVERIFY2(res == datasize*2, QString("expected %1. got %2").arg(80*2).arg(res).toStdString().c_str());




//        int* closeResults = manager.close();
//        free(closeResults);
//    }
}

void TestAlawFileManager::test_seek()
{

}

//void test_seek() {
////    //one channel file does not exist
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
//}


QTEST_APPLESS_MAIN(TestAlawFileManager)

#include "tst_testalawfilemanager.moc"
