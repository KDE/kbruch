



########### next target ###############

set(kbruch_test_SRCS 
   task.cpp 
   ratio.cpp 
   ratio_test.cpp 
   primenumber.cpp 
   primenumber_test.cpp 
   kbruch_test.cpp )

kde4_automoc(${kbruch_test_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(kbruch_test ${kbruch_test_SRCS})

target_link_libraries(kbruch_test  ${KDE4_KDECORE_LIBS} boost_unit_test_framework-gcc kdeui )

endif(KDE4_BUILD_TESTS)

########### install files ###############





#original Makefile.am contents follow:

#check_PROGRAMS = kbruch_test
#
#kbruch_test_SOURCES = task.cpp ratio.cpp ratio_test.cpp primenumber.cpp primenumber_test.cpp kbruch_test.cpp
#
#kbruch_test_LDADD   = -lboost_unit_test_framework-gcc $(LIB_KDEUI)
#
## the library search path.
#kbruch_test_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
#EXTRA_DIST = primenumber.cpp primenumber.h ratio.cpp ratio.h task.cpp task.h primenumber_test.cpp ratio_test.cpp kbruch_test.cpp
#
## set the include path for X, qt and KDE
#INCLUDES= $(all_includes)
