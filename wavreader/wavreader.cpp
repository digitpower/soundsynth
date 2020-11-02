#ifdef GUI_SUPPORT
#include <QApplication>
#include <QWidget>
#include "eventaudiobufferfilled.h"
#endif
#include <stdlib.h>          //for malloc
#include <pthread.h>
#include "wavreader.h"
#include "SPSCQueue.h"
#include "strings.h"         //for bzero
#include <stdio.h>           //for FILE*
#include <unistd.h>
#include "utils.h"


#include <string.h>          //for srcpy

int g_chunkCounter;

void* readWavPackets(void* data) {
    WavReader* wavreader = static_cast<WavReader*>(data);
    char* audiodata = (char*)malloc(sizeof(char) * g_audio_buffer_length);
    (void)audiodata;
    FILE* fp = NULL;

    g_chunkCounter = 0;

#ifdef GUI_SUPPORT
    QList<QWidget*> allWidgetsList = QApplication::allWidgets();
    QWidget* mediaPlayerWidget = 0;
    foreach (QWidget* widget, allWidgetsList) {
        if(widget && widget->objectName().toStdString() == "MediaPlayer")
        {
            mediaPlayerWidget = widget;
            break;
        }
    }
    long fsize = 0;
    long samplesInFile = 0;
    double fileSeekRatio = -1.0;
    long   callbackframesInFile = -1;
    bool   fileCloseHappened        = false;
    bool   fileFirstTimeOpen = true;
#endif
    while(1){
        if(!wavreader->commandsFromMainToNonRt.empty())
        {
            WavReader::CommandFromMainToNonRt commFromMainThread = wavreader->commandsFromMainToNonRt.front();
            if(commFromMainThread.m_commandCode == wavreader->CMD_FROM_MAIN_TO_NRT_OPEN_FILE)
            {
                fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                if(true == fileFirstTimeOpen)
                {
                    fileFirstTimeOpen = false;
                    wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_PLAY_AUDIO);
                    wavreader->commandsFromMainToNonRt.pop();
                }
                else if(true == fileCloseHappened)
                {
                    fileCloseHappened = false;
                    wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_PLAY_AUDIO);
                    wavreader->commandsFromMainToNonRt.pop();
                }
            }
            if(commFromMainThread.m_commandCode == wavreader->CMD_FROM_MAIN_TO_NRT_SEEK_FILE)
            {
                fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_SEEK_FILE);
                fileSeekRatio = commFromMainThread.m_fileSeekRatio;
                wavreader->commandsFromMainToNonRt.pop();
            }
            if(commFromMainThread.m_commandCode == wavreader->CMD_FROM_MAIN_TO_NRT_CLOSE_FILE)
            {
                fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_CLOSE_FILE);
                wavreader->commandsFromMainToNonRt.pop();
            }
        }
        while(1)
        {
            if(!wavreader->commandsFromRtQueue->empty()) {
                int* ptr_command = wavreader->commandsFromRtQueue->front();
                int command = *ptr_command;
                wavreader->commandsFromRtQueue->pop();


                if(wavreader->CMD_FROMRT_CLOSE_FILE == command)
                {
                    fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                    fclose(fp);
                    wavreader->clearToRtQueue();
                    wavreader->commandsToRtQueue->push(wavreader->CMD_TORT_FILE_CLOSED);
                    fileCloseHappened = true;
                }

                if(wavreader->CMD_FROMRT_OPEN_FILE == command) {
                    fp = fopen(wavreader->m_fileName, "rb");
                    if(!fp) {
                        fprintf(stderr, "cannot open file %s\n", wavreader->m_fileName);
                    } else {
                        fseek(fp, 0L, SEEK_END);
                        fsize = ftell(fp);
                        samplesInFile = fsize / NUM_CHANNELS / sizeof(SAMPLE);
                        fseek(fp, 0L, SEEK_SET);
                        callbackframesInFile = fsize / NUM_CHANNELS / sizeof(SAMPLE) / FRAMES_PER_BUFFER;
                        g_chunkCounter = 0;
                        if(wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_FILE_OPENED)) {
                            fprintf(stderr, "CMD_TORT_FILE_OPENED pushed in commandsToRtQueue\n");
                            while (wavreader->toRtQueue->size() < wavreader->chunksInRingBuffer) {
                                DataChunk* chunk = (DataChunk*)malloc(sizeof(DataChunk*));
                                chunk->audioData = (SAMPLE*)new char[sizeof(char) * g_audio_buffer_length];
                                unsigned long n_read = fread(chunk->audioData, 1, g_audio_buffer_length, fp);
                                (void)n_read;
                                //                            fprintf(stderr, "%s %s %d %d\n", __FILE__, wavreader->m_fileName, command, __LINE__);
                                if(wavreader->toRtQueue->try_push((void*)chunk))
                                {

                                }
                            }
                        }
                        wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_GREAT_CHUNK_SENT);
                    }
                }
                if (wavreader->CMD_FROMRT_REQUEST_NEXT == command) {
                    while(wavreader->toRtQueue->size() < wavreader->chunksInRingBuffer)
                    {
                        DataChunk* chunk = (DataChunk*)malloc(sizeof(DataChunk*));
                        char* ptr_dt = new char[sizeof(char) * g_audio_buffer_length];
                        chunk->audioData = (SAMPLE*)ptr_dt;
                        chunk->chunkCounter = g_chunkCounter;
                        g_chunkCounter++;
                        unsigned long n_read = fread(chunk->audioData, 1, g_audio_buffer_length, fp);
#ifdef GUI_SUPPORT
                        if(mediaPlayerWidget)
                        {
                            QApplication::postEvent(mediaPlayerWidget, new EventAudioBufferFilled(static_cast<qint64>(g_chunkCounter*FRAMES_PER_BUFFER),
                                                                                                  static_cast<qint64>(samplesInFile)));
                        }
#endif
                        (void)n_read;
                        bool pushres = wavreader->toRtQueue->try_push((void*)chunk);
                        if(!pushres)
                            fprintf(stderr, "*********************** push result %d\n", pushres);
                    }
                    wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_CHUNK_REFILLED);
                }
                if (wavreader->CMD_FROMRT_SEEK_FILE == command) {
                    fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                    wavreader->clearToRtQueue();
                    fseek(fp, fileSeekRatio*fsize - static_cast<qint64>(fileSeekRatio*fsize) % (NUM_CHANNELS*sizeof(SAMPLE)), SEEK_SET);
                    g_chunkCounter = callbackframesInFile*fileSeekRatio;
                    wavreader->commandsToRtQueue->try_push(wavreader->CMD_TORT_FILE_SEEKED);
                    fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                }
            }
            break; /** შესაძლოა main thread-დან ახალი ბრძანება იყოს მოსული, ამიტომ გამოვიდეთ ამ შიდა ციკლიდან*/
        }
        usleep(20000);
    }
}


//FILE* g_file;

WavReader::WavReader() : Ugen()
{
//    g_file = fopen("/tmp/testfile", "wb");


    commandsFromRtQueue = new SPSCQueue<int>(16);
    commandsToRtQueue = new SPSCQueue<int>(16);

    wav_reader_stream_state = IDLE;

    chunksInRingBuffer = 256;
    toRtQueue = new SPSCQueue<void*>(chunksInRingBuffer);
}

void WavReader::seekFile(double seekRatio)
{
    CommandFromMainToNonRt cmd;
    cmd.m_commandCode = CMD_FROM_MAIN_TO_NRT_SEEK_FILE;
    cmd.m_fileSeekRatio = seekRatio;
    commandsFromMainToNonRt.push(cmd);
}


void WavReader::refill_buffers(void *outputBuffer, unsigned long framesPerBuffer)
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

    if(IDLE == wav_reader_stream_state) {
//        fprintf(stderr, "elements_dequeued %d %s %d\n", elements_dequeued, __FILE__, __LINE__);
        if(elements_dequeued == 1) {
            if(command == CMD_TORT_PLAY_AUDIO)
            {
               fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                wav_reader_stream_state = OPENING;
                commandsFromRtQueue->try_push(CMD_FROMRT_OPEN_FILE);
            }
            if(command == CMD_TORT_CLOSE_FILE)
            {
                fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                wav_reader_stream_state = FILE_IS_CLOSING;
                commandsFromRtQueue->try_push(CMD_FROMRT_CLOSE_FILE);
            }
        }
        bzero(outputBuffer, g_audio_buffer_length);
        return;
    }
    if(OPENING == wav_reader_stream_state) {
        fprintf(stderr, "elements_dequeued command: %d %d %s %d\n", command, elements_dequeued, __FILE__, __LINE__);
        if(elements_dequeued == 1 && command == CMD_TORT_FILE_OPENED) {
            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            wav_reader_stream_state = OPENED;
        }
        bzero(outputBuffer, g_audio_buffer_length);
        return;
    }
    if(OPENED == wav_reader_stream_state) {
        fprintf(stderr, "elements_dequeued %d %s %d\n", elements_dequeued, __FILE__, __LINE__);
        if(elements_dequeued == 1 && command == CMD_TORT_GREAT_CHUNK_SENT) {
            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            wav_reader_stream_state = PLAYING;
        }
        bzero(outputBuffer, g_audio_buffer_length);
        return;
    }
    if(PLAYING == wav_reader_stream_state) {
        if(elements_dequeued == 1) {
            if(CMD_TORT_CLOSE_FILE == command)
            {
                wav_reader_stream_state = FILE_IS_CLOSING;
                commandsFromRtQueue->try_push(CMD_FROMRT_CLOSE_FILE);
            }
            if(CMD_TORT_SEEK_FILE == command)
            {
                fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
                wav_reader_stream_state = FILE_IS_SEEKING;
                commandsFromRtQueue->try_push(CMD_FROMRT_SEEK_FILE);
            }
//            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            bzero(outputBuffer, g_audio_buffer_length);
            return;
        }
        if(toRtQueue->size() < toRtQueue->capacity()/2)
        {
            if(commandsFromRtQueue->try_push(CMD_FROMRT_REQUEST_NEXT))
                wav_reader_stream_state = REQUEST_FETCHING;
        }
//        fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
        pullDataAndPlay_wavreader(outputBuffer, framesPerBuffer);
        return;
    }
    if(REQUEST_FETCHING == wav_reader_stream_state) {
//        fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
        if(elements_dequeued == 1 && (command == CMD_TORT_CHUNK_REFILLED))
        {
//            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            wav_reader_stream_state = PLAYING;
        }
//        fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
        pullDataAndPlay_wavreader(outputBuffer, framesPerBuffer);
        return;
    }
    if(FILE_IS_CLOSING == wav_reader_stream_state)
    {
        fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
        if(command == CMD_TORT_FILE_CLOSED)
        {
            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            wav_reader_stream_state = IDLE;
        }
        bzero(outputBuffer, g_audio_buffer_length);
    }
    if(FILE_IS_SEEKING == wav_reader_stream_state)
    {
        fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
        if(command == CMD_TORT_FILE_SEEKED)
        {
            fprintf(stderr, "%s %d\n", __FILE__, __LINE__);
            wav_reader_stream_state = PLAYING;
        }
        bzero(outputBuffer, g_audio_buffer_length);
    }
    fprintf(stderr, "Here is problem!!!\n");
}

void WavReader::clearToRtQueue()
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

void WavReader::loadFile(const char *fileName)
{
    strcpy(m_fileName, fileName);
    if(!m_fileIsAlreadyLoaded)
    {
        m_fileIsAlreadyLoaded = true;
        pthread_create(&m_wavReaderThread, NULL, readWavPackets, this);
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

void WavReader::pullDataAndPlay_wavreader(void *outputBuffer, unsigned long framesPerBuffer)
{
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    if(!toRtQueue->empty())
    {
        DataChunk* chunk = static_cast<DataChunk*>(*toRtQueue->front());
//        fprintf(stderr, "*********************** chunk->chunkCounter %d %s %d\n", chunk->chunkCounter, __FILE__, __LINE__);
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
