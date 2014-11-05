project(kbruch)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.3.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
include(ECMInstallIcons)

find_package(Qt5 5.2.0 CONFIG REQUIRED Core Widgets)
find_package(KF5 5.0.0 REQUIRED Config DocTools I18n IconThemes WidgetsAddons XmlGui)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory( doc )
add_subdirectory( src )

install(FILES kbruch.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata/)
