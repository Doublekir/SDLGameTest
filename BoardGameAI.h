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
    //! Game state
    BoardGame *_game;
    //! List of destination squares in order of priority
    std::vector<Position> _destPriority;
    //! List of start squares in order of leave priority
    std::vector<Position> _leavePriority;

    //! Board search configurations
    enum class SearchMode
    {
        ACCESSIBLE, //! Search for any accessible squares
        NEXT_MOVE, //! Search for any black pawns to take target square
        PAWN_CAN_MOVE, //! Search for black pawns that can move down or to the right
        IGNORE_WHITE //! Search for closest black pawns that can move, ignoring white ones
    };

    //! Breadth-first search uses queue and depth-first would use stack
    //! Returns suggested move and a list of squares accessible from src
    template<class Container>
    std::pair<Move, std::set<Position> > search(const Position &src, SearchMode mode) const;

    //! Breadth-first search starting from src square
    inline std::pair<Move, std::set<Position> > breadthFirstSearch(const Position &src, SearchMode mode)
    {
        return search<std::queue<Position> >(src, mode);
    }

    //! Move validation
    bool isLegal(const Move &move) const;
    //! Search for the best available move
    Move getNextMove();
public:
    explicit BoardGameAI(BoardGame *game);
    //! AI action
    bool act() { return _game->makeMove(getNextMove()); }
};


#endif //SDLGAMETEST_BOARDGAMEAI_H
