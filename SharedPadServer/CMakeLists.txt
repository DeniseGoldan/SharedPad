cmake_minimum_required(VERSION 3.6)
project(SharePadServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(include)

set(SOURCE_FILES include
        Sources/main.cpp
        Sources/Server.cpp Headers/Server.h
        Sources/ErrorHandler.cpp Headers/ErrorHandler.h
        Sources/ClientInformation.cpp Headers/ClientInformation.h
        Sources/JsonResponseGenerator.cpp Headers/JsonResponseGenerator.h
        Headers/StatusCodesAndDescriptions.h
        Sources/JsonRequestParser.cpp
        Headers/JsonRequestParser.h
        Sources/GenericResponse.cpp Headers/GenericResponse.h Sources/User.cpp Headers/User.h Sources/SpecializedResponse.cpp Headers/SpecializedResponse.h)

add_executable(SharePadServer ${SOURCE_FILES})