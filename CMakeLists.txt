project(kbruch)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.7.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
include(ECMInstallIcons)

find_package(Qt5 5.2.0 CONFIG REQUIRED
  Core
  Widgets
)

find_package(KF5 5.0.0 REQUIRED
  Config
  Crash
  DocTools
  I18n
  WidgetsAddons
  XmlGui
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(ECMAddAppIcon)

# use sane compile flags
add_definitions(
  -fexceptions
  -DQT_USE_QSTRINGBUILDER
  -DQT_NO_CAST_TO_ASCII
  -DQT_NO_CAST_FROM_ASCII
  -DQT_STRICT_ITERATORS
  -DQT_NO_URL_CAST_FROM_STRING
  -DQT_NO_CAST_FROM_BYTEARRAY
  -DQT_NO_SIGNALS_SLOTS_KEYWORDS
  -DQT_USE_FAST_OPERATOR_PLUS
)

add_subdirectory( doc )
add_subdirectory( src )

install(FILES org.kde.kbruch.appdata.xml DESTINATION ${KDE_INSTALL_DATAROOTDIR}/appdata/)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
