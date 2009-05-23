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
    src/entryviewdialog.h \
    src/query.h
SOURCES += src/AppUi.cpp \
    src/firumainwindow.cpp \
    src/main.cpp \
    src/firuqt.cpp \
    src/trainerdialog.cpp \
    src/data.cpp \
    src/dbschema.cpp \
    src/entryviewdialog.cpp \
    src/query.cpp
FORMS += src/firumainwindow.ui \
    src/firuqt.ui \
    src/trainerdialog.ui \
    src/entryviewdialog.ui
RESOURCES += 
DEFINES += SQLITE_THREADSAFE=0 \
    SQLITE_OMIT_LOAD_EXTENSION \
    SQLITE_OMIT_DEPRECATED
symbian { 
    TARGET.UID3 = 0xE92D4440
    HEADERS += firuqt.loc \
        src/AppUi_S60.h \
        external/sqlite/sqlite3.h
    SOURCES += firuqt.rss \
        firuqt_reg.rss \
        firuqt_reg.rss \
        src/AppUi_S60.cpp \
        external/sqlite/sqlite3.c
    DEFINES += SQLITE_OS_UNIX=1
    DEPENDPATH += external/sqlite
}
unix { 
    CONFIG += qt \
        debug
    LIBS += -lsqlite3
    SOURCES += /usr/share/qtcreator/gdbmacros/gdbmacros.cpp
}
win32 { 
    CONFIG += qt \
        debug
    DEPENDPATH += external/sqlite
    INCLUDEPATH += external/sqlite
    SOURCES += external/sqlite/sqlite3.c
}
