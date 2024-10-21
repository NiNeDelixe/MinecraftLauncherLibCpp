#include_directories(../thirdparty)
#include(CheckCXXCompilerFlag)


include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories (${CMAKE_SOURCE_DIR}/src/) 
include_directories(${PROJECT_INCLUDE_DIR})

#file(GLOB UNIT_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

#include_directories(${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/Utils/)

#set(TEST_REQUIRED_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
#set(TEST_INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/Utils/StringUtils.h)
#message(${TEST_REQUIRED_SOURCES})
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

#find_package (Boost COMPONENTS unit_test_framework REQUIRED)

add_definitions (-DBOOST_TEST_DYN_LINK)

include_directories ( ${CMAKE_SOURCE_DIR}/scr
                      ${Boost_INCLUDE_DIRS})
                      
find_package(nlohmann_json CONFIG REQUIRED)
#target_link_libraries(VersionProfileTest PRIVATE nlohmann_json::nlohmann_json)

foreach (_test_name ${TESTS_SOURCE_NAME})
    file(GLOB temp ${CMAKE_SOURCE_DIR}/src/${_test_name}.cpp)
    list(APPEND TEST_REQUIRED_SOURCES ${temp})
endforeach ()

message(${TEST_REQUIRED_SOURCES})

foreach (_test ${TESTS})
  add_executable (${_test} ${_test}.cpp ${TEST_REQUIRED_SOURCES})
  target_link_libraries (${_test} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  #add_test (NAME ${_test} COMMAND ${_test})
  set_property(TARGET ${_test} PROPERTY CXX_STANDARD 17)
endforeach ()

#include_directories (${Boost_INCLUDE_DIRS})

#add_definitions (-DBOOST_TEST_DYN_LINK) 

#add_executable (StringUtilsTest StringUtilsTest.cpp)
#add_executable (VersionProfileTest VersionProfileTests.cpp)


#target_link_libraries (StringUtilsTest
#${Boost_FILESYSTEM_LIBRARY}
#${Boost_SYSTEM_LIBRARY}
#${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

