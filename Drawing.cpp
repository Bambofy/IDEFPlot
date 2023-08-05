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
#include "Layouting.h"
#include "Drawing.h"

namespace IDEF
{

void DrawRoutes(Avoid::Router* ConnectedRouter, 
    std::vector<std::string>& Diagram, 
    const ActivityDiagram& TargetDiagram)
{
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
            LineStartPoint.Row = TargetDiagram.Height - (uint32_t)(round(FirstPoint.y));
            LineEndPoint.Column = (uint32_t)(round(SecondPoint.x));
            LineEndPoint.Row = TargetDiagram.Height - (uint32_t)(round(SecondPoint.y));
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
}

void DrawBoxOutline(std::vector<std::string>& Diagram,
    const ActivityBox& SelectedBox)
{
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
}

void DrawBoxLabel(std::vector<std::string>& Diagram,
    const ActivityBox& SelectedBox)
{
    FilePosition BoxTopLeft;
    FilePosition BoxBottomRight;
    uint8_t NumLines;
    uint32_t NumChars;
    FilePosition BoxLabelStartPosition;
    bool WrapFlag;
    FilePosition Cursor;

    BoxTopLeft.Row = SelectedBox.Center.Row - (SelectedBox.Height / 2u);
    BoxTopLeft.Column = SelectedBox.Center.Column - (SelectedBox.Width / 2u);
    BoxBottomRight.Row = SelectedBox.Center.Row + (SelectedBox.Height / 2u);
    BoxBottomRight.Column = SelectedBox.Center.Column + (SelectedBox.Width / 2u);
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
    Cursor.Row = BoxBottomRight.Row - 1u;
    Cursor.Column = BoxBottomRight.Column - 1u;
    NumChars = SelectedBox.NodeNumber.length();
    for (uint32_t CharIndex = 0u; CharIndex < NumChars; CharIndex++)
    {
        Diagram[Cursor.Row][Cursor.Column] = SelectedBox.NodeNumber[(NumChars-1u) - CharIndex];
        Cursor.Column--;
    }
    Diagram[Cursor.Row][Cursor.Column] = 'A';
}

void DrawBoxStubArrows(std::vector<std::string>& Diagram,
    const ActivityBox& SelectedBox)
{
    uint32_t ControlStubCount;
    uint32_t MechanismStubCount;
    uint32_t CallStubCount;

    for (const Stub &SelectedStub : SelectedBox.InputStubs)
    {
        const InputStub& SelectedInputStub = std::get<InputStub>(SelectedStub);

        for (uint32_t StubCharIndex = 0u; StubCharIndex < SelectedInputStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == 0u)
            {
                Diagram[SelectedInputStub.Position.Row][SelectedInputStub.Position.Column - 1u] = '>';
            }
            else if (StubCharIndex == (SelectedInputStub.Length-1u))
            {   
                Diagram[SelectedInputStub.Position.Row][SelectedInputStub.Position.Column - 1u - StubCharIndex] = '+';
            }
            else
            {
                Diagram[SelectedInputStub.Position.Row][SelectedInputStub.Position.Column - 1u - StubCharIndex] = '-';
            }
        }
    }
    for (const Stub &SelectedStub : SelectedBox.OutputStubs)
    {
        const OutputStub& SelectedOutputStub = std::get<OutputStub>(SelectedStub);

        for (uint32_t StubCharIndex = 0u; StubCharIndex < SelectedOutputStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == (SelectedOutputStub.Length-1u))
            {   
                Diagram[SelectedOutputStub.Position.Row][SelectedOutputStub.Position.Column + 1u + StubCharIndex] = '+';
            }
            else
            {
                Diagram[SelectedOutputStub.Position.Row][SelectedOutputStub.Position.Column + 1u + StubCharIndex] = '-';
            }
        }
    }
    ControlStubCount = SelectedBox.ControlStubs.size();
    for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlStubCount; ControlStubIndex++)
    {
        const ControlStub &SelectedControlStub = std::get<ControlStub>(SelectedBox.ControlStubs[ControlStubIndex]);
        for (uint32_t StubCharIndex = 0u; StubCharIndex < SelectedControlStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == 0u)
            {
                Diagram[SelectedControlStub.Position.Row - 1u - StubCharIndex][SelectedControlStub.Position.Column] = 'V';
            }
            else if (StubCharIndex == (SelectedControlStub.Length-1u))
            {   
                Diagram[SelectedControlStub.Position.Row - 1u - StubCharIndex][SelectedControlStub.Position.Column] = '+';
            }
            else
            {
                Diagram[SelectedControlStub.Position.Row - 1u - StubCharIndex][SelectedControlStub.Position.Column] = '|';
            }
        }
    }
    MechanismStubCount = SelectedBox.MechanismStubs.size();
    for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismStubCount; MechanismStubIndex++)
    {
        const MechanismStub &SelectedMechanismStub = std::get<MechanismStub>(SelectedBox.MechanismStubs[MechanismStubIndex]);

        for (uint32_t StubCharIndex = 0u; StubCharIndex < SelectedMechanismStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == 0u)
            {
                Diagram[SelectedMechanismStub.Position.Row + 1u + StubCharIndex][SelectedMechanismStub.Position.Column] = '^';
            }
            else if (StubCharIndex == (SelectedMechanismStub.Length-1u))
            {   
                Diagram[SelectedMechanismStub.Position.Row + 1u + StubCharIndex][SelectedMechanismStub.Position.Column] = '+';
            }
            else
            {
                Diagram[SelectedMechanismStub.Position.Row + 1u + StubCharIndex][SelectedMechanismStub.Position.Column] = '|';
            }
        }
    }
    CallStubCount = SelectedBox.CallStubs.size();
    for (uint32_t CallStubIndex = 0u; CallStubIndex < CallStubCount; CallStubIndex++)
    {
        const CallStub &SelectedCallStub = std::get<CallStub>(SelectedBox.CallStubs[CallStubIndex]);

        for (uint32_t StubCharIndex = 0u; StubCharIndex < SelectedCallStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == 0u)
            {
                Diagram[SelectedCallStub.Position.Row + 1u + StubCharIndex][SelectedCallStub.Position.Column] = '^';
            }
            else if (StubCharIndex == (SelectedCallStub.Length-1u))
            {   
                Diagram[SelectedCallStub.Position.Row + 1u + StubCharIndex][SelectedCallStub.Position.Column] = '+';
            }
            else
            {
                Diagram[SelectedCallStub.Position.Row + 1u + StubCharIndex][SelectedCallStub.Position.Column] = '|';
            }
        }
    }  
}

void DrawBoxStubLabels(std::vector<std::string>& Diagram,
    const ActivityBox& SelectedBox)
{
    uint32_t ControlStubCount;
    uint32_t MechanismStubCount;
    uint32_t CallStubCount;

    for (const Stub &SelectedStub : SelectedBox.InputStubs)
    {
        const InputStub& SelectedInputStub = std::get<InputStub>(SelectedStub);
        uint32_t LabelLength;

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
    ControlStubCount = SelectedBox.ControlStubs.size();
    for (uint32_t ControlStubIndex = 0u; ControlStubIndex < ControlStubCount; ControlStubIndex++)
    {
        const ControlStub &SelectedControlStub = std::get<ControlStub>(SelectedBox.ControlStubs[ControlStubIndex]);
        uint32_t LabelLength;

        LabelLength = SelectedControlStub.Name.length();
        for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
        {
            uint32_t CharColumn;
            uint32_t CharRow;

            CharColumn = SelectedControlStub.Position.Column + CharIndex + 1u;
            CharRow = SelectedControlStub.Position.Row - 2u - ControlStubIndex;
            Diagram[CharRow][CharColumn] = SelectedControlStub.Name[CharIndex];
        }
    }
    MechanismStubCount = SelectedBox.MechanismStubs.size();
    for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < MechanismStubCount; MechanismStubIndex++)
    {
        const MechanismStub &SelectedMechanismStub = std::get<MechanismStub>(SelectedBox.MechanismStubs[MechanismStubIndex]);
        uint32_t LabelLength;

        LabelLength = SelectedMechanismStub.Name.length();
        for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
        {
            uint32_t CharColumn;
            uint32_t CharRow;

            CharColumn = SelectedMechanismStub.Position.Column + CharIndex + 1u;
            CharRow = SelectedMechanismStub.Position.Row + 2u + MechanismStubIndex;
            Diagram[CharRow][CharColumn] = SelectedMechanismStub.Name[CharIndex];
        }
    }
    CallStubCount = SelectedBox.CallStubs.size();
    for (uint32_t CallStubIndex = 0u; CallStubIndex < CallStubCount; CallStubIndex++)
    {
        const CallStub &SelectedCallStub = std::get<CallStub>(SelectedBox.CallStubs[CallStubIndex]);
        uint32_t LabelLength;

        LabelLength = SelectedCallStub.Name.length();
        for (uint32_t CharIndex = 0u; CharIndex < LabelLength; CharIndex++)
        {
            uint32_t CharColumn;
            uint32_t CharRow;

            CharColumn = SelectedCallStub.Position.Column + CharIndex + 1u;
            CharRow = SelectedCallStub.Position.Row + MechanismStubCount + 2u + CallStubIndex;
            Diagram[CharRow][CharColumn] = SelectedCallStub.Name[CharIndex];
        }
    }
}

void DrawBoxStubs(std::vector<std::string>& Diagram, 
    const ActivityBox& SelectedBox)
{
    DrawBoxStubArrows(Diagram, SelectedBox);
    DrawBoxStubLabels(Diagram, SelectedBox);
}


void DrawBoundaryStubs(std::vector<std::string>& Diagram,
    const ActivityDiagram& TargetDiagram)
{
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

        for (uint32_t StubCharIndex = 0u; StubCharIndex < BoundaryInputStub.Length; StubCharIndex++)
        {
            Diagram[BoundaryInputStub.Position.Row][BoundaryInputStub.Position.Column + StubCharIndex] = '-';
        }
    }
    for (uint32_t StubIndex = 0u; StubIndex < NumControlBoundaryStubs; StubIndex++)
    {
        const Stub& IteratedStub = TargetDiagram.ControlBoundaryStubs[StubIndex];
        const ControlStub& BoundaryControlStub = std::get<ControlStub>(IteratedStub);

        for (uint32_t StubCharIndex = 0u; StubCharIndex < BoundaryControlStub.Length; StubCharIndex++)
        {
            Diagram[BoundaryControlStub.Position.Row + StubCharIndex][BoundaryControlStub.Position.Column] = '|';
        }
    }
    for (uint32_t StubIndex = 0u; StubIndex < NumOutputBoundaryStubs; StubIndex++)
    {
        const Stub& IteratedStub = TargetDiagram.OutputBoundaryStubs[StubIndex];
        const OutputStub& BoundaryOutputStub = std::get<OutputStub>(IteratedStub);
        
        for (uint32_t StubCharIndex = 0u; StubCharIndex < BoundaryOutputStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == 0u || StubCharIndex == 1u)
            {
                Diagram[BoundaryOutputStub.Position.Row][BoundaryOutputStub.Position.Column - 1u] = '>';
            }
            else
            {
                Diagram[BoundaryOutputStub.Position.Row][BoundaryOutputStub.Position.Column - StubCharIndex] = '-';
            }
        }
    }
    for (uint32_t StubIndex = 0u; StubIndex < NumMechanismBoundaryStubs; StubIndex++)
    {
        const Stub& IteratedStub = TargetDiagram.MechanismBoundaryStubs[StubIndex];
        const MechanismStub& BoundaryMechanismStub = std::get<MechanismStub>(IteratedStub);
        
        Diagram[BoundaryMechanismStub.Position.Row][BoundaryMechanismStub.Position.Column] = 'V';
        for (uint32_t StubCharIndex = 0u; StubCharIndex < BoundaryMechanismStub.Length; StubCharIndex++)
        {
            Diagram[BoundaryMechanismStub.Position.Row - StubCharIndex][BoundaryMechanismStub.Position.Column] = '|';
        }
    }
    for (uint32_t StubIndex = 0u; StubIndex < NumCallBoundaryStubs; StubIndex++)
    {
        const Stub& IteratedStub = TargetDiagram.CallBoundaryStubs[StubIndex];
        const CallStub& BoundaryCallStub = std::get<CallStub>(IteratedStub);

        for (uint32_t StubCharIndex = 0u; StubCharIndex < BoundaryCallStub.Length; StubCharIndex++)
        {
            if (StubCharIndex == 0u)
            {
                Diagram[BoundaryCallStub.Position.Row][BoundaryCallStub.Position.Column] = '^';
            }
            else
            {
                Diagram[BoundaryCallStub.Position.Row][BoundaryCallStub.Position.Column - StubCharIndex] = '|';
            }
        }
    }
}

void DrawBoxes(std::vector<std::string>& Diagram,
    const ActivityDiagram& TargetDiagram)
{
    uint32_t ActivityBoxNum;

    ActivityBoxNum = TargetDiagram.Boxes.size();
    for (uint32_t ActivityBoxIndex = 0u; ActivityBoxIndex < ActivityBoxNum; ActivityBoxIndex++)
    {
        const ActivityBox &SelectedBox = TargetDiagram.Boxes[ActivityBoxIndex];

        DrawBoxOutline(Diagram, SelectedBox);
        DrawBoxLabel(Diagram, SelectedBox);
        DrawBoxStubs(Diagram, SelectedBox);
    }
}

void DrawNodeNumberSection(std::vector<std::string>& Diagram, 
    const ActivityDiagram& TargetDiagram)
{
    const NodeNumberSection& NumberBarSection = std::get<NodeNumberSection>(TargetDiagram.Frame.BottomBar.NodeNumberSection);
    FilePosition Cursor;
    uint32_t NumChars;

    for (uint32_t Row = NumberBarSection.TopLeft.Row; Row < NumberBarSection.TopLeft.Row + NumberBarSection.Height; Row++)
    {
        Diagram[Row][NumberBarSection.TopLeft.Column + NumberBarSection.Width] = '|'; 
    }
    Diagram[NumberBarSection.TopLeft.Row][NumberBarSection.TopLeft.Column] = '+';
    Diagram[NumberBarSection.TopLeft.Row][NumberBarSection.TopLeft.Column + NumberBarSection.Width] = '+';
    Diagram[NumberBarSection.TopLeft.Row + (NumberBarSection.Height-1u)][0u] = '+';
    Diagram[NumberBarSection.TopLeft.Row + (NumberBarSection.Height-1u)][NumberBarSection.TopLeft.Column + NumberBarSection.Width] = '+';
    Cursor.Row = NumberBarSection.TopLeft.Row;
    Cursor.Column = NumberBarSection.TopLeft.Column;
    Cursor.Row += 1u;
    Cursor.Column += 1u;
    Diagram[Cursor.Row][Cursor.Column] = 'N';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'o';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'd';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'e';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = ':';
    Cursor.Column++;
    NumChars = NumberBarSection.Content.length();
    for (uint32_t CharIndex = 0u; CharIndex < NumChars; CharIndex++)
    {
        Diagram[Cursor.Row][Cursor.Column] = NumberBarSection.Content[CharIndex];
        Cursor.Column++;
    }
}

void DrawTitleSection(std::vector<std::string>& Diagram, const ActivityDiagram& TargetDiagram)
{
    const TitleSection& TitleBarSection = std::get<TitleSection>(TargetDiagram.Frame.BottomBar.TitleSection);
    FilePosition Cursor;
    uint32_t NumChars;

    for (uint32_t Row = TitleBarSection.TopLeft.Row; Row < TitleBarSection.TopLeft.Row + TitleBarSection.Height; Row++)
    {
        Diagram[Row][TitleBarSection.TopLeft.Column + TitleBarSection.Width] = '|'; 
    }
    Diagram[TitleBarSection.TopLeft.Row][TitleBarSection.TopLeft.Column] = '+';
    Diagram[TitleBarSection.TopLeft.Row][TitleBarSection.TopLeft.Column + TitleBarSection.Width] = '+';
    Diagram[TitleBarSection.TopLeft.Row + (TitleBarSection.Height-1u)][0u] = '+';
    Diagram[TitleBarSection.TopLeft.Row + (TitleBarSection.Height-1u)][TitleBarSection.TopLeft.Column + TitleBarSection.Width] = '+';
    Cursor.Row = TitleBarSection.TopLeft.Row;
    Cursor.Column = TitleBarSection.TopLeft.Column;
    Cursor.Row += 1u;
    Cursor.Column += 1u;
    Diagram[Cursor.Row][Cursor.Column] = 'T';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'i';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 't';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'l';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'e';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = ':';
    Cursor.Column++;
    NumChars = TitleBarSection.Content.length();
    for (uint32_t CharIndex = 0u; CharIndex < NumChars; CharIndex++)
    {
        Diagram[Cursor.Row][Cursor.Column] = TitleBarSection.Content[CharIndex];
        Cursor.Column++;
    }
}


void DrawCNumberSection(std::vector<std::string>& Diagram,
    const ActivityDiagram& TargetDiagram)
{
    const CNumberSection& CNumSection = std::get<CNumberSection>(TargetDiagram.Frame.BottomBar.CNumberSection);
    FilePosition Cursor;
    uint32_t NumChars;

    for (uint32_t Row = CNumSection.TopLeft.Row; Row < CNumSection.TopLeft.Row + CNumSection.Height; Row++)
    {
        Diagram[Row][CNumSection.TopLeft.Column + CNumSection.Width] = '|'; 
    }
    Diagram[CNumSection.TopLeft.Row][CNumSection.TopLeft.Column] = '+';
    Diagram[CNumSection.TopLeft.Row][CNumSection.TopLeft.Column + CNumSection.Width] = '+';
    Diagram[CNumSection.TopLeft.Row + (CNumSection.Height-1u)][0u] = '+';
    Diagram[CNumSection.TopLeft.Row + (CNumSection.Height-1u)][CNumSection.TopLeft.Column + CNumSection.Width] = '+';
    Cursor.Row = CNumSection.TopLeft.Row;
    Cursor.Column = CNumSection.TopLeft.Column;
    Cursor.Row += 1u;
    Cursor.Column += 1u;
    Diagram[Cursor.Row][Cursor.Column] = 'C';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'N';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'u';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'm';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'b';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'e';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = 'r';
    Cursor.Column++;
    Diagram[Cursor.Row][Cursor.Column] = ':';
    Cursor.Column++;
    NumChars = CNumSection.Content.length();
    for (uint32_t CharIndex = 0u; CharIndex < NumChars; CharIndex++)
    {
        Diagram[Cursor.Row][Cursor.Column] = CNumSection.Content[CharIndex];
        Cursor.Column++;
    }
}

void DrawBottomBar(std::vector<std::string>& Diagram, const ActivityDiagram& TargetDiagram)
{
    for (uint32_t Column = TargetDiagram.Frame.BottomBar.TopLeft.Column; Column < TargetDiagram.Width-1u; Column++)
    {
        Diagram[TargetDiagram.Frame.BottomBar.TopLeft.Row][Column] = '-';
    }
    DrawNodeNumberSection(Diagram, TargetDiagram);
    DrawTitleSection(Diagram, TargetDiagram);
    DrawCNumberSection(Diagram, TargetDiagram);
}

void DrawFrame(std::vector<std::string>& Diagram, const ActivityDiagram& TargetDiagram)
{
    for (uint32_t Column = 0u; Column < TargetDiagram.Width; Column++)
    {
        Diagram[0u][Column] = '-';
        Diagram[TargetDiagram.Height - 1][Column] = '-';
    }
    for (uint32_t Row = 0u; Row < TargetDiagram.Height; Row++)
    {
        Diagram[Row][0u] = '|';
        Diagram[Row][TargetDiagram.Width - 1u] = '|';
    }
    Diagram[0u][0u] = '+';
    Diagram[0u][TargetDiagram.Width - 1u] = '+';
    Diagram[TargetDiagram.Height - 1u][TargetDiagram.Width - 1u] = '+';
    Diagram[TargetDiagram.Height - 1u][0u] = '+';
    DrawBottomBar(Diagram, TargetDiagram);
}

std::vector<std::string> DrawDiagram(const ActivityDiagram &TargetDiagram, 
    Avoid::Router *ConnectedRouter)
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
    DrawRoutes(ConnectedRouter, Diagram, TargetDiagram);
    DrawBoxes(Diagram, TargetDiagram);
    DrawBoundaryStubs(Diagram, TargetDiagram);
    DrawFrame(Diagram, TargetDiagram);

    return Diagram;
}

}
