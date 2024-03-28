# CMakeList.txt: проект CMake для MinecraftLauncherLib; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set(vcpkg "C:\\Users\\alkor\\source\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
if(NOT CMAKE_TOOLCHAIN_FILE AND EXISTS "${vcpkg}")
  set(CMAKE_TOOLCHAIN_FILE "${vcpkg}"
      CACHE FILEPATH "Vcpkg toolchain file")
  message(STATUS "vcpkg toolchain found: ${CMAKE_TOOLCHAIN_FILE}")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "mclauncherlib_tests")


# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MinecraftLauncherLib")

# Добавьте источник в исполняемый файл этого проекта.
add_executable(MinecraftLauncherLib "include/MinecraftLauncherLib.h" "include/MinecraftLauncherLib/VersionProfile.h" "include/MinecraftLauncherLib/UserProfile.h" "include/MinecraftLauncherLib/JavaArguments.h" "test/main.cpp" "include/MinecraftLauncherLib/Argument.h" "include/MinecraftLauncherLib/Types/Vector.h" "include/MinecraftLauncherLib/Utils/Macros.h" "include/MinecraftLauncherLib/Utils/PathUtils.h" "include/MinecraftLauncherLib/Utils/StringUtils.h"  "include/MinecraftLauncherLib/Utils/MemoryUtils.h" "include/MinecraftLauncherLib/Arguments.h" "include/MinecraftLauncherLib/UserArguments.h" "include/MinecraftLauncherLib/Libraries.h" "include/MinecraftLauncherLib/SingleLibrary.h" "src/SingleLibrary.cpp" "include/MinecraftLauncherLib/CommandLine.h" "src/CommandLine.cpp")
#add_executable(MinecraftLauncherLib "src/MemoryUtils.cpp" "src/PathUtils.cpp" "src/StringUtils.cpp")

#target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

#set_property(TARGET tgt PROPERTY CXX_STANDARD 11)

  set_property(TARGET MinecraftLauncherLib PROPERTY CXX_STANDARD 17)
  #set_property(TARGET MinecraftLauncherLib PROPERTY CXX_STANDARD 17)
if (CMAKE_VERSION VERSION_GREATER 3.8)
#else
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

if(DEFINED CTL_CACHE_LINE_SIZE)
    message(${CTL_CACHE_LINE_SIZE})
    add_compile_definitions(CTL_CACHE_LINE_SIZE=${CTL_CACHE_LINE_SIZE})
endif()

include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/)

enable_testing ()
set(TESTS StringUtilsTest VersionProfileTest SingleLibraryTest)
set(TESTS_SOURCE_NAME StringUtils VersionProfile SingleLibrary) 

add_subdirectory(${CMAKE_SOURCE_DIR}/test) 

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

#add_test (NAME StringUtilsTest COMMAND StringUtilsTest)
#add_test (NAME VersionProfileTest COMMAND VersionProfileTest)

  #set (TESTS Vector3fTest Vector2fTest) #MovableObjectTest)


foreach (_test ${TESTS})
  #add_executable (${_test} ${CMAKE_SOURCE_DIR}/test/${_test}.cpp)
  #target_link_libraries (${_test} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  add_test (NAME ${_test} COMMAND ${_test})
  #set_property(TARGET ${_test} PROPERTY CXX_STANDARD 17)
endforeach ()

#set_property(TARGET VersionProfileTest PROPERTY CXX_STANDARD 17)


find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories (${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/include
                     ${Boost_INCLUDE_DIRS}
                     )
add_definitions (-DBOOST_TEST_DYN_LINK) 

target_link_libraries (MinecraftLauncherLib
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )




find_package( LibLZMA REQUIRED )
target_include_directories(MinecraftLauncherLib PRIVATE ${LIBLZMA_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBLZMA_LIBRARIES})

find_package(nlohmann_json CONFIG REQUIRED)
#target_link_libraries(MinecraftLauncherLib PRIVATE nlohmann_json::nlohmann_json)