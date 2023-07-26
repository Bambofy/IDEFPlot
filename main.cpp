#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <map>
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

    class StubSource
    {
    public:
        std::string StubName;
    };

	class Stub
	{
	public:
		std::string Name;
		FilePosition Position;
        std::vector<Source> Sources;
	};

	class UMLStub : public Stub
	{
	public:
	};

	class IDEFStub : public Stub
	{
	public:
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
        uint32_t NumStubSources;

		NewActivityStub.Name = InputXMLNode.attrib("Name").as_string();
        NumStubSources = InputXMLNode.children().size();
        for (uint32_t StubSourceIndex = 0u; StubSourceIndex < NumStubSources; StubSourceIndex++)
        {
            const pugixml::xml_node& StubSourceXMLNode = InputXMLNode.children()[StubSourceIndex];
            StubSource NewStubSource;
            
            NewStubSource.Name = StubSourceXMLNode.attrib("Name").as_string();
            NewActivityStub.Sources.push_back(NewStubSource);
        }
		NewActivityStub.Interface = InputXMLNode.attrib("Interface").as_string();
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

		NumBoxes = Diagram.Boxes.size();	
		ColumnWidth = Diagram.Width / NumBoxes;
		RowHeight = (Diagram.Height - BottomBarHeight) / NumBoxes;
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

    /*
     * Places the libavoid connection ends at each stub on the activity boxes.
     *
     * Functional Breakdown:
     *     1. Creates a map from stub labels to connection ends.
     *     2. Iterates each box on the diagram.
     *     3. Creates connection ends for each stub on the boxes.
     *     4. Adds connection ends to the map along with the stub labels.
     * Parameters:
     *     1. LayedoutDiagram - A layed out IDEF activity diagram.
     * Returns:
     *     1. A map of stub labels to libavoid connection ends.
     */
    std::map<IDEFStub, Avoid::ConnEnd> PlaceBoxStubConnectionEnds(const IDEFActivityDiagram& LayedoutDiagram)
    {
        std::map<std::string, Avoid::ConnEnd> InterfaceStubsMap;
        uint32_t NumActivityBoxes;

        NumActivityBoxes = LayedoutDiagram.Boxes.size();
        for (uint32_t ActivityBoxIndex = 0u; ActivityBoxIndex < NumActivityBoxes; ActivityBoxIndex++)
        {
                const IDEFActivityBox& SelectedBox = LayedoutDiagram.Boxes[ActivityBoxIndex];
                uint32_t InputInterfaceStubCount;
                uint32_t ControlInterfaceStubCount;
                uint32_t OutputInterfaceStubCount;
                uint32_t MechanismInterfaceStubCount;
                uint32_t CallInterfaceStubCount;
                
                InputInterfaceStubCount = SelectedBox.InputStubs.size();
                ControlInterfaceStubCount = SelectedBox.ControlStubs.size();
                OutputInterfaceStubCount = SelectedBox.OutputStubs.size();
                MechanismInterfaceStubCount = SelectedBox.MechanismStubs.size();
                CallInterfaceStubCount = SelectedBox.CallStubs.size();
                for (uint32_t InputStubIndex = 0u; InputStubIndex < InputInterfacesStubCount; InputStubIndex++)
                {
                    const IDEFStub& InterfaceInputStub = SelectedBox.InputStubs[InputStubIndex];
                    Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceInputStub.Position.Column, InterfaceInputStub.Position.Row)); 

                    InterfaceStubsMap.insert(InterfaceInputStub, ConnectionEnd);
                }
                for (uint32_t OutputStubIndex = 0u; OutputStubIndex < OutputInterfacesStubCount; OutputStubIndex++)
                {
                    const IDEFStub& InterfaceOutputStub = SelectedBox.OutputStubs[OutputStubIndex];
                    Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceOutputStub.Position.Column, InterfaceOutputStub.Position.Row)); 

                    InterfaceStubsMap.insert(InterfaceOutputstub, ConnectionEnd);
                }
                for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlInterfacesStubCount; ControlStubIndex++)
                {
                    const IDEFStub& InterfaceControlStub = SelectedBox.ControlStubs[ControlStubIndex];
                    Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceControlStub.Position.Column, InterfaceControlStub.Position.Row)); 

                    InterfaceStubsMap.insert(InterfaceControlStub, ConnectionEnd);
                }
                for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismInterfacesStubCount; MechanismStubIndex++)
                {
                    const IDEFStub& InterfaceMechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
                    Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceMechanismStub.Position.Column, InterfaceMechanismStub.Position.Row)); 

                    InterfaceStubsMap.insert(InterfaceMechanismStub, ConnectionEnd);
                }
                for (uint32_t CallStubIndex = 0u; CallStubIndex < CallInterfacesStubCount; CallStubIndex++)
                {
                    const IDEFStub& InterfaceCallStub = SelectedBox.CallStubs[CallStubIndex];
                    Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceCallStub.Position.Column, InterfaceCallStub.Position.Row)); 

                    InterfaceStubsMap.insert(InterfaceCallStub, ConnectionEnd)
                }
        }

        return InterfaceStubsMap;
 
    }

    /*
     * Places libavoid stubs at each parent interface stub.
     *
     * Functional Breakdown:
     *     1. Creates a map from parent stub labels to connection ends.
     *     2. Iterates each parent interface stub.
     *     3. Creates connection ends for each parent interface stub;
     *     4. Adds the connection end and parent interface stub label to the map.
     * Parameters:
     *     1. LayedOutDiagram - A layed-out IDEF activity diagram.
     * Local Variables:
     *     1. InterfaceStubsMap - A mapping from IDEF activity stub label contents to libavoid connection ends.
     *     2. InputInterfaceStubCount - The number of input stubs on the parent input interface.
     *     3. ControlInterfaceStubCount - The number of control stubs on the parent control interface.
     *     4. OutputInterfaceStubCount - The number of output stubs on the parent output interface.
     *     5. MechanismInterfaceStubCount - The number of mechanism stubs on the parent mechanism interface.
     * Returns: 
     *     1. A map of parent interface stub labels to libavoid connection ends.
     */
    std::map<IDEFStub, Avoid::ConnEnd> PlaceBoundaryStubConnectionEnds(const IDEFActivityDiagram& LayedoutDiagram)
    {
        std::map<std::string, Avoid::ConnEnd> InterfaceStubsMap;
        uint32_t InputInterfaceStubCount;
        uint32_t ControlInterfaceStubCount;
        uint32_t OutputInterfaceStubCount;
        uint32_t MechanismInterfaceStubCount;
        uint32_t CallInterfaceStubCount;
        
        InputInterfaceStubCount = LayedoutDiagram.InputInterfaceStubs.size();
        ControlInterfaceStubCount = LayedoutDiagram.ControlInterfaceStubs.size();
        OutputInterfaceStubCount = LayedoutDiagram.OutputInterfaceStubs.size();
        MechanismInterfaceStubCount = LayedoutDiagram.MechanismInterfaceStubs.size();
        CallInterfaceStubCount = LayedoutDiagram.CallInterfaceStubs.size();
        for (uint32_t InputStubIndex = 0u; InputStubIndex < InputInterfacesStubCount; InputStubIndex++)
        {
            const IDEFStub& InterfaceInputStub = LayedoutDiagram.InputInterfaceStubs[InputStubIndex];
            Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceInputStub.Position.Column, InterfaceInputStub.Position.Row)); 

            InterfaceStubsMap.insert(InterfaceInputStub, ConnectionEnd);
        }
        for (uint32_t OutputStubIndex = 0u; OutputStubIndex < OutputInterfacesStubCount; OutputStubIndex++)
        {
            const IDEFStub& InterfaceOutputStub = LayedoutDiagram.OutputInterfaceStubs[OutputStubIndex];
            Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceOutputStub.Position.Column, InterfaceOutputStub.Position.Row)); 

            InterfaceStubsMap.insert(InterfaceOutputStub, ConnectionEnd);
        }
        for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlInterfacesStubCount; ControlStubIndex++)
        {
            const IDEFStub& InterfaceControlStub = LayedoutDiagram.ControlInterfaceStubs[ControlStubIndex];
            Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceControlStub.Position.Column, InterfaceControlStub.Position.Row)); 

            InterfaceStubsMap.insert(InterfaceControlStub, ConnectionEnd);
        }
        for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismInterfacesStubCount; MechanismStubIndex++)
        {
            const IDEFStub& InterfaceMechanismStub = LayedoutDiagram.MechanismInterfaceStubs[MechanismStubIndex];
            Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceMechanismStub.Position.Column, InterfaceMechanismStub.Position.Row)); 

            InterfaceStubsMap.insert(InterfaceMechanismStub, ConnectionEnd);
        }
        for (uint32_t CallStubIndex = 0u; CallStubIndex < CallInterfacesStubCount; CallStubIndex++)
        {
            const IDEFStub& InterfaceCallStub = LayedoutDiagram.CallInterfaceStubs[CallStubIndex];
            Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceCallStub.Position.Column, InterfaceCallStub.Position.Row)); 

            InterfaceStubsMap.insert(InterfaceCallStub, ConnectionEnd);
        }

        return InterfaceStubsMap;
    }

    /*
     * Places connection ends at each activity box stub and the parent boundary interface stubs.
     *
     * Parameters:
     *     1. LayedoutDiagram - An IDEF Activity Diagram which has been layed out onto a file surface. 
     * Returns:
     *     1. BoxStubLabelConnMap - Map of activity box stub labels to avoid connection ends.
     *     2. BoundaryStubLabelConnMap - Map of parent interface stub labels to avoid connection ends.
     * Functional Breakdown:
     *     1. Places connection ends at each activity box stub.
     *     2. Places connection ends at each parent interface stub.
     * Local Variables:
     */
    void PlaceConnectionEnds(const IDEFActivityDiagram& LayedoutDiagram, std::map<IDEFStub, Avoid:ConnEnd>& BoxStubConnMap, std::map<IDEFStub, Avoid::ConnEnd>& BoundaryStubConnMap)
    {
        BoxStubConnMap = PlaceBoxStubConnectionEnds(LayedoutDiagram);
        BoundaryStubConnMap = PlaceBoundaryStubConnectionEnds(LayedoutDiagram);
    }

    /*
     * Places obstacles for each box in the diagram.
     *
     * Parameters:
     *     1. Layed-out idef activity diagram (LayedoutDiagram) - An activity diagram with all of its elements layed onto a text file.
     * Returns:
     *     1. List of Avoid Rectangles (Rectangles) - A list of rectangle shapes based on the activity diagram boxes.
     * Functional Breakdown:
     *     1. Iterates each box in the activity diagram.
     *     2. Constructs a rectangle for each box.
     *     3. Adds it to the list.
     * Local Variables:
     *     1. Number of activity boxes (NumBoxes).
     *     2. Selected activity box (SelectedBox).
     *     3. New rectangle (NewRect).
     */
    void PlaceObstacles(const IDEFActivityDiagram& LayedoutDiagram, std::vector<Avoid::Rectangle>& Rectangles)
    {
        uint32_t NumBoxes;

        NumBoxes = LayedoutDiagram.Boxes.size();
        for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
        {
            const IDEFActivityBox& SelectedBox = LayedoutDiagram.Boxes[BoxIndex];
            Avoid::Rectangle NewRect;
            FilePosition BoxTopLeft;
            FilePosition BoxBottomRight;
        
            BoxTopLeft.Column = SelectedBox.Position.Column - (SelectedBox.Width / 2u);
            BoxTopLeft.Row = SelectedBox.Position.Row - (SelectedBox.Height / 2u);
            BoxBottomRight.Column = SelectedBox.Position.Column + (SelectedBox.Width / 2u);
            BoxBottomRight.Row = SelectedBox.Position.Row + (SelectedBox.Height / 2u);
            Rectangles.push_back(Avoid::Point(BoxTopLeft.Column, BoxTopLeft.Row), Avoid::Point(BoxBottomRight.Column, BoxBottomRight.Row));
        }
    }

    /*
     * Prepares an avoid router from the ends.
     *
     * Parameters:
     *     1. BoxStubLabelConnMap - A map of box stub labels to avoid connection ends.
     *     2. BoundaryStubLabelConnMap - A map of parent interface stub labels to avoid connection ends.
     * Returns:
     *     1. PreparedRouter - An avoid router with all connection ends joined.
     * Functional Breakdown:
     *     1. Iterates each box stub.
     *     2. Iterates each other box stub.
     *     3. Checks if their labels match.
     *     4. Connects the stubs with an avoid connref.
     *     5. Iterates each boundary stub.
     *     6. Checks if the iterated stub's label matches the boundaries.
     *     7. Connects the interface stub with an avoid connnref.
     */
    Avoid::Router* ConstructRouter(std::map<std::string, Avoid::ConnEnd>& BoxStubConnMap, std::map<std::string, Avoid::ConnEnd>& BoundaryStubConnMap, std::vector<Avoid::Rectangle>& Rectangles)
    {
        Avoid::Router* ConstructedRouter;
        uint32_t NumRects;

        ConstructedRouter = new Avoid::Router(Avoid::OrthogonalRouting);
        NumRects = Rectangles.size();
        for (uint32_t RectangleIndex = 0u; RectangleIndex < NumRects; RectangleIndex++)
        {
            Avoid::Rectangle SelectedRectangle;
            Avoid::ShapeRef* ShapeReference;  

            SelectedRectangle = Rectangles[RectangleIndex];
            ShapeReference = new Avoid::ShapeRef(ConstructedRouter, SelectedRectangle);          
        }
        for (const std::pair<IDEFStub, Avoid::ConnEnd>& BoxStubPair : BoxStubConnMap)
        {
            for (const StubSource& StubSource : BoxStubPair.first.Sources)
            {
                for (const std::pair<IDEFStub, Avoid::ConnEnd>& PotentialInterfaceSourceStubPair : BoundaryStubConnMap)
                {
                    if (PotentialInterfaceSourceStubPair.first.Name == StubSource.StubName)
                    {
                        Avoid::ConnRef* NewConnectionRef;

                        NewConnectionRef = new Avoid::ConnRef(ConstructedRouter, PotentialInterfaceSourceStubPair.second, BoxStubPair.second);
                    }
                } 
                for (const std::pair<IDEFStub, Avoid::ConnEnd>& PotentialSourceStubPair : BoxStubConnMap)
                {
                    if (PotentialSourceStubPair.first.Name == StubSource.StubName)
                    {
                        Avoid::ConnRef* NewConnectionRef;

                        NewConnectionRef = new Avoid::ConnRef(ConstructedRouter, PotentialSourceStubPair.second, BoxStubPair.second);
                    }
                }                
            }
        }

        return ConstructedRouter;
    }

    /*
     * Draws a diagram and its associated routed avoid connections.
     *
     * Parameters:
     *     1. Target diagram to draw.
     *     2. Router with connections.
     * Return:
     *     1. Diagram as a vector of strings.
     * Functional Breakdown:
     *     1. Creates the diagram string vector.
     *     2. Draws the activity boxes.
     *     3. Draws the connections.
     *     4. Draws the stub labels.
     */
    std::vector<std::string> DrawDiagram(const IDEFActivityDiagram& TargetDiagram, Avoid::Router* ConnectedRouter)
    {
        std::vector<std::string> Diagram;
        uint32_t ActivityBoxNum;

        for (uint32_t RowNum = 0u; RowNum < TargetDiagram.Height; RowNum++)
        {
            std::string RowString;
            
            RowString.resize(TargetDiagram.Width + 1u);
            Diagram.push_back(RowString);
        }
        // Drawing the arrows.
        for (Avoid::ConnRef* ConnRef : ConnectedRouter->connRefs)
            {
                const Polyline& Route = ConnRef->displayRoute();
                uint32_t NumVertices;
                uint32_t VertexIndex;
                std::string TravelDir;

                NumVertices = Route.size();
                for (uint32_t VertexIndex = 0u; VertexIndex < (NumVertices-1u); VertexIndex++)
                {
                    uint32_t NextVertexIndex;
                    FilePosition LineStartPoint;
                    FilePosition LineEndPoint;
                    
                    NextVertexIndex = VertexIndex + 1u; 
                    const Point& FirstPoint = Route.at(VertexIndex);
                    const Point& SecondPoint = Route.at(NextVertexIndex);
                    LineStartPoint.Column = (uint32_t)(round(FirstPoint.x));
                    LineStartPoint.Row = (uint32_t)(round(FirstPoint.y));
                    LineEndPoint.Column = (uint32_t)(round(SecondPoint.x));
                    LineEndPoint.Row = (uint32_t)(round(SecondPoint.y));
                    if (LineEndPoint.Row > LineStartPoint.Row)
                    {
                        TravelDir = "Down";
                    }
                    if (LineEndPoint.Row < LineStartPoint.Row)
                    {
                        TravelDir = "Up";
                    }
                    if (LineEndPoint.Column > LineStartPoint.Column)
                    {
                        TravelDir = "Right";
                    }
                    if (LineEndPoint.Column < LineStartPoint.Column)
                    {
                        TravelDir = "Left";
                    }
                    if (TravelDir == "Down")
                    {   
                        uint32_t CursorRow;
                        uint32_t CursorColumn;
                        
                        CursorColumn = LineStartPoint.Column;
                        for (CursorRow = LineStartPoint.Row; CursorRow < LineEndPoint.Row; CursorRow++)
                        {
                            if (CursorRow == LineEndPoint.Row)
                            {
                                Diagram[CursorRow][CursorColumn] = "v";
                            }
                            else
                            {
                                Diagram[CursorRow][CursorColumn] = '|';
                            }
                        }
                    }
                    else if (TravelDir == "Up")
                    {
                        uint32_t CursorRow;
                        uint32_t CursorColumn;
                        
                        CursorColumn = LineStartPoint.Column;
                        for (CursorRow = LineStartPoint.Row; CursorRow > LineEndPoint.Row; CursorRow--)
                        {
                            if (CursorRow == LineEndPoint.Row)
                            {
                                Diagram[CursorRow][CursorColumn] = "^";
                            }
                            else
                            {
                                Diagram[CursorRow][CursorColumn] = '|';
                            }
                        }
                    }
                    else if (TravelDir == "Right")
                    {
                        uint32_t CursorRow;
                        uint32_t CursorColumn;
                        
                        CursorRow = LineStartPoint.Row;
                        for (CursorColumn = LineStartPoint.Column; CursorColumn < LineEndPoint.Column; CursorColumn++)
                        {
                            if (CursorColumn == LineEndPoint.Column)
                            {
                                Diagram[CursorRow][CursorColumn] = ">";
                            }
                            else
                            {
                                Diagram[CursorRow][CursorColumn] = '-';
                            }
                        }
                    }
                    else if (TravelDir == "Left")
                    {
                        uint32_t CursorRow;
                        uint32_t CursorColumn;
                        
                        CursorRow = LineStartPoint.Row;
                        for (CursorColumn = LineStartPoint.Column; CursorColumn > LineEndPoint.Column; CursorColumn++)
                        {
                            if (CursorColumn == LineEndPoint.Column)
                            {
                                Diagram[CursorRow][CursorColumn] = "<";
                            }
                            else
                            {
                                Diagram[CursorRow][CursorColumn] = '-';
                            }
                        }
                    }
                    else
                    {
                        throw std::runtime_error("Unknown travel direction");
                    }
                }
            }

        ActivityBoxNum = TargetDiagram.Boxes.size();
        for (uint32_t ActivityBoxIndex = 0u; ActivityBoxIndex < ActivityBoxNum; ActivityBoxIndex++)
        {
            const IDEFActivityBox& SelectedBox = TargetDiagram.Boxes[ActivityBoxIndex];
            FilePosition BoxTopLeft;
            FilePosition BoxBottomRight;

            BoxTopLeft.Row = SelectedBox.Position.Row - (SelectedBox.Height/2u);
            BoxTopLeft.Column = SelectedBox.Position.Column - (SelectedBox.Width/2u);
            BoxBottomRight.Row = SelectedBox.Position.Row + (SelectedBox.Width/2u);
            BoxBottomRight.Column = SelectedBox.Position.Column + (SelectedBox.Height/2u);
            for (uint32_t CursorX = BoxTopLeft.Position.Column; CursorX < BoxBottomRight.Position.Column; CursorX++)
            {
                for (uint32_t CursorY = BoxTopLeft.Position.Row; CursorY < BoxBottomRight.Position.Row; CursorY++)
                {
                    if (CursorX == BoxTopLeft.Position.Column)
                    {
                        if (CursorY == BoxTopLeft.Position.Row)
                        {
                            Diagram[CursorY][CursorX] = '+';
                        }
                        else if (CursorY == BoxBottomRight.Position.Row)
                        {
                            Diagram[CursorY][CursorX] = '+';
                        }
                        else
                        {
                            Diagram[CursorY][CursorX] = '|';
                        }
                    }
                    else if (CursorX == BoxBottomRight.Position.Column)
                    {
                        if (CursorY == BoxTopLeft.Position.Row)
                        {
                            Diagram[CursorY][CursorX] = '+';
                        }
                        else if (CursorY = BoxBottomLeft.Position.Row)
                        {
                            Diagram[CursorY][CursorX] = '+';
                        }
                        else
                        {
                            Diagram[CursorY][CursorX] = '|';
                        }
                    }
                    else
                    {
                        if (CursorX == BoxTopLeft.Position.Column)
                        {
                            Diagram[CursorY][CursorX] = '+';
                        }
                        else if (CursorX == BoxBottomRight.Position.Column)
                        {
                            Diagram[CursorY][CursorY] = '+';
                        }
                        else
                        {
                            Diagram[CursorX][CursorY] = '-';
                        }
                    }
                }
                for (const IDEFStub& InputStub : SelectedBox.InputStubs)
                {
                    uint32_t LabelLength;
                    
                    LabelLength = InputStub.Label.length();
                    for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
                    {
                        uint32_t CharColumn;
                        uint32_t CharRow;
                        
                        CharColumn = InputStub.Position.Column - (LabelLength - CharIndex);
                        CharRow = InputStub.Position.Row;
                        Diagram[CharColumn][CharRow] = InputStub.Label[CharIndex];
                    }
                }
                for (const IDEFStub& OutputStub : SelectedBox.OutputStubs)
                {
                    uint32_t LabelLength;
                    
                    LabelLength = OutputStub.Label.length();
                    for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
                    {
                        uint32_t CharColumn;
                        uint32_t CharRow;
                        
                        CharColumn = OutputStub.Position.Column + CharIndex;
                        CharRow = OutputStub.Position.Row;
                        Diagram[CharColumn][CharRow] = OutputStub.Label[CharIndex];
                    }
                }
                uint32_t ControlStubCount;
                ControlStubCount = SelectedBox.ControlStubs.size();
                for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlstubCount; ControlStubIndeX++)
                {
                    const IDEFStub& ControlStub = SelectedBox.ControlStubs[ControlStubIndex];
                    uint32_t LabelLength;
                    
                    LabelLength = ControlStub.Label.length();
                    for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
                    {
                        uint32_t CharColumn;
                        uint32_t CharRow;
                        
                        CharColumn = ControlStub.Position.Column + CharIndex;
                        CharRow = ControlStub.Position.Row - 1u - ControlStubIndex;
                        Diagram[CharColumn][CharRow] = ControlStub.Label[CharIndex];
                    }
                }
                uint32_t MechanismStubCount;
                MechanismStubCount = SelectedBox.MechanismStubs.size();
                for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismStubCount; MechanismStubIndeX++)
                {
                    const IDEFStub& MechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
                    uint32_t LabelLength;
                    
                    LabelLength = MechanismStub.Label.length();
                    for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
                    {
                        uint32_t CharColumn;
                        uint32_t CharRow;
                        
                        CharColumn = MechanismStub.Position.Column + CharIndex;
                        CharRow = ControlStub.Position.Row + 1u + MechanismStubIndex;
                        Diagram[CharColumn][CharRow] = MechanismStub.Label[CharIndex];
                    }
                }
                uint32_t CallStubCount;
                CallStubCount = SelectedBox.CallStubs.size();
                for (uint32_t CallStubIndex = 0u; CallStubIndex < CallStubCount; CallStubIndeX++)
                {
                    const IDEFStub& CallStub = SelectedBox.CallStubs[CallStubIndex];
                    uint32_t LabelLength;
                    
                    LabelLength = CallStub.Label.length();
                    for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
                    {
                        uint32_t CharColumn;
                        uint32_t CharRow;
                        
                        CharColumn = CallStub.Position.Column + CharIndex;
                        CharRow = CallStub.Position.Row + MechanismStubCount + 1u + CallStubIndex;
                        Diagram[CharColumn][CharRow] = CallStub.Label[CharIndex];
                    }
                }
            }
         }
        return Diagram;      
    }
}

int main(int argc, char** argv)
{
	return 0;
}
