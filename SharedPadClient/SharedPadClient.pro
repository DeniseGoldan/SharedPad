#-------------------------------------------------
#
# Project created by Denise
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SharedPadClient
TEMPLATE = app


SOURCES += main.cpp\
    ErrorHandler.cpp \
    GenericRequestMessage.cpp \
    GenericResponseMessage.cpp \
    JsonResponseMessageParser.cpp \
    JsonRequestMessageGenerator.cpp \
    Client.cpp \
    spdlog/fmt/bundled/format.cc \
    spdlog/fmt/bundled/ostream.cc \
    MainWindow.cpp \
    NotepadWindow.cpp

HEADERS  += \
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
    Client.h \
    rapidjson/internal/regex.h \
    rapidjson/fwd.h \
    rapidjson/istreamwrapper.h \
    rapidjson/ostreamwrapper.h \
    rapidjson/schema.h \
    rapidjson/stream.h \
    spdlog/details/async_log_helper.h \
    spdlog/details/async_logger_impl.h \
    spdlog/details/file_helper.h \
    spdlog/details/log_msg.h \
    spdlog/details/logger_impl.h \
    spdlog/details/mpmc_bounded_q.h \
    spdlog/details/null_mutex.h \
    spdlog/details/os.h \
    spdlog/details/pattern_formatter_impl.h \
    spdlog/details/registry.h \
    spdlog/details/spdlog_impl.h \
    spdlog/fmt/bundled/format.h \
    spdlog/fmt/bundled/ostream.h \
    spdlog/fmt/bundled/printf.h \
    spdlog/fmt/fmt.h \
    spdlog/fmt/ostr.h \
    spdlog/sinks/android_sink.h \
    spdlog/sinks/ansicolor_sink.h \
    spdlog/sinks/base_sink.h \
    spdlog/sinks/dist_sink.h \
    spdlog/sinks/file_sinks.h \
    spdlog/sinks/msvc_sink.h \
    spdlog/sinks/null_sink.h \
    spdlog/sinks/ostream_sink.h \
    spdlog/sinks/sink.h \
    spdlog/sinks/stdout_sinks.h \
    spdlog/sinks/syslog_sink.h \
    spdlog/sinks/wincolor_sink.h \
    spdlog/async_logger.h \
    spdlog/common.h \
    spdlog/formatter.h \
    spdlog/logger.h \
    spdlog/spdlog.h \
    spdlog/tweakme.h \
    MainWindow.h \
    NotepadWindow.h

FORMS    += \
    NotepadWindow.ui \
    MainWindow.ui