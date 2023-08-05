#ifndef DRAWING_H
#define DRAWING_H

namespace IDEF
{

std::vector<std::string> DrawDiagram(const ActivityDiagram &TargetDiagram, Avoid::Router *ConnectedRouter, uint32_t BoxHeight, uint32_t BoxWidth, uint32_t BoxMargin);

}

#endif
