//
// Created by doublekir on 5/4/23.
//

#ifndef SDLGAMETEST_BOARDGAME_H
#define SDLGAMETEST_BOARDGAME_H

#include <SDL.h>
#include <string>

//! Possible states of a board square
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
    //! Comparison operator for std::set
    bool operator<(const Position &cmp) const { return x * 8 + y < cmp.x * 8 + cmp.y; }
    //! Comparison operator for std::find
    bool operator==(const Position &cmp) const { return x == cmp.x && y == cmp.y; }
};

using Move = std::pair<Position, Position>;

//! Board game state
class BoardGame
{
    friend class BoardRenderer;

    SquareState _board[8][8] = {SquareState::EMPTY};
    Position _selectedField = {7, 7};
    Position _draggedField = {-1, -1};
    bool _drawSelection = false;
    bool _dragged = false;
    SquareState _turnOrder = SquareState::WHITE_PAWN;

    bool isGameOverWhite() const;
    bool isGameOverBlack() const;
public:

    BoardGame();
    void resetGame();
    SquareState at(const Position &pos) const { return _board[pos.x][pos.y]; }
    Position selectedField() const  { return _selectedField; }
    Position draggedField() const { return _draggedField; }
    //! Selected square movement (for keyboard/gamepad)
    bool moveSelected(const Position &diff);
    //! Selected square movement (for mouse controls and drag&drop)
    void setHovered(const Position &diff);
    bool setDragged(const Position &pos);
    bool makeMove(const Move &move);
};


//! Board game renderer
class BoardRenderer
{
    BoardGame *_game; // initialized in constructor
    SDL_Renderer *_renderer; // initialized in constructor
    SDL_Texture* _boardTexture = nullptr;
    SDL_Texture* _white = nullptr;
    SDL_Texture* _black = nullptr;
    SDL_Texture* _active = nullptr;

    SDL_Texture* loadTexture( std::string path );
public:
    BoardRenderer(BoardGame *game, SDL_Renderer *renderer);
    void render();
    Position squareAt(const int &x, const int &y) const { return {x / 60, y / 60}; }
};

#endif //SDLGAMETEST_BOARDGAME_H
