include_directories ( ${CMAKE_SOURCE_DIR}/kbruch/src )

set(kbruch_test_SRCS 
   ratio_test.cpp 
   primenumber_test.cpp 
   kbruch_test.cpp )

kde4_automoc(${kbruch_test_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(kbruch_test ${kbruch_test_SRCS})

target_link_libraries(kbruch_test  ${KDE4_KDECORE_LIBS} boost_unit_test_framework-gcc ${KDE4_KDEUI_LIBS} )

endif(KDE4_BUILD_TESTS)
