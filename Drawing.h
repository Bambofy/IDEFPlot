#ifndef DRAWING_H
#define DRAWING_H

namespace IDEF
{

std::vector<std::string> DrawDiagram(const ActivityDiagram &TargetDiagram, 
	Avoid::Router *ConnectedRouter);

}

#endif
