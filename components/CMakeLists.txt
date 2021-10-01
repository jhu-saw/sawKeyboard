#
# (C) Copyright 2011-2021 Johns Hopkins University (JHU), All Rights Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---

cmake_minimum_required (VERSION 3.10)
project (sawKeyboard VERSION 1.3.0)

# Core libraries
set (REQUIRED_CISST_LIBRARIES cisstCommon
                              cisstVector
                              cisstOSAbstraction
                              cisstMultiTask
                              cisstParameterTypes)

find_package (cisst 1.1.0 COMPONENTS ${REQUIRED_CISST_LIBRARIES})

if (cisst_FOUND_AS_REQUIRED)

  # load cisst configuration
  include (${CISST_USE_FILE})

  # catkin/ROS paths
  cisst_is_catkin_build (sawKeyboard_IS_CATKIN_BUILT)
  if (sawKeyboard_IS_CATKIN_BUILT)
    set (sawKeyboard_CONFIG_FILE_DIR
         "${CATKIN_DEVEL_PREFIX}/share/sawKeyboard/cmake")
    set (LIBRARY_OUTPUT_PATH "${CATKIN_DEVEL_PREFIX}/lib")
  else ()
    set (sawKeyboard_CONFIG_FILE_DIR "${sawKeyboard_BINARY_DIR}")
  endif ()

  # create/configure file for find_package (sawKeyboard)
  set (sawKeyboard_INCLUDE_DIR
       "${sawKeyboard_SOURCE_DIR}/include"
       "${sawKeyboard_BINARY_DIR}/include")
  set (sawKeyboard_LIBRARY_DIR "${LIBRARY_OUTPUT_PATH}")
  set (sawKeyboard_LIBRARIES sawKeyboard)

  # Generate sawKeyboardRevision.h
  configure_file ("${sawKeyboard_SOURCE_DIR}/code/sawKeyboardRevision.h.in"
                  "${sawKeyboard_BINARY_DIR}/include/sawKeyboard/sawKeyboardRevision.h")

  include_directories (${sawKeyboard_INCLUDE_DIR})
  set (sawKeyboard_HEADER_DIR "${sawKeyboard_SOURCE_DIR}/include/sawKeyboard")
  add_library (sawKeyboard
               code/mtsKeyboard.cpp
               ${sawKeyboard_HEADER_DIR}/sawKeyboardExport.h
               ${sawKeyboard_HEADER_DIR}/mtsKeyboard.h
               ${sawKeyboard_BINARY_DIR}/include/sawKeyboard/sawKeyboardRevision.h)

  set_target_properties (sawKeyboard PROPERTIES
                         VERSION ${sawKeyboard_VERSION}
                         COMPONENT sawKeyboard
                         FOLDER "sawKeyboard")

  cisst_target_link_libraries (sawKeyboard ${REQUIRED_CISST_LIBRARIES})

  set (sawKeyboard_CMAKE_CONFIG_FILE
       "${sawKeyboard_CONFIG_FILE_DIR}/sawKeyboardConfig.cmake")
  configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/sawKeyboardConfig.cmake.in"
                  "${sawKeyboard_CMAKE_CONFIG_FILE}"
                  @ONLY)

  cisst_add_config_version ("${sawKeyboard_CONFIG_FILE_DIR}/sawKeyboardConfigVersion.cmake"
                            VERSION ${sawKeyboard_VERSION}
                            DESTINATION "share/sawKeyboard"
                            COMPONENT sawKeyboard)

  # Install target for headers and library
  install (DIRECTORY "${sawKeyboard_SOURCE_DIR}/include/sawKeyboard"
                     "${sawKeyboard_BINARY_DIR}/include/sawKeyboard"
           DESTINATION include
           COMPONENT sawKeyboard-dev
           PATTERN .svn EXCLUDE)

  install (TARGETS sawKeyboard COMPONENT sawKeyboard
           RUNTIME DESTINATION bin
           LIBRARY DESTINATION lib
           ARCHIVE DESTINATION lib)

  install (FILES "${sawKeyboard_CMAKE_CONFIG_FILE}"
           DESTINATION "share/sawKeyboard"
           COMPONENT sawKeyboard)

else (cisst_FOUND_AS_REQUIRED)
  message ("Information: code in ${CMAKE_CURRENT_SOURCE_DIR} will not be compiled, it requires ${REQUIRED_CISST_LIBRARIES}")
endif (cisst_FOUND_AS_REQUIRED)
