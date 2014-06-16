#include "millerrender.hpp"

MillerRender::MillerRender()
{
    gLookAtOther = true;
    gPosition1 = vec3(-1.5f, 0.0f, 0.0f);
   //  gOrientation1;
     // Initialise GLFW
     if( !glfwInit() )
     {
         fprintf( stderr, "Failed to initialize GLFW\n" );
         //return -1;exit
     }

     glfwWindowHint(GLFW_SAMPLES, 4);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // Open a window and create its OpenGL context
     window = glfwCreateWindow( 1024, 768, "Tutorial 17 - Rotations", NULL, NULL);
     if( window == NULL ){
         fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
         glfwTerminate();
        // return -1;exit
     }
     glfwMakeContextCurrent(window);

     // Initialize GLEW
     glewExperimental = true; // Needed for core profile
     if (glewInit() != GLEW_OK) {
         fprintf(stderr, "Failed to initialize GLEW\n");
        // return -1; exit
     }


     //initGL(window);

     // Initialize the GUI
     TwInit(TW_OPENGL_CORE, NULL);
     TwWindowSize(1024, 768);
     TwBar * EulerGUI = TwNewBar("Euler settings");
    //     TwBar * QuaternionGUI = TwNewBar("Quaternion settings");
     TwSetParam(EulerGUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
  //       TwSetParam(QuaternionGUI, NULL, "position", TW_PARAM_CSTRING, 1, "808 16");

     TwAddVarRW(EulerGUI, "Euler X", TW_TYPE_FLOAT, &gOrientation1.x, "step=0.01");
     TwAddVarRW(EulerGUI, "Euler Y", TW_TYPE_FLOAT, &gOrientation1.y, "step=0.01");
     TwAddVarRW(EulerGUI, "Euler Z", TW_TYPE_FLOAT, &gOrientation1.z, "step=0.01");
     TwAddVarRW(EulerGUI, "Pos X"  , TW_TYPE_FLOAT, &gPosition1.x, "step=0.1");
     TwAddVarRW(EulerGUI, "Pos Y"  , TW_TYPE_FLOAT, &gPosition1.y, "step=0.1");
     TwAddVarRW(EulerGUI, "Pos Z"  , TW_TYPE_FLOAT, &gPosition1.z, "step=0.1");


     //TwAddVarRW(QuaternionGUI, "Quaternion", TW_TYPE_QUAT4F, &gOrientation2, "showval=true open=true ");
     //TwAddVarRW(QuaternionGUI, "Use LookAt", TW_TYPE_BOOL8 , &gLookAtOther, "help='Look at the other monkey ?'");

     // Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
     glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
     glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
     glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
     glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
     glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar



     // Ensure we can capture the escape key being pressed below
     glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
     glfwSetCursorPos(window, 1024/2, 768/2);

     // Dark blue background
     glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

     // Enable depth test
     glEnable(GL_DEPTH_TEST);
     // Accept fragment if it closer to the camera than the former one
     glDepthFunc(GL_LESS);

     // Cull triangles which normal is not towards the camera
     glEnable(GL_CULL_FACE);


     // Read our .obj file
     std::vector<unsigned short> indices;
     std::vector<glm::vec3> indexed_vertices;
     std::vector<glm::vec2> indexed_uvs;
     std::vector<glm::vec3> indexed_normals;

     // load model
     //char* file = "/home/kaeon/MyProgram/src/rim.stl";
     //char* file = "/home/kaeon/MyProgram/src/box.stl";

     // 放方塊＋銑刀會有問題 但是放銑刀＋銑刀不會有問題
     char* file = "/home/kaeon/MyProgram/OpenGL-33-myproject/src/cube.obj";
     //char* file = "/home/kaeon/MyProgram/OpenGL-33-myproject/src/ES4.STL";

     //char* file = "/home/kaeon/MyProgram/src/suzanne.obj";
     //char* file = "/home/kaeon/MyProgram/src/monkey.obj";
     //loadOBJ(file,outIndices,vertexArray,uvArray,normalArray);

     //bool res = loadAssImp(file, indices, indexed_vertices, indexed_uvs, indexed_normals);
     loadOBJ(file, indices,indexed_vertices,indexed_uvs,indexed_normals);

     ChangeVerticesCoord(indexed_vertices);



     GLuint VertexArrayID;   
     glGenVertexArrays(1, &VertexArrayID);
     glBindVertexArray(VertexArrayID);
     GLuint WP_VertexArrayID;
     glGenVertexArrays(1, &WP_VertexArrayID);
     glBindVertexArray(WP_VertexArrayID);

     // Create and compile our GLSL program from the shaders
     //GLuint programID = LoadShaders( "/home/kaeon/MyProgram/opengl_test_success/SimpleTransform.vertexshader", "/home/kaeon/MyProgram/opengl_test_success/SingleColor.fragmentshader" );
     programID = LoadShaders(
                 "/home/kaeon/MyProgram/OpenGL-33-myproject/src/StandardShading.vertexshader",
                 "/home/kaeon/MyProgram/OpenGL-33-myproject/src/StandardShading.fragmentshader" );


     // Get a handle for our "MVP" uniform
     MatrixID = glGetUniformLocation(programID, "MVP");
     ViewMatrixID = glGetUniformLocation(programID, "V");
     ModelMatrixID = glGetUniformLocation(programID, "M");

     // Load the texture
     Texture = loadDDS("/home/kaeon/MyProgram/OpenGL-33-myproject/src/uvmap.DDS");

     // Get a handle for our "myTextureSampler" uniform
     TextureID  = glGetUniformLocation(programID, "myTextureSampler");


     /***==================== My triangle=============================e **/
     std::vector<unsigned short> indices2;//(101*101);
     std::vector<glm::vec3> indexed_vertices2;//(101*101);
     std::vector<glm::vec2> indexed_uvs2;
     std::vector<glm::vec3> indexed_normals2;
     //
     /*
     for (int i = 0; i < 101; i++) {
         for (int j = 0; j < 101; j++) {
             double z = sin(float(i)/10.0)*sin(float(i)/10.0);
             indexed_vertices2[i] = glm::vec3( i-50, j-50, z-20.0);
         }
     }*/
     //   CalculateIndices(indices2);
     // calculate indices
    loadOBJ("/home/kaeon/MyProgram/OpenGL-33-myproject/src/ES4.STL", indices2, indexed_vertices2, indexed_uvs2,indexed_normals2);
//    loadOBJ("/home/kaeon/MyProgram/OpenGL-33-myproject/src/cube.obj", indices2,indexed_vertices2,indexed_uvs2,indexed_normals2);

     ChangeVerticesCoord(indexed_vertices2);
     /***==================================================================**/



     // Load it into a VBO

     GLuint vertexbuffer;
     glGenBuffers(1, &vertexbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//     glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
     glBufferData(GL_ARRAY_BUFFER, (indexed_vertices.size() + indexed_vertices2.size()) * sizeof(glm::vec3), 0, GL_STATIC_DRAW);
     glBufferSubData(GL_ARRAY_BUFFER, 0 ,indexed_vertices.size()*sizeof(glm::vec3), &indexed_vertices[0] );
     glBufferSubData(GL_ARRAY_BUFFER, indexed_vertices.size()*sizeof(glm::vec3), indexed_vertices2.size()*sizeof(glm::vec3), &indexed_vertices2[0]);


     GLuint uvbuffer;
     glGenBuffers(1, &uvbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
     //glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
     glBufferData(GL_ARRAY_BUFFER, (indexed_uvs.size()+indexed_uvs2.size() )* sizeof(glm::vec2), 0, GL_STATIC_DRAW);
     glBufferSubData(GL_ARRAY_BUFFER, 0 ,indexed_uvs.size()*sizeof(glm::vec2), &indexed_uvs[0] );
     glBufferSubData(GL_ARRAY_BUFFER, indexed_uvs.size()*sizeof(glm::vec2), indexed_uvs2.size()*sizeof(glm::vec2), &indexed_uvs2[0]);

     GLuint normalbuffer;
     glGenBuffers(1, &normalbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
     //glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
     glBufferData(GL_ARRAY_BUFFER, (indexed_normals.size()+indexed_normals2.size() )* sizeof(glm::vec3), 0, GL_STATIC_DRAW);
     glBufferSubData(GL_ARRAY_BUFFER, 0 ,indexed_normals.size()*sizeof(glm::vec3), &indexed_normals[0] );
     glBufferSubData(GL_ARRAY_BUFFER, indexed_normals.size()*sizeof(glm::vec3), indexed_normals2.size()*sizeof(glm::vec3), &indexed_normals2[0]);


     // Generate a buffer for the indices as well
     GLuint elementbuffer;
     glGenBuffers(1, &elementbuffer);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
     //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indices.size()+indices2.size() )* sizeof(unsigned short), 0, GL_STATIC_DRAW);
     glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0 ,indices.size()*sizeof(unsigned short), &indices[0] );
     glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), indices2.size()*sizeof(unsigned short), &indices2[0]);

     // Get a handle for our "LightPosition" uniform
     glUseProgram(programID);
     GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");


     // For speed computation
     double lastTime = glfwGetTime();
     double lastFrameTime = lastTime;
     int nbFrames = 0;
     std::cout<<"test0"<<std::endl;
         float tt = 0.0;
     do{

         // Measure speed
         double currentTime = glfwGetTime();

         float deltaTime = (float)(currentTime - lastFrameTime);
         lastFrameTime = currentTime;

         nbFrames++;
         if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
             // printf and reset
             printf("%f ms/frame\n", 1000.0/double(nbFrames));
             nbFrames = 0;
             lastTime += 1.0;
         }


         // Clear the screen
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // Use our shader
         glUseProgram(programID);

         /*
         // Compute the MVP matrix from keyboard and mouse input
      //   computeMatricesFromInputs();
         glm::mat4 ProjectionMatrix = getProjectionMatrix();
         glm::mat4 ViewMatrix = getViewMatrix();
         glm::mat4 ModelMatrix = glm::mat4(1.0);
         glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

         // Send our transformation to the currently bound shader,
         // in the "MVP" uniform
         glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
         glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
         glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
 */
         glm::mat4 ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 110.0f);// display range
         glm::mat4 ViewMatrix = glm::lookAt(
                     //glm::vec3( 0, 0, 70 ), // Camera is here
                     glm::vec3( 20, 30, 70 ), // Camera is here
                     //glm::vec3(gOrientation1.x,0,0),// and looks here
                    glm::vec3( 0, 0, 0 ), // and looks here
                     //glm::vec3( 0, 1, 0 )  // Head is up (set to 0,-1,0 to look upside-down)
                     glm::vec3( 3, 10, 5 )  // Head is up (set to 0,-1,0 to look upside-down)
                     );


         glm::vec3 lightPos = glm::vec3(gPosition1.x,2,10);
           //glm::vec3 lightPos = glm::vec3(0,2,10);
         glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

         // Bind our texture in Texture Unit 0
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, Texture);
         // Set our "myTextureSampler" sampler to user Texture Unit 0
         glUniform1i(TextureID, 0);

         // 1rst attribute buffer : vertices
         glEnableVertexAttribArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
         glVertexAttribPointer(
                     0,                  // attribute
                     3,                  // size
                     GL_FLOAT,           // type
                     GL_FALSE,           // normalized?
                     0,                  // stride
                     (void*)0            // array buffer offset
                     );


         // 2nd attribute buffer : UVs
         glEnableVertexAttribArray(1);
         glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
         glVertexAttribPointer(
                     1,                                // attribute
                     2,                                // size
                     GL_FLOAT,                         // type
                     GL_FALSE,                         // normalized?
                     0,                                // stride
                     (void*)0                          // array buffer offset
                     );

         // 3rd attribute buffer : normals
         glEnableVertexAttribArray(2);
         glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
         glVertexAttribPointer(
                     2,                                // attribute
                     3,                                // size
                     GL_FLOAT,                         // type
                     GL_FALSE,                         // normalized?
                     0,                                // stride
                     (void*)0                          // array buffer offset
                     );

         // Index buffer
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


         glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


         { // Euler

             // As an example, rotate arount the vertical axis at 180\B0/sec
         /*    gOrientation1.z += 3.14159f/2.0f * deltaTime * 5;
             gOrientation1.x = 3.14159f/2;
             gPosition1.y = 40;

             // Build the model matrix
             glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
             glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
             glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
             glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;*/
             gOrientation1.z += 3.14159f/2.0f * deltaTime;
             gOrientation1.x = 20;3.14159f/2;
             gPosition1.y = 10;
             tt = tt + 0.01f;
             gPosition1.x = -10+5.0*sin(tt);
             //gPosition1.z = tt;//20.0*sin(tt);

             // Build the model matrix
             glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
             glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
             glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
            //  glm::mat4 ModelMatrix = eulerAngleYXZ((float)3,(float)0,(float)0)*translate(mat4(), glm::vec3(5,0,0)) *TranslationMatrix* RotationMatrix * ScalingMatrix;



             glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

             // Send our transformation to the currently bound shader,
             // in the "MVP" uniform
             glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
             glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
             glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);



             // Draw the triangles !
             glDrawElements(
                         GL_TRIANGLES,      // mode
                         indices.size(),    // count
                         GL_UNSIGNED_SHORT,   // type
                         (void*)0           // element array buffer offset
                         );

         }
//=============================================================================//
         glEnableVertexAttribArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
         glVertexAttribPointer(
                     0,                  // attribute
                     3,                  // size
                     GL_FLOAT,           // type
                     GL_FALSE,           // normalized?
                     0,                  // stride
                     (void*)(0+indexed_vertices.size()*sizeof(glm::vec3))            // array buffer offset
                     );


         // 2nd attribute buffer : UVs
         glEnableVertexAttribArray(1);
         glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
         glVertexAttribPointer(
                     1,                                // attribute
                     2,                                // size
                     GL_FLOAT,                         // type
                     GL_FALSE,                         // normalized?
                     0,                                // stride
                     (void*)(0+indexed_uvs.size()*sizeof(glm::vec2))                          // array buffer offset
                     );

         // 3rd attribute buffer : normals
         glEnableVertexAttribArray(2);
         glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
         glVertexAttribPointer(
                     2,                                // attribute
                     3,                                // size
                     GL_FLOAT,                         // type
                     GL_FALSE,                         // normalized?
                     0,                                // stride
                     (void*)(0+indexed_normals.size()*sizeof(glm::vec3))                          // array buffer offset
                     );

         // Index buffer
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


         glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


         { // Euler

             // As an example, rotate arount the vertical axis at 180\B0/sec
         /*    gOrientation1.z += 3.14159f/2.0f * deltaTime * 5;
             gOrientation1.x = 3.14159f/2;
             gPosition1.y = 40;

             // Build the model matrix
             glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
             glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
             glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
             glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;*/
             gOrientation1.z += 3.14159f/2.0f * deltaTime/1000.0;
             gOrientation1.x = 3.14159f/2;
             gPosition1.y = 10;40;
             tt = tt + 0.01f;
             gPosition1.x = 20.0*sin(tt/100.0);
             //gPosition1.z = tt;//20.0*sin(tt);

             // Build the model matrix
             glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
             glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
             glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
            //  glm::mat4 ModelMatrix = eulerAngleYXZ((float)3,(float)0,(float)0)*translate(mat4(), glm::vec3(5,0,0)) *TranslationMatrix* RotationMatrix * ScalingMatrix;



             glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

             // Send our transformation to the currently bound shader,
             // in the "MVP" uniform
             glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
             glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
             glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);



             // Draw the triangles !
             glDrawElements(
                         GL_TRIANGLES,      // mode
                         indices2.size(),    // count
                         GL_UNSIGNED_SHORT,   // type
                         (void*)(0 + indices.size())           // element array buffer offset
                         );

         }


         //======================================================================================//



         glDisableVertexAttribArray(0);
         glDisableVertexAttribArray(1);
         glDisableVertexAttribArray(2);

         // Draw GUI
         TwDraw();

         // Swap buffers
         glfwSwapBuffers(window);
         glfwPollEvents();

     } // Check if the ESC key was pressed or the window was closed
     while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0 );

     // Cleanup VBO and shader
     glDeleteBuffers(1, &vertexbuffer);
     glDeleteBuffers(1, &uvbuffer);
     glDeleteBuffers(1, &normalbuffer);
     glDeleteBuffers(1, &elementbuffer);
     glDeleteProgram(programID);
     glDeleteTextures(1, &Texture);
     glDeleteVertexArrays(1, &VertexArrayID);
     glDeleteVertexArrays(1, &WP_VertexArrayID);
}
MillerRender::~MillerRender(){

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

bool MillerRender::initGL(GLFWwindow*& window){
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return false;
    }
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(window, 1024/2, 768/2);// I don't what its function

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    return true;
}
bool MillerRender::loadAssImp(
        const char * path,
        std::vector<unsigned short> & indices,
        std::vector<glm::vec3> & vertices,
        std::vector<glm::vec2> & uvs,
        std::vector<glm::vec3> & normals
        ){

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( path,
                                              aiProcess_CalcTangentSpace|            //后处理标志，自动计算切线和副法线
                                              aiProcess_Triangulate|                //后处理标志，自动将四边形面转换为三角面
                                              aiProcess_JoinIdenticalVertices|    //后处理标志，自动合并相同的顶点
                                              aiProcess_SortByPType);                //后处理标志，将不同图元放置到不同的模型中去，图片类型可能是点、直线、三角形等
    /// const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
    if( !scene) {
        fprintf( stderr, importer.GetErrorString());
        getchar();
        return false;
    }
    const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

    // Fill vertices positions
    vertices.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D pos = mesh->mVertices[i];
        vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }

    // Fill vertices texture coordinates
    uvs.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        //     std::cout<<i<<std::endl;
        //      aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
        //        uvs.push_back(glm::vec2(UVW.x, UVW.y));
    }


    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        if (mesh->HasTextureCoords(0)) {
            //uvArray[i * 2 + 0] = mesh->mTextureCoords[0][i].x;
            //uvArray[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
            uvs.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }
    }


    // Fill vertices normals
    normals.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D n = mesh->mNormals[i];
        normals.push_back(glm::vec3(n.x, n.y, n.z));
    }

    // Fill face indices
    indices.reserve(3*mesh->mNumFaces);
    for (unsigned int i=0; i<mesh->mNumFaces; i++){
        // Assume the model has only triangles.
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    // The "scene" pointer will be deleted automatically by "importer"
    return true;
}

bool MillerRender::loadOBJ(    const char *path,    std::vector<unsigned short> &outIndices,
                 std::vector<glm::vec3> &outVertices,
                 std::vector<glm::vec2> &uvArray,
                 std::vector<glm::vec3> &normalArray    ){

    Assimp::Importer importer;
    // 使用导入器导入选定的模型文件 // sintel.obj
    const aiScene* scene = importer.ReadFile( path,
                                              aiProcess_CalcTangentSpace|            //后处理标志，自动计算切线和副法线
                                              aiProcess_Triangulate|                //后处理标志，自动将四边形面转换为三角面
                                              aiProcess_JoinIdenticalVertices|    //后处理标志，自动合并相同的顶点
                                              aiProcess_SortByPType);                //后处理标志，将不同图元放置到不同的模型中去，图片类型可能是点、直线、三角形等
    //const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast);
    if( !scene)
    {
        //导入错误，获取错误信息并进行相应的处理
        std::cout << importer.GetErrorString()<<std::endl;
        //DoTheErrorLogging( importer.GetErrorString());
        return false;
    }


    aiMesh* mesh = scene->mMeshes[0];

    int numOfFaces = mesh->mNumFaces;
    int numOfIndices = numOfFaces * 3;
    /*outIndices.resize(numOfIndices);

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        //getchar();
        //std::cout<<"he"<<std::endl;

        const aiFace &face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        outIndices[i * 3 + 0] = face.mIndices[0];
        outIndices[i * 3 + 1] = face.mIndices[1];
        outIndices[i * 3 + 2] = face.mIndices[2];
    }
    */

    //
    // Fill face indices
    outIndices.reserve(3*mesh->mNumFaces);
    for (unsigned int i=0; i<mesh->mNumFaces; i++){
        // Assume the model has only triangles.
        outIndices.push_back(mesh->mFaces[i].mIndices[0]);
        outIndices.push_back(mesh->mFaces[i].mIndices[1]);
        outIndices.push_back(mesh->mFaces[i].mIndices[2]);
    }
    //

    int numOfVertices = mesh->mNumVertices;
    outVertices.resize(numOfVertices * 3);
    normalArray.resize(numOfVertices * 3);
    uvArray.resize(numOfVertices * 2);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        if (mesh->HasPositions()) {
            outVertices[i] = glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);

            ///  outVertices[i * 3 + 0] = mesh->mVertices[i].x;
            ///      outVertices[i * 3 + 1] = mesh->mVertices[i].y;
            ///     outVertices[i * 3 + 2] = mesh->mVertices[i].z;
        }

        if (mesh->HasNormals()) {
            normalArray[i] = glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            ///     normalArray[i * 3 + 0] = mesh->mNormals[i].x;
            ///      normalArray[i * 3 + 1] = mesh->mNormals[i].x;
            //      normalArray[i * 3 + 2] = mesh->mNormals[i].x;
        }

        if (mesh->HasTextureCoords(0)) {
            uvArray[i] = (glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
            //          uvArray[i * 2 + 0] = mesh->mTextureCoords[0][i].x;
            //            uvArray[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
        }
    }
    return true;
}



void MillerRender::ChangeVerticesCoord(std::vector<glm::vec3>& indexed_vertices){
    GLfloat Cx,Cy,Cz;
    Cx = 0;
    Cy = 0;
    Cz = 0;
    for(int i = 0; i < indexed_vertices.size();i++){
        Cx += indexed_vertices[i].x;
        Cy += indexed_vertices[i].y;
        Cz += indexed_vertices[i].z;
    }
    Cx = Cx/indexed_vertices.size();
    Cy = Cy/indexed_vertices.size();
    Cz = Cz/indexed_vertices.size();
    std::cout<< Cx<<std::endl;
    std::cout<< Cy<<std::endl;
    std::cout<< Cz<<std::endl;
    for(int i = 0; i < indexed_vertices.size();i++){
        indexed_vertices[i] = indexed_vertices[i] - glm::vec3(2.1,2.1,Cz); // 2.3 close maybe 2.1 is very close
    }//  修正模型的位置, 讓旋轉中心看起來像再軸向


}

void MillerRender::CalculateIndices( std::vector<unsigned short>& indices){
    //int i = 0;
    indices.resize(101*100*6);
    for (int y = 0; y < 101; y++) {
        for (int x = 0; x < 100; x++) {
           /* indices.push_back(y * 101 + x);
            indices.push_back(y * 101 + x + 1);
            indices.push_back((y + 1) * 101 + x + 1);

            indices.push_back(y * 101 + x+1);// +1 or not ?? I think this indices was wrong, so I  add it one
            indices.push_back((y + 1) * 101 + x + 1);
            indices.push_back((y + 1) * 101 + x);*/

            indices[0 + 6*x + 6*100*y] = y * 101 + x;
            indices[1 + 6*x + 6*100*y] = y * 101 + x + 1;
            indices[2 + 6*x + 6*100*y] = (y + 1) * 101 + x + 1;

            indices[3 + 6*x + 6*100*y] = y * 101 + x+1;// +1 or not ?? I think this indices was wrong, so I  add it one
            indices[4 + 6*x + 6*100*y] = (y + 1) * 101 + x + 1;
            indices[5 + 6*x + 6*100*y] = (y + 1) * 101 + x;
        }
    }

}
