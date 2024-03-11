#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
} App;

static class GraphicsInterface
{
private:
    // A struct that holds the main graphical data for the game window.
    static App app;

public:
    // A boolean to determine if the graphics thread should terminate.
    static bool terminateGraphicsThread;

    /**
     * @brief Initializes the SDL library and sets the main game window data.
     */
    static void InitSDL(void);

    /**
     * @brief Handles input for the main game window.
     *
     * Currently only handles when the user presses the exit "X" on the game window
     */
    static void DoInput(void);

    /**
     * @brief Clears the renderer and displays the main background colour.
     */
    static void PrepareScene(void);

    /**
     * @brief Presents the scene.
     */
    static void PresentScene(void);

    /**
     * @brief Loads an image from the project files.
     *
     * @param filename The path of the image to load.
     */
    static SDL_Surface* LoadImage(const char* filename);

    /**
     * @brief Creates a texture.
     *
     * @param filename The path of the image to load.
     */
    static SDL_Texture* CreateTexture(const char* filename);

    /**
     * @brief Creates a text texture.
     *
     * @param font The font to display the text in.
     * @param text The text to display.
     */
    static SDL_Texture* CreateText(TTF_Font* font, const char* text);

    /**
     * @brief Passes a texture to the renderer to be displayed.
     *
     * @param texture The texture to display on screen.
     * @param x The X coordinate of the texture on screen.
     * @param y The Y coordinate of the texture on screen.
     */
    static void Blit(SDL_Texture* texture, int x, int y);

    /**
     * @brief Handles the main game window graphics.
     *
     * Creates the game textures, initializes library components,
     * and runs all of the SDL specific methods when needed.
     */
    static void GraphicsHandler();
};