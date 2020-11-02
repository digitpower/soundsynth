#include <stdlib.h>            //for calloc, malloc
#include <string.h>            //for strcpy
#include <sys/socket.h>        //for AF_INET, setsockopt, bind, listen, accept
#include <netinet/in.h>        //for sockaddr_in
#include <arpa/inet.h>         //for inet_pton

#include <stdio.h>             //for printf
#include <pthread.h>
#include <unistd.h>            //for usleep

#include "utils.h"

#include "tcpstreamlistener.h"

short MESSAGE_CODE_AUDIO_CHUNK;
short GREATE_AUDIO_CHUNK_SENDED;

short MORE_AUDIO_NEEDED;


enum StreamState tcp_stream_state;


int* g_commandFromRt_RefillQueue;

int* g_commandToRt_ResumePlay;

char* g_bindipaddr;

SPSCQueue<void*>* toRtQueue;
SPSCQueue<void*>* fromRtQueue;

SPSCQueue<void*>* commandsFromRtQueue;
SPSCQueue<void*>* commandsToRtQueue;

int chunksInRingBuffer;


pthread_t udpThread;

void init_tcp_stremlistener(char* ipaddr, int ipaddrlen)
{


    pthread_create(&udpThread, NULL, readUdpPackets, NULL);

    MESSAGE_CODE_AUDIO_CHUNK = 1;
    GREATE_AUDIO_CHUNK_SENDED = 2;

    MORE_AUDIO_NEEDED = 3;


    g_commandFromRt_RefillQueue = (int*)malloc(sizeof(int));
    *g_commandFromRt_RefillQueue = 1;

    g_commandToRt_ResumePlay = (int*)malloc(sizeof(int));
    *g_commandToRt_ResumePlay = 1;


    const int ipaddrsize = 20;
    g_bindipaddr = (char*)calloc(ipaddrsize, 1);
    strcpy(g_bindipaddr, ipaddr);
    g_bindipaddr[ipaddrlen] = 0;


    chunksInRingBuffer = 256;
    toRtQueue = new SPSCQueue<void*>(chunksInRingBuffer);
    commandsFromRtQueue = new SPSCQueue<void*>(16);
    commandsToRtQueue = new SPSCQueue<void*>(16);


    tcp_stream_state = IDLE;

}

void receiveNextAudioData(int connectedfd, char* _buffCode, char* _dataLengthArray, char* _chunkCounterArray)
{
    (void)_buffCode;
    (void)_chunkCounterArray;
    (void)_dataLengthArray;
    while(1)
    {
        char buffCode[2] = {0};
        int elementsread1 = readBytesFromSocket(connectedfd, buffCode, 2);
        short code = atoi(buffCode);

        if(code == MESSAGE_CODE_AUDIO_CHUNK)
        {
            char dataLengthArray[4] = {0};
            int elementsread2 = readBytesFromSocket(connectedfd, dataLengthArray, 4);
            short dataSize = strToShort(dataLengthArray, 4);
            if(dataSize != 1764)
            {
                int a = 0;
                a++;
                for(int i = 0;i < 4;i++)
                    printf("%02x ", dataLengthArray[i]);
                printf(" elementsread1=%d elementsread2=%d %d\n", elementsread1, elementsread2, __LINE__);
            }
            char chunkCounterArray[5] = {0};
            int elementsread3 = readBytesFromSocket(connectedfd, chunkCounterArray, 5);
            (void)elementsread3;


            DataChunk* chunk = (DataChunk*)calloc(1, sizeof(DataChunk*));
            chunk->audioData = (SAMPLE*)calloc(dataSize, 1);
            chunk->chunkCounter = atoi(chunkCounterArray);

            readBytesFromSocket(connectedfd, (char*)chunk->audioData, (size_t)dataSize);


//            printf("    ----elementsread1=%d elementsread2=%d elementsread3=%d buffCode=%s code=%d dataLengthArray=%s dataSize=%d chunkCounterArray=%s chunk->chunkCounter=%d __LINE__=%d\n",
//                   elementsread1, elementsread2, elementsread3, buffCode, code, dataLengthArray, dataSize, chunkCounterArray, atoi(chunkCounterArray), __LINE__);


            bool pushresult = toRtQueue->try_push((void*)chunk);
            if(pushresult)
                printf("pushresult is %d\n", pushresult);
        }
        else if(code == GREATE_AUDIO_CHUNK_SENDED)
        {
            commandsToRtQueue->try_push((void*)g_commandToRt_ResumePlay);
            break;
        }
    }
}

void* readUdpPackets(void* data)
{
    (void)data;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        printf("%s %d\n", strerror(errno), __LINE__);
        return NULL;
    }

    int option = 1;
    int res = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if(res == -1)
    {
        printf("%s %d\n", strerror(errno), __LINE__);
        return NULL;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, g_bindipaddr, &(servaddr.sin_addr));
    servaddr.sin_port = htons(6666);


    if(bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("%s %d\n", strerror(errno), __LINE__);
        return NULL;
    }


    if(listen(sockfd, 5) == -1)
    {
        printf("%s %d\n", strerror(errno), __LINE__);
        return NULL;
    }

    int connectedfd = accept(sockfd, NULL, NULL);
    if(connectedfd == -1)
    {
        printf("%s %d\n", strerror(errno), __LINE__);
        return NULL;
    }



    const int MAX_AUDIO_CHUNK_SIZE = 2000;
    char audioChunk[MAX_AUDIO_CHUNK_SIZE];
    (void)audioChunk;


    const int CODESIZE = 2;
    (void)CODESIZE;
    char buffCode[2];

    const int BYTESSIZE = 4;
    (void)BYTESSIZE;
    char dataLengthArray[4];

    const int CHUNKCOUNTERSIZE = 5;
    (void)CHUNKCOUNTERSIZE;
    char chunkCounterArray[5];




    receiveNextAudioData(connectedfd, buffCode, dataLengthArray, chunkCounterArray);

    /** ციკლი, რომელიც უსმენს realtime audio message-ებს*/
    while(1)
    {

        if(!commandsFromRtQueue->empty())
        {

            int* ptr_command = (int*)commandsFromRtQueue->front();
            int command = *ptr_command;
            commandsFromRtQueue->pop();

            if(command == 1)
            {
                char buffer[4];

                short* code = (short*)buffer;
                *code = MORE_AUDIO_NEEDED;


                short* ptrAudioPacketsCount = (short*)(buffer+2);
                *ptrAudioPacketsCount = short(toRtQueue->capacity() - toRtQueue->size());

                ssize_t res =  write(connectedfd, (char*)&buffer, sizeof(buffer));
                if(res)
                {

                }
                receiveNextAudioData(connectedfd, buffCode, dataLengthArray, chunkCounterArray);
            }
        }
        usleep(10000);
    }
}

void pullDataAndPlay(void *outputBuffer, unsigned long framesPerBuffer)
{
    SAMPLE *wptr = (SAMPLE*)outputBuffer;

    if(!toRtQueue->empty())
    {
        DataChunk* chunk = (DataChunk*)toRtQueue->front();
        toRtQueue->pop();
        SAMPLE* rptr = (SAMPLE*)chunk->audioData;
        for( unsigned long i=0; i<framesPerBuffer; i++ )
        {
            SAMPLE left = *rptr++;
            SAMPLE right = *rptr++;
            *wptr++ = left*1.0;
            *wptr++ = right*1.0;
        }
    }
    else
    {
        bzero(outputBuffer, g_audio_buffer_length);
        printf("lock free queue was empty\n");
    }
}

void tcpstreamlistener_refill_buffers(void *outputBuffer, unsigned long framesPerBuffer)
{
    int** ptr_command;
    (void)ptr_command;
    int elements_dequeued = 0;
    int command = -1;
    if(!commandsToRtQueue->empty())
    {
        ptr_command = (int **)commandsToRtQueue->front();
        commandsToRtQueue->pop();
        elements_dequeued = 1;
    }
    if(IDLE == tcp_stream_state) {
        if(elements_dequeued == 1 && command == 1)
            tcp_stream_state = PLAYING;
        bzero(outputBuffer, g_audio_buffer_length);
    }
    else if(PLAYING == tcp_stream_state) {
        if(toRtQueue->size() < static_cast<size_t>(chunksInRingBuffer)/2)
        {
            commandsFromRtQueue->try_push(g_commandFromRt_RefillQueue);
            tcp_stream_state = REQUEST_FETCHING;
        }
        pullDataAndPlay(outputBuffer, framesPerBuffer);
    }
    else if (REQUEST_FETCHING == tcp_stream_state) {
        if(elements_dequeued == 1 && (command == *g_commandToRt_ResumePlay))
            tcp_stream_state = PLAYING;
        pullDataAndPlay(outputBuffer, framesPerBuffer);
    }
}
