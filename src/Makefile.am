bin_PROGRAMS = kbruch

kbruch_COMPILE_FIRST = version.h
kbruch_SOURCES = exercisebase.cpp taskvieweroptionsbase.ui task.cpp ratio.cpp taskview.cpp statisticsview.cpp primenumber.cpp kbruch.cpp mainqtwidget.cpp taskwidget.cpp fractionbasewidget.cpp resultwidget.cpp exercisecompare.cpp ratiowidget.cpp rationalwidget.cpp exerciseconvert.cpp exercisefactorize.cpp factorizedwidget.cpp settingsclass.kcfgc
kbruch_LDADD   =  $(LIB_KDEUI)
# the library search path.
kbruch_LDFLAGS = $(all_libraries) $(KDE_RPATH)

EXTRA_DIST = kbruch.cpp kbruch.h kbruch.desktop hi32-app-kbruch.png hi16-app-kbruch.png lo32-app-kbruch.png lo16-app-kbruch.png mainqtwidget.cpp mainqtwidget.h primenumber.cpp primenumber.h statisticsview.cpp statisticsview.h exercisebase.cpp exercisebase.h taskview.cpp taskview.h ratio.cpp ratio.h task.cpp task.h taskwidget.cpp taskwidget.h fractionbasewidget.cpp fractionbasewidget.h resultwidget.cpp resultwidget.h exercisecompare.cpp exercisecompare.h ratiowidget.cpp ratiowidget.h rationalwidget.cpp rationalwidget.h exerciseconvert.cpp exerciseconvert.h exercisefactorize.cpp exercisefactorize.h factorizedwidget.cpp factorizedwidget.h kbruch.rc taskvieweroptionsbase.ui

xdg_apps_DATA = kbruch.desktop
kde_kcfg_DATA = kbruch.kcfg

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

rcdir = $(kde_datadir)/kbruch
rc_DATA = kbruchui.rc

messages: rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kbruch.pot; \
	fi

version.h: $(top_srcdir)/kbruch/VERSION
	printf "#undef KBRUCH_VERSION\n#define KBRUCH_VERSION \"`cat $(top_srcdir)/kbruch/VERSION`\"\n" > version.h

CLEANFILES = version.h

noinst_HEADERS = version.h
