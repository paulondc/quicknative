RESOURCES += $$PWD/source/quicknative.qrc

android {
    QT += androidextras

    HEADERS += $$PWD/source/cpp/nativefiledialogext.h \
               $$PWD/source/cpp/nativefiledialogandroid.h

    SOURCES += $$PWD/source/cpp/nativefiledialogext.cpp \
               $$PWD/source/cpp/nativefiledialogandroid.cpp
}
