#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <memory>

#include "BoardGame.h"
#include "BoardGameAI.h"

//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;
//The window renderer
SDL_Renderer* gRenderer = nullptr;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == nullptr )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == nullptr )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

void close()
{
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        std::shared_ptr<BoardGame> game(new BoardGame);
        std::shared_ptr<BoardRenderer> renderer(new BoardRenderer(game.get(), gRenderer));
        std::shared_ptr<BoardGameAI> ai(new BoardGameAI(game.get()));

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        //While application is running
        while( !quit )
        {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN)
                {
                    //Select surfaces based on key press
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_UP:
                            game->moveSelected({0, -1});
                            break;

                        case SDLK_DOWN:
                            game->moveSelected({0, 1});
                            break;

                        case SDLK_LEFT:
                            game->moveSelected({-1, 0});
                            break;

                        case SDLK_RIGHT:
                            game->moveSelected({1, 0});
                            break;

                        case SDLK_w:
                            if (game->makeMove({game->selectedField(), game->selectedField() + Position({0, -1})}))
                            {
                                game->moveSelected({0, -1});
                                ai->act();
                            }
                            break;

                        case SDLK_s:
                            if (game->makeMove({game->selectedField(), game->selectedField() + Position({0, 1})}))
                            {
                                game->moveSelected({0, 1});
                                ai->act();
                            }
                            break;

                        case SDLK_a:
                            if (game->makeMove({game->selectedField(), game->selectedField() + Position({-1, 0})}))
                            {
                                game->moveSelected({-1, 0});
                                ai->act();
                            }
                            break;

                        case SDLK_d:
                            if (game->makeMove({game->selectedField(), game->selectedField() + Position({1, 0})}))
                            {
                                game->moveSelected({1, 0});
                                ai->act();
                            }
                            break;
                        case SDLK_r:
                            game->resetGame();
                            break;
                    }
                }
                else if (e.type == SDL_MOUSEMOTION)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    game->setHovered(renderer->squareAt(x, y));
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    game->setDragged(renderer->squareAt(x, y));
                }
                else if (e.type == SDL_MOUSEBUTTONUP)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (game->makeMove({game->draggedField(), renderer->squareAt(x, y)}))
                    {
                        ai->act();
                    }
                    game->setDragged({-1, -1});
                }
            }

            renderer->render();
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}