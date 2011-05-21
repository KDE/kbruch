project(kbruch)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${KDE4_INCLUDES} ${QT4_INCLUDES})

add_subdirectory( doc )
add_subdirectory( src )
