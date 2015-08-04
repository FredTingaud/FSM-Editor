set(dummy_SRCS
    DummySettings.cpp
    DummySettings.h
    dummy/DummyReader.cpp
    dummy/DummyReader.h
    dummy/DummyState.h
    dummy/DummyTransition.h
    dummy/DummyVisitor.cpp
    dummy/DummyVisitor.h
    dummy/DummyWriter.cpp
    dummy/DummyWriter.h
    )

add_library(dummy-settings ${dummy_SRCS})

target_link_libraries(dummy-settings
    Qt5::Widgets
    Qt5::Gui
    )
