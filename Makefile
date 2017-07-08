# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = construction.cpp \
    global.cpp \
    wiz.cpp \
    tirs.cpp \
    WizardGroup.cpp

champion-dists = prologin.h \
    global.h \
    WizardGroup.h

champion-cxxflags = -ggdb3 -Wall -std=c++11 -O2

# Evite de toucher a ce qui suit
champion-dists += prologin.h
include ../includes/rules.mk
