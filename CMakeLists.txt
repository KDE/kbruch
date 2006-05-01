kde4_header()
project(kbruch)

add_subdirectory( src ) 
add_subdirectory( testcases ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = src testcases
#
#EXTRA_DIST = AUTHORS ChangeLog README TODO kbruch.lsm VERSION
#
#VERSION:=`cat ${PWD}/VERSION`
#include ../admin/Doxyfile.am
