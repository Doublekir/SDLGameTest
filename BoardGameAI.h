//
// Created by doublekir on 5/7/23.
//

#ifndef SDLGAMETEST_BOARDGAMEAI_H
#define SDLGAMETEST_BOARDGAMEAI_H

#include "BoardGame.h"

#include <set>
#include <vector>
#include <queue>

class BoardGameAI {
    BoardGame *_game;
    //! List of destination squares in order of priority
    std::vector<Position> _destPriority;

    //! Board search configurations
    enum class SearchMode
    {
        ACCESSIBLE, //! Search for any accessible squares
        NEXT_MOVE, //! Search for any black pawns
        PAWN_CAN_MOVE, //! Search for black pawns that can move down or to the right
        IGNORE_WHITE //! Search for black pawns, skipping white ones
    };
    //! Breadth-first search uses queue and depth-first uses stack
    template<class Container>
    std::pair<Move, std::set<Position> > search(const Position &src, SearchMode mode) const;

    //! Breadth-first search starting from src square.
    //! Returns suggested move and a list of accessible squares
    inline std::pair<Move, std::set<Position> > breadthFirstSearch(const Position &src, SearchMode mode)
    {
        return search<std::queue<Position> >(src, mode);
    }
    Move getNextMove();
public:
    explicit BoardGameAI(BoardGame *game);
    bool act() { return _game->makeMove(getNextMove()); }
};


#endif //SDLGAMETEST_BOARDGAMEAI_H
