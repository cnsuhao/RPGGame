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
  GPU_Target* screen = GPU_Init(800, 600, GPU_DEFAULT_INIT_FLAGS);
  if (screen == NULL)
    return 1;
  // GPU load image
  GPU_Image* image = GPU_LoadImage("./../Content/test_image.png");
  if (image == NULL)
    return 2;
  // Standard SDL
  SDL_Event event;

  if (GPU_GetDebugLevel() == GPU_DEBUG_LEVEL_MAX)
    std::cout << "Compiling vertex shader:" << "\n";
  unsigned int vertShader = GPU_LoadShader(GPU_VERTEX_SHADER,
                                           "./../Content/Shaders/Menu.vert");
  std::cout << GPU_GetShaderMessage() << "\n";
  if (GPU_GetDebugLevel() == GPU_DEBUG_LEVEL_MAX)
    std::cout << "Compiling fragment shader:" << "\n";
  unsigned int fragShader = GPU_LoadShader(GPU_FRAGMENT_SHADER,
                                           "./../Content/Shaders/Menu.frag");
  std::cout << GPU_GetShaderMessage() << "\n";
  unsigned int shaderprogram = GPU_CreateShaderProgram();


  GPU_AttachShader(shaderprogram, vertShader);
  GPU_AttachShader(shaderprogram, fragShader);
  GPU_LinkShaderProgram(shaderprogram);

  GPU_ShaderBlock shaderblock = GPU_LoadShaderBlock(
      shaderprogram, "gpu_Vertex", "gpu_TexCoords",
      "gpu_Color", "gpu_ModelViewProjectionMatrix");

  int texlocation = GPU_GetUniformLocation(shaderprogram, "tex");
  GPU_ActivateShaderProgram(shaderprogram, &shaderblock);
  GPU_SetShaderImage(image, texlocation, 0);



  GPU_Camera cam = GPU_GetCamera(screen);
  cam.x = -100;
  GPU_SetCamera(screen, &cam);

  // fps counter variables
  unsigned int lastSecond = 0;
  unsigned int fps = 0;

  unsigned char done = 0;
  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        done = 1;
    }
    // Update logic
    GPU_SetCamera(screen, &cam);
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



