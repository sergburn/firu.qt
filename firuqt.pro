TEMPLATE = app
TARGET = firuqt
QT += core \
    gui
CONFIG += qt debug
HEADERS += src/AppUi.h \
    src/firumainwindow.h \
    src/firuqt.h \
    src/trainerdialog.h \
    src/data.h \
    src/dbschema.h
SOURCES += src/AppUi.cpp \
    src/firumainwindow.cpp \
    src/main.cpp \
    src/firuqt.cpp \
    src/trainerdialog.cpp \
    src/data.cpp \
    src/dbschema.cpp
FORMS += src/firumainwindow.ui \
    src/firuqt.ui \
    src/trainerdialog.ui
RESOURCES += 
DEFINES += SQLITE_THREADSAFE=0 \
    SQLITE_OMIT_LOAD_EXTENSION
symbian: {
    TARGET.UID3 = 0xE92D4440
    HEADERS += firuqt.loc \
        src/AppUi_S60.h \
        external/sqlite/sqlite3.h
    SOURCES += firuqt.rss \
        firuqt_reg.rss \
        src/firuqt_reg.rss \
        src/AppUi_S60.cpp \
        external/sqlite/sqlite3.c
    DEFINES += SQLITE_OS_UNIX=1 \
        SQLITE_THREADSAFE=0 \
        SQLITE_OMIT_LOAD_EXTENSION
    DEPENDPATH += external/sqlite
}
unix: {
    LIBS += -lsqlite3
}
win32: {
}
debug: {
    SOURCES += /usr/share/qtcreator/gdbmacros/gdbmacros.cpp
}
