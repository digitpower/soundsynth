#ifndef ALAWDECODER_H
#define ALAWDECODER_H

#define ENGINE_SAMPLE float
#include <inttypes.h>

void decodeAlaw(ENGINE_SAMPLE *decodedData, char* sourceData, int srcLength);
int16_t decodeAlawByte(int8_t a_val);

#endif // ALAWDECODER_H
