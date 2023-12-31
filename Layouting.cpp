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

void LayoutFrame(ActivityDiagram &Diagram)
{
    NodeNumberSection& TargetNodeNumberSection = std::get<NodeNumberSection>(Diagram.Frame.BottomBar.NodeNumberSection);
    TitleSection& TargetTitleSection = std::get<TitleSection>(Diagram.Frame.BottomBar.TitleSection);
    CNumberSection& TargetCNumberSection = std::get<CNumberSection>(Diagram.Frame.BottomBar.CNumberSection);
    
    Diagram.Frame.BottomBar.Height = 3u;
    Diagram.Frame.BottomBar.TopLeft.Row = Diagram.Height - Diagram.Frame.BottomBar.Height;
    Diagram.Frame.BottomBar.TopLeft.Column = 0u;
    TargetNodeNumberSection.Width = Diagram.Width / 4u;
    TargetNodeNumberSection.TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    TargetNodeNumberSection.TopLeft.Column = Diagram.Frame.BottomBar.TopLeft.Column;
    TargetNodeNumberSection.Height = Diagram.Frame.BottomBar.Height;
    TargetTitleSection.Width = 2u * (Diagram.Width / 4u);
    TargetTitleSection.TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    TargetTitleSection.TopLeft.Column = TargetNodeNumberSection.TopLeft.Column + TargetNodeNumberSection.Width;
    TargetTitleSection.Height = Diagram.Frame.BottomBar.Height;
    TargetCNumberSection.Width = Diagram.Width / 4u;
    TargetCNumberSection.TopLeft.Row = Diagram.Frame.BottomBar.TopLeft.Row;
    TargetCNumberSection.TopLeft.Column = TargetTitleSection.TopLeft.Column + TargetTitleSection.Width;
    TargetCNumberSection.Height = Diagram.Frame.BottomBar.Height;
}

void LayoutBoxes(ActivityDiagram &Diagram, 
    uint32_t BoxWidth, 
    uint32_t BoxHeight, 
    uint32_t BoxXGap,
    uint32_t BoxYGap)
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
    BoxSectionHeight = (NumBoxes * BoxHeight) + ((NumBoxes-1u) * BoxYGap);
    BoxSectionWidth = (NumBoxes * BoxWidth) + ((NumBoxes-1u) * BoxXGap);
    RowHeight = BoxSectionHeight / (1u+NumBoxes);
    ColumnWidth = BoxSectionWidth / (1u+NumBoxes);
    ColumnCenterOffset = (Diagram.Width / 2u) - (BoxSectionWidth / 2u);
    RowCenterOffset = (Diagram.Height / 2u) - (BoxSectionHeight / 2u);
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
        Cursor.Column += BoxWidth + BoxXGap;
        Cursor.Row += BoxHeight + BoxYGap;
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
            SelectedInputStub.Length = 3u + InputStubIndex;
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
            SelectedOutputStub.Length = 3u + OutputStubIndex;
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
            SelectedControlStub.Length = 3u + ControlStubIndex;
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
            SelectedMechanismStub.Position.Column = SelectedMechanismStub.Position.Column + ColumnOffset;
            SelectedMechanismStub.Length = 3u + MechanismStubIndex;
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
            SelectedCallStub.Length = 4u;
        }
    }
}

void FindInnerStub(const ActivityDiagram& Diagram, const Stub& BoundaryStub, Stub& FoundStub, bool& FoundFlag)
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

std::vector<ActivityBox> LocateConnectedBoxes(ActivityDiagram& Diagram, std::string& TargetStubName)
{
    std::vector<ActivityBox> ConnectedBoxes;

    for (ActivityBox& SelectedBox : Diagram.Boxes)
    {
        for (Stub& SelectedStub : SelectedBox.InputStubs)
        {
            InputStub& SelectedInputStub = std::get<InputStub>(SelectedStub);

            if (SelectedInputStub.Name == TargetStubName)
            {
                ConnectedBoxes.push_back(SelectedBox);
            }
            else
            {
                for (StubSource& SelectedSource : SelectedInputStub.Sources)
                {
                    if (SelectedSource.StubName == TargetStubName)
                    {
                        ConnectedBoxes.push_back(SelectedBox);
                    }
                }
            }
        }
        for (Stub& SelectedStub : SelectedBox.OutputStubs)
        {
            OutputStub& SelectedOutputStub = std::get<OutputStub>(SelectedStub);

            if (SelectedOutputStub.Name == TargetStubName)
            {
                ConnectedBoxes.push_back(SelectedBox);
            }
            else
            {
                for (StubSource& SelectedSource : SelectedOutputStub.Sources)
                {
                    if (SelectedSource.StubName == TargetStubName)
                    {
                        ConnectedBoxes.push_back(SelectedBox);
                    }
                }
            }
        }
        for (Stub& SelectedStub : SelectedBox.ControlStubs)
        {
            ControlStub& SelectedControlStub = std::get<ControlStub>(SelectedStub);

            if (SelectedControlStub.Name == TargetStubName)
            {
                ConnectedBoxes.push_back(SelectedBox);
            }
            else
            {
                for (StubSource& SelectedSource : SelectedControlStub.Sources)
                {
                    if (SelectedSource.StubName == TargetStubName)
                    {
                        ConnectedBoxes.push_back(SelectedBox);
                    }
                }
            }
        }
        for (Stub& SelectedStub : SelectedBox.MechanismStubs)
        {
            MechanismStub& SelectedMechanismStub = std::get<MechanismStub>(SelectedStub);

            if (SelectedMechanismStub.Name == TargetStubName)
            {
                ConnectedBoxes.push_back(SelectedBox);
            }
            else
            {
                for (StubSource& SelectedSource : SelectedMechanismStub.Sources)
                {
                    if (SelectedSource.StubName == TargetStubName)
                    {
                        ConnectedBoxes.push_back(SelectedBox);
                    }
                }
            }
        }
        for (Stub& SelectedStub : SelectedBox.CallStubs)
        {
            CallStub& SelectedCallStub = std::get<CallStub>(SelectedStub);

            if (SelectedCallStub.Name == TargetStubName)
            {
                ConnectedBoxes.push_back(SelectedBox);
            }
            else
            {
                for (StubSource& SelectedSource : SelectedCallStub.Sources)
                {
                    if (SelectedSource.StubName == TargetStubName)
                    {
                        ConnectedBoxes.push_back(SelectedBox);
                    }
                }
            }
        }
    }

    return ConnectedBoxes;
}

bool CheckStubOverlap(Stub FirstStub, Stub SecondStub)
{
    bool Overlaps;

    Overlaps = false;
    if (std::holds_alternative<InputStub>(FirstStub))
    {
        InputStub FirstInputStub;

        FirstInputStub = std::get<InputStub>(FirstStub);
        if (std::holds_alternative<InputStub>(SecondStub))
        {
            InputStub SecondInputStub;

            SecondInputStub = std::get<InputStub>(SecondStub);
            if (FirstInputStub.Position.Row == SecondInputStub.Position.Row)
            {
                Overlaps = true;
            }
            else if (FirstInputStub.Position.Row == (SecondInputStub.Position.Row - 1u))
            {
                Overlaps = true;
            }
            else if (FirstInputStub.Position.Row == (SecondInputStub.Position.Row + 1u))
            {
                Overlaps = true;
            }
        }
    }
    else if (std::holds_alternative<OutputStub>(FirstStub))
    {
        OutputStub FirstOutputStub;

        FirstOutputStub = std::get<OutputStub>(FirstStub);
        if (std::holds_alternative<OutputStub>(SecondStub))
        {
            OutputStub SecondOutputStub;

            SecondOutputStub = std::get<OutputStub>(SecondStub);
            if (FirstOutputStub.Position.Row == SecondOutputStub.Position.Row)
            {
                Overlaps = true;
            }
            else if (FirstOutputStub.Position.Row == (SecondOutputStub.Position.Row - 1u))
            {
                Overlaps = true;
            }
            else if (FirstOutputStub.Position.Row == (SecondOutputStub.Position.Row + 1u))
            {
                Overlaps = true;
            }
        }
    }
    else if (std::holds_alternative<ControlStub>(FirstStub))
    {
        ControlStub FirstControlStub;

        FirstControlStub = std::get<ControlStub>(FirstStub);
        if (std::holds_alternative<ControlStub>(SecondStub))
        {
            ControlStub SecondControlStub;

            SecondControlStub = std::get<ControlStub>(SecondStub);
            if (FirstControlStub.Position.Column == SecondControlStub.Position.Column)
            {
                Overlaps = true;
            }
            else if (FirstControlStub.Position.Column == (SecondControlStub.Position.Column - 1u))
            {
                Overlaps = true;
            }
            else if (FirstControlStub.Position.Column == (SecondControlStub.Position.Column + 1u))
            {
                Overlaps = true;
            }
        }
    }
    else if (std::holds_alternative<MechanismStub>(FirstStub))
    {
        MechanismStub FirstMechanismStub;

        FirstMechanismStub = std::get<MechanismStub>(FirstStub);
        if (std::holds_alternative<MechanismStub>(SecondStub))
        {
            MechanismStub SecondMechanismStub;

            SecondMechanismStub = std::get<MechanismStub>(SecondStub);
            if (FirstMechanismStub.Position.Column == SecondMechanismStub.Position.Column)
            {
                Overlaps = true;
            }
            else if (FirstMechanismStub.Position.Column == (SecondMechanismStub.Position.Column - 1u))
            {
                Overlaps = true;
            }
            else if (FirstMechanismStub.Position.Column == (SecondMechanismStub.Position.Column + 1u))
            {
                Overlaps = true;
            }
        }
    }
    else if (std::holds_alternative<CallStub>(FirstStub))
    {
        CallStub FirstCallStub;

        FirstCallStub = std::get<CallStub>(FirstStub);
        if (std::holds_alternative<CallStub>(SecondStub))
        {
            CallStub SecondCallStub;

            SecondCallStub = std::get<CallStub>(SecondStub);
            if (FirstCallStub.Position.Column == SecondCallStub.Position.Column)
            {
                Overlaps = true;
            }
            else if (FirstCallStub.Position.Column == (SecondCallStub.Position.Column - 1u))
            {
                Overlaps = true;
            }
            else if (FirstCallStub.Position.Column == (SecondCallStub.Position.Column + 1u))
            {
                Overlaps = true;
            }
        }
    }

    return Overlaps;
}

void ShiftInputStubs(ActivityDiagram& Diagram)
{
    uint32_t NumInputStubs;

    NumInputStubs = Diagram.InputBoundaryStubs.size();
    for (uint32_t InputStubIndex = 0u; InputStubIndex < NumInputStubs; InputStubIndex++)
    {
        for (uint32_t OtherInputStubIndex = 0u; OtherInputStubIndex < NumInputStubs; OtherInputStubIndex++)
        {
            if (InputStubIndex != OtherInputStubIndex)
            {
                InputStub FirstStub;
                InputStub SecondStub;

                FirstStub = std::get<InputStub>(Diagram.InputBoundaryStubs[InputStubIndex]);
                SecondStub = std::get<InputStub>(Diagram.InputBoundaryStubs[OtherInputStubIndex]);
                for (uint32_t RowOffset = FirstStub.Position.Row; RowOffset > 0u; RowOffset--)
                {
                    if (CheckStubOverlap(FirstStub, SecondStub))
                    {
                        FirstStub.Position.Row--;
                    }
                }
                if (CheckStubOverlap(FirstStub, SecondStub))
                {
                    throw std::runtime_error("Could not shift input boundary stubs.");
                }
                Diagram.InputBoundaryStubs[InputStubIndex] = FirstStub;
            }
        }
    }
}

void ShiftOutputStubs(ActivityDiagram& Diagram)
{
    uint32_t NumOutputStubs;

    NumOutputStubs = Diagram.OutputBoundaryStubs.size();
    for (uint32_t OutputStubIndex = 0u; OutputStubIndex < NumOutputStubs; OutputStubIndex++)
    {
        for (uint32_t OtherOutputStubIndex = 0u; OtherOutputStubIndex < NumOutputStubs; OtherOutputStubIndex++)
        {
            if (OutputStubIndex != OtherOutputStubIndex)
            {
                OutputStub FirstStub;
                OutputStub SecondStub;

                FirstStub = std::get<OutputStub>(Diagram.OutputBoundaryStubs[OutputStubIndex]);
                SecondStub = std::get<OutputStub>(Diagram.OutputBoundaryStubs[OtherOutputStubIndex]);
                for (uint32_t RowOffset = FirstStub.Position.Row; RowOffset > 0u; RowOffset--)
                {
                    if (CheckStubOverlap(FirstStub, SecondStub))
                    {
                        FirstStub.Position.Row--;
                    }
                }
                if (CheckStubOverlap(FirstStub, SecondStub))
                {
                    throw std::runtime_error("Could not shift output boundary stubs.");
                }
                Diagram.OutputBoundaryStubs[OutputStubIndex] = FirstStub;
            }
        }
    }
}

void ShiftControlStubs(ActivityDiagram& Diagram)
{
    uint32_t NumControlStubs;

    NumControlStubs = Diagram.ControlBoundaryStubs.size();
    for (uint32_t ControlStubIndex = 0u; ControlStubIndex < NumControlStubs; ControlStubIndex++)
    {
        for (uint32_t OtherControlStubIndex = 0u; OtherControlStubIndex < NumControlStubs; OtherControlStubIndex++)
        {
            if (ControlStubIndex != OtherControlStubIndex)
            {
                ControlStub FirstStub;
                ControlStub SecondStub;

                FirstStub = std::get<ControlStub>(Diagram.ControlBoundaryStubs[ControlStubIndex]);
                SecondStub = std::get<ControlStub>(Diagram.ControlBoundaryStubs[OtherControlStubIndex]);
                for (uint32_t ColumnOffset = FirstStub.Position.Column; ColumnOffset < Diagram.Width; ColumnOffset++)
                {
                    if (CheckStubOverlap(FirstStub, SecondStub))
                    {
                        FirstStub.Position.Column++;
                    }
                }
                if (CheckStubOverlap(FirstStub, SecondStub))
                {
                    throw std::runtime_error("Could not shift control boundary stubs.");
                }
                Diagram.ControlBoundaryStubs[ControlStubIndex] = FirstStub;
            }
        }
    }
}

void ShiftMechanismStubs(ActivityDiagram& Diagram)
{
    uint32_t NumMechanismStubs;

    NumMechanismStubs = Diagram.MechanismBoundaryStubs.size();
    for (uint32_t MechanismStubIndex = 0u; MechanismStubIndex < NumMechanismStubs; MechanismStubIndex++)
    {
        for (uint32_t OtherMechanismStubIndex = 0u; OtherMechanismStubIndex < NumMechanismStubs; OtherMechanismStubIndex++)
        {
            if (MechanismStubIndex != OtherMechanismStubIndex)
            {
                MechanismStub FirstStub;
                MechanismStub SecondStub;

                FirstStub = std::get<MechanismStub>(Diagram.MechanismBoundaryStubs[MechanismStubIndex]);
                SecondStub = std::get<MechanismStub>(Diagram.MechanismBoundaryStubs[OtherMechanismStubIndex]);
                for (uint32_t MechanismOffset = FirstStub.Position.Column; MechanismOffset < Diagram.Width; MechanismOffset++)
                {
                    if (CheckStubOverlap(FirstStub, SecondStub))
                    {
                        FirstStub.Position.Column++;
                    }
                }
                if (CheckStubOverlap(FirstStub, SecondStub))
                {
                    throw std::runtime_error("Could not shift mechanism boundary stubs.");
                }
                Diagram.MechanismBoundaryStubs[MechanismStubIndex] = FirstStub;
            }
        }
    }
}

void ShiftBoundaryStubs(ActivityDiagram &Diagram)
{
    ShiftInputStubs(Diagram);
    ShiftOutputStubs(Diagram);
    ShiftControlStubs(Diagram);
    ShiftMechanismStubs(Diagram);
}

void LayoutBoundaryStubs(ActivityDiagram &Diagram, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxXGap, uint32_t BoxYGap)
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
    BoxSectionHeight = (NumBoxes * BoxHeight) + ((NumBoxes - 1u) * BoxYGap);
    BoxSectionWidth = (NumBoxes * BoxWidth) + ((NumBoxes - 1u) * BoxXGap);
    RowHeight = BoxSectionHeight / (1u + NumBoxes);
    ColumnWidth = BoxSectionWidth / (1u + NumBoxes);
    ColumnCenterOffset = (Diagram.Width / 2u) - (BoxSectionWidth / 2u);
    RowCenterOffset = (Diagram.Height / 2u) - (BoxSectionHeight / 2u);
    StubIndex = 0u;
    for (Stub& BoundaryStub : Diagram.InputBoundaryStubs)
    {
        Stub FoundStub;
        bool FoundFlag;
        InputStub& BoundaryInputStub = std::get<InputStub>(BoundaryStub);

        FindInnerStub(Diagram, BoundaryStub, FoundStub, FoundFlag);
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

                BoundaryInputStub.Position.Row = FoundControlStub.Position.Row - FoundControlStub.Length;
                BoundaryInputStub.Position.Column = 0u;
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

        FindInnerStub(Diagram, BoundaryStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<InputStub>(FoundStub))
            {
                InputStub& FoundInputStub = std::get<InputStub>(FoundStub);
                
                BoundaryControlStub.Position.Row = 0u;
                BoundaryControlStub.Position.Column = FoundInputStub.Position.Column - FoundInputStub.Length;
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

        FindInnerStub(Diagram, BoundaryStub, FoundStub, FoundFlag);
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

        FindInnerStub(Diagram, BoundaryStub, FoundStub, FoundFlag);
        if (FoundFlag)
        {
            if (std::holds_alternative<MechanismStub>(FoundStub))
            {
                MechanismStub& FoundMechanismStub = std::get<MechanismStub>(FoundStub);
                
                BoundaryMechanismStub.Position.Row = Diagram.Height - 1u - Diagram.Frame.BottomBar.Height;
                BoundaryMechanismStub.Position.Column = FoundMechanismStub.Position.Column;
            }
        }
        else
        {
            BoundaryMechanismStub.Position.Column = (1u+StubIndex) * ColumnWidth;
            BoundaryMechanismStub.Position.Row = Diagram.Height - 1u - Diagram.Frame.BottomBar.Height;
        }
        StubIndex++;
    }
}

void ChangeBoundaryStubLengths(ActivityDiagram& Diagram)
{
    uint32_t NumControlStubs;
    uint32_t NumOutputStubs;
    uint32_t NumMechanismStubs;
    uint32_t NumInputStubs;

    NumInputStubs = Diagram.InputBoundaryStubs.size();
    NumOutputStubs = Diagram.OutputBoundaryStubs.size();
    NumControlStubs = Diagram.ControlBoundaryStubs.size();
    NumMechanismStubs = Diagram.MechanismBoundaryStubs.size();
    for (uint32_t I = 0u; I < NumInputStubs; I++)
    {
        InputStub InputBoundaryStub;
        uint32_t StubsAbove;

        StubsAbove = 0u;
        InputBoundaryStub = std::get<InputStub>(Diagram.InputBoundaryStubs[I]);
        for (uint32_t K = 0u; K < NumInputStubs; K++)
        {
            if (I == K)
            {
                continue;
            }
            else
            {
                InputStub OtherInputBoundaryStub;

                OtherInputBoundaryStub = std::get<InputStub>(Diagram.InputBoundaryStubs[K]);
                if (OtherInputBoundaryStub.Position.Row < InputBoundaryStub.Position.Row)
                {
                    StubsAbove++;
                }
            }
        }
        InputBoundaryStub.Length = 3u + StubsAbove;
        Diagram.InputBoundaryStubs[I] = InputBoundaryStub;
    }
    for (uint32_t I = 0u; I < NumOutputStubs; I++)
    {
        OutputStub OutputBoundaryStub;
        uint32_t StubsAbove;

        StubsAbove = 0u;
        OutputBoundaryStub = std::get<OutputStub>(Diagram.OutputBoundaryStubs[I]);
        for (uint32_t K = 0u; K < NumOutputStubs; K++)
        {
            if (I == K)
            {
                continue;
            }
            else
            {
                OutputStub OtherOutputBoundaryStub;

                OtherOutputBoundaryStub = std::get<OutputStub>(Diagram.OutputBoundaryStubs[K]);
                if (OtherOutputBoundaryStub.Position.Row < OutputBoundaryStub.Position.Row)
                {
                    StubsAbove++;
                }
            }
        }
        OutputBoundaryStub.Length = 3u + StubsAbove;
        Diagram.OutputBoundaryStubs[I] = OutputBoundaryStub;
    }
    for (uint32_t I = 0u; I < NumControlStubs; I++)
    {
        ControlStub ControlBoundaryStub;
        uint32_t StubsLeftwards;

        StubsLeftwards = 0u;
        ControlBoundaryStub = std::get<ControlStub>(Diagram.ControlBoundaryStubs[I]);
        for (uint32_t K = 0u; K < NumControlStubs; K++)
        {
            if (I == K)
            {
                continue;
            }
            else
            {
                ControlStub OtherControlBoundaryStub;

                OtherControlBoundaryStub = std::get<ControlStub>(Diagram.ControlBoundaryStubs[K]);
                if (OtherControlBoundaryStub.Position.Column < ControlBoundaryStub.Position.Column)
                {
                    StubsLeftwards++;
                }
            }
        }
        ControlBoundaryStub.Length = 3u + StubsLeftwards;
        Diagram.ControlBoundaryStubs[I] = ControlBoundaryStub;
    }
    for (uint32_t I = 0u; I < NumMechanismStubs; I++)
    {
        MechanismStub MechanismBoundaryStub;
        uint32_t StubsLeftwards;

        StubsLeftwards = 0u;
        MechanismBoundaryStub = std::get<MechanismStub>(Diagram.MechanismBoundaryStubs[I]);
        for (uint32_t K = 0u; K < NumMechanismStubs; K++)
        {
            if (I == K)
            {
                continue;
            }
            else
            {
                MechanismStub OtherMechanismBoundaryStub;

                OtherMechanismBoundaryStub = std::get<MechanismStub>(Diagram.MechanismBoundaryStubs[K]);
                if (OtherMechanismBoundaryStub.Position.Column < MechanismBoundaryStub.Position.Column)
                {
                    StubsLeftwards++;
                }
            }
        }
        MechanismBoundaryStub.Length = 3u + StubsLeftwards;
        Diagram.MechanismBoundaryStubs[I] = MechanismBoundaryStub;
    }
}

void LayoutActivityDiagram(ActivityDiagram &LoadedDiagram, 
    uint32_t Width, 
    uint32_t Height, 
    uint32_t BoxWidth, 
    uint32_t BoxHeight, 
    uint32_t BoxXGap, 
    uint32_t BoxYGap)
{
    LoadedDiagram.Width = Width;
    LoadedDiagram.Height = Height;
    LayoutFrame(LoadedDiagram);
    LayoutBoxes(LoadedDiagram, BoxWidth, BoxHeight, BoxXGap, BoxYGap);
    LayoutBoxStubs(LoadedDiagram);
    LayoutBoundaryStubs(LoadedDiagram, BoxWidth, BoxHeight, BoxXGap, BoxYGap);
    ChangeBoundaryStubLengths(LoadedDiagram);
    ShiftBoundaryStubs(LoadedDiagram);
}

}