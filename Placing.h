#ifndef PLACING_H
#define PLACING_H

namespace IDEF
{

std::map<Stub, Avoid::ConnEnd> PlaceBoxStubConnEnds(const ActivityDiagram& LayedOutDiagram, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin);
std::map<Stub, Avoid::ConnEnd> PlaceBoundaryStubConnEnds(const ActivityDiagram& LayedOutDiagram, const uint32_t BoxWidth, const uint32_t BoxHeight, const uint32_t BoxMargin);
void PlaceObstacles(const ActivityDiagram &LayedoutDiagram, std::vector<Avoid::Rectangle> &Rectangles, uint32_t BoxWidth, uint32_t BoxHeight, uint32_t BoxMargin);
Avoid::Router *ConstructRouter(std::map<Stub, Avoid::ConnEnd> &BoxStubsMap,
                               std::map<Stub, Avoid::ConnEnd>& BoundaryStubsMap,
                              std::vector<Avoid::Rectangle> &Rectangles);
}

#endif
