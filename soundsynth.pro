TEMPLATE = subdirs


SUBDIRS += resampler \
    alawwaveformgenerator \
    audiofilereader/audiofilemanagers/alawdecoder \
    waveformgenerators/pcmwaveformgenerator
SUBDIRS += audiofilereader/audiofilemanagers/alawfilemanager
SUBDIRS += audiofilereader/audiofilemanagers/pcmfilemanager
SUBDIRS += audiofilereader/audiofilemanagers/audiofilemanager

#SUBDIRS += unittests
SUBDIRS += audiofilereader
SUBDIRS += ugen_loudness
SUBDIRS += lib
SUBDIRS += audioplayer

