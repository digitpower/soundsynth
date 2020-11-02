#ifndef STREAM_H
#define STREAM_H

enum StreamState {
    IDLE             = 0,
    OPENING          = 1,
    OPENED           = 2,
    PLAYING          = 3,
    REQUEST_FETCHING = 4,
    PAUSING          = 5,
    FILE_IS_CLOSING  = 6,
    FILE_IS_SEEKING  = 7
};

#endif // STREAM_H
