

########### next target ###############

remove_definitions(-DQT3_SUPPORT_WARNINGS)
remove_definitions(-DQT3_SUPPORT)

macro(CREATE_VERSION _in_FILE _out_FILE)
	FILE(READ ${_in_FILE} _contents)
	FILE(WRITE ${_out_FILE} "#undef KBRUCH_VERSION\n")
	STRING(REGEX REPLACE "\n" "" _contents "${_contents}" )
	FILE(APPEND ${_out_FILE} "#define KBRUCH_VERSION \"${_contents}\"\n")
endmacro(CREATE_VERSION)

macro_additional_clean_files( ${CMAKE_CURRENT_BINARY_DIR}/version.h )

create_version(${CMAKE_SOURCE_DIR}/kbruch/VERSION ${CMAKE_CURRENT_BINARY_DIR}/version.h)
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

kde4_automoc(kbruch ${kbruch_SRCS})

kde4_add_ui_files(kbruch_SRCS taskvieweroptionsbase.ui )

kde4_add_kcfg_files(kbruch_SRCS settingsclass.kcfgc )

kde4_add_executable(kbruch ${kbruch_SRCS})

target_link_libraries(kbruch  ${KDE4_KDEUI_LIBS} )

install(TARGETS kbruch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kbruch.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kbruchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )

kde4_install_icons( ${ICON_INSTALL_DIR}   )

