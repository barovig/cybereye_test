TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -L$$PWD/../../../root/lib/ -lopencv_world -lcybereye -lpthread

INCLUDEPATH += $$PWD/../../../root/include
#INCLUDEPATH += $$PWD/../../../root/include/cybereye
DEPENDPATH += $$PWD/../../../root/include
QMAKE_LFLAGS += "-Wl,-rpath=/home/semargl/root/lib"
