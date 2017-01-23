// Copyright 2017 Anas Youssef Idiab
// #include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <iostream>
#ifdef _WIN32
#include <math.h>
#endif

int main(int argc, char* argv[]) {
  std::cout << argc << "\n";
  for (int i = 0; i < argc; i++) {
    std::cout << argv[i] << "\n";
  }

  // GPU init (screen)
  GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
  GPU_Target* screen = GPU_Init(1280, 720, GPU_DEFAULT_INIT_FLAGS);
  if (screen == NULL)
    return 1;
  // GPU load image
  GPU_Image* image = GPU_LoadImage("Content/test_image.png");
  if (image == NULL)
    return 2;
  // Standard SDL
  SDL_Event event;

  if (GPU_GetDebugLevel() == GPU_DEBUG_LEVEL_MAX)
    std::cout << "Compiling vertex shader:" << "\n";
  unsigned int vertShader = GPU_LoadShader(GPU_VERTEX_SHADER,
                                           "Content/Shaders/Menu.vert");
  std::cout << GPU_GetShaderMessage() << "\n";
  if (GPU_GetDebugLevel() == GPU_DEBUG_LEVEL_MAX)
    std::cout << "Compiling fragment shader:" << "\n";
  unsigned int fragShader = GPU_LoadShader(GPU_FRAGMENT_SHADER,
                                           "Content/Shaders/Menu.frag");
  std::cout << GPU_GetShaderMessage() << "\n";
  unsigned int shaderprogram = GPU_CreateShaderProgram();


  GPU_AttachShader(shaderprogram, vertShader);
  GPU_AttachShader(shaderprogram, fragShader);
  GPU_LinkShaderProgram(shaderprogram);

  GPU_ShaderBlock shaderblock = GPU_LoadShaderBlock(
      shaderprogram, "gpu_Vertex", "gpu_TexCoords",
      "gpu_Color", "gpu_ModelViewProjectionMatrix");

  int timelocation = GPU_GetUniformLocation(shaderprogram, "time");
  int camveclocation = GPU_GetUniformLocation(shaderprogram, "camvec");
  // int texlocation = GPU_GetUniformLocation(shaderprogram, "tex");
  GPU_ActivateShaderProgram(shaderprogram, &shaderblock);
  // GPU_SetShaderImage(image, texlocation, 0);

  GPU_Camera cam = GPU_GetCamera(screen);
  cam.x = -500;
  cam.y = -200;
  //cam.zoom = 1.75f;
  GPU_SetCamera(screen, &cam);

  // fps counter variables
  unsigned int lastSecond = 0;
  unsigned int fps = 0;

  bool ismoving[2] = {false, false};
  float camvec[2] = {0, 0};

  unsigned char done = 0;
  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        done = 1;
      if (event.type == SDL_KEYUP){
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          done = 1;
        }
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    ismoving[0] = false;
    if (state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]) {
      ismoving[0] = true;
      camvec[0] = 0.5f;
    } else if (!state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D]) {
      ismoving[0] = true;
      camvec[0] = -0.5f;
    }

    ismoving[1] = false;
    if (state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]) {
      ismoving[1] = true;
      camvec[1] = 0.5f;
    } else if (!state[SDL_SCANCODE_W] && state[SDL_SCANCODE_S]) {
      ismoving[1] = true;
      camvec[1] = -0.5f;
    }


    if (!ismoving[0] && camvec[0] != 0) {
      camvec[0] /= 1.1f;
      if (camvec[0] <= 0.01f && camvec[0] >= -0.01f) {
        camvec[0] = 0;
      }
    }

    if (!ismoving[1] && camvec[1] != 0) {
      camvec[1] /= 1.1f;
      if (camvec[1] <= 0.01f && camvec[1] >= -0.01f) {
        camvec[1] = 0;
      }
    }

    // Update camera
    cam.x -= camvec[0] * 40.0f;
    cam.y -= camvec[1] * 40.0f;
    GPU_SetCamera(screen, &cam);

    // Update logic
    // GPU_SetCamera(screen, &cam);

    GPU_SetUniformf(timelocation, static_cast<float>(SDL_GetTicks())/1000.0f);
    GPU_SetUniformfv(camveclocation, 1, 2, camvec);

    // GPU drawing
    GPU_Clear(screen);
    GPU_Blit(image, NULL, screen, image->w/2, image->h/2);
    GPU_Flip(screen);
    // Count fps
    if (lastSecond == static_cast<unsigned int>(floor(SDL_GetTicks()/1000))) {
      fps++;
    } else {
      fps++;
      std::cout << "FPS:" << fps << "\n";
      fps = 0;
      lastSecond = static_cast<unsigned int>(floor(SDL_GetTicks()/1000));
    }
  }
  GPU_FreeShaderProgram(shaderprogram);
  GPU_FreeShader(vertShader);
  GPU_FreeShader(fragShader);
  GPU_Quit();
  return 0;
}



