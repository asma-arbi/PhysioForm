QT       += core gui sql
QT += core gui charts
QT += printsupport
QT += charts
QT += core gui network
QT += printsupport
QT+= sql
QT += network
QT += core gui serialport
QT += core gui svg
QT += core gui quick
QT += multimedia
QT += multimedia multimediawidgets
QT += axcontainer
QT += texttospeech
QT += quickwidgets
QT += widgets designer
QT += location


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
    connexion.cpp \
    main.cpp \
    mainwindow.cpp \
    employeC.cpp \
    patientc.cpp \
    chat.cpp \
    notification.cpp\
    employe.cpp\
    patient.cpp \
    traitement.cpp \
    traitementC.cpp \
    paiement.cpp \
    paiementC.cpp \
    reservation.cpp \
    reservationc.cpp
HEADERS += \
    connexion.h \
    mainwindow.h \
    employeC.h \
    patientc.h \
    chat.h \
    notification.h\
    employe.h \
    patient.h \
    traitement.h \
    traitementC.h \
    paiement.h \
    paiementC.h \
    reservation.h \
    reservationc.h

FORMS += \
    home.ui \
    mainwindow.ui \
    chat.ui \
    patient.ui \
    employe.ui\
    patient.ui \
    traitement.ui \
    paiement.ui \
    reservation.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    photos/Photos.qrc \
    qml.qrc
