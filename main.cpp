//
//  main.cpp
//  Snake
//
//  Created by Kevin on 2017/3/23.
//  Copyright © 2017年 Kevin. All rights reserved.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <vector>
#include <iostream>

#include "SnakePlane.hpp"
#include "Snake.hpp"

const int WIDTH = 400;
const int HEIGTH = 400;
const float PointSize = 40.f;

using namespace std;

shared_ptr<Snake> snake;
Direction dir;

sig_atomic_t run = 1;

vector<Position> SnakePoints(const std::shared_ptr<const Snake> s);
void keycallback(GLFWwindow *window, int key, int scancode, int action, int mods);

int
main(int argc, const char * argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGTH, "SNAKE", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keycallback);
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    int height, width;
    
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    Shader shader("vertex.glsl", "fragment.glsl");
    
    size_t BufferSize = (height / PointSize) * (width / PointSize);
    
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, BufferSize * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLchar *)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    auto sp_w = width/PointSize, sp_h = height/PointSize;
    
    SnakePlane sp(sp_w, sp_h);
    snake = sp.NewSnake();

    vector<GLfloat> vertices;
    
    auto lastTime = glfwGetTime() - 100;
    auto currentTime = lastTime;
    
    while ( run ) {
        
        glfwPollEvents();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        currentTime = glfwGetTime();
        if ( currentTime - lastTime >= 0.2 ) {
            lastTime = currentTime;
            
            snake->Turn(dir);
            snake->Forward();
            if (sp.CollisionDetect(snake))
            {
                std::cout << "end" << endl;
                snake = sp.NewSnake();
            }

            auto p = SnakePoints(snake);
            p.push_back(sp.food);
            
            vertices.clear();
            for (auto i: p) {
                /* + 0.5 to align to edge */
                GLfloat x = ((GLfloat)i.x + 0.5 - sp_w/2) / (sp_w/2);
                GLfloat y = ((GLfloat)i.y + 0.5 - sp_h/2) / (sp_h/2);
                vertices.push_back(x);
                vertices.push_back(y);
            }
        }
        
        /* .95 for space between adjacent points */
        shader.Use();
        
        glUniform3f(glGetUniformLocation(shader.program(), "Color"), 1.f, .0f, .0f);
        glUniform1f(glGetUniformLocation(shader.program(), "PointSize"), PointSize * 0.95);
        
        glBindVertexArray(VAO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (vertices.size() - 2) * sizeof(GLfloat), addressof(vertices[0]));
        glDrawArrays(GL_POINTS, 0, (vertices.size() - 2) / 2);
        
        shader.Use();
        glUniform3f(glGetUniformLocation(shader.program(), "Color"), 1.f, 1.f, 0.f);
        glUniform1f(glGetUniformLocation(shader.program(), "PointSize"), PointSize * 0.8);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(GLfloat), &(*(vertices.end() - 2)));
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    exit(0);
}

vector<Position> 
SnakePoints(const std::shared_ptr<const Snake> s)
{
    vector<Position> points;
    auto sn    = s->get_joints();
    auto begin = sn.first;
    auto end   = sn.second;
    for (auto i = begin, j = i + 1; j < end; j++, i++) {
        if ( i->x == j->x ) {
            int l = min(i->y, j->y);
            int h = max(i->y, j->y);
            for (int z = l; z <= h; z++)
                points.emplace_back(i->x, z);
        }
        if ( i->y == j->y ) {
            int l = min(i->x, j->x);
            int h = max(i->x, j->x);
            for (int z = l; z <= h; z++)
                points.emplace_back(z, i->y);
        }
    }
    return points;
}

void
keycallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        run = 0;
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        dir = SUP;
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        dir = SDOWN;
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        dir = SLEFT;
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        dir = SRIGHT;
}

