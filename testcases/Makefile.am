bin_PROGRAMS = kbruch_test

kbruch_test_SOURCES = task.cpp ratio.cpp primzahl.cpp primzahl_test.cpp kbruch_test.cpp

kbruch_test_LDADD   = -lboost_unit_test_framework-gcc $(LIB_KDEUI)

# the library search path.
kbruch_test_LDFLAGS = $(all_libraries) $(KDE_RPATH)

EXTRA_DIST = primzahl.cpp primzahl.h ratio.cpp ratio.h task.cpp task.h primzahl_test.cpp primzahl_test.h kbruch_test.cpp

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
