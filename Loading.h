#ifndef LOADING_H
#define LOADING_H

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
    uint32_t Length;
    
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
    uint32_t Length;

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
    uint32_t Length;

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
    uint32_t Length;

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
    uint32_t Length;

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

InputStub LoadInputStub(const pugi::xml_node& InputStubXMLNode);
OutputStub LoadOutputStub(const pugi::xml_node& OutputStubXMLNode);
ControlStub LoadControlStub(const pugi::xml_node& ControlStubXMLNode);
MechanismStub LoadMechanismStub(const pugi::xml_node& MechanismStubXMLNode);
CallStub LoadCallStub(const pugi::xml_node& CallStubXMLNode);
ActivityBox LoadActivity(const pugi::xml_node &ActivityNode);
ActivityDiagram LoadActivityDiagram(const pugi::xml_node &ActivityDiagramNode);
Model LoadModel(const pugi::xml_node &ModelNode);
std::vector<Model> LoadModelsFile(const std::string &FilePath);

}

#endif