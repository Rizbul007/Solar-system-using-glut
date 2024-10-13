#include <GL/glut.h>
#include <cmath>

// Constants for solar system
const float SUN_RADIUS = 0.2f;
const float PLANET_RADIUS = 0.05f;
const int NUM_PLANETS = 6; // Number of planets
const float ORBIT_RADIUS[] = {0.4f, 0.6f, 0.8f, 1.0f, 1.2f, 1.4f}; // Radii of orbits for 6 planets
const float ORBIT_SPEED[] = {0.01f, 0.007f, 0.005f, 0.004f, 0.003f, 0.002f}; // Orbital speeds for 6 planets
float planetAngles[NUM_PLANETS] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // Angles of planets

// Colors for planets
const float PLANET_COLORS[NUM_PLANETS][3] = {
    {1.0f, 0.0f, 0.0f}, // Red
    {0.0f, 1.0f, 0.0f}, // Green
    {0.0f, 0.0f, 1.0f}, // Blue
    {1.0f, 1.0f, 0.0f}, // Yellow
    {0.5f, 0.5f, 1.0f}, // Light Blue
    {0.5f, 0.2f, 0.8f}  // Purple
};

// Variables for solar system rotation
float systemRotationAngle = 0.0f; // Angle to rotate the entire solar system
float systemRotationSpeed = 0.1f; // Speed of solar system rotation

// Function to draw a sphere
void drawSphere(float radius) {
    glutWireSphere(radius, 20, 20);
}

// Function to draw orbit line
void drawOrbit(float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float angle = i * (M_PI / 180.0f); // Convert degrees to radians
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set the camera view, zooming out by increasing the z-value
    gluLookAt(0.0f, 0.0f, 5.0f,  // Camera position (Zoom out by setting z = 5.0)
              0.0f, 0.0f, 0.0f,  // Look at the center (origin)
              0.0f, 1.0f, 0.0f); // Up vector (positive Y-axis)

    // Apply rotation to the entire solar system
    glRotatef(systemRotationAngle, 1.0f, 0.0f, 0.0f); // Rotate around X-axis (top to bottom)

    // Draw the Sun
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the Sun
    drawSphere(SUN_RADIUS);

    // Draw the planets and their orbits
    for (int i = 0; i < NUM_PLANETS; i++) {
        // Draw the orbit line
        glColor3f(1.0f, 1.0f, 1.0f); // White color for orbit
        drawOrbit(ORBIT_RADIUS[i]);

        // Calculate the position of the planet
        float x = ORBIT_RADIUS[i] * cos(planetAngles[i]);
        float z = ORBIT_RADIUS[i] * sin(planetAngles[i]);

        // Draw the planet
        glPushMatrix();
        glTranslatef(x, 0.0f, z); // Translate to the planet's orbit position
        glColor3f(PLANET_COLORS[i][0], PLANET_COLORS[i][1], PLANET_COLORS[i][2]); // Set color for each planet
        drawSphere(PLANET_RADIUS);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void update(int value) {
    // Update the angles for the planets' orbits
    for (int i = 0; i < NUM_PLANETS; i++) {
        planetAngles[i] += ORBIT_SPEED[i];
        if (planetAngles[i] > 2 * M_PI) {
            planetAngles[i] -= 2 * M_PI;
        }
    }

    // Update the rotation of the solar system (top-to-bottom clockwise)
    systemRotationAngle += systemRotationSpeed;
    if (systemRotationAngle > 360.0f) {
        systemRotationAngle -= 360.0f; // Keep the angle within 0 to 360 degrees
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Approximately 60 FPS
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)800 / (float)600, 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System: Zoomed Out with Top-to-Bottom Clockwise Rotation");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}
