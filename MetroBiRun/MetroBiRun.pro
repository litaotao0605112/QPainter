QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CustomWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CustomWidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui
# 仅Release构建生效
CONFIG(release, debug|release) {
    MSVC {
        # 移除默认 /O2 /O1 优化开关
        QMAKE_CXXFLAGS_RELEASE -= /O2 /O1 /Ox
        # 关闭全部优化
        QMAKE_CXXFLAGS_RELEASE += /O1
        # 可选：添加调试符号，方便调试Release程序
        #QMAKE_CXXFLAGS_RELEASE += /Zi
        #QMAKE_LFLAGS_RELEASE += /DEBUG
    }
else {
        # MinGW / GCC / Clang
        QMAKE_CXXFLAGS_RELEASE -= -O2 -O3 -O1 -Os
       QMAKE_CXXFLAGS_RELEASE += -O1  # 无优化
        #QMAKE_CXXFLAGS_RELEASE += -g   # 生成调试符号
    }
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
