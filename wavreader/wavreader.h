#ifndef WAV_READER
#define WAV_READER


#include <queue>
#include "SPSCQueue.h"
#include "stream.h"
#include "ugen.h"

using namespace rigtorp;

class WavReader : public Ugen{
public:
    WavReader();
    void seekFile(double seekRatio);
    void refill_buffers(void *outputBuffer, unsigned long framesPerBuffer);
    void clearToRtQueue();
    SPSCQueue<int>* commandsFromRtQueue;
    int CMD_FROMRT_OPEN_FILE    = 1;
    int CMD_FROMRT_REQUEST_NEXT = 2;
    int CMD_FROMRT_CLOSE_FILE   = 3;
    int CMD_FROMRT_SEEK_FILE    = 4;

    SPSCQueue<int>* commandsToRtQueue;
    int CMD_TORT_PLAY_AUDIO       = 1;
    int CMD_TORT_FILE_OPENED      = 2;
    int CMD_TORT_GREAT_CHUNK_SENT = 3;
    int CMD_TORT_CHUNK_REFILLED   = 4;
    int CMD_TORT_PAUSE_AUDIO      = 5;
    int CMD_TORT_CLOSE_FILE       = 6;
    int CMD_TORT_FILE_CLOSED      = 7;
    int CMD_TORT_SEEK_FILE        = 8;
    int CMD_TORT_FILE_SEEKED      = 9;


    struct CommandFromMainToNonRt {
        int m_commandCode;
        double m_fileSeekRatio;
    };

    std::queue<CommandFromMainToNonRt> commandsFromMainToNonRt;
    int CMD_FROM_MAIN_TO_NRT_OPEN_FILE  = 1;
    int CMD_FROM_MAIN_TO_NRT_CLOSE_FILE = 2;
    int CMD_FROM_MAIN_TO_NRT_SEEK_FILE  = 3;






    SPSCQueue<void*>* toRtQueue;
    SPSCQueue<void*>* fromRtQueue;


    int chunksInRingBuffer;
    char m_fileName[500] = {0};
public:
    void loadFile(const char* fileName);
private:
    void pullDataAndPlay_wavreader(void *outputBuffer, unsigned long framesPerBuffer);
private:
    pthread_t m_wavReaderThread;
    enum StreamState wav_reader_stream_state;
    bool m_fileIsAlreadyLoaded = false;
};

#endif
