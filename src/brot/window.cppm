module;

#include "fmt/core.h"
#include "SDL3/SDL.h"
#include <atomic>

export module brot.window;

export namespace brot {
inline const std::string window_tag = "brot::window";

class window {
public:
    window();
    ~window();

    window(const window &) = delete;
    window &operator=(const window &) = delete;

    window(window &&) noexcept = delete;
    window &operator=(window &&) = delete;

    [[nodiscard]] SDL_Window *handle() const;

private:
    SDL_Window *sdl_handle_{nullptr};

    void initialize_sdl_();

    void open_();

    static std::atomic_bool is_sdl_initialized_;
    static std::atomic_size_t open_window_count_;
};
} // namespace brot

std::atomic_bool brot::window::is_sdl_initialized_;
std::atomic_size_t brot::window::open_window_count_;

brot::window::window() {
    if (!is_sdl_initialized_) {
        initialize_sdl_();
    }
    open_();
}

brot::window::~window() {
    SDL_DestroyWindow(sdl_handle_);
    open_window_count_--;

    if (open_window_count_ == 0) {
        SDL_Quit();
        is_sdl_initialized_ = false;
    }
}

SDL_Window *brot::window::handle() const {
    return sdl_handle_;
}

void brot::window::initialize_sdl_() {
    auto sdl_version = SDL_GetVersion();
    fmt::println("Using SDL v{}.{}.{}",
                 SDL_VERSIONNUM_MAJOR(sdl_version),
                 SDL_VERSIONNUM_MINOR(sdl_version),
                 SDL_VERSIONNUM_MICRO(sdl_version));

    if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        throw std::runtime_error(fmt::format("Failed to initialize SDL video subsystem: {}", SDL_GetError()));
    }
}

void brot::window::open_() {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    sdl_handle_ = SDL_CreateWindow("Indev", 512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!sdl_handle_) {
        throw std::runtime_error(fmt::format("Failed to create SDL window: {}", SDL_GetError()));
    }
    SDL_SetWindowPosition(sdl_handle_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    open_window_count_++;
}
