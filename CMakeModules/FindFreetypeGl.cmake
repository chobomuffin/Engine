# - Try to find Freetype-gl
# Once done, this will define
#
#  FREETYPE_GL_FOUND - system has Freetype-gl
#  FREETYPE_GL_INCLUDE_DIR - the Freetype-gl include directories
#  FREETYPE_GL_LIBRARIES - link these to use Freetype-gl

FIND_PATH( FREETYPE_GL_INCLUDE_DIR freetype-gl/freetype-gl.h
  /usr/include
  /usr/local/include
  /opt/local/include
)

FIND_LIBRARY( FREETYPE_GL_LIBRARY freetype-gl
  /usr/lib64
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

IF(FREETYPE_GL_INCLUDE_DIR AND FREETYPE_GL_LIBRARY)
  SET( FREETYPE_GL_FOUND TRUE )
  SET( FREETYPE_GL_LIBRARIES ${FREETYPE_GL_LIBRARY} )
ENDIF(FREETYPE_GL_INCLUDE_DIR AND FREETYPE_GL_LIBRARY)

IF(FREETYPE_GL_FOUND)
   IF(NOT FREETYPE_GL_FIND_QUIETLY)
      MESSAGE(STATUS "Found freetype-gl: ${FREETYPE_GL_LIBRARY}")
   ENDIF(NOT FREETYPE_GL_FIND_QUIETLY)
ELSE(FREETYPE_GL_FOUND)
   IF(FREETYPE_GL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find libfreetype-gl")
   ENDIF(FREETYPE_GL_FIND_REQUIRED)
ENDIF(FREETYPE_GL_FOUND)
