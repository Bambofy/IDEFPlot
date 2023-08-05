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
#include "Loading.h"
#include "Placing.h"
#include "Drawing.h"
#include "Layouting.h"

namespace IDEF
{

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
    NewStub.Length = 0u;

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
    NewStub.Length = 0u;

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
    NewStub.Length = 0u;

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
    NewStub.Length = 0u;

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
    NewStub.Length = 0u;

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
    NewDiagram.Frame.BottomBar.TitleSection = TitleSection();
    NewDiagram.Frame.BottomBar.CNumberSection = CNumberSection();
    NodeNumberSection& TargetNodeNumberSection = std::get<NodeNumberSection>(NewDiagram.Frame.BottomBar.NodeNumberSection);
    TitleSection& TargetTitleSection = std::get<TitleSection>(NewDiagram.Frame.BottomBar.TitleSection);
    CNumberSection& TargetCNumberSection = std::get<CNumberSection>(NewDiagram.Frame.BottomBar.CNumberSection);

    NewDiagram.Frame.BottomBar.Height = 4u;
    TargetNodeNumberSection.Content = ActivityDiagramNode.attribute("Number").as_string();
    TargetNodeNumberSection.TopLeft.Row = 0u;
    TargetNodeNumberSection.TopLeft.Column = 0u;
    TargetTitleSection.Content = ActivityDiagramNode.attribute("Title").as_string();
    TargetTitleSection.TopLeft.Row = 0u;
    TargetTitleSection.TopLeft.Column = 0u;
    TargetCNumberSection.Content = ActivityDiagramNode.attribute("CNumber").as_string();
    TargetCNumberSection.TopLeft.Row = 0u;
    TargetCNumberSection.TopLeft.Column = 0u;
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

}
