include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic  -std=c++20  -Wall -Werror -Wno-deprecated -Wno-unused-function ")