#ifndef AUDIO_ENGINE
#define AUDIO_ENGINE


#include <portaudio.h>
#include <list>

class WavReader;
class AudioFileReader;
class UGenLoudness;
class Ugen;

class AudioEngine {
public:
    AudioEngine();
    int startAudioEngine();
    int stopAudioEngine();

    void loadAudioFiles(char **audioFiles, int fileCount);
    void seekFile(double ratio);



    void setVolumeRatios(double* volumeRatios);
private:
    void loadGenerators();
    PaError cleanupPortAudio(PaError err);
private:
    PaStream* m_stream = nullptr;
public:
    AudioFileReader* m_audioFileReader = nullptr;
    UGenLoudness* m_ugenLoudness;
    std::list<Ugen*> m_audioUgenList;
};


#endif
