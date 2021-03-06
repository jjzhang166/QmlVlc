#ifndef QMLVLCPLAYERPROXY_H
#define QMLVLCPLAYERPROXY_H

#include <QObject>

#include "QmlVlcVideoOutput.h"
#include "QmlVlcAudio.h"
#include "QmlVlcInput.h"
#include "QmlVlcPlaylist.h"
#include "QmlVlcSubtitle.h"
#include "QmlVlcVideo.h"
#include "QmlVlcMediaDesc.h"

class QmlVlcPlayerProxy
    : public QObject,
      public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

protected:
    virtual void classBegin();
    virtual void componentComplete();

private:
    static void OnLibVlcEvent_proxy( const libvlc_event_t* e, void *param );
    //libvlc events arrives from separate thread
    void vlcEvents( bool Attach );

public:
    explicit QmlVlcPlayerProxy( vlc::player* player, QObject* parent = 0 );
    ~QmlVlcPlayerProxy();

    Q_PROPERTY( QString vlcVersion READ get_vlcVersion )

    Q_PROPERTY( QString mrl READ get_mrl WRITE set_mrl )

    Q_PROPERTY( bool playing READ get_playing NOTIFY playingChanged )
    Q_PROPERTY( double length READ get_length NOTIFY mediaPlayerMediaChanged)
    Q_PROPERTY( double position READ get_position WRITE set_position NOTIFY mediaPlayerPositionChanged )
    Q_PROPERTY( double time READ get_time WRITE set_time NOTIFY mediaPlayerTimeChanged )
    Q_PROPERTY( unsigned volume READ get_volume WRITE set_volume )
    Q_PROPERTY( bool fullscreen READ get_fullscreen WRITE set_fullscreen )

    enum State {
        NothingSpecial = ::libvlc_NothingSpecial,
        Opening = ::libvlc_Opening,
        Buffering = ::libvlc_Buffering,
        Playing = ::libvlc_Playing,
        Paused = ::libvlc_Paused,
        Stopped = ::libvlc_Stopped,
        Ended = ::libvlc_Ended,
        Error = ::libvlc_Error,
    };
    Q_ENUMS( State )
    Q_PROPERTY( State state READ get_state NOTIFY stateChanged )

    Q_PROPERTY( QmlVlcAudio* audio READ get_audio CONSTANT )
    Q_PROPERTY( QmlVlcInput* input READ get_input CONSTANT )
    Q_PROPERTY( QmlVlcPlaylist* playlist READ get_playlist CONSTANT )
    Q_PROPERTY( QmlVlcSubtitle* subtitle READ get_subtitle CONSTANT )
    Q_PROPERTY( QmlVlcVideo* video READ get_video CONSTANT )
    Q_PROPERTY( QmlVlcMediaDesc* mediaDesc READ get_mediaDesc CONSTANT )

Q_SIGNALS:
    void playingChanged();
    void stateChanged();

public:
    void OnLibVlcEvent( const libvlc_event_t* e );

    //QML Api
    QString get_vlcVersion();

    Q_INVOKABLE void play( const QString& mrl );
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void mute();
    Q_INVOKABLE void unMute();
    Q_INVOKABLE void toggleMute();

    QString get_mrl();
    void set_mrl( const QString& mrl );

    bool get_playing();

    double get_length();

    double get_position();
    void set_position( double );

    double get_time();
    void set_time( double );

    unsigned int get_volume();
    void set_volume( unsigned int );

    bool get_fullscreen();
    void set_fullscreen( bool fs );

    State get_state();

    Q_INVOKABLE void toggleFullscreen();

Q_SIGNALS:
    /* async events from libvlc */
    void mediaPlayerMediaChanged();
    void mediaPlayerNothingSpecial();
    void mediaPlayerOpening();
    void mediaPlayerBuffering();
    void mediaPlayerPlaying();
    void mediaPlayerPaused();
    void mediaPlayerForward();
    void mediaPlayerBackward();
    void mediaPlayerEncounteredError();
    void mediaPlayerEndReached();
    void mediaPlayerStopped();

    void mediaPlayerTimeChanged();
    void mediaPlayerPositionChanged();
    void mediaPlayerSeekableChanged();
    void mediaPlayerPausableChanged();

public:
    QmlVlcAudio*     get_audio()     { return &m_audio; }
    QmlVlcInput*     get_input()     { return &m_input; }
    QmlVlcPlaylist*  get_playlist()  { return &m_playlist; }
    QmlVlcSubtitle*  get_subtitle()  { return &m_subtitle; }
    QmlVlcVideo*     get_video()     { return &m_video; }
    QmlVlcMediaDesc* get_mediaDesc() { return &m_mediaDesc; }

private:
    QmlVlcAudio     m_audio;
    QmlVlcInput     m_input;
    QmlVlcPlaylist  m_playlist;
    QmlVlcSubtitle  m_subtitle;
    QmlVlcVideo     m_video;
    QmlVlcMediaDesc m_mediaDesc;

protected:
    vlc::player& player() { assert( m_player ); return *m_player; }

private:
    vlc::player *const m_player;
};
#endif // QMLVLCPLAYERPROXY_H
