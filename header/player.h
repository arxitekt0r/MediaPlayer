#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QSlider>
#include <QMediaMetaData>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QPixmap>
#include <QListWidget>
#include <QDirIterator>
#include <QStandardPaths>
#include <QRandomGenerator>

class Player : public QWidget {
    Q_OBJECT
public:
    Player(QWidget *parrent = nullptr);
    ~Player();

private slots:
    void handle_play();
    void handle_next();
    void handle_prev();
    void handle_mute();

    void handle_shuffle();
    void handle_repeat();

    void handle_volume_slider(int new_value);
    void handle_audio_slider(int new_value);

    void handle_audio_position(qint64 new_pos);

    void handle_meta_data();

    void handle_music_selection();

    void handle_switching_tracks();
    void handle_switching_list_widget();
private:
    QPushButton *start;
    QPushButton *next;
    QPushButton *prev;
    QPushButton *repeat;
    QPushButton *shuffle;

    QSlider *volume_slider;
    QSlider *audio_slider;

    QLabel *music_name;

    QLabel *duration;

    QPushButton *mute;

    QMediaPlayer *player;
    QAudioOutput *out;

    QPushButton *browse_music;
    QStringList *playlist;
    QFileDialog *dialog;

    QListWidget *list_widget;

    int repeat_signal;
    int shuffle_signal;
    bool flag;
    int index;

    QIcon play_icon;
    QIcon pause_icon;
    QIcon mute_icon;
    QIcon unmute_icon;
    QIcon next_icon;
    QIcon previous_icon;
    QIcon browse_icon;
    QIcon repeat_icon;
    QIcon repeatone_icon;
    QIcon repeatoff_icon;
    QIcon shuffle_icon;
    QIcon shuffleoff_icon;
private:
    void init();
    void setup();
    void setup_conections();
    void chlp();
    QStringList getMusicFiles();
};

#endif // PLAYER_H
