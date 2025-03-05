#include "player.h"

Player::Player(QWidget* parrent)
    : QWidget{parrent}
    , play_icon(":/ico/icons/play.ico")
    , pause_icon(":/ico/icons/pause.ico")
    , mute_icon(":/ico/icons/mute.ico")
    , unmute_icon(":/ico/icons/unmute.ico")
    , next_icon(":/ico/icons/next.ico")
    , previous_icon(":/ico/icons/previous.ico")
    , browse_icon(":/ico/icons/browse.ico")
    , repeat_icon(":/ico/icons/repeat.ico")
    , repeatone_icon(":/ico/icons/repeatone.ico")
    , repeatoff_icon(":/ico/icons/repeatoff.ico")
    , shuffle_icon(":/ico/icons/shuffle.ico")
    , shuffleoff_icon(":/ico/icons/shuffleoff.ico")
{
    setFixedSize(600, 300);
    init();
    setup();
    setup_conections();
    chlp();
}

Player::~Player()
{
    if(start)
    {
        delete start;
    }
    if(next)
    {
        delete next;
    }
    if(prev)
    {
        delete prev;
    }
    if(volume_slider)
    {
        delete volume_slider;
    }
    if(audio_slider)
    {
        delete audio_slider;
    }
    if(music_name)
    {
        delete music_name;
    }
    if(duration)
    {
        delete duration;
    }
    if(mute)
    {
        delete mute;
    }
    if(out)
    {
        delete out;
    }
    if(player)
    {
        delete player;
    }
    if(browse_music)
    {
        delete browse_music;
    }
    if(playlist)
    {
        delete playlist;
    }
    if(dialog)
    {
        delete dialog;
    }
    if(list_widget)
    {
        delete list_widget;
    }
}

QStringList Player::getMusicFiles()
{
    QStringList musicFiles;
    QString musicPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    if (!musicPath.isEmpty()) {
        QDir musicDir(musicPath);
        if (musicDir.exists())
        {
            musicFiles = musicDir.entryList(QStringList() << "*.mp3", QDir::Files);
            for (int i = 0; i < musicFiles.size(); ++i)
            {
                musicFiles[i] = musicDir.absoluteFilePath(musicFiles[i]);
                list_widget ->addItem(QFileInfo(musicFiles[i]).fileName().replace(".mp3", ""));
            }
            player -> setSource(musicFiles[0]);
        }
    }
    return musicFiles;
}

void Player::handle_play()
{
    if (player->isPlaying())
    {
        player->pause();
        start->setIcon(play_icon);
    }
    else if(playlist && !(playlist ->isEmpty()))
    {
        player->play();
        start->setIcon(pause_icon);
    }
}

void Player::handle_next()
{
    if(index + 1 < playlist -> size())
    {
        index++;
    }
    else
    {
        index = 0;
    }
}

void Player::handle_prev()
{
    int active = index;
    if(index > 0)
    {
        qDebug() << "Active "<< active <<"Set ACtive Audio Track " << active - 1;
        index--;
    }
    else
    {
        index = playlist -> size() - 1;
    }
}

void Player::handle_mute()
{
    if(out->isMuted())
    {
        out->setMuted(false);
        mute -> setIcon(unmute_icon);
    }
    else
    {
        out->setMuted(true);
        mute -> setIcon(mute_icon);
    }
}

void Player::handle_shuffle()
{
    //0 shuffle off
    //1 shuffle on
    shuffle_signal ^= 1;
    if(shuffle_signal == 1)
    {
        shuffle->setIcon(shuffle_icon);
    }
    else
    {
        shuffle -> setIcon(shuffleoff_icon);
    }
}

void Player::handle_repeat()
{
    // 0 repeat off
    // 1 repeat one
    // 2 repeat
    repeat_signal++;
    if(repeat_signal == 3)
    {
        repeat_signal = 0;
        repeat ->setIcon(repeatoff_icon);
    }
    else if(repeat_signal == 1)
    {
        repeat ->setIcon(repeatone_icon);
    }
    else if(repeat_signal == 2)
    {
        repeat -> setIcon(repeat_icon);
    }
}

void Player::handle_volume_slider(int new_value)
{
    out->setVolume(float(new_value)/100);
}

void Player::handle_audio_slider(int new_value)
{
    if (!flag)
    {
        flag = true;
        player->setPosition(qint64(new_value) * 1000);
        flag = false;
    }

    qint64 seconds = new_value;

    int min = seconds / 60;
    seconds %= 60;
    duration->setText(QString::number(min) + ":" + QString::number(seconds));
}

void Player::handle_audio_position(qint64 new_pos)
{
    if (!flag)
    {
        flag  = true;
        audio_slider->setValue(new_pos / 1000);
        flag = false;
    }
}

void Player::handle_meta_data()
{
    audio_slider->setMinimum(0);

    audio_slider->setMaximum(player->duration() / 1000);

    QString songTitle = player->metaData().value(QMediaMetaData::Title).toString();
    if(!songTitle.isEmpty())
    {
        music_name -> setText(songTitle);
    }
    else
    {
        music_name -> setText("Music");
    }
}

void Player::handle_music_selection()
{
    dialog = new QFileDialog();
    QStringList music_list = dialog -> getOpenFileNames(
        nullptr
        ,"Select Music Files"
        , QDir::homePath() + "/Music"
        , "MP3 Files (*.mp3)"
        );
    if(!music_list.isEmpty())
    {
        if(playlist == nullptr)
        {
            this -> playlist =  new QStringList(music_list);
            player -> setSource(playlist -> at(0));
            index = 0;
        }
        else
        {
            this -> playlist -> append(music_list);
        }
        for(QString str : music_list)
        {
            list_widget ->addItem(QFileInfo(str).fileName().replace(".mp3", ""));
        }
    }
    delete dialog;
    dialog = nullptr;
}

void Player::handle_switching_tracks()
{
    player -> pause();
    player -> setSource(QUrl::fromLocalFile(playlist -> at(index)));
    start ->setIcon(pause_icon);
    player -> play();
}

void Player::handle_switching_list_widget()
{
    if(index != list_widget -> currentRow())
    {
        index = list_widget -> currentRow();
        handle_switching_tracks();
    }
}

void Player::init()
{
    flag = false;
    dialog = nullptr;
    index = -1;
    repeat_signal = 0;
    shuffle_signal = 0;

    start = new QPushButton(this);
    start -> setIcon(play_icon);
    next = new QPushButton(this);
    next ->setIcon(next_icon);
    prev = new QPushButton(this);
    prev -> setIcon(previous_icon);

    shuffle = new QPushButton(this);
    shuffle ->setIcon(shuffleoff_icon);

    repeat = new QPushButton(this);
    repeat ->setIcon(repeatoff_icon);

    volume_slider = new QSlider(this);
    audio_slider = new QSlider(this);

    music_name = new QLabel(this);

    duration = new QLabel("00:00", this);

    mute = new QPushButton(this);
    mute ->setIcon(unmute_icon);

    browse_music = new QPushButton(this);
    browse_music -> setIcon(browse_icon);

    player = new QMediaPlayer(this);
    out = new QAudioOutput(this);

    list_widget = new QListWidget(this);

    player->setAudioOutput(out);
    QStringList tmp = getMusicFiles();
    if(!tmp.isEmpty())
    {
        playlist = new QStringList(tmp);
    }
}

void Player::setup()
{
    music_name->setGeometry(10, 210, 580, 30);
    music_name->setAlignment(Qt::AlignCenter);
    list_widget ->setGeometry(150, 10, 300, 150);

    volume_slider->setGeometry(490, 255, 100, 40);
    volume_slider->setOrientation(Qt::Horizontal);
    volume_slider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    background: #d3d3d3;"
        "    height: 8px;"
        "    border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #3498db;"
        "    border: 1px solid #2980b9;"
        "    width: 18px;"
        "    height: 18px;"
        "    margin: -5px 0;"
        "    border-radius: 9px;"
        "}"
        "QSlider::add-page:horizontal {"
        "    background: #ecf0f1;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #2ecc71;"
        "}"
        "QSlider::tickmarks:horizontal {"
        "    background: #e74c3c;"
        "    height: 2px;"
        "    width: 4px;"
        "}"
        "QSlider::tickmarks:horizontal:disabled {"
        "    background: #7f8c8d;"
        "}"
        );

    mute->setGeometry(450, 260, 30, 30);
    mute ->setStyleSheet("QPushButton { background: white;}");
    start->setGeometry(230, 260, 30, 30);
    start->setStyleSheet("QPushButton { background: white;}");
    prev->setGeometry(190, 260, 30, 30);
    prev ->setStyleSheet("QPushButton { background: white;}");
    next->setGeometry(270, 260, 30, 30);
    next ->setStyleSheet("QPushButton { background: white;}");
    shuffle -> setGeometry(150, 260, 30, 30);
    shuffle ->setStyleSheet("QPushButton { background: white;}");
    repeat -> setGeometry(310, 260, 30, 30);
    repeat ->setStyleSheet("QPushButton { background: white;}");
    audio_slider->setOrientation(Qt::Horizontal);
    audio_slider->setGeometry(10, 230, 530, 30);
    audio_slider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    background: #d3d3d3;"
        "    height: 8px;"
        "    border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #3498db;"
        "    border: 1px solid #2980b9;"
        "    width: 18px;"
        "    height: 18px;"
        "    margin: -5px 0;"
        "    border-radius: 9px;"
        "}"
        "QSlider::add-page:horizontal {"
        "    background: #ecf0f1;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #2ecc71;"
        "}"
        "QSlider::tickmarks:horizontal {"
        "    background: #e74c3c;"
        "    height: 2px;"
        "    width: 4px;"
        "}"
        "QSlider::tickmarks:horizontal:disabled {"
        "    background: #7f8c8d;"
        "}"
        );
    duration->setGeometry(550, 230, 40, 30);
    browse_music -> setGeometry(10, 10, 30, 30);
    browse_music ->setStyleSheet("QPushButton { background: white;}");
}

void Player::setup_conections()
{
    connect(start,SIGNAL(clicked()), this, SLOT(handle_play()));

    connect(next, SIGNAL(clicked(bool)), this, SLOT(handle_next()));
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(handle_prev()));

    connect(mute, SIGNAL(clicked(bool)), this, SLOT(handle_mute()));
    connect(volume_slider, SIGNAL(valueChanged(int)), this, SLOT(handle_volume_slider(int)));


    connect(player, SIGNAL(metaDataChanged()), this, SLOT(handle_meta_data()));


    connect(audio_slider, SIGNAL(valueChanged(int)), this, SLOT(handle_audio_slider(int))) ;
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(handle_audio_position(qint64)));

    connect(browse_music, SIGNAL(clicked(bool)), this, SLOT(handle_music_selection()));

    connect(next, SIGNAL(clicked(bool)), this, SLOT(handle_switching_tracks()));
    connect(prev, SIGNAL(clicked(bool)), this, SLOT(handle_switching_tracks()));

    connect(list_widget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(handle_switching_list_widget()));

    connect(repeat, SIGNAL(clicked(bool)), this, SLOT(handle_repeat()));
    connect(shuffle, SIGNAL(clicked(bool)), this, SLOT(handle_shuffle()));

    connect(player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        qint64 duration = player -> duration();
        if (position >= duration && duration > 0)
        {
            if(repeat_signal == 0)
            {
                player -> pause();
            }
            if(repeat_signal == 1)
            {
                player -> play();
            }
            if(repeat_signal == 2)
            {
                if(shuffle_signal == 0)
                {
                    handle_next();
                    handle_switching_tracks();
                }
                else
                {
                    int randomNumber = QRandomGenerator::global()->bounded(playlist ->size() - 1);
                    index = randomNumber;
                    handle_switching_tracks();
                }
            }
        }
    });

}



void Player::chlp()
{
    volume_slider->setMinimum(0);
    volume_slider->setMaximum(100);
    volume_slider->setValue(20);
}
