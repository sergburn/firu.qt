TEMPLATE = app
TARGET = firuqt

# CONFIG += sqlite
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
    src/firudebug.h \
    src/model/sqlgenerator.h \
    src/model/reversetest.h \
    src/trainer.h \
    src/model/exercise.h
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
    src/firudebug.cpp \
    src/model/sqlgenerator.cpp \
    src/model/reversetest.cpp \
    src/trainer.cpp \
    src/model/exercise.cpp \
    src/model/model.cpp
FORMS += src/firumainwindow.ui \
    src/firuqt.ui \
    src/trainerdialog.ui \
    src/entryviewdialog.ui
RESOURCES += firuqt.qrc
sqlite:DEFINES += FIRU_USE_SQLITE
else:QT += sql
symbian { 
    TARGET.UID3 = 0xE92D4440
    TARGET.CAPABILITY = ReadUserData \
        WriteUserData
    HEADERS += firuqt.loc \
        src/AppUi_S60.h
    SOURCES += firuqt.rss \
        firuqt_reg.rss \
        firuqt_reg.rss \
        src/AppUi_S60.cpp
    LIBS += -lcharconv
    INCLUDEPATH += /epoc32/include/middleware \
        ./
    sqlite { 
        DEFINES += SQLITE_OS_UNIX=1 \
            SQLITE_THREADSAFE=0 \
            SQLITE_OMIT_LOAD_EXTENSION \
            SQLITE_OMIT_DEPRECATED
        DEPENDPATH += external/sqlite
        HEADERS += external/sqlite/sqlite3.h \
            src/sqlite_symbian.h
        SOURCES += external/sqlite/sqlite3_part2.cpp \
            src/sqlite_symbian.cpp
    }
}
linux-g++ { 
    CONFIG += qt \
        debug
    sqlite:LIBS += -lsqlite3
}

# SOURCES += /usr/share/qtcreator/gdbmacros/gdbmacros.cpp
win32:CONFIG += qt \
    debug
