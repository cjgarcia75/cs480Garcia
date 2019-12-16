#include "graphics.h"

Graphics::Graphics()
{
  switchShader = false;
}

Graphics::~Graphics()
{
  delete dynamicsWorld;
  delete solver;
  delete broadphase;
  delete dispatcher;
  delete collisionConfig;
}

bool Graphics::Initialize(int width, int height, std::string vsFile, std::string fsFile, int numOfBalls)
{
  g_numOfBalls = numOfBalls;
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif
  
  if(!BulletInit())
  {
    printf("Bullet Failed to Initialize\n");
    return false;
  }

  // For OpenGL 3. This is the shapes VAO?
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // spawn balls
  for (int i = 0; i < g_numOfBalls; i++)
  {
    balls[i] = new Object("../assets/ball.obj", "../assets/brick.jpeg", "ball", 1.0, 1.0, btVector3((12 - (i*2)) , 5.3, i));
    balls[i]->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
    dynamicsWorld->addRigidBody(balls[i]->GetRigidBody());
  }
  //set rest of objects
  bumper1 = new Object("../assets/CylinderBouncers.obj", "../assets/chrome.jpeg", "bouncer1", 1.0, 1.0, btVector3(-17, -42, 14));
  bumper2 = new Object("../assets/CylinderBouncers.obj", "../assets/chrome.jpeg", "bouncer2", 1.0, 1.0, btVector3(-27, -44, 12));
  bumper3 = new Object("../assets/CylinderBouncers.obj", "../assets/chrome.jpeg", "bouncer3", 1.0, 1.0, btVector3(-24, -44, 15));
  obsticle1 = new Object("../assets/Obsticle1Fix.obj", "../assets/wood.jpeg", "obsticle1", 0.0, 0.0, btVector3(-17, -42, 14));
  ramp2 = new Object("../assets/Ramp2Fix.obj", "../assets/wood.jpeg", "slide1", 0.0, 0.0, btVector3(-33, -68, -34.5));
  //rotator = new Object("../assets/rotatorFix.obj", "../assets/wood.jpeg", "rotator", 0.0, 0.0, btVector3(0, 0, 0));
  slide1 = new Object("../assets/slide1Fix.obj", "../assets/wood.jpeg", "slide1", 0.0, 0.0, btVector3(-6.5, -75, -26));
  funnel = new Object("../assets/Funnelfix.obj", "../assets/brick.jpeg", "funnel", 0.0, 0.0, btVector3(0, 0, 0));
  tunnel = new Object("../assets/TunnelFix.obj", "../assets/wood.jpeg", "tunnel", 0.0, 0.0, btVector3(0, -12, 0));
  tunnelJump = new Object("../assets/TunnelJumpFix.obj", "../assets/wood.jpeg", "tunnelJump", 0.0, 0.0, btVector3(-50, -49, 10));
  
  dynamicsWorld->addRigidBody(bumper1->GetRigidBody());
  bumper1->GetRigidBody()->setRestitution(1);
  dynamicsWorld->addRigidBody(bumper2->GetRigidBody());
  bumper2->GetRigidBody()->setRestitution(1);
  dynamicsWorld->addRigidBody(bumper3->GetRigidBody());
  bumper3->GetRigidBody()->setRestitution(1);
  dynamicsWorld->addRigidBody(obsticle1->GetRigidBody());
  dynamicsWorld->addRigidBody(ramp2->GetRigidBody());
  //dynamicsWorld->addRigidBody(rotator->GetRigidBody());
  dynamicsWorld->addRigidBody(slide1->GetRigidBody());
  dynamicsWorld->addRigidBody(funnel->GetRigidBody());
  dynamicsWorld->addRigidBody(tunnel->GetRigidBody());
  dynamicsWorld->addRigidBody(tunnelJump->GetRigidBody());

  bumper1->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  bumper1->GetRigidBody()->setCollisionFlags(bumper1->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  bumper2->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  bumper2->GetRigidBody()->setCollisionFlags(bumper2->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  bumper3->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  bumper3->GetRigidBody()->setCollisionFlags(bumper3->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

  // Set up the shaders
  m_shader = new Shader();
  //otherShader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if((!m_shader->AddShader(GL_VERTEX_SHADER, vsFile, fsFile)))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if((!m_shader->AddShader(GL_FRAGMENT_SHADER, vsFile, fsFile)))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  /// For other shader
  /*if(!otherShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }*/

  // Add the vertex shader
  /*if(!otherShader->AddShader(GL_VERTEX_SHADER, "../shaders/cube_shader-vV2.txt", "../shaders/vertex_shader-fv2.txt"))
  {
    printf(" hereVertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!otherShader->AddShader(GL_FRAGMENT_SHADER, "../shaders/cube_shader-vV2.txt", "../shaders/vertex_shader-fv2.txt"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }*/

  // Connect the program
  /*if(!otherShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }*/

  // Locate the projection matrix in the shader
  /*otherProjectionMatrix = otherShader->GetUniformLocation("projectionMatrix");
  if (otherProjectionMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("other_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  otherViewMatrix = otherShader->GetUniformLocation("viewMatrix");
  if (otherViewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("other_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  otherModelMatrix = otherShader->GetUniformLocation("modelMatrix");
  if (otherModelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("other_modelMatrix not found\n");
    return false;
  }*/

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, unsigned int input)
{
  dynamicsWorld->stepSimulation(dt, 1);
  
  for (int i = 0; i < g_numOfBalls; i++)
  {
    balls[i]->Update(input);
  }
  
  bumper1->Update(input);
  bumper2->Update(input);
  bumper3->Update(input);
  obsticle1->Update(input);
  ramp2->Update(input);
  //rotator->Update(input);
  slide1->Update(input);
  funnel->Update(input);
  tunnel->Update(input);
  tunnelJump->Update(input);
}

void Graphics::Render(bool w, bool a, bool s, bool d, bool r, bool f, unsigned int dt, double xPos, double yPos, unsigned int camInput)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  float color[3],
        amb[3] = {0.5f, 0.5f, 0.5f},
        diff[3] = {0.5f, 0.5f, 0.5f},
        spec[3] = {0.5f, 0.5f, 0.5f},
        cons = 1.0f,
        lin = 0.09f,
        quad = 0.032f;
  
  for (int i = 0; i < g_numOfBalls; i++)
  {
    tempMat = balls[i]->GetModel();
    tempVec[i+3] = glm::vec3(tempMat[3]);  
  }
  if (switchShader == 0)
  {  
    // Start the correct program
    m_shader->Enable();
    
    //Lighting
    GLint temp;
    temp = m_shader->GetUniformLocation("numOfLights");
    glUniform1i(temp, g_numOfBalls + 3);
    if (g_numOfBalls > 0)
    {
      color[0] = 1.0f;
      color[1] = 0.0f;
      color[2] = 0.0f;      
      
      SetLights(3, tempVec, color, amb, diff, spec, cons, lin, quad);
    }
    //ball 2
    
    if (g_numOfBalls > 1)
    {
      color[0] = 0.0f;
      color[1] = 1.0f;
      color[2] = 0.0f;      
      
      SetLights(4, tempVec, color, amb, diff, spec, cons, lin, quad);
    }
    
    //ball 3
    if (g_numOfBalls > 2)
    {
      color[0] = 0.0f;
      color[1] = 0.0f;
      color[2] = 1.0f;      
      
      SetLights(5, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 4
    if (g_numOfBalls > 3)
    {
      color[0] = 0.1f;
      color[1] = 0.9f;
      color[2] = 1.0f;      
      
      SetLights(6, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 5
    if (g_numOfBalls > 4)
    {
      color[0] = 0.5f;
      color[1] = 0.1f;
      color[2] = 0.7f;      
      
      SetLights(7, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 6
    if (g_numOfBalls > 5)
    {
      color[0] = 0.9f;
      color[1] = 0.2f;
      color[2] = 0.3f;      
      
      SetLights(8, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 7
    if (g_numOfBalls > 6)
    {
      color[0] = 0.3f;
      color[1] = 0.5f;
      color[2] = 0.5f;      
      
      SetLights(9, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 8
    if (g_numOfBalls > 7)
    {
      color[0] = 1.0f;
      color[1] = 1.0f;
      color[2] = 0.0f;      
      
      SetLights(10, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 9
    if (g_numOfBalls > 8)
    {
      color[0] = 1.0f;
      color[1] = 0.0f;
      color[2] = 1.0f;      
      
      SetLights(11, tempVec, color, amb, diff, spec, cons, lin, quad);
    }

        //ball 10
    if (g_numOfBalls > 9)
    {
      color[0] = 0.0f;
      color[1] = 1.0f;
      color[2] = 1.0f;      
      
      SetLights(12, tempVec, color, amb, diff, spec, cons, lin, quad);
    }
    //bumper lights1
    
    tempMat = bumper1->GetModel();
    tempVec[0] = glm::vec3(tempMat[3]);  
    
    color[0] = 0.1f;
    color[1] = 0.9f;
    color[2] = 1.0f;      
      
    SetLights(0, tempVec, color, amb, diff, spec, cons, lin, quad);
    
    //bumperlights2
    tempMat = bumper2->GetModel();
    tempVec[1] = glm::vec3(tempMat[3]);  
    
    color[0] = 0.8f;
    color[1] = 0.4f;
    color[2] = 0.0f;      
      
    SetLights(1, tempVec, color, amb, diff, spec, cons, lin, quad);

    //bumper lights3
    tempMat = bumper3->GetModel();
    tempVec[2] = glm::vec3(tempMat[3]);  
    
    color[0] = 0.6f;
    color[1] = 0.2f;
    color[2] = 0.6f;      
      
    SetLights(2, tempVec, color, amb, diff, spec, cons, lin, quad);

    // Send in the projection and view to the shader
    //m_camera->mouseInput(xPos, yPos);
    m_camera->Update(w, a, s, d, r, f, tempVec[camInput]);
    //m_camera->mouseInput(xPos, yPos);
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

    flag = false;

    // ball
    for (int j = 0; j < g_numOfBalls; j++)
    {
      glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(balls[j]->GetModel()));
      balls[j]->Render(m_modelMatrix, m_shader, flag);
    }

    //bumper1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper1->GetModel()));
    bumper1->Render(m_modelMatrix, m_shader, flag);

        //bumper2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper2->GetModel()));
    bumper2->Render(m_modelMatrix, m_shader, flag);

        //bumper3
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper3->GetModel()));
    bumper3->Render(m_modelMatrix, m_shader, flag);

    // obsticle1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(obsticle1->GetModel()));
    obsticle1->Render(m_modelMatrix, m_shader, flag);
    
    // ramp2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ramp2->GetModel()));
    ramp2->Render(m_modelMatrix, m_shader, flag);
    
    // rotator
    //glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(rotator->GetModel()));
    //rotator->Render(m_modelMatrix, m_shader, flag);
    
    // slide1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(slide1->GetModel()));
    slide1->Render(m_modelMatrix, m_shader, flag);
    
    // funnel
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(funnel->GetModel()));
    funnel->Render(m_modelMatrix, m_shader, flag);
    
    //tunnel
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tunnel->GetModel()));
    tunnel->Render(m_modelMatrix, m_shader, flag);

    //tunnelJump
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tunnelJump->GetModel()));
    tunnelJump->Render(m_modelMatrix, m_shader, flag);

    // Get any errors from OpenGL
    auto error = glGetError();
    if ( error != GL_NO_ERROR )
    {
      string val = ErrorString( error );
      std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
  }
  /*else if (switchShader == 1)
  {
    // Start the correct program
    otherShader->Enable();

    tempMat = obsticle1->GetModel();
    tempVec[0] = glm::vec3(tempMat[3]); 
    
    GLint temp;
    temp = m_shader->GetUniformLocation("numOfLights");
    glUniform1i(temp, 1);
    temp = m_shader->GetUniformLocation("lightColor");
    glUniform3f(temp, 1.0, 1.0, 0.5);
    temp = m_shader->GetUniformLocation("light[0].position");
    glUniform3f(temp, tempVec[0].x, tempVec[0].y, tempVec[0].z); // these zeros need to be the position of the ball tempVec.x, tempVec.y, tempVec.z
    temp = m_shader->GetUniformLocation("light[0].ambient");
    glUniform3f(temp, 1.1f, 1.1f, 1.1f);
    temp = m_shader->GetUniformLocation("light[0].diffuse");
    glUniform3f(temp, 0.8f, 0.8f, 0.8f);
    temp = m_shader->GetUniformLocation("light[0].specular");
    glUniform3f(temp, 1.0f, 1.0f, 1.0f);
    temp = m_shader->GetUniformLocation("light[0].constant");
    glUniform1f(temp, 1.0f);
    temp = m_shader->GetUniformLocation("light[0].linear");
    glUniform1f(temp, 0.09f);
    temp = m_shader->GetUniformLocation("light[0].quadratic");
    glUniform1f(temp, 0.032f);

    // Send in the projection and view to the shader
    glUniformMatrix4fv(otherProjectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
    glUniformMatrix4fv(otherViewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

    flag = false;
    
    // obsticle1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(obsticle1->GetModel()));
    obsticle1->Render(m_modelMatrix, m_shader, flag);
    
    // ramp2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ramp2->GetModel()));
    ramp2->Render(m_modelMatrix, m_shader, flag);
    
    // rotator
    //glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(rotator->GetModel()));
    //rotator->Render(m_modelMatrix, m_shader, flag);
    
    // slide1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(slide1->GetModel()));
    slide1->Render(m_modelMatrix, m_shader, flag);
    
    // funnel
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(funnel->GetModel()));
    funnel->Render(m_modelMatrix, m_shader, flag);
    
    // tunnel
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tunnel->GetModel()));
    tunnel->Render(m_modelMatrix, m_shader, flag);

    //tunnelJump
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tunnelJump->GetModel()));
    tunnelJump->Render(m_modelMatrix, m_shader, flag);

    // Get any errors from OpenGL
    auto error = glGetError();
    if ( error != GL_NO_ERROR )
    {
      string val = ErrorString( error );
      std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
  }*/
}

void Graphics::SetLights(int ballNum, glm::vec3 tempVec[], float lc[3], float a[3], float d[3], float s[3], float c, float l, float q)
{
  GLint temp;
  std::string t = "pointLights[" + to_string(ballNum) + "].";
  std::string tempString;

  tempString = t + "lightColor";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform3f(temp, lc[0], lc[1], lc[2]);
  
  tempString = t + "position";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform3f(temp, tempVec[ballNum].x, tempVec[ballNum].y, tempVec[ballNum].z);
  
  tempString = t + "ambient";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform3f(temp, a[0], a[1], a[2]);
  
  tempString = t + "diffuse";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform3f(temp, d[0], d[1], d[2]);
  
  tempString = t + "specular";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform3f(temp, s[0], s[1], s[2]);
  
  tempString = t + "constant";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform1f(temp, c);
  
  tempString = t + "linear";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform1f(temp, l);
  
  tempString = t + "quadratic";
  temp = m_shader->GetUniformLocation(tempString.c_str());
  glUniform1f(temp, q);
}

bool Graphics::BulletInit()
{
  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  dynamicsWorld->setGravity(btVector3(0, -6, 0));
  
  return true;
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

void Graphics::SwitchShader()
{
  switchShader ? switchShader = false : switchShader = true;
}
