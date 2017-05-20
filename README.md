# FSM Editor

![alt tag](https://cloud.githubusercontent.com/assets/16545838/26177823/a74d1792-3b5b-11e7-8f3c-ccc77520c72e.png)

## Overview
FSM Editor is a small implementation of a graphical editor for Finite State Machines.

It can be customized to save and load FSM graphs to any format or language by overriding the Settings class. It can also be embedded in another Qt project.

The main widget and entry point is FSMEditor.

FSM Editor can run on any platform supported by Qt, including Windows, Unix and Mac OS.

## Usage
To create a state, double click on the scene. The code of the state appears and can be edited on the right side text area.

To link two states with a transition, click on the red arrow on the top right corner of the source state and drag it on top of the destination state. The condition of the transition can also be edited on the right side of the screen.

States can be renamed by pressing F2 or the rename button of the toolbar.

States and transitions can be removed by selecting them and pressing delete or the delete button of the toolbar.

Zoom In and Out can be performed by Ctrl + wheel or with dedicated toolbar actions .

When pressing the space bar or the mouse center button, the editor switches to scroll mode and it is possible to scroll the scene by dragging the mouse.

## Dependencies
- Qt 5.X
- CMake 3.X

## Resources
Icons are from Google Material Design pack, licensed under the Creative Common Attribution 4.0 International License (CC-BY 4.0).

## Qt4
If necessary, it should be possible to compile it with Qt4 by just adapting the CMake scpripts.

## Installation
See INSTALL for compilation instructions.

## License
See LICENSE for licensing information.
