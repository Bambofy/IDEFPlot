#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <libavoid/libavoid.h>
#include <map>
#include <pugixml.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace IDEF
{
class FilePosition
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
    bool operator<(const Stub &Other) const
    {
        int32_t ProjectedPosition;
        int32_t OtherProjectedPosition;

        ProjectedPosition = (this->Position.Row * 1000u) + this->Position.Column;
        OtherProjectedPosition = (Other.Position.Row * 1000u) + Other.Position.Column;
        return (ProjectedPosition == OtherProjectedPosition);
    }
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;
};

class UMLStub : public Stub
{
  public:
};

class IDEFStub : public Stub
{
  public:
};

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
    std::vector<IDEFStub> InputStubs;
    std::vector<IDEFStub> OutputStubs;
    std::vector<IDEFStub> ControlStubs;
    std::vector<IDEFStub> MechanismStubs;
    std::vector<IDEFStub> CallStubs;
};

class IDEFStateBox : public IDEFBox
{
  public:
    std::vector<IDEFStub> InputStubs;
    std::vector<IDEFStub> OutputStubs;
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
    std::vector<IDEFStub> BoundaryStubs;
};

class IDEFActivityDiagram : public IDEFDiagram
{
  public:
    std::vector<IDEFActivityBox> Boxes;
    std::vector<IDEFStub> BoundaryInputStubs;
    std::vector<IDEFStub> BoundaryOutputStubs;
    std::vector<IDEFStub> BoundaryControlStubs;
    std::vector<IDEFStub> BoundaryMechanismStubs;
    std::vector<IDEFStub> BoundaryCallStubs;
};

class Model
{
  public:
    std::string Title;
    Glossary AttachedGlossary;
    std::vector<ClassDiagram> ClassDiagrams;
    std::vector<IDEFStateDiagram> StateDiagrams;
    std::vector<IDEFActivityDiagram> ActivityDiagrams;
};

GlossaryEntry LoadGlossaryEntry(const pugi::xml_node &TargetGlossaryEntryNode)
{
    std::string EntryTitle;
    std::string EntryDescription;
    std::string EntryType;
    GlossaryEntry NewGlossaryEntry;

    EntryTitle = TargetGlossaryEntryNode.attribute("Title").as_string();
    EntryDescription = TargetGlossaryEntryNode.attribute("Description").as_string();
    EntryType = TargetGlossaryEntryNode.attribute("Type").as_string();
    NewGlossaryEntry.Title = EntryTitle;
    NewGlossaryEntry.Description = EntryDescription;
    NewGlossaryEntry.Type = EntryType;

    return NewGlossaryEntry;
}

Glossary LoadGlossary(const pugi::xml_node &TargetGlossaryNode)
{
    Glossary NewGlossary;

    for (const pugi::xml_node &EntryNode : TargetGlossaryNode.children())
    {
        GlossaryEntry NewGlossaryEntry;

        NewGlossaryEntry = LoadGlossaryEntry(EntryNode);
        NewGlossary.push_back(NewGlossaryEntry);
    }

    return NewGlossary;
}

IDEFActivityStub LoadActivityStub(const pugi::xml_node &InputXMLNode)
{
    IDEFActivityStub NewActivityStub;

    NewActivityStub.Name = InputXMLNode.attribute("Name").as_string();
    for (const pugi::xml_node &StubSourceXMLNode : InputXMLNode.children())
    {
        StubSource NewStubSource;

        NewStubSource.StubName = StubSourceXMLNode.attribute("Name").as_string();
        NewActivityStub.Sources.push_back(NewStubSource);
    }
    NewActivityStub.Position.Row = 0u;
    NewActivityStub.Position.Column = 0u;

    return NewActivityStub;
}

IDEFActivityBox LoadActivity(const pugi::xml_node &ActivityNode)
{
    IDEFActivityBox NewActivityBox;

    NewActivityBox.Name = ActivityNode.attribute("Name").as_string();
    NewActivityBox.Width = 0u;
    NewActivityBox.Height = 0u;
    NewActivityBox.Center.Row = 0u;
    NewActivityBox.Center.Column = 0u;
    for (const pugi::xml_node &XMLStub : ActivityNode.children())
    {
        if (strcmp(XMLStub.name(), "Input") == 0)
        {
            IDEFActivityStub NewInputStub;

            NewInputStub = LoadActivityStub(XMLStub);
            NewActivityBox.InputStubs.push_back(NewInputStub);
        }
        else if (strcmp(XMLStub.name(), "Output") == 0)
        {
            IDEFActivityStub NewOutputStub;

            NewOutputStub = LoadActivityStub(XMLStub);
            NewActivityBox.OutputStubs.push_back(NewOutputStub);
        }
        else if (strcmp(XMLStub.name(), "Control") == 0)
        {
            IDEFActivityStub NewControlStub;

            NewControlStub = LoadActivityStub(XMLStub);
            NewActivityBox.ControlStubs.push_back(NewControlStub);
        }
        else if (strcmp(XMLStub.name(), "Mechanism") == 0)
        {
            IDEFActivityStub NewMechanismStub;

            NewMechanismStub = LoadActivityStub(XMLStub);
            NewActivityBox.MechanismStubs.push_back(NewMechanismStub);
        }
        else if (strcmp(XMLStub.name(), "Call") == 0)
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

    return NewActivityBox;
}

IDEFActivityDiagram LoadActivityDiagram(const pugi::xml_node &ActivityDiagramNode)
{
    IDEFActivityDiagram NewActivityDiagram;

    NewActivityDiagram.Frame.Bar.NumberSection.Content = ActivityDiagramNode.attribute("Number").as_string();
    NewActivityDiagram.Frame.Bar.NumberSection.TopLeft.Row = 0u;
    NewActivityDiagram.Frame.Bar.NumberSection.TopLeft.Column = 0u;
    NewActivityDiagram.Frame.Bar.TitleSection.Content = ActivityDiagramNode.attribute("Title").as_string();
    NewActivityDiagram.Frame.Bar.TitleSection.TopLeft.Row = 0u;
    NewActivityDiagram.Frame.Bar.TitleSection.TopLeft.Column = 0u;
    NewActivityDiagram.Frame.Bar.CNumberSection.Content = ActivityDiagramNode.attribute("CNumber").as_string();
    NewActivityDiagram.Frame.Bar.CNumberSection.TopLeft.Row = 0u;
    NewActivityDiagram.Frame.Bar.CNumberSection.TopLeft.Column = 0u;
    NewActivityDiagram.Width = 0u;
    NewActivityDiagram.Height = 0u;
    for (const pugi::xml_node &ChildXMLNode : ActivityDiagramNode.children())
    {
        if (strcmp(ChildXMLNode.name(), "Input") == 0)
        {
            IDEFActivityStub NewInputStub;

            NewInputStub = LoadActivityStub(ChildXMLNode);
            NewActivityDiagram.BoundaryInputStubs.push_back(NewInputStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Output") == 0)
        {
            IDEFActivityStub NewOutputStub;

            NewOutputStub = LoadActivityStub(ChildXMLNode);
            NewActivityDiagram.BoundaryOutputStubs.push_back(NewOutputStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Control") == 0)
        {
            IDEFActivityStub NewControlStub;

            NewControlStub = LoadActivityStub(ChildXMLNode);
            NewActivityDiagram.BoundaryControlStubs.push_back(NewControlStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Mechanism") == 0)
        {
            IDEFActivityStub NewMechanismStub;

            NewMechanismStub = LoadActivityStub(ChildXMLNode);
            NewActivityDiagram.BoundaryMechanismStubs.push_back(NewMechanismStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Call") == 0)
        {
            IDEFActivityStub NewCallStub;

            NewCallStub = LoadActivityStub(ChildXMLNode);
            NewActivityDiagram.BoundaryCallStubs.push_back(NewCallStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Activity") == 0)
        {
            IDEFActivityBox NewActivityBox;

            NewActivityBox = LoadActivity(ChildXMLNode);
            NewActivityDiagram.Boxes.push_back(NewActivityBox);
        }
        else
        {
            std::string ErrorMessage;

            ErrorMessage = "Unknown XML activity diagram child node kind: ";
            ErrorMessage += ChildXMLNode.name();

            throw std::runtime_error(ErrorMessage);
        }
    }

    return NewActivityDiagram;
}

Model LoadModel(const pugi::xml_node &ModelNode)
{
    Model NewModel;

    NewModel.Title = ModelNode.attribute("Name").as_string();
    for (const pugi::xml_node &ChildNode : ModelNode.children())
    {
        if (strcmp(ChildNode.name(), "ActivityDiagram") == 0)
        {
            IDEFActivityDiagram NewDiagram;

            NewDiagram = LoadActivityDiagram(ChildNode);
            NewModel.ActivityDiagrams.push_back(NewDiagram);
        }
        else if (strcmp(ChildNode.name(), "ClassDiagram") == 0)
        {
        }
        else if (strcmp(ChildNode.name(), "StateDiagram") == 0)
        {
        }
        else if (strcmp(ChildNode.name(), "Glossary") == 0)
        {
        }
        else
        {
            std::string ErrorMessage;

            ErrorMessage = "Unknown diagram type: ";
            ErrorMessage += ChildNode.name();
            throw std::runtime_error(ErrorMessage);
        }
    }

    return NewModel;
}

std::vector<Model> LoadModelsFile(const std::string &FilePath)
{
    pugi::xml_document ModelsXMLDocument;
    pugi::xml_node RootXMLNode;
    pugi::xml_parse_result ParseResult;
    std::vector<Model> LoadedModels;

    ParseResult = ModelsXMLDocument.load_file(FilePath.c_str());
    RootXMLNode = ModelsXMLDocument.child("Models");
    for (const pugi::xml_node &ChildXMLNode : RootXMLNode.children())
    {
        Model LoadedModel;

        LoadedModel = LoadModel(ChildXMLNode);
        LoadedModels.push_back(LoadedModel);
    }

    return LoadedModels;
}

void LayoutFrame(IDEFActivityDiagram &Diagram)
{
    static const uint32_t TopLeftRow = 0u;
    static const uint32_t TopLeftColumn = 0u;
    static const uint32_t BottomBarHeight = 3u;

    Diagram.Frame.Bar.TopLeft.Row = Diagram.Height - BottomBarHeight;
    Diagram.Frame.Bar.TopLeft.Column = TopLeftColumn;

    Diagram.Frame.Bar.NumberSection.Width = Diagram.Width / 3u;
    Diagram.Frame.Bar.NumberSection.TopLeft.Row = Diagram.Frame.Bar.TopLeft.Row;
    Diagram.Frame.Bar.NumberSection.TopLeft.Column = Diagram.Frame.Bar.TopLeft.Column;

    Diagram.Frame.Bar.TitleSection.Width = Diagram.Width / 3u;
    Diagram.Frame.Bar.TitleSection.TopLeft.Row = Diagram.Frame.Bar.TopLeft.Row;
    Diagram.Frame.Bar.TitleSection.TopLeft.Column =
        Diagram.Frame.Bar.TopLeft.Column + Diagram.Frame.Bar.TitleSection.Width;

    Diagram.Frame.Bar.CNumberSection.Width = Diagram.Width / 3u;
    Diagram.Frame.Bar.CNumberSection.TopLeft.Row = Diagram.Frame.Bar.TopLeft.Row;
    Diagram.Frame.Bar.CNumberSection.TopLeft.Column =
        Diagram.Frame.Bar.TopLeft.Column + Diagram.Frame.Bar.CNumberSection.Width;
}

void LayoutBoxes(IDEFActivityDiagram &Diagram)
{
    static const uint32_t BottomBarHeight = 3u;
    uint32_t NumBoxes;
    uint32_t ColumnWidth;
    uint32_t RowHeight;

    NumBoxes = Diagram.Boxes.size();
    ColumnWidth = Diagram.Width / NumBoxes;
    RowHeight = (Diagram.Height - BottomBarHeight) / NumBoxes;
    std::cout << "Columns are " << ColumnWidth << " wide" << std::endl;
    std::cout << "Rows are " << RowHeight << " tall" << std::endl;
    for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
    {
        IDEFActivityBox &SelectedBox = Diagram.Boxes[BoxIndex];
        uint32_t ColumnPadding;
        uint32_t RowPadding;
        const uint32_t BoxWidth = 10;
        const uint32_t BoxHeight = 3;

        SelectedBox.Width = BoxWidth;
        SelectedBox.Height = BoxHeight;
        ColumnPadding = ColumnWidth / 2u;
        RowPadding = RowHeight / 2u;
        SelectedBox.Center.Column = ColumnPadding + (ColumnWidth * BoxIndex);
        SelectedBox.Center.Row = RowPadding + (RowHeight * BoxIndex);
    }
}

void LayoutBoxStubs(IDEFActivityDiagram &Diagram)
{
    uint32_t NumBoxes;

    NumBoxes = Diagram.Boxes.size();
    for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
    {
        IDEFActivityBox &SelectedBox = Diagram.Boxes[BoxIndex];
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
        InputInterfaceDivisions = NumInputStubs + 1u;
        InputInterfaceDivisionWidth = SelectedBox.Height / InputInterfaceDivisions;
        for (uint32_t InputStubIndex = 0u; InputStubIndex < NumInputStubs; InputStubIndex++)
        {
            IDEFStub &SelectedInputStub = SelectedBox.InputStubs[InputStubIndex];
            uint32_t RowOffset;

            RowOffset = InputInterfaceDivisionWidth * (1u + InputStubIndex);
            SelectedInputStub.Position.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
            SelectedInputStub.Position.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
            SelectedInputStub.Position.Row = SelectedInputStub.Position.Row + RowOffset;
        }
        OutputInterfaceDivisions = NumOutputStubs + 1u;
        OutputInterfaceDivisionWidth = SelectedBox.Height / OutputInterfaceDivisions;
        for (uint32_t OutputStubIndex = 0u; OutputStubIndex < NumOutputStubs; OutputStubIndex++)
        {
            IDEFStub &SelectedOutputStub = SelectedBox.OutputStubs[OutputStubIndex];
            uint32_t RowOffset;

            RowOffset = OutputInterfaceDivisionWidth * (1u + OutputStubIndex);
            SelectedOutputStub.Position.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
            SelectedOutputStub.Position.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
            SelectedOutputStub.Position.Row = SelectedOutputStub.Position.Row + RowOffset;
        }
        ControlInterfaceDivisions = NumControlStubs + 1u;
        ControlInterfaceDivisionWidth = SelectedBox.Width / ControlInterfaceDivisions;
        for (uint32_t ControlStubIndex = 0u; ControlStubIndex < NumControlStubs; ControlStubIndex++)
        {
            IDEFStub &SelectedControlStub = SelectedBox.ControlStubs[ControlStubIndex];
            uint32_t ColumnOffset;

            ColumnOffset = ControlInterfaceDivisionWidth * (1u + ControlStubIndex);
            SelectedControlStub.Position.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
            SelectedControlStub.Position.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
            SelectedControlStub.Position.Column = SelectedControlStub.Position.Column + ColumnOffset;
        }
        MechanismInterfaceDivisions = NumMechanismStubs + 1u;
        MechanismInterfaceDivisionWidth = (SelectedBox.Width / 2u) / MechanismInterfaceDivisions;
        for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < NumMechanismStubs; MechanismStubIndex++)
        {
            IDEFStub &SelectedMechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
            uint32_t ColumnOffset;

            ColumnOffset = MechanismInterfaceDivisionWidth * (1u + MechanismStubIndex);
            SelectedMechanismStub.Position.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
            SelectedMechanismStub.Position.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
            SelectedMechanismStub.Position.Column = SelectedMechanismStub.Position.Row + ColumnOffset;
        }
        CallInterfaceDivisions = NumCallStubs + 1u;
        CallInterfaceDivisionWidth = (SelectedBox.Width / 2) / CallInterfaceDivisions;
        for (uint32_t CallStubIndex = 0u; CallStubIndex < NumCallStubs; CallStubIndex++)
        {
            IDEFStub &SelectedCallStub = SelectedBox.CallStubs[CallStubIndex];
            uint32_t ColumnOffset;

            ColumnOffset = CallInterfaceDivisionWidth * (1u + CallStubIndex);
            SelectedCallStub.Position.Column = SelectedBox.Center.Column;
            SelectedCallStub.Position.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
            SelectedCallStub.Position.Column = SelectedCallStub.Position.Column + ColumnOffset;
        }
    }
}

void LayoutBoundaryStubs(IDEFActivityDiagram &Diagram)
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
        IDEFStub &SelectedStub = Diagram.BoundaryInputStubs[InputStubIndex];
        uint8_t RowOffset;

        RowOffset = (1 + InputStubIndex) * InputDivisionWidth;
        SelectedStub.Position.Column = 0u;
        SelectedStub.Position.Row = RowOffset;
    }
    for (uint8_t OutputStubIndex = 0u; OutputStubIndex < NumOutputStubs; OutputStubIndex++)
    {
        IDEFStub &SelectedStub = Diagram.BoundaryOutputStubs[OutputStubIndex];
        uint8_t RowOffset;

        RowOffset = (1 + OutputStubIndex) * OutputDivisionWidth;
        SelectedStub.Position.Column = Diagram.Width;
        SelectedStub.Position.Row = RowOffset;
    }
    for (uint8_t ControlStubIndex = 0u; ControlStubIndex < NumControlStubs; ControlStubIndex++)
    {
        IDEFStub &SelectedStub = Diagram.BoundaryControlStubs[ControlStubIndex];
        uint8_t ColumnOffset;

        ColumnOffset = (1 + ControlStubIndex) * ControlDivisionWidth;
        SelectedStub.Position.Row = 0u;
        SelectedStub.Position.Column = ColumnOffset;
    }
    for (uint8_t MechanismStubIndex = 0u; MechanismStubIndex < NumMechanismStubs; MechanismStubIndex++)
    {
        IDEFStub &SelectedStub = Diagram.BoundaryMechanismStubs[MechanismStubIndex];
        uint8_t ColumnOffset;

        ColumnOffset = (1 + MechanismStubIndex) * MechanismDivisionWidth;
        SelectedStub.Position.Row = Diagram.Height;
        SelectedStub.Position.Column = ColumnOffset;
    }
    for (uint8_t CallStubIndex = 0u; CallStubIndex < NumCallStubs; CallStubIndex++)
    {
        IDEFStub &SelectedStub = Diagram.BoundaryCallStubs[CallStubIndex];
        uint8_t ColumnOffset;

        ColumnOffset = (1 + CallStubIndex) * CallDivisionWidth;
        SelectedStub.Position.Row = Diagram.Height;
        SelectedStub.Position.Column = Diagram.Width / 2u;
        SelectedStub.Position.Column = SelectedStub.Position.Column + ColumnOffset;
    }
}

void LayoutActivityDiagram(IDEFActivityDiagram &LoadedDiagram, uint32_t Width, uint32_t Height)
{
    LoadedDiagram.Width = Width;
    LoadedDiagram.Height = Height;
    LayoutFrame(LoadedDiagram);
    LayoutBoxes(LoadedDiagram);
    LayoutBoxStubs(LoadedDiagram);
    LayoutBoundaryStubs(LoadedDiagram);
}

std::map<IDEFStub, Avoid::ConnEnd> PlaceBoxStubConnectionEnds(const IDEFActivityDiagram &LayedoutDiagram)
{
    std::map<IDEFStub, Avoid::ConnEnd> InterfaceStubsMap;
    uint32_t NumActivityBoxes;

    NumActivityBoxes = LayedoutDiagram.Boxes.size();
    for (uint32_t ActivityBoxIndex = 0u; ActivityBoxIndex < NumActivityBoxes; ActivityBoxIndex++)
    {
        const IDEFActivityBox &SelectedBox = LayedoutDiagram.Boxes[ActivityBoxIndex];
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
        for (uint32_t InputStubIndex = 0u; InputStubIndex < InputInterfaceStubCount; InputStubIndex++)
        {
            const IDEFStub &InterfaceInputStub = SelectedBox.InputStubs[InputStubIndex];
            Avoid::ConnEnd ConnectionEnd(
                Avoid::Point(InterfaceInputStub.Position.Column, InterfaceInputStub.Position.Row));

            InterfaceStubsMap.insert({InterfaceInputStub, ConnectionEnd});
        }
        for (uint32_t OutputStubIndex = 0u; OutputStubIndex < OutputInterfaceStubCount; OutputStubIndex++)
        {
            const IDEFStub &InterfaceOutputStub = SelectedBox.OutputStubs[OutputStubIndex];
            Avoid::ConnEnd ConnectionEnd(
                Avoid::Point(InterfaceOutputStub.Position.Column, InterfaceOutputStub.Position.Row));

            InterfaceStubsMap.insert({InterfaceOutputStub, ConnectionEnd});
        }
        for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlInterfaceStubCount; ControlStubIndex++)
        {
            const IDEFStub &InterfaceControlStub = SelectedBox.ControlStubs[ControlStubIndex];
            Avoid::ConnEnd ConnectionEnd(
                Avoid::Point(InterfaceControlStub.Position.Column, InterfaceControlStub.Position.Row));

            InterfaceStubsMap.insert({InterfaceControlStub, ConnectionEnd});
        }
        for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismInterfaceStubCount; MechanismStubIndex++)
        {
            const IDEFStub &InterfaceMechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
            Avoid::ConnEnd ConnectionEnd(
                Avoid::Point(InterfaceMechanismStub.Position.Column, InterfaceMechanismStub.Position.Row));

            InterfaceStubsMap.insert({InterfaceMechanismStub, ConnectionEnd});
        }
        for (uint32_t CallStubIndex = 0u; CallStubIndex < CallInterfaceStubCount; CallStubIndex++)
        {
            const IDEFStub &InterfaceCallStub = SelectedBox.CallStubs[CallStubIndex];
            Avoid::ConnEnd ConnectionEnd(
                Avoid::Point(InterfaceCallStub.Position.Column, InterfaceCallStub.Position.Row));

            InterfaceStubsMap.insert({InterfaceCallStub, ConnectionEnd});
        }
    }

    return InterfaceStubsMap;
}

std::map<IDEFStub, Avoid::ConnEnd> PlaceBoundaryStubConnectionEnds(const IDEFActivityDiagram &LayedoutDiagram)
{
    std::map<IDEFStub, Avoid::ConnEnd> InterfaceStubsMap;
    uint32_t InputInterfaceStubCount;
    uint32_t ControlInterfaceStubCount;
    uint32_t OutputInterfaceStubCount;
    uint32_t MechanismInterfaceStubCount;
    uint32_t CallInterfaceStubCount;

    InputInterfaceStubCount = LayedoutDiagram.BoundaryInputStubs.size();
    ControlInterfaceStubCount = LayedoutDiagram.BoundaryControlStubs.size();
    OutputInterfaceStubCount = LayedoutDiagram.BoundaryOutputStubs.size();
    MechanismInterfaceStubCount = LayedoutDiagram.BoundaryMechanismStubs.size();
    CallInterfaceStubCount = LayedoutDiagram.BoundaryCallStubs.size();
    for (uint32_t InputStubIndex = 0u; InputStubIndex < InputInterfaceStubCount; InputStubIndex++)
    {
        const IDEFStub &InterfaceInputStub = LayedoutDiagram.BoundaryInputStubs[InputStubIndex];
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceInputStub.Position.Column, InterfaceInputStub.Position.Row));

        InterfaceStubsMap.insert({InterfaceInputStub, ConnectionEnd});
    }
    for (uint32_t OutputStubIndex = 0u; OutputStubIndex < OutputInterfaceStubCount; OutputStubIndex++)
    {
        const IDEFStub &InterfaceOutputStub = LayedoutDiagram.BoundaryOutputStubs[OutputStubIndex];
        Avoid::ConnEnd ConnectionEnd(
            Avoid::Point(InterfaceOutputStub.Position.Column, InterfaceOutputStub.Position.Row));

        InterfaceStubsMap.insert({InterfaceOutputStub, ConnectionEnd});
    }
    for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlInterfaceStubCount; ControlStubIndex++)
    {
        const IDEFStub &InterfaceControlStub = LayedoutDiagram.BoundaryControlStubs[ControlStubIndex];
        Avoid::ConnEnd ConnectionEnd(
            Avoid::Point(InterfaceControlStub.Position.Column, InterfaceControlStub.Position.Row));

        InterfaceStubsMap.insert({InterfaceControlStub, ConnectionEnd});
    }
    for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismInterfaceStubCount; MechanismStubIndex++)
    {
        const IDEFStub &InterfaceMechanismStub = LayedoutDiagram.BoundaryMechanismStubs[MechanismStubIndex];
        Avoid::ConnEnd ConnectionEnd(
            Avoid::Point(InterfaceMechanismStub.Position.Column, InterfaceMechanismStub.Position.Row));

        InterfaceStubsMap.insert({InterfaceMechanismStub, ConnectionEnd});
    }
    for (uint32_t CallStubIndex = 0u; CallStubIndex < CallInterfaceStubCount; CallStubIndex++)
    {
        const IDEFStub &InterfaceCallStub = LayedoutDiagram.BoundaryCallStubs[CallStubIndex];
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(InterfaceCallStub.Position.Column, InterfaceCallStub.Position.Row));

        InterfaceStubsMap.insert({InterfaceCallStub, ConnectionEnd});
    }

    return InterfaceStubsMap;
}

void PlaceConnectionEnds(const IDEFActivityDiagram &LayedoutDiagram, std::map<IDEFStub, Avoid::ConnEnd> &BoxStubConnMap,
                         std::map<IDEFStub, Avoid::ConnEnd> &BoundaryStubConnMap)
{
    BoxStubConnMap = PlaceBoxStubConnectionEnds(LayedoutDiagram);
    BoundaryStubConnMap = PlaceBoundaryStubConnectionEnds(LayedoutDiagram);
}

void PlaceObstacles(const IDEFActivityDiagram &LayedoutDiagram, std::vector<Avoid::Rectangle> &Rectangles)
{
    uint32_t NumBoxes;

    NumBoxes = LayedoutDiagram.Boxes.size();
    for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
    {
        const IDEFActivityBox &SelectedBox = LayedoutDiagram.Boxes[BoxIndex];
        FilePosition BoxTopLeft;
        FilePosition BoxBottomRight;

        BoxTopLeft.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
        BoxTopLeft.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
        BoxBottomRight.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
        BoxBottomRight.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
        Avoid::Rectangle NewRect(Avoid::Point(BoxTopLeft.Column, BoxTopLeft.Row),
                                 Avoid::Point(BoxBottomRight.Column, BoxBottomRight.Row));
        Rectangles.push_back(NewRect);
    }
}

Avoid::Router *ConstructRouter(std::map<IDEFStub, Avoid::ConnEnd> &BoxStubConnMap,
                               std::map<IDEFStub, Avoid::ConnEnd> &BoundaryStubConnMap,
                               std::vector<Avoid::Rectangle> &Rectangles)
{
    Avoid::Router *ConstructedRouter;
    uint32_t NumRects;

    ConstructedRouter = new Avoid::Router(Avoid::OrthogonalRouting);
    NumRects = Rectangles.size();
    for (uint32_t RectangleIndex = 0u; RectangleIndex < NumRects; RectangleIndex++)
    {
        Avoid::ShapeRef *ShapeReference;

        Avoid::Rectangle SelectedRectangle = Rectangles[RectangleIndex];
        ShapeReference = new Avoid::ShapeRef(ConstructedRouter, SelectedRectangle);
    }
    for (const std::pair<IDEFStub, Avoid::ConnEnd> &BoxStubPair : BoxStubConnMap)
    {
        for (const StubSource &StubSource : BoxStubPair.first.Sources)
        {
            for (const std::pair<IDEFStub, Avoid::ConnEnd> &PotentialInterfaceSourceStubPair : BoundaryStubConnMap)
            {
                if (PotentialInterfaceSourceStubPair.first.Name == StubSource.StubName)
                {
                    Avoid::ConnRef *NewConnectionRef;

                    NewConnectionRef = new Avoid::ConnRef(ConstructedRouter, PotentialInterfaceSourceStubPair.second,
                                                          BoxStubPair.second);
                }
            }
            for (const std::pair<IDEFStub, Avoid::ConnEnd> &PotentialSourceStubPair : BoxStubConnMap)
            {
                if (PotentialSourceStubPair.first.Name == StubSource.StubName)
                {
                    Avoid::ConnRef *NewConnectionRef;

                    NewConnectionRef =
                        new Avoid::ConnRef(ConstructedRouter, PotentialSourceStubPair.second, BoxStubPair.second);
                }
            }
        }
    }

    return ConstructedRouter;
}

std::vector<std::string> DrawDiagram(const IDEFActivityDiagram &TargetDiagram, Avoid::Router *ConnectedRouter)
{
    std::vector<std::string> Diagram;
    uint32_t ActivityBoxNum;
    std::string BlankDiagramRow(TargetDiagram.Width, ' ');

    for (uint32_t RowNum = 0u; RowNum < TargetDiagram.Height; RowNum++)
    {
        Diagram.push_back(BlankDiagramRow);
    }
    std::cout << "Diagram width " << TargetDiagram.Width << " height " << TargetDiagram.Height << std::endl;
    /* Drawing the arrows.
    for (Avoid::ConnRef *ConnRef : ConnectedRouter->connRefs)
    {
        const Avoid::PolyLine &Route = ConnRef->displayRoute();
        uint32_t NumVertices;
        uint32_t VertexIndex;
        std::string TravelDir;

        NumVertices = Route.size();
        for (uint32_t VertexIndex = 0u; VertexIndex < (NumVertices - 1u); VertexIndex++)
        {
            uint32_t NextVertexIndex;
            FilePosition LineStartPoint;
            FilePosition LineEndPoint;

            NextVertexIndex = VertexIndex + 1u;
            const Avoid::Point &FirstPoint = Route.at(VertexIndex);
            const Avoid::Point &SecondPoint = Route.at(NextVertexIndex);
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
                        Diagram[CursorRow][CursorColumn] = 'v';
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
                        Diagram[CursorRow][CursorColumn] = '^';
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
                        Diagram[CursorRow][CursorColumn] = '>';
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
                for (CursorColumn = LineStartPoint.Column; CursorColumn > LineEndPoint.Column; CursorColumn--)
                {
                    if (CursorColumn == LineEndPoint.Column)
                    {
                        Diagram[CursorRow][CursorColumn] = '<';
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
    */
    ActivityBoxNum = TargetDiagram.Boxes.size();
    for (uint32_t ActivityBoxIndex = 0u; ActivityBoxIndex < ActivityBoxNum; ActivityBoxIndex++)
    {
        const IDEFActivityBox &SelectedBox = TargetDiagram.Boxes[ActivityBoxIndex];
        FilePosition BoxTopLeft;
        FilePosition BoxBottomRight;

        std::cout << SelectedBox.Width;
        std::cout << ",";
        std::cout << SelectedBox.Height;
        std::cout << ":";
        std::cout << SelectedBox.Center.Column;
        std::cout << ",";
        std::cout << SelectedBox.Center.Row;
        std::cout << std::endl;
        BoxTopLeft.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
        BoxTopLeft.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
        BoxBottomRight.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
        BoxBottomRight.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
        std::cout << BoxTopLeft.Column;
        std::cout << ",";
        std::cout << BoxTopLeft.Row;
        std::cout << " to ";
        std::cout << BoxBottomRight.Column;
        std::cout << ",";
        std::cout << BoxBottomRight.Row;
        std::cout << std::endl;
        for (uint32_t CursorX = BoxTopLeft.Column; CursorX <= BoxBottomRight.Column; CursorX++)
        {
            for (uint32_t CursorY = BoxTopLeft.Row; CursorY <= BoxBottomRight.Row; CursorY++)
            {
                if (CursorX == BoxTopLeft.Column)
                {
                    if (CursorY == BoxTopLeft.Row)
                    {
                        Diagram[CursorY][CursorX] = '+';
                    }
                    else if (CursorY == BoxBottomRight.Row)
                    {
                        Diagram[CursorY][CursorX] = '+';
                    }
                    else
                    {
                        Diagram[CursorY][CursorX] = '|';
                    }
                }
                else if (CursorX == BoxBottomRight.Column)
                {
                    if (CursorY == BoxTopLeft.Row)
                    {
                        Diagram[CursorY][CursorX] = '+';
                    }
                    else if (CursorY == BoxBottomRight.Row)
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
                    if (CursorY == BoxTopLeft.Row)
                    {
                        Diagram[CursorY][CursorX] = '-';
                    }
                    else if (CursorY == BoxBottomRight.Row)
                    {
                        Diagram[CursorY][CursorX] = '-';
                    }
                }
            }
        }
        continue;
        std::cout << "Drawing input stubs." << std::endl;
        for (const IDEFStub &InputStub : SelectedBox.InputStubs)
        {
            uint32_t LabelLength;

            LabelLength = InputStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = (InputStub.Position.Column - LabelLength) + CharIndex;
                CharRow = InputStub.Position.Row;
                std::cout << "Placing input stub at " << CharColumn << ", " << CharRow << std::endl;
                Diagram[CharRow][CharColumn] = InputStub.Name[CharIndex];
            }
        }
        std::cout << "Done placing input stubs" << std::endl;
        for (const IDEFStub &OutputStub : SelectedBox.OutputStubs)
        {
            uint32_t LabelLength;

            LabelLength = OutputStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = OutputStub.Position.Column + CharIndex;
                CharRow = OutputStub.Position.Row;
                std::cout << "Placing output stub at " << CharColumn << ", " << CharRow << std::endl;
                Diagram[CharRow][CharColumn] = OutputStub.Name[CharIndex];
            }
        }
        std::cout << "Done placing output stubs" << std::endl;
        uint32_t ControlStubCount;
        ControlStubCount = SelectedBox.ControlStubs.size();
        for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlStubCount; ControlStubIndex++)
        {
            const IDEFStub &ControlStub = SelectedBox.ControlStubs[ControlStubIndex];
            uint32_t LabelLength;

            LabelLength = ControlStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = ControlStub.Position.Column + CharIndex;
                CharRow = ControlStub.Position.Row - 1u - ControlStubIndex;
                std::cout << "Placing control stub at " << CharColumn << ", " << CharRow << std::endl;
                Diagram[CharRow][CharColumn] = ControlStub.Name[CharIndex];
            }
        }
        std::cout << "Done placing control stubs" << std::endl;
        uint32_t MechanismStubCount;
        MechanismStubCount = SelectedBox.MechanismStubs.size();
        for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismStubCount; MechanismStubIndex++)
        {
            const IDEFStub &MechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
            uint32_t LabelLength;

            LabelLength = MechanismStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = MechanismStub.Position.Column + CharIndex;
                CharRow = MechanismStub.Position.Row + 1u + MechanismStubIndex;
                std::cout << "Placing mechanism stub at " << CharColumn << ", " << CharRow << std::endl;
                Diagram[CharRow][CharColumn] = MechanismStub.Name[CharIndex];
            }
        }
        std::cout << "Done placing mechanism stubs" << std::endl;
        uint32_t CallStubCount;
        CallStubCount = SelectedBox.CallStubs.size();
        for (uint32_t CallStubIndex = 0u; CallStubIndex < CallStubCount; CallStubIndex++)
        {
            const IDEFStub &CallStub = SelectedBox.CallStubs[CallStubIndex];
            uint32_t LabelLength;

            LabelLength = CallStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = CallStub.Position.Column + CharIndex;
                CharRow = CallStub.Position.Row + MechanismStubCount + 1u + CallStubIndex;
                std::cout << "Placing call stub at " << CharColumn << ", " << CharRow << std::endl;
                Diagram[CharRow][CharColumn] = CallStub.Name[CharIndex];
            }
        }
        std::cout << "Done place call stubs" << std::endl;
    }
    return Diagram;
}
}

void Test1(char *InputFilePath, char *OutputFilePath)
{
    std::vector<IDEF::Model> Models;

    std::cout << "Running Test 1" << std::endl;

    Models = IDEF::LoadModelsFile(InputFilePath);
    for (IDEF::Model &SelectedModel : Models)
    {
        for (IDEF::IDEFActivityDiagram &SelectedActivityDiagram : SelectedModel.ActivityDiagrams)
        {
            std::fstream OutputFileStream;
            std::map<IDEF::IDEFStub, Avoid::ConnEnd> BoxStubConnMap;
            std::map<IDEF::IDEFStub, Avoid::ConnEnd> BoundaryStubConnMap;
            std::vector<Avoid::Rectangle> Obstacles;
            Avoid::Router *Router;
            std::vector<std::string> Diagram;
            uint32_t RowNumber;

            IDEF::LayoutActivityDiagram(SelectedActivityDiagram, 400, 120);
            IDEF::PlaceConnectionEnds(SelectedActivityDiagram, BoxStubConnMap, BoundaryStubConnMap);
            IDEF::PlaceObstacles(SelectedActivityDiagram, Obstacles);
            Router = IDEF::ConstructRouter(BoxStubConnMap, BoundaryStubConnMap, Obstacles);
            Diagram = DrawDiagram(SelectedActivityDiagram, Router);

            RowNumber = 0u;
            OutputFileStream.open(OutputFilePath, std::ios_base::out);
            for (const std::string &Line : Diagram)
            {
                OutputFileStream << Line;
                OutputFileStream << '\n';
                RowNumber = RowNumber + 1u;
            }
            OutputFileStream.close();

            delete Router;
        }
    }
}

int main(int argc, char **argv)
{
    Test1(argv[1u], argv[2u]);
    return 0;
}
