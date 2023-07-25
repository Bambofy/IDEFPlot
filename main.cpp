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
		std::string Source;
		FilePosition Position;
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

	typedef std::vector<GlossaryEntry> Glossary;

	class Diagram
	{
	public:
		std::string Title;
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
		std::string NodeNumber;
		std::string CNumber;
		std::string Author;
		std::string Date;
		std::string Status;
		std::string Version;
	}:

	class IDEFStateDiagram : public IDEFDiagram
	{
	public:
		std::vector<IDEFStateBox> Boxes;
		std::vector<IDEFStateStub> BoundaryStubs;
	}:

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
		NewActivityStub.FilePosition.X = 0;
		NewActivityStub.FilePosition.Y = 0;	

		return NewActivityStub;
	}

	IDEFActivityBox LoadActivity(const pugixml::xml_node& ActivityNode)
	{
		IDEFActivityBox NewActivityBox;
		uint16_t NumStubs;

		NewActivityBox.Name = ActivityNode.attrib("Name").as_string();
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

		NewActivityDiagram.NodeNumber = ActivityDiagramNode.attrib("Number").to_string();
		NewActivityDiagram.Title = ActivityDiagramNode.attrib("Title").to_string();
		NewActivityDiagram.CNumber = ActivityDiagramNode.attrib("CNumber").to_string();
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
}

int main(int argc, char** argv)
{
	return 0;
}
