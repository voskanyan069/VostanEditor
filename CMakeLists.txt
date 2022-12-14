# CMake version
CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

# Project information
PROJECT( VostanEditor VERSION 1.0 )

# Build specifiactions
SET( CMAKE_CXX_STANDARD 20 )
SET( CMAKE_BUILD_TYPE DEBUG )

# CXX Flags
SET( CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )

# Find 3rd party modules
FIND_PACKAGE( CURL REQUIRED )
FIND_PACKAGE( Boost REQUIRED )
FIND_PACKAGE( TCL REQUIRED )
FIND_PACKAGE( Qt5 COMPONENTS Core Widgets REQUIRED )

# Local libraries
SET( TARGET_LIBS ${TARGET_LIBS} cmd_line )
SET( TARGET_LIBS ${TARGET_LIBS} commands )
SET( TARGET_LIBS ${TARGET_LIBS} remote )
SET( TARGET_LIBS ${TARGET_LIBS} curlwrp )
SET( TARGET_LIBS ${TARGET_LIBS} io )
SET( TARGET_LIBS ${TARGET_LIBS} tcl8.6 )

# 3rd party libraries
SET( TARGET_LIBS ${TARGET_LIBS} CURL::libcurl )
SET( TARGET_LIBS ${TARGET_LIBS} ${TCL_LIBRARY} )
SET( TARGET_LIBS ${TARGET_LIBS} Qt5::Core )
SET( TARGET_LIBS ${TARGET_LIBS} Qt5::Widgets )

# Definitions
ADD_DEFINITIONS( ${Qt5Widgets_DEFINITIONS} )

# Define files hierarchy structure
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/cmd_line
    ${PROJECT_SOURCE_DIR}/src/commands
    ${PROJECT_SOURCE_DIR}/src/curlwrp
    ${PROJECT_SOURCE_DIR}/src/remote
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/io
    ${PROJECT_SOURCE_DIR}/third_party/cli/include/
    ${PROJECT_SOURCE_DIR}/third_party/nlohmann/include/
    ${BOOST_INCLUDE_DIRS}
    ${TCL_INCLUDE_PATH}
    ${Qt5Widgets_INCLUDE_DIRS}
    )

MESSAGE( STATUS Qt5::Core )

# Specify files for each local library
FILE( GLOB CMD_LINE_SOURCES "${PROJECT_SOURCE_DIR}/src/cmd_line/*.cxx" )
FILE( GLOB CMDS_SOURCES "${PROJECT_SOURCE_DIR}/src/commands/*.cxx" )
FILE( GLOB CURLWRP_SOURCES "${PROJECT_SOURCE_DIR}/src/curlwrp/*.cxx" )
FILE( GLOB REMOTE_SOURCES "${PROJECT_SOURCE_DIR}/src/remote/*.cxx" )
FILE( GLOB IO_SOURCES "${PROJECT_SOURCE_DIR}/src/io/*.cxx" )

# Define local libraries
ADD_LIBRARY( cmd_line SHARED ${CMD_LINE_SOURCES} )
ADD_LIBRARY( commands SHARED ${CMDS_SOURCES} )
ADD_LIBRARY( curlwrp SHARED ${CURLWRP_SOURCES} )
ADD_LIBRARY( remote SHARED ${REMOTE_SOURCES} )
ADD_LIBRARY( io SHARED ${IO_SOURCES} )

# Define executables
ADD_EXECUTABLE( VostanEditor "${PROJECT_SOURCE_DIR}/src/main.cxx" )

# Link executables to libraries
TARGET_LINK_LIBRARIES( VostanEditor ${TARGET_LIBS} )

# Copy commands
FILE( COPY "${PROJECT_SOURCE_DIR}/scripts" DESTINATION "${CMAKE_BINARY_DIR}" )
