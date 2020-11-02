#include <stdio.h>
#include <strings.h>            //for bzero
#include "audiofilereader.h"
#include "ugen_loudness.h"
#include "audioengine.h"
#include "tcpstreamlistener.h"
#include "audioengineparams.h"


static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    AudioEngine* audioEngine = static_cast<AudioEngine*>(userData);

#if 1
    if(!audioEngine->m_audioUgenList.empty())
    {
        for(auto iter = audioEngine->m_audioUgenList.begin(); iter != audioEngine->m_audioUgenList.end(); ++iter)
            (*iter)->refill_buffers(outputBuffer, framesPerBuffer);
    }
#else
    if(audioEngine->m_audioFileReader)
        audioEngine->m_audioFileReader->refill_buffers(outputBuffer, framesPerBuffer);
#endif
    else
        bzero(outputBuffer, g_audio_buffer_length);
    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    return paContinue;
}


AudioEngine::AudioEngine()
{
    g_audio_buffer_length = FRAMES_PER_BUFFER*NUM_CHANNELS*sizeof(SAMPLE);
    fprintf(stderr, "%d %s %d\n", g_audio_buffer_length, __FILE__, __LINE__);
    loadGenerators();
}

int AudioEngine::startAudioEngine()
{
    /* Playback recorded data.  -------------------------------------------- */
    PaError err = Pa_Initialize();
    if( err != paNoError )
        return cleanupPortAudio(err);
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        return cleanupPortAudio(err);
    }
    outputParameters.channelCount = NUM_CHANNELS;                     /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    printf("%d\n", __LINE__);
    m_stream = 0;
    err = Pa_OpenStream(
              &m_stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              playCallback,
              this );
    if( err != paNoError )
        return cleanupPortAudio(err);

    if( m_stream )
    {
        err = Pa_StartStream( m_stream );
        if( err != paNoError )
            return cleanupPortAudio(err);
    }
    return paNoError;
}

int AudioEngine::stopAudioEngine()
{
    if(m_stream)
    {
        PaError err = Pa_CloseStream( m_stream );
        if( err != paNoError )
            return cleanupPortAudio(err);
        printf("audio engine stopped\n");
    }
    return 0;
}

void AudioEngine::loadAudioFiles(char** audioFiles, int fileCount)
{
    if(m_audioFileReader)
    {
        m_audioFileReader->loadFiles(audioFiles, fileCount);
    }
}

void AudioEngine::seekFile(double ratio)
{
    if(m_audioFileReader)
    {
        m_audioFileReader->seekFile(ratio);
    }
}

void AudioEngine::setVolumeRatios(double *volumeRatios)
{
    if(m_ugenLoudness)
        m_ugenLoudness->setVolumeRatios(volumeRatios);
}

void AudioEngine::loadGenerators()
{
    m_audioFileReader = new AudioFileReader;
    m_ugenLoudness = new UGenLoudness;
    m_audioUgenList.push_back(m_audioFileReader);
    m_audioUgenList.push_back(m_ugenLoudness);
}

PaError AudioEngine::cleanupPortAudio(PaError err)
{
        printf("inside done label\n");
        Pa_Terminate();
        if( err != paNoError )
        {
            fprintf( stderr, "An error occured while using the portaudio stream\n" );
            fprintf( stderr, "Error number: %d\n", err );
            fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
            err = 1;          /* Always return 0 or 1, but no other return codes. */
        }
        return err;
}
