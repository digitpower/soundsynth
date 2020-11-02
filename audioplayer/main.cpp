#ifdef GUI_SUPPORT
#include "mediaplayer.h"
#else
#include <string.h>
#include "audioengine.h"
#endif
#include <signal.h>
#include "tcpstreamlistener.h"
#include "audiofilereader.h"

int g_audioStopped;

void stopaudio_sigaction(int signal, siginfo_t* si, void* arg) {
    (void)signal;
    (void)si;
    (void)arg;
    g_audioStopped = 1;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("usage: audioplayer bindipaddress\n");
        return -1;
    }
    g_audioStopped = 0;
#ifndef GUI_SUPPORT
    AudioEngine* audioEngine = new AudioEngine;
    int err = audioEngine->startAudioEngine();
    return err;
#else
    QApplication app(argc, argv);
    MediaPlayer player;
    player.show();
    return app.exec();
#endif
}
