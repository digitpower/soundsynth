#ifndef ALAWWAVEFORMGENERATOR_H
#define ALAWWAVEFORMGENERATOR_H

#include <stdio.h>
#include "alawwaveformgenerator_global.h"

class ALAWWAVEFORMGENERATOR_EXPORT AlawWaveFormGenerator
{
public:
    AlawWaveFormGenerator(char* fileName);
    void pixelsUpdated(int pixels);
    void fileContentUpdated();
private:
    bool recalculate();
private:
    FILE* m_file = nullptr;
    char* m_fileName = nullptr;
    int m_pixels = 0;
};

#endif // ALAWWAVEFORMGENERATOR_H
