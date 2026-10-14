cmake_minimum_required(VERSION 3.25)
project(threepp_sinus1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # programmet havner ved
#siden av threepps DLL-er
include(FetchContent)
if(MINGW)
set(BUILD_SHARED_LIBS ON) # threepp som DLL, så et nytt bygg lenker på sekunder
endif()
FetchContent_Declare(
threepp
GIT_REPOSITORY https://github.com/markaren/threepp.git
GIT_TAG 2026-09-28
GIT_SHALLOW TRUE
SYSTEM
)
FetchContent_MakeAvailable(threepp)
add_executable(threepp_sinus main.cpp)
target_link_libraries(threepp_sinus PRIVATE threepp::threepp)
if(MSVC)
target_compile_options(threepp_sinus PRIVATE /W4)
else()
target_compile_options(threepp_sinus PRIVATE -Wall -Wextra)
endif()