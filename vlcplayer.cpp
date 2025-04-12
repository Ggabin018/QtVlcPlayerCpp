#include "vlcplayer.h"
#include "ui_vlcplayer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "common.h"
#include "file_manager.h"
#include <QLabel>
#include <QProcess>

VLCPlayer::VLCPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(std::make_unique<Ui::VLCPlayer>()),
    debugWindow(std::make_unique<DebugWindow>())
{
    ui->setupUi(this);
    fileManager file_manager{"VlcPlayer"};

    //QMessageBox::information(this, "Test", file_manager.getConfigDir().c_str());
    //QMessageBox::information(this, "Test", file_manager.getDataDir().c_str());

    const char* vlc_args[] = {
        "--no-video-title-show",
        "--input-repeat=-1",
        "--quiet"
    };
    vlcInstance = std::make_unique<VLC::Instance>(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    if (!vlcInstance) {
        QMessageBox::critical(this, "Error", "Could not initialize VLC");
        return;
    }

    vlcMediaPlayer = std::make_unique<VLC::MediaPlayer>(*vlcInstance);
    if (!vlcMediaPlayer) {
        QMessageBox::critical(this, "Error", "Could not create media player");
        return;
    }

    connect(ui->btnOpen, &QPushButton::clicked, this, &VLCPlayer::openFile);
    connect(ui->btnPlayPause, &QPushButton::clicked, this, &VLCPlayer::playPause);
    connect(ui->btnStop, &QPushButton::clicked, this, &VLCPlayer::stop);
    connect(ui->btnFav, &QPushButton::clicked, this, addToFav);

    connect(ui->slider, &QSlider::sliderMoved, this, &VLCPlayer::setPosition);
    connect(ui->audioSlider, &QSlider::sliderMoved, this, &VLCPlayer::setVolume);

    connect(ui->btnDebug, &QPushButton::clicked, this, &VLCPlayer::stop);


    // init class logger
    qInstallMessageHandler(VlcPlayerMessageHandler);
}


void VLCPlayer::openFile()
{
    currentFile = QFileDialog::getOpenFileName(this, "Open Media File",
                                               "", "Media Files (*.mp3 *.mp4 *.avi *.mkv *.wav)");

    if (currentFile.isEmpty()) return;

    if (vlcMedia) {
        qInfo("Release previous media");
        vlcMedia.reset();
    }

    vlcMedia = std::make_unique<VLC::Media>(*vlcInstance, currentFile.toStdString(), VLC::Media::FromPath);

    std::string s_tmp("Create new media: ");
    s_tmp.append(currentFile.toUtf8().constData());
    qInfo("%s", s_tmp.c_str());
    if (!vlcMedia) {
        QMessageBox::critical(this, "Error", "Could not create media");
        return;
    }

    vlcMediaPlayer->setMedia(*vlcMedia);

    // Prepare video output
#ifdef Q_OS_WINDOWS
    vlcMediaPlayer->setDrawable((void*)ui->videoWidget->winId());
#elif defined(Q_OS_LINUX)
    vlcMediaPlayer->setXwindow(ui->videoWidget->winId());
#elif defined(Q_OS_MAC)
    vlcMediaPlayer->setDrawable((void*)ui->videoWidget->winId());
#endif

    vlcMediaPlayer->play();

    ui->btnPlayPause->setText("Pause");
}

void VLCPlayer::playPause()
{
    if (!vlcMediaPlayer) return;

    if (vlcMediaPlayer->isPlaying()) {
        ui->btnPlayPause->setIcon(QIcon(":/icons/play.png"));
        vlcMediaPlayer->pause();
    } else {
        ui->btnPlayPause->setIcon(QIcon(":/icons/pause.png"));
        vlcMediaPlayer->play();
    }
}

void VLCPlayer::stop()
{
    if (!vlcMediaPlayer) return;

    vlcMediaPlayer->stop();
    ui->btnPlayPause->setText("Play");
}

void VLCPlayer::setPosition(int position)
{
    if (!vlcMediaPlayer) return;

    float pos = position / (float)ui->slider->maximum();
    vlcMediaPlayer->setPosition(pos);
}

float VLCPlayer::getPosition()
{
    if (!vlcMediaPlayer) return -1;

    return vlcMediaPlayer->position() * vlcMediaPlayer->length() / 1000;
}

void VLCPlayer::setVolume(int volume)
{
    if (!vlcMediaPlayer) return;

    vlcMediaPlayer->setVolume(volume);
}

float VLCPlayer::getVolume()
{
    if (!vlcMediaPlayer) return -1;

    return vlcMediaPlayer->volume();
}

void VLCPlayer::on_btnDebug_clicked()
{

    if (!debugWindow->isVisible()) {
        setupDebugFunctions();
        debugWindow->setupDebugFunctions(debugFunctions);
        debugWindow->show();
        debugWindow->addDebugMessage("Debug window show");
    } else {
        debugWindow->hide();
        debugWindow->addDebugMessage("Debug window hide");
    }
}

void VLCPlayer::setupDebugFunctions()
{
    // Clear existing debug functions
    debugFunctions.clear();

    // Add various debug functions
    debugFunctions.push_back({"Player Info", [this]() -> QString {
                                  return QString("Player State: %1\nCurrent Media: %2\nDuration: %3 sec\nVolume: %4 vol")
                                  .arg(vlcMediaPlayer->isPlaying() ? "Playing" : "Stopped", currentFile) // Replace with actual media path
                                      .arg(getPosition())
                                      .arg(getVolume()); // Replace with actual duration
                              }});

    debugFunctions.push_back({"System Info", []() -> QString {
                                  QStringList info;
                                  info << "Operating System: " + QSysInfo::prettyProductName();
                                  info << "Kernel: " + QSysInfo::kernelVersion();
                                  info << "Architecture: " + QSysInfo::currentCpuArchitecture();
                                  return info.join("\n");
                              }});
}
