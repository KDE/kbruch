add_subdirectory( pics ) 

########### next target ###############

# Don't clean the following file, or kbruch will not compiler after
#make clean
macro_additional_clean_files( ${CMAKE_CURRENT_BINARY_DIR}/kbruch_version.h )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

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
   exercisepercentage.cpp   
   ratiowidget.cpp 
   rationalwidget.cpp 
   exerciseconvert.cpp 
   statisticsbarwidget.cpp
   exercisefactorize.cpp
   AppMenuWidget.cpp
   FractionPainter.cpp
   FractionRingWidget.cpp )

kde4_add_ui_files(kbruch_SRCS taskcolorsbase.ui taskgeneralbase.ui taskfontsbase.ui)

kde4_add_kcfg_files(kbruch_SRCS settingsclass.kcfgc )

kde4_add_app_icon(kbruch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-kbruch.png")

kde4_add_executable(kbruch ${kbruch_SRCS})

target_link_libraries(kbruch  ${KDE4_KDEUI_LIBS} )

install(TARGETS kbruch  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES kbruch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kbruchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )
install( FILES AppMenuWidgetui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )
install( FILES FractionRingWidgetui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )

kde4_install_icons( ${ICON_INSTALL_DIR} )
