#ifndef ENGINE_NOTATION_PARSER_H_
#define ENGINE_NOTATION_PARSER_H_

#include "absl/status/statusor.h"
#include <string>

#include "engine/move.h"
#include "engine/position.h"

absl::StatusOr<Move>
ParseAlgebraicNotation(const std::string& original_notation, Color color,
                       const Position& position);

#endif // ENGINE_NOTATION_PARSER_H_
