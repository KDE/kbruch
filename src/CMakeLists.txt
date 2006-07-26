



########### next target ###############

macro(CREATE_VERSION _in_FILE _out_FILE)
	FILE(READ ${_in_FILE} _contents)
	FILE(WRITE ${_out_FILE} "#undef KBRUCH_VERSION\n")
	STRING(REGEX REPLACE "\n" "" _contents "${_contents}" )
	FILE(APPEND ${_out_FILE} "#define KBRUCH_VERSION \"${_contents}\"")
endmacro(CREATE_VERSION)

macro_additional_clean_files( ${CMAKE_CURRENT_BINARY_DIR}/version.h )

create_version(${CMAKE_SOURCE_DIR}/kbruch/VERSION version.h)
set(kbruch_SRCS
   exercisebase.cpp 
   task.cpp 
   ratio.cpp 
   taskview.cpp 
   statisticsview.cpp 
   primenumber.cpp 
   kbruch.cpp 
   mainqtwidget.cpp 
   taskwidget.cpp 
   fractionbasewidget.cpp 
   resultwidget.cpp 
   exercisecompare.cpp 
   ratiowidget.cpp 
   rationalwidget.cpp 
   exerciseconvert.cpp 
   exercisefactorize.cpp 
   factorizedwidget.cpp )

kde4_automoc(${kbruch_SRCS})

kde4_add_ui3_files(kbruch_SRCS taskvieweroptionsbase.ui )

kde4_add_kcfg_files(kbruch_SRCS settingsclass.kcfgc )

kde4_add_executable(kbruch ${kbruch_SRCS})

target_link_libraries(kbruch  ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} )

install(TARGETS kbruch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kbruch.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kbruchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )

kde4_install_icons( ${ICON_INSTALL_DIR}  crystalsvg )
kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )




#original Makefile.am contents follow:

#bin_PROGRAMS = kbruch
#
#kbruch_COMPILE_FIRST = version.h
#kbruch_SOURCES = exercisebase.cpp taskvieweroptionsbase.ui task.cpp ratio.cpp taskview.cpp statisticsview.cpp primenumber.cpp kbruch.cpp mainqtwidget.cpp taskwidget.cpp fractionbasewidget.cpp resultwidget.cpp exercisecompare.cpp ratiowidget.cpp rationalwidget.cpp exerciseconvert.cpp exercisefactorize.cpp factorizedwidget.cpp settingsclass.kcfgc
#kbruch_LDADD   =  $(LIB_KDEUI)
## the library search path.
#kbruch_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
#EXTRA_DIST = kbruch.cpp kbruch.h kbruch.desktop hi32-app-kbruch.png hi16-app-kbruch.png lo32-app-kbruch.png lo16-app-kbruch.png mainqtwidget.cpp mainqtwidget.h primenumber.cpp primenumber.h statisticsview.cpp statisticsview.h exercisebase.cpp exercisebase.h taskview.cpp taskview.h ratio.cpp ratio.h task.cpp task.h taskwidget.cpp taskwidget.h fractionbasewidget.cpp fractionbasewidget.h resultwidget.cpp resultwidget.h exercisecompare.cpp exercisecompare.h ratiowidget.cpp ratiowidget.h rationalwidget.cpp rationalwidget.h exerciseconvert.cpp exerciseconvert.h exercisefactorize.cpp exercisefactorize.h factorizedwidget.cpp factorizedwidget.h kbruch.rc taskvieweroptionsbase.ui
#
#xdg_apps_DATA = kbruch.desktop
#kde_kcfg_DATA = kbruch.kcfg
#
#KDE_ICON = AUTO
#
######## kdevelop will overwrite this part!!! (end)############
## this 10 paths are KDE specific. Use them:
## kde_htmldir       Where your docs should go to. (contains lang subdirs)
## kde_appsdir       Where your application file (.kdelnk) should go to.
## kde_icondir       Where your icon should go to.
## kde_minidir       Where your mini icon should go to.
## kde_datadir       Where you install application data. (Use a subdir)
## kde_locale        Where translation files should go to.(contains lang subdirs)
## kde_cgidir        Where cgi-bin executables should go to.
## kde_confdir       Where config files should go to.
## kde_mimedir       Where mimetypes should go to.
## kde_toolbardir    Where general toolbar icons should go to.
## kde_wallpaperdir  Where general wallpapers should go to.
#
## set the include path for X, qt and KDE
#INCLUDES= $(all_includes)
#
#METASOURCES = AUTO
#
#rcdir = $(kde_datadir)/kbruch
#rc_DATA = kbruchui.rc
#
#messages: rc.cpp
#	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
#	if test -n "$$LIST"; then \
#	 $(XGETTEXT) $$LIST -o $(podir)/kbruch.pot; \
#	fi
#
#version.h: $(top_srcdir)/kbruch/VERSION
#	printf "#undef KBRUCH_VERSION\n#define KBRUCH_VERSION \"`cat $(top_srcdir)/kbruch/VERSION`\"\n" > version.h
#
#CLEANFILES = version.h
#
#noinst_HEADERS = version.h
