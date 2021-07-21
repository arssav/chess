#ifndef ENGINE_GAME_ENGINE_H_
#define ENGINE_GAME_ENGINE_H_

#include <vector>

#include "engine/move.h"
#include "engine/position.h"

bool MoveIsValid(const Position& position, const Move& move);

// Returns a vector of possible moves for a piece at a given position.
// Includes moves taking an enemy king, as these are only possible after a
// check-mate.
std::vector<Move> GenerateMovesForAPiece(const Position& position, int x,
                                         int y);

#endif // ENGINE_GAME_ENGINE_H_