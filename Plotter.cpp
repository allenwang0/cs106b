#include "Plotter.h"
#include "strlib.h"
using namespace std;

void runPlotterScript(istream& input) {
 /*
 Initially, the pen is set at Up and at (0,0). This function cycles through each line of the input.
 The variables prevXVar and prevYVar are used to store the previous position of the pen.
 First, the line is converted to lower case to account for case insensitivity.
 Next, the line is split into command segments. Since there are 3 different line lengths,
 this allows for three if statements corresponding to the number of command segments.
 If the size is 1, the first segment sets the pen into Up or Down positions.
 If the size is 2, the first segment sets which variable is changed, color or width, and the second sets that particular style descriptor.
 If the size is 3, the first segment sets whether the pen is moving absolutely or relatively. If the pen is moving absolutely and
 the pen is down, the pen draws a line from the previous position to the position ([second segment], [third segment]). If the pen
 is moving relatively and the pen is down, the pen draws a line from the previous position to the position
 (prevXVar + [second segment], prevYVar + [third segment]).
 */
    PenStyle style = { 1, "black" };
    bool isPenUp = true;
    double xVar = 0.0;
    double yVar = 0.0;
    for (string line; getline(input, line); ) {
        toLowerCaseInPlace(line);
        Vector<string> commSeg = stringSplit(line, ' ');
        double prevXVar = xVar;
        double prevYVar = yVar;
        if (commSeg.size() == 1) {
            if (startsWith(commSeg[0], "pendown")) {
                isPenUp = false;
            }
            else if (startsWith(commSeg[0], "penup")) {
                isPenUp = true;
            }
        }
        else if (commSeg.size() == 2) {
            if (startsWith(commSeg[0], "pencolor")) {
                style.color = commSeg[1];
            }
            else if (startsWith(commSeg[0], "penwidth")) {
                style.width = stringToReal(commSeg[1]);
            }
        }
        else if (commSeg.size() == 3) {
            if (startsWith(commSeg[0], "moveabs")) {
                xVar = stringToReal(commSeg[1]);
                yVar = stringToReal(commSeg[2]);
                if (isPenUp == false) {
                    drawLine(prevXVar, prevYVar, xVar, yVar, style);
                }
            }
            else if (startsWith(commSeg[0], "moverel")) {
                xVar = prevXVar + stringToReal(commSeg[1]);
                yVar = prevYVar + stringToReal(commSeg[2]);
                if (isPenUp == false) {
                    drawLine(prevXVar, prevYVar, xVar, yVar, style);
                }
            }
        }
    }
}
