project(kbruch)

cmake_minimum_required(VERSION 2.8.12)
set(QT_MIN_VERSION "5.9.0")
set(KF5_MIN_VERSION "5.46.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
include(ECMInstallIcons)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
  Core
  Widgets
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED
  Config
  Crash
  DocTools
  I18n
  WidgetsAddons
  XmlGui
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(ECMAddAppIcon)

# use sane compile flags
add_definitions(
  -fexceptions
  -DQT_NO_URL_CAST_FROM_STRING
)

add_subdirectory( doc )
add_subdirectory( src )

install(FILES org.kde.kbruch.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
