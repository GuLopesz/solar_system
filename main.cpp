#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>

struct Sphere{
    float radius, distance, orbit_vel, rotation_vel;
};

Sphere sun = {2.0f, 0.0f, 0.0f, 0.0f};
Sphere mercury = {0.2f, 4.0f, 47.4f, 10.0f};
Sphere venus = {0.35f, 6.5f, 35.0f, -10.0f};
Sphere earth = {0.5f, 9.0f, 30.0f, 90.0f};
Sphere mars = {0.4f, 11.0f, 24.0f, 50.0f};
Sphere jupiter = {0.9f, 15.0f, 13.0f, 200.0f};
Sphere saturn = {0.8f, 18.0f, 9.7f, 150.0f};
Sphere uranus = {0.7f, 21.0f, 6.8f, 100.0f};
Sphere neptune = {0.7f, 24.0f, 5.4f, 95.0f};

void drawSphere(Sphere s, float time){
    glPushMatrix();
        float orbit_angle = time * s.orbit_vel;
        glRotatef(orbit_angle, 0, 1, 0);
        glTranslatef(s.distance, 0, 0);

        float rotation_angle = time * s.rotation_vel;
        glRotatef(rotation_angle, 0, 1, 0);
        glutSolidSphere(s.radius, 50, 50);
    glPopMatrix();
}

int main(){
    int argc = 1;
    char* argv[1] = {(char*)"App"};
    glutInit(&argc, argv);

    if(!glfwInit()){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Sistema Solar", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 800.0/600.0, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 10, 25, 0, 0, 0, 0, 1, 0);

        float time = glfwGetTime();

        drawSphere(sun, time);
        drawSphere(mercury, time);
        drawSphere(venus, time);
        drawSphere(earth, time);
        drawSphere(mars, time);
        drawSphere(jupiter, time);
        drawSphere(saturn, time);
        drawSphere(uranus, time);
        drawSphere(neptune, time);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
