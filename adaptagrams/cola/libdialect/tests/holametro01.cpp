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

#include "libdialect/commontypes.h"
#include "libdialect/io.h"
#include "libdialect/util.h"
#include "libdialect/graphs.h"
#include "libdialect/opts.h"
#include "libdialect/logging.h"
#include "libdialect/hola.h"

using namespace dialect;

int main(void) {
    Graph_SP graph = buildGraphFromTglfFile("graphs/" "metro/sydney.tglf");
    HolaOpts opts;
    bool debug = false;
    if (debug) doHOLA(*graph, opts, new Logger("output/", "holametro01_", true));
    else doHOLA(*graph, opts);
    writeStringToFile(graph->writeTglf(), "output/" "holametro01_" "layout.tglf");
    writeStringToFile(graph->writeSvg(), "output/" "svg/" "holametro01_" "layout.svg");
}
