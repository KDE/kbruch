

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


kde4_add_ui_files(kbruch_SRCS taskvieweroptionsbase.ui )

kde4_add_kcfg_files(kbruch_SRCS settingsclass.kcfgc )

set(kbruch_icons
    ${KDE4_ICON_DIR}/oxygen/16x16/apps/kbruch.png
    ${KDE4_ICON_DIR}/oxygen/32x32/apps/kbruch.png
    ${KDE4_ICON_DIR}/oxygen/48x48/apps/kbruch.png
    ${KDE4_ICON_DIR}/oxygen/64x64/apps/kbruch.png
    ${KDE4_ICON_DIR}/oxygen/128x128/apps/kbruch.png
)
kde4_add_win32_app_icon(kbruch_SRCS ${kbruch_icons})

kde4_add_executable(kbruch ${kbruch_SRCS})

target_link_libraries(kbruch  ${KDE4_KDEUI_LIBS} )

install(TARGETS kbruch  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kbruch.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kbruchui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kbruch )

kde4_install_icons( ${ICON_INSTALL_DIR}   )

