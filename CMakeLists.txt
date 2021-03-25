cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 11)

add_executable(calculator main.cpp Calculator.cpp Calculator.h Token.cpp Token.h Analyzer.cpp Analyzer.h Expression.cpp Expression.h ExpressionFactory.cpp ExpressionFactory.h)