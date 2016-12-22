#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T03:05:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SharedPadClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ErrorHandler.cpp \
    GenericRequestMessage.cpp \
    GenericResponseMessage.cpp \
    JsonResponseMessageParser.cpp \
    JsonRequestMessageGenerator.cpp \
    Client.cpp

HEADERS  += mainwindow.h \
    rapidjson/error/en.h \
    rapidjson/error/error.h \
    rapidjson/internal/biginteger.h \
    rapidjson/internal/diyfp.h \
    rapidjson/internal/dtoa.h \
    rapidjson/internal/ieee754.h \
    rapidjson/internal/itoa.h \
    rapidjson/internal/meta.h \
    rapidjson/internal/pow10.h \
    rapidjson/internal/stack.h \
    rapidjson/internal/strfunc.h \
    rapidjson/internal/strtod.h \
    rapidjson/internal/swap.h \
    rapidjson/msinttypes/inttypes.h \
    rapidjson/msinttypes/stdint.h \
    rapidjson/allocators.h \
    rapidjson/document.h \
    rapidjson/encodedstream.h \
    rapidjson/encodings.h \
    rapidjson/filereadstream.h \
    rapidjson/filewritestream.h \
    rapidjson/memorybuffer.h \
    rapidjson/memorystream.h \
    rapidjson/pointer.h \
    rapidjson/prettywriter.h \
    rapidjson/rapidjson.h \
    rapidjson/reader.h \
    rapidjson/stringbuffer.h \
    rapidjson/writer.h \
    NamespaceSPP.h \
    ErrorHandler.h \
    GenericRequestMessage.h \
    GenericResponseMessage.h \
    JsonResponseMessageParser.h \
    JsonRequestMessageGenerator.h \
    Client.h

FORMS    += mainwindow.ui
