TEMPLATE = app
TARGET = firuqt
QT += core \
    gui
HEADERS += src/AppUi.h \
    src/firumainwindow.h \
    src/firuqt.h \
    src/trainerdialog.h \
    src/data.h \
    src/dbschema.h \
    external/sqlite/sqlite3.h
SOURCES += src/AppUi.cpp \
    src/firumainwindow.cpp \
    src/main.cpp \
    src/firuqt.cpp \
    src/trainerdialog.cpp \
    src/data.cpp \
    src/dbschema.cpp \
    external/sqlite/sqlite3.c
FORMS += src/firumainwindow.ui \
    src/firuqt.ui \
    src/trainerdialog.ui
RESOURCES += 
INCLUDEPATH += external/sqlite
DEFINES += SQLITE_THREADSAFE=0 \
    SQLITE_OMIT_LOAD_EXTENSION
symbian: {
    TARGET.UID3 = 0xE92D4440
    HEADERS += firuqt.loc \
        src/AppUi_S60.h
    SOURCES += firuqt.rss \
        firuqt_reg.rss \
        src/firuqt_reg.rss \
        src/AppUi_S60.cpp
    DEFINES += SQLITE_OS_UNIX=1 \
        SQLITE_THREADSAFE=0 \
        SQLITE_OMIT_LOAD_EXTENSION
}
