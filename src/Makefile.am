bin_PROGRAMS = kbruch
kbruch_SOURCES = task.cpp ratio.cpp taskview.cpp statisticsview.cpp primzahl.cpp kbruch.cpp mainqtwidget.cpp
kbruch_LDADD   =  $(LIB_KDEUI)
# the library search path. 
kbruch_LDFLAGS = $(all_libraries) $(KDE_RPATH)

EXTRA_DIST = kbruch.cpp kbruch.h kbruch.desktop hi32-app-kbruch.png hi16-app-kbruch.png lo32-app-kbruch.png lo16-app-kbruch.png mainqtwidget.cpp mainqtwidget.h primzahl.cpp primzahl.h statisticsview.cpp statisticsview.h taskview.cpp taskview.h ratio.cpp ratio.h task.cpp task.h kbruch.rc

mydatadir = $(kde_appsdir)/Applications/
mydata_DATA = kbruch.desktop

KDE_ICON = AUTO

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# Uncomment the following two lines if you add a ui.rc file for your application to make use of
# KDE
rcdir = $(kde_datadir)/kbruch
rc_DATA = kbruchui.rc

#WARNING: if you use a ui.rc file above, use:
# messages: rc.cpp
# instead of 
# messages:
messages: 
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kbruch.pot; \
	fi

