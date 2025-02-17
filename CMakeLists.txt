cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED INSTALLED_VCPKG_ROOT AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Vcpkg not found. Check INSTALLED_VCPKG_ROOT or VCPKG_ROOT variables.")
endif()

if(DEFINED INSTALLED_VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${INSTALLED_VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")
    include(${CMAKE_TOOLCHAIN_FILE})
    set(VCPKG_ROOT ${INSTALLED_VCPKG_ROOT})
    message(STATUS "cmake toolchin file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

option(ENABLE_TESTS "Enable building tests" ON)

project("MinecraftLauncherLib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#headers
include_directories(${PROJECT_SOURCE_DIR}/include)

#sources
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(ninedelixe::mcllib ALIAS ${PROJECT_NAME})

#libraries
if (ENABLE_TESTS)
    find_package(Boost REQUIRED COMPONENTS unit_test_framework)
endif()

find_package(LibLZMA REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(curlcpp CONFIG REQUIRED)

find_package(libzip CONFIG REQUIRED)


set(LIBRARIES ${Boost_LIBRARIES} nlohmann_json::nlohmann_json ${LIBLZMA_LIBRARIES} curlcpp::curlcpp libzip::zip)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

#testing
if (ENABLE_TESTS)
    enable_testing()

    file(GLOB TEST_SOURCES "tests/*.cpp")

    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

        add_executable(${TEST_NAME} ${TEST_FILE})

        target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME} ${LIBRARIES})

        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()