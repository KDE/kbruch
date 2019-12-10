add_subdirectory( pics ) 

########### next target ###############

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KBRUCH VERSION_HEADER kbruch_version.h)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(kbruch_SRCS
   PrimeFactorsLineEdit.cpp
   ExerciseBase.cpp
   Task.cpp
   Ratio.cpp
   TaskView.cpp
   StatisticsView.cpp
   PrimeNumber.cpp
   KBruch.cpp
   MainQtWidget.cpp
   TaskWidget.cpp
   FractionBaseWidget.cpp
   ResultWidget.cpp
   ExerciseCompare.cpp
   ExercisePercentage.cpp
   ExerciseMixedNumbers.cpp
   RatioWidget.cpp
   RationalWidget.cpp
   ExerciseConvert.cpp
   StatisticsBarWidget.cpp
   ExerciseFactorize.cpp
   AppMenuWidget.cpp
   FractionPainter.cpp
   FractionRingWidget.cpp )

ki18n_wrap_ui(kbruch_SRCS taskcolorsbase.ui taskfontsbase.ui)

kconfig_add_kcfg_files(kbruch_SRCS settingsclass.kcfgc )

add_executable(kbruch ${kbruch_SRCS})

target_link_libraries(kbruch
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::Crash
    KF5::I18n
    KF5::WidgetsAddons
    KF5::XmlGui
)

install(TARGETS kbruch  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.kbruch.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
install( FILES kbruchui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kbruch )
install( FILES AppMenuWidgetui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kbruch )
install( FILES FractionRingWidgetui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kbruch )

ecm_install_icons( ICONS 16-apps-kbruch.png 22-apps-kbruch.png 32-apps-kbruch.png 48-apps-kbruch.png 64-apps-kbruch.png sc-apps-kbruch.svgz DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor )
