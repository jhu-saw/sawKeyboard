#
# $Id$
#
# (C) Copyright 2011-2012 Johns Hopkins University (JHU), All Rights
# Reserved.
#
# --- begin cisst license - do not edit ---
#
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
#
# --- end cisst license ---

# Core library
set (REQUIRED_CISST_LIBRARIES cisstCommon
                              cisstVector
                              cisstOSAbstraction
                              cisstMultiTask)

find_package (cisst REQUIRED ${REQUIRED_CISST_LIBRARIES})

if (cisst_FOUND_AS_REQUIRED)

  # load cisst configuration
  include (${CISST_USE_FILE})

  # create/configure file for find_package (sawKeyboard)
  set (sawKeyboard_INCLUDE_DIR "${sawKeyboard_SOURCE_DIR}/include")
  set (sawKeyboard_LIBRARY_DIR "${LIBRARY_OUTPUT_PATH}")
  set (sawKeyboard_LIBRARIES sawKeyboard)

  include_directories (${sawKeyboard_INCLUDE_DIR})
  set (sawKeyboard_HEADER_DIR "${sawKeyboard_INCLUDE_DIR}/sawKeyboard")
  add_library (sawKeyboard
               mtsKeyboard.cpp
               ${sawKeyboard_HEADER_DIR}/sawKeyboardExport.h
               ${sawKeyboard_HEADER_DIR}/mtsKeyboard.h)
  set_property (TARGET sawKeyboard PROPERTY FOLDER "sawKeyboard")
  cisst_target_link_libraries (sawKeyboard ${REQUIRED_CISST_LIBRARIES})

  configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/sawKeyboardConfig.cmake.in"
                  "${sawKeyboard_BINARY_DIR}/sawKeyboardConfig.cmake"
                   @ONLY@)

  # Install target for headers and library
  install (DIRECTORY
           ${sawKeyboard_INCLUDE_DIR}/sawKeyboard
           DESTINATION include
           PATTERN .svn EXCLUDE)

  install (TARGETS sawKeyboard
           RUNTIME DESTINATION bin
           LIBRARY DESTINATION lib
           ARCHIVE DESTINATION lib)

  install (FILES "${sawKeyboard_BINARY_DIR}/sawKeyboardConfig.cmake"
           DESTINATION "${CISST_CMAKE_INSTALL_SUFFIX}/saw")

else (cisst_FOUND_AS_REQUIRED)
  message ("Information: code in ${CMAKE_CURRENT_SOURCE_DIR} will not be compiled, it requires ${REQUIRED_CISST_LIBRARIES}")
endif (cisst_FOUND_AS_REQUIRED)
