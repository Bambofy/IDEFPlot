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

struct FilePosition
{
    uint32_t Row;
    uint32_t Column;
};

struct StubSource
{
    std::string StubName;
};

struct InputStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;
};

struct OutputStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;
};

struct ControlStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;
};

struct MechanismStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;
};

struct CallStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;
};

typedef std::variant<InputStub, 
                     OutputStub, 
                     ControlStub, 
                     MechanismStub> Stub;

struct ActivityBox
{
    std::vector<Stub> InputStubs;
    std::vector<Stub> OutputStubs;
    std::vector<Stub> ControlStubs;
    std::vector<Stub> MechanismStubs;
    std::vector<Stub> CallStubs;
    std::string Name;
    FilePosition Center;
    uint32_t Width;
    uint32_t Height;
    uint32_t Padding;
    std::string NodeNumber;
};

struct NodeNumberSection 
{
    FilePosition TopLeft;
    std::string Content;
    uint32_t Width;
    uint32_t Height;
};

struct TitleSection
{
    FilePosition TopLeft;
    std::string Content;
    uint32_t Width;
    uint32_t Height;
};

struct CNumberSection
{
    FilePosition TopLeft;
    std::string Content;
    uint32_t Width;
    uint32_t Height;
};

typedef std::variant<NodeNumberSection,
                     TitleSection,
                     CNumberSection> DiagramSection;

struct DiagramBar
{
    FilePosition TopLeft;
    uint32_t Height;
    DiagramSection NumberSection;
    DiagramSection TitleSection;
    DiagramSection CNumberSection;
};

struct DiagramFrame
{
    DiagramBar BottomBar;
};

struct ActivityDiagram
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;
    std::vector<ActivityBox> Boxes;
    std::vector<Stub> InputBoundaryStubs;
    std::vector<Stub> OutputBoundaryStubs;
    std::vector<Stub> ControlBoundaryStubs;
    std::vector<Stub> MechanismBoundaryStubs;
    std::vector<Stub> CallBoundaryStubs;
};

struct Model
{
    std::string Title;
    std::vector<ActivityDiagram> ActivityDiagrams;
};

Stub LoadInputStub(const pugi::xml_node& InputStubXMLNode)
{
    InputStub NewStub;
    
    NewStub.Name = InputStubXMLNode.attribute("Name").as_string();
    for (const pugi::xml_node &StubSourceXMLNode : InputStubXMLNode.children())
    {
        StubSource NewStubSource;

        NewStubSource.StubName = StubSourceXMLNode.attribute("Name").as_string();
        NewStub.Sources.push_back(NewStubSource);
    }
    NewStub.Position.Row = 0u;
    NewStub.Position.Column = 0u;

    return NewStub;   
}

Stub LoadOutputStub(const pugi::xml_node& OutputStubXMLNode)
{
    OutputStub NewStub;

    NewStub.Name = OutputStubXMLNode.attribute("Name").as_string();
    for (const pugi::xml_node &StubSourceXMLNode : OutputStubXMLNode.children())
    {
        StubSource NewStubSource;

        NewStubSource.StubName = StubSourceXMLNode.attribute("Name").as_string();
        NewStub.Sources.push_back(NewStubSource);
    }
    NewStub.Position.Row = 0u;
    NewStub.Position.Column = 0u;

    return NewStub;   
}

Stub LoadControlStub(const pugi::xml_node& ControlStubXMLNode)
{
    ControlStub NewStub;

    NewStub.Name = ControlStubXMLNode.attribute("Name").as_string();
    for (const pugi::xml_node &StubSourceXMLNode : ControlStubXMLNode.children())
    {
        StubSource NewStubSource;

        NewStubSource.StubName = StubSourceXMLNode.attribute("Name").as_string();
        NewStub.Sources.push_back(NewStubSource);
    }
    NewStub.Position.Row = 0u;
    NewStub.Position.Column = 0u;

    return NewStub;   
}

Stub LoadMechanismStub(const pugi::xml_node& MechanismStubXMLNode)
{
    MechanismStub NewStub;

    NewStub.Name = MechanismStubXMLNode.attribute("Name").as_string();
    for (const pugi::xml_node &StubSourceXMLNode : MechanismStubXMLNode.children())
    {
        StubSource NewStubSource;

        NewStubSource.StubName = StubSourceXMLNode.attribute("Name").as_string();
        NewStub.Sources.push_back(NewStubSource);
    }
    NewStub.Position.Row = 0u;
    NewStub.Position.Column = 0u;

    return NewStub;   
}

Stub LoadCallStub(const pugi::xml_node& CallStubXMLNode)
{
    CallStub NewStub;

    NewStub.Name = CallStubXMLNode.attribute("Name").as_string();
    for (const pugi::xml_node &StubSourceXMLNode : CallStubXMLNode.children())
    {
        StubSource NewStubSource;

        NewStubSource.StubName = StubSourceXMLNode.attribute("Name").as_string();
        NewStub.Sources.push_back(NewStubSource);
    }
    NewStub.Position.Row = 0u;
    NewStub.Position.Column = 0u;

    return NewStub;   
}

ActivityBox LoadActivity(const pugi::xml_node &ActivityNode)
{
    ActivityBox NewActivityBox;

    NewActivityBox.Name = ActivityNode.attribute("Name").as_string();
    NewActivityBox.Width = 0u;
    NewActivityBox.Height = 0u;
    NewActivityBox.Center.Row = 0u;
    NewActivityBox.Center.Column = 0u;
    NewActivityBox.Padding = 3u;
    for (const pugi::xml_node &XMLStub : ActivityNode.children())
    {
        Stub NewStub;

        if (strcmp(XMLStub.name(), "Input") == 0)
        {
            NewStub = LoadInputStub(XMLStub);
            NewActivityBox.InputStubs.push_back(NewStub);
        }
        else if (strcmp(XMLStub.name(), "Output") == 0)
        {
            NewStub = LoadOutputStub(XMLStub);
            NewActivityBox.OutputStubs.push_back(NewStub);
        }
        else if (strcmp(XMLStub.name(), "Control") == 0)
        {
            NewStub = LoadControlStub(XMLStub);
            NewActivityBox.ControlStubs.push_back(NewStub);
        }
        else if (strcmp(XMLStub.name(), "Mechanism") == 0)
        {
            NewStub = LoadMechanismStub(XMLStub);
            NewActivityBox.MechanismStubs.push_back(NewStub);
        }
        else if (strcmp(XMLStub.name(), "Call") == 0)
        {
            NewStub = LoadCallStub(XMLStub);
            NewActivityBox.CallStubs.push_back(NewStub);
        }
        else
        {
            throw std::runtime_error("Unknown activity stub kind");
        }
    }

    return NewActivityBox;
}

ActivityDiagram LoadActivityDiagram(const pugi::xml_node &ActivityDiagramNode)
{
    ActivityDiagram NewDiagram;

    std::get<NumberSection>(NewDiagram.Frame.BottomBar.NumberSection).Content = ActivityDiagramNode.attribute("Number").as_string();
    std::get<NumberSection>(NewDiagram.Frame.BottomBar.NumberSection).TopLeft.Row = 0u;
    std::get<NumberSection>(NewDiagram.Frame.BottomBar.NumberSection).TopLeft.Column = 0u;
    std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection).Content = ActivityDiagramNode.attribute("Title").as_string();
    std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection).TopLeft.Row = 0u;
    std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection).TopLeft.Column = 0u;
    std::get<CNumberSection>(NewDiagram.Frame.BottomBar.CNumberSection).Content = ActivityDiagramNode.attribute("CNumber").as_string();
    std::get<CNumberSection>(NewDiagram.Frame.BottomBar.CNumberSection).TopLeft.Row = 0u;
    std::get<CNumberSection>(NewDiagram.Frame.BottomBar.CNumberSection).TopLeft.Column = 0u;
    NewDiagram.Width = 0u;
    NewDiagram.Height = 0u;
    for (const pugi::xml_node &ChildXMLNode : ActivityDiagramNode.children())
    {
        Stub NewStub;

        if (strcmp(ChildXMLNode.name(), "Input") == 0)
        {
            NewStub = LoadInputStub(ChildXMLNode);
            NewDiagram.BoundaryInputStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Output") == 0)
        {
            NewStub = LoadOutputStub(ChildXMLNode);
            NewDiagram.BoundaryOutputStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Control") == 0)
        {
            NewStub = LoadControlStub(ChildXMLNode);
            NewDiagram.BoundaryControlStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Mechanism") == 0)
        {
            NewStub = LoadMechanismStub(ChildXMLNode);
            NewDiagram.BoundaryMechanismStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Call") == 0)
        {
            NewStub = LoadCallStub(ChildXMLNode);
            NewDiagram.BoundaryCallStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Activity") == 0)
        {
            ActivityBox NewActivityBox;

            NewActivityBox = LoadActivity(ChildXMLNode);
            NewDiagram.Boxes.push_back(NewActivityBox);
        }
        else
        {
            std::string ErrorMessage;

            ErrorMessage = "Unknown XML activity diagram child node kind: ";
            ErrorMessage += ChildXMLNode.name();

            throw std::runtime_error(ErrorMessage);
        }
    }

    return NewDiagram;
}

Model LoadModel(const pugi::xml_node &ModelNode)
{
    Model NewModel;

    NewModel.Title = ModelNode.attribute("Name").as_string();
    for (const pugi::xml_node &ChildNode : ModelNode.children())
    {
        ActivityDiagram NewDiagram;

        NewDiagram = LoadActivityDiagram(ChildNode);
        NewModel.ActivityDiagrams.push_back(NewDiagram);
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

void LayoutFrame(ActivityDiagram &Diagram)
{
    static const uint32_t TopLeftRow = 0u;
    static const uint32_t TopLeftColumn = 0u;
    static const uint32_t BottomBarHeight = 3u;

    Diagram.Frame.BottomBar.TopLeft.Row = Diagram.Height - BottomBarHeight;
    Diagram.Frame.BottomBar.TopLeft.Column = TopLeftColumn;
    std::get<NumberSection>(Diagram.Frame.BottomBar.NumberSection).Width = Diagram.Width / 3u;
    std::get<NumberSection>(Diagram.Frame.BottomBar.NumberSection).TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    std::get<NumberSection>(Diagram.Frame.BottomBar.NumberSection).TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column;
    std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).Width = Diagram.Width / 3u;
    std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).TopLeft.Column = Diagram.Frame.Bar.TopLeft.Column + std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).Width;
    std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).Width = Diagram.Width / 3u;
    std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).TopLeft.Column = Diagram.Frame.Bar.TopLeft.Column + Diagram.Frame.BottomBar.CNumberSection.Width;
}

void LayoutBoxes(ActivityDiagram &Diagram)
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
        ActivityBox &SelectedBox = Diagram.Boxes[BoxIndex];
        uint32_t ColumnPadding;
        uint32_t RowPadding;
        const uint32_t BoxWidth = 32;
        const uint32_t BoxHeight = 8;

        SelectedBox.Width = BoxWidth;
        SelectedBox.Height = BoxHeight;
        ColumnPadding = ColumnWidth / 2u;
        RowPadding = RowHeight / 2u;
        SelectedBox.Center.Column = ColumnPadding + (ColumnWidth * BoxIndex);
        SelectedBox.Center.Row = RowPadding + (RowHeight * BoxIndex);
    }
}

void LayoutBoxStubs(ActivityDiagram &Diagram)
{
    uint32_t NumBoxes;

    NumBoxes = Diagram.Boxes.size();
    for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
    {
        ActivityBox &SelectedBox = Diagram.Boxes[BoxIndex];
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
            InputStub& SelectedInputStub = std::get<InputStub>(SelectedBox.InputStubs[InputStubIndex]);
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
            OutputStub &SelectedOutputStub = std::get<OutputStub>(SelectedBox.OutputStubs[OutputStubIndex]);
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
            ControlStub &SelectedControlStub = std::get<ControlStub>(SelectedBox.ControlStubs[ControlStubIndex]);
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
            MechanismStub &SelectedMechanismStub = std::get<MechanismStub>(SelectedBox.MechanismStubs[MechanismStubIndex]);
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
            CallStub &SelectedCallStub = std::get<CallStub>(SelectedBox.CallStubs[CallStubIndex]);
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
        InputStub &SelectedStub = std::get<InputStub>(Diagram.BoundaryInputStubs[InputStubIndex]);
        uint8_t RowOffset;

        RowOffset = (1 + InputStubIndex) * InputDivisionWidth;
        SelectedStub.Position.Column = 0u;
        SelectedStub.Position.Row = RowOffset;
    }
    for (uint8_t OutputStubIndex = 0u; OutputStubIndex < NumOutputStubs; OutputStubIndex++)
    {
        OutputStub &SelectedStub = std::get<OutputStub>(Diagram.BoundaryOutputStubs[OutputStubIndex]);
        uint8_t RowOffset;

        RowOffset = (1 + OutputStubIndex) * OutputDivisionWidth;
        SelectedStub.Position.Column = Diagram.Width;
        SelectedStub.Position.Row = RowOffset;
    }
    for (uint8_t ControlStubIndex = 0u; ControlStubIndex < NumControlStubs; ControlStubIndex++)
    {
        ControlStub &SelectedStub = std::get<ControlStub>(Diagram.BoundaryControlStubs[ControlStubIndex]);
        uint8_t ColumnOffset;

        ColumnOffset = (1 + ControlStubIndex) * ControlDivisionWidth;
        SelectedStub.Position.Row = 0u;
        SelectedStub.Position.Column = ColumnOffset;
    }
    for (uint8_t MechanismStubIndex = 0u; MechanismStubIndex < NumMechanismStubs; MechanismStubIndex++)
    {
        MechanismStub &SelectedStub = std::get<MechanismStub>(Diagram.BoundaryMechanismStubs[MechanismStubIndex]);
        uint8_t ColumnOffset;

        ColumnOffset = (1 + MechanismStubIndex) * MechanismDivisionWidth;
        SelectedStub.Position.Row = Diagram.Height;
        SelectedStub.Position.Column = ColumnOffset;
    }
    for (uint8_t CallStubIndex = 0u; CallStubIndex < NumCallStubs; CallStubIndex++)
    {
        CallStub &SelectedStub = std::get<CallStub>(Diagram.BoundaryCallStubs[CallStubIndex]);
        uint8_t ColumnOffset;

        ColumnOffset = (1 + CallStubIndex) * CallDivisionWidth;
        SelectedStub.Position.Row = Diagram.Height;
        SelectedStub.Position.Column = Diagram.Width / 2u;
        SelectedStub.Position.Column = SelectedStub.Position.Column + ColumnOffset;
    }
}

void LayoutActivityDiagram(ActivityDiagram &LoadedDiagram, uint32_t Width, uint32_t Height)
{
    LoadedDiagram.Width = Width;
    LoadedDiagram.Height = Height;
    LayoutFrame(LoadedDiagram);
    LayoutBoxes(LoadedDiagram);
    LayoutBoxStubs(LoadedDiagram);
    LayoutBoundaryStubs(LoadedDiagram);
}

vstd::map<Stub, Avoid::ConnEnd> PlaceBoxStubConnEnds(const ActivityDiagram& LayedOutDiagram)
{
    std::map<Stub, Avoid::ConnEnd> BoxStubsMap;
    uint32_t NumActivityBoxes;

    NumActivityBoxes = LayedOutDiagram.Boxes.size();
    for (ActivityBox& SelectedBox : LayedOutDiagram)
    {
        for (Stub SelectedStub : SelectedBox.InputStubs)
        {
            Avoid::ConnEnd ConnectionEnd(Avoid::Point(Stub.Position.Column, Stub.Position.Row));
            BoxStubsMap.insert({SelectedStub, ConnEnd});
        }
    }

    return BoxStubsMap;
}

std::map<Stub, Avoid::ConnEnd> PlaceBoundaryStubConnEnds(const ActivityDiagram& LayedOutDiagram)
{
    std::map<Stub, Avoid::ConnEnd> BoundaryStubsMap;

    for (Stub BoundaryStub : LayedOutDiagram.InputBoundaryStubs)
    {
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(BoundaryStub.Position.Column, BoundaryStub.Position.Row));
        BoundaryStubsMap.insert({BoundaryStub, ConnectionEnd};
    }
    for (Stub BoundaryStub : LayedOutDiagram.OutputBoundaryStubs)
    {
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(BoundaryStub.Position.Column, BoundaryStub.Position.Row));
        BoundaryStubsMap.insert({BoundaryStub, ConnectionEnd};
    }
    for (Stub BoundaryStub : LayedOutDiagram.ControlBoundaryStubs)
    {
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(BoundaryStub.Position.Column, BoundaryStub.Position.Row));
        BoundaryStubsMap.insert({BoundaryStub, ConnectionEnd};
    }
    for (Stub BoundaryStub : LayedOutDiagram.MechanismBoundaryStubs)
    {
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(BoundaryStub.Position.Column, BoundaryStub.Position.Row));
        BoundaryStubsMap.insert({BoundaryStub, ConnectionEnd};
    }
    for (Stub BoundaryStub : LayedOutDiagram.CallBoundaryStubs)
    {
        Avoid::ConnEnd ConnectionEnd(Avoid::Point(BoundaryStub.Position.Column, BoundaryStub.Position.Row));
        BoundaryStubsMap.insert({BoundaryStub, ConnectionEnd};
    }
   
    return BoundaryStubsMap;
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

Avoid::Router *ConstructRouter(std::map<Stub, Avoid::ConnEnd> &BoxStubsMap,
                               std::map<Stub, Avoid::ConnEnd>& BoundaryStubsMap
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
    for (const std::pair<Stub, Avoid::ConnEnd> &BoxStubPair : BoxStubsMap)
    {
        const Stub& FirstStub = BoxStubPair.first;

        for (const std::pair<Stub, Avoid::ConnEnd> &OtherBoxStubPair : BoxStubsMap)
        {
            const Stub& OtherStub = OtherBoxStubPair.second;
 
            if (FirstStub.Column != OtherStub.Column)
            {
                if (FirstStub.Row != OtherStub.Row)
                {
                    if (FirstStub.Name == OtherStub.Name)
                    {
                        if (std::holds_alternative<OutputStub>(FirstStub))
                        {
                            if (std::holds_alternative<InputStub>(OtherStub))
                            {
                                Avoid::ConnRef* NewRef;
                                
                                NewRef = Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
                            }
                            else if (std::holds_alternative<ControlStub>(OtherBoxStubPair))
                            {
                                Avoid::ConnRef* NewRef;
                                
                                NewRef = Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
                            }
                        }
                    }
                    else
                    {
                        for (const StubSource& Source : FirstStub.Sources)
                        {
                            if (Source.StubName == OtherStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                NewRef = Avoid::ConnRef(ConstructedRouter, OtherBoxStubPair.second, BoxStubPair.second);
                            }
                        }
                    }
                }
            }            
        }
    }   
    ConstructedRouter->processTransaction();
    
    return ConstructedRouter;
}

std::vector<std::string> DrawDiagram(const ActivityDiagram &TargetDiagram, Avoid::Router *ConnectedRouter)
{
    std::vector<std::string> Diagram;
    uint32_t ActivityBoxNum;
    std::string BlankDiagramRow(TargetDiagram.Width, ' ');

    for (uint32_t RowNum = 0u; RowNum < TargetDiagram.Height; RowNum++)
    {
        Diagram.push_back(BlankDiagramRow);
    }
    std::cout << "Diagram width " << TargetDiagram.Width << " height " << TargetDiagram.Height << std::endl;
    for (Avoid::ConnRef *ConnRef : ConnectedRouter->connRefs)
    {
        const Avoid::PolyLine &Route = ConnRef->displayRoute();
        uint32_t NumVertices;
        uint32_t VertexIndex;
        std::string TravelDir;

        std::cout << "Drawing an avoid connection" << std::endl;
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
            std::cout << "Drawing a line from (" << LineStartPoint.Column << ", " << LineStartPoint.Row << ") to ";
            std::cout << "(" << LineEndPoint.Column << ", " << LineEndPoint.Row << ")" << std::endl;
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
    ActivityBoxNum = TargetDiagram.Boxes.size();
    for (uint32_t ActivityBoxIndex = 0u; ActivityBoxIndex < ActivityBoxNum; ActivityBoxIndex++)
    {
        const ActivityBox &SelectedBox = TargetDiagram.Boxes[ActivityBoxIndex];
        FilePosition BoxTopLeft;
        FilePosition BoxBottomRight;

        BoxTopLeft.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
        BoxTopLeft.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
        BoxBottomRight.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
        BoxBottomRight.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
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
        uint8_t NumLines;
        uint32_t NumChars;
        FilePosition BoxLabelStartPosition;
        bool WrapFlag;
        FilePosition Cursor;

        NumChars = SelectedBox.Name.length();
        WrapFlag = NumChars > (SelectedBox.Width - (SelectedBox.Padding*2u));
        NumLines = NumChars / (SelectedBox.Width - (SelectedBox.Padding*2u));
        if (WrapFlag == true)
        {
            uint32_t CharIndex;
            bool ColumnOffsetFlag;

            ColumnOffsetFlag = false;
            BoxLabelStartPosition.Column = BoxTopLeft.Column + SelectedBox.Padding;
            BoxLabelStartPosition.Row = SelectedBox.Center.Row - (NumLines/2u);
            Cursor.Column = BoxLabelStartPosition.Column;
            Cursor.Row = BoxLabelStartPosition.Row;
            for (CharIndex = 0u; CharIndex < NumChars; CharIndex++)
            {
                Diagram[Cursor.Row][Cursor.Column] = SelectedBox.Name[CharIndex];
                Cursor.Column++;
                if (Cursor.Column > (BoxBottomRight.Column - SelectedBox.Padding))
                {
                    uint32_t CharsLeft;
                    
                    Cursor.Row++;
                    CharsLeft = (NumChars - CharIndex);
                    if (CharsLeft > (SelectedBox.Width-(SelectedBox.Padding*2u)))
                    {
                        Cursor.Column = BoxLabelStartPosition.Column;
                    }
                    else
                    {
                        uint32_t FinalLineWidth;

                        Cursor.Column = (SelectedBox.Center.Column - (CharsLeft/2u));
                    }
               }
            } 
        }
        else
        {
            uint32_t CharIndex;
            FilePosition BoxLabelEndPositon;

            BoxLabelStartPosition.Row = SelectedBox.Center.Row;
            BoxLabelStartPosition.Column = SelectedBox.Center.Column - (NumChars/2u);
            Cursor.Column = BoxLabelStartPosition.Column;
            Cursor.Row = BoxLabelStartPosition.Row;
            for (CharIndex = 0u; CharIndex < NumChars; CharIndex++)
            {
                Diagram[Cursor.Row][Cursor.Column] = SelectedBox.Name[CharIndex];
                Cursor.Column++;
            }
        }
        std::cout << "Drawing input stubs." << std::endl;
        for (const Stub &SelectedStub : SelectedBox.InputStubs)
        {
            uint32_t LabelLength;

            LabelLength = InputStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = (TargetStub.Position.Column - LabelLength) + CharIndex;
                CharRow = TargetStub.Position.Row-1u;
                Diagram[CharRow][CharColumn] = InputStub.Name[CharIndex];
            }
        }
        std::cout << "Done Drawing input stubs" << std::endl;
        for (const Stub &OutputStub : SelectedBox.OutputStubs)
        {
            uint32_t LabelLength;

            LabelLength = OutputStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = OutputStub.Position.Column + CharIndex + 1u;
                CharRow = OutputStub.Position.Row - 1u;
                Diagram[CharRow][CharColumn] = OutputStub.Name[CharIndex];
            }
        }
        std::cout << "Done drawing output stubs" << std::endl;
        uint32_t ControlStubCount;
        ControlStubCount = SelectedBox.ControlStubs.size();
        for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlStubCount; ControlStubIndex++)
        {
            const Stub &ControlStub = SelectedBox.ControlStubs[ControlStubIndex];
            uint32_t LabelLength;

            LabelLength = ControlStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = ControlStub.Position.Column + CharIndex;
                CharRow = ControlStub.Position.Row - 1u - ControlStubIndex;
                Diagram[CharRow][CharColumn] = ControlStub.Name[CharIndex];
            }
        }
        std::cout << "Done drawing control stubs" << std::endl;
        uint32_t MechanismStubCount;
        MechanismStubCount = SelectedBox.MechanismStubs.size();
        for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismStubCount; MechanismStubIndex++)
        {
            const Stub &MechanismStub = SelectedBox.MechanismStubs[MechanismStubIndex];
            uint32_t LabelLength;

            LabelLength = MechanismStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = MechanismStub.Position.Column + CharIndex;
                CharRow = MechanismStub.Position.Row + 1u + MechanismStubIndex;
                Diagram[CharRow][CharColumn] = MechanismStub.Name[CharIndex];
            }
        }
        std::cout << "Done drawing mechanism stubs" << std::endl;
        uint32_t CallStubCount;
        CallStubCount = SelectedBox.CallStubs.size();
        for (uint32_t CallStubIndex = 0u; CallStubIndex < CallStubCount; CallStubIndex++)
        {
            const Stub &CallStub = SelectedBox.CallStubs[CallStubIndex];
            uint32_t LabelLength;

            LabelLength = CallStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = CallStub.Position.Column + CharIndex;
                CharRow = CallStub.Position.Row + MechanismStubCount + 1u + CallStubIndex;
                Diagram[CharRow][CharColumn] = CallStub.Name[CharIndex];
            }
        }
        std::cout << "Done drawing call stubs" << std::endl;
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
            std::map<IDEF::Stub, Avoid::ConnEnd> BoxStubsMap;
            std::map<IDEF::Stub, Avoid::ConnEnd> BoundaryStubsMap;       
            std::vector<Avoid::Rectangle> Obstacles;
            Avoid::Router *Router;
            std::vector<std::string> Diagram;
            uint32_t RowNumber;

            IDEF::LayoutActivityDiagram(SelectedActivityDiagram, 400, 120);
            IDEF::PlaceObstacles(SelectedActivityDiagram, Obstacles);
            BoxStubsMap = IDEF::PlaceBoxStubConnEnds(SelectedActivityDiagram);
            BoundaryStubsMap = IDEF::PlaceBoundaryStubConnEnds(SelectedActivityDiagram);
            Router = IDEF::ConstructRouter(BoxStubsMap,BoundaryStubsMap, Obstacles);
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
