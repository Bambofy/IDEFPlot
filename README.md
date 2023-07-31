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
2. Run 'make PugiXMLTarget CC=g++'.
3. Run 'make LibAvoidTarget CC=g++'.
4. Run 'make IDEFPlotTarget CC=g++'.

Notices:
1. If the compiler complains about missing "basic_string" or other C++ features then you might have missed out specifiying the compile when running make. The correct make command is "make IDEFPlotTarget CC=g++".

## Contributing
Please feel free to make PRs and raise issues however new features are not welcome at this time.

The main design of this program is a single C++ file. In time this file will be divided into more files but the priority at
the moment is to have a program that can be compiled from an archive. The programming style is C styled C++ without classes
and using std::variants instead inheritance. Dynamic memory is not used at all and is deferred to the STL containers to
handle.
The program is divided into around 4 main functional groups; loading the diagram's XML into C++ objects, laying out the
diagram onto a file, routing connections between the boxes and drawing the elements. 

## Using
IDEF0 is a complex functional modelling tool and the XML specification is a description of the IDEF0 diagrams.

## XML Specification
The XML specification describes a complete IDEF0 functional model. Each element of the specification represents different parts of the actual diagram elements for example; `<Activity>` `<Input>`.

## Credits
1. Adaptagram's libavoid - For the automatic arrow routing features.
2. pugixml - For the XML parsing features.
3. SvgBob - For the ASCII characters to use for the graphics.
