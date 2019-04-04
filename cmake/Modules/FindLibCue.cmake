# - Try to find LibCue
# Once done this will define
#  LIBCUE_FOUND - System has LibCue
#  LIBCUE_INCLUDE_DIRS - The LibCue include directories
#  LIBCUE_LIBRARIES - The libraries needed to use LibCue
#  LIBCUE_DEFINITIONS - Compiler switches required for using LibCue

find_package(PkgConfig)
pkg_check_modules(PC_LIBCUE QUIET libcue)
set(LIBCUE_DEFINITIONS ${PC_LIBCUE_CFLAGS_OTHER})

find_path(LIBCUE_INCLUDE_DIR libcue/libcue.h
          HINTS ${PC_LIBCUE_INCLUDEDIR} ${PC_LIBCUE_INCLUDE_DIRS}
          PATH_SUFFIXES libcue)

find_library(LIBCUE_LIBRARY NAMES cue libcue
             HINTS ${PC_LIBCUE_LIBDIR} ${PC_LIBCUE_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibCue DEFAULT_MSG LIBCUE_LIBRARY LIBCUE_INCLUDE_DIR)

mark_as_advanced(LIBCUE_INCLUDE_DIR LIBCUE_LIBRARY)

set(LIBCUE_LIBRARIES ${LIBCUE_LIBRARY})
set(LIBCUE_INCLUDE_DIRS ${LIBCUE_INCLUDE_DIR})
