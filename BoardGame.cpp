//
// Created by doublekir on 5/4/23.
//

#include "BoardGame.h"
#include <SDL_image.h>

BoardGame::BoardGame()
{
    resetGame();
}

bool BoardGame::moveSelected(const Position &diff) {
    if (!_drawSelection)
    {
        _drawSelection = true;
        return true;
    }
    if ((_selectedField + diff).valid())
    {
        _selectedField += diff;
        return true;
    }
    return false;
}

void BoardGame::setHovered(const Position &diff)
{
    _drawSelection = true;
    _selectedField = diff;
}

bool BoardGame::setDragged(const Position &pos) {
    if (!pos.valid())
    {
        _dragged = false;
        return false;
    }
    if (_board[pos.x][pos.y] == _turnOrder)
    {
        _draggedField = pos;
        _dragged = true;
        return true;
    }
    return false;
}

bool BoardGame::makeMove(const Move &move) {
    auto from = move.first, to = move.second;
    if (!from.valid() || !to.valid())
        return false;
    //! Move can only be made 1 step at a time horizontally or vertically
    if (abs(from.x - to.x) + abs(from.y - to.y) != 1)
        return false;
    if (_board[from.x][from.y] == _turnOrder && _board[to.x][to.y] == SquareState::EMPTY)
    {
        _board[to.x][to.y] = _board[from.x][from.y];
        _board[from.x][from.y] = SquareState::EMPTY;
        _turnOrder = _turnOrder == SquareState::BLACK_PAWN ? SquareState::WHITE_PAWN : SquareState::BLACK_PAWN;
        return true;
    }
    return false;
}

bool BoardGame::isGameOverWhite() const
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (_board[i][j] != SquareState::WHITE_PAWN)
                return false;
        }
    }
    return true;
}

bool BoardGame::isGameOverBlack() const
{
    for (int i = 5; i < 8; ++i)
    {
        for (int j = 5; j < 8; ++j)
        {
            if (_board[i][j] != SquareState::BLACK_PAWN)
                return false;
        }
    }
    return true;
}

void BoardGame::resetGame() {
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (i < 3 && j < 3)
                _board[i][j] = SquareState::BLACK_PAWN;
            else if (i > 4 && j > 4)
                _board[i][j] = SquareState::WHITE_PAWN;
            else
                _board[i][j] = SquareState::EMPTY;
        }
    }
    _turnOrder = SquareState::WHITE_PAWN;
    _draggedField = {-1, -1};
    _drawSelection = false;
    _dragged = false;
}

BoardRenderer::BoardRenderer(BoardGame *game, SDL_Renderer *renderer) :
    _game(game),
    _renderer(renderer)
{
    _boardTexture = loadTexture("chessboard.png");
    _black = loadTexture("blackpawn.png");
    _white = loadTexture("whitepawn.png");
    _active = loadTexture("border.png");
}

void BoardRenderer::render()
{
    SDL_RenderClear(_renderer);
    // Render board texture to screen
    SDL_RenderCopy(_renderer, _boardTexture, nullptr, nullptr);
    // Render pieces
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            SDL_Rect rect = {i * 60, j * 60, 60, 60};
            switch(_game->_board[i][j])
            {
                case SquareState::BLACK_PAWN:
                    SDL_RenderCopy(_renderer, _black, nullptr, &rect);
                    break;
                case SquareState::WHITE_PAWN:
                    SDL_RenderCopy(_renderer, _white, nullptr, &rect);
                    break;
            }
        }
    }
    if (_game->_drawSelection)
    {
        SDL_Rect rect = {_game->_selectedField.x * 60, _game->_selectedField.y * 60, 60, 60};
        SDL_RenderCopy(_renderer, _active, nullptr, &rect);
    }
    if (_game->_dragged)
    {
        SDL_Rect rect = {_game->_selectedField.x * 60, _game->_selectedField.y * 60, 60, 60};
        if (_game->_turnOrder == SquareState::WHITE_PAWN)
        {
            SDL_SetTextureAlphaMod(_white, 100);
            SDL_RenderCopy(_renderer, _white, nullptr, &rect);
            SDL_SetTextureAlphaMod(_white, 255);
        }
        else
        {
            SDL_SetTextureAlphaMod(_black, 100);
            SDL_RenderCopy(_renderer, _black, nullptr, &rect);
            SDL_SetTextureAlphaMod(_black, 255);
        }
    }
    // Update screen
    SDL_RenderPresent(_renderer);
}

SDL_Texture *BoardRenderer::loadTexture(std::string path)
{
    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
        if(newTexture == nullptr)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
