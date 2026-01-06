enable_testing()

set(TEST_SOURCES 
    SingleLibraryTest.cpp
    StringUtilsTest.cpp
    VersionProfileTest.cpp
)

add_executable(tests ${TEST_SOURCES})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    target_link_libraries(${test_name} 
        PRIVATE 
            MinecraftLauncherLib 
            Boost::unit_test_framework
    )


foreach(test_source ${TEST_SOURCES})

    get_filename_component(test_name ${test_source} NAME_WE)

    add_test(NAME ${test_name} test_source)
    
endforeach(test_source ${TEST_SOURCES})

