#ifndef EVENTAUDIOBUFFERFILLED_H
#define EVENTAUDIOBUFFERFILLED_H


#include <QEvent>

class EventAudioBufferFilled : public QEvent {

public:
    EventAudioBufferFilled(qint64 tickSamples, qint64 fileSampleCounter)
        : QEvent(QEvent::User),
          m_tickSamples(tickSamples),
          m_fileSamplesCount(fileSampleCounter)
    {}
    qint64 m_tickSamples = 0;
    qint64 m_fileSamplesCount = 0;
};

#endif // EVENTAUDIOBUFFERFILLED_H
