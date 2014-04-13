SOURCES += \
    main.cpp
unix|win32: LIBS += -L$$PWD/../../../../usr/local/systemc230/lib-linux/ -lsystemc

INCLUDEPATH += $$PWD/../../../../usr/local/systemc230/include
DEPENDPATH += $$PWD/../../../../usr/local/systemc230/include

win32: PRE_TARGETDEPS += $$PWD/../../../../usr/local/systemc230/lib-linux/systemc.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/systemc230/lib-linux/libsystemc.a
