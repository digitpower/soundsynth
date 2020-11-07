#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QDialog>
#include "ui_mediaplayer.h"


class AudioEngine;
class MediaPlayer : public QDialog, public Ui::MediaPlayer
{
    Q_OBJECT
public:
    explicit MediaPlayer(QDialog *parent = nullptr);
private slots:
    void on_btnPlayPause_clicked();
private slots:
    void on_btnStartEngine_clicked();
    void on_btnChooseFile_clicked();

    void on_seekSlider_valueChanged(int value);

    void on_seekSlider_sliderPressed();

    void on_seekSlider_sliderMoved(int position);

    void on_seekSlider_sliderReleased();

    void on_seekSlider_rangeChanged(int min, int max);

    void on_seekSlider_actionTriggered(int action);

    void on_volumeLeft_valueChanged(int value);

    void on_volumeRight_valueChanged(int value);

protected:
    void closeEvent(QCloseEvent* event);
    bool event(QEvent* event);
    bool eventFilter(QObject *object, QEvent *event);
private:
    AudioEngine* m_audioEngine;
private:
    void updateAudioEngineVolumeRatios();
    char **generateFilesArray(const QStringList& selectedFiles);
};

#endif // MEDIAPLAYER_H
