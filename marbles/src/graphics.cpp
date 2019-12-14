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

bool Graphics::Initialize(int width, int height, std::string vsFile, std::string fsFile)
{
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
  
  // Set up objects
  ball = new Object("../assets/ball.obj", "../assets/brick.jpeg", "ball", 1.0, 1.0, btVector3(14, 5.3, 0));
  obsticle1 = new Object("../assets/Obsticle1Fix.obj", "../assets/wood.jpeg", "obsticle1", 0.0, 0.0, btVector3(-17, -42, 14));
  ramp2 = new Object("../assets/Ramp2Fix.obj", "../assets/wood.jpeg", "slide1", 0.0, 0.0, btVector3(-33, -68, -34.5));
  //rotator = new Object("../assets/rotatorFix.obj", "../assets/wood.jpeg", "rotator", 0.0, 0.0, btVector3(0, 0, 0));
  slide1 = new Object("../assets/slide1Fix.obj", "../assets/wood.jpeg", "slide1", 0.0, 0.0, btVector3(-6.5, -75, -26));
  funnel = new Object("../assets/Funnelfix.obj", "../assets/brick.jpeg", "funnel", 0.0, 0.0, btVector3(0, 0, 0));
  tunnel = new Object("../assets/TunnelFix.obj", "../assets/wood.jpeg", "tunnel", 0.0, 0.0, btVector3(0, -12, 0));
  tunnelJump = new Object("../assets/TunnelJumpFix.obj", "../assets/wood.jpeg", "tunnelJump", 0.0, 0.0, btVector3(-50, -49, 10));
  
  ball->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  
  dynamicsWorld->addRigidBody(ball->GetRigidBody());
  dynamicsWorld->addRigidBody(obsticle1->GetRigidBody());
  dynamicsWorld->addRigidBody(ramp2->GetRigidBody());
  //dynamicsWorld->addRigidBody(rotator->GetRigidBody());
  dynamicsWorld->addRigidBody(slide1->GetRigidBody());
  dynamicsWorld->addRigidBody(funnel->GetRigidBody());
  dynamicsWorld->addRigidBody(tunnel->GetRigidBody());
  dynamicsWorld->addRigidBody(tunnelJump->GetRigidBody());

  // Set up the shaders
  m_shader = new Shader();
  otherShader = new Shader();
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
  if(!otherShader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!otherShader->AddShader(GL_VERTEX_SHADER, "../shaders/vertex_shader-v.txt", "../shaders/vertex_shader-f.txt"))
  {
    printf(" hereVertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!otherShader->AddShader(GL_FRAGMENT_SHADER, "../shaders/vertex_shader-v.txt", "../shaders/vertex_shader-f.txt"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!otherShader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  otherProjectionMatrix = otherShader->GetUniformLocation("projectionMatrix");
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
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, unsigned int input)
{
  dynamicsWorld->stepSimulation(dt, 1);
  
  ball->Update(input);
  obsticle1->Update(input);
  ramp2->Update(input);
  //rotator->Update(input);
  slide1->Update(input);
  funnel->Update(input);
  tunnel->Update(input);
  tunnelJump->Update(input);
}

void Graphics::Render(bool w, bool a, bool s, bool d, unsigned int dt, double xPos, double yPos)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

  tempMat = ball->GetModel();
  tempVec = glm::vec3(tempMat[3]);  
  if (switchShader == 0)
  {  
    // Start the correct program
    m_shader->Enable();
    
    //Lighting
    GLint temp;
    temp = m_shader->GetUniformLocation("numOfLights");
    glUniform1i(temp, 1);
    temp = m_shader->GetUniformLocation("lightColor");
    glUniform3f(temp, 1.0, 1.0, 0.0);
    temp = m_shader->GetUniformLocation("pointLights[0].position");
    glUniform3f(temp, tempVec.x, tempVec.y, tempVec.z); // these zeros need to be the position of the ball tempVec.x, tempVec.y, tempVec.z
    temp = m_shader->GetUniformLocation("pointLights[0].ambient");
    glUniform3f(temp, 0.05f, 0.05f, 0.05f);
    temp = m_shader->GetUniformLocation("pointLights[0].diffuse");
    glUniform3f(temp, 0.8f, 0.8f, 0.8f);
    temp = m_shader->GetUniformLocation("pointLights[0].specular");
    glUniform3f(temp, 1.0f, 1.0f, 1.0f);
    temp = m_shader->GetUniformLocation("pointLights[0].constant");
    glUniform1f(temp, 1.0f);
    temp = m_shader->GetUniformLocation("pointLights[0].linear");
    glUniform1f(temp, 0.09f);
    temp = m_shader->GetUniformLocation("pointLights[0].quadratic");
    glUniform1f(temp, 0.032f);

    // Send in the projection and view to the shader
    //m_camera->mouseInput(xPos, yPos);
    m_camera->Update(w, a, s, d, dt, tempVec);
    //m_camera->mouseInput(xPos, yPos);
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

    flag = false;

    // ball
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
    ball->Render(m_modelMatrix, m_shader, flag);
    
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
  else if (switchShader == 1)
  {
    // Start the correct program
    otherShader->Enable();
    
    GLint temp;
    temp = m_shader->GetUniformLocation("numOfLights");
    glUniform1i(temp, 1);
    temp = m_shader->GetUniformLocation("lightColor");
    glUniform3f(temp, 1.0, 1.0, 0.5);
    temp = m_shader->GetUniformLocation("light[0].position");
    glUniform3f(temp, tempVec.x, tempVec.y, tempVec.z); // these zeros need to be the position of the ball tempVec.x, tempVec.y, tempVec.z
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

    // ball
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
    ball->Render(m_modelMatrix, m_shader, flag);
    
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
  }
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
