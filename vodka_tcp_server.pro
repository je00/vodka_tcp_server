QT -= gui
QT += quick

CONFIG += c++11 console
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    demoserver.cpp


HEADERS += \
    demoserver.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = volt+.ico

RESOURCES += \
    images.qrc
