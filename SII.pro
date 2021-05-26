QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addanimal.cpp \
    addarea.cpp \
    addrelation.cpp \
    animaltable.cpp \
    areatable.cpp \
    databasehelper.cpp \
    editanimaldialog.cpp \
    editdeletedialog.cpp \
    editmonthdialog.cpp \
    knowlegeeditorwindow.cpp \
    main.cpp \
    predictwindow.cpp \
    relationstable.cpp \
    resultwindow.cpp \
    startwindow.cpp

HEADERS += \
    addanimal.h \
    addarea.h \
    addrelation.h \
    animaltable.h \
    areatable.h \
    customvalidator.h \
    databasehelper.h \
    editanimaldialog.h \
    editdeletedialog.h \
    editmonthdialog.h \
    knowlegeeditorwindow.h \
    predictwindow.h \
    relationstable.h \
    resultwindow.h \
    startwindow.h

FORMS += \
    addanimal.ui \
    addarea.ui \
    addrelation.ui \
    animaltable.ui \
    areatable.ui \
    editanimaldialog.ui \
    editdeletedialog.ui \
    editmonthdialog.ui \
    knowlegeeditorwindow.ui \
    predictwindow.ui \
    relationstable.ui \
    resultwindow.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
