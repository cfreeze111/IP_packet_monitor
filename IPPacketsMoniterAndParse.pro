QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 -lwpcap -lpacket -lws2_32 -libsock32

LIBS += -lws2_32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

NpcapHome += $$PWD/third_party/npcap-sdk-1.13

INCLUDEPATH +=  \
    $${NpcapHome}/Include \
    $${NpcapHome}/Include/pcap \

LIBS += \
    $${NpcapHome}/Lib/x64/Packet.lib \
    $${NpcapHome}/Lib/x64/wpcap.lib \
    $${NpcapHome}/Lib/Packet.lib \
    $${NpcapHome}/Lib/wpcap.lib

SOURCES += \
    src/barchartsbuilder.cpp \
    src/graphthread.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/moniterthread.cpp \
    src/mynpacp.cpp \
    src/selectnetworkcard.cpp \
    src/uiparsetable.cpp \
    src/uiselectnetworkcard.cpp

HEADERS += \
    headers/barchartsbuilder.h \
    headers/barchartsbuilder.h \
    headers/graphthread.h \
    headers/mainwindow.h \
    headers/moniterthread.h \
    headers/mynpacp.h \
    headers/selectnetworkcard.h \
    headers/uiparsetable.h \
    headers/uiselectnetworkcard.h

FORMS += \
    mainwindow.ui \
    uiparsetable.ui \
    uiselectnetworkcard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES +=

RC_ICONS = ./resources/design.ico
