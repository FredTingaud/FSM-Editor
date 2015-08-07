project(FSMEditor)

cmake_minimum_required(VERSION 3.0.0)

cmake_policy(SET CMP0003 NEW)

find_package(Qt5Widgets REQUIRED HINTS C:/Qt/Qt5.3.1/5.3/msvc2013_64/lib/cmake/Qt5Widgets)

ADD_DEFINITIONS(-DQT_DLL)
if(WIN32)
  set(QT_USE_QTMAIN TRUE)
  set(QT_USE_QTMAIN TRUE)

  add_definitions(-DWINVER=0x0501)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

if (UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif (UNIX)

set(CMAKE_AUTOMOC ON)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(allSRCS
    ExportVisitor.h
    FSMEditor.cpp
    FSMEditor.h
    FSMScene.cpp
    FSMScene.h
    Settings.cpp
    Settings.h
    fsm-elements/FSMElement.cpp
    fsm-elements/FSMElement.h
    fsm-elements/State.cpp
    fsm-elements/State.h
    fsm-elements/Transition.cpp
    fsm-elements/Transition.h
    io/FSMReader.h
    io/FSMWriter.h
    model/Graph.h
    model/GraphElement.h
    model/GraphState.h
    model/GraphStateImpl.h
    model/GraphTransition.h
    model/GraphTransitionImpl.h
    undo/AddStateCommand.cpp
    undo/AddStateCommand.h
    undo/AddTransition.cpp
    undo/AddTransition.h
    undo/DeleteStateCommand.cpp
    undo/DeleteStateCommand.h
    undo/DeleteTransition.cpp
    undo/DeleteTransition.h
    undo/MoveStateCommand.cpp
    undo/MoveStateCommand.h
    undo/RenameState.cpp
    undo/RenameState.h
    undo/StartStateCommand.cpp
    undo/StartStateCommand.h
    undo/UpdateCode.cpp
    undo/UpdateCode.h
    )

set(linkLibraries
    Qt5::Widgets
    Qt5::Gui
    )

source_group("elements" REGULAR_EXPRESSION fsm-elements/*)
source_group("io" REGULAR_EXPRESSION fsm-elements/*)

add_library(FSM-lib ${allSRCS})
target_link_libraries(FSM-lib ${linkLibraries})

add_subdirectory(settings)

set(mainSRCS
    main.cpp
    )

set(mainLibraries
    FSM-lib
    )

add_executable(EditorFSM ${mainSRCS})
target_link_libraries(EditorFSM ${mainLibraries} dummy-settings)
