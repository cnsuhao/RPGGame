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

  // Gpu init (screen)
  GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
  GPU_Target* screen = GPU_Init(1280, 720, GPU_DEFAULT_INIT_FLAGS | SDL_WINDOW_RESIZABLE);
  if (screen == NULL)
    return 1;



  // GPU_SetFullscreen(true, true);
  // GPU load image
  GPU_Image* image = GPU_LoadImage("Content/test_image2.png");
  if (image == NULL)
    return 2;
  GPU_SetImageFilter(image, GPU_FILTER_NEAREST);

  // Standard SDL
  SDL_Event event;


  unsigned int vertShader = GPU_LoadShader(GPU_VERTEX_SHADER,
                                           "Content/Shaders/Menu.vert");
  std::cout << GPU_GetShaderMessage() << "\n";
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



  unsigned int SSCAVertShader = GPU_LoadShader(GPU_VERTEX_SHADER,
                                           "Content/Shaders/SSCA.vert");
  std::cout << GPU_GetShaderMessage() << "\n";
  unsigned int SSCAFragShader = GPU_LoadShader(GPU_FRAGMENT_SHADER,
                                           "Content/Shaders/SSCA.frag");
  std::cout << GPU_GetShaderMessage() << "\n";
  unsigned int SSCAprogram = GPU_CreateShaderProgram();


  GPU_AttachShader(SSCAprogram, SSCAVertShader);
  GPU_AttachShader(SSCAprogram, SSCAFragShader);
  GPU_LinkShaderProgram(SSCAprogram);

  GPU_ShaderBlock SSCAshaderblock = GPU_LoadShaderBlock(
      SSCAprogram, "gpu_Vertex", "gpu_TexCoords",
      "gpu_Color", "gpu_ModelViewProjectionMatrix");


  int SSCAlocation = GPU_GetUniformLocation(SSCAprogram, "SSCAAmount");
  int timelocation = GPU_GetUniformLocation(shaderprogram, "time");
  int camveclocation = GPU_GetUniformLocation(shaderprogram, "camvec");
  // int texlocation = GPU_GetUniformLocation(shaderprogram, "tex");
  // GPU_ActivateShaderProgram(shaderprogram, &SSCAshaderblock);
  // GPU_SetShaderImage(image, texlocation, 0);

  GPU_Image* framebuffer = GPU_CreateImage(screen->w, screen->h, GPU_FORMAT_RGB);
  GPU_Target* frametarget = GPU_LoadTarget(framebuffer);
  // screen->w, screen->h

  GPU_Camera cam = GPU_GetCamera(screen);
  cam.x = -500;
  cam.y = -200;

  // cam.zoom = 1.75f;
  GPU_SetCamera(frametarget, &cam);

  // fps counter variables
  unsigned int lastSecond = 0;
  unsigned int fps = 0;

  bool ismoving[2] = {false, false};
  float camvec[2] = {0, 0};

  GPU_ClearRGB(screen, 51, 51, 51);
  GPU_ClearRGB(frametarget, 51, 51, 51);
  //  GPU_SetViewport(frametarget, viewport);
  // GPU_SetViewport(screen, viewport);
  // GPU_Set

  // GPU_SetVirtualResolution(screen, 1920, 1080);
  GPU_SetVirtualResolution(frametarget, 1280, 720);
  // frametarget->w = 1920;
  // frametarget->h = 1080;

  unsigned char done = 0;
  while (!done) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
        done = 1;
      } else if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          GPU_SetWindowResolution(event.window.data1, event.window.data2);
          GPU_FreeTarget(frametarget);
          GPU_FreeImage(framebuffer);
          framebuffer = GPU_CreateImage(event.window.data1,
                                        event.window.data2, GPU_FORMAT_RGB);
          frametarget = GPU_LoadTarget(framebuffer);
          GPU_SetVirtualResolution(frametarget, 1280, 720);
        }
      } else if (event.type == SDL_KEYUP) {
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
    cam.x -= camvec[0] * 60.0f;
    cam.y -= camvec[1] * 60.0f;
    GPU_SetCamera(frametarget, &cam);

    // Update logic
    // GPU_SetCamera(screen, &cam);

    // GPU drawing
    // GPU_Clear(screen);
    // GPU_Clear(frametarget);
    // GPU_ClearRGB(screen, 51, 51, 51);
    GPU_ClearRGB(frametarget, 51, 51, 51);

    GPU_ActivateShaderProgram(shaderprogram, &shaderblock);
    GPU_SetUniformf(timelocation, static_cast<float>(SDL_GetTicks())/1000.0f);
    GPU_SetUniformfv(camveclocation, 1, 2, camvec);
    // GPU_Blit(image, NULL, frametarget, image->w/2, image->h/2);
    GPU_BlitScale(image,
                  NULL,
                  frametarget,
                  image->w/2,
                  image->h/2,
                  10,
                  10);

    // GPU_DeactivateShaderProgram();
    GPU_ActivateShaderProgram(SSCAprogram, &SSCAshaderblock);
    //  GPU_DeactivateShaderProgram();

     GPU_Blit(framebuffer, NULL, screen, screen->w/2.0f,
              screen->h/2.0f);
    // GPU_BlitScale(framebuffer,
    //              NULL,
    //              screen,
    //              screen->w/2,
    //              screen->h/2,
    //              screen->w/1280.0f,
    //              screen->h/720.0f);
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

  // De-initialize
  GPU_FreeShaderProgram(SSCAprogram);
  GPU_FreeShader(SSCAVertShader);
  GPU_FreeShader(SSCAFragShader);
  GPU_FreeShaderProgram(shaderprogram);
  GPU_FreeShader(vertShader);
  GPU_FreeShader(fragShader);
  GPU_FreeTarget(frametarget);
  GPU_FreeTarget(screen);
  GPU_FreeImage(image);
  GPU_FreeImage(framebuffer);
  GPU_Quit();
  return 0;
}



