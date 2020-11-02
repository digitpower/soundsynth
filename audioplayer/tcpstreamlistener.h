#ifndef TCPSTREAMLISTENER
#define TCPSTREAMLISTENER

#include "SPSCQueue.h"
#include "stream.h"

extern enum StreamState tcp_stream_state;


extern short MESSAGE_CODE_AUDIO_CHUNK;
extern short GREATE_AUDIO_CHUNK_SENDED;

extern short MORE_AUDIO_NEEDED;
extern int* g_commandFromRt_RefillQueue;
extern int* g_commandToRt_ResumePlay;
extern char* g_bindipaddr;

using namespace rigtorp;
extern SPSCQueue<void*>* toRtQueue;
extern SPSCQueue<void*>* fromRtQueue;

extern SPSCQueue<void*>* commandsFromRtQueue;
extern SPSCQueue<void*>* commandsToRtQueue;

extern int chunksInRingBuffer;

void init_tcp_stremlistener(char *ipaddr, int ipaddrlen);

void receiveNextAudioData(int connectedfd, char* _buffCode, char* _dataLengthArray, char* _chunkCounterArray);
void* readUdpPackets(void* data);
void pullDataAndPlay(void *outputBuffer, unsigned long framesPerBuffer);

void tcpstreamlistener_refill_buffers(void *outputBuffer, unsigned long framesPerBuffer);

#endif // TCPSTREAMLISTENER

