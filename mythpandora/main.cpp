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

// C++ headers
#include <unistd.h>

// QT headers
#include <QApplication>

// MythTV headers
#include <mythcontext.h>
#include <mythplugin.h>
#include <mythpluginapi.h>
#include <mythversion.h>
#include <mythmainwindow.h>

// MythPandora headers
#include "mythpandora.h"

using namespace std;

static MythPianoService* gMythPianoService = NULL;
MythPianoService * GetMythPianoService()
{
  if (!gMythPianoService)
    gMythPianoService = new MythPianoService();

  return gMythPianoService;
}

void runPandora(void);
int  RunPandora(void);
void setupKeys(void);


void setupKeys(void)
{
  REG_JUMP("MythPandora", QT_TRANSLATE_NOOP("MythPandora",
					    "Sample plugin"), "", runPandora);

  REG_KEY("MythPandora", "VOLUMEDOWN",  "Volume down", "[,{,F10,Volume Down");
  REG_KEY("MythPandora", "VOLUMEUP",    "Volume up",   "],},F11,Volume Up");
  REG_KEY("MythPandora", "PLAY",        "Play",        "p");
  REG_KEY("MythPandora", "PAUSE",       "Pause",        " ");
  REG_KEY("MythPandora", "NEXTTRACK",   "Move to the next track", ",,<,Q,Home");
}

int mythplugin_init(const char *libversion)
{
  if (!gContext->TestPopupVersion("mythpandora",
				  libversion,
				  MYTH_BINARY_VERSION))
    return -1;
  setupKeys();
  return 0;
}

void runPandora(void)
{
  RunPandora();
}

int RunPandora()
{
  // Setup Piano Service
  MythPianoService *service = GetMythPianoService();

  // try logging in here.  If it works, then don't show the login dialog.
  if (service->Login() != 0) {
    showLoginDialog();
    return 0;
  }

  showStationSelectDialog();
  return 0;
}

int mythplugin_run(void)
{
  return RunPandora();
}

int mythplugin_config(void)
{
  return -1;
}

int showLoginDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPandoraConfig *config = new MythPandoraConfig(mainStack, "pandoraconfig");
  if (config->Create()) {
    mainStack->AddScreen(config);
    return 0;
  } else {
    delete config;
    return -1;
  }
  return 0;
}

int showStationSelectDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPandoraStationSelect *select = new MythPandoraStationSelect(mainStack, "pandorastations");
  if (select->Create()) {
    mainStack->AddScreen(select);
    return 0;
  } else {
    delete select;
    return -1;
  }
  return 0;
}

int showPlayerDialog()
{
  MythScreenStack *mainStack = GetMythMainWindow()->GetMainStack();
  MythPandora *mythpandora = new MythPandora(mainStack, "pandora");
  
  if (mythpandora->Create()){
    mainStack->AddScreen(mythpandora);
  } else {
    delete mythpandora;
    return -1;
  }
  return 0;
}
