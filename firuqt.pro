TEMPLATE = app
TARGET = firuqt
QT += core \
    gui
HEADERS += src/AppUi.h \
    src/firumainwindow.h \
    src/firuqt.h \
    src/trainerdialog.h \
    src/model/database.h \
    src/entryviewdialog.h \
    src/model/query.h \
    src/model/word.h \
    src/model/translation.h \
    src/model/translationquery.h \
    src/model/model.h \
    src/firuapp.h \
    src/model/wordquery.h \
    src/model/dictionary.h \
    src/model/itembase.h \
    src/model/itemextensionbase.h \
    src/firudebug.h \
    src/model/sqlgenerator.h
SOURCES += src/AppUi.cpp \
    src/firumainwindow.cpp \
    src/main.cpp \
    src/firuqt.cpp \
    src/trainerdialog.cpp \
    src/model/database.cpp \
    src/entryviewdialog.cpp \
    src/model/query.cpp \
    src/model/word.cpp \
    src/model/translation.cpp \
    src/model/translationquery.cpp \
    src/firuapp.cpp \
    src/model/wordquery.cpp \
    src/model/dictionary.cpp \
    src/model/itembase.cpp \
    src/model/itemextensionbase.cpp \
    src/firudebug.cpp \
    src/model/sqlgenerator.cpp
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
