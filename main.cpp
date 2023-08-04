#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <variant>
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
    bool operator<(const InputStub& RHS) const
    {
        uint32_t ThisEncodedPosition;
        uint32_t RHSEncodedPosition;

        ThisEncodedPosition = Position.Column + (Position.Row * 1000u);
        RHSEncodedPosition = RHS.Position.Column + (RHS.Position.Row * 1000u);

        return ThisEncodedPosition < RHSEncodedPosition;
    }
};

struct OutputStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;

    bool operator<(const OutputStub& RHS) const
    {
        uint32_t ThisEncodedPosition;
        uint32_t RHSEncodedPosition;

        ThisEncodedPosition = Position.Column + (Position.Row * 1000u);
        RHSEncodedPosition = RHS.Position.Column + (RHS.Position.Row * 1000u);

        return ThisEncodedPosition < RHSEncodedPosition;
    }
};

struct ControlStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;

    bool operator<(const ControlStub& RHS) const
    {
        uint32_t ThisEncodedPosition;
        uint32_t RHSEncodedPosition;

        ThisEncodedPosition = Position.Column + (Position.Row * 1000u);
        RHSEncodedPosition = RHS.Position.Column + (RHS.Position.Row * 1000u);

        return ThisEncodedPosition < RHSEncodedPosition;
    }
};

struct MechanismStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;

    bool operator<(const MechanismStub& RHS) const
    {
        uint32_t ThisEncodedPosition;
        uint32_t RHSEncodedPosition;

        ThisEncodedPosition = Position.Column + (Position.Row * 1000u);
        RHSEncodedPosition = RHS.Position.Column + (RHS.Position.Row * 1000u);

        return ThisEncodedPosition < RHSEncodedPosition;
    }
};

struct CallStub
{
    std::string Name;
    FilePosition Position;
    std::vector<StubSource> Sources;

    bool operator<(const CallStub& RHS) const
    {
        uint32_t ThisEncodedPosition;
        uint32_t RHSEncodedPosition;

        ThisEncodedPosition = Position.Column + (Position.Row * 1000u);
        RHSEncodedPosition = RHS.Position.Column + (RHS.Position.Row * 1000u);

        return ThisEncodedPosition < RHSEncodedPosition;
    }
};

typedef std::variant<InputStub, OutputStub, ControlStub, MechanismStub, CallStub> Stub;

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

typedef std::variant<NodeNumberSection, TitleSection, CNumberSection> DiagramSection;

struct DiagramBar
{
    FilePosition TopLeft;
    uint32_t Height;
    DiagramSection NodeNumberSection;
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
    DiagramFrame Frame;
};

struct Model
{
    std::string Title;
    std::vector<ActivityDiagram> ActivityDiagrams;
};

InputStub LoadInputStub(const pugi::xml_node& InputStubXMLNode)
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

OutputStub LoadOutputStub(const pugi::xml_node& OutputStubXMLNode)
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

ControlStub LoadControlStub(const pugi::xml_node& ControlStubXMLNode)
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

MechanismStub LoadMechanismStub(const pugi::xml_node& MechanismStubXMLNode)
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

CallStub LoadCallStub(const pugi::xml_node& CallStubXMLNode)
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

    NewDiagram.Frame.BottomBar.NodeNumberSection = NodeNumberSection();
    std::get<NodeNumberSection>(NewDiagram.Frame.BottomBar.NodeNumberSection).Content = ActivityDiagramNode.attribute("Number").as_string();
    std::get<NodeNumberSection>(NewDiagram.Frame.BottomBar.NodeNumberSection).TopLeft.Row = 0u;
    std::get<NodeNumberSection>(NewDiagram.Frame.BottomBar.NodeNumberSection).TopLeft.Column = 0u;
    NewDiagram.Frame.BottomBar.TitleSection = TitleSection();
    std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection).Content = ActivityDiagramNode.attribute("Title").as_string();
    std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection).TopLeft.Row = 0u;
    std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection).TopLeft.Column = 0u;
    NewDiagram.Frame.BottomBar.CNumberSection = CNumberSection();
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
            NewDiagram.InputBoundaryStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Output") == 0)
        {
            NewStub = LoadOutputStub(ChildXMLNode);
            NewDiagram.OutputBoundaryStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Control") == 0)
        {
            NewStub = LoadControlStub(ChildXMLNode);
            NewDiagram.ControlBoundaryStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Mechanism") == 0)
        {
            NewStub = LoadMechanismStub(ChildXMLNode);
            NewDiagram.MechanismBoundaryStubs.push_back(NewStub);
        }
        else if (strcmp(ChildXMLNode.name(), "Call") == 0)
        {
            NewStub = LoadCallStub(ChildXMLNode);
            NewDiagram.CallBoundaryStubs.push_back(NewStub);
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
    std::get<NodeNumberSection>(Diagram.Frame.BottomBar.NodeNumberSection).Width = Diagram.Width / 3u;
    std::get<NodeNumberSection>(Diagram.Frame.BottomBar.NodeNumberSection).TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    std::get<NodeNumberSection>(Diagram.Frame.BottomBar.NodeNumberSection).TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column;
    std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).Width = Diagram.Width / 3u;
    std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column + std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection).Width;
    std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).Width = Diagram.Width / 3u;
    std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column + std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection).Width;
}

void LayoutBoxes(ActivityDiagram &Diagram, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin)
{
    uint32_t NumBoxes;
    FilePosition Cursor;
    uint32_t ColumnCenterOffset;
    uint32_t RowCenterOffset;
    uint32_t RowHeight;
    uint32_t ColumnWidth;
    uint32_t BoxSectionHeight;
    uint32_t BoxSectionWidth;
    
    NumBoxes = Diagram.Boxes.size();
    BoxSectionHeight = (NumBoxes * BoxHeight) + (NumBoxes * BoxMargin);
    BoxSectionWidth = (NumBoxes * BoxWidth) + (NumBoxes * BoxMargin);
    RowHeight = BoxSectionHeight / (1u+NumBoxes);
    ColumnWidth = BoxSectionWidth / (1u+NumBoxes);
    ColumnCenterOffset = (Diagram.Width / 2u) - (BoxSectionWidth / 2u);
    RowCenterOffset = (Diagram.Height / 2u) - (BoxSectionHeight / 2u);
    std::cout << "Diagram width=" << Diagram.Width << ", Box Section Width = " << BoxSectionWidth << std::endl;
    std::cout << "Diagram height=" << Diagram.Height << ", Box Section Height = " << BoxSectionHeight << std::endl;
    Cursor.Column = ColumnCenterOffset;
    Cursor.Row = RowCenterOffset;
    Cursor.Column += (BoxWidth/2u);
    Cursor.Row += (BoxHeight/2u);
    for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
    {
        ActivityBox& SelectedBox = Diagram.Boxes[BoxIndex];
        
        SelectedBox.Width = BoxWidth;
        SelectedBox.Height = BoxHeight;
        SelectedBox.Center.Column = Cursor.Column;
        SelectedBox.Center.Row = Cursor.Row;
        std::cout << "Center Column: " << SelectedBox.Center.Column << "." << std::endl;
        std::cout << "Center Row: " << SelectedBox.Center.Row << "." << std::endl;
        Cursor.Column += BoxWidth + BoxMargin;
        Cursor.Row += BoxHeight + BoxMargin;
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

void FindInnerStub(const ActivityDiagram& Diagram, 
    const Stub& BoundaryStub, 
    Stub& FoundStub, 
    bool& FoundFlag)
{
    FoundFlag = false;
    for (const ActivityBox& SelectedBox : Diagram.Boxes)
    {
        if (std::holds_alternative<InputStub>(BoundaryStub))
        {
            const InputStub& BoundaryInputStub = std::get<InputStub>(BoundaryStub);
            
            for (const Stub& SelectedStub : SelectedBox.InputStubs)
            {
                const InputStub& SelectedInputStub = std::get<InputStub>(SelectedStub);
                
                if (SelectedInputStub.Name == BoundaryInputStub.Name)
                {
                    FoundStub = SelectedInputStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& StubSource : SelectedInputStub.Sources)
                    {
                        if (StubSource.StubName == BoundaryInputStub.Name)
                        {
                            FoundStub = SelectedInputStub;
                            FoundFlag = true;
                        }
                    }
                }
            }   
            for (const Stub& SelectedStub : SelectedBox.ControlStubs)
            {
                const ControlStub& SelectedControlStub = std::get<ControlStub>(SelectedStub);
                
                if (SelectedControlStub.Name == BoundaryInputStub.Name)
                {
                    FoundStub = SelectedControlStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& StubSource : SelectedControlStub.Sources)
                    {
                        if (StubSource.StubName == BoundaryInputStub.Name)
                        {
                            FoundStub = SelectedControlStub;
                            FoundFlag = true;
                        }
                    }
                }
            }
        }
        else if (std::holds_alternative<ControlStub>(BoundaryStub))
        {
            const ControlStub& BoundaryControlStub = std::get<ControlStub>(BoundaryStub);
            
            for (const Stub& SelectedStub : SelectedBox.InputStubs)
            {
                const InputStub& SelectedInputStub = std::get<InputStub>(SelectedStub);
                
                if (SelectedInputStub.Name == BoundaryControlStub.Name)
                {
                    FoundStub = SelectedInputStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& SelectedStubSource : SelectedInputStub.Sources)
                    {
                        if (SelectedStubSource.StubName == BoundaryControlStub.Name)
                        {
                            FoundStub = SelectedInputStub;
                            FoundFlag = true;
                        }
                    }
                }
            }
            for (const Stub& SelectedStub : SelectedBox.ControlStubs)
            {
                const ControlStub& SelectedControlStub = std::get<ControlStub>(SelectedStub);

                if (SelectedControlStub.Name == BoundaryControlStub.Name)
                {
                    FoundStub = SelectedControlStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& SelectedStubSource : SelectedControlStub.Sources)
                    {
                        if (SelectedStubSource.StubName == BoundaryControlStub.Name)
                        {
                            FoundStub = SelectedControlStub;
                            FoundFlag = true;
                        }
                    }
                }
            }
        }
        else if (std::holds_alternative<OutputStub>(BoundaryStub))
        {
            const OutputStub& BoundaryOutputStub = std::get<OutputStub>(BoundaryStub);
            
            for (const Stub& SelectedStub : SelectedBox.OutputStubs)
            {
                const OutputStub& SelectedOutputStub = std::get<OutputStub>(SelectedStub);

                if (SelectedOutputStub.Name == BoundaryOutputStub.Name)
                {
                    FoundStub = SelectedOutputStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& BoundaryStubSource : BoundaryOutputStub.Sources)
                    {
                        if (BoundaryStubSource.StubName == SelectedOutputStub.Name)
                        {
                            FoundStub = SelectedOutputStub;
                            FoundFlag = true;
                        }
                    } 
                }
            }    
        }
        else if (std::holds_alternative<MechanismStub>(BoundaryStub))
        {
            const MechanismStub& BoundaryMechanismStub = std::get<MechanismStub>(BoundaryStub);

            for (const Stub& SelectedStub : SelectedBox.MechanismStubs)
            {
                const MechanismStub& BoxMechanismStub = std::get<MechanismStub>(SelectedStub);

                if (BoundaryMechanismStub.Name == BoxMechanismStub.Name)
                {
                    FoundStub = BoxMechanismStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& BoxStubSource : BoxMechanismStub.Sources)
                    {
                        if (BoxStubSource.StubName == BoundaryMechanismStub.Name)
                        {
                            FoundStub = BoxMechanismStub;
                            FoundFlag = true;
                        }
                    }
                }
            }
        }
        else if (std::holds_alternative<CallStub>(BoundaryStub))
        {
            const CallStub& BoundaryCallStub = std::get<CallStub>(BoundaryStub);

            for (const Stub& SelectedStub : SelectedBox.CallStubs)
            {
                const CallStub& BoxCallStub = std::get<CallStub>(SelectedStub);

                if (BoundaryCallStub.Name == BoxCallStub.Name)
                {
                    FoundStub = BoxCallStub;
                    FoundFlag = true;
                }
                else
                {
                    for (const StubSource& BoundaryStubSource : BoundaryCallStub.Sources)
                    {
                        if (BoundaryStubSource.StubName == BoxCallStub.Name)
                        {
                            FoundStub = BoxCallStub;
                            FoundFlag = true;
                        }
                    }
                }
            }
        }
        if (FoundFlag == true)
        {
            break;
        }
    }
}

void LayoutBoundaryStubs(ActivityDiagram &Diagram, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin)
{
    uint32_t BoxSectionHeight;
    uint32_t BoxSectionWidth;
    uint32_t RowHeight;
    uint32_t ColumnWidth;
    uint32_t NumBoxes;
    uint32_t StubIndex;
    uint32_t ColumnCenterOffset;
    uint32_t RowCenterOffset;
    
    NumBoxes = Diagram.Boxes.size();
    BoxSectionHeight = (NumBoxes * BoxHeight) + (NumBoxes * BoxMargin);
    BoxSectionWidth = (NumBoxes * BoxWidth) + (NumBoxes * BoxMargin);
    RowHeight = BoxSectionHeight / (1u+NumBoxes);
    ColumnWidth = BoxSectionWidth / (1u+NumBoxes);
    ColumnCenterOffset = (Diagram.Width / 2u) - (BoxSectionWidth / 2u);
    RowCenterOffset = (Diagram.Height / 2u) - (BoxSectionHeight / 2u);
    StubIndex = 0u;
    for (Stub& BoundaryStub : Diagram.InputBoundaryStubs)
    {
        Stub FoundStub;
        bool FoundFlag;
        InputStub& BoundaryInputStub = std::get<InputStub>(BoundaryStub);
        
        FindInnerStub(Diagram, BoundaryInputStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<InputStub>(FoundStub))
            {
                InputStub& FoundInputStub = std::get<InputStub>(FoundStub);
                
                BoundaryInputStub.Position.Row = FoundInputStub.Position.Row;
                BoundaryInputStub.Position.Column = 0u;
            }
            else if (std::holds_alternative<ControlStub>(FoundStub))
            {
                ControlStub& FoundControlStub = std::get<ControlStub>(FoundStub);

                BoundaryInputStub.Position.Row = 0u;
                BoundaryInputStub.Position.Column = FoundControlStub.Position.Column;
            }
        }
        else
        {
            BoundaryInputStub.Position.Column = 0u;
            BoundaryInputStub.Position.Row = (1u+StubIndex) * RowHeight;
        }
        StubIndex++;
    }
    StubIndex = 0u;
    for (Stub& BoundaryStub : Diagram.ControlBoundaryStubs)
    {
        Stub FoundStub;
        bool FoundFlag;
        ControlStub& BoundaryControlStub = std::get<ControlStub>(BoundaryStub);

        FindInnerStub(Diagram, BoundaryControlStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<InputStub>(FoundStub))
            {
                InputStub& FoundInputStub = std::get<InputStub>(FoundStub);
                
                BoundaryControlStub.Position.Row = 0u;
                BoundaryControlStub.Position.Column = FoundInputStub.Position.Column - (BoxMargin/2u);
            }
            else if (std::holds_alternative<ControlStub>(FoundStub))
            {
                ControlStub& FoundControlStub = std::get<ControlStub>(FoundStub);

                BoundaryControlStub.Position.Row = 0u;
                BoundaryControlStub.Position.Column = FoundControlStub.Position.Column;
            }
        }
        else
        {
            BoundaryControlStub.Position.Column = (1u+StubIndex) * ColumnWidth;
            BoundaryControlStub.Position.Row = 0u;
        }
        StubIndex++;
    }
    StubIndex = 0u;
    for (Stub& BoundaryStub : Diagram.OutputBoundaryStubs)
    {
        Stub FoundStub;
        bool FoundFlag;
        OutputStub& BoundaryOutputStub = std::get<OutputStub>(BoundaryStub);

        FindInnerStub(Diagram, BoundaryOutputStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<OutputStub>(FoundStub))
            {
                OutputStub& FoundOutputStub = std::get<OutputStub>(FoundStub);
                
                BoundaryOutputStub.Position.Row = FoundOutputStub.Position.Row;
                BoundaryOutputStub.Position.Column = Diagram.Width - 1u;
            }
        }
        else
        {
            BoundaryOutputStub.Position.Column = Diagram.Width - 1u;
            BoundaryOutputStub.Position.Row = (1u+StubIndex) * RowHeight;
        }
        StubIndex++;
    }
    StubIndex = 0u;
    for (Stub& BoundaryStub : Diagram.MechanismBoundaryStubs)
    {
        Stub FoundStub;
        bool FoundFlag;
        MechanismStub& BoundaryMechanismStub = std::get<MechanismStub>(BoundaryStub);

        FindInnerStub(Diagram, BoundaryMechanismStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<MechanismStub>(FoundStub))
            {
                MechanismStub& FoundMechanismStub = std::get<MechanismStub>(FoundStub);
                
                BoundaryMechanismStub.Position.Row = BoxSectionHeight - 1u;
                BoundaryMechanismStub.Position.Column = FoundMechanismStub.Position.Column;
            }
        }
        else
        {
            BoundaryMechanismStub.Position.Column = (1u+StubIndex) * ColumnWidth;
            BoundaryMechanismStub.Position.Row = BoxSectionHeight - 1u;
        }
        StubIndex++;
    }
    StubIndex = 0u;
    for (Stub& BoundaryStub : Diagram.CallBoundaryStubs)
    {
        Stub FoundStub;
        bool FoundFlag;
        CallStub& BoundaryCallStub = std::get<CallStub>(BoundaryStub);

        FindInnerStub(Diagram, BoundaryCallStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<CallStub>(FoundStub))
            {
                CallStub& FoundCallStub = std::get<CallStub>(FoundStub);
                
                BoundaryCallStub.Position.Row = BoxSectionHeight - 1u;
                BoundaryCallStub.Position.Column = FoundCallStub.Position.Column;
            }
        }
        else
        {
            BoundaryCallStub.Position.Column = (1u+StubIndex) * ColumnWidth;
            BoundaryCallStub.Position.Row = BoxSectionHeight - 1u;
        }
        StubIndex++;
    }
}

void LayoutActivityDiagram(ActivityDiagram &LoadedDiagram, 
    uint32_t Width, uint32_t Height, 
    uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin)
{
    LoadedDiagram.Width = Width;
    LoadedDiagram.Height = Height;
    LayoutFrame(LoadedDiagram);
    LayoutBoxes(LoadedDiagram, BoxWidth, BoxHeight, BoxMargin);
    LayoutBoxStubs(LoadedDiagram);
    LayoutBoundaryStubs(LoadedDiagram, BoxWidth, BoxHeight, BoxMargin);
} 

std::map<Stub, Avoid::ConnEnd> PlaceBoxStubConnEnds(const ActivityDiagram& LayedOutDiagram, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin)
{
    const uint32_t StubLength = 3u;
    std::map<Stub, Avoid::ConnEnd> BoxStubsMap;
    
    for (const ActivityBox& SelectedBox : LayedOutDiagram.Boxes)
    {
        for (Stub SelectedStub : SelectedBox.InputStubs)
        {
            uint32_t StubColumn;
            uint32_t StubRow;
            uint32_t AvoidX;
            uint32_t AvoidY;
            InputStub SelectedInputStub;
            Avoid::ConnEnd ConnectionEnd;

            SelectedInputStub = std::get<InputStub>(SelectedStub);
            StubColumn = SelectedInputStub.Position.Column - StubLength;
            StubRow = SelectedInputStub.Position.Row;
            AvoidX = StubColumn;
            AvoidY = LayedOutDiagram.Height - StubRow;
            ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
            BoxStubsMap.insert({SelectedStub, ConnectionEnd});
        }
        for (Stub SelectedStub : SelectedBox.OutputStubs)
        {
            uint32_t StubColumn;
            uint32_t StubRow;
            uint32_t AvoidX;
            uint32_t AvoidY;
            OutputStub SelectedOutputStub;
            Avoid::ConnEnd ConnectionEnd;
           
            SelectedOutputStub = std::get<OutputStub>(SelectedStub);
            StubColumn = SelectedOutputStub.Position.Column + StubLength;
            StubRow = SelectedOutputStub.Position.Row;
            AvoidX = StubColumn;
            AvoidY = LayedOutDiagram.Height - StubRow;
            ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
            BoxStubsMap.insert({SelectedStub, ConnectionEnd});
        }
        for (Stub SelectedStub: SelectedBox.ControlStubs)
        {
            uint32_t StubColumn;
            uint32_t StubRow;
            uint32_t AvoidX;
            uint32_t AvoidY;
            ControlStub SelectedControlStub;
            Avoid::ConnEnd ConnectionEnd;

            SelectedControlStub = std::get<ControlStub>(SelectedStub);
            StubColumn = SelectedControlStub.Position.Column;
            StubRow = SelectedControlStub.Position.Row - StubLength;
            AvoidX = StubColumn;
            AvoidY = LayedOutDiagram.Height - StubRow;
            ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
            BoxStubsMap.insert({SelectedStub, ConnectionEnd});
        }
        for (Stub SelectedStub : SelectedBox.MechanismStubs)
        {
            uint32_t StubColumn;
            uint32_t StubRow;
            uint32_t AvoidX;
            uint32_t AvoidY;
            Avoid::ConnEnd ConnectionEnd;
            MechanismStub SelectedMechanismStub;

            SelectedMechanismStub = std::get<MechanismStub>(SelectedStub);
            StubColumn = SelectedMechanismStub.Position.Column;
            StubRow = SelectedMechanismStub.Position.Row + StubLength;
            AvoidX = StubColumn;
            AvoidY = LayedOutDiagram.Height - StubRow;
            ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
            BoxStubsMap.insert({SelectedStub, ConnectionEnd});
        }
        for (Stub SelectedStub : SelectedBox.CallStubs)
        {
            uint32_t StubColumn;
            uint32_t StubRow;
            uint32_t AvoidX;
            uint32_t AvoidY;
            Avoid::ConnEnd ConnectionEnd;
            CallStub SelectedCallStub;

            SelectedCallStub = std::get<CallStub>(SelectedStub);
            StubColumn = SelectedCallStub.Position.Column;
            StubRow = SelectedCallStub.Position.Row + StubLength;
            AvoidX = StubColumn;
            AvoidY = LayedOutDiagram.Height - StubRow;
            ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
            BoxStubsMap.insert({SelectedStub, ConnectionEnd});
        } 
    }

    return BoxStubsMap;
}

std::map<Stub, Avoid::ConnEnd> PlaceBoundaryStubConnEnds(const ActivityDiagram& LayedOutDiagram, const uint32_t BoxWidth, const uint32_t BoxHeight, const uint32_t BoxMargin)
{
    const uint32_t StubLength = 3u;
    std::map<Stub, Avoid::ConnEnd> BoundaryStubsMap;

    for (const Stub& BoundaryStub : LayedOutDiagram.InputBoundaryStubs)
    {
        uint32_t StubColumn;
        uint32_t StubRow;
        uint32_t AvoidX;
        uint32_t AvoidY;
        const InputStub& BoundaryInputStub = std::get<InputStub>(BoundaryStub);
        Avoid::ConnEnd ConnectionEnd;

        StubColumn = BoundaryInputStub.Position.Column + StubLength;
        StubRow = BoundaryInputStub.Position.Row;
        AvoidX = StubColumn;
        AvoidY = LayedOutDiagram.Height - StubRow;
        ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
        BoundaryStubsMap.insert({BoundaryInputStub, ConnectionEnd});
    }
    for (const Stub BoundaryStub : LayedOutDiagram.OutputBoundaryStubs)
    {
        uint32_t StubColumn;
        uint32_t StubRow;
        uint32_t AvoidX;
        uint32_t AvoidY;
        const OutputStub& BoundaryOutputStub = std::get<OutputStub>(BoundaryStub);
        Avoid::ConnEnd ConnectionEnd;

        StubColumn = BoundaryOutputStub.Position.Column - StubLength;
        StubRow = BoundaryOutputStub.Position.Row;
        AvoidX = StubColumn;
        AvoidY = LayedOutDiagram.Height - StubRow;
        ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
        BoundaryStubsMap.insert({BoundaryOutputStub, ConnectionEnd});
    }
    for (const Stub BoundaryStub : LayedOutDiagram.ControlBoundaryStubs)
    {
        uint32_t StubColumn;
        uint32_t StubRow;
        uint32_t AvoidX;
        uint32_t AvoidY;
        const ControlStub& BoundaryControlStub = std::get<ControlStub>(BoundaryStub);
        Avoid::ConnEnd ConnectionEnd;
   
        StubColumn = BoundaryControlStub.Position.Column;
        StubRow = BoundaryControlStub.Position.Row + StubLength;
        AvoidX = StubColumn;
        AvoidY = LayedOutDiagram.Height - StubRow;
        ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
        BoundaryStubsMap.insert({BoundaryControlStub, ConnectionEnd});
    }
    for (const Stub BoundaryStub : LayedOutDiagram.MechanismBoundaryStubs)
    {
        uint32_t StubColumn;
        uint32_t StubRow;
        uint32_t AvoidX;
        uint32_t AvoidY;
        const MechanismStub& BoundaryMechanismStub = std::get<MechanismStub>(BoundaryStub);
        Avoid::ConnEnd ConnectionEnd;

        StubColumn = BoundaryMechanismStub.Position.Column;
        StubRow = BoundaryMechanismStub.Position.Row - StubLength;
        AvoidX = StubColumn;
        AvoidY = LayedOutDiagram.Height - StubRow;
        ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
        BoundaryStubsMap.insert({BoundaryMechanismStub, ConnectionEnd});
    }
    for (const Stub BoundaryStub : LayedOutDiagram.CallBoundaryStubs)
    {
        uint32_t StubColumn;
        uint32_t StubRow;
        uint32_t AvoidX;
        uint32_t AvoidY;
        const CallStub& BoundaryCallStub = std::get<CallStub>(BoundaryStub);
        Avoid::ConnEnd ConnectionEnd;

        StubColumn = BoundaryCallStub.Position.Column;
        StubRow = BoundaryCallStub.Position.Row - StubLength;
        AvoidX = StubColumn;
        AvoidY = LayedOutDiagram.Height - StubRow;
        ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
        BoundaryStubsMap.insert({BoundaryCallStub, ConnectionEnd});
    }
   
    return BoundaryStubsMap;
}

void PlaceObstacles(const ActivityDiagram &LayedoutDiagram, std::vector<Avoid::Rectangle> &Rectangles, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin)
{
    uint32_t NumBoxes;

    NumBoxes = LayedoutDiagram.Boxes.size();
    for (uint32_t BoxIndex = 0u; BoxIndex < NumBoxes; BoxIndex++)
    {
        const ActivityBox &SelectedBox = LayedoutDiagram.Boxes[BoxIndex];
        FilePosition BoxTopLeft;
        FilePosition BoxBottomRight;
        Avoid::Point BoxTopLeftAvoid;
        Avoid::Point BoxBottomRightAvoid;
        Avoid::Rectangle NewRect(Avoid::Point(0,0), Avoid::Point(0,0));

        BoxTopLeft.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
        BoxTopLeft.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
        BoxBottomRight.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
        BoxBottomRight.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
        BoxTopLeftAvoid.x = BoxTopLeft.Column;
        BoxTopLeftAvoid.y = LayedoutDiagram.Height - BoxTopLeft.Row;
        BoxBottomRightAvoid.x = BoxBottomRight.Column;
        BoxBottomRightAvoid.y = LayedoutDiagram.Height - BoxBottomRight.Row;
        NewRect = Avoid::Rectangle(BoxTopLeftAvoid, BoxBottomRightAvoid);
        std::cout << "Making a rectangle at: " << std::endl;
        std::cout << "  Top Left (" << BoxTopLeft.Column << ", " << BoxTopLeft.Row << ")" << std::endl;
        std::cout << "  Bottom Right (" << BoxBottomRight.Column << ", " << BoxBottomRight.Row << ")" << std::endl;
        std::cout << "  Avoid Top Left (" << BoxTopLeftAvoid.x << "," << BoxTopLeftAvoid.y << ")" << std::endl;
        std::cout << "  Avoid Bottom Right (" << BoxBottomRightAvoid.x << "," << BoxBottomRightAvoid.y << ")" << std::endl;
        Rectangles.push_back(NewRect);
    }
}

Avoid::Router *ConstructRouter(std::map<Stub, Avoid::ConnEnd> &BoxStubsMap,
                               std::map<Stub, Avoid::ConnEnd>& BoundaryStubsMap,
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
    // This section connects boundary stubs to box stubs.
    // TODO move this section into a procedure.
    for (const std::pair<Stub, Avoid::ConnEnd> &BoundaryStubPair : BoundaryStubsMap)
    {
        const Stub& BoundaryStub = BoundaryStubPair.first;

        for (const std::pair<Stub, Avoid::ConnEnd>& BoxStubPair : BoxStubsMap)
        {
            const Stub& BoxStub = BoxStubPair.first;
              
            if (std::holds_alternative<InputStub>(BoundaryStub))
            {
                const InputStub& BoundaryInputStub = std::get<InputStub>(BoundaryStub);

                if (std::holds_alternative<InputStub>(BoxStub))
                {
                    const InputStub& BoxInputStub = std::get<InputStub>(BoxStub);

                    if (BoxInputStub.Name == BoundaryInputStub.Name)
                    {
                        Avoid::ConnRef* NewConn;
                        
                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& BoxInputSource : BoxInputStub.Sources)
                        {
                            if (BoxInputSource.StubName == BoundaryInputStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                                
                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
                else if (std::holds_alternative<ControlStub>(BoxStub))
                {
                    const ControlStub& BoxControlStub = std::get<ControlStub>(BoxStub);

                    if (BoxControlStub.Name == BoundaryInputStub.Name)
                    {
                        Avoid::ConnRef* NewConn;
                        
                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& BoxControlSource : BoxControlStub.Sources)
                        {
                            if (BoxControlSource.StubName == BoundaryInputStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                                
                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
            }
            else if (std::holds_alternative<ControlStub>(BoundaryStub))
            {
                const ControlStub& BoundaryControlStub = std::get<ControlStub>(BoundaryStub);

                if (std::holds_alternative<InputStub>(BoxStub))
                {
                    const InputStub& BoxInputStub = std::get<InputStub>(BoxStub);

                    if (BoxInputStub.Name == BoundaryControlStub.Name)
                    { 
                        Avoid::ConnRef* NewConn;

                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& BoxInputSource : BoxInputStub.Sources)
                        {
                            if (BoxInputSource.StubName == BoundaryControlStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                             
                                std::cout << "Adding a connection ref" << std::endl;  
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl; 
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
                else if (std::holds_alternative<ControlStub>(BoxStub))
                {
                    const ControlStub& BoxControlStub = std::get<ControlStub>(BoxStub);

                    if (BoxControlStub.Name == BoundaryControlStub.Name)
                    {
                        Avoid::ConnRef* NewConn;

                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& BoxControlSource : BoxControlStub.Sources)
                        {
                            if (BoxControlSource.StubName == BoundaryControlStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                          
                                std::cout << "Adding a connection ref" << std::endl;      
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
            }
            else if (std::holds_alternative<MechanismStub>(BoundaryStub))
            {
                const MechanismStub& BoundaryMechanismStub = std::get<MechanismStub>(BoundaryStub);

                if (std::holds_alternative<MechanismStub>(BoxStub))
                {
                    const MechanismStub& BoxMechanismStub = std::get<MechanismStub>(BoxStub);

                    if (BoxMechanismStub.Name == BoundaryMechanismStub.Name)
                    { 
                        Avoid::ConnRef* NewConn;

                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& BoxMechanismSource : BoxMechanismStub.Sources)
                        {
                            if (BoxMechanismSource.StubName == BoundaryMechanismStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                                
                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoundaryStubPair.second.position().x << "," << BoundaryStubPair.second.position().y << ")";
                        std::cout << " to (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")" << std::endl;
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    // This section connects box stubs to other boxes and boundary stubs.
    for (const std::pair<Stub, Avoid::ConnEnd> &BoxStubPair : BoxStubsMap)
    {
        const Stub& FirstStub = BoxStubPair.first;

        for (const std::pair<Stub, Avoid::ConnEnd> &OtherBoxStubPair : BoxStubsMap)
        {
            const Stub& OtherStub = OtherBoxStubPair.first;

            if (std::holds_alternative<OutputStub>(FirstStub))
            {
                const OutputStub& FirstOutputStub = std::get<OutputStub>(FirstStub);
                
                if (std::holds_alternative<InputStub>(OtherStub))
                {
                    const InputStub& OtherInputStub = std::get<InputStub>(OtherStub);
    
                    if (FirstOutputStub.Name == OtherInputStub.Name)
                    {
                        Avoid::ConnRef* NewRef;
    
                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << OtherBoxStubPair.second.position().x << "," << OtherBoxStubPair.second.position().y << ")" << std::endl;
                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& Source : OtherInputStub.Sources)
                        {
                            if (Source.StubName == FirstOutputStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << OtherBoxStubPair.second.position().x << "," << OtherBoxStubPair.second.position().y << ")" << std::endl;
                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
                else if (std::holds_alternative<ControlStub>(OtherStub))
                {
                    const ControlStub& OtherControlStub = std::get<ControlStub>(OtherStub);

                    if (FirstOutputStub.Name == OtherControlStub.Name)
                    {
                        Avoid::ConnRef* NewRef;
                        
                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << OtherBoxStubPair.second.position().x << "," << OtherBoxStubPair.second.position().y << ")" << std::endl;
                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& Source : OtherControlStub.Sources)
                        {
                            if (Source.StubName == FirstOutputStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << OtherBoxStubPair.second.position().x << "," << OtherBoxStubPair.second.position().y << ")" << std::endl;
                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
            }
        }
        for (const std::pair<Stub, Avoid::ConnEnd> &BoundaryBoxStubPair : BoundaryStubsMap)
        {
            const Stub& BoundaryStub = BoundaryBoxStubPair.first;

            if (std::holds_alternative<OutputStub>(FirstStub))
            {
                const OutputStub& FirstOutputStub = std::get<OutputStub>(FirstStub);

                if (std::holds_alternative<OutputStub>(BoundaryStub))
                {
                    const OutputStub& BoundaryOutputStub = std::get<OutputStub>(BoundaryStub);

                    if (FirstOutputStub.Name == BoundaryOutputStub.Name)
                    {
                        Avoid::ConnRef* NewRef;

                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << BoundaryBoxStubPair.second.position().x << "," << BoundaryBoxStubPair.second.position().y << ")" << std::endl;
                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& Source : BoundaryOutputStub.Sources)
                        {
                            if (Source.StubName == FirstOutputStub.Name)
                            {
                                Avoid::ConnRef* NewRef;
    
                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << BoundaryBoxStubPair.second.position().x << "," << BoundaryBoxStubPair.second.position().y << ")" << std::endl;
                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
            }
            else if (std::holds_alternative<CallStub>(FirstStub))
            {
                const CallStub& FirstCallStub = std::get<CallStub>(FirstStub);

                if (std::holds_alternative<CallStub>(BoundaryStub))
                {
                    const CallStub& BoundaryCallStub = std::get<CallStub>(BoundaryStub);

                    if (FirstCallStub.Name == BoundaryCallStub.Name)
                    {
                        Avoid::ConnRef* NewRef;

                        std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << BoundaryBoxStubPair.second.position().x << "," << BoundaryBoxStubPair.second.position().y << ")" << std::endl;
                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                    }
                    else
                    {
                        for (const StubSource& Source : BoundaryCallStub.Sources)
                        {
                            if (Source.StubName == FirstCallStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                std::cout << "Adding a connection ref" << std::endl;
                        std::cout << "From (" << BoxStubPair.second.position().x << "," << BoxStubPair.second.position().y << ")";
                        std::cout << " to (" << BoundaryBoxStubPair.second.position().x << "," << BoundaryBoxStubPair.second.position().y << ")" << std::endl;
                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    Avoid::ConnRefList ConnRefs = ConstructedRouter->connRefs;
    std::cout << ConnRefs.size() << std::endl;
    for (Avoid::ConnRef* ConnRef : ConnRefs)
    {
        std::pair<Avoid::ConnEnd, Avoid::ConnEnd> ConnEnds = ConnRef->endpointConnEnds();
        std::cout << "From " << ConnEnds.first.position().x << "," << ConnEnds.first.position().y << ")";
        std::cout << "To " << ConnEnds.second.position().x << "," << ConnEnds.second.position(). y << ")" << std::endl;
    }
    std::cout << "Routing connections" << std::endl;
    ConstructedRouter->processTransaction();
    
    return ConstructedRouter;
}

std::vector<std::string> DrawDiagram(const ActivityDiagram &TargetDiagram, Avoid::Router *ConnectedRouter, uint32_t BoxHeight, uint32_t BoxWidth, uint32_t BoxMargin)
{
    std::vector<std::string> Diagram;
    uint32_t ActivityBoxNum;
    std::string BlankDiagramRow(TargetDiagram.Width, ' ');
    uint32_t NumBoxes;

    NumBoxes = TargetDiagram.Boxes.size();
    for (uint32_t RowNum = 0u; RowNum < TargetDiagram.Height; RowNum++)
    {
        Diagram.push_back(BlankDiagramRow);
    }
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
            std::cout << "First point (" << FirstPoint.x << "," << FirstPoint.y << ")" << std::endl;
            std::cout << "Second point (" << SecondPoint.x << "," << SecondPoint.y << ")" << std::endl;
            LineStartPoint.Column = (uint32_t)(round(FirstPoint.x));
            LineStartPoint.Row = TargetDiagram.Height - (uint32_t)(round(FirstPoint.y));
            LineEndPoint.Column = (uint32_t)(round(SecondPoint.x));
            LineEndPoint.Row = TargetDiagram.Height - (uint32_t)(round(SecondPoint.y));
            std::cout << "Line Start Pont (Column:" << LineStartPoint.Column << ", Row:" << LineStartPoint.Row << ")" << std::endl;
            std::cout << "Line End Point (Column:" << LineEndPoint.Column << ", Row:" << LineEndPoint.Row << ")" << std::endl;
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
                for (CursorRow = LineStartPoint.Row; CursorRow <= LineEndPoint.Row; CursorRow++)
                {
                    if (Diagram[CursorRow][CursorColumn] == '+')
                    {
                        continue;
                    } 
                    else
                    {
                        if (CursorRow == LineEndPoint.Row)
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else if ( (CursorRow == LineStartPoint.Row) && (CursorRow == LineStartPoint.Column) )
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else
                        {
                            Diagram[CursorRow][CursorColumn] = '|';
                        }
                       
                    }
                }
            }
            else if (TravelDir == "Up")
            {
                uint32_t CursorRow;
                uint32_t CursorColumn;

                CursorColumn = LineStartPoint.Column;
                for (CursorRow = LineStartPoint.Row; CursorRow >= LineEndPoint.Row; CursorRow--)
                {
                    if (Diagram[CursorRow][CursorColumn] == '+')
                    {
                        continue;
                    } 
                    else
                    {
                        if (CursorRow == LineEndPoint.Row)
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else if ( CursorRow == LineStartPoint.Row)
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else
                        {
                            Diagram[CursorRow][CursorColumn] = '|';
                        }
                    }
                }
            }
            else if (TravelDir == "Right")
            {
                uint32_t CursorRow;
                uint32_t CursorColumn;

                CursorRow = LineStartPoint.Row;
                for (CursorColumn = LineStartPoint.Column; CursorColumn <= LineEndPoint.Column; CursorColumn++)
                {
                    if (Diagram[CursorRow][CursorColumn] == '+')
                    {
                        continue;
                    } 
                    else
                    {
                        if (CursorColumn == LineEndPoint.Column)
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else if (CursorColumn == LineStartPoint.Column)
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else
                        {
                            Diagram[CursorRow][CursorColumn] = '-';
                        }
                    }
                }
            }
            else if (TravelDir == "Left")
            {
                uint32_t CursorRow;
                uint32_t CursorColumn;

                CursorRow = LineStartPoint.Row;
                for (CursorColumn = LineStartPoint.Column; CursorColumn >= LineEndPoint.Column; CursorColumn--)
                {
                    if (Diagram[CursorRow][CursorColumn] == '+')
                    {
                        continue;
                    } 
                    else
                    {
                        if (CursorColumn == LineEndPoint.Column)
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else if ( (CursorRow == LineStartPoint.Row) && (CursorColumn == LineStartPoint.Column))
                        {
                            Diagram[CursorRow][CursorColumn] = '+';
                        }
                        else
                        {
                            Diagram[CursorRow][CursorColumn] = '-';
                        }
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
        for (const Stub &SelectedStub : SelectedBox.InputStubs)
        {
            const InputStub& SelectedInputStub = std::get<InputStub>(SelectedStub);
            uint32_t LabelLength;

            Diagram[SelectedInputStub.Position.Row][SelectedInputStub.Position.Column - 3u] = '-';
            Diagram[SelectedInputStub.Position.Row][SelectedInputStub.Position.Column - 2u] = '-';
            Diagram[SelectedInputStub.Position.Row][SelectedInputStub.Position.Column - 1u] = '>';
            LabelLength = SelectedInputStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = (SelectedInputStub.Position.Column - LabelLength) + CharIndex;
                CharRow = SelectedInputStub.Position.Row - 1u;
                Diagram[CharRow][CharColumn] = SelectedInputStub.Name[CharIndex];
            }
        }
        for (const Stub &SelectedStub : SelectedBox.OutputStubs)
        {
            const OutputStub& SelectedOutputStub = std::get<OutputStub>(SelectedStub);
            uint32_t LabelLength;

            Diagram[SelectedOutputStub.Position.Row][SelectedOutputStub.Position.Column + 2u] = '-';
            Diagram[SelectedOutputStub.Position.Row][SelectedOutputStub.Position.Column + 1u] = '-';
            LabelLength = SelectedOutputStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = SelectedOutputStub.Position.Column + CharIndex + 1u;
                CharRow = SelectedOutputStub.Position.Row - 1u;
                Diagram[CharRow][CharColumn] = SelectedOutputStub.Name[CharIndex];
            }
        }
        uint32_t ControlStubCount;
        ControlStubCount = SelectedBox.ControlStubs.size();
        for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlStubCount; ControlStubIndex++)
        {
            const ControlStub &SelectedControlStub = std::get<ControlStub>(SelectedBox.ControlStubs[ControlStubIndex]);
            uint32_t LabelLength;

            Diagram[SelectedControlStub.Position.Row - 2u][SelectedControlStub.Position.Column] = '|';
            Diagram[SelectedControlStub.Position.Row - 1u][SelectedControlStub.Position.Column] = 'V';
            LabelLength = SelectedControlStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = SelectedControlStub.Position.Column + CharIndex;
                CharRow = SelectedControlStub.Position.Row - 1u - ControlStubIndex;
                Diagram[CharRow][CharColumn] = SelectedControlStub.Name[CharIndex];
            }
        }
        uint32_t MechanismStubCount;
        MechanismStubCount = SelectedBox.MechanismStubs.size();
        for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismStubCount; MechanismStubIndex++)
        {
            const MechanismStub &SelectedMechanismStub = std::get<MechanismStub>(SelectedBox.MechanismStubs[MechanismStubIndex]);
            uint32_t LabelLength;

            Diagram[SelectedMechanismStub.Position.Row + 2u][SelectedMechanismStub.Position.Column] = '|';
            Diagram[SelectedMechanismStub.Position.Row + 1u][SelectedMechanismStub.Position.Column] = '^';
            LabelLength = SelectedMechanismStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = SelectedMechanismStub.Position.Column + CharIndex;
                CharRow = SelectedMechanismStub.Position.Row + 1u + MechanismStubIndex;
                Diagram[CharRow][CharColumn] = SelectedMechanismStub.Name[CharIndex];
            }
        }
        uint32_t CallStubCount;
        CallStubCount = SelectedBox.CallStubs.size();
        for (uint32_t CallStubIndex = 0u; CallStubIndex < CallStubCount; CallStubIndex++)
        {
            const CallStub &SelectedCallStub = std::get<CallStub>(SelectedBox.CallStubs[CallStubIndex]);
            uint32_t LabelLength;

            Diagram[SelectedCallStub.Position.Row + 2u][SelectedCallStub.Position.Column] = '-';
            Diagram[SelectedCallStub.Position.Row + 1u][SelectedCallStub.Position.Column] = '^';
            LabelLength = SelectedCallStub.Name.length();
            for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
            {
                uint32_t CharColumn;
                uint32_t CharRow;

                CharColumn = SelectedCallStub.Position.Column + CharIndex;
                CharRow = SelectedCallStub.Position.Row + MechanismStubCount + 1u + CallStubIndex;
                Diagram[CharRow][CharColumn] = SelectedCallStub.Name[CharIndex];
            }
        }
        uint32_t NumCallBoundaryStubs;
        uint32_t NumInputBoundaryStubs;
        uint32_t NumOutputBoundaryStubs;
        uint32_t NumMechanismBoundaryStubs;
        uint32_t NumControlBoundaryStubs;

        NumCallBoundaryStubs = TargetDiagram.CallBoundaryStubs.size();
        NumInputBoundaryStubs = TargetDiagram.InputBoundaryStubs.size();
        NumOutputBoundaryStubs = TargetDiagram.OutputBoundaryStubs.size();
        NumMechanismBoundaryStubs = TargetDiagram.MechanismBoundaryStubs.size();
        NumControlBoundaryStubs = TargetDiagram.ControlBoundaryStubs.size();
        for (uint32_t StubIndex = 0u; StubIndex < NumInputBoundaryStubs; StubIndex++)
        {
            const Stub& IteratedStub = TargetDiagram.InputBoundaryStubs[StubIndex];
            const InputStub& BoundaryInputStub = std::get<InputStub>(IteratedStub);

            Diagram[BoundaryInputStub.Position.Row][BoundaryInputStub.Position.Column] = '-';
            Diagram[BoundaryInputStub.Position.Row][BoundaryInputStub.Position.Column + 1u] = '-';
            Diagram[BoundaryInputStub.Position.Row][BoundaryInputStub.Position.Column + 2u] = '-';
        }
        for (uint32_t StubIndex = 0u; StubIndex < NumControlBoundaryStubs; StubIndex++)
        {
            const Stub& IteratedStub = TargetDiagram.ControlBoundaryStubs[StubIndex];
            const ControlStub& BoundaryControlStub = std::get<ControlStub>(IteratedStub);
            
            Diagram[BoundaryControlStub.Position.Row][BoundaryControlStub.Position.Column] = '|';
            Diagram[BoundaryControlStub.Position.Row + 1u][BoundaryControlStub.Position.Column] = '|';
            Diagram[BoundaryControlStub.Position.Row + 2u][BoundaryControlStub.Position.Column] = '|';
        }
        for (uint32_t StubIndex = 0u; StubIndex < NumOutputBoundaryStubs; StubIndex++)
        {
            const Stub& IteratedStub = TargetDiagram.OutputBoundaryStubs[StubIndex];
            const OutputStub& BoundaryOutputStub = std::get<OutputStub>(IteratedStub);
            
            Diagram[BoundaryOutputStub.Position.Row][BoundaryOutputStub.Position.Column - 2u] = '-';
            Diagram[BoundaryOutputStub.Position.Row][BoundaryOutputStub.Position.Column - 1u] = '-';
            Diagram[BoundaryOutputStub.Position.Row][BoundaryOutputStub.Position.Column] = '>';
        }
        for (uint32_t StubIndex = 0u; StubIndex < NumMechanismBoundaryStubs; StubIndex++)
        {
            const Stub& IteratedStub = TargetDiagram.MechanismBoundaryStubs[StubIndex];
            const MechanismStub& BoundaryMechanismStub = std::get<MechanismStub>(IteratedStub);
            
            Diagram[BoundaryMechanismStub.Position.Row - 2u][BoundaryMechanismStub.Position.Column] = '|';
            Diagram[BoundaryMechanismStub.Position.Row - 1u][BoundaryMechanismStub.Position.Column] = '|';
            Diagram[BoundaryMechanismStub.Position.Row][BoundaryMechanismStub.Position.Column] = '|';
        }
        for (uint32_t StubIndex = 0u; StubIndex < NumCallBoundaryStubs; StubIndex++)
        {
            const Stub& IteratedStub = TargetDiagram.CallBoundaryStubs[StubIndex];
            const CallStub& BoundaryCallStub = std::get<CallStub>(IteratedStub);
            
            Diagram[BoundaryCallStub.Position.Row - 2u][BoundaryCallStub.Position.Column] = '|';
            Diagram[BoundaryCallStub.Position.Row - 1u][BoundaryCallStub.Position.Column] = '|';
            Diagram[BoundaryCallStub.Position.Row][BoundaryCallStub.Position.Column] = 'V';
        }
    }
    return Diagram;
}
}


int main(int argc, char **argv)
{
    char* InputFilePath;
    char* OutputFilePath;
    const uint32_t DiagramWidth = 400;
    const uint32_t DiagramHeight = 120;
    const uint32_t BoxWidth = 32;
    const uint32_t BoxHeight = 4;
    const uint32_t BoxMargin = 20;
    std::vector<IDEF::Model> Models;

    InputFilePath = argv[1u];
    OutputFilePath = argv[2u];
    Models = IDEF::LoadModelsFile(InputFilePath);
    for (IDEF::Model &SelectedModel : Models)
    {
        for (IDEF::ActivityDiagram &SelectedActivityDiagram : SelectedModel.ActivityDiagrams)
        {
            std::fstream OutputFileStream;
            std::map<IDEF::Stub, Avoid::ConnEnd> BoxStubsMap;
            std::map<IDEF::Stub, Avoid::ConnEnd> BoundaryStubsMap;       
            std::vector<Avoid::Rectangle> Obstacles;
            Avoid::Router *Router;
            std::vector<std::string> Diagram;
            uint32_t RowNumber;

            IDEF::LayoutActivityDiagram(SelectedActivityDiagram, DiagramWidth, DiagramHeight, BoxWidth, BoxHeight, BoxMargin);
            IDEF::PlaceObstacles(SelectedActivityDiagram, Obstacles, BoxWidth, BoxHeight, BoxMargin);
            BoxStubsMap = IDEF::PlaceBoxStubConnEnds(SelectedActivityDiagram, BoxWidth, BoxHeight, BoxMargin);
            BoundaryStubsMap = IDEF::PlaceBoundaryStubConnEnds(SelectedActivityDiagram, BoxWidth, BoxHeight, BoxMargin);
            Router = IDEF::ConstructRouter(BoxStubsMap,BoundaryStubsMap, Obstacles);
            Diagram = IDEF::DrawDiagram(SelectedActivityDiagram, Router, BoxWidth, BoxHeight, BoxMargin);
            RowNumber = 0u;
            OutputFileStream.open(OutputFilePath, std::ios_base::out);
            for (const std::string &Line : Diagram)
            {
                OutputFileStream << Line;
                OutputFileStream << '\n';
                RowNumber = RowNumber + 1u;
            }
            OutputFileStream.close();
            Diagram.clear(); 
            delete Router;
        }
    }

    return 0;
}
