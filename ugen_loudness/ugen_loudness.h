#ifndef UGEN_LOUDNESS
#define UGEN_LOUDNESS

#include "ugen.h"

class UGenLoudness : public Ugen {
public:
    UGenLoudness();
    void setVolumeRatios(double *volumeRatios);
    void refill_buffers(void *outputBuffer, unsigned long framesPerBuffer);
private:
    static const int CHANNEL_COUNT = 2;
    double m_channelSoundsRatios[CHANNEL_COUNT];
};

#endif
