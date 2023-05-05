//
// Created by doublekir on 5/4/23.
//

#ifndef SDLGAMETEST_BOARDGAME_H
#define SDLGAMETEST_BOARDGAME_H

#include <SDL.h>
#include <string>

//! Possible states of a single board square
enum class SquareState
{
    EMPTY,
    WHITE_PAWN,
    BLACK_PAWN
};


//! Field position
struct Position
{
    int x = 0;
    int y = 0;

    //! Checks if position belongs to the 8x8 board
    bool valid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }
    //! Destination of a step in an arbitrary direction
    //! Example: pos += {1, 0} is a step to the right
    Position operator+(const Position &diff) const { return {x + diff.x, y + diff.y}; }
    void operator+=(const Position &diff) { x += diff.x; y += diff.y; }
};


//! Board game state
class BoardGame
{
    friend class BoardRenderer;

    SquareState _board[8][8] = {SquareState::EMPTY};
    Position _selectedField = {7, 7};
    bool _drawSelection = false;

public:
    BoardGame();
    bool moveSelected(const Position &diff);
};


//! Board game renderer
class BoardRenderer
{
    BoardGame *_game; // initialized in constructor
    SDL_Renderer *_renderer; // initialized in constructor
    SDL_Texture* _board = nullptr;
    SDL_Texture* _white = nullptr;
    SDL_Texture* _black = nullptr;
    SDL_Texture* _active = nullptr;

    SDL_Texture* loadTexture( std::string path );
public:
    BoardRenderer(BoardGame *game, SDL_Renderer *renderer);
    bool render();
};

#endif //SDLGAMETEST_BOARDGAME_H
