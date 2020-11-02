#ifndef RESAMPLER_H
#define RESAMPLER_H

#include "resampler_global.h"
#include <samplerate.h>

class RESAMPLER_EXPORT Resampler
{
public:
    Resampler();
    void resample(float* data_in, long input_frames, float* data_out, long output_frames, double src_ratio);
private:
    class SampleRateConverter {
    public:
        SampleRateConverter();
        ~SampleRateConverter();
        SRC_STATE* m_resampleHandle = nullptr;
        SRC_DATA* m_srcData = nullptr;
    } m_sampleRateConverter;

};

#endif // RESAMPLER_H
