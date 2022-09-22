cmake_minimum_required(VERSION 3.5)

################################################################################
# Specific section
################################################################################

# Set project name
project("Rapidjson")
# Set project path
set(DIR_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
# Set applications path
set(DIR_APP "${DIR_PROJECT}/app")
# Set application libraries path
set(DIR_APP_LIBS "${DIR_APP}/lib")
# Set utilities path
set(DIR_UTILITIES "${DIR_PROJECT}/utilities")
# Set external libraries path
set(DIR_EXTERNALS "${DIR_PROJECT}/external")
# Set souces paths
set(DIR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"
)
# Set includes paths
set(DIR_INCLUDES 
    "${CMAKE_CURRENT_SOURCE_DIR}/../inc/"
    "${DIR_UTILITIES}/string/inc"
    "${DIR_UTILITIES}/fs/file/inc"
    "${DIR_EXTERNALS}/rapidjson/inc"
)

################################################################################
# Generic section
################################################################################

# Set flags
set(CMAKE_CXX_FLAGS "-g -O -c")
# Add build option
option(BUILD_APPLICATION "Build ${PROJECT_NAME}" ON)
# Add include files Path
include_directories(${PROJECT_NAME} INTERFACE ${DIR_INCLUDES})
# Add lib source
file(GLOB LIB_SOURCES ${DIR_SOURCES})
#  Add module binary
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})
# Set Target properties
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "out"
    LIBRARY_OUTPUT_DIRECTORY "out"
)

