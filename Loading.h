#ifndef LOADING_H
#define LOADING_H

namespace idef {

/*
 * File position
 */
struct filepos {
  /*
   * Row
   */
  uint32_t row;

  /*
   * Column
   */
  uint32_t column;

  /*
   * Equals operator
   * 
   * Local variables:
   *   1. This encoded position (tencpos).
   *   2. RHS encoded position (rencpos).
   */
  bool operator==(const filepos& rhs) const {
    return (row == rhs.row) && (column == rhs.column);
  }
};

/*
 * Stub source
 */
struct stubsource {
    /*
     * Stub name
     */
    std::string stubname;
};

/*
 * Input stub
 */
struct inputstub {
  /*
   * Name
   */
  std::string name;

  /*
   * Position
   */
  filepos position;

  /*
   * Sources
   */
  std::vector<stubsource> sources;

  /*
   * Length
   */
  uint32_t length;
    
  /*
   * Less-than operator.
   * 
   * Local variables:
   *   1. This encoded position (tencpos).
   *   2. RHS encoded position (rencpos).
   */
  bool operator<(const inputstub& rhs) const {
    uint32_t tencpos;
    uint32_t rencpos;

    tencpos = position.column + (position.row * 1000u);
    rencpos = rhs.position.column + (rhs.position.row * 1000u);

    return tencpos < rencpos;
  }
};

/*
 * Output stub
 */
struct outputstub {
  /*
   * Name
   */
  std::string name;
  
  /*
   * Position
   */
  filepos position;
  
  /*
   * Sources
   */
  std::vector<stubsource> sources;

  /*
   * Length
   */
  uint32_t length;

  /*
   * Less-than operator.
   * 
   * Local variables:
   *   1. This encoded position (tencpos).
   *   2. RHS encoded position (rencpos).
   */
  bool operator<(const outputstub& rhs) const {
      uint32_t tencpos;
      uint32_t rencpos;

      tencpos = position.column + (position.row * 1000u);
      rencpos = rhs.position.column + (rhs.position.row * 1000u);

      return tencpos < rencpos;
  }
};

/*
 * Control stub
 */
struct controlstub {
  /*
   * Name
   */
  std::string name;

  /*
   * Position
   */
  filepos position;

  /*
   * Sources
   */
  std::vector<stubsource> sources;

  /*
   * Length
   */
  uint32_t length;

  /*
   * Less-than operator.
   * 
   * Local variables:
   *   1. This encoded position (tencpos).
   *   2. RHS encoded position (rencpos).
   */
  bool operator<(const controlstub& rhs) const {
    uint32_t tencpos;
    uint32_t rencpos;

    tencpos = position.column + (position.row * 1000u);
    rencpos = rhs.position.column + (rhs.position.row * 1000u);

    return tencpos < rencpos;
  }
};

/*
 * Mechanism stub
 */
struct mechanismstub {
  /*
   * Name
   */
  std::string name;
  
  /*
   * Position
   */
  filepos position;
  
  /*
   * Sources
   */
  std::vector<stubsource> sources;

  /*
   * Length
   */
  uint32_t length;

  /*
   * Less-than operator.
   * 
   * Local variables:
   *   1. This encoded position (tencpos).
   *   2. RHS encoded position (rencpos).
   */
  bool operator<(const mechanismstub& rhs) const {
    uint32_t tencpos;
    uint32_t rencpos;

    tencpos = position.column + (position.row * 1000u);
    rencpos = rhs.position.column + (rhs.position.row * 1000u);

    return tencpos < rencpos;
  }
};

/*
 * Call stub
 */
struct callstub {
  /*
   * Name
   */
  std::string name;

  /*
   * Position
   */
  filepos position;

  /*
   * Sources
   */
  std::vector<stubsource> sources;

  /*
   * Length
   */
  uint32_t length;

  /*
   * Less-than operator.
   *
   * 1. This encoded position (tencpos).
   * 2. Right-hand-side encoded positions (rencpos).
   */
  bool operator<(const callstub& rhs) const {
    uint32_t tencpos;
    uint32_t rencpos;

    tencpos = position.column + (position.row * 1000u);
    rencpos = rhs.position.column + (rhs.position.row * 1000u);

    return tencpos < rencpos;
  }
};

/*
 * Stub
 */
typedef std::variant<inputstub, 
  outputstub, controlstub, mechanismstub, callstub> stub;

/*
 * Activity box.
 */
struct activitybox {
  /*
   * Input stubs.
   */
  std::vector<stub> inputstubs;
  
  /*
   * Output stubs.
   */
  std::vector<stub> outputstubs;
  
  /*
   * Control stubs.
   */
  std::vector<stub> controlstubs;
  
  /*
   * Mechanism stubs.
   */
  std::vector<stub> mechanismstubs;
  
  /*
   * Call stubs.
   */
  std::vector<stub> callstubs;
  
  /*
   * Name.
   */
  std::string name;
  
  /*
   * Center.
   */
  filepos center;
  
  /*
   * Width.
   */
  uint32_t width;

  /*
   * Height.
   */
  uint32_t height;
  
  /*
   * Padding.
   */
  uint32_t padding;
  
  /*
   * Node number.
   */
  std::string nodenumber;
  
  /*
   * DRE.
   */
  std::string dre;
};

/*
 * Node number section.
 */
struct nnumbersection {
  /*
   * Top left.
   */
  filepos topleft;
  
  /*
   * Content.
   */
  std::string content;
  
  /*
   * Width.
   */
  uint32_t width;
  
  /*
   * Height.
   */
  uint32_t height;
};

/*
 * Title section.
 */
struct titlesection {
  /*
   * Top left.
   */
  filepos topleft;

  /*
   * Content.
   */
  std::string content;
  
  /*
   * Width.
   */ 
  uint32_t width;

  /*
   * Height.
   */  
  uint32_t height;
};

/*
 * CNumber section.
 */
struct cnumsection {
  /*
   * Top left.
   */
  filepos topleft;

  /*
   * Content.
   */
  std::string content;
  
  /*
   * Width.
   */ 
  uint32_t width;

  /*
   * Height.
   */  
  uint32_t height;
};

/*
 * Diagram section.
 */
typedef std::variant<nnumbersection, 
    titlesection, cnumsection> diasection;

/*
 * Diagram bar.
 */
struct diagrambar {
  /*
   * Top left.
   */
  filepos topleft;

  /*
   * Height.
   */
  uint32_t height;

  /*
   * Node number section.
   */
  diasection nnumbersection;

  /*
   * Title section.
   */
  diasection titlesection;

  /*
   * CNumber Section.
   */
  diasection cnumsection;
};

/*
 * Diagram Frame.
 */
struct diagramframe {
  /*
   * Bottom bar.
   */
  diagrambar bottombar;
};

/*
 * Activity diagram.
 *
 * 1. Input boundary stubs (inbstubs).
 * 2. Output boundary stubs (outbstubs).
 * 3. Control boundary stubs (conbstubs).
 * 4. Mechanism boundary stubs (mecbstubs).
 */
struct activitydia {
  /*
   * Title.
   */
  std::string title;

  /*
   * Width.
   */
  uint32_t width;

  /*
   * Height.
   */
  uint32_t height;
 
  /*
   * Boxes.
   */
  std::vector<activitybox> boxes;
 
  /*
   * Input boundary stubs.
   */
  std::vector<stub> inbstubs;

  /*
   * Output boundary stubs.
   */
  std::vector<stub> outbstubs;

  /*
   * Control boundary stubs.
   */
  std::vector<stub> conbstubs;
 
  /*
   * Mechanism boundary stubs.
   */
  std::vector<stub> mecbstubs;
  
  /*
   * Frame
   */
  diagramframe frame;
};

/*
 * Model
 */
struct model {
  /*
   * Title.
   */
  std::string title;

  /*
   * Activity diagrams.
   */
  std::vector<activitydia> activitydias;
};

/*
 * Interface
 */
enum interface {
  /*
   * Input interface.
   */
  ininterface,

  /*
   * Output interface.
   */
  outinterface,
  
  /*
   * Control interface.
   */
  continterface,
  
  /*
   * Mechanism interface.
   */
  mechinterface,
  
  /*
   * Call interface.
   */
  callinterface
};

/*
 * Load input stub
 *
 * Parameters:
 *   1. Input stub xml node (instubnode).
 *
 * Local variables:
 *   1. New stub (nstub).
 *   2. Stub source node (ssourcenode).
 *   3. New stub source (nssource).
 */
inputstub loadinstub(const pugi::xml_node& instubnode);

/*
 * Load output stub
 *
 * Parameters:
 *   1. Output stub xml node (outstubnode).
 *
 * Local variables:
 *   1. New stub (nstub).
 *   2. Stub source node (ssourcenode).
 *   3. New stub source (nssource).
 */
outputstub loadoutstub(const pugi::xml_node& outstubnode);

/*
 * Load control stub
 *
 * Parameters:
 *   1. Output stub xml node (constubnode).
 *
 * Local variables:
 *   1. New stub (nstub).
 *   2. Stub source node (ssourcenode).
 *   3. New stub source (nssource). 
 */
controlstub loadconstub(const pugi::xml_node& constubnode);

/*
 * Load mechanism stub
 *
 * Parameters:
 *   1. Mechanism stub xml node (mechstubnode).
 *
 * Local variables:
 *   1. New stub (nstub).
 *   2. Stub source node (ssourcenode).
 *   3. New stub source (nssource). 
 */
mechanismstub loadmechstub(const pugi::xml_node& mechstubnode);

/*
 * Load call stub
 *
 * Parameters:
 *   1. Call stub xml node (cstubnode).
 *
 * Local variables:
 *   1. New stub (nstub).
 */
callstub loadcallstub(const pugi::xml_node& cstubnode);

/*
 * Load activity diagram
 *
 * Parameters:
 *   1. File path (fpath).
 *
 * Local variables:
 *   1. Diagram XML document (diadoc).
 *   2. Parse result (pres).
 *   3. Activity diagram node (actdnode).
 *   4. New diagram (newdia).
 */
activitydia loadactivitydia(const pugi::xml_node& activitynode);

}

#endif
