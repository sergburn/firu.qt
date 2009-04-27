TEMPLATE = app
TARGET = firuqt
QT += core \
    gui
HEADERS += src/AppUi.h \
    src/firumainwindow.h \    
    src/firuqt.h \    
    src/trainerdialog.h \    
    src/data.h
SOURCES += src/AppUi.cpp \    
		src/firumainwindow.cpp \
		src/main.cpp \
		src/firuqt.cpp \
		src/trainerdialog.cpp \
		src/data.cpp
FORMS += src/firumainwindow.ui \
    src/firuqt.ui \
    src/trainerdialog.ui
RESOURCES += 
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
}
