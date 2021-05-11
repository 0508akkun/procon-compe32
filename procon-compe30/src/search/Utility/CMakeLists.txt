cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++14 -lcpprest -lboost_system -lssl -lcrypto -pthread")
add_library(Utility STATIC
    Array.cpp
    Color.cpp
)
