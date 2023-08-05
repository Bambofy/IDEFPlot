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

int main(int argc, char **argv)
{
    char* InputFilePath;
    char* OutputFilePath;
    uint32_t DiagramWidth;
    uint32_t DiagramHeight;
    uint32_t BoxWidth;
    uint32_t BoxHeight;
    uint32_t BoxXGap;
    uint32_t BoxYGap;
    std::vector<IDEF::Model> Models;

    InputFilePath = argv[1u];
    OutputFilePath = argv[2u];
    DiagramWidth = std::atoi(argv[3u]);
    DiagramHeight = std::atoi(argv[4u]);
    BoxWidth = std::atoi(argv[5u]);
    BoxHeight = std::atoi(argv[6u]);
    BoxXGap = std::atoi(argv[7u]);
    BoxYGap = std::atoi(argv[8u]);
    Models = IDEF::LoadModelsFile(InputFilePath);
    for (IDEF::Model &SelectedModel : Models)
    {
        for (IDEF::ActivityDiagram &SelectedActivityDiagram : SelectedModel.ActivityDiagrams)
        {
            std::fstream OutputFileStream;
            std::map<IDEF::Stub, Avoid::ConnEnd> BoxStubsMap;
            std::map<IDEF::Stub, Avoid::ConnEnd> BoundaryStubsMap;       
            std::vector<Avoid::Rectangle> Obstacles;
            Avoid::Router *Router;
            std::vector<std::string> Diagram;
            uint32_t RowNumber;

            IDEF::LayoutActivityDiagram(SelectedActivityDiagram, DiagramWidth, DiagramHeight, BoxWidth, BoxHeight, BoxXGap, BoxYGap);
            IDEF::PlaceObstacles(SelectedActivityDiagram, Obstacles);
            BoxStubsMap = IDEF::PlaceBoxStubConnEnds(SelectedActivityDiagram);
            BoundaryStubsMap = IDEF::PlaceBoundaryStubConnEnds(SelectedActivityDiagram);
            Router = IDEF::ConstructRouter(BoxStubsMap,BoundaryStubsMap, Obstacles);
            Diagram = IDEF::DrawDiagram(SelectedActivityDiagram, Router);
            RowNumber = 0u;
            OutputFileStream.open(OutputFilePath, std::ios_base::out);
            for (const std::string &Line : Diagram)
            {
                OutputFileStream << Line;
                OutputFileStream << '\n';
                RowNumber = RowNumber + 1u;
            }
            OutputFileStream.close();
            Diagram.clear(); 
            delete Router;
        }
    }

    return 0;
}
