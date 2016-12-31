cmake_minimum_required(VERSION 3.6)
project(SharePadServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(include)

set(SOURCE_FILES include
        Sources/main.cpp
        Sources/Server.cpp Headers/Server.h
        Sources/ErrorHandler.cpp Headers/ErrorHandler.h
        Sources/ClientInformation.cpp Headers/ClientInformation.h
        Sources/JsonResponseMessageGenerator.cpp Headers/JsonResponseMessageGenerator.h
        Headers/StatusCodesAndDescriptions.h
        Sources/JsonRequestMessageParser.cpp
        Headers/JsonRequestMessageParser.h
        Sources/JsonMessageContainers/LoginResponseMessage.cpp Headers/JsonMessageContainers/LoginResponseMessage.h
        Sources/JsonMessageContainers/GenericResponseMessage.cpp Headers/JsonMessageContainers/GenericResponseMessage.h Sources/User.cpp Headers/User.h)

add_executable(SharePadServer ${SOURCE_FILES})