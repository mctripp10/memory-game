/* Michael Tripp
 * CS441 | Project 1
 * 2/22/2022
 *
 * A popular children’s game called the Memory Game in which a set of 16 cards containing pairs of
 * matching objects are shuffled and dealt in a 4 ×4 grid, face down. In this solitaire version, a player
 * can turn over any two cards and if they match, can remove them from the board. Otherwise, they
 * are turned over and the player gets to turn over two more cards. The object is to remove all of
 * the cards in as least turns as possible.
 */

#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;

// The many global variables used throughout the program
const int width = 520;
const int height = 600;
int turns;
int cardVals [16] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8};   // Values 1-8 assigned to each card. Two cards with the same value are a match
bool cardMatched [16];
int firstCardVal;
int firstCardIndex;
int secondCardVal;
int secondCardIndex;
bool canPress;
bool flipFirstCardFront;
bool flipFirstCardBack;
bool flipSecondCardFront;
bool flipSecondCardBack;
bool flipBothCards;
bool matchFound;

string *s;      // String to be displayed on the screen

// Variables used to define the size, position, and color of the rectangles on the screen
GLfloat xpos[16] = {30, 30, 30, 30, 150, 150, 150, 150, 270, 270, 270, 270, 390, 390, 390, 390},
        ypos[16] = {150, 290, 430, 570, 150, 290, 430, 570, 150, 290, 430, 570, 150, 290, 430, 570};
GLfloat rectsSizeX[16];
GLfloat rectsSizeY[16];
GLfloat rectSizeX = 100;
GLfloat rectSizeY = 120;
GLfloat red[16], blue[16], green[16];

// Method to print a given string to the screen at a given location
int printString(int rx, int ry, void* font, string s)
{
    glColor3f(0.8, 0.0, 0.0);
    glRasterPos2i(rx,ry);
    for(int i=0; i<s.size(); i++) {
        glutBitmapCharacter(font, s[i]);
        rx += glutBitmapWidth(font, s[i]);
    }
    return rx;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw all rectangles with corresponding colors, positions, and sizes
    for (int i = 0; i < 16; i++) {
        glColor3f(red[i], green[i], blue[i]);
        glRectf(xpos[i], ypos[i], xpos[i]+rectsSizeX[i], ypos[i]-rectsSizeY[i]);
    }
    // Print turns to screen
    s = new string("Turns: " + std::to_string(turns));
	printString(20, 10, GLUT_BITMAP_9_BY_15, *s);
	glutSwapBuffers();
}

// Method to initialize all global variables
void initializeBoard () {
    // Initialize all arrays
    for(int i = 0; i < 16; i++) {
        red[i] = 0.8;
        green[i] = 0.0;
        blue[i] = 0.0;
        cardMatched[i] = false;
        rectsSizeX[i] = 100;
        rectsSizeY[i] = 120;
	}
	std::random_shuffle(cardVals, cardVals + 16);  // Randomize card pairings

	// Integer variables used to keep track of card values, indexes, and # of turns
	turns = 0;
	firstCardVal = 0;
    firstCardIndex = -1;
    secondCardVal = 0;
    secondCardIndex = -1;

    // Booleans used to control animation starts and stops
    canPress = true;
    flipFirstCardFront = false;
    flipFirstCardBack = false;
    flipSecondCardFront = false;
    flipSecondCardBack = false;
    flipBothCards = false;
    matchFound = false;
}

// Defines what to do depending on selected menu entry
void menu (int id) {
    switch (id) {
    case 0:         // Restart
        initializeBoard();
        glutPostRedisplay();
        break;
    case 1:        // Quit
        exit(0);
    default:
        break;
    }
}

// Function to create the menu attached to the right click button with two menu entries, Restart & Quit, calling our above menu function
void createMenu () {
    glutCreateMenu(menu);
    glutAddMenuEntry("Restart", 0);
    glutAddMenuEntry("Quit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void myinit()
{
    glClearColor (1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	createMenu();
	initializeBoard();
}

// Sets the RGB color of the corresponding rectangle based on given cardVal and cardIndex, with each cardVal associated with a specific color
void setColor(int cardVal, int cardIndex) {
    // Note: I am keeping the colors similar on purpose to increase the difficulty :^)
    switch (cardVal) {
    case 0:         // Match found, color rectangle white
        red[cardIndex] = 1.0;
        green[cardIndex] = 1.0;
        blue[cardIndex] = 1.0;
        break;
    case 1:
        red[cardIndex] = 0.0;
        green[cardIndex] = 0.0;
        blue[cardIndex] = 1.0;
        break;
    case 2:
        red[cardIndex] = 0.5;
        green[cardIndex] = 0.0;
        blue[cardIndex] = 0.5;
        break;
    case 3:
        red[cardIndex] = 1.0;
        green[cardIndex] = 0.0;
        blue[cardIndex] = 0.0;
        break;
    case 4:
        red[cardIndex] = 0.5;
        green[cardIndex] = 0.5;
        blue[cardIndex] = 0.0;
        break;
    case 5:
        red[cardIndex] = 0.0;
        green[cardIndex] = 1.0;
        blue[cardIndex] = 0.0;
        break;
    case 6:
        red[cardIndex] = 0.0;
        green[cardIndex] = 0.5;
        blue[cardIndex] = 0.5;
        break;
    case 7:
        red[cardIndex] = 0.2;
        green[cardIndex] = 0.8;
        blue[cardIndex] = 0.2;
        break;
    case 8:
        red[cardIndex] = 0.2;
        green[cardIndex] = 0.2;
        blue[cardIndex] = 0.2;
        break;
    default:        // Reset card back to default color, red
        red[cardIndex] = 0.8;
        green[cardIndex] = 0.0;
        blue[cardIndex] = 0.0;
    }
}

// Glut timer function that runs after the second card is pressed
void glutTimer (int val) {
    if (matchFound) {       // Color cards white if match found
        setColor(0, firstCardIndex);
        setColor(0, secondCardIndex);

        // Reset values for next two cards to be pressed
        firstCardVal = 0;
        firstCardIndex = -1;
        matchFound = false;
        canPress = true;
    } else {                // Flip first and second cards "back over" if match not found
        firstCardVal = secondCardVal = -1;
        flipFirstCardFront = flipSecondCardFront = true;
    }
    turns++;        // Increment turn count
    glutPostRedisplay();
}

// Idle function that controls the animation of the cards
void idle (void) {
    float i = 5;      // Flip speed

    /*
     * Flip first card up
     */

    // Flip first card downward halfway
    if (flipFirstCardFront && rectsSizeX[firstCardIndex] >= 0) {
        xpos[firstCardIndex] += i;
        rectsSizeX[firstCardIndex] -= i*2;
    // First card is now halfway flipped and front is no longer visible. Set up for flipping first card backside upward
    } else if (flipFirstCardFront && rectsSizeX[firstCardIndex] < 0) {
        flipFirstCardFront = false;
        flipFirstCardBack = true;
        setColor(firstCardVal, firstCardIndex);  // Change color to backside color
    }

    // Flip first card the rest of the way, revealing the backside of the card
    if (flipFirstCardBack && rectsSizeX[firstCardIndex] < rectSizeX) {
        xpos[firstCardIndex] -= i;
        rectsSizeX[firstCardIndex] += i*2;
    // First card backside is now fully flipped upward. End animation
    } else if (flipFirstCardBack && rectsSizeX[firstCardIndex] >= rectSizeX)
        flipFirstCardBack = false;

    /*
     * Flip second card up
     */

    // Flip second card downward halfway
    if (flipSecondCardFront && rectsSizeX[secondCardIndex] >= 0) {
        xpos[secondCardIndex] += i;
        rectsSizeX[secondCardIndex] -= i*2;
    // Second card is now halfway flipped and front is no longer visible. Set up for flipping second card backside upward
    } else if (flipSecondCardFront && rectsSizeX[secondCardIndex] < 0) {
        flipSecondCardFront = false;
        flipSecondCardBack = true;
        setColor(secondCardVal, secondCardIndex);       // Change color to backside color
    }

    // Flip second card the rest of the way, revealing the backside of the card
    if (flipSecondCardBack && rectsSizeX[secondCardIndex] < rectSizeX) {
        xpos[secondCardIndex] -= i;
        rectsSizeX[secondCardIndex] += i*2;
    // Second card backside is now fully flipped upward. End animation
    } else if (flipSecondCardBack && rectsSizeX[secondCardIndex] >= rectSizeX) {
        flipSecondCardBack = false;
        if (flipBothCards) {        // Match not found: glutTimerFunc is called to flip both cards back over in 1 second
            flipBothCards = false;
            glutTimerFunc(1000, glutTimer, -1);
        } else {                    // Match found: simply reset values for next two cards to be pressed
            firstCardVal = 0;
            firstCardIndex = -1;
            canPress = true;
        }
    }
    glutPostRedisplay();
}

// Mouse function which detects which card is pressed on left click and initiates card flip animations accordingly
void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        y = height - y;

        // Redraw rectangles in back buffer with red RGB values of 0-15 for picking
        for (int i = 0; i < 16; i++) {
            glColor3ub(i, 0, 0);
            glRectf(xpos[i], ypos[i], xpos[i]+rectsSizeX[i], ypos[i]-rectsSizeY[i]);
        }

        // Read pixels in from mouse click and identify the index of the corresponding card pressed
        GLubyte colorVal[1];
        glReadPixels(x, y, 1, 1, GL_RED, GL_UNSIGNED_BYTE, colorVal);
        int index = colorVal[0];

        // Do nothing if improper index (index > 15), card is already matched (cardMatched[index]), card is already selected (index == firstCardIndex), or card animation has not yet finished (!canPress)
        if (index > 15 || cardMatched[index] || index == firstCardIndex || !canPress){
            return;
        }

        // Execute following code when left mouse button is pressed
        if (button == GLUT_LEFT_BUTTON) {
            if (firstCardVal == 0) {        // First card is pressed
                firstCardVal = cardVals[index];
                firstCardIndex = index;
                flipFirstCardFront = true;      // Begin flip animation for first card
            } else {                        // Second card is pressed
                canPress = false;               // Prevent user from pressing any other cards until both cards are flipped back over
                secondCardVal = cardVals[index];
                secondCardIndex = index;
                if (firstCardVal == secondCardVal) {    // If both card values are the same, they are a match!
                    matchFound = true;
                    flipBothCards = true;                   // Setup idle animation to flip both cards back over after second card is flipped up
                    flipSecondCardFront = true;             // Flip second card up
                    cardMatched[firstCardIndex] = true;     // Record that both cards have been matched
                    cardMatched[secondCardIndex] = true;
                } else {                                // If both card values are not the same, we don't have a match :(
                    flipBothCards = true;                   // Setup idle animation to flip both cards back over after second card is flipped up
                    flipSecondCardFront = true;             // Flip second card up
                }
            }
            glutPostRedisplay();
        }
    }
}

// Reshape function that allows the view port to remain proportionate and centered with the window when the window is reshaped
void reshape(int w, int h)
{
    float ar = ((float) width)/height;
    if (ar > ((float) w)/h)
        glViewport(0, (h-(w/ar))/2, w, w/ar);
    else
        glViewport((w-(h*ar))/2, 0, h*ar, h);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Memory Game");
    myinit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMainLoop();
}
