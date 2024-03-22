#include "GraphicsInterface.h"

#include <iostream>

App GraphicsInterface::app;
bool GraphicsInterface::terminateGraphicsThread = false;

void GraphicsInterface::InitSDL(void) {
    // Window size
    int windowWidth = 960;
    int windowHeight = 540;

    int rendererFlags = SDL_RENDERER_ACCELERATED;

    app.window = SDL_CreateWindow("Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0
    );

    // Error check
    if (!app.window) {
        printf("Failed to open %d x %d window: %s\n", windowWidth, windowHeight, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    // Error check
    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void GraphicsInterface::DoInput(void)
{
    SDL_Event event;
    // Handles input events.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        default:
            break;
        }
    }
}

void GraphicsInterface::PrepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 51, 187, 255, 255);
    SDL_RenderClear(app.renderer);
}

void GraphicsInterface::PresentScene(void) {
    SDL_RenderPresent(app.renderer);
}

SDL_Surface* GraphicsInterface::LoadImage(const char* filename) {
    // Loads an image.
    SDL_Surface* image;
    image = IMG_Load(filename);
    if (image == NULL) {
        exit(1);
        return 0;
    }
    return image;
}

SDL_Texture* GraphicsInterface::CreateTexture(const char* filename) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, LoadImage(filename));
    return texture;
}

SDL_Texture* GraphicsInterface::CreateText(TTF_Font* font, const char* text) {
    // Creates text.
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    return texture;
}

void GraphicsInterface::Blit(SDL_Texture* texture, int x, int y)
{
    // Draws a texture on the screen.
    SDL_Rect dest{};

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void GraphicsInterface::GraphicsHandler() {
    memset(&app, 0, sizeof(App));

    // Initializing library components
    InitSDL();
    TTF_Init();

    // Font creation
    TTF_Font* font = TTF_OpenFont("EazyChat.ttf", 28);

    // Creating and loading game textures
    SDL_Texture* mapTexture = CreateTexture("map.png");

    SDL_Texture* northAmerica = CreateText(font, "North America");
    SDL_Texture* southAmerica = CreateText(font, "South America");
    SDL_Texture* europe = CreateText(font, "Europe");
    SDL_Texture* africa = CreateText(font, "Africa");
    SDL_Texture* aisa = CreateText(font, "Aisa");
    SDL_Texture* australia = CreateText(font, "Australia");

    // Main graphics loop
    while (!terminateGraphicsThread) {
        PrepareScene();

        DoInput();
        // Displays textures on screen
        Blit(mapTexture, 0, 0);
        Blit(northAmerica, 100, 30);
        Blit(southAmerica, 20, 400);
        Blit(europe, 295, 195);
        Blit(africa, 390, 400);
        Blit(aisa, 850, 235);
        Blit(australia, 650, 490);

        PresentScene();

        SDL_Delay(16);
    }

    // Frees up rendering components
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}