#include <iostream>
#include "fmt/core.h"
#include "glad/gl.h"
#include "SDL3/SDL.h"

import brot.maths;

int main(int, char *[]) {
    std::cout << "2 + 2 = " << brot::add(2, 2) << std::endl;

    auto sdl_version = SDL_GetVersion();
    fmt::println("Using SDL v{}.{}.{}",
                 SDL_VERSIONNUM_MAJOR(sdl_version),
                 SDL_VERSIONNUM_MINOR(sdl_version),
                 SDL_VERSIONNUM_MICRO(sdl_version));

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        fmt::println("Failed to initialize SDL video subsystem: {}", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    auto *window = SDL_CreateWindow("Indev", 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window) {
        fmt::println("Failed to create SDL window: {}", SDL_GetError());
        return 1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    auto *sdl_gl_ctx = SDL_GL_CreateContext(window);
    if (!sdl_gl_ctx) {
        fmt::println("Failed to create SDL GL context: {}", SDL_GetError());
        return 1;
    }

    GladGLContext gl{};
    auto glad_version = gladLoadGLContext(&gl, (GLADloadfunc) SDL_GL_GetProcAddress);
    fmt::println("Using Glad v{}.{}",
                 GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));

    SDL_ShowWindow(window);

    bool running = true;
    do {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }

        gl.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    } while (running);

    SDL_GL_DestroyContext(sdl_gl_ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
