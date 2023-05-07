//
// Created by doublekir on 5/7/23.
//

#include "BoardGameAI.h"

#include <algorithm>

BoardGameAI::BoardGameAI(BoardGame *game) :
    _game(game),
    _destPriority{{7,7},{6,7},{7,6},{5,7},{6,6},{7,5},{5,6},{6,5},{5,5}}
{

}

template <class Container>
std::pair<Move, std::set<Position> > BoardGameAI::search(const Position &src, SearchMode mode) const {
    Container next;
    std::set<Position> checked;
    std::set<Position> accessible;
    if (mode == SearchMode::ACCESSIBLE)
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                Position pos{i, j};
                if (_game->at(pos) == SquareState::BLACK_PAWN)
                {
                    next.push(pos);
                    checked.insert(pos);
                }
            }
        }
    }
    else
    {
        next.push(src);
        checked.insert(src);
    }
    // Pawn for the case when neither pawn can step right nor down
    Move reserve = {{-1, -1}, {-1, -1}};

    Position directions[4] {{0,1},{1,0},{0,-1},{-1,0}}; // Down - right - up - left
    while (!next.empty())
    {
        auto pos = next.front();
        for(auto direction : directions)
        {
            auto neighbor = pos + direction;
            if (!neighbor.valid())
                continue;
            if(checked.count(neighbor))
                continue;
            if (_game->at(neighbor) == SquareState::EMPTY)
            {
                next.push(neighbor);
                checked.insert(neighbor);
                accessible.insert(neighbor);
            }
            else if (_game->at(neighbor) == SquareState::WHITE_PAWN)
            {
                if (mode == SearchMode::IGNORE_WHITE)
                    next.push(neighbor);
                checked.insert(neighbor);
            }
            else if (_game->at(neighbor) == SquareState::BLACK_PAWN)
            {
                if(mode == SearchMode::NEXT_MOVE)
                {
                    // Any black pawn not already in place fits
                    auto fromPriority = std::find(_destPriority.begin(), _destPriority.end(), neighbor);
                    auto toPriority = std::find(_destPriority.begin(), _destPriority.end(), src);
                    // If pawn is not already in place (fromPriority == end) or destination is first in priority list
                    if (toPriority < fromPriority)
                    {
                        return {{neighbor, pos}, accessible};
                    }
                    else
                    {
                        checked.insert(neighbor);
                    }
                }
                else if (mode == SearchMode::PAWN_CAN_MOVE || mode == SearchMode::IGNORE_WHITE)
                {
                    // Check if pawn can step right or down
                    auto down = neighbor + directions[0];
                    if (down.valid() && _game->at(down) == SquareState::EMPTY)
                        return {{neighbor, down}, accessible};
                    auto right = neighbor + directions[1];
                    if (right.valid() && _game->at(right) == SquareState::EMPTY)
                        return {{neighbor, right}, accessible};
                    // If pawn can step at all, it is reserved and the search continues
                    auto up = neighbor + directions[2];
                    if (up.valid() && _game->at(up) == SquareState::EMPTY)
                        reserve = {neighbor, up};
                    auto left = neighbor + directions[3];
                    if (left.valid() && _game->at(left) == SquareState::EMPTY)
                        reserve = {neighbor, left};
                    checked.insert(neighbor);
                    next.push(neighbor);
                }
                else if (mode == SearchMode::ACCESSIBLE)
                {
                    next.push(neighbor);
                    checked.insert(neighbor);
                    accessible.insert(neighbor);
                }
            }
        }
        next.pop();
    }

    return {reserve, accessible}; // Reserve is invalid when all paths are blocked by white pawns, checked in getNextTurn()
}

Move BoardGameAI::getNextMove() {
    //! Find black pawn closest to upper left square
    Move reserve = breadthFirstSearch({0, 0}, SearchMode::IGNORE_WHITE).first;
    std::set<Position> accessible = breadthFirstSearch({-1, -1}, SearchMode::ACCESSIBLE).second;
    Position prioritized = {-1, -1};
    for (auto pos : _destPriority)
    {
        if (accessible.count(pos))
        {
            prioritized = pos;
            auto move = breadthFirstSearch(prioritized, SearchMode::NEXT_MOVE).first;
            if (move.first.valid())
            {
                return move;
            }
            break;
        }
    }
    if (prioritized.valid())
    {
        return breadthFirstSearch(prioritized, SearchMode::NEXT_MOVE).first;
    }
    else
    {
        return reserve;
    }
}
