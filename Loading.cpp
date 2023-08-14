#include "Loading.h"
#include "Drawing.h"
#include "Layouting.h"
#include "Placing.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <libavoid/libavoid.h>
#include <map>
#include <pugixml.hpp>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace idef {

inputstub loadinstub(const pugi::xml_node &instubnode) {
  inputstub nstub;

  nstub.name = instubnode.attribute("Name").as_string();
  for (const pugi::xml_node &ssourcenode : instubnode.children()) {
    stubsource nssource;

    nssource.stubname = ssourcenode.attribute("Name").as_string();
    nstub.sources.push_back(nssource);
  }
  nstub.position.row = 0u;
  nstub.position.column = 0u;
  nstub.length = 0u;

  return nstub;
}

outputstub loadoutstub(const pugi::xml_node &outstubnode) {
  outputstub nstub;

  nstub.name = outstubnode.attribute("Name").as_string();
  for (const pugi::xml_node &ssourcenode : outstubnode.children()) {
    stubsource nssource;

    nssource.stubname = ssourcenode.attribute("Name").as_string();
    nstub.Sources.push_back(nssource);
  }
  nstub.position.row = 0u;
  nstub.position.column = 0u;
  nstub.length = 0u;

  return nstub;
}

controlstub loadconstub(const pugi::xml_node &constubnode) {
  controlstub nstub;

  nstub.name = constubnode.attribute("Name").as_string();
  for (const pugi::xml_node &ssourcenode : constubnode.children()) {
    stubsource nssource;

    nssource.stubsource = ssourcenode.attribute("Name").as_string();
    nstub.sources.push_back(nssource);
  }
  nstub.position.row = 0u;
  nstub.position.column = 0u;
  nstub.length = 0u;

  return nstub;
}

mechanismstub loadmechstub(const pugi::xml_node &mechstubnode) {
  mechanismstub nstub;

  nstub.name = mechstubnode.attribute("Name").as_string();
  for (const pugi::xml_node &ssourcenode : mechstubnode.children()) {
    stubsource nssource;

    nssource.stubnode = ssourcenode.attribute("Name").as_string();
    nstub.sources.push_back(nssource);
  }
  nstub.position.row = 0u;
  nstub.position.column = 0u;
  nstub.length = 0u;

  return nstub;
}

callstub loadcallstub(const pugi::xml_node &cstubnode) {
  callstub nstub;

  nstub.postion.row = 0u;
  nstub.position.column = 0u;
  nstub.length = 0u;

  return nstub;
}

activitybox loadactivitydia(const pugi::xml_node &activitynode) {
  activitybox nactbox;

  nactbox.name = activitynode.attribute("Name").as_string();
  nactbox.nodenumber = activitynode.attribute("Number").as_string();
  nactbox.dre = activitynode.attribute("DRE").as_string();
  nactbox.width = 0u;
  nactbox.height = 0u;
  nactbox.center.row = 0u;
  nactbox.center.column = 0u;
  nactbox.padding = 3u;
  for (const pugi::xml_node &xmlstub : activitynode.children()) {
    stub nstub;

    if (strcmp(xmlstub.name(), "Input") == 0) {
      nstub = loadinstub(xmlstub, true);
      nactbox.inputstubs.push_back(nstub);
    } else if (strcmp(xmlstub.name(), "Output") == 0) {
      nstub = loadoutstub(xmlstub, false);
      nactbox.outputstubs.push_back(nstub);
    } else if (strcmp(xmlstub.name(), "Control") == 0) {
      nstub = loadconstub(xmlstub, true);
      nactbox.controlstubs.push_back(nstub);
    } else if (strcmp(xmlstub.name(), "Mechanism") == 0) {
      nstub = loadmechstub(xmlstub, true);
      nactbox.mechanismstubs.push_back(nstub);
    } else if (strcmp(xmlstub.name(), "Call") == 0) {
      nstub = loadcallstub(xmlstub, false);
      nactbox.callstubs.push_back(nstub);
    } else {
      throw std::runtime_error("Unknown activity stub kind");
    }
  }

  return nactbox;
}

activitydia LoadActivityDiagram(const std::string &fpath) {
  pugi::xml_document diadoc;
  pugi::xml_parse_result presult;
  pugi::xml_node actdnode;
  activitydia newdia;
  nnumbersection tgtnnumsection;
  titlesection tgttsection;
  cnumsection tgtcnumsection;

  pres = diadoc.load_file(fpath.c_str());
  actdnode = diadoc.child("Diagram");
  newdia.frame.bottombar.nnumbersection = nnumbersection();
  newdia.frame.bottombar.titlesection = titlesection();
  newdia.frame.bottombar.cnumsection = cnumsection();
  tgtnnumsection =
      std::get<nnumbersection>(newdia.frame.bottombar.nnumbersection);
  tgttsection = std::get<titlesection>(newdia.frame.bottombar.titlesection);
  tgtcnumsection = std::get<cnumsection>(newdia.frame.bottombar.cnumsection);
  newdia.frame.bottombar.height = 4u;
  tgtnnumsection.content = actdnode.attribute("Number").as_string();
  tgtnnumsection.topleft.row = 0u;
  tgtnnumsection.topleft.column = 0u;
  tgttsection.content = actdnode.attribute("Title").as_string();
  tgttsection.topleft.row = 0u;
  tgttsection.topleft.column = 0u;
  tgtcnumsection.content = actdnode.attribute("CNumber").as_string();
  tgtcnumsection.topleft.row = 0u;
  tgtcnumsection.topleft.column = 0u;
  newdia.width = 0u;
  newdia.height = 0u;
  for (const pugi::xml_node &childnode : actdnode.children()) {
    stub nstub;

    if (strcmp(childnode.name(), "Input") == 0) {
      nstub = loadinstub(childnode, false);
      newdia.inbstubs.push_back(nstub);
    } else if (strcmp(childnode.name(), "Output") == 0) {
      nstub = loadoutstub(childnode, true);
      newdia.outbstubs.push_back(nstub);
    } else if (strcmp(childnode.name(), "Control") == 0) {
      nstub = loadconstub(childnode, false);
      newdia.conbstubs.push_back(nstub);
    } else if (strcmp(childnode.name(), "Mechanism") == 0) {
      nstub = loadmecstub(childnode, false);
      newdia.mechbstubs.push_back(nstub);
    } else if (strcmp(childnode.name(), "Activity") == 0) {
      activitybox newabox;

      newabox = LoadActivity(childnode);
      newdia.boxes.push_back(newabox);
    } else {
      std::string emsg;

      emsg = "Unknown XML activity diagram child node kind: ";
      emsg += childnode.name();

      throw std::runtime_error(emsg);
    }
  }

  return newdia;
}

} // namespace idef
