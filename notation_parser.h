#ifndef CHESS_NOTATION_PARSER_H_
#define CHESS_NOTATION_PARSER_H_

#include "absl/status/statusor.h"
#include <string>

#include "chess/move.h"
#include "chess/position.h"

absl::StatusOr<Move>
ParseAlgebraicNotation(const std::string& original_notation,
                       const Position& position);

#endif // CHESS_NOTATION_PARSER_H_
