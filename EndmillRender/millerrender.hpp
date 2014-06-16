#ifndef MILLERRENDER_H
#define MILLERRENDER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
//#include<boost/filesystem.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

using namespace glm;

#include<AntTweakBar.h>


#include<common/shader.hpp>
#include<common/texture.hpp>
#include<common/controls.hpp>
#include<common/quaternion_utils.hpp>
//#include"/home/kaeon/GL/common/quaternion_utils.hpp"


#include <assimp/Importer.hpp>      // 导入器在该头文件中定义
#include <assimp/scene.h>           // 读取到的模型数据都放在scene中
#include <assimp/postprocess.h>     // 该头文件中包含后处理的标志位定义
#include<assert.h>
  using namespace std;

class MillerRender
{
public:
    MillerRender();
    ~MillerRender();
private:
    GLFWwindow* window;
    bool gLookAtOther;// = true;

     GLuint programID;
     GLuint MatrixID;// = glGetUniformLocation(programID, "MVP");
     GLuint ViewMatrixID;// = glGetUniformLocation(programID, "V");
     GLuint ModelMatrixID;// = glGetUniformLocation(programID, "M");
     GLuint Texture;// = loadDDS("C:/Users/Kaeon/Desktop/opengl/OpenGL-tutorial_v0014_33/src/uvmap.DDS");
     GLuint TextureID;//  = glGetUniformLocation(programID, "myTextureSampler");



    vec3 gPosition1;//(-1.5f, 0.0f, 0.0f);
    vec3 gOrientation1;
    vec3 gPosition2;
    vec3 gOrientation2;


    bool loadOBJ(    const char *path,    std::vector<unsigned short> &outIndices,
                     std::vector<glm::vec3> &outVertices,
                     std::vector<glm::vec2> &uvArray,
                     std::vector<glm::vec3> &normalArray    );
    bool initGL(GLFWwindow*& window);
    bool loadAssImp(
            const char * path,
            std::vector<unsigned short> & indices,
            std::vector<glm::vec3> & vertices,
            std::vector<glm::vec2> & uvs,
            std::vector<glm::vec3> & normals
            );
    void loadMVP();
    void ChangeVerticesCoord(std::vector<vec3> &indexed_vertices);
    void CalculateIndices(std::vector<unsigned short> &indices);
};

#endif // MILLERRENDER_H





// theritical, loadOBJ and loadAssImp is same, but loadAssImp has some bug.......I still don't know how it happen

