#ifndef PCMWAVEFORMGENERATOR_H
#define PCMWAVEFORMGENERATOR_H

#include <stdio.h>

class PcmWaveFormGenerator
{
public:
    PcmWaveFormGenerator(char* fileName);
    void pixelsUpdated(int pixels);
    void fileContentUpdated();
private:
    bool recalculate();
private:
    FILE* m_file = nullptr;
    char* m_fileName = nullptr;
    int m_pixels = 0;
    int m_sampleSize = 2;
    int m_channelCount = 2;
};

#endif // PCMWAVEFORMGENERATOR_H
