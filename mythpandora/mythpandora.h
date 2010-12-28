/*
Copyright (c) 2010
	Doug Turner < dougt@dougt.org >

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef MYTHPANDORA_H
#define MYTHPANDORA_H

// MythTV headers
#include <QTimer>
#include <QHttp>
#include <QTemporaryFile>


#include "mythscreentype.h"
#include "mythuibutton.h"
#include "mythuibuttonlist.h"
#include "mythuiimage.h"
#include "mythuitextedit.h"
#include "audiooutput.h"
#include <pthread.h>

extern "C" {
#include <piano.h>
#include <waitress.h>
#include "player.h"
}

class MythPianoService;
MythPianoService * GetMythPianoService();

int showLoginDialog();
int showStationSelectDialog();
int showPlayerDialog();

class MythPianoServiceListener
{
 public:
  virtual void RecvMessage(const char* message) = 0;
};

class MythPianoService : public QObject
{
 Q_OBJECT

 public:
  MythPianoService();
  ~MythPianoService();

  int Login();
  void PauseToggle();
  void GetPlaylist();

  void StartPlayback();
  void StopPlayback();
  void NextSong();

  void VolumeUp();
  void VolumeDown();
  int  Volume();
  void ToggleMute();

  void WriteAudio(char* samples, size_t bytes);
  void BroadcastMessage(const char *format, ...);

  void SetMessageListener(MythPianoServiceListener* listener);
  void RemoveMessageListener(MythPianoServiceListener* listener);

  int PianoCall (PianoRequestType_t type,
		 void *data,
		 PianoReturn_t *pRet,
		 WaitressReturn_t *wRet);
  WaitressReturn_t PianoHttpRequest(WaitressHandle_t *waith,
				    PianoRequest_t *req);

  PianoHandle_t      m_Piano;
  WaitressHandle_t   m_Waith;
  struct audioPlayer m_Player;
  pthread_t          m_PlayerThread;
  AudioOutput*       m_AudioOutput;
  PianoSong_t*       m_Playlist;
  PianoStation_t*    m_Station;

  PianoStation_t*    m_CurrentStation;
  PianoSong_t*       m_CurrentSong;

  MythPianoServiceListener* m_Listener;

  QTimer*            m_Timer;

  private slots:
    void heartbeat(void);
};

/** \class MythPandora
 */
class MythPandora : public MythScreenType, public MythPianoServiceListener
{
  Q_OBJECT
  public:
    MythPandora(MythScreenStack *parent, QString name);
    virtual ~MythPandora();

    bool Create(void);
    bool keyPressEvent(QKeyEvent *);

    void RecvMessage(const char* message);

  private:
    MythUIText     *m_titleText;
    MythUIText     *m_songText;
    MythUIText     *m_artistText;
    MythUIText     *m_albumText;
    MythUIText     *m_playTimeText;
    MythUIText     *m_stationText;
    MythUIButton   *m_logoutBtn;
    MythUIButton   *m_stationsBtn;
    MythUIText     *m_outText;
    MythUIImage    *m_coverartImage;
      
    QHttp          *m_coverArtFetcher;
    QTemporaryFile *m_coverArtTempFile;
    QTimer         *m_Timer;

  private slots:
    void heartbeat(void);
    void coverArtFetched(void);
    void logoutCallback();
    void selectStationCallback();
};


class MythPandoraConfig : public MythScreenType, public MythPianoServiceListener
{
  Q_OBJECT
  public:
    MythPandoraConfig(MythScreenStack *parent, QString name);
    ~MythPandoraConfig();
  
    bool Create(void);
    bool keyPressEvent(QKeyEvent *);
    void RecvMessage(const char* message) {
      if (m_outText)
	m_outText->SetText(QString(message));
    }

  private:
    MythUITextEdit   *m_nameEdit;
    MythUITextEdit   *m_passwordEdit;
    MythUIText       *m_outText;
    MythUIButton     *m_loginBtn;
    
  private slots:
    void loginCallback();
};


class MythPandoraStationSelect : public MythScreenType
{
  Q_OBJECT
  public:
    MythPandoraStationSelect(MythScreenStack *parent, QString name);
    ~MythPandoraStationSelect();
  
    bool Create(void);
    bool keyPressEvent(QKeyEvent *);

  private:
    MythUIButtonList *m_stations;    

   private slots:
    void stationSelectedCallback(MythUIButtonListItem *item);
};

#endif /* MYTHPANDORA_H */
