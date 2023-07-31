# IDEFPlot

IDEFPlot is a C++ program that converts an XML specification of IDEF models
into ascii diagrams. It performs routing automatically from the details of the
XML tags. It may not be 100% bug-free and feature complete but hopefully it will be of some use.

Made by Richard Bamford 2023

github.com/Bambofy/IDEFPlot

## Building
The operating system used to develop this program was Debian GNU/Linux 12 (bookworm) however the code is platform independent as far as i am aware. The compiler used to was GNU gcc version 12.2.0 (Debian 12.2.0-14) and the target architecture is "x86_64-linux-gnu".

Steps to compile:
    1. Open the root folder with a terminal.
    2. Run 'make'.

Notices:
    1. If the compiler complains about missing "basic_string" or other C++ features then you might have missed out specifiying the compile when running make. The correct make command is "make IDEFPlotTarget CC=g++".

## XML Specification
The XML specification describes a complete IDEF0 functional model. Each element of the specification represents different parts of the actual diagram elements for example; `<Activity>` `<Input>`.

## Credits
1. Adaptagram's libavoid - For the automatic arrow routing features.
2. pugixml - For the XML parsing features.
3. SvgBob - For the ASCII characters to use for the graphics.
