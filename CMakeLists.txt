project(kbruch)

add_subdirectory( src ) 
#add_subdirectory( testcases ) 




########### install files ###############





#original Makefile.am contents follow:

#SUBDIRS = src testcases
#
#EXTRA_DIST = AUTHORS ChangeLog README TODO kbruch.lsm VERSION
#
#VERSION:=`cat ${PWD}/VERSION`
#include ../admin/Doxyfile.am
