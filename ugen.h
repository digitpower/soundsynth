#ifndef UGEN_H
#define UGEN_H

class Ugen {
public:
    Ugen() {}
    virtual void refill_buffers(void *outputBuffer, unsigned long framesPerBuffer) = 0;
};

#endif
