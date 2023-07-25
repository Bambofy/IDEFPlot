#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <libavoid/libavoid.h>
#include <pugixml.hpp>

namespace TMModeller
{
	class FileCoordinate
	{
	public:
		uint32_t Row;
		uint32_t Column;
	};

	class Stub
	{
	public:
		std::string Name;
		FilePosition Position;
	};

	class UMLStub : public Stub
	{
	public:
	};

	class IDEFStub : public Stub
	{
	public:
		std::string Source;
	}:

	class IDEFStateStub : public IDEFStub
	{
	public:
	};

	class IDEFActivityStub : public IDEFStub
	{
	public:
	};

	class Box
	{
	public:
		std::string Name;
		FilePosition Center;
		uint32_t Width;
		uint32_t Height;
	};

	class UMLBox : public Box
	{
	public:
	};

	class UMLClassBox : public UMLBox
	{
	public:
	};

	class IDEFBox : public Box
	{
	public:
		std::string NodeNumber;
	};

	class IDEFActivityBox : public IDEFBox
	{
	public:
		std::vector<IDEFActivityStub> InputStubs;
		std::vector<IDEFActivityStub> OutputStubs;
		std::vector<IDEFActivityStub> ControlStubs;
		std::vector<IDEFActivityStub> MechanismStubs;
		std::vector<IDEFActivityStub> CallStubs;
	};

	class IDEFStateBox : public IDEFBox
	{
	public:
		std::vector<IDEFStateStub> InputStubs;
		std::vector<IDEFStateStub> OutputStubs;
	};

	class GlossaryEntry
	{
	public:
		std::string Title;
		std::string Description;
		std::string Type;
	};

	class IDEFDiagramBarSection
	{
	public:
		FilePosition TopLeft;
		std::string Content;
		uint32_t Width;
		uint32_t Height;
	};

	class IDEFNodeNumberSection : public IDEFDiagramBarSection
	{
	public:
	};

	class IDEFTitleSection : public IDEFDiagramBarSection
	{
	public:
	};

	class IDEFCNumberSection : public IDEFDiagramBarSection
	{
	public:
	};

	class IDEFDiagramBar
	{
	public:
		FilePosition TopLeft;
		uint32_t Height;
		IDEFNodeNumberSection NumberSection;
		IDEFTitleSection TitleSection;
		IDEFCNumberSection CNumberSection;
	};

	class IDEFDiagramFrame
	{
	public:
		IDEFDiagramBar Bar;
	};

	typedef std::vector<GlossaryEntry> Glossary;

	class Diagram
	{
	public:
		std::string Title;
		uint32_t Width;
		uint32_t Height;
	};

	class UMLDiagram : public Diagram
	{
	public:
	};

	class ClassDiagram : public UMLDiagram
	{
	public:
	};

	class IDEFDiagram : public Diagram
	{
	public:
		IDEFDiagramFrame Frame;
	};

	class IDEFStateDiagram : public IDEFDiagram
	{
	public:
		std::vector<IDEFStateBox> Boxes;
		std::vector<IDEFStateStub> BoundaryStubs;
	};

	class IDEFActivityDiagram : public IDEFDiagram
	{
	public:
		std::vector<IDEFActivityBox> Boxes;
		std::vector<IDEFActivityStub> BoundaryStubs;
	};

	class Model
	{
	public:
		std::string Title;
		Glossary Glossary;
		std::vector<ClassDiagram> ClassDiagrams;
		std::vector<IDEFStateDiagram> StateDiagrams;
		std::vector<IDEFActivityDiagram> ActivityDiagrams;
	};

	GlossaryEntry LoadGlossaryEntry(const pugixml::xml_node& TargetGlossaryEntryNode)
	{
		std::string EntryTitle;
		std::string EntryDescription;
		std::string EntryType;
		GlossaryEntry NewGlossaryEntry;

		EntryTitle = TargetGlossaryEntryNode.attrib("Title").as_string();
		EntryDescription = TargetGlossaryEntryNode.attrib("Description").as_string();
		EntryType = TargetGlossaryEntryNode.attrib("Type").as_string();
		NewGlossaryEntry.Title = EntryTitle;
		NewGlossaryEntry.Description = EntryDescription;
		NewGlossaryEntry.Type = EntryType;

		return NewGlossaryEntry;
	}

	Glossary LoadGlossary(const pugixml::xml_node& TargetGlossaryNode)
	{
		uint32_t NumEntryNodes;
		Glossary NewGlossary;

		NumEntryNodes = TargetGlossaryNode.children().size();
		for (uint32_t i = 0u; i < NumEntryNodes; i++)
		{
			pugixml::xml_node EntryNode;
			GlossaryEntry NewGlossaryEntry;

			EntryNode = TargetGlossaryNode.children()[i];
			NewGlossaryEntry = LoadGlossaryEntry(EntryNode);
			NewGlossary.Entries.push_back(NewGlossaryEntry);
		}

		return NewGlossary;
	}

	
	IDEFActivityStub LoadActivityStub(const pugixml::xml_node& InputXMLNode)
	{
		IDEFActivityStub NewActivityStub;

		NewActivityStub.Name = InputXMLNode.attrib("Name").as_string();
		NewActivityStub.Source = InputXMLNode.attrib("Source").as_string();
		NewActivityStub.FilePosition.Row = 0u;
		NewActivityStub.FilePosition.Column = 0u;	

		return NewActivityStub;
	}

	IDEFActivityBox LoadActivity(const pugixml::xml_node& ActivityNode)
	{
		IDEFActivityBox NewActivityBox;
		uint16_t NumStubs;

		NewActivityBox.Name = ActivityNode.attrib("Name").as_string();
		NewActivityBox.Width = 0u;
		NewActivityBox.Height = 0u;
		NewActivityBox.Position.Row = 0u;
		NewActivityBox.Position.Column = 0u;
		NumStubs = ActivityNode.children().size();
		for (uint16_t i = 0u; i < NumStubs; i++)
		{
			const pugixml::xml_node& XMLStub;

			XMLStub = ActivityNode.children()[i];
			if (XMLStub.tag() == "Input")
			{
				IDEFActivityStub NewInputStub;

				NewInputStub = LoadActivityStub(XMLStub);
				NewActivityBox.InputStubs.push_back(NewInputStub);
			}
			else if (XMLStub.tag() == "Output")
			{
				IDEFActivityStub NewOutputStub;

				NewOutputStub = LoadActivityStub(XMLStub);
				NewActivityBox.OutputStubs.push_back(NewOutputStub);		
			}
			else if (XMLStub.tag() == "Control")
			{
				IDEFActivityStub NewControlStub;

				NewControlStub = LoadActivityStub(XMLStub);
				NewActivityBox.ControlStubs.push_back(NewControlStub);
			}
			else if (XMLStub.tag() == "Mechanism")
			{
				IDEFActivityStub NewMechanismStub;

				NewMechanismStub = LoadActivityStub(XMLStub);
				NewActivityBox.MechanismStubs.push_back(NewMechanismStub);
			}
			else if (XMLStub.tag() == "Call")
			{
				IDEFActivityStub NewCallStub;

				NewCallStub = LoadActivityStub(XMLStub);
				NewActivityBox.CallStubs.push_back(NewCallStub);
			}
			else
			{
				throw std::runtime_error("Unknown activity stub kind");
			}
		}

		return NewActivity;
	}

	IDEFActivityDiagram LoadActivityDiagram(const pugixml::xml_node& ActivityDiagramNode)
	{
		IDEFActivityDiagram NewActivityDiagram;
		uint16_t NumChildNodes;

		NewActivityDiagram.Frame.Bar.NumberSection.Content = ActivityDiagramNode.attrib("Number").to_string();
		NewActivityDiagram.Frame.Bar.NumberSection.FilePosition.Row = 0u;
		NewActivityDiagram.Frame.Bar.NumberSection.FilePosition.Column = 0u;
		NewActivityDiagram.Frame.Bar.TitleSection.Content = ActivityDiagramNode.attrib("Title").to_string();
		NewActivityDiagram.Frame.Bar.TitleSection.FilePosition.Row = 0u;
		NewActivityDiagram.Frame.Bar.TitleSection.FilePosition.Column = 0u;
		NewActivityDiagram.Frame.Bar.CNumberSection.Content = ActivityDiagramNode.attrib("CNumber").to_string();
		NewActivityDiagram.Frame.Bar.CNumberSection.FilePosition.Row = 0u;
		NewActivityDiagram.Frame.Bar.CNumberSection.FilePosition.Column = 0u;
		NewActivityDiagram.Width = 0u;
		NewActivityDiagram.Height = 0u;
		NumChildNodes = ActivityDiagramNode.children().size();
		for (uint16_t i = 0u; i < NumChildNodes; i++)
		{
			const pugixml::xml_node& ChildXMLNode;

			ChildXMLNode = ActivityDiagramNode.children()[i];
			if (ChildXMLNode.tag() == "Input")
			{
				IDEFActivityStub NewInputStub;

				NewInputStub = LoadActivityStub(ChildXMLNode);
				NewActivityDiagram.BoundaryInputStubs.push_back(NewInputStub);
			}
			else if (ChildXMLNode.tag() == "Output")
			{
				IDEFActivityStub NewOutputStub;

				NewOutputStub = LoadActivityStub(ChildXMLNode);
				NewActivityDiagram.BoundaryOutputStubs.push_back(NewOutputStub);		
			}
			else if (ChildXMLNode.tag() == "Control")
			{
				IDEFActivityStub NewControlStub;

				NewControlStub = LoadActivityStub(ChildXMLNode);
				NewActivityDiagram.BoundaryControlStubs.push_back(NewControlStub);
			}
			else if (ChildXMLNode.tag() == "Mechanism")
			{
				IDEFActivityStub NewMechanismStub;

				NewMechanismStub = LoadActivityStub(ChildXMLNode);
				NewActivityDiagram.BoundaryMechanismStubs.push_back(NewMechanismStub);
			}
			else if (ChildXMLNode.tag() == "Call")
			{
				IDEFActivityStub NewCallStub;

				NewCallStub = LoadActivityStub(ChildXMLNode);
				NewActivityDiagram.BoundaryCallStubs.push_back(NewCallStub);
			}
			else if (ChildXMLNode.tag() == "Activity")
			{
				IDEFActivityBox NewActivityBox;

				NewActivityBox = LoadActivity(ChildXMLNode);
				NewActivityDiagram.Boxes.push_back(NewActivityBox);
			}
			else
			{
				throw std::runtime_error("Unknown xml activity diagram child node kind");
			}
		}

		return NewActivityDiagram;
	}

	Model LoadModel(const pugixml::xml_node& ModelNode)
	{
		Model NewModel;
		uint32_t NumChildNodes;

		NewModel.Title = ModelNode.attrib("Name");
		NumChildNodes = ModelNode.children().size();
		for (uint32t i = 0u; i < NumChildNodes; i++)
		{
			const pugixml::xml_node& ChildNode = ModelNode.children()[i];

			if (ChildNode.tag() == "ActivityDiagram")
			{
				IDEFActivityDiagram NewDiagram;

				NewDiagram = LoadACtivityDiagram(ChildNode);
				NewModel.ActivityDiagrams.push_back(NewDiagram);
			}
			else if (ChildNode.tag() == "ClassDiagram")
			{
				throw runtime_error("Class diagrams are not implemented yet.");
			}
			else if (ChildNode.tag() == "StateDiagram")
			{
				throw runtime_error("State diagrams are not implemented yet.");
			}
			else
			{
				throw std::runtime_error("Unknown diagram type");
			}
		}

		return NewModel;
	}

	std::vector<Model> LoadModelsFile(const std::string& FilePath)
	{
		pugixml::xml_document ModelsXMLDocument;
		pugixml::xml_node RootXMLNode;
		pugixml::xml_parse_result ParseResult;
		std::vector<Model> LoadedModels;
		uint32_t NumModelChildNodes;

		ParseResult = ModelsXMLDocument.load(FilePath.c_str());
		RootXMLNode = ModelsXMLDocument("Models");
		NumModelChildNodes = RootXMLNode.children().size();
		for (uint32_t i = 0u; i < NumModelChildNodes; i++)
		{
			const pugixml::xml_node& ChildXMLNode = RootXMLNode.children()[i];
			Model LoadedModel;

			LoadedModel = LoadModel(ChildXMLNode);
			LoadedModels.push_back(LoadedModel);
		}

		return LoadedModels
	}

	void LayoutFrame(IDEFActivityDiagram& Diagram)
	{
		static const uint32_t TopLeftRow = 0u;
		static const uint32_t TopLeftColumn = 0u;
		static const uint32_t BottomBarHeight = 3u;

		Diagram.Frame.BottomBar.TopLeft.Row = Diagram.Height-BottomBarHeight;
		Diagram.Frame.BottomBar.TopLeft.Column = TopLeftColumn;
		
		Diagram.Frame.BottomBar.NumberSection.Width = Diagram.Width / 3u;
		Diagram.Frame.BottomBar.NumberSection.TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
		Diagram.Frame.BottomBar.NumberSection.TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column;

		Diagram.Frame.BottomBar.TitleSection.Width = Diagram.Width / 3u;
		Diagram.Frame.BottomBar.TitleSection.TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
		Diagram.Frame.BottomBar.TitleSection.TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column + Diagram.Frame.BottomBar.TitleSection.Width;

		Diagram.Frame.BottomBar.CNumberSection.Width = Diagram.Width / 3u;
		Diagram.Frame.BottomBar.CNumberSection.TopLeft.Row = Diagram.Frame.BottomBar.Top.Row;
		Diagram.Frame.BottomBar.CNumberSection.TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column + Diagram.Frame.BottomBar.CNumberSection.Width;
	}

	void LayoutBoxes(IDEFActivityDiagram& Diagram)
	{
		static const uint32_t BottomBarHeight = 3u;
		uint32_t NumBoxes;
		uint32_t ColumnWidth;
		uint32_t RowHeight;

		/* 1. Calculate the boxes centers. */
		/* 1.1. Calculates the number of boxes. */
		NumBoxes = Diagram.Boxes.size();	
		/* 1.2. Calculates the column width and row height for the staircase layout. */
		ColumnWidth = Diagram.Width / NumBoxes;
		RowHeight = (Diagram.Height - BottomBarHeight) / NumBoxes;
		/* 1.3. Calculates the centers for each box. */
		for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
		{
			IDEFActivityBox& SelectedBox;
			uint32_t ColumnOffset;
			uint32_t RowOffset;
			
			ColumnPadding = ColumnWidth / 2u;
			RowPadding = RowWidth / 2u;
			SelectedBox.Center.Column = ColumnPadding + (ColumnWidth*BoxIndex);
			SelectedBox.Center.Row = RowPadding + (RowHeight*BoxIndex);
		}
	}

	void LayoutBoxStubs(IDEFActivityDiagram& Diagram)
	{
		uint32_t NumBoxes;

		/* 1. Selects each box. */
		/* 1.1. Selects each interface of the box. */
		/* 1.2. Calculate the number of stubs on the interface */
		/* 1.3. Adds 1 to that number. */
		/* 1.4. Selects each stub on the interface. */
		/* 1.5. Calculates the offset from the interfaces starting position. */
		/* 1.6. Positions the stub. */
		NumBoxes = Diagram.Boxes.size();
		for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
		{
			IDEFActivityBox& SelectedBox = Diagram.Boxes[BoxIndex];
			uint8_t NumInputStubs;
			uint8_t NumOutputStubs;
			uint8_t NumControlStubs;
			uint8_t NumMechanismStubs;
			uint8_t NumCallStubs;
			uint8_t InputInterfaceDivisions;
			uint8_t InputInterfaceDivisionWidth;
			uint8_t OutputInterfaceDivisions;
			uint8_t OutputInterfaceDivisionWidth;		
			uint8_t ControlInterfaceDivisions;
			uint8_t ControlInterfaceDivisionWidth;
			uint8_t MechanismInterfaceDivisions;
			uint8_t MechanismInterfaceDivisionWidth;
			uint8_t CallInterfaceDivisions;
			uint8_t CallInterfaceDivisionWidth;

			NumInputStubs = SelectedBox.InputStubs.size();
			NumOutputStubs = SelectedBox.OutputStubs.size();
			NumControlStubs = SelectedBox.ControlStubs.size();
			NumMechanismStubs = SelectedBox.MechanismStubs.size();
			NumCallStubs = SelectedBox.CallStubs.size();
			StartingPosition.Column = SelectedBox.Center - (SelectedBox.Width / 2u);
			StartingPosition.Row = SelectedBox.Center - (SelectedBox.Height / 2u);
			InputInterfaceDivisions = NumInputStubs + 1u;
			InputInterfaceDivisionWidth = SelectedBox.Height / InputInterfaceDivisions;
			for (uint32_t InputStubIndex = 0u; InputStubIndex < NumInputStubs; InputStubIndex++)
			{
				IDEFStub& SelectedInputStub = SelectedBox.InputStubs[InputStubIndex];
				uint32_t RowOffset;

				RowOffset = InputInterfaceDivisionWidth * (1u + InputStubIndex);
				SelectedInputStub.Position.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
				SelectedInputStub.Position.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
				SelectedInputStub.Position.Row = SelectedInputStub.Position.Row + RowOffset;
			}
			OutputInterfaceDivisons = NumOutputstubs + 1u;
			OutputInterfaceDivisionWidth = SelectedBox.Height / OutputInterfaceDivisions;
			for (uint32_t OutputStubIndex = 0u; OutputStubindex < NumOutputStubs; OutputStubIndex++)
			{
				IDEFStub& SelectedOutputStub = SelectedBox.OutputStubs[OutputStubIndex];
				uint32_t RowOffset;

				RowOffset = OutputInterfaceDivisionWidth * (1u + OutputStubIndex);
				SelectedOutputStub.Position.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
				SelectedOutputStub.Position.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
				SelectedOutputStub.Position.Row = SelectedOutputStub.Position.Row + RowOffset;
			}
			ControlInterfaceDivisons = NumControlstubs + 1u;
			ControlInterfaceDivisionWidth = SelectedBox.Width / ControlInterfaceDivisions;
			for (uint32_t ControlStubIndex = 0u; ControlStubindex < NumControlStubs; ControlStubIndex++)
			{
				IDEFStub& SelectedControlStub = SelectedBox.ControlStubs[ControlStubIndex];
				uint32_t ColumnOffset;

				ColumnOffset = ControlInterfaceDivisionWidth * (1u + ControlStubIndex);
				SelectedControlStub.Position.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
				SelectedControlStub.Position.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
				SelectedControlStub.Position.Column = SelectedControlStub.Position.Column + ColumnOffset;
			}
			MechanismInterfaceDivisons = NumMechanismstubs + 1u;
			MechanismInterfaceDivisionWidth = (SelectedBox.Width/2u) / MechanismInterfaceDivisions;
			for (uint32_t MechanismStubIndex = 0u; MechanismStubindex < NumMechanismStubs; MechanismStubIndex++)
			{
				IDEFStub& SelectedMechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
				uint32_t ColumnOffset;

				ColumnOffset = MechanismInterfaceDivisionWidth * (1u + MechanismStubIndex);
				SelectedMechanismStub.Position.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
				SelectedMechanismStub.Position.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
				SelectedMechanismStub.Position.Column = SelectedMechanismStub.Position.Row + ColumnOffset;
			}
			CallInterfaceDivisons = NumCallstubs + 1u;
			CallInterfaceDivisionWidth = (SelectedBox.Width/2) / CallInterfaceDivisions;
			for (uint32_t CallStubIndex = 0u; CallStubindex < NumCallStubs; CallStubIndex++)
			{
				IDEFStub& SelectedCallStub = SelectedBox.CallStubs[CallStubIndex];
				uint32_t ColumnOffset;

				ColumnOffset = CallInterfaceDivisionWidth * (1u + CallStubIndex);
				SelectedCallStub.Position.Column = SelectedBox.Center.Column;
				SelectedCallStub.Position.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
				SelectedCallStub.Position.Column = SelectedCallStub.Position.Column + ColumnOffset;
			}
		}

	}

	void LayoutBoundaryStubs(IDEFActivityDiagram& Diagram)
	{
		/* 2. Lays out the stubs on the boundary */
		/* 2.1. Selects each interface. */
		/* 2.2. Calculates the number of divisions. */
		/* 2.3. Calculates the width of of a division. */
		/* 2.4. Selects each stub. */
		/* 2.5. Places the stub. */
		uint8_t NumInputDivisions;
		uint8_t NumOutputDivisions;
		uint8_t NumControlDivisions;
		uint8_t NumMechanismDivisions;
		uint8_t NumCallDivisions;
		uint8_t InputDivisionWidth;
		uint8_t ControlDivisionWidth;
		uint8_t OutputDivisionWidth;
		uint8_t MechanismDivisionWidth;
		uint8_t CallDivisionWidth;
		uint8_t NumInputStubs;
		uint8_t NumOutputStubs;
		uint8_t NumControlStubs;
		uint8_t NumMechanismStubs;
		uint8_t NumCallStubs;

		NumInputStubs = Diagram.BoundaryInputStubs.size();
		NumOutputStubs = Diagram.BoundaryOutputStubs.size();
		NumControlStubs = Diagram.BoundaryControlStubs.size();
		NumMechanismStubs = Diagram.BoundaryMechanismStubs.size();
		NumCallStubs = Diagram.BoundaryCallStubs.size();
		NumInputDivisions = NumInputStubs + 1u;
		NumOutputDivisions = NumOutputStubs + 1u;
		NumControlDivisions = NumControlStubs + 1u;
		NumMechanismDivisions = NumMechanismStubs + 1u;
		NumCallDivisions = NumCallStubs + 1u;
		InputDivisionWidth = Diagram.Height / NumInputDivisions;	
		ControlDivisionWidth = Diagram.Width / NumControlDivisions;
		OutputDivisionWidth = Diagram.Height / NumOutputDivisions;
		MechanismDivisionWidth = (Diagram.Width / 2u) / NumMechanismDivisions;
		CallDivisionWidth = (Diagram.Width / 2u) / NumCallDivisions;
		for (uint8_t InputStubIndex = 0u; InputStubIndex < NumInputStubs; InputStubIndex++)
		{
			IDEFStub& SelectedStub = Diagram.BoundaryInputStubs[InputStubIndex];
			uint8_t RowOffset;

			RowOffset = (1 + InputStubIndex) * InputDivisionWidth;
			SelectedStub.Position.Row = 0u;
			SelectedStub.Position.Column = 0u;
			SelectedStub.Position.Row = SelectedStub.Position.Row + RowOffset;
		}
		for (uint8_t OutputStubIndex = 0u; OutputStubIndex < NumOutputStubs; OutputStubIndex++)
		{
			IDEFStub& SelectedStub = Diagram.BoundaryOutputStubs[OutputStubIndex];
			uint8_t RowOffset;

			RowOffset = (1 + OutputStubIndex) * OutputDivisionWidth;
			SelectedStub.Position.Row = 0u;
			SelectedStub.Position.Column = Diagram.Width;
			SelectedStub.Position.Row = SelectedStub.Position.Row + RowOffset;
		}
		for (uint8_t ControlStubIndex = 0u; ControlStubIndex < NumControlStubs; ControlStubIndex++)
		{
			IDEFStub& SelectedStub = Diagram.BoundaryControlStubs[ControlStubIndex];
			uint8_t ColumnOffset;

			ColumnOffset = (1 + ControlStubIndex) * ControlDivisionWidth;
			SelectedStub.Position.Row = 0u;
			SelectedStub.Position.Column = 0u;
			SelectedStub.Position.Column = SelectedStub.Position.Column + ColumnOffset;
		}
		for (uint8_t MechanismStubIndex = 0u; MechanismStubIndex < NumMechanismStubs; MechanismStubIndex++)
		{
			IDEFStub& SelectedStub = Diagram.BoundaryMechanismStubs[MechanismStubIndex];
			uint8_t ColumnOffset;

			ColumnOffset = (1 + MechanismStubIndex) * MechanismDivisionWidth;
			SelectedStub.Position.Row = Diagram.Height;
			SelectedStub.Position.Column = 0u;
			SelectedStub.Position.Column = SelectedStub.Position.Column + ColumnOffset;
		}
		for (uint8_t CallStubIndex = 0u; CallStubIndex < NumCallStubs; CallStubIndex++)
		{
			IDEFStub& SelectedStub = Diagram.BoundaryCallStubs[CallStubIndex];
			uint8_t RowOffset;

			RowOffset = (1 + CallStubIndex) * CallDivisionWidth;
			SelectedStub.Position.Row = 0u;
			SelectedStub.Position.Column = Diagram.Width / 2u;
			SelectedStub.Position.Column = SelectedStub.Position.Column + ColumnOffset;
		}
	}

	void LayoutDiagram(IDEFActivityDiagram& LoadedDiagram, uint32_t Width, uint32_t Height)
	{
		LoadedDiagram.Width = Width;
		LoadedDiagram.Height = Height;
		LayoutFrame(LoadedDiagram);
		LayoutBoxes(LoadedDiagram);
		LayoutBoxStubs(LoadedDiagram);
		LayoutBoundaryStubs(LoadedDiagram);
	}

	libavoid::ConnEnd PlaceConnectors(IDEFActivityDiagram& LayedoutDiagram)
	{
	}
}

int main(int argc, char** argv)
{
	return 0;
}
