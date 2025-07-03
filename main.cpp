#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

float camX = 0.0f;
float camY = 10.0f;
float camZ = 25.0f;

float camDirX = 0.0f;
float camDirY = 0.0f;
float camDirZ = -1.0f;

float rightX = -camDirZ;
float rightZ = camDirX;


struct Sphere{
    float radius, distance, orbit_vel, rotation_vel;
    GLuint texture;
};

GLuint loadTexture(const char* path){
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if(data){
        GLenum model = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, model, width, height, 0, model, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cerr <<"Erro de textura"<< path<< std::endl;
        return 0;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void drawOrbit(float radius){
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 360; i += 2){
        float angle = i * M_PI / 180.0f;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void drawSphere(Sphere s, float time){
    glPushMatrix();
        float orbit_angle = time * s.orbit_vel;
        glRotatef(orbit_angle, 0, 1, 0);
        glTranslatef(s.distance, 0, 0);

        float rotation_angle = time * s.rotation_vel;
        glRotatef(rotation_angle, 0, 1, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, s.texture);
        GLUquadric* sphere = gluNewQuadric();
        gluQuadricTexture(sphere, GL_TRUE);
        gluSphere(sphere, s.radius, 50, 50);
        gluDeleteQuadric(sphere);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void keyboardInput(GLFWwindow* window){
    float speed = 3.0f;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camX += camDirX * speed;
        camY += camDirY * speed;
        camZ += camDirZ * speed;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camX -= camDirX * speed;
        camY -= camDirY * speed;
        camZ -= camDirZ * speed;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camX += rightX * speed;
        camZ += rightZ * speed;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camX -= rightX * speed;
        camZ -= rightZ * speed;
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camY += speed;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camY -= speed;
    }
}

int main(){
    int argc = 1;
    char* argv[1] = {(char*)"App"};

    if(!glfwInit()){
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Sistema Solar", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_DEPTH_TEST);

    Sphere sun = {2.0f, 0.0f, 0.0f, 0.0f};
    Sphere mercury = {0.2f, 4.0f, 47.4f, 10.0f};
    Sphere venus = {0.35f, 6.5f, 35.0f, -10.0f};
    Sphere earth = {0.5f, 9.0f, 30.0f, 90.0f};
    Sphere mars = {0.4f, 11.0f, 24.0f, 50.0f};
    Sphere jupiter = {0.9f, 15.0f, 13.0f, 200.0f};
    Sphere saturn = {0.8f, 18.0f, 9.7f, 150.0f};
    Sphere uranus = {0.7f, 21.0f, 6.8f, 100.0f};
    Sphere neptune = {0.7f, 24.0f, 5.4f, 95.0f};

    sun.texture = loadTexture("images/sunmap.jpg");
    mercury.texture = loadTexture("images/mercurymap.jpg");
    venus.texture = loadTexture("images/venusmap.jpg");
    earth.texture = loadTexture("images/earthmap.jpg");
    mars.texture = loadTexture("images/marsmap.jpg");
    jupiter.texture = loadTexture("images/jupitermap.jpg");
    saturn.texture = loadTexture("images/saturnmap.jpg");
    uranus.texture = loadTexture("images/uranusmap.jpg");
    neptune.texture = loadTexture("images/neptunemap.jpg");

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 800.0/600.0, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camX, camY, camZ, camX + camDirX, camY + camDirY, camZ + camDirZ, 0.0f, 1.0f, 0.0f);

        keyboardInput(window);

        float time = glfwGetTime();

        drawSphere(sun, time);

        drawOrbit(mercury.distance);
        drawSphere(mercury, time);

        drawOrbit(venus.distance);
        drawSphere(venus, time);

        drawOrbit(earth.distance);
        drawSphere(earth, time);

        drawOrbit(mars.distance);
        drawSphere(mars, time);

        drawOrbit(jupiter.distance);
        drawSphere(jupiter, time);

        drawOrbit(saturn.distance);
        drawSphere(saturn, time);

        drawOrbit(uranus.distance);
        drawSphere(uranus, time);

        drawOrbit(neptune.distance);
        drawSphere(neptune, time);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
