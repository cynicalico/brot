#include <iostream>
#include "fmt/core.h"
#include "glad/gl.h"
#include "SDL3/SDL.h"

import brot.window;
import brot.engine;

int main(int, char *[]) {
    auto e = brot::engine();

    e.declare_dependencies<brot::stage::update>("a", {"b"});
    e.declare_dependencies<brot::stage::update>("b", {"c"});

    e.register_receiver<brot::stage::update>("a", [](const auto &){
        fmt::println("a update");
    });

    e.register_receiver<brot::stage::update>("b", [](const auto &){
        fmt::println("b update");
    });

    e.register_receiver<brot::stage::update>("c", [](const auto &){
        fmt::println("c update");
    });

    e.run_stage<brot::stage::update>(0.0);

//    auto w = brot::window();
//
//    auto *sdl_gl_ctx = SDL_GL_CreateContext(w.handle());
//    if (!sdl_gl_ctx) {
//        fmt::println("Failed to create SDL GL context: {}", SDL_GetError());
//        return 1;
//    }
//
//    GladGLContext gl{};
//    auto glad_version = gladLoadGLContext(&gl, (GLADloadfunc) SDL_GL_GetProcAddress);
//    fmt::println("Using Glad v{}.{}",
//                 GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));
//
//    SDL_ShowWindow(w.handle());
//
//    bool running = true;
//    do {
//        SDL_Event e;
//        while (SDL_PollEvent(&e)) {
//            switch (e.type) {
//                case SDL_EVENT_QUIT:
//                    running = false;
//                    break;
//            }
//        }
//
//        gl.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        SDL_GL_SwapWindow(w.handle());
//    } while (running);
//
//    SDL_GL_DestroyContext(sdl_gl_ctx);
//
//    return 0;
}
