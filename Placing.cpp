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
#include "Layouting.h"
#include "Drawing.h"
#include "Placing.h"

namespace IDEF
{

std::map<Stub, Avoid::ConnEnd> PlaceBoxStubConnEnds(const ActivityDiagram& LayedOutDiagram)
{
    std::map<Stub, Avoid::ConnEnd> BoxStubsMap;
    uint32_t StubCount;
    
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
            StubColumn = SelectedInputStub.Position.Column - SelectedInputStub.Length;
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
            StubColumn = SelectedOutputStub.Position.Column + SelectedOutputStub.Length;
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
            StubRow = SelectedControlStub.Position.Row - SelectedControlStub.Length;
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
            StubRow = SelectedMechanismStub.Position.Row + SelectedMechanismStub.Length;
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
            StubRow = SelectedCallStub.Position.Row + SelectedCallStub.Length;
            AvoidX = StubColumn;
            AvoidY = LayedOutDiagram.Height - StubRow;
            ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
            BoxStubsMap.insert({SelectedStub, ConnectionEnd});
        } 
    }

    return BoxStubsMap;
}

std::map<Stub, Avoid::ConnEnd> PlaceBoundaryStubConnEnds(const ActivityDiagram& LayedOutDiagram)
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

        StubColumn = BoundaryInputStub.Position.Column + BoundaryInputStub.Length;
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

        StubColumn = BoundaryOutputStub.Position.Column - BoundaryOutputStub.Length;
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
        StubRow = BoundaryControlStub.Position.Row + BoundaryControlStub.Length;
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
        StubRow = BoundaryMechanismStub.Position.Row - BoundaryMechanismStub.Length;
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
        StubRow = BoundaryCallStub.Position.Row - BoundaryCallStub.Length;
        AvoidX = StubColumn;
        AvoidY = LayedOutDiagram.Height - StubRow;
        ConnectionEnd = Avoid::ConnEnd(Avoid::Point(AvoidX, AvoidY));
        BoundaryStubsMap.insert({BoundaryCallStub, ConnectionEnd});
    }
   
    return BoundaryStubsMap;
}

void PlaceObstacles(const ActivityDiagram &LayedoutDiagram, 
    std::vector<Avoid::Rectangle> &Rectangles)
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
                        
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& BoxInputSource : BoxInputStub.Sources)
                        {
                            if (BoxInputSource.StubName == BoundaryInputStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                                
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
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
                        
                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& BoxControlSource : BoxControlStub.Sources)
                        {
                            if (BoxControlSource.StubName == BoundaryInputStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                                
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
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

                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& BoxInputSource : BoxInputStub.Sources)
                        {
                            if (BoxInputSource.StubName == BoundaryControlStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                             
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
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

                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& BoxControlSource : BoxControlStub.Sources)
                        {
                            if (BoxControlSource.StubName == BoundaryControlStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                          
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
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

                        NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& BoxMechanismSource : BoxMechanismStub.Sources)
                        {
                            if (BoxMechanismSource.StubName == BoundaryMechanismStub.Name)
                            {
                                Avoid::ConnRef* NewConn;
                                
                                NewConn = new Avoid::ConnRef(ConstructedRouter, BoundaryStubPair.second, BoxStubPair.second);
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
    
                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& Source : OtherInputStub.Sources)
                        {
                            if (Source.StubName == FirstOutputStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
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
                        
                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& Source : OtherControlStub.Sources)
                        {
                            if (Source.StubName == FirstOutputStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, OtherBoxStubPair.second);                            }
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

                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& Source : BoundaryOutputStub.Sources)
                        {
                            if (Source.StubName == FirstOutputStub.Name)
                            {
                                Avoid::ConnRef* NewRef;
    
                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
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

                        NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
                    }
                    else
                    {
                        for (const StubSource& Source : BoundaryCallStub.Sources)
                        {
                            if (Source.StubName == FirstCallStub.Name)
                            {
                                Avoid::ConnRef* NewRef;

                                NewRef = new Avoid::ConnRef(ConstructedRouter, BoxStubPair.second, BoundaryBoxStubPair.second);
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

}
