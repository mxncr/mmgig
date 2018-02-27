##############################################################################
# CMake file for mmgig
##############################################################################

# CMake 2.8.11 is required for 2 reasons:
# - it is the first version that fully supports the specification of Visual
# Studio toolsets (v110_xp).
# - it is the version that supports the command string(TIMESTAMP ...)
cmake_minimum_required(VERSION 2.8.11)

project(mmgig)

if(EXISTS ${CMAKE_SOURCE_DIR}/../../../../Vorpaline/trunk/)
   set(
      GEOGRAM_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../../../Vorpaline/trunk"
      CACHE PATH "full path to the Geogram (or Vorpaline) installation"
   )
else()
   set(
      GEOGRAM_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../../geogram/"
      CACHE PATH "full path to the Geogram (or Vorpaline) installation"
   )
endif()

set(
   GRAPHITE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../../../GraphiteThree/"
   CACHE PATH "full path to the Graphite installation"
)

include(${GRAPHITE_SOURCE_DIR}/cmake/graphite.cmake)

if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeOptions.txt)
   message(STATUS "Using options file: ${CMAKE_SOURCE_DIR}/CMakeOptions.txt")
   include(${CMAKE_SOURCE_DIR}/CMakeOptions.txt)
endif()


##############################################################################

include_directories(${CMAKE_SOURCE_DIR}/../../)

# DLLs/shared objects are generated in Graphite binary directory

set(
   CMAKE_RUNTIME_OUTPUT_DIRECTORY
   ${GRAPHITE_SOURCE_DIR}/${RELATIVE_BIN_OUTPUT_DIR}
)

set(
   CMAKE_LIBRARY_OUTPUT_DIRECTORY
   ${GRAPHITE_SOURCE_DIR}/${RELATIVE_LIB_OUTPUT_DIR}
)


##############################################################################

SET(MMG_DIR "$ENV{HOME}/ext/mmg")
SET(MMG_INCLUDE_DIR "${MMG_DIR}/build/include")
SET(MMG_LIBRARY "${MMG_DIR}/build/lib/libmmg.so")
message(STATUS "MMG_DIR (given by user): ${MMG_DIR}")
message(STATUS "MMG_INCLUDE_DIR: ${MMG_INCLUDE_DIR}")
message(STATUS "MMG_LIBRARY: ${MMG_LIBRARY}")
if(NOT EXISTS ${MMG_LIBRARY})
    message(FATAL_ERROR "libmmg.so not found at ${MMG_LIBRARY}. Please set MMG_DIR in the CMakeLists.txt file")
endif()

INCLUDE_DIRECTORIES(${MMG_INCLUDE_DIR})

##############################################################################

aux_source_directories(SOURCES "Source Files\\common" common)
aux_source_directories(SOURCES "Source Files\\algo" algo)
aux_source_directories(SOURCES "Source Files\\commands" commands)
gomgen(mmgig)

add_library(mmgig ${SOURCES})

target_link_libraries(mmgig mesh scene_graph skin gom_gom gom)
target_link_libraries(mmgig renderer image basic)
target_link_libraries(mmgig ${MMG_LIBRARY})
