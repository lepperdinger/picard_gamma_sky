# Find Healpix

FIND_PATH(HEALPIX_INCLUDE_DIR
NAMES
        healpix_map.h
PATHS
        /usr/include/
        /usr/include/healpix_cxx
        /usr/local/include/
        /usr/local/include/healpix_cxx
	${PATH_HEALPIX}/src/cxx/generic_gcc/include
)

FIND_LIBRARY(HEALPIX_LIBRARIES
NAMES
        healpix_cxx
PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
	${PATH_HEALPIX}/src/cxx/generic_gcc/lib
)

FIND_LIBRARY(HEALPIX_CXXSUPPORT_LIBRARIES
NAMES
        cxxsupport
PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
	${PATH_HEALPIX}/src/cxx/generic_gcc/lib
	)

      
SET(HEALPIX_FOUND FALSE)

IF(HEALPIX_INCLUDE_DIR AND CCFITS_LIBRARIES)
  SET(CCFITS_FOUND TRUE)
  MESSAGE(STATUS "HealPix library found!")
  MESSAGE(STATUS "HEALPIX_INCLUDE_DIR=${HEALPIX_INCLUDE_DIR}")
  MESSAGE(STATUS "HEALPIX_LIBRARIES=${HEALPIX_LIBRARIES}")
  IF(HEALPIX_CXXSUPPORT_LIBRARIES)
    MESSAGE(STATUS "Found CxxSupport libraries for HealPix")
    MESSAGE(STATUS "HEALPIX_CXXSUPPORT_LIBRARIES=${HEALPIX_CXXSUPPORT_LIBRARIES}")
  ENDIF()
ELSE()
  MESSAGE(STATUS "HealPix not found")
  MESSAGE(STATUS "HEALPIX_INCLUDE_DIR=${HEALPIX_INCLUDE_DIR}")
  MESSAGE(STATUS "HEALPIX_LIBRARIES=${HEALPIX_LIBRARIES}")
ENDIF()  
