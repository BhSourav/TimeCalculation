####################################################################################
## Created by Sourav Bhattacharjee on 26.07.25.
####################################################################################
cmake_minimum_required(VERSION 3.31)
project(TimeCalculation VERSION 0.0.1 LANGUAGES CXX)

####################################################################################
#Default Configs
####################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

####################################################################################
# Build Configs
####################################################################################
set(BUILD_SHARED_LIBS ON)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME}::${CMAKE_PROJECT_NAME} ALIAS ${CMAKE_PROJECT_NAME})
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION}
)
target_sources(${CMAKE_PROJECT_NAME}
        PUBLIC
        FILE_SET interfaces
        TYPE CXX_MODULES
        FILES
        "interfaces/TimeBase.cppm"
        "interfaces/StopWatch.cppm"
        #PRIVATE
)
target_sources(${CMAKE_PROJECT_NAME}
        PUBLIC
        FILE_SET include
        TYPE HEADERS
        FILES
        #"cmake/export.h"
        "includes/template-header.h"
        #PRIVATE
)
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:MYLIB_STATIC_DEFINE>")
message(${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/cmake>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)

####################################################################################
# Export Header
####################################################################################
include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()
generate_export_header(${CMAKE_PROJECT_NAME} EXPORT_FILE_NAME
        cmake/${export_file_name})

####################################################################################
###################################################################################
# executable
###################################################################################
#add_executable(${CMAKE_PROJECT_NAME}_exec "main.cpp")
#target_link_libraries(${CMAKE_PROJECT_NAME}_exec PUBLIC ${CMAKE_PROJECT_NAME})
###################################################################################

####################################################################################
# Unit Testing Setup
####################################################################################
# Add an option to enable/disable tests from the command line (-DBUILD_TESTING=ON/OFF)
option(BUILD_TESTING "Build the unit tests" OFF)

# Only set up testing infrastructure if the user has enabled it.
if(BUILD_TESTING)
    # Enable CTest
    enable_testing()

    # Use FetchContent to download GoogleTest at configure time.
    # This is the modern, preferred way and doesn't require users to pre-install it.
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip # Release 1.14.0
    )
    # Tell FetchContent to make the library available, but only if it hasn't been
    # fetched by another part of the project.
    FetchContent_MakeAvailable(googletest)

    # Add the subdirectory that contains the test definitions.
    add_subdirectory(UnitTests)
endif()