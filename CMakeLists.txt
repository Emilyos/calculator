cmake_minimum_required(VERSION 3.17)
project(calculator)

set(CMAKE_CXX_STANDARD 14)

add_executable(calculator main.cpp Calculator.cpp Calculator.h Token.cpp Token.h LexicalAnalyzer.cpp LexicalAnalyzer.h Expression.cpp Expression.h ExpressionFactory.cpp ExpressionFactory.h)