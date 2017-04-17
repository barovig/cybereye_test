TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    server_main.cpp \
    ceye_server.cpp

LIBS += -L$$PWD/../../../root/lib/  -lcybereye  -lopencv_world -lboost_system -lboost_serialization -lpthread

INCLUDEPATH += $$PWD/../../../root/include
#INCLUDEPATH += $$PWD/../../../root/include/cybereye
DEPENDPATH += $$PWD/../../../root/include
QMAKE_LFLAGS += "-Wl,-rpath=/home/semargl/root/lib"
