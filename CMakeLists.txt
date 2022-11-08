# CMake version
CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

# Project information
PROJECT( VostanEditor VERSION 1.0 )

# Build specifiactions
SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_BUILD_TYPE DEBUG )

# Find 3rd party modules
FIND_PACKAGE( CURL REQUIRED )

# Local libraries
SET( TARGET_LIBS ${TARGET_LIBS} cmd )
SET( TARGET_LIBS ${TARGET_LIBS} curlwrp )
SET( TARGET_LIBS ${TARGET_LIBS} remote )
# 3rd party libraries
SET( TARGET_LIBS ${TARGET_LIBS} CURL::libcurl )

# Define files hierarchy structure
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/inc/cmd
    ${PROJECT_SOURCE_DIR}/src/cmd
    ${PROJECT_SOURCE_DIR}/inc/curlwrp
    ${PROJECT_SOURCE_DIR}/src/curlwrp
    ${PROJECT_SOURCE_DIR}/inc/remote
    ${PROJECT_SOURCE_DIR}/src/remote
    ${PROJECT_SOURCE_DIR}/third_party/cli/include/
    )

# Specify files for each local library
FILE( GLOB CMD_SOURCES "${PROJECT_SOURCE_DIR}/src/cmd/*.cxx" )
FILE( GLOB CURLWRP_SOURCES "${PROJECT_SOURCE_DIR}/src/curlwrp/*.cxx" )
FILE( GLOB REMOTE_SOURCES "${PROJECT_SOURCE_DIR}/src/remote/*.cxx" )

# Define local libraries
ADD_LIBRARY( cmd SHARED ${CMD_SOURCES} )
ADD_LIBRARY( curlwrp SHARED ${CURLWRP_SOURCES} )
ADD_LIBRARY( remote SHARED ${REMOTE_SOURCES} )

# Define executables
ADD_EXECUTABLE( VostanEditor "${PROJECT_SOURCE_DIR}/src/main.cxx" )

# Link executables to libraries
TARGET_LINK_LIBRARIES( VostanEditor ${TARGET_LIBS} )
