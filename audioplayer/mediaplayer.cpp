#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "audioengine.h"
#include "mediaplayer.h"
#include "eventaudiobufferfilled.h"

MediaPlayer::MediaPlayer(QDialog *parent) :
    QDialog(parent),
    m_audioEngine(nullptr)
{
    setupUi(this);
    btnChooseFile->setEnabled(false);
    on_btnStartEngine_clicked();
    seekSlider->setTracking(false);
    seekSlider->installEventFilter(this);
    on_volumeLeft_valueChanged(0);
    on_volumeRight_valueChanged(0);
}


void MediaPlayer::on_btnPlayPause_clicked()
{

}

void MediaPlayer::on_btnStartEngine_clicked()
{
    m_audioEngine = new AudioEngine;
    int res = m_audioEngine->startAudioEngine();
    if(res)
    {
        QMessageBox::critical(this, "Error", "Audio Engine Cannot Start");
        delete m_audioEngine;
    }
    else
    {
        btnChooseFile->setEnabled(true);
        btnStartEngine->setVisible(false);
    }
}

void MediaPlayer::closeEvent(QCloseEvent *event)
{
    if(m_audioEngine)
        m_audioEngine->stopAudioEngine();
    QDialog::closeEvent(event);
}

bool MediaPlayer::event(QEvent *event)
{
    if(event->type() == QEvent::User)
    {
        EventAudioBufferFilled* ev = static_cast<EventAudioBufferFilled*>(event);
        if(ev)
        {

            seekSlider->setValue(seekSlider->maximum() * (double(ev->m_tickSamples) / ev->m_fileSamplesCount));
//            fprintf(stderr, "ticksamples: %d filesamples: %d\n", ev->m_tickSamples, ev->m_fileSamplesCount);
        }
    }
    return QDialog::event(event);
}

bool MediaPlayer::eventFilter(QObject *object, QEvent *event)
{
    if(object == seekSlider && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent && m_audioEngine)
        {
            fprintf(stderr, "mouse pressed on %d position. seekslider width is: %d\n", mouseEvent->x(), seekSlider->width());
            m_audioEngine->seekFile(double(mouseEvent->x()) / seekSlider->width());
            return true;
        }
    }
    return QDialog::eventFilter(object, event);
}

void MediaPlayer::updateAudioEngineVolumeRatios()
{
    const int CHANNEL_COUNT = 2;
    double volumeRatios[CHANNEL_COUNT];
    volumeRatios[0] = double(volumeLeft->value()) / volumeLeft->maximum();
    volumeRatios[1] = double(volumeRight->value()) / volumeRight->maximum();
    if(m_audioEngine)
        m_audioEngine->setVolumeRatios(volumeRatios);
}

char** MediaPlayer::generateFilesArray(const QStringList &selectedFiles)
{
    if(selectedFiles.isEmpty())
        return nullptr;
    int elementsInArray = selectedFiles.count() + 1;
    char** audioFiles = (char**)malloc(elementsInArray * sizeof(char*));
    for(int i = 0; i < selectedFiles.size(); i++)
    {
        QString selectedFile = selectedFiles.at(i);
        audioFiles[i] = (char*)malloc((selectedFile.size() + 1) * sizeof(char));
        strcpy(audioFiles[i], selectedFile.toStdString().c_str());
    }
    audioFiles[elementsInArray-1] = nullptr;
    return audioFiles;
}

#include <QDebug>
void MediaPlayer::on_btnChooseFile_clicked()
{
    if(m_audioEngine)
    {
        QStringList selectedFiles = QFileDialog::getOpenFileNames(this, "", "/home/user/Music");
        if(!selectedFiles.isEmpty())
        {
            char** audioFiles = generateFilesArray(selectedFiles);
//            lineEditFileName->setText(selectedFile);
            m_audioEngine->loadAudioFiles(audioFiles, selectedFiles.count());
        }
    }
}

void MediaPlayer::on_seekSlider_valueChanged(int value)
{
//    fprintf(stderr, "seekSlider value changed. value: %d\n", value);
}

void MediaPlayer::on_seekSlider_sliderPressed()
{
//    fprintf(stderr, "seekSlider pressed\n");
}

void MediaPlayer::on_seekSlider_sliderMoved(int position)
{
//    fprintf(stderr, "seekSlider moved\n");
}

void MediaPlayer::on_seekSlider_sliderReleased()
{
//    fprintf(stderr, "seekSlider released. NOw slider position is %d\n", seekSlider->pos().x());
}

void MediaPlayer::on_seekSlider_rangeChanged(int min, int max)
{
//    fprintf(stderr, "seekSlider moved. min %d. max %d\n", min, max);
}

void MediaPlayer::on_seekSlider_actionTriggered(int action)
{
    fprintf(stderr, "seekSlider actionTriggered. action: %d\n", action);
}

void MediaPlayer::on_volumeLeft_valueChanged(int value)
{
    (void)value;
    updateAudioEngineVolumeRatios();
}

void MediaPlayer::on_volumeRight_valueChanged(int value)
{
    (void)value;
    updateAudioEngineVolumeRatios();
}
