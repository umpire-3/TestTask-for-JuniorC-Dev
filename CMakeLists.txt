cmake_minimum_required(VERSION 3.6)
project(JunTestTask)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES MiniParser.cpp MiniParser.h interval.h XmlWriter.cpp XmlWriter.h XmlException.cpp XmlException.h)
add_executable(JunTestTask ${SOURCE_FILES} main.cpp)
target_link_libraries(JunTestTask pthread)

include_directories(GoogleMock/include)
link_directories(GoogleMock/lib)

add_executable(Tests ${SOURCE_FILES} Tests/tests_main.cpp)
target_link_libraries(Tests libgtest.a pthread)
