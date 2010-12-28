include ( ../../mythconfig.mak )
include ( ../../settings.pro )
include ( ../../programs-libs.pro )

QT += xml sql opengl qt3support network

TEMPLATE = lib
CONFIG += plugin thread
TARGET = mythpandora
target.path = $${LIBDIR}/mythtv/plugins
INSTALLS += target

INCLUDEPATH += ../libpiano/src
INCLUDEPATH += ../libwaitress/src
INCLUDEPATH += ../libezxml/src
INCLUDEPATH += $${PREFIX}/include/mythtv
INCLUDEPATH += $${PREFIX}/include/mythtv/libmyth
INCLUDEPATH += $${PREFIX}/include/mythtv/libmythui
INCLUDEPATH += $${PREFIX}/include/mythtv/libmythdb

LIBS += -lmythavformat
LIBS += -lmythavcodec
LIBS += -lmythavcore
LIBS += -lmythavutil

LIBS += -lmad -lfaad

# Input
HEADERS += config.h mythpandora.h player.h
SOURCES += main.cpp player.c mythpandora.cpp

SOURCES += ../libezxml/src/ezxml.c
HEADERS += ../libezxml/src/ezxml.h

SOURCES += ../libwaitress/src/waitress.c
HEADERS += ../libwaitress/src/waitress.h

SOURCES  += ../libpiano/src/crypt.c ../libpiano/src/piano.c ../libpiano/src/xml.c
HEADERS  += ../libpiano/src/config.h ../libpiano/src/crypt_key_output.h ../libpiano/src/xml.h ../libpiano/src/crypt.h ../libpiano/src/piano.h ../libpiano/src/crypt_key_input.h ../libpiano/src/piano_private.h

include ( ../../libs-targetfix.pro )
