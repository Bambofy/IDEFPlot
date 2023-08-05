PugiXMLTarget:
	$(CC) -g -std=c++20 -c pugixml/src/pugixml.cpp -Ipugixml/src/

LibAvoidTarget:
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/actioninfo.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/connectionpin.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/connector.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/connend.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/geometry.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/geomtypes.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/graph.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/hyperedge.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/hyperedgeimprover.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/hyperedgetree.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/junction.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/makepath.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/mtst.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/obstacle.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/orthogonal.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/router.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/scanline.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/shape.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/timer.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/vertices.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/viscluster.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/visibility.cpp -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c adaptagrams/cola/libavoid/vpsc.cpp -Iadaptagrams/cola/

IDEFPlotTarget:
	$(CC) -g -std=c++20 -c main.cpp -Ipugixml/src/ -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c Placing.cpp -Ipugixml/src/ -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c Drawing.cpp -Ipugixml/src/ -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c Layouting.cpp -Ipugixml/src/ -Iadaptagrams/cola/
	$(CC) -g -std=c++20 -c Loading.cpp -Ipugixml/src/ -Iadaptagrams/cola/
	$(CC) -g -std=c++20 main.o Placing.o Drawing.o Layouting.o Loading.o pugixml.o actioninfo.o connectionpin.o connector.o connend.o geometry.o geomtypes.o graph.o hyperedge.o hyperedgeimprover.o hyperedgetree.o junction.o makepath.o mtst.o obstacle.o orthogonal.o router.o scanline.o shape.o timer.o vertices.o viscluster.o visibility.o vpsc.o
