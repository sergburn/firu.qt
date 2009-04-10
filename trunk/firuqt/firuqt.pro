TEMPLATE = app
TARGET = firuqt
QT += core \
    gui
HEADERS += AppUi.h \
    firumainwindow.h \
    firuqt.h \
    trainerdialog.h
SOURCES += AppUi.cpp \
    firumainwindow.cpp \
    main.cpp \
    firuqt.cpp \
    trainerdialog.cpp
FORMS += firumainwindow.ui \
    firuqt.ui \
    trainerdialog.ui
RESOURCES += 
symbian: { 
    TARGET.UID3 = 0xE92D4440
    HEADERS += AppUi_S60.h
    SOURCES += firuqt_reg.rss \
        AppUi_S60.cpp
}
