add_subdirectory( pics ) 

########### next target ###############

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KBRUCH VERSION_HEADER kbruch_version.h)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(kbruch_SRCS
   kbruch.qrc
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
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::CoreAddons
    KF6::Crash
    KF6::I18n
    KF6::WidgetsAddons
    KF6::XmlGui
)

install(TARGETS kbruch  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.kbruch.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES kbruch.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )

ecm_install_icons( ICONS 16-apps-kbruch.png 22-apps-kbruch.png 32-apps-kbruch.png 48-apps-kbruch.png 64-apps-kbruch.png 128-apps-kbruch.png sc-apps-kbruch.svgz DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor )
