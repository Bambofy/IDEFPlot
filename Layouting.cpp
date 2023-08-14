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

#include "Drawing.h"
#include "Layouting.h"
#include "Placing.h"
#include "loading.h"

namespace idef {

void layframe(activitydia &dia) {
  nnumbersection tgtnnsec;
  titlesection tgttitlesec;
  cnumsection tgtcnumsec;

  tgtnnsec = std::get<nnumbersection>(dia.frame.bottombar.nnumbersection);
  tgttitlesec = std::get<titlesection>(dia.frame.bottombar.titlesection);
  tgtcnumsec = = std::get<cnumsection>(dia.frame.bottombar.cnumsection);
  dia.frame.bottombar.height = 3u;
  dia.frame.bottombar.height.row = dia.height - dia.frame.bottombar.height;
  dia.frame.bottombar.height.column = 0u;
  tgtnnsec.width = dia.width / 4u;
  tgtnnsec.height.row = dia.frame.bottombar.height.row;
  tgtnnsec.height.column = dia.frame.bottombar.height.column;
  tgtnnsec.height = dia.frame.bottombar.height;
  tgttitlesec.width = 2u * (dia.width / 4u);
  tgttitlesec.height.row = dia.frame.bottombar.height.row;
  tgttitlesec.height.column = tgtnnsec.height.column + tgtnnsec.width;
  tgttitlesec.height = dia.frame.bottombar.height;
  tgtcnumsec.width = dia.width / 4u;
  tgtcnumsec.height.row = dia.frame.bottombar.height.row;
  tgtcnumsec.height.column = tgttitlesec.height.column + tgttitlesec.width;
  tgtcnumsec.height = dia.frame.bottombar.height;
  dia.frame.bottombar.nnumbersection = tgtnnsec;
  dia.frame.bottombar.titlesection = tgttitlesec;
  dia.frame.bottombar.cnumsection = tgtcnumsec;
}

void layboxes(activitydia &dia, uint32_t bwidth, uint32_t bheight,
              uint32_t xgap, uint32_t ygap) {
  uint32_t nboxes;
  filepos curs;
  uint32_t ccenteroff;
  uint32_t rcenteroff;
  uint32_t rheight;
  uint32_t cwidth;
  uint32_t bsheight;
  uint32_t bswidth;

  nboxes = dia.boxes.size();
  bsheight = (nboxes * bheight) + ((nboxes - 1u) * ygap);
  bswidth = (nboxes * bwidth) + ((nboxes - 1u) * xgap);
  rheight = bsheight / (1u + nboxes);
  cwidth = bswidth / (1u + nboxes);
  ccenteroff = (dia.width / 2u) - (bswidth / 2u);
  rcenteroff = (dia.height / 2u) - (bsheight / 2u);
  curs.column = ccenteroff;
  curs.row = rcenteroff;
  curs.column += (bwidth / 2u);
  curs.row += (bheight / 2u);
  for (uint32_t bindx = 0u; bindx < nboxes; bindx++) {
    activitybox sbox;

    sbox = dia.boxes[bindx];
    sbox.width = bwidth;
    sbox.height = bheight;
    sbox.center.column = curs.column;
    sbox.center.row = curs.row;
    dia.boxes[bindx] = sbox;
    curs.column += bwidth + xgap;
    curs.row += bwidth + ygap;
  }
}

void layboxstubs(activitydia &dia) {
  uint32_t nboxes;

  nboxes = dia.boxes.size();
  for (uint32_t bindx = 0u; bindx < nboxes; bindx++) {
    activitybox &sbox = dia.boxes[bindx];
    uint8_t nistubs;
    uint8_t nostubs;
    uint8_t nconstubs;
    uint8_t nmechstubs;
    uint8_t ncallstubs;
    uint8_t numidivs;
    uint8_t idivwidth;
    uint8_t numodivs;
    uint8_t odivwidth;
    uint8_t numcondivs;
    uint8_t conidivwidth;
    uint8_t nummechdivs;
    uint8_t mechdivwidth;
    uint8_t numcalldivs;
    uint8_t callidivwidth;

    nistubs = sbox.inputstubs.size();
    nostubs = sbox.outputstubs.size();
    nconstubs = sbox.controlstubs.size();
    nmechstubs = sbox.mechanismstubs.size();
    ncallstubs = sbox.callstubs.size();
    numidivs = nistubs + 1u;
    numodivs = nostubs + 1u;
    nummechdivs = nmechstubs + 1u;
    numcondivs = nconstubs + 1u;
    numcalldivs = ncallstubs + 1u;
    idivwidth = sbox.height / numidivs;
    odivwidth = sbox.height / numodivs;
    condivwidth = sbox.width / numcondivs;
    mechdivwidth = (sbox.width / 2u) / nummechdivs;
    calldivwidth = (sbox.width / 2) / numcalldivs;
    for (uint32_t istubi = 0u; istubi < nistubs; istubi++) {
      inputstub &selistub = std::get<inputstub>(sbox.inputstubs[istubi]);
      uint32_t rowo;

      rowo = idivwidth * (1u + istubi);
      selistub.position.column = sbox.center.column - (sbox.width / 2u);
      selistub.position.row = sbox.center.row - (sbox.height / 2u);
      selistub.position.row = selistub.position.row + rowo;
      selistub.length = 3u + istubi;
    }
    for (uint32_t ostubi = 0u; ostubi < nostubs; ostubi++) {
      outputstub &selostub = std::get<outputstub>(sbox.outputstubs[ostubi]);
      uint32_t rowo;

      rowo = odivwidth * (1u + ostubi);
      selostub.position.column = sbox.center.column + (sbox.width / 2u);
      selostub.position.row = sbox.center.row - (sbox.height / 2u);
      selostub.position.row = selostub.position.row + rowo;
      selostub.length = 3u + ostubi;
    }
    for (uint32_t constubi = 0u; constubi < nconstubs; constubi++) {
      controlstub &selconstub =
          std::get<controlstub>(sbox.controlstubs[constubi]);
      uint32_t columnoff;

      columnoff = condivwidth * (1u + constubi);
      selconstub.position.column = sbox.center.column - (sbox.width / 2u);
      selconstub.position.row = sbox.center.row - (sbox.height / 2u);
      selconstub.position.column = selconstub.position.column + columnoff;
      selconstub.length = 3u + constubi;
    }
    for (uint32_t mechstubi = 0u; mechstubi < nmechstubs; mechstubi++) {
      mechanismstub &selmechstub =
          std::get<mechanismstub>(sbox.mechanismstubs[mechstubi]);
      uint32_t columnoff;

      columnoff = mechdivwidth * (1u + mechstubi);
      selmechstub.position.column = sbox.center.column - (sbox.width / 2u);
      selmechstub.position.row = sbox.center.row + (sbox.height / 2u);
      selmechstub.position.column = selmechstub.position.column + columnoff;
      selmechstub.length = 3u + mechstubi;
    }
    for (uint32_t callstubi = 0u; callstubi < ncallstubs; callstubi++) {
      callstub &selcallstub = std::get<callstub>(sbox.callstubs[callstubi]);
      uint32_t columnoff;

      columnoff = calldivwidth * (1u + callstubi);
      selcallstub.position.column = sbox.center.column;
      selcallstub.position.row = sbox.center.row + (sbox.height / 2u);
      selcallstub.position.column = selcallstub.position.column + columnoff;
      selcallstub.length = 4u;
    }
  }
}

void finnerstub(const activitydia &dia, const stub &bstub, stub &fstub,
                bool &fflag) {

  fflag = false;
  for (const activitybox &sbox : dia.boxes) {
    if (std::holds_alternative<inputstub>(bstub)) {
      const inputstub &binstub = std::get<inputstub>(bstub);

      for (const stub &selstub : sbox.inputstubs) {
        const inputstub &selinstub = std::get<inputstub>(selstub);

        if (selinstub.name == binstub.name) {
          fstub = selinstub;
          fflag = true;
        } else {
          for (const stubsource &stubsource : selinstub.sources) {
            if (stubsource.stubname == binstub.name) {
              fstub = selinstub;
              fflag = true;
            }
          }
        }
      }
      for (const stub &selstub : sbox.controlstubs) {
        const controlstub &selconstub = std::get<controlstub>(selstub);

        if (selconstub.name == binstub.name) {
          fstub = selconstub;
          fflag = true;
        } else {
          for (const stubsource &stubsource : selconstub.sources) {
            if (stubsource.stubname == binstub.name) {
              fstub = selconstub;
              fflag = true;
            }
          }
        }
      }
    } else if (std::holds_alternative<controlstub>(bstub)) {
      const controlstub &bconstub = std::get<controlstub>(bstub);

      for (const stub &selstub : sbox.inputstubs) {
        const inputstub &selinstub = std::get<inputstub>(selstub);

        if (selinstub.name == bconstub.name) {
          fstub = selinstub;
          fflag = true;
        } else {
          for (const stubsource &selstubsrc : selinstub.sources) {
            if (selstubsrc.stubname == bconstub.name) {
              fstub = selinstub;
              fflag = true;
            }
          }
        }
      }
      for (const stub &selstub : sbox.controlstubs) {
        const controlstub &selconstub = std::get<controlstub>(selstub);

        if (selconstub.name == bconstub.name) {
          fstub = selconstub;
          fflag = true;
        } else {
          for (const stubsource &selstubsrc : selconstub.sources) {
            if (selstubsrc.stubname == bconstub.name) {
              fstub = selconstub;
              fflag = true;
            }
          }
        }
      }
    } else if (std::holds_alternative<outputstub>(bstub)) {
      const outputstub &boutstub = std::get<outputstub>(bstub);

      for (const stub &selstub : sbox.a) {
        const outputstub &selostub = std::get<outputstub>(selstub);

        if (selostub.name == boutstub.name) {
          fstub = selostub;
          fflag = true;
        } else {
          for (const stubsource &bstubsrc : boutstub.sources) {
            if (bstubsrc.stubname == selostub.name) {
              fstub = selostub;
              fflag = true;
            }
          }
        }
      }
    } else if (std::holds_alternative<mechanismstub>(bstub)) {
      const mechanismstub &bmechstub = std::get<mechanismstub>(bstub);

      for (const stub &selstub : sbox.MechanismStubs) {
        const mechanismstub &bxmechstub = std::get<mechanismstub>(selstub);

        if (bmechstub.name == bxmechstub.name) {
          fstub = bxmechstub;
          fflag = true;
        } else {
          for (const stubsource &BoxStubSource : bxmechstub.sources) {
            if (BoxStubSource.stubname == bmechstub.name) {
              fstub = bxmechstub;
              fflag = true;
            }
          }
        }
      }
    } else if (std::holds_alternative<callstub>(bstub)) {
      const callstub &bcallstub = std::get<callstub>(bstub);

      for (const stub &selstub : sbox.CallStubs) {
        const callstub &bxcallstub = std::get<callstub>(selstub);

        if (bcallstub.name == bxcallstub.name) {
          fstub = bxcallstub;
          fflag = true;
        } else {
          for (const stubsource &bstubsrc : bcallstub.sources) {
            if (bstubsrc.stubname == bxcallstub.name) {
              fstub = bxcallstub;
              fflag = true;
            }
          }
        }
      }
    }
    if (fflag == true) {
      break;
    }
  }
}

std::vector<activitybox> lconnboxes(activitydia &dia, std::string &tsname) {
  std::vector<activitybox> connboxes;

  for (activitybox &sbox : dia.boxes) {
  }
  for (stub &sstub : sbox.inputstubs) {
    inputstub &selinstub = std::get<inputstub>(sstub);

    if (selinstub.name == tsname) {
      connboxes.push_back(sbox);
    } else {
      for (stubsource &selsource : selinstub.sources) {
        if (selsource.stubname == tsname) {
          connboxes.push_back(sbox);
        }
      }
    }
  }
  for (stub &sstub : sbox.outputstubs) {
    outputstub &selostub = std::get<outputstub>(sstub);

    if (selostub.name == tsname) {
      connboxes.push_back(sbox);
    } else {
      for (stubsource &selsource : selostub.sources) {
        if (selsource.stubname == tsname) {
          connboxes.push_back(sbox);
        }
      }
    }
  }
  for (stub &sstub : sbox.controlstubs) {
    controlstub &selconstub = std::get<controlstub>(sstub);

    if (selconstub.name == tsname) {
      connboxes.push_back(sbox);
    } else {
      for (stubsource &selsource : selconstub.sources) {
        if (selsource.stubname == tsname) {
          connboxes.push_back(sbox);
        }
      }
    }
  }
  for (stub &sstub : sbox.mechanismstubs) {
    mechanismstub &selmstub = std::get<mechanismstub>(sstub);

    if (selmstub.name == tsname) {
      connboxes.push_back(sbox);
    } else {
      for (stubsource &selsource : selmstub.sources) {
        if (selsource.stubname == tsname) {
          connboxes.push_back(sbox);
        }
      }
    }
  }
  for (stub &sstub : sbox.callstubs) {
    callstub &selcallstub = std::get<callstub>(sstub);

    if (selcallstub.name == tsname) {
      connboxes.push_back(sbox);
    } else {
      for (stubsource &selsource : selcallstub.sources) {
        if (selsource.stubname == tsname) {
          connboxes.push_back(sbox);
        }
      }
    }
  }
}

return connboxes;
}

bool checksoverlap(stub fstub, stub sstub) {
  bool ov;

  ov = false;
  if (std::holds_alternative<inputstub>(fstub)) {
    inputstub firstistub;

    firstistub = std::get<inputstub>(fstub);
    if (std::holds_alternative<inputstub>(sstub)) {
      inputstub secondistub;

      secondistub = std::get<inputstub>(sstub);
      if (firstistub.position.row == secondistub.position.row) {
        ov = true;
      } else if (firstistub.position.row == (secondistub.position.row - 1u)) {
        ov = true;
      } else if (firstistub.position.row == (secondistub.position.row + 1u)) {
        ov = true;
      }
    }
  } else if (std::holds_alternative<outputstub>(fstub)) {
    outputstub firstostub;

    firstostub = std::get<outputstub>(fstub);
    if (std::holds_alternative<outputstub>(sstub)) {
      outputstub secondostub;

      secondostub = std::get<outputstub>(sstub);
      if (firstostub.position.row == secondostub.position.row) {
        ov = true;
      } else if (firstostub.position.row == (secondostub.position.row - 1u)) {
        ov = true;
      } else if (firstostub.position.row == (secondostub.position.row + 1u)) {
        ov = true;
      }
    }
  } else if (std::holds_alternative<controlstub>(fstub)) {
    controlstub firstconstub;

    firstconstub = std::get<controlstub>(fstub);
    if (std::holds_alternative<controlstub>(sstub)) {
      controlstub secondconstub;

      secondconstub = std::get<controlstub>(sstub);
      if (firstconstub.position.column == secondconstub.position.column) {
        ov = true;
      } else if (firstconstub.position.column ==
                 (secondconstub.position.column - 1u)) {
        ov = true;
      } else if (firstconstub.position.column ==
                 (secondconstub.position.column + 1u)) {
        ov = true;
      }
    }
  } else if (std::holds_alternative<mechanismstub>(fstub)) {
    mechanismstub firstmechstub;

    firstmechstub = std::get<mechanismstub>(fstub);
    if (std::holds_alternative<mechanismstub>(sstub)) {
      mechanismstub secondmechstub;

      secondmechstub = std::get<mechanismstub>(sstub);
      if (firstmechstub.position.column == secondmechstub.position.column) {
        ov = true;
      } else if (firstmechstub.position.column ==
                 (secondmechstub.position.column - 1u)) {
        ov = true;
      } else if (firstmechstub.position.column ==
                 (secondmechstub.position.column + 1u)) {
        ov = true;
      }
    }
  } else if (std::holds_alternative<callstub>(fstub)) {
    callstub firstcstub;

    firstcstub = std::get<callstub>(fstub);
    if (std::holds_alternative<callstub>(sstub)) {
      callstub secondcstub;

      secondcstub = std::get<callstub>(sstub);
      if (FirstCallStub.position.column == secondcstub.position.column) {
        ov = true;
      } else if (FirstCallStub.position.column ==
                 (secondcstub.position.column - 1u)) {
        ov = true;
      } else if (FirstCallStub.position.column ==
                 (secondcstub.position.column + 1u)) {
        ov = true;
      }
    }
  }

  return ov;
}

void sinstubs(activitydia &dia) {
  uint32_t ninstubs;

  ninstubs = dia.inbstubs.size();
  for (uint32_t instubi = 0u; instubi < ninstubs; instubi++) {
    for (uint32_t oinstubi = 0u; oinstubi < ninstubs; oinstubi++) {
      if (instubi != oinstubi) {
        inputstub fstub;
        inputstub sstub;
        uint32_t roff;

        fstub = std::get<inputstub>(dia.inbstubs[instubi]);
        sstub = std::get<inputstub>(dia.inbstubs[oinstubi]);
        for (roff = fstub.position.row; roff > 0u; roff--) {
          if (checksoverlap(fstub, sstub)) {
            fstub.position.row--;
          }
        }
        if (checksoverlap(fstub, sstub)) {
          throw std::runtime_error("Could not shift input boundary stubs.");
        }
        dia.inbstubs[instubi] = fstub;
      }
    }
  }
}

void soutstubs(activitydia &dia) {
  uint32_t nostubs;
  uint32_t outstubi;
  uint32_t ooutstubi;

  nostubs = dia.outbstubs.size();
  for (outstubi = 0u; outstubi < nostubs; outstubi++) {
    for (ooutstubi = 0u; ooutstubi < nostubs; ooutstubi++) {
      if (outstubi != ooutstubi) {
        outputstub fstub;
        outputstub sstub;
        uint32_t roff;

        fstub = std::get<outputstub>(dia.outbstubs[outstubi]);
        sstub = std::get<outputstub>(dia.outbstubs[ooutstubi]);
        for (roff = fstub.position.row; roff > 0u; roff--) {
          if (checksoverlap(fstub, sstub)) {
            fstub.position.row--;
          }
        }
        if (checksoverlap(fstub, sstub)) {
          throw std::runtime_error("Could not shift output boundary stubs.");
        }
        dia.outbstubs[outstubi] = FirstStub;
      }
    }
  }
}

void sconstubs(activitydia &dia) {
  uint32_t nconstubs;
  uint32_t cstubi;
  uint32_t ostubi;

  nconstubs = dia.conbstubs.size();
  for (cstubi = 0u; cstubi < nconstubs; cstubi++) {
    for (ostubi = 0u; ostubi < nconstubs; ostubi++) {
      if (cstubi != ostubi) {
        controlstub fstub;
        controlstub sstub;
        uint32_t coff;

        fstub = std::get<controlstub>(dia.conbstubs[cstubi]);
        sstub = std::get<controlstub>(dia.conbstubs[ostubi]);
        for (coff = fstub.position.column; coff < dia.width; coff++) {
          if (checksoverlap(fstub, sstub)) {
            fstub.position.column++;
          }
        }
        if (checksoverlap(fstub, sstub)) {
          throw std::runtime_error("Could not shift control boundary stubs.");
        }
        dia.conbstubs[cstubi] = fstub;
      }
    }
  }
}

void smechstubs(activitydia &dia) {
  uint32_t nmechstubs;
  uint32_t mstubi;
  uint32_t omstubi;

  nmechstubs = dia.mecbstubs.size();
  for (mstubi = 0u; mstubi < nmechstubs; mstubi++) {
    for (omstubi = 0u; omstubi < nmechstubs; omstubi++) {
      if (mstubi != omstubi) {
        mechanismstub fstub;
        mechanismstub sstub;
        uint32_t mstuboff;

        fstub = std::get<mechanismstub>(dia.mecbstubs[mstubi]);
        sstub = std::get<mechanismstub>(dia.mecbstubs[omstubi]);
        for (mstuboff = fstub.position.column; mstuboff < dia.width;
             mstuboff++) {
          if (checksoverlap(fstub, sstub)) {
            fstub.position.column++;
          }
        }
        if (checksoverlap(fstub, sstub)) {
          throw std::runtime_error("Could not shift mechanism boundary stubs.");
        }
        dia.mechbstubs[mstubi] = fstub;
      }
    }
  }
}

void sbstubs(activitydia &dia) {
  sinstubs(dia);
  soutstubs(dia);
  sconstubs(dia);
  smechstubs(dia);
}

void laybstubs(activitydia &dia, uint32_t bwidth, uint32_t bheight,
               uint32_t bxgap, uint32_t bygap) {
  uint32_t BoxSectionheight;
  uint32_t BoxSectionwidth;
  uint32_t Rowheight;
  uint32_t columnwidth;
  uint32_t NumBoxes;
  uint32_t StubIndex;
  uint32_t columnCenterOffset;
  uint32_t RowCenterOffset;

  NumBoxes = dia.boxes.size();
  BoxSectionheight = (NumBoxes * bheight) + ((NumBoxes - 1u) * bygap);
  BoxSectionwidth = (NumBoxes * bwidth) + ((NumBoxes - 1u) * bxgap);
  Rowheight = BoxSectionheight / (1u + NumBoxes);
  columnwidth = BoxSectionwidth / (1u + NumBoxes);
  columnCenterOffset = (dia.width / 2u) - (BoxSectionwidth / 2u);
  RowCenterOffset = (dia.height / 2u) - (BoxSectionheight / 2u);
  StubIndex = 0u;
  for (stub &BoundaryStub : dia.InputBoundaryStubs) {
    stub FoundStub;
    bool FoundFlag;
    inputstub &BoundaryInputStub = std::get<inputstub>(BoundaryStub);

    FindInnerStub(dia, BoundaryStub, FoundStub, FoundFlag);
    if (FoundFlag) {
      if (std::holds_alternative<inputstub>(FoundStub)) {
        inputstub &FoundInputStub = std::get<inputstub>(FoundStub);

        BoundaryInputStub.position.row = FoundInputStub.position.row;
        BoundaryInputStub.position.column = 0u;
      } else if (std::holds_alternative<controlstub>(FoundStub)) {
        controlstub &FoundControlStub = std::get<controlstub>(FoundStub);

        BoundaryInputStub.position.row =
            FoundControlStub.position.row - FoundControlStub.length;
        BoundaryInputStub.position.column = 0u;
      }
    } else {
      BoundaryInputStub.position.column = 0u;
      BoundaryInputStub.position.row = (1u + StubIndex) * Rowheight;
    }
    StubIndex++;
  }
  StubIndex = 0u;
  for (stub &BoundaryStub : dia.ControlBoundaryStubs) {
    stub FoundStub;
    bool FoundFlag;
    controlstub &BoundaryControlStub = std::get<controlstub>(BoundaryStub);

    FindInnerStub(dia, BoundaryStub, FoundStub, FoundFlag);
    if (FoundFlag) {
      if (std::holds_alternative<inputstub>(FoundStub)) {
        inputstub &FoundInputStub = std::get<inputstub>(FoundStub);

        BoundaryControlStub.position.row = 0u;
        BoundaryControlStub.position.column =
            FoundInputStub.position.column - FoundInputStub.length;
      } else if (std::holds_alternative<controlstub>(FoundStub)) {
        controlstub &FoundControlStub = std::get<controlstub>(FoundStub);

        BoundaryControlStub.position.row = 0u;
        BoundaryControlStub.position.column = FoundControlStub.position.column;
      }
    } else {
      BoundaryControlStub.position.column = (1u + StubIndex) * columnwidth;
      BoundaryControlStub.position.row = 0u;
    }
    StubIndex++;
  }
  StubIndex = 0u;
  for (stub &BoundaryStub : dia.OutputBoundaryStubs) {
    stub FoundStub;
    bool FoundFlag;
    outputstub &BoundaryOutputStub = std::get<outputstub>(BoundaryStub);

    FindInnerStub(dia, BoundaryStub, FoundStub, FoundFlag);
    if (FoundFlag) {
      if (std::holds_alternative<outputstub>(FoundStub)) {
        outputstub &FoundOutputStub = std::get<outputstub>(FoundStub);

        BoundaryOutputStub.position.row = FoundOutputStub.position.row;
        BoundaryOutputStub.position.column = dia.width - 1u;
      }
    } else {
      BoundaryOutputStub.position.column = dia.width - 1u;
      BoundaryOutputStub.position.row = (1u + StubIndex) * Rowheight;
    }
    StubIndex++;
  }
  StubIndex = 0u;
  for (stub &BoundaryStub : dia.MechanismBoundaryStubs) {
    stub FoundStub;
    bool FoundFlag;
    mechanismstub &BoundaryMechanismStub =
        std::get<mechanismstub>(BoundaryStub);

    FindInnerStub(dia, BoundaryStub, FoundStub, FoundFlag);
    if (FoundFlag) {
      if (std::holds_alternative<mechanismstub>(FoundStub)) {
        mechanismstub &FoundMechanismStub = std::get<mechanismstub>(FoundStub);

        BoundaryMechanismStub.position.row =
            dia.height - 1u - dia.frame.bottombar.height;
        BoundaryMechanismStub.position.column =
            FoundMechanismStub.position.column;
      }
    } else {
      BoundaryMechanismStub.position.column = (1u + StubIndex) * columnwidth;
      BoundaryMechanismStub.position.row =
          dia.height - 1u - dia.frame.bottombar.height;
    }
    StubIndex++;
  }
}

void cbstublens(activitydia &dia) {
  uint32_t NumControlStubs;
  uint32_t nostubs;
  uint32_t NumMechanismStubs;
  uint32_t ninstubs;

  ninstubs = dia.InputBoundaryStubs.size();
  nostubs = dia.OutputBoundaryStubs.size();
  NumControlStubs = dia.ControlBoundaryStubs.size();
  NumMechanismStubs = dia.MechanismBoundaryStubs.size();
  for (uint32_t I = 0u; I < ninstubs; I++) {
    inputstub InputBoundaryStub;
    uint32_t StubsAbove;

    StubsAbove = 0u;
    InputBoundaryStub = std::get<inputstub>(dia.InputBoundaryStubs[I]);
    for (uint32_t K = 0u; K < ninstubs; K++) {
      if (I == K) {
        continue;
      } else {
        inputstub OtherInputBoundaryStub;

        OtherInputBoundaryStub = std::get<inputstub>(dia.InputBoundaryStubs[K]);
        if (OtherInputBoundaryStub.position.row <
            InputBoundaryStub.position.row) {
          StubsAbove++;
        }
      }
    }
    InputBoundaryStub.length = 3u + StubsAbove;
    dia.InputBoundaryStubs[I] = InputBoundaryStub;
  }
  for (uint32_t I = 0u; I < nostubs; I++) {
    outputstub OutputBoundaryStub;
    uint32_t StubsAbove;

    StubsAbove = 0u;
    OutputBoundaryStub = std::get<outputstub>(dia.OutputBoundaryStubs[I]);
    for (uint32_t K = 0u; K < nostubs; K++) {
      if (I == K) {
        continue;
      } else {
        outputstub OtherOutputBoundaryStub;

        OtherOutputBoundaryStub =
            std::get<outputstub>(dia.OutputBoundaryStubs[K]);
        if (OtherOutputBoundaryStub.position.row <
            OutputBoundaryStub.position.row) {
          StubsAbove++;
        }
      }
    }
    OutputBoundaryStub.length = 3u + StubsAbove;
    dia.OutputBoundaryStubs[I] = OutputBoundaryStub;
  }
  for (uint32_t I = 0u; I < NumControlStubs; I++) {
    controlstub ControlBoundaryStub;
    uint32_t StubsLeftwards;

    StubsLeftwards = 0u;
    ControlBoundaryStub = std::get<controlstub>(dia.ControlBoundaryStubs[I]);
    for (uint32_t K = 0u; K < NumControlStubs; K++) {
      if (I == K) {
        continue;
      } else {
        controlstub OtherControlBoundaryStub;

        OtherControlBoundaryStub =
            std::get<controlstub>(dia.ControlBoundaryStubs[K]);
        if (OtherControlBoundaryStub.position.column <
            ControlBoundaryStub.position.column) {
          StubsLeftwards++;
        }
      }
    }
    ControlBoundaryStub.length = 3u + StubsLeftwards;
    dia.ControlBoundaryStubs[I] = ControlBoundaryStub;
  }
  for (uint32_t I = 0u; I < NumMechanismStubs; I++) {
    mechanismstub MechanismBoundaryStub;
    uint32_t StubsLeftwards;

    StubsLeftwards = 0u;
    MechanismBoundaryStub =
        std::get<mechanismstub>(dia.MechanismBoundaryStubs[I]);
    for (uint32_t K = 0u; K < NumMechanismStubs; K++) {
      if (I == K) {
        continue;
      } else {
        mechanismstub OtherMechanismBoundaryStub;

        OtherMechanismBoundaryStub =
            std::get<mechanismstub>(dia.MechanismBoundaryStubs[K]);
        if (OtherMechanismBoundaryStub.position.column <
            MechanismBoundaryStub.position.column) {
          StubsLeftwards++;
        }
      }
    }
    MechanismBoundaryStub.length = 3u + StubsLeftwards;
    dia.MechanismBoundaryStubs[I] = MechanismBoundaryStub;
  }
}

void layactivitydia(activitydia &dia, uint32_t dwidth, uint32_t dheight,
                    uint32_t bwidth, uint32_t bheight, uint32_t xgap,
                    uint32_t ygap) {
  dia.width = dwidth;
  dia.height = dheight;
  layframe(dia);
  layboxes(dia, bwidth, bheight, xgap, ygap);
  layboxstubs(dia);
  laybstubs(dia, bwidth, bheight, xgap, ygap);
  cbstublens(dia);
  ShiftBoundaryStubs(dia);
}
}
