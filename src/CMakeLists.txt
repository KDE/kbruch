
add_subdirectory( pics ) 


########### next target ###############

macro_additional_clean_files( ${CMAKE_CURRENT_BINARY_DIR}/kbruch_version.h )
file(READ ${CMAKE_SOURCE_DIR}/kbruch/VERSION KBRUCH_VERSION )
string(REGEX REPLACE "\n" "" KBRUCH_VERSION ${KBRUCH_VERSION} )
configure_file(${CMAKE_SOURCE_DIR}/kbruch/src/kbruch_version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kbruch_version.h)

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


kde4_add_ui_files(kbruch_SRCS taskcolorsbase.ui taskgeneralbase.ui taskfontsbase.ui)

kde4_add_kcfg_files(kbruch_SRCS settingsclass.kcfgc )

kde4_add_app_icon(kbruch_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/kbruch.png")
#kde4_add_app_icon(kbruch_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/ox*-apps-kbruch.png")

kde4_add_executable(kbruch ${kbruch_SRCS})

target_link_libraries(kbruch  ${KDE4_KDEUI_LIBS} )

install(TARGETS kbruch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kbruch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kbruchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )

kde4_install_icons( ${ICON_INSTALL_DIR}   )

