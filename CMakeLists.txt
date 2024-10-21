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

find_package( LibLZMA REQUIRED )

find_package(nlohmann_json CONFIG REQUIRED)

set(LIBRARIES ${Boost_LIBRARIES} nlohmann_json::nlohmann_json ${LIBLZMA_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

#testing
enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_FILE ${TEST_SOURCES})
    # Получаем имя файла без пути и расширения
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # Создаем отдельное исполняемое тестовое приложение для каждого теста
    add_executable(${TEST_NAME} ${TEST_FILE})

    # Линкуем с библиотекой MyLibrary и Boost
    target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME} ${LIBRARIES})

    # Добавляем тест в CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()