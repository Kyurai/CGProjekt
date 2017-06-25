#include "myglwidget.h"



//resize OpenGl-Window
void MyGLWidget::resizeGL(int width, int height)
{
    height = (height == 0) ? 1 : height;
    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);

}


//Initialize OpenGL

void MyGLWidget::initializeGL(){


        initializeOpenGLFunctions();

        //setup fullscreen quad geometry
            glm::vec2 quadVerts[4];
            quadVerts[0] = glm::vec2(-1,-1);
            quadVerts[1] = glm::vec2(1,-1);
            quadVerts[2] = glm::vec2(1,1);
            quadVerts[3] = glm::vec2(-1,1);
            //setup quad indices
            GLushort quadIndices[]={ 0,1,2,0,2,3};
            //setup quad vertex array and vertex buffer objects
            glGenVertexArrays(1, &quadVAOID);
            glGenBuffers(1, &quadVBOID);
            glGenBuffers(1, &quadIndicesID);

            glBindVertexArray(quadVAOID);
                glBindBuffer (GL_ARRAY_BUFFER, quadVBOID);
                //pass quad vertices to vertex buffer object
                glBufferData (GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);


                //enable vertex attribute array for vertex position
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,0,0);

                //pass quad indices to element array buffer
                glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, quadIndicesID);
                glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), &quadIndices[0], GL_STATIC_DRAW);

            //get the mesh path for loading of textures
            std::string mesh_path = mesh_filename.substr(0, mesh_filename.find_last_of("/")+1);

            //load the obj model
            vector<unsigned short> indices2;
            vector<glm::vec3> vertices2;
            if(!obj.Load(mesh_filename.c_str(), meshes, vertices, indices, materials, aabb, vertices2, indices2)) {
                cout<<"Cannot load the 3ds mesh"<<endl;
                exit(EXIT_FAILURE);
            }



            int total =0;
            //check the total number of non empty textures since we will use this
            //information to creare a single array texture to store all textures
            for(size_t k=0;k<materials.size();k++) {
                if(materials[k]->map_Kd != "") {
                    total++;
                }
            }

            //load material textures
            for(size_t k=0;k<materials.size();k++) {
                //if the diffuse texture name is not empty
                if(materials[k]->map_Kd != "") {
                    if(k==0) {
                        //generate a new OpenGL array texture
                        glGenTextures(1, &textureID);
                        glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
                        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
                        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);
                    }
                    int texture_width = 0, texture_height = 0, channels=0;

                    const string& filename =  materials[k]->map_Kd;

                    std::string full_filename = mesh_path;
                    full_filename.append(filename);

                    //use SOIL to load the texture
                    GLubyte* pData = SOIL_load_image(full_filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_AUTO);
                    if(pData == NULL) {
                        cerr<<"Cannot load image: "<<full_filename.c_str()<<endl;
                        exit(EXIT_FAILURE);
                    }

                    //Flip the image on Y axis
                    int i,j;
                    for( j = 0; j*2 < texture_height; ++j )
                    {
                        int index1 = j * texture_width * channels;
                        int index2 = (texture_height - 1 - j) * texture_width * channels;
                        for( i = texture_width * channels; i > 0; --i )
                        {
                            GLubyte temp = pData[index1];
                            pData[index1] = pData[index2];
                            pData[index2] = temp;
                            ++index1;
                            ++index2;
                        }
                    }
                    //get the image format
                    GLenum format = GL_RGBA;
                    switch(channels) {
                        case 2:	format = GL_RG32UI; break;
                        case 3: format = GL_RGB;	break;
                        case 4: format = GL_RGBA;	break;
                    }

                    //if this is the first texture, allocate the array texture
                    if(k==0) {
                        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, texture_width, texture_height, total, 0, format, GL_UNSIGNED_BYTE, NULL);
                    }
                    //modify the existing texture
                    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,0,0,k, texture_width, texture_height, 1, format, GL_UNSIGNED_BYTE, pData);

                    //release the SOIL image data
                    SOIL_free_image_data(pData);
                }
            }

        flatShader = new GLSLShader();
        shader = new GLSLShader();
        raytraceShader = new GLSLShader();
        pathtraceShader = new GLSLShader();

        flatShader->LoadFromFile(GL_VERTEX_SHADER,"/home/dustin/Documents/CG_Prakt/FinalProject/shaders/flat.vert");
        flatShader->LoadFromFile(GL_FRAGMENT_SHADER,"/home/dustin/Documents/CG_Prakt/FinalProject/shaders/flat.frag");
        flatShader->CreateAndLinkProgram();

        flatShader->Use();
            flatShader->AddAttribute("vVertex");
            flatShader->AddUniform("MVP");
        flatShader->UnUse();

        //load raytracing shader
            raytraceShader->LoadFromFile(GL_VERTEX_SHADER, "/home/dustin/Documents/CG_Prakt/FinalProject/shaders/raytracer.vert");
            raytraceShader->LoadFromFile(GL_FRAGMENT_SHADER, "/home/dustin/Documents/CG_Prakt/FinalProject/shaders/raytracer.frag");
            //compile and link shader
            raytraceShader->CreateAndLinkProgram();
            raytraceShader->Use();
                //add attribute and uniform
                raytraceShader->AddAttribute("vVertex");
                raytraceShader->AddUniform("MV");
                raytraceShader->AddUniform("P");
                raytraceShader->AddUniform("eyePos");
                raytraceShader->AddUniform("invMVP");
                raytraceShader->AddUniform("light_position");
                raytraceShader->AddUniform("backgroundColor");
                raytraceShader->AddUniform("aabb.min");
                raytraceShader->AddUniform("aabb.max");
                raytraceShader->AddUniform("vertex_positions");
                raytraceShader->AddUniform("triangles_list");
                raytraceShader->AddUniform("VERTEX_TEXTURE_SIZE");
                raytraceShader->AddUniform("TRIANGLE_TEXTURE_SIZE");

                //set values of constant uniforms as initialization
                glUniform1f(raytraceShader->getUniform("VERTEX_TEXTURE_SIZE"), (float)vertices2.size());
                glUniform1f(raytraceShader->getUniform("TRIANGLE_TEXTURE_SIZE"), (float)indices2.size()/4);
                glUniform3fv(raytraceShader->getUniform("aabb.min"),1, glm::value_ptr(aabb.min));
                glUniform3fv(raytraceShader->getUniform("aabb.max"),1, glm::value_ptr(aabb.max));
                glUniform4fv(raytraceShader->getUniform("backgroundColor"),1, glm::value_ptr(bg));
                glUniform1i(raytraceShader->getUniform("vertex_positions"), 1);
                glUniform1i(raytraceShader->getUniform("triangles_list"), 2);
            raytraceShader->UnUse();


            //load pathtracing shader
                pathtraceShader->LoadFromFile(GL_VERTEX_SHADER, "/home/dustin/Documents/CG_Prakt/FinalProject/shaders/pathtracer.vert");
                pathtraceShader->LoadFromFile(GL_FRAGMENT_SHADER, "/home/dustin/Documents/CG_Prakt/FinalProject/shaders/pathtracer.frag");
                //compile and link shader
                pathtraceShader->CreateAndLinkProgram();
                pathtraceShader->Use();
                    //add attribute and uniform
                    pathtraceShader->AddAttribute("vVertex");
                    pathtraceShader->AddUniform("eyePos");
                    pathtraceShader->AddUniform("invMVP");
                    pathtraceShader->AddUniform("light_position");
                    pathtraceShader->AddUniform("backgroundColor");
                    pathtraceShader->AddUniform("aabb.min");
                    pathtraceShader->AddUniform("aabb.max");
                    pathtraceShader->AddUniform("vertex_positions");
                    pathtraceShader->AddUniform("triangles_list");
                    pathtraceShader->AddUniform("time");
                    pathtraceShader->AddUniform("VERTEX_TEXTURE_SIZE");
                    pathtraceShader->AddUniform("TRIANGLE_TEXTURE_SIZE");
                    pathtraceShader->AddUniform("BOUNCES");


                    //set values of constant uniforms as initialization
                    glUniform1f(pathtraceShader->getUniform("VERTEX_TEXTURE_SIZE"), (float)vertices2.size());
                    glUniform1f(pathtraceShader->getUniform("TRIANGLE_TEXTURE_SIZE"), (float)indices2.size()/4);
                    glUniform3fv(pathtraceShader->getUniform("aabb.min"),1, glm::value_ptr(aabb.min));
                    glUniform3fv(pathtraceShader->getUniform("aabb.max"),1, glm::value_ptr(aabb.max));
                    glUniform4fv(pathtraceShader->getUniform("backgroundColor"),1, glm::value_ptr(bg));
                    glUniform1i(pathtraceShader->getUniform("vertex_positions"), 1);
                    glUniform1i(pathtraceShader->getUniform("triangles_list"), 2);
                pathtraceShader->UnUse();



        shader->LoadFromFile(GL_VERTEX_SHADER,"/home/dustin/Documents/CG_Prakt/FinalProject/shaders/shader.vert");
        shader->LoadFromFile(GL_FRAGMENT_SHADER,"/home/dustin/Documents/CG_Prakt/FinalProject/shaders/shader.frag");
        shader->CreateAndLinkProgram();

        shader->Use();
        //add attribute and uniform
                shader->AddAttribute("vVertex");
                shader->AddAttribute("vNormal");
                shader->AddAttribute("vUV");
                shader->AddUniform("MV");
                shader->AddUniform("N");
                shader->AddUniform("P");
                shader->AddUniform("textureMap");
                shader->AddUniform("textureIndex");
                shader->AddUniform("useDefault");
                shader->AddUniform("light_position");
                shader->AddUniform("diffuse_color");
                //set values of constant uniforms as initialization

                glUniform1i(shader->getUniform("textureMap"), 0);
        shader->UnUse();

        //setup the vertex array object and vertex buffer object for the mesh
            //geometry handling
            glGenVertexArrays(1, &vaoID);
            glGenBuffers(1, &vboVerticesID);
            glGenBuffers(1, &vboIndicesID);

            glBindVertexArray(vaoID);
                glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
                //pass mesh vertices
                glBufferData (GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &(vertices[0].pos.x), GL_STATIC_DRAW);

                //enable vertex attribute array for vertex position
                glEnableVertexAttribArray(shader->getAttribute("vVertex"));
                glVertexAttribPointer(shader->getAttribute("vVertex"), 3, GL_FLOAT, GL_FALSE,sizeof(Vertex),0);


                //enable vertex attribute array for vertex normal
                glEnableVertexAttribArray(shader->getAttribute("vNormal"));
                glVertexAttribPointer(shader->getAttribute("vNormal"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(offsetof(Vertex, normal)) );


                //enable vertex attribute array for vertex texture coordinates
                glEnableVertexAttribArray(shader->getAttribute("vUV"));
                glVertexAttribPointer(shader->getAttribute("vUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(offsetof(Vertex, uv)) );



                //if we have a single material, it means the 3ds model contains one mesh
                //we therefore load it into an element array buffer
                if(materials.size()==1) {
                    //pass indices to the element array buffer if there is a single material
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*indices.size(), &(indices[0]), GL_STATIC_DRAW);
                }


            glBindVertexArray(0);

            //setup vao and vbo stuff for the light position crosshair
            glm::vec3 crossHairVertices[6];
            crossHairVertices[0] = glm::vec3(-0.5f,0,0);
            crossHairVertices[1] = glm::vec3(0.5f,0,0);
            crossHairVertices[2] = glm::vec3(0, -0.5f,0);
            crossHairVertices[3] = glm::vec3(0, 0.5f,0);
            crossHairVertices[4] = glm::vec3(0,0, -0.5f);
            crossHairVertices[5] = glm::vec3(0,0, 0.5f);

            //setup light gizmo vertex array and vertex buffer object IDs
            glGenVertexArrays(1, &lightVAOID);
            glGenBuffers(1, &lightVerticesVBO);
            glBindVertexArray(lightVAOID);

                glBindBuffer (GL_ARRAY_BUFFER, lightVerticesVBO);
                //pass crosshair vertices to the buffer object
                glBufferData (GL_ARRAY_BUFFER, sizeof(crossHairVertices), &(crossHairVertices[0].x), GL_STATIC_DRAW);

                //enable vertex attribute array for vertex position
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,0);



            //use spherical coordinates to get the light position
            lightPosOS.x = radius * cos(theta)*sin(phi);
            lightPosOS.y = radius * cos(phi);
            lightPosOS.z = radius * sin(theta)*sin(phi);

            //pass position to 1D texture bound to texture unit 1
            glGenTextures(1, &texVerticesID);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture( GL_TEXTURE_2D, texVerticesID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            GLfloat* pData = new GLfloat[vertices2.size()*4];
            int count = 0;
            for(size_t i=0;i<vertices2.size();i++) {
                pData[count++] = vertices2[i].x;
                pData[count++] = vertices2[i].y;
                pData[count++] = vertices2[i].z;
                pData[count++] = 0;
            }
            //allocate a floating point texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, vertices2.size(),1, 0, GL_RGBA, GL_FLOAT, pData);

            //delete the data pointer
            delete [] pData;



            //store the mesh topology in another texture bound to texture unit 2
            glGenTextures(1, &texTrianglesID);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture( GL_TEXTURE_2D, texTrianglesID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            GLushort* pData2 = new GLushort[indices2.size()];
            count = 0;
            for(size_t i=0;i<indices2.size();i+=4) {
                pData2[count++] = (indices2[i]);
                pData2[count++] = (indices2[i+1]);
                pData2[count++] = (indices2[i+2]);
                pData2[count++] = (indices2[i+3]);
            }
            //allocate an integer format texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16I, indices2.size()/4,1, 0, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, pData2);

            //delete heap allocated buffer
            delete [] pData2;



            //set texture unit 0 as active texture unit
            glActiveTexture(GL_TEXTURE0);

        QPoint p = this->mapFromGlobal(QCursor ::pos());
        lastX = p.x();
        lastY = p.y();


        timer.start();


        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glDepthFunc(GL_LEQUAL);

        //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        //glClearDepth(1.0f);
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);








}

void MyGLWidget::paintGL(){
        ++total_frames;

        float currentFrame = (timer.elapsed())/1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        double fps;
        if(timer.elapsed() >= 1000.0 ){
            fps = total_frames / ((double)timer.elapsed()/1000.0);
            total_frames = 0;
        }
        std:cout << "FPS: " << fps << std::endl;


        //std::cout << "deltaTime: " << deltaTime << std::endl;



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





        //glm::mat4 modelMatrix, perspectiveMatrix, viewMatrix;


        P = glm::perspective(glm::radians(camera.Zoom), (float)MyGLWidget::width() / (float)MyGLWidget::height(), 0.1f, 1000.0f);
        V = camera.GetViewMatrix();

        MV = V * M;
        //viewMatrix = camera.GetViewMatrix();



        //get the eye position and inverse of MVP matrix
            glm::mat4 invMV  = glm::inverse(MV);
            //glm::vec3 eyePos = glm::vec3(invMV[3][0],invMV[3][1],invMV[3][2]);
            glm::mat4 invMVP = glm::inverse(P*MV);

            glm::vec3 eyePos = camera.getEyePos();

            //if raytracing is enabled
            if(bRaytrace) {
                //set the raytracing shader
                raytraceShader->Use();
                    //pass shader uniforms
                    glUniformMatrix4fv(raytraceShader->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV));
                    glUniformMatrix4fv(raytraceShader->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P));
                    glUniform3fv(raytraceShader->getUniform("eyePos"), 1, glm::value_ptr(eyePos));
                    glUniformMatrix4fv(raytraceShader->getUniform("invMVP"), 1, GL_FALSE, glm::value_ptr(invMVP));
                    glUniform3fv(raytraceShader->getUniform("light_position"),1, &(lightPosOS.x));
                        //draw a fullscreen quad
                    //bind the quad vertex array object
                        glBindVertexArray(quadVAOID);
                        //draw two triangles
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
                //unbind raytracing shader
                raytraceShader->UnUse();
            } else if(bPathtrace)
            {
                pathtraceShader->Use();
                            //pass shader uniforms
                            glUniform3fv(pathtraceShader->getUniform("eyePos"), 1, glm::value_ptr(eyePos));
                            glUniform1f(pathtraceShader->getUniform("time"), currentFrame);
                            glUniform3fv(pathtraceShader->getUniform("light_position"),1, &(lightPosOS.x));
                            glUniformMatrix4fv(pathtraceShader->getUniform("invMVP"), 1, GL_FALSE, glm::value_ptr(invMVP));
                            glUniform1i(pathtraceShader->getUniform("BOUNCES"), this->bounces);

                                //draw a fullscreen quad
                            glBindVertexArray(quadVAOID);
                            //draw two triangles
                            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);




                        //unbind pathtracing shader
                        pathtraceShader->UnUse();

            } else {
                //do rasterization
                //bind the mesh vertex array object
                glBindVertexArray(vaoID); {
                    //bind the mesh rendering shader
                    shader->Use();
                        //set the shader uniforms
                        glUniformMatrix4fv(shader->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV));
                        glUniformMatrix3fv(shader->getUniform("N"), 1, GL_FALSE, glm::value_ptr(glm::inverseTranspose(glm::mat3(MV))));
                        glUniformMatrix4fv(shader->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P));
                        glUniform3fv(shader->getUniform("light_position"),1, &(lightPosOS.x));

                        //loop through all materials
                        for(size_t i=0;i<materials.size();i++) {
                            Material* pMat = materials[i];

                            //if material texture filename is not empty
                            //dont use the default colour
                            if(pMat->map_Kd !="") {
                                glUniform1f(shader->getUniform("useDefault"), 0.0);
                                glUniform1i(shader->getUniform("textureIndex"), i);
                            }
                            else
                                //otherwise we have no texture, we use a default colour
                                glUniform1f(shader->getUniform("useDefault"), 1.0);

                            //if we have a single material, we render the whole mesh in a single call
                            if(materials.size()==1)
                                glDrawElements(GL_TRIANGLES,  indices.size() , GL_UNSIGNED_SHORT, 0);
                            else
                                //otherwise we render the submesh
                                glDrawElements(GL_TRIANGLES, pMat->count, GL_UNSIGNED_SHORT, (const GLvoid*)(&indices[pMat->offset]));

                        }

                    //unbind the shader
                    shader->UnUse();
                }
            }

            //disable depth testing
            glDisable(GL_DEPTH_TEST);

            //draw the light gizmo, set the light vertexx array object
            glBindVertexArray(lightVAOID); {
                //set the modelling transform for the light crosshair gizmo
                glm::mat4 T = glm::translate(glm::mat4(1), lightPosOS);
                //bind the shader
                flatShader->Use();
                    //set shader uniforms and draw lines
                    glUniformMatrix4fv(flatShader->getUniform("MVP"), 1, GL_FALSE, glm::value_ptr(P*MV*T));
                        glDrawArrays(GL_LINES, 0, 6);
                //unbind the shader
                flatShader->UnUse();
            }
            //enable depth test
            glEnable(GL_DEPTH_TEST);

        update();
}

void MyGLWidget::wheelEvent(QWheelEvent *event){

    camera.ProcessMouseScroll(event->angleDelta().y() / 8);






}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{


    float xOffset = (float)event->x() - lastX;
    float yOffset = lastY - (float)event->y();

    lastX = (float)event->x();
    lastY = (float)event->y();

    camera.ProcessMouseMovement(xOffset,yOffset);


}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{

     switch(event->key()){
     case Qt::Key_W:
     case Qt::Key_Up:
         camera.ProcessKeyboard(FORWARD,deltaTime);
         break;
     case Qt::Key_A:
     case Qt::Key_Left:
         camera.ProcessKeyboard(LEFT,deltaTime);
         break;
     case Qt::Key_D:
     case Qt::Key_Right:
         camera.ProcessKeyboard(RIGHT,deltaTime);
         break;
     case Qt::Key_S:
     case Qt::Key_Down:
         camera.ProcessKeyboard(BACKWARD,deltaTime);
         break;
     case Qt::Key_K:
         radius += 1.0f;

         lightPosOS.x = radius * cos(theta)*sin(phi);
         lightPosOS.y = radius * cos(phi);
         lightPosOS.z = radius * sin(theta)*sin(phi);
         break;
     case Qt::Key_L:
         radius -= 1.0f;

         lightPosOS.x = radius * cos(theta)*sin(phi);
         lightPosOS.y = radius * cos(phi);
         lightPosOS.z = radius * sin(theta)*sin(phi);
         break;
     case Qt::Key_Space:
         if(bRaytrace)
         {
             bRaytrace = false;
         }
         else
         {
             bRaytrace = true;
         }
         break;
      case Qt::Key_P:
         if(bPathtrace)
         {
             bPathtrace = false;
         }
         else
         {
             bPathtrace = true;
         }

     default:
         QOpenGLWidget::keyPressEvent(event);
     }
 }


void MyGLWidget::receiveBounces(int bounces){
    this->bounces = bounces;
}



