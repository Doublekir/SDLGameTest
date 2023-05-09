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
    EMPTY, //! No pawn
    WHITE_PAWN, //! White pawn
    BLACK_PAWN //! Black pawn
};


//! Board position
struct Position
{
    int x = 0;
    int y = 0;

    //! Checks if position belongs to the 8x8 board
    bool valid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }
    //! Destination of a step in an arbitrary direction.
    //! Example: pos + {1, 0} is a step to the right
    Position operator+(const Position &diff) const { return {x + diff.x, y + diff.y}; }
    //! Step in an arbitrary direction
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

    //! Square status
    SquareState _board[8][8] = {SquareState::EMPTY};
    //! Square selected with keyboard or mouse
    Position _selectedField = {7, 7};
    //! Square being dragged with mouse
    Position _draggedField = {-1, -1};
    //! Status of field selection
    bool _drawSelection = false;
    //! Status of drag&drop
    bool _dragged = false;
    //! Player currently taking action
    SquareState _turnOrder = SquareState::WHITE_PAWN;

    //! Win condition for white pawns
    bool isGameOverWhite() const;
    //! Win condition for black pawns
    bool isGameOverBlack() const;
public:

    BoardGame();
    //! Game reset
    void resetGame();
    //! State of square at pos
    SquareState at(const Position &pos) const { return _board[pos.x][pos.y]; }
    //! Field selection getter
    Position selectedField() const  { return _selectedField; }
    //! Drag&drop position getter
    Position draggedField() const { return _draggedField; }
    //! Selected square movement (for keyboard/gamepad)
    bool moveSelected(const Position &diff);
    //! Selected square movement (for mouse controls and drag&drop)
    void setHovered(const Position &diff);
    //! Mouse drag initialization
    bool setDragged(const Position &pos);
    //!
    bool makeMove(const Move &move);
};


//! Board game renderer
class BoardRenderer
{
    //! Game state
    BoardGame *_game; // initialized in constructor
    //! SDL renderer
    SDL_Renderer *_renderer; // initialized in constructor
    //! Board texture
    SDL_Texture* _boardTexture = nullptr;
    //! White pawn texture
    SDL_Texture* _white = nullptr;
    //! Black pawn texture
    SDL_Texture* _black = nullptr;
    //! Active square border texture
    SDL_Texture* _active = nullptr;

    //! Load texture for hardware-accelerated rendering
    SDL_Texture* loadTexture( std::string path );
public:
    BoardRenderer(BoardGame *game, SDL_Renderer *renderer);
    //! Game graphics rendering
    void render();
    //! Position of board square represented by window pixel {x, y}
    Position squareAt(const int &x, const int &y) const;
};

#endif //SDLGAMETEST_BOARDGAME_H
