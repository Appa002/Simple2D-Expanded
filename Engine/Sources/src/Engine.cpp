#include "../header/Engine.h"
#include "../header/types.h"

namespace Globals {
    GLuint shader_programme;
    SDL_Window* window;
}

struct DpiInfo {
    float dpi;
    float defaultDpi;
};


DpiInfo getDisplayDpi(int displayIndex = 0) {
    const float kSysDefaultDpi =
#ifdef __APPLE__
        72.0f;
#elif defined(_WIN32)
        96.0f;
#else
        0;
    static_assert(false, "No system default DPI set for this platform.");
#endif

    DpiInfo info;

    if (SDL_GetDisplayDPI(displayIndex, NULL, &info.dpi, NULL) != 0) {
        // Failed to get DPI, so just return the default value.
        info.dpi = kSysDefaultDpi;
    }
    info.defaultDpi = kSysDefaultDpi;
    return info;
}


Simple2D::vec2 getScaledWindowSize(int resX, int resY, DpiInfo info) {
    Simple2D::vec2 scaledSize;
    scaledSize.x = resX * info.dpi / info.defaultDpi;
    scaledSize.y = resY * info.dpi / info.defaultDpi;
    return scaledSize;
}

int Simple2D::destroyWindow()
{
    SDL_DestroyWindow(Globals::window);
    SDL_Quit();
    return 0;
}

int Simple2D::createWindow(std::string name, int resX, int resY)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    vec2 windowSize = getScaledWindowSize(resX, resY, getDisplayDpi());

    Globals::window = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowSize.x,
        windowSize.y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_GLContext context = SDL_GL_CreateContext(Globals::window);
    SDL_SetWindowResizable(Globals::window, SDL_TRUE);
    SDL_GL_MakeCurrent(Globals::window, context);

    glewExperimental = GL_TRUE;
    glewInit();
    SDL_GL_SetSwapInterval(1);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    printf("Renderer: %s\n", renderer);
    printf("OpenGL Version: %s\n", version);
    std::cout << std::endl;

    glEnable(GL_DEPTH);
    glDepthFunc(GL_LESS);

    glClearColor(0.3f, 0.3f, 0.3f, 0);

    Simple2D::GameObjectRenderer::get()->setupOpenglBuffer();
    Simple2D::GameObjectRenderer::get()->setupShaders();
    // Simple2D::GameObjectRenderer::get()->setShaderProgramme(Globals::shader_programme);
    Simple2D::UiManager::get()->init();
    return 0;
}

int Simple2D::startMainLoop()
{

    SDL_Event event;
    bool bQuit = false;
    while (!bQuit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                if (Simple2D::MapManager::get()->getCurrentMap())
                    Simple2D::MapManager::get()->unloadMap();
                bQuit = true;
            }
            if (!bQuit && Simple2D::MapManager::get()->getCurrentMap())
                Simple2D::MapManager::get()->getCurrentMap()->eventHandelAll(event);

        }

        if (!bQuit) {

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(Globals::shader_programme);

            if (Simple2D::MapManager::get()->getCurrentMap()) {
                Simple2D::MapManager::get()->getCurrentMap()->fixedUpdateAll();
                Simple2D::MapManager::get()->getCurrentMap()->updateAll();
                Simple2D::MapManager::get()->getCurrentMap()->renderAll();
                Simple2D::UiManager::get()->renderAll();
            }

            SDL_GL_SwapWindow(Globals::window);
        }
    }
    return 0;

}
