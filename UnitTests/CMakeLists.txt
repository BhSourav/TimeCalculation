####################################################################################
## Created by Sourav Bhattacharjee on 26.07.25.
####################################################################################
add_executable(run_tests
        TimeBase_test.cpp
        MockTimeBase.cpp
        MockTimeBase.h
        Alarm_test.cpp
        MockAlarm.cpp
        MockAlarm.h
)
target_compile_definitions( run_tests PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY )
# Link your test executable against your main library target.
# This gives your tests access to the code you want to test.
target_link_libraries(run_tests
        PRIVATE
        TimeCalculation::TimeCalculation
)

# Link against GoogleTest.
# The GTest::gtest_main target provides a main() function and links all
# necessary gtest and gmock libraries.
target_link_libraries(run_tests
        PRIVATE
        GTest::gtest_main
        GTest::gmock_main
)

target_include_directories(run_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/_deps/googletest-src/googletest/include
        ${CMAKE_SOURCE_DIR}/_deps/googletest-src/googlemock/include
)

# This CMake command automatically finds all tests (TEST_F, TEST, etc.)
# in your executable and adds them to CTest.
include(GoogleTest)
gtest_discover_tests(run_tests)