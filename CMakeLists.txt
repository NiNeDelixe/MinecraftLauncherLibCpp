cmake_minimum_required(VERSION 3.5)

# Интеграция с vcpkg
# Проверяем, если ли переменная VCPKG_ROOT или переменная окружения
if(NOT DEFINED INSTALLED_VCPKG_ROOT AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "vcpkg не найден. Убедитесь, что vcpkg установлен и VCPKG_ROOT задан.")
endif()

# Если переменная окружения VCPKG_ROOT задана, указываем путь к toolchain-файлу vcpkg
if(DEFINED INSTALLED_VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${INSTALLED_VCPKG_ROOT}\\scripts\\buildsystems\\vcpkg.cmake")
    include(${CMAKE_TOOLCHAIN_FILE})
    set(VCPKG_ROOT ${INSTALLED_VCPKG_ROOT})
    message(STATUS "cmake toolchin file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project("MinecraftLauncherLib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#headers
include_directories(${PROJECT_SOURCE_DIR}/include)

#sources
#file(GLOB_RECURSE SOURCES "src/**/*.cpp")
file(GLOB SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

#libraries
find_package(Boost REQUIRED COMPONENTS unit_test_framework locale)

find_package(LibLZMA REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(curlcpp CONFIG REQUIRED)

find_package(libzip CONFIG REQUIRED)

# Найдите Java и JNI
#find_package(Java 1.8 REQUIRED)
#find_package(JNI REQUIRED)

# Убедитесь, что JNI найден
#if (JNI_FOUND)
#    message(STATUS "JNI found: ${JNI_INCLUDE_DIRS}")
#    message(STATUS "JNI libraries: ${JNI_LIBRARIES}")
#    include_directories(${JNI_INCLUDE_DIRS})
#else()
#    message(FATAL_ERROR "JNI not found!")
#endif()

set(LIBRARIES ${Boost_LIBRARIES} nlohmann_json::nlohmann_json ${LIBLZMA_LIBRARIES} curlcpp::curlcpp libzip::zip)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

#testing
enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME} ${LIBRARIES})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()