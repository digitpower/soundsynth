#include "resampler.h"

Resampler::Resampler()
{
}

void Resampler::resample(float* data_in, long input_frames, float* data_out, long output_frames, double src_ratio)
{
    SRC_DATA* src_data = m_sampleRateConverter.m_srcData;


    src_data->data_in = data_in;
    src_data->input_frames = input_frames;
    src_data->data_out = data_out;
    src_data->output_frames = output_frames;
    src_data->src_ratio = src_ratio;
    src_process(m_sampleRateConverter.m_resampleHandle, src_data);
}

Resampler::SampleRateConverter::SampleRateConverter()
{
    int error;
    m_resampleHandle = src_new (SRC_LINEAR, 1, &error);
    m_srcData = new SRC_DATA;
}

Resampler::SampleRateConverter::~SampleRateConverter()
{
    src_delete(m_resampleHandle);
    delete m_srcData;
}
