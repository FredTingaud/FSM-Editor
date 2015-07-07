project(FSMEditor)

cmake_minimum_required(VERSION 3.0.0)

cmake_policy(SET CMP0003 NEW)

find_package(Qt5Widgets REQUIRED HINTS C:/Qt/Qt5.3.1/5.3/msvc2013_64/lib/cmake/Qt5Widgets)

ADD_DEFINITIONS(-DQT_DLL)
if(WIN32)
  set(QT_USE_QTMAIN TRUE)
  set(QT_USE_QTMAIN TRUE)
endif(WIN32)

get_target_property(QtCore_location Qt5::Core LOCATION)
get_filename_component(QT_BIN ${QtCore_location} DIRECTORY)

if(WIN32)
  add_custom_target(deployQT set "PATH=%PATH%;${QT_BIN}"
                    COMMAND
                    ${QT_BIN}/windeployqt ${CMAKE_BINARY_DIR}/debug/EditorFSM.exe
                    )

  add_definitions(-DWINVER=0x0501)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

if (UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif (UNIX)

set(CMAKE_AUTOMOC ON)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(allSRCS
    FSMEditor.cpp
    FSMEditor.h
    FSMScene.cpp
    FSMScene.h
    fsm-elements/State.cpp
    fsm-elements/State.h
    fsm-elements/Transition.cpp
    fsm-elements/Transition.h
    undo/AddStateCommand.cpp
    undo/AddStateCommand.h
    undo/ChangeTransition.cpp
    undo/ChangeTransition.h
    undo/DeleteStateCommand.cpp
    undo/DeleteStateCommand.h
    undo/MoveStateCommand.cpp
    undo/MoveStateCommand.h
    )

set(linkLibraries
    Qt5::Widgets
    Qt5::Gui
    )

source_group("elements" REGULAR_EXPRESSION fsm-elements/*)

add_library(FSM-lib ${allSRCS})
target_link_libraries(FSM-lib ${linkLibraries})

set(mainSRCS
    main.cpp
    )

set(mainLibraries
    FSM-lib
    )

add_executable(EditorFSM ${mainSRCS})
target_link_libraries(EditorFSM ${mainLibraries})
