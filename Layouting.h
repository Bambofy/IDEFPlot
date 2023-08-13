#ifndef LAYOUTING_H
#define LAYOUTING_H

namespace idef {

void layframe(activitydia& dia);

/*
 * Layout boxes.
 *
 * Parameters:
 *   1. Diagram (dia).
 *   2. Box width (bwidth).
 *   3. Box height (bheight).
 *   4. Box x gap (xgap).
 *   5. Box y gap (ygap).
 *
 * Local Variables:
 *   1. Number of boxes (nboxes).
 *   2. Cursor (curs).
 *   3. Column center offset (ccenteroff).
 *   4. Row center offset (rcenteroff).
 *   5. Row height (rheight).
 *   6. Column width (cwidth).
 *   7. Box section height (bsheight).
 *   8. Box section width (bswidth).
 *   9. Box index (bindx).
 *  10. Selected box (sbox).
 */
void layboxes(activitydia& dia, uint32_t bwidth, uint32_t bheight, uint32_t xgap,
 uint32_t ygap);

/*
 * Layout box stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 *
 * Local Variables:
 *   1. Number of boxes (nboxes).
 *   2. Box index (bindx).
 *   3. Number of input stubs (nistubs).
 *   4. Number of output stubs (nostubs).
 *   5. Number of control stubs (nconstubs).
 *   6. Number of mechanism stubs (nmechstubs).
 *   7. Number of call stubs (ncallstubs).
 *   8. Number of input interface divisions (numidivs).
 *   9. Input interface division width (idivwidth).
 *  10. Number of output interface divisions (numodivs).
 *  11. Output interface division width (odivwidth).
 *  12. Number of control interface divisions (numcondivs).
 *  13. Control interface division width (conidivwidth).
 *  14. Number of mechanism interface divisions (nummechdivs).
 *  15. Mechanism interface division width (mechdivwidth).
 *  16. Number of call interface division (numcalldivs).
 *  17. Call interface division width (callidivwidth).
 *  18. Selected box (sbox).
 *  19. Input stub index (istubi).
 *  20. Row offset (rowo).
 *  21. Selected input stub (selistub).
 *  22. Output stub index (ostubi).
 *  23. Selected output stub (selostub).
 *  24. Control stub index (constubi).
 *  25. Selected control stub (selconstub).
 *  26. Column offset (columnoff).
 *  27. Mechanism stub index (mechstubi).
 *  28. Selected mechanism stub (selmechstub).
 *  29. Call stub index (callstubi).
 */
void layboxstubs(activitydia& dia);

/*
 * Find inner stub.
 *
 * Parameters:
 *   1. Diagram (dia).
 *   2. Boundary stub (bstub).
 *   3. Found stub (fstub).
 *   4. Found flag (fflag).
 */
void finnerstub(const activitydia& dia, const stub& bstub, stub& fstub, 
	bool& fflag);

/*
 * Locate connected boxes.
 *
 * Parameters:
 *   1. Activity diagram (dia).
 *   2. Target stub name (tsname).
 */
std::vector<activitybox> lconnboxes(activitydia& dia, std::string& tsname);

/*
 * Check stubs overlap.
 *
 * Parameters:
 *   1. First stub (fstub).
 *   2. Second stub (stub).
 */
bool checksoverlap(stub fstub, stub sstub);

/*
 * Shift input stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 */
void sinstubs(activitydia& dia);

/*
 * Shift output stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 */
void soutstubs(activitydia& dia);

/*
 * Shift control stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 */
void sconstubs(activitydia& dia);

/*
 * Shift mechanism stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 */
void smechstubs(activitydia& dia);

/*
 * Shift boundary stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 */
void shiftbstubs(activitydia& dia);

/*
 * Change boundary stub lengths.
 *
 * Parameters:
 *   1. Diagram (dia). 
 */
void cbstublens(ActivityDiagram& dia);

/*
 * Layout boundary stubs.
 *
 * Parameters:
 *   1. Diagram (dia).
 *   2. Box width (bwidth).
 *   3. Box height (bheight).
 *   4. Box x gap (xgap).
 *   5. Box y gap (ygap).
 */
void laybstubs(activitydia& dia, uint32_t bwidth, uint32_t bheight, uint32_t xgap,
 uint32_t ygap);

/*
 * Layout activity diagram.
 *
 * Parameters:
 *   1. Diagram (dia).
 *   2. Diagram width (dwidth).
 *   3. Diagram height (dheight).
 *   4. Box width (bwidth).
 *   5. Box height (bheight).
 *   6. Box x gap (xgap).
 *   7. Box y gap (ygap).
 */
void layactivitydia(activitydia& dia, uint32_t dwidth, uint32_t dheight, 
	uint32_t bwidth, uint32_t bheight, uint32_t xgap, uint32_t ygap); 
}

#endif