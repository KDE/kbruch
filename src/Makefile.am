####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kbruch
kbruch_SOURCES = tool.cpp task.cpp ratio.cpp qt_task.cpp qt_stat.cpp qt_para.cpp primzahl.cpp gui_qt.cpp kbruch.cpp 
kbruch_LDADD   =  $(LIB_QT) $(LIB_KDECORE) $(LIB_KDEUI)  $(LIBSOCKET)


EXTRA_DIST = kbruch.cpp kbruch.h kbruch.desktop hi32-app-kbruch.png hi16-app-kbruch.png lo32-app-kbruch.png lo16-app-kbruch.png gui_qt.cpp gui_qt.h primzahl.cpp primzahl.h qt_para.cpp qt_para.h qt_stat.cpp qt_stat.h qt_task.cpp qt_task.h ratio.cpp ratio.h task.cpp task.h tool.cpp tool.h 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kbruch.desktop $(kde_appsdir)/Applications/kbruch.desktop
	$(mkinstalldirs) $(kde_icondir)/hicolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/hi32-app-kbruch.png $(kde_icondir)/hicolor/32x32/apps/kbruch.png
	$(mkinstalldirs) $(kde_icondir)/hicolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/hi16-app-kbruch.png $(kde_icondir)/hicolor/16x16/apps/kbruch.png
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-kbruch.png $(kde_icondir)/locolor/32x32/apps/kbruch.png
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-kbruch.png $(kde_icondir)/locolor/16x16/apps/kbruch.png

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/kbruch.desktop
	-rm -f $(kde_icondir)/hicolor/32x32/apps/kbruch.png
	-rm -f $(kde_icondir)/hicolor/16x16/apps/kbruch.png
	-rm -f $(kde_icondir)/locolor/32x32/apps/kbruch.png
	-rm -f $(kde_icondir)/locolor/16x16/apps/kbruch.png

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

# the library search path. 
kbruch_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# Uncomment the following two lines if you add a ui.rc file for your application to make use of
# KDE
#rcdir = $(kde_datadir)/kbruch
#rc_DATA = kbruchui.rc

#WARNING: if you use a ui.rc file above, use:

# messages: rc.cpp

# instead of 

# messages:

messages: 
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kbruch.pot; \
	fi

