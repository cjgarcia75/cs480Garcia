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
  
  // Set up objects ("modelFilePath", "textureFilePath", "nameOfPhysicsObject", mass, inertia, pos)
  resetBox = new Object("../assets/resetBox.obj", "../assets/white.png", "resetBox", 0.0, 0.0, btVector3(0, 0, 0));
  ball = new Object("../assets/ball.obj", "../assets/chrome.jpeg", "ball", 5.0, 10.0, btVector3(-7, 0, -11.6));
  board = new Object("../assets/boardP1.obj", "../assets/space.jpg", "board", 0.0, 0.0, btVector3(0, 0, 0));
  
  sBumper1 = new Object("../assets/sBumper1.obj", "../assets/venus.jpg", "sBumper1", 0.0, 0.0, btVector3(0, 0, 0));
  sBumper2 = new Object("../assets/sBumper2.obj", "../assets/mars.jpg", "sBumper2", 0.0, 0.0, btVector3(0, 0, 0));
  sBumper3 = new Object("../assets/sBumper3.obj", "../assets/earth.jpg", "sBumper3", 0.0, 0.0, btVector3(0, 0, 0));
  
  flipper1 = new Object("../assets/Flipper.obj", "../assets/chrome.jpeg", "flipper1", 0.0, 10.0, btVector3(-2, -2, -12.9));
  flipper2 = new Object("../assets/Flipper.obj", "../assets/chrome.jpeg", "flipper2", 0.0, 10.0, btVector3(5, -2, -12.9));
  
  boarder1 = new Object("../assets/boarder1.obj", "../assets/green.jpeg", "boarder1", 0.0, 0.0, btVector3(0, 0, 0));
  boarder2 = new Object("../assets/boarder2.obj", "../assets/green.jpeg", "boarder2", 0.0, 0.0, btVector3(0, 0, 0));
  boarder3 = new Object("../assets/boarder3.obj", "../assets/green.jpeg", "boarder3", 0.0, 0.0, btVector3(0, 0, 0));
  boarder4 = new Object("../assets/boarder4.obj", "../assets/green.jpeg", "boarder4", 0.0, 0.0, btVector3(0, 0, 0));
  boarder5 = new Object("../assets/boarder5.obj", "../assets/purple.png", "boarder5", 0.0, 0.0, btVector3(0, 0, 0));
  boarder6 = new Object("../assets/boarder6.obj", "../assets/purple.png", "boarder6", 0.0, 0.0, btVector3(0, 0, 0));

  ball->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  flipper1->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  flipper2->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
  
  dynamicsWorld->addRigidBody(resetBox->GetRigidBody());
  dynamicsWorld->addRigidBody(ball->GetRigidBody());
  dynamicsWorld->addRigidBody(board->GetRigidBody());
  dynamicsWorld->addRigidBody(sBumper1->GetRigidBody());
  dynamicsWorld->addRigidBody(sBumper2->GetRigidBody());
  dynamicsWorld->addRigidBody(sBumper3->GetRigidBody());
  dynamicsWorld->addRigidBody(flipper1->GetRigidBody());
  dynamicsWorld->addRigidBody(flipper2->GetRigidBody());

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

void Graphics::Update(unsigned int dt, unsigned int input, int pull_back, bool launched)
{
  dynamicsWorld->stepSimulation(dt, 1);
  
/*  
  if(!launched)
  {
    if(pull_back < 1)
    {
      pLight1->ChangeColor(m_shader, 'w');
    
      if(pull_back >= 1)
      {
        pLight1->ChangeColor(m_shader, 'g');
      }
    }
  }
  else
  {
    pLight1->ChangeColor(m_shader, 'w');
  }
*/
    resetBox->Update(input, pull_back, launched);
    ball->Update(input, pull_back, launched);
    board->Update(input, pull_back, launched);
    
    sBumper1->Update(input, pull_back, launched);
    sBumper2->Update(input, pull_back, launched);
    sBumper3->Update(input, pull_back, launched);
    
    flipper1->Update(input, pull_back, launched);
    flipper2->Update(input, pull_back, launched);
    
    boarder1->Update(input, pull_back, launched);
    boarder2->Update(input, pull_back, launched);
    boarder3->Update(input, pull_back, launched);
    boarder4->Update(input, pull_back, launched);
    boarder5->Update(input, pull_back, launched);
    boarder6->Update(input, pull_back, launched);
    
    //pLight1->Update(input, pull_back, launched);
}

void Graphics::Render(float spot, float amb, float spec)
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
    GLint temp = m_shader->GetUniformLocation("light.position");
    glUniform3f(temp, tempVec.x, 10, tempVec.z); // these zeros need to be the position of the ball tempVec.x, tempVec.y, tempVec.z
    temp = m_shader->GetUniformLocation("light.direction");
    glUniform3f(temp, 0.0, -1.0, 0.0);
    temp = m_shader->GetUniformLocation("light.cutOff");
    glUniform1f(temp, glm::cos(glm::radians(spot)));
    temp = m_shader->GetUniformLocation("viewPos");
    glUniform3f(temp, 0, 0, 0);
    temp = m_shader->GetUniformLocation("light.ambient");
    glUniform3f(temp, amb, amb, amb);
    temp = m_shader->GetUniformLocation("light.diffuse");
    glUniform3f(temp, 0.8f, 0.8f, 0.8f);
    temp = m_shader->GetUniformLocation("light.specular");
    glUniform3f(temp, spec, spec, spec);
    temp = m_shader->GetUniformLocation("light.constant");
    glUniform1f(temp, 1.0f);
    temp = m_shader->GetUniformLocation("light.linear");
    glUniform1f(temp, 0.09f);
    temp = m_shader->GetUniformLocation("light.quadratic");
    glUniform1f(temp, 0.032f);

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

    flag = false;
    
    // reset box
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(resetBox->GetModel()));
    resetBox->Render(m_modelMatrix, m_shader, flag);
    
    // ball
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
    ball->Render(m_modelMatrix, m_shader, flag);
    
    // board
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
    board->Render(m_modelMatrix, m_shader, flag);
    
    // sBumper1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sBumper1->GetModel()));
    sBumper1->Render(m_modelMatrix, m_shader, flag);
    
    // sBumper2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sBumper2->GetModel()));
    sBumper2->Render(m_modelMatrix, m_shader, flag);
    
    // sBumper3
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sBumper3->GetModel()));
    sBumper3->Render(m_modelMatrix, m_shader, flag);
    
    // flipper1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(flipper1->GetModel()));
    flipper1->Render(m_modelMatrix, m_shader, flag);
    
    // flipper2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(flipper2->GetModel()));
    flipper2->Render(m_modelMatrix, m_shader, flag);
    
    // boarder1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder1->GetModel()));
    boarder1->Render(m_modelMatrix, m_shader, flag);
    
    // boarder2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder2->GetModel()));
    boarder2->Render(m_modelMatrix, m_shader, flag);
    
    // boarder3
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder3->GetModel()));
    boarder3->Render(m_modelMatrix, m_shader, flag);
    
    // boarder4
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder4->GetModel()));
    boarder4->Render(m_modelMatrix, m_shader, flag);
    
    // boarder5
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder5->GetModel()));
    boarder5->Render(m_modelMatrix, m_shader, flag);
    
    // boarder6
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder6->GetModel()));
    boarder6->Render(m_modelMatrix, m_shader, flag);
    
    // pLight1
//    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(pLight1->GetModel()));
//    pLight1->Render(m_modelMatrix, m_shader, flag);

    // Get any errors from OpenGL
    auto error = glGetError();
    if ( error != GL_NO_ERROR )
    {
      string val = ErrorString( error );
      std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
  }
  if (switchShader == 1)
  {
    // Start the correct program
    otherShader->Enable();
    GLint temp = otherShader->GetUniformLocation("light.position");
    glUniform3f(temp, tempVec.x, 10, tempVec.z); // these zeros need to be the position of the ball tempVec.x, tempVec.y, tempVec.z
    temp = otherShader->GetUniformLocation("light.direction");
    glUniform3f(temp, 0.0, -1.0, 0.0);
    temp = otherShader->GetUniformLocation("light.cutOff");
    glUniform1f(temp, glm::cos(glm::radians(spot)));
    temp = otherShader->GetUniformLocation("viewPos");
    glUniform3f(temp, 0, 0, 0);
    temp = otherShader->GetUniformLocation("light.ambient");
    glUniform3f(temp, amb, amb, amb);
    temp = otherShader->GetUniformLocation("light.diffuse");
    glUniform3f(temp, 0.8f, 0.8f, 0.8f);
    temp = otherShader->GetUniformLocation("light.specular");
    glUniform3f(temp, spec, spec, spec);
    temp = otherShader->GetUniformLocation("light.constant");
    glUniform1f(temp, 1.0f);
    temp = otherShader->GetUniformLocation("light.linear");
    glUniform1f(temp, 0.09f);
    temp = otherShader->GetUniformLocation("light.quadratic");
    glUniform1f(temp, 0.032f);

    // Send in the projection and view to the shader
    glUniformMatrix4fv(otherProjectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
    glUniformMatrix4fv(otherViewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

    flag = false;
    
    // reset box
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(resetBox->GetModel()));
    resetBox->Render(m_modelMatrix, otherShader, flag);
    
    //ball
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
    ball->Render(m_modelMatrix, otherShader, flag);
    
    // board
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
    board->Render(m_modelMatrix, otherShader, flag);
    
    // sBumper1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sBumper1->GetModel()));
    sBumper1->Render(m_modelMatrix, otherShader, flag);
    
    // sBumper2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sBumper2->GetModel()));
    sBumper2->Render(m_modelMatrix, otherShader, flag);
    
    // sBumper3
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sBumper3->GetModel()));
    sBumper3->Render(m_modelMatrix, otherShader, flag);
    
    // flipper1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(flipper1->GetModel()));
    flipper1->Render(m_modelMatrix, otherShader, flag);
    
    // flipper2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(flipper2->GetModel()));
    flipper2->Render(m_modelMatrix, otherShader, flag);
    
    // boarder1
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder1->GetModel()));
    boarder1->Render(m_modelMatrix, otherShader, flag);
    
    // boarder2
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder2->GetModel()));
    boarder2->Render(m_modelMatrix, otherShader, flag);
    
    // boarder3
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder3->GetModel()));
    boarder3->Render(m_modelMatrix, otherShader, flag);
    
    // boarder4
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder4->GetModel()));
    boarder4->Render(m_modelMatrix, otherShader, flag);
    
    // boarder5
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder5->GetModel()));
    boarder5->Render(m_modelMatrix, otherShader, flag);
    
    // boarder6
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(boarder6->GetModel()));
    boarder6->Render(m_modelMatrix, otherShader, flag);
    
    // pLight1
//    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(pLight1->GetModel()));
//    pLight1->Render(m_modelMatrix, otherShader, flag);
    
    // Get any errors from OpenGL
    auto error = glGetError();
    if ( error != GL_NO_ERROR )
    {
      string val = ErrorString( error );
      std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
  }
}
/*
void Graphics::CheckLights(Object* obj, int num, int pull_back, bool launched)
{
  if(!launched)
  {
    if(pull_back < 1)
    {
      pLight1 = new Object("../assets/pLight1.obj", "../assets/white.png", "pLight1", 0.0, 0.0, btVector3(0, 0, 0));
    
      if(pull_back >= 1)
      {
        pLight1->ChangeColor(m_shader, 'g');
      }
    }
  }
  else
  {
    pLight1->ChangeColor(m_shader, 'w');
  }
}
*/
bool Graphics::BulletInit()
{
  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
  dynamicsWorld->setGravity(btVector3(0, -15, 0));
  
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
