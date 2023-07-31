/*
 * vim: ts=4 sw=4 et tw=0 wm=0
 *
 * libdialect - A library for computing DiAlEcT layouts:
 *                 D = Decompose/Distribute
 *                 A = Arrange
 *                 E = Expand/Emend
 *                 T = Transform
 *
 * Copyright (C) 2018  Monash University
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * See the file LICENSE.LGPL distributed with the library.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author(s):   Steve Kieffer   <http://skieffer.info>
*/

#include <string>
#include <vector>

#include "libdialect/commontypes.h"
#include "libdialect/io.h"
#include "libdialect/util.h"
#include "libdialect/graphs.h"
#include "libdialect/opts.h"
#include "libdialect/logging.h"
#include "libdialect/hola.h"

using namespace dialect;

using std::vector;
using std::string;

int main(void) {

    Graph_SP graph = buildGraphFromTglfFile("graphs/" "special/lone_node.tglf");
    HolaOpts opts;
    bool debug = false;
    if (debug) doHOLA(*graph, opts, new Logger("output/", "holalonenode_", true));
    else doHOLA(*graph, opts);
    writeStringToFile(graph->writeTglf(), "output/" "holalonenode_" "layout.tglf");

}
