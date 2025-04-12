#pragma once

#include <QMainWindow>
#include <vlcpp/vlc.hpp>
#include <functional>
#include <vector>
#include "ui_vlcplayer.h"
#include "debug_window.h"

class VLCPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit VLCPlayer(QWidget *parent = nullptr);

private slots:
    void openFile();
    void playPause();
    void stop();
    void setPosition(int position);
    float getPosition();
    void setVolume(int volume);
    float getVolume();
    void on_btnDebug_clicked();
    void setupDebugFunctions();

private:
    std::unique_ptr<Ui::VLCPlayer> ui;
    std::unique_ptr<DebugWindow> debugWindow;
    std::vector<std::pair<QString, std::function<QString()>>> debugFunctions;

    // VLC-specific members
    std::unique_ptr<VLC::Instance> vlcInstance;
    std::unique_ptr<VLC::MediaPlayer> vlcMediaPlayer;
    std::unique_ptr<VLC::Media> vlcMedia;
    QString currentFile;
};

