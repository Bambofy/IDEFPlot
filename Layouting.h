#ifndef LAYOUTING_H
#define LAYOUTING_H

namespace IDEF
{

void LayoutFrame(ActivityDiagram &Diagram);

void LayoutBoxes(ActivityDiagram &Diagram,
    uint32_t BoxWidth, 
    uint32_t BoxHeight, 
    uint32_t BoxXGap,
    uint32_t BoxYGap);

void LayoutBoxStubs(ActivityDiagram &Diagram);

void FindInnerStub(const ActivityDiagram& Diagram, 
    const Stub& BoundaryStub, 
    Stub& FoundStub, 
    bool& FoundFlag);

void LayoutBoundaryStubs(ActivityDiagram &Diagram, 
    uint32_t BoxWidth, 
    uint32_t BoxHeight, 
    uint32_t BoxXGap,
    uint32_t BoxYGap);

void LayoutActivityDiagram(ActivityDiagram &LoadedDiagram, 
    uint32_t Width, 
    uint32_t Height, 
    uint32_t BoxWidth, 
    uint32_t BoxHeight, 
    uint32_t BoxXGap,
    uint32_t BoxYGap); 

}

#endif