#ifndef PLACING_H
#define PLACING_H

namespace IDEF
{

std::map<Stub, Avoid::ConnEnd> PlaceBoxStubConnEnds(const ActivityDiagram& LayedOutDiagram);

std::map<Stub, Avoid::ConnEnd> PlaceBoundaryStubConnEnds(const ActivityDiagram& LayedOutDiagram);

void PlaceObstacles(const ActivityDiagram &LayedoutDiagram, 
    std::vector<Avoid::Rectangle> &Rectangles);

Avoid::Router *ConstructRouter(std::map<Stub, Avoid::ConnEnd> &BoxStubsMap,
    std::map<Stub, Avoid::ConnEnd>& BoundaryStubsMap,
    std::vector<Avoid::Rectangle> &Rectangles);

}

#endif
