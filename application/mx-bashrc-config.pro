#-------------------------------------------------
#
# Project created by QtCreator 2019-04-27T11:02:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mx-bashrc-config
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        window.cpp \
    buffer.cpp \
    searcher.cpp \
    tab.cpp \
    prompttab.cpp \
    aliastab.cpp \
    aliasstream.cpp

HEADERS  += window.h \
    buffer.h \
    searcher.h \
    tab.h \
    global.h \
    prompttab.h \
    aliastab.h \
    aliasstream.h

FORMS    += \
    aliastab.ui \
    window_fix.ui \
    prompttab_fix.ui

TRANSLATIONS += translations/mx-bashrc-config_am.ts \
                translations/mx-bashrc-config_ar.ts \
                translations/mx-bashrc-config_bg.ts \
                translations/mx-bashrc-config_ca.ts \
                translations/mx-bashrc-config_cs.ts \
                translations/mx-bashrc-config_da.ts \
                translations/mx-bashrc-config_de.ts \
                translations/mx-bashrc-config_el.ts \
                translations/mx-bashrc-config_es.ts \
                translations/mx-bashrc-config_et.ts \
                translations/mx-bashrc-config_eu.ts \
                translations/mx-bashrc-config_fa.ts \
                translations/mx-bashrc-config_fi.ts \
                translations/mx-bashrc-config_fr.ts \
                translations/mx-bashrc-config_he_IL.ts \
                translations/mx-bashrc-config_hi.ts \
                translations/mx-bashrc-config_hr.ts \
                translations/mx-bashrc-config_hu.ts \
                translations/mx-bashrc-config_id.ts \
                translations/mx-bashrc-config_is.ts \
                translations/mx-bashrc-config_it.ts \
                translations/mx-bashrc-config_ja.ts \
                translations/mx-bashrc-config_ja_JP.ts \
                translations/mx-bashrc-config_kk.ts \
                translations/mx-bashrc-config_ko.ts \
                translations/mx-bashrc-config_lt.ts \
                translations/mx-bashrc-config_mk.ts \
                translations/mx-bashrc-config_mr.ts \
                translations/mx-bashrc-config_nb.ts \
                translations/mx-bashrc-config_nl.ts \
                translations/mx-bashrc-config_pl.ts \
                translations/mx-bashrc-config_pt.ts \
                translations/mx-bashrc-config_pt_BR.ts \
                translations/mx-bashrc-config_ro.ts \
                translations/mx-bashrc-config_ru.ts \
                translations/mx-bashrc-config_sk.ts \
                translations/mx-bashrc-config_sl.ts \
                translations/mx-bashrc-config_sq.ts \
                translations/mx-bashrc-config_sr.ts \
                translations/mx-bashrc-config_sv.ts \
                translations/mx-bashrc-config_tr.ts \
                translations/mx-bashrc-config_uk.ts \
                translations/mx-bashrc-config_zh_CN.ts \
                translations/mx-bashrc-config_zh_TW.ts


RESOURCES += \
    resource.qrc
