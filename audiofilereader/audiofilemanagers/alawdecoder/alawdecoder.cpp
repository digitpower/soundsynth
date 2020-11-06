#include "alawdecoder.h"

void decodeAlaw(ENGINE_SAMPLE *decodedData, char* sourceData, int srcLength)
{
    for(int i = 0;i < srcLength; i++)
    {
        int16_t decodedSample = decodeAlawByte(sourceData[i]);
        ENGINE_SAMPLE f = ((ENGINE_SAMPLE) decodedSample) / (ENGINE_SAMPLE) 32768;
        if( f > 1 ) f = 1;
        if( f < -1 ) f = -1;
        decodedData[i] = f;
    }
}

int16_t decodeAlawByte(int8_t a_val)
{
    //https://github.com/GStreamer/gst-plugins-good/blob/master/gst/law/alaw-decode.c
    int16_t t;
    int16_t seg;

    a_val ^= 0x55;
    t = a_val & 0x7f;
    if (t < 16)
      t = (t << 4) + 8;
    else {
      seg = (t >> 4) & 0x07;
      t = ((t & 0x0f) << 4) + 0x108;
      t <<= seg - 1;
    }
    return ((a_val & 0x80) ? t : -t);
}
