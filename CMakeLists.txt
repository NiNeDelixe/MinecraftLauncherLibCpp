cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,   RelWithDebInfo>:ProgramDatabase>>")
endif()

option(MCLLib_BUILD_TESTS "" ON)

project("MinecraftLauncherLib" VERSION 1.0.1 DESCRIPTION "mylib description")

set(CMAKE_CXX_STANDARD 17)

#linux ls -1 src/*.cpp
#win dir /b /a-d src\*.cpp 
add_library(${PROJECT_NAME} 
src/Arguments.cpp
src/Command.cpp
src/CommandLine.cpp
src/DownloadWorker.cpp
src/FabricDownloader.cpp
src/ForgeDownloader.cpp
src/Java.cpp
src/Libraries.cpp
src/MemoryUtils.cpp
src/PathUtils.cpp
src/Rules.cpp
src/Setting.cpp
src/SingleLibrary.cpp
src/UserArguments.cpp
src/UserOptions.cpp
src/VanillaDownloader.cpp
src/VersionProfile.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include/MCLLib-api.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/)


include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/)

if(MCLLib_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests) 
endif(MCLLib_BUILD_TESTS)


find_package(LibLZMA REQUIRED)
find_package(liblzma CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost COMPONENTS locale REQUIRED)
find_package(curlcpp CONFIG REQUIRED)
find_package(libzip CONFIG REQUIRED)

set(NBT_BUILD_TESTS OFF)
add_subdirectory(thirdparty/libnbtplusplus)
 
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        LibLZMA::LibLZMA
        liblzma::liblzma
        nlohmann_json::nlohmann_json
        curlcpp::curlcpp
        Boost::locale
        libzip::zip
        nbt++
)