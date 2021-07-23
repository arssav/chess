#ifndef ENGINE_GAME_ENGINE_H_
#define ENGINE_GAME_ENGINE_H_

#include <unordered_set>
#include <vector>

#include "engine/base.h"
#include "engine/move.h"
#include "engine/position.h"

bool MoveIsValid(const Position& position, const Move& move);

// Returns a vector of possible moves for a piece at a given position.
// Includes moves taking an enemy king, as these are only possible after a
// check-mate.
// TODO: Disallow moves which put the King under check, namely taking protected
// enemy pieces and moving the pieces which protect the king.
std::vector<Move> GenerateMovesForAPiece(const Position& position, int x,
                                         int y);

// Includes squares under attack from any piece of a given color.
// TODO: Also include pieces of attacking_color protected by other pieces. That
// would disallow the king to take protected pieces.
std::unordered_set<Square> GetSquaresUnderAttack(const Position& position,
                                                 Color attacking_color);

#endif // ENGINE_GAME_ENGINE_H_