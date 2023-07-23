#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <libavoid/libavoid.h>
#include <pugixml.hpp>

namespace Math
{
	class Point
	{
	public:
		float X;
		float Y;
	};
}

namespace IDEF
{
	class Stub
	{
	public:
		std::string Label;
		Math::Point Position;
	};

	class Input : public Stub
	{
	public:
	}

	class Output : public Stub
	{
	public:
	};

	class Control : public Stub
	{
	public:
	};

	class Mechanism : public Stub
	{
	public:
	};

	class Call : public Stub
	{
	public:
	}:

	class Connection
	{
	public:
		std::string Source;
		std::vector<std::string> Destinations;
	};

	class Box
	{
	public:
		std::string Number;
		std::string Title;
		std::vector<Input> Inputs;
		std::vector<Output> Outputs;
		std::vector<Control> Controls;
		std::vector<Mechanism> Mechanisms;
		std::vector<Call> Calls;
		std::string Child;
		Math::Point Position;
		float Width;
		float Height;
	};

	class Diagram
	{
	public:
		std::string Number;
		std::string Title;
		std::string CNumber;
		std::vector<Box> Boxes;
		std::vector<Connection> Connections;
		std::vector<Input> Inputs;
		std::vector<Output> Outputs;
		std::vector<Control> Controls;
		std::vector<Mechanism> Mechanisms;
		std::vector<Call> Calls;
	};

	Diagram LoadDiagram(const std::string& FilePath)
	{
	}

	void LayoutDiagram();

	void DrawDiagram();
}

int main(int argc, char** argv)
{
	return 0;
}
