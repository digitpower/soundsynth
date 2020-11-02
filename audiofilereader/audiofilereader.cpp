#ifdef GUI_SUPPORT
#include <QApplication>
#include <QWidget>
#include "eventaudiobufferfilled.h"
#endif
#include <stdlib.h>          //for malloc
#include <pthread.h>
#include <stdio.h>           //for FILE*
#include <unistd.h>
#include <string.h>          //for srcpy
#include "audiofilereader.h"
#include "audiofilemanagerproxy.h"
#include "SPSCQueue.h"
#include "strings.h"         //for bzero
#include "utils.h"


/** pid_t x = syscall(__NR_gettid); */
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
/** pid_t x = syscall(__NR_gettid); */




int g_chunkCounter;


int get_file_count_from_fileNames(char** fileNames)
{
    int i = 0;
    while(fileNames[i] != nullptr)
        i++;
    return i;
}


void* readAudioFilePackets(void* data) {
    AudioFileReader* audiofilereader = static_cast<AudioFileReader*>(data);
    char* audiodata = (char*)malloc(sizeof(char) * g_audio_buffer_length);
    (void)audiodata;

    AudioFileManagerProxy* audioFileManagerProxy = nullptr;
    g_chunkCounter = 0;

#ifdef GUI_SUPPORT
    QList<QWidget*> allWidgetsList = QApplication::allWidgets();
    QWidget* mediaPlayerWidget = nullptr;
    foreach (QWidget* widget, allWidgetsList) {
        if(widget && widget->objectName().toStdString() == "MediaPlayer")
        {
            mediaPlayerWidget = widget;
            break;
        }
    }
    long framesInFile = 0;
    double fileSeekRatio = -1.0;
    long   callbacksInFile = -1;
    bool   fileCloseHappened        = false;
    bool   fileFirstTimeOpen = true;
#endif
    while(1){
        /** main thread-დან მოსულ ბრძანებეს გადავამისამართებთ realtime thread-ისკენ*/
        if(!audiofilereader->commandsFromMainToNonRt.empty())
        {
            AudioFileReader::CommandFromMainToNonRt commFromMainThread = audiofilereader->commandsFromMainToNonRt.front();
            if(commFromMainThread.m_commandCode == audiofilereader->CMD_FROM_MAIN_TO_NRT_OPEN_FILE)
            {
                if(true == fileFirstTimeOpen)
                {
                    fileFirstTimeOpen = false;
                    audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_PLAY_AUDIO);
                    audiofilereader->commandsFromMainToNonRt.pop();
                }
                else if(true == fileCloseHappened)
                {
                    fileCloseHappened = false;
                    audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_PLAY_AUDIO);
                    audiofilereader->commandsFromMainToNonRt.pop();
                }
            }
            if(commFromMainThread.m_commandCode == audiofilereader->CMD_FROM_MAIN_TO_NRT_SEEK_FILE)
            {
                audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_SEEK_FILE);
                fileSeekRatio = commFromMainThread.m_fileSeekRatio;
                audiofilereader->commandsFromMainToNonRt.pop();
            }
            if(commFromMainThread.m_commandCode == audiofilereader->CMD_FROM_MAIN_TO_NRT_CLOSE_FILE)
            {
                audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_CLOSE_FILE);
                audiofilereader->commandsFromMainToNonRt.pop();
            }
        }

        /** აქ დავამუშავებთ realtime thread-დან მიღებულ ბრძანებებს*/
        while(1)
        {

            if(!audiofilereader->commandsFromRtQueue->empty()) {
                int* ptr_command = audiofilereader->commandsFromRtQueue->front();
                int command = *ptr_command;
                audiofilereader->commandsFromRtQueue->pop();


                if(audiofilereader->CMD_FROMRT_CLOSE_FILE == command)
                {
                    audioFileManagerProxy->close();
                    delete audioFileManagerProxy;

//                    pid_t x = syscall(__NR_gettid);
                    audiofilereader->clearToRtQueue();
                    audiofilereader->commandsToRtQueue->push(audiofilereader->CMD_TORT_FILE_CLOSED);
                    fileCloseHappened = true;
                }

                if(audiofilereader->CMD_FROMRT_OPEN_FILE == command) {
                    int fileCount = get_file_count_from_fileNames(audiofilereader->m_fileNames);
                    audioFileManagerProxy = new AudioFileManagerProxy(audiofilereader->m_fileNames, fileCount);



                    audioFileManagerProxy->open();
                    if(false) {
                    } else {
                        framesInFile = audioFileManagerProxy->getFrameCount();
                        callbacksInFile = framesInFile / FRAMES_PER_BUFFER;


                        g_chunkCounter = 0;
                        if(audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_FILE_OPENED)) {
                            fprintf(stderr, "CMD_TORT_FILE_OPENED pushed in commandsToRtQueue\n");
                            while (audiofilereader->toRtQueue->size() < audiofilereader->chunksInRingBuffer) {
                                DataChunk* chunk = (DataChunk*)malloc(sizeof(DataChunk*));
                                chunk->audioData = (SAMPLE*)new char[sizeof(char) * g_audio_buffer_length];
                                EngineAudioChunk engineAudioChunk;
                                engineAudioChunk.channelsNeeded = 2;
                                engineAudioChunk.durationNeeded = 10;
                                engineAudioChunk.sampleRateNeeded = 44100;
                                audioFileManagerProxy->readData(chunk->audioData, engineAudioChunk);
                                if(audiofilereader->toRtQueue->try_push((void*)chunk))
                                {

                                }
                            }
                        }
                        audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_GREAT_CHUNK_SENT);
                    }
                }
                if (audiofilereader->CMD_FROMRT_REQUEST_NEXT == command) {
                    while(audiofilereader->toRtQueue->size() < audiofilereader->chunksInRingBuffer)
                    {
                        DataChunk* chunk = (DataChunk*)malloc(sizeof(DataChunk*));
                        char* ptr_dt = new char[sizeof(char) * g_audio_buffer_length];
                        chunk->audioData = (SAMPLE*)ptr_dt;
                        chunk->chunkCounter = g_chunkCounter;
                        g_chunkCounter++;
                        EngineAudioChunk engineAudioChunk;
                        engineAudioChunk.channelsNeeded = 2;
                        engineAudioChunk.durationNeeded = 10;
                        engineAudioChunk.sampleRateNeeded = 44100;
                        audioFileManagerProxy->readData(chunk->audioData, engineAudioChunk);
#ifdef GUI_SUPPORT
                        if(mediaPlayerWidget)
                        {
                            QApplication::postEvent(mediaPlayerWidget, new EventAudioBufferFilled(static_cast<qint64>(g_chunkCounter*FRAMES_PER_BUFFER),
                                                                                                  static_cast<qint64>(framesInFile)));
                        }
#endif
                        bool pushres = audiofilereader->toRtQueue->try_push((void*)chunk);
                        if(!pushres)
                            fprintf(stderr, "*********************** push result %d\n", pushres);
                    }
                    audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_CHUNK_REFILLED);
                }
                if (audiofilereader->CMD_FROMRT_SEEK_FILE == command) {
                    audiofilereader->clearToRtQueue();
                    int seekRes = audioFileManagerProxy->seek(fileSeekRatio);
                    g_chunkCounter = callbacksInFile*fileSeekRatio;
                    audiofilereader->commandsToRtQueue->try_push(audiofilereader->CMD_TORT_FILE_SEEKED);
                }
            }
            break; /** შესაძლოა main thread-დან ახალი ბრძანება იყოს მოსული, ამიტომ გამოვიდეთ ამ შიდა ციკლიდან*/
        }
        usleep(20000);
    }
}


//FILE* g_file;

AudioFileReader::AudioFileReader() : Ugen()
{
    commandsFromRtQueue = new SPSCQueue<int>(16);
    commandsToRtQueue = new SPSCQueue<int>(16);

    audiofile_reader_stream_state = IDLE;

    chunksInRingBuffer = 256;
    toRtQueue = new SPSCQueue<void*>(chunksInRingBuffer);
}

void AudioFileReader::seekFile(double seekRatio)
{
    CommandFromMainToNonRt cmd;
    cmd.m_commandCode = CMD_FROM_MAIN_TO_NRT_SEEK_FILE;
    cmd.m_fileSeekRatio = seekRatio;
    commandsFromMainToNonRt.push(cmd);
}

void AudioFileReader::refill_buffers(void *outputBuffer, unsigned long framesPerBuffer)
{
    (void)outputBuffer;
    (void)framesPerBuffer;


    int elements_dequeued = 0;
    int* ptr_command;
    int command;


    if(!commandsToRtQueue->empty())
    {
        ptr_command = commandsToRtQueue->front();
        commandsToRtQueue->pop();
        command = *ptr_command;
        elements_dequeued = 1;
    }

    if(IDLE == audiofile_reader_stream_state) {
//        fprintf(stderr, "elements_dequeued %d %s %d\n", elements_dequeued, __FILE__, __LINE__);
        if(elements_dequeued == 1) {
            if(command == CMD_TORT_PLAY_AUDIO)
            {
                audiofile_reader_stream_state = OPENING;
                commandsFromRtQueue->try_push(CMD_FROMRT_OPEN_FILE);
            }
            if(command == CMD_TORT_CLOSE_FILE)
            {
                audiofile_reader_stream_state = FILE_IS_CLOSING;
                commandsFromRtQueue->try_push(CMD_FROMRT_CLOSE_FILE);
            }
        }
        bzero(outputBuffer, g_audio_buffer_length);
        return;
    }
    if(OPENING == audiofile_reader_stream_state) {
        fprintf(stderr, "elements_dequeued command: %d %d %s %d\n", command, elements_dequeued, __FILE__, __LINE__);
        if(elements_dequeued == 1 && command == CMD_TORT_FILE_OPENED) {
            audiofile_reader_stream_state = OPENED;
        }
        bzero(outputBuffer, g_audio_buffer_length);
        return;
    }
    if(OPENED == audiofile_reader_stream_state) {
        fprintf(stderr, "elements_dequeued %d %s %d\n", elements_dequeued, __FILE__, __LINE__);
        if(elements_dequeued == 1 && command == CMD_TORT_GREAT_CHUNK_SENT) {
            audiofile_reader_stream_state = PLAYING;
        }
        bzero(outputBuffer, g_audio_buffer_length);
        return;
    }
    if(PLAYING == audiofile_reader_stream_state) {
        if(elements_dequeued == 1) {
            if(CMD_TORT_CLOSE_FILE == command)
            {
                audiofile_reader_stream_state = FILE_IS_CLOSING;
                commandsFromRtQueue->try_push(CMD_FROMRT_CLOSE_FILE);
            }
            if(CMD_TORT_SEEK_FILE == command)
            {
                audiofile_reader_stream_state = FILE_IS_SEEKING;
                commandsFromRtQueue->try_push(CMD_FROMRT_SEEK_FILE);
            }
            bzero(outputBuffer, g_audio_buffer_length);
            return;
        }
        if(toRtQueue->size() < toRtQueue->capacity()/2)
        {
            if(commandsFromRtQueue->try_push(CMD_FROMRT_REQUEST_NEXT))
                audiofile_reader_stream_state = REQUEST_FETCHING;
        }
        pullDataAndPlay_audiofilereader(outputBuffer, framesPerBuffer);
        return;
    }
    if(REQUEST_FETCHING == audiofile_reader_stream_state) {
        if(elements_dequeued == 1 && (command == CMD_TORT_CHUNK_REFILLED))
        {
            audiofile_reader_stream_state = PLAYING;
        }
        pullDataAndPlay_audiofilereader(outputBuffer, framesPerBuffer);
        return;
    }
    if(FILE_IS_CLOSING == audiofile_reader_stream_state)
    {
        if(command == CMD_TORT_FILE_CLOSED)
        {
            audiofile_reader_stream_state = IDLE;
        }
        bzero(outputBuffer, g_audio_buffer_length);
    }
    if(FILE_IS_SEEKING == audiofile_reader_stream_state)
    {
        if(command == CMD_TORT_FILE_SEEKED)
        {
            audiofile_reader_stream_state = PLAYING;
        }
        bzero(outputBuffer, g_audio_buffer_length);
    }
    fprintf(stderr, "Here is problem!!!\n");
}

void AudioFileReader::clearToRtQueue()
{
    while(!toRtQueue->empty())
    {
        DataChunk* chunk = (DataChunk* )*toRtQueue->front();
#if 0
        delete[] chunk->audioData;
        free(chunk);
#endif
        toRtQueue->pop();
    }
}

void AudioFileReader::loadFiles(char** audioFiles, int fileCount)
{
    m_fileNames = audioFiles;
    pid_t x = syscall(__NR_gettid);

    if(!m_fileIsAlreadyLoaded)
    {
        m_fileIsAlreadyLoaded = true;
        pthread_create(&m_audioFileReaderThread, NULL, readAudioFilePackets, this);
    }
    else {
        CommandFromMainToNonRt cmd;
        cmd.m_commandCode = CMD_FROM_MAIN_TO_NRT_CLOSE_FILE;
        commandsFromMainToNonRt.push(cmd);
    }
    CommandFromMainToNonRt cmd;
    cmd.m_commandCode = CMD_FROM_MAIN_TO_NRT_OPEN_FILE;
    commandsFromMainToNonRt.push(cmd);
}

void AudioFileReader::pullDataAndPlay_audiofilereader(void *outputBuffer, unsigned long framesPerBuffer)
{
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    if(!toRtQueue->empty())
    {
        DataChunk* chunk = static_cast<DataChunk*>(*toRtQueue->front());
        toRtQueue->pop();
        SAMPLE* rptr = (SAMPLE*)chunk->audioData;

//        fwrite(chunk->audioData, 1, g_audio_buffer_length, g_file);

        for( unsigned long i=0; i<framesPerBuffer; i++ )
        {
            SAMPLE left = *rptr++;
            SAMPLE right = *rptr++;
            *wptr++ = left;
            *wptr++ = right;
        }
    }
    else
    {
        bzero(outputBuffer, g_audio_buffer_length);
        fprintf(stderr, "lock free queue was empty\n");
    }
}
