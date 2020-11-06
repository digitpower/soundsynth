#include "alawwaveformgenerator.h"
#include "alawdecoder.h"

AlawWaveFormGenerator::AlawWaveFormGenerator(char* fileName)
{
}

void AlawWaveFormGenerator::pixelsUpdated(int pixels)
{
    m_pixels = pixels;
    bool res = recalculate();
}

void AlawWaveFormGenerator::fileContentUpdated()
{
    bool res = recalculate();
}

bool AlawWaveFormGenerator::recalculate()
{
    if(!m_file)
        m_file = fopen(m_fileName, "rb");
    if(!m_file)
        return false;
    fseek(m_file, 0L, SEEK_END);
    long fsize = ftell(m_file);
    fseek(m_file, 0L, SEEK_SET);

    long sampleCount = fsize ;
    long samplesOnEachPixel = sampleCount / m_pixels;


    char* buffer = (char*)malloc(sizeof(char)*samplesOnEachPixel);
    float* decodedData = (float*)malloc(sizeof(float)*samplesOnEachPixel);

    while(fread(buffer,1,samplesOnEachPixel,m_file))
    {
        decodeAlaw(decodedData, buffer, samplesOnEachPixel);
        float max = *(std::max(decodedData, decodedData + samplesOnEachPixel));
        float min = *(std::min(decodedData, decodedData + samplesOnEachPixel));
    }
    return true;
}
