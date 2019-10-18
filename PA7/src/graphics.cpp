#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, std::string vsFile, std::string fsFile, std::string objFile)
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
  
  // read texture file names
  std::ifstream file("../config/textures.txt", std::ios::in | std::ios::binary);
  if(!file)
  {
    std::cout << "Could not open textures.txt\n";
    return false;
  }
  for(int i = 0; i < numPlanets; i++)
  {
    std::getline(file, texFiles[i]);
  }
  file.close();

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create the planets
  for(int i = 0; i < numPlanets; i++)
  {
    planets[i] = new Object(objFile, texFiles[i]);
  }
  ConfigPlanets("../config/config.txt");
  
  rings = new Object("../assets/ring.obj", "../assets/rings.jpg");
  ConfigRings();

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, vsFile, fsFile))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, vsFile, fsFile))
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

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, int index, int speed)
{
  // Update the object
  for(int i = 0; i < numPlanets; i++)
  {
    if(planets[i]->GetMoon())
    {
      if(planets[i]->GetName() == "moon")
        planets[i]->SetParent(planets[earth]->GetTran());
      
      else if(planets[i]->GetName() == "phobos" || planets[i]->GetName() == "deimos")
        planets[i]->SetParent(planets[mars]->GetTran());
        
      else if(planets[i]->GetName() == "europa" || planets[i]->GetName() == "io" || planets[i]->GetName() == "ganymede" || planets[i]->GetName() == "callisto")
        planets[i]->SetParent(planets[jupiter]->GetTran());
        
      else if(planets[i]->GetName() == "mimas" || planets[i]->GetName() == "enceladus" || planets[i]->GetName() == "tethys" || planets[i]->GetName() == "dione" 
              || planets[i]->GetName() == "rhea" || planets[i]->GetName() == "titan" || planets[i]->GetName() == "iapetus")
        planets[i]->SetParent(planets[saturn]->GetTran());
        
      else if(planets[i]->GetName() == "miranda" || planets[i]->GetName() == "ariel" || planets[i]->GetName() == "umbriel"
              || planets[i]->GetName() == "titania" || planets[i]->GetName() == "oberon")
        planets[i]->SetParent(planets[uranus]->GetTran());
        
      else if(planets[i]->GetName() == "triton")
        planets[i]->SetParent(planets[neptune]->GetTran());
        
      else if(planets[i]->GetName() == "charon")
        planets[i]->SetParent(planets[pluto]->GetTran());
    }
    
    ChangeSpeed(planets[i], speed);
      
    planets[i]->Update(dt);
  }
  rings->SetParent(planets[saturn]->GetTran());
  rings->Update(dt);
  
  ChangeCamera(index);
}

void Graphics::ChangeCamera(int i)
{
  if(i == system)
    m_camera->Update(glm::vec3(0.0, 20.0, -40.0), glm::vec3(0.0, 0.0, 0.0));
  else
  {
    glm::mat4 temp(planets[i]->GetTran());
    glm::vec3 t(temp[3]);
    
    if(i == sun)
      m_camera->Update(glm::vec3(t.x, (t.y + 1.0), (t.z - 9.0)), glm::vec3(t.x, t.y, t.z));
      
    else if(i == mercury || i == venus || i == earth || i == mars || i == pluto)
      m_camera->Update(glm::vec3(t.x, (t.y + 0.2), (t.z - 0.5)), glm::vec3(t.x, t.y, t.z));
      
    else if(i == uranus || i == neptune)
      m_camera->Update(glm::vec3(t.x, (t.y + 1.0), (t.z - 4.0)), glm::vec3(t.x, t.y, t.z));
    
    else
      m_camera->Update(glm::vec3(t.x, (t.y + 1.0), (t.z - 6.0)), glm::vec3(t.x, t.y, t.z));
  }
}

void Graphics::ChangeSpeed(Object* p, int speed)
{  
  if(speed == 1)
    return;
    
  else if(speed == 2)
    p->SetSpeed2();
    
  else if(speed == 3)
    p->SetSpeed3();
    
  else if(speed == 4)
    p->SetSpeed1();
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the object
  for(int i = 0; i < numPlanets; i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(planets[i]->GetModel()));
    planets[i]->Render();
  }
  
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(rings->GetModel()));
  rings->Render();

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::ConfigPlanets(std::string file)
{
  // open config file
  std::ifstream ifile(file, std::ios::in | std::ios::binary);
  if(!ifile)
  {
    std::cout << "Could not open config file\n";
    return;
  }
  
  std::string temp, trash;
  bool b;
  int i, j;
  float f, g, h, k, t, o;
  
  // skip first 13 lines of config file
  for(int z = 0; z < 14; z++)
  {
    ifile.ignore(50, '\n');
  }
  
  // set values for each planet
  for(int c = 0; c < numPlanets; c++)
  {  
    ifile >> temp >> b >> f >> i >> g >> j >> h >> k >> t >> o >> trash;
  
    planets[c]->SetName(temp);
  
    planets[c]->SetMoon(b);
  
    planets[c]->SetScale(f);
  
    planets[c]->SetSpin(i);
  
    planets[c]->SetSSpeed(g);
  
    planets[c]->SetRot(j);
  
    planets[c]->SetRSpeed(h);
  
    planets[c]->SetRadius(k);
    
    planets[c]->SetTilt(t);
    
    planets[c]->SetOrbital(o);
  }
  
  ifile.close();
}

void Graphics::ConfigRings()
{
  rings->SetName("rings");
  rings->SetMoon(false);
  rings->SetRing(true);
  rings->SetScale(2.0f);
  rings->SetSpin(1);
  rings->SetSSpeed(0.0005f);
  rings->SetRot(0);
  rings->SetRSpeed(0.0f);
  rings->SetRadius(0.0f);
  rings->SetTilt(26.73f);
  rings->SetOrbital(0.0f);
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

