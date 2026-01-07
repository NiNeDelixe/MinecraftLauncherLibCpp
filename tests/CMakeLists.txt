enable_testing()

set(TEST_SOURCES 
CommandLineTest.cpp
SingleLibraryTest.cpp
StringUtilsTest.cpp
UserOptionsTest.cpp
VersionDownloadTest.cpp
)


foreach(test_source ${TEST_SOURCES})

    get_filename_component(test_name ${test_source} NAME_WE)

    

    add_executable(${test_name} ${test_source})

    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    target_link_libraries(${test_name} 
        PRIVATE 
            MinecraftLauncherLib 
            Boost::unit_test_framework
    )

    add_test(NAME ${test_name}Tester COMMAND ${test_name})
    
endforeach(test_source ${TEST_SOURCES})

