/* Allen Wang
 * CS 106B
 * Assignment 3 Problem 1
 */
#include "Sierpinski.h"
#include "error.h"
using namespace std;

/**
 * Draws a triangle with the specified corners in the specified window. Feel free
 * to edit this function as you see fit to change things like color, fill style,
 * etc. in whatever way you'd like!
 *
 * @param window The window in which to draw the triangle.
 * @param x0 y0 The first corner of the triangle.
 * @param x1 y1 The second corner of the triangle.
 * @param x2 y2 The third corner of the triangle.
 */
void drawTriangle(GWindow& window,
                  double x0, double y0,
                  double x1, double y1,
                  double x2, double y2) {
    window.setColor("black");
    window.fillPolygon({ x0, y0, x1, y1, x2, y2 });
}

// Recursively draws triangles of half the side length as the original while skipping the potential middle triangle.
void drawSierpinskiTriangle(GWindow& window,
                            double x0, double y0,
                            double x1, double y1,
                            double x2, double y2,
                            int order) {
    if (order < 0) {
        error("ERROR: NEGATIVE ORDER");
    }
    else if (order == 0) {
        drawTriangle(window, x0, y0, x1, y1, x2, y2);
    }
    else {
        double xMidpoint0 = (x0 + x1) / 2;
        double xMidpoint1 = (x1 + x2) / 2;
        double xMidpoint2 = (x0 + x2) / 2;
        double yMidpoint0 = (y0 + y1) / 2;
        double yMidpoint1 = (y1 + y2) / 2;
        double yMidpoint2 = (y0 + y2) / 2;
        // Recursively draws the triangle at the bottom right.
        drawSierpinskiTriangle(window,
                               x2, y2,
                               xMidpoint1, yMidpoint1,
                               xMidpoint2, yMidpoint2,
                               order - 1);
        // Recursively draws the triangle at the top.
        drawSierpinskiTriangle(window,
                               xMidpoint0, yMidpoint0,
                               x0, y0,
                               xMidpoint2, yMidpoint2,
                               order - 1);
        // Recursivley draws the triangle at the bottom left.
        drawSierpinskiTriangle(window,
                               xMidpoint0, yMidpoint0,
                               xMidpoint1, yMidpoint1,
                               x1, y1,
                               order - 1);
    }
}
