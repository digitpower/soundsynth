#include "pcmwaveformgenerator.h"

PcmWaveFormGenerator::PcmWaveFormGenerator(char *fileName)
{

}

void PcmWaveFormGenerator::pixelsUpdated(int pixels)
{
    m_pixels = pixels;
    bool res = recalculate();
}

void PcmWaveFormGenerator::fileContentUpdated()
{
    bool res = recalculate();
}

bool PcmWaveFormGenerator::recalculate()
{
    if(!m_file)
        m_file = fopen(m_fileName, "rb");
    if(!m_file)
        return false;
    fseek(m_file, 0L, SEEK_END);
    long fsize = ftell(m_file);
    fseek(m_file, 0L, SEEK_SET);

    long frameCount = fsize / m_channelCount / m_sampleSize;
    long framesOnEachPixel = frameCount / m_pixels;




    return true;
}
