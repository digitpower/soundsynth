#include "ugen_loudness.h"
#include "SPSCQueue.h"
#include "utils.h"

UGenLoudness::UGenLoudness() : Ugen()
{
}

void UGenLoudness::setVolumeRatios(double *volumeRatios)
{
    m_channelSoundsRatios[0] = volumeRatios[0];
    m_channelSoundsRatios[1] = volumeRatios[1];
}

void UGenLoudness::refill_buffers(void *outputBuffer, unsigned long framesPerBuffer)
{
    SAMPLE* wptr = (SAMPLE*)outputBuffer;
    for(ulong i = 0;i < framesPerBuffer;i++)
    {
        *wptr *= m_channelSoundsRatios[0];
        wptr++;
        *wptr *= m_channelSoundsRatios[1];
        wptr++;
    }
}
