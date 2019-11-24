#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gcomm" for configuration "Debug"
set_property(TARGET gcomm APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gcomm PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libgcomm.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS gcomm )
list(APPEND _IMPORT_CHECK_FILES_FOR_gcomm "${_IMPORT_PREFIX}/lib/libgcomm.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
