qt_add_executable(MultiLine
    tst_multiline.h tst_multiline.cpp
)

add_test(NAME MultiLine COMMAND MultiLine)

target_include_directories(MultiLine
    PUBLIC
    ../..
    ../../widgets
    ../../core
)
target_link_libraries(MultiLine PRIVATE
    Qt::Core
    Qt::Test
    GraphView
)
