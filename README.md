# Wizard's Chess

A Windows C++/CLI prototype connecting a console chess game to a physical board controller over a serial port. Chess moves become grid movements and magnet commands for an Arduino-facing motion system.

## Watch the board

[Watch the original Wizard’s Chess demonstration](https://www.youtube.com/watch?v=A76TZXOia7I) · June 2015 · 2:26

## Explore the design

- [BackEndChessGame.cpp](Wizard%27s%20Chess/BackEndChessGame.cpp) defines the board, pieces, move validation, and console game loop.
- [MotorConnector.cpp](Wizard%27s%20Chess/MotorConnector.cpp) tracks occupied and discard spaces, converts chess squares into a motion grid, and writes movement commands through `System.IO.Ports.SerialPort`.
- [MotorConnector.h](Wizard%27s%20Chess/MotorConnector.h) defines the controller interface.

The controller uses a **9600 baud** serial connection. Each movement command contains a direction/magnet-state digit followed by two-digit X and Y magnitudes. The code moves a captured piece to a discard location before moving the attacking piece.

## Build prerequisites

The checked-in [Visual C++ project](Wizard%27s%20Chess/BackEndChessGame.vcxproj) targets:

- Windows, Win32 configuration
- Visual C++ `v120` toolset (Visual Studio 2013)
- C++/CLI with .NET Framework 4.5

Open `Wizard's Chess/BackEndChessGame.vcxproj` in a compatible Visual Studio installation. A newer installation may require retargeting the compiler and framework. Select Debug or Release and build the project; modern-toolchain compatibility has not been verified.

## Hardware integration

At startup, the controller asks for a serial port name such as `COM3`. The source expects compatible firmware and a physical motion/magnet assembly. Arduino firmware, wiring diagrams, a parts list, and calibration instructions are not included, so this repository is a source archive rather than a complete hardware reproduction guide.

## Project status

The repository preserves the original prototype and its generated Visual Studio `Debug` files. Those generated files are historical artifacts, not a verified release. The chess rules and motion planning should be reviewed before adapting the design; no automated tests or current hardware validation are recorded here.

## License and provenance

No license or detailed source provenance is recorded in this repository. Reuse permissions and any external chess-code origins need to be documented before offering it as an open-source package.
