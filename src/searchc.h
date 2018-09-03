// Copyright 2006-2008, 2016-2018 by Jon Dart. All Rights Reserved.

#ifndef _SEARCHC_H
#define _SEARCHC_H

#include "board.h"
#include "constant.h"
#include "chess.h"

#include <array>

struct NodeInfo;
class Board;

class SearchContext {
public:
    SearchContext();

    virtual ~SearchContext();

    void clear();

    void clearKiller();

    void setKiller(const Move & move,unsigned ply)
    {
        if (!MovesEqual(move,killers1[ply])) {
           killers2[ply] = killers1[ply];
        }
        killers1[ply] = move;
    }

    void getKillers(unsigned ply,Move &k1,Move &k2) const noexcept
    {
        k1 = killers1[ply]; k2 = killers2[ply];
    }

    int scoreForOrdering (Move m, NodeInfo *, ColorType side) const noexcept;

    void updateStats(const Board &, NodeInfo *parentNode);

    static const int HISTORY_MAX;

    template<class T>
    using PieceToArray = std::array<std::array<T, 64>, 16>;

    template<class T>
    using PieceTypeToArray = std::array<std::array<T, 64>, 8>;

    template<class T>
    using PieceTypeToMatrix = PieceTypeToArray< PieceTypeToArray<T> >;

    template<class T>
    using ButterflyArray = std::array<std::array<std::array<T, 64>, 64>, 2>;

    Move getCounterMove(const Board &board, Move prev) const {
        ColorType oside = board.oppositeSide();
        return (*counterMoves)[MakePiece(PieceMoved(prev),oside)][DestSquare(prev)];
    }

    void setCounterMove(const Board &board, Move prev, Move counter) {
        ColorType oside = board.oppositeSide();
        (*counterMoves)[MakePiece(PieceMoved(prev),oside)][DestSquare(prev)] = counter;
    }

private:

    Move killers1[Constants::MaxPly];
    Move killers2[Constants::MaxPly];

    ButterflyArray<int> *history;

    PieceToArray<Move> *counterMoves;

    PieceTypeToMatrix<int> *counterMoveHistory, *fuMoveHistory;

    int bonus(int depth) const noexcept;

    void update(int &val, int bonus, int divisor);
};

#endif
