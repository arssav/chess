#include "engine/notation_parser.h"

#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include <memory>
#include <unordered_map>

#include "engine/game_engine.h"
#include "engine/move.h"
#include "engine/piece.h"

namespace {

const std::unordered_map<char, Kind> NOTATION_TO_KIND = {
    {'K', Kind::KING},   {'Q', Kind::QUEEN}, {'B', Kind::BISHOP},
    {'N', Kind::KNIGHT}, {'R', Kind::ROOK},
};

// Convenience function for constructing error statuses.
// template <typename... Args>
// absl::Status InvalidArgumentFormat(constexpr std::string& format_string,
//                                    const Args&... args) {
//   return absl::InvalidArgumentError(absl::StrFormat(format_string, args...));
// }

absl::StatusOr<Kind> TryParsingKind(std::string* notation) {
  if (notation == nullptr || notation->empty()) {
    return absl::InvalidArgumentError("cannot parse kind from empty notation");
  }
  const auto maybe_notation_and_kind = NOTATION_TO_KIND.find((*notation)[0]);
  if (maybe_notation_and_kind != NOTATION_TO_KIND.end()) {
    *notation = notation->substr(1);
  }
  return maybe_notation_and_kind == NOTATION_TO_KIND.end()
             ? Kind::PAWN
             : maybe_notation_and_kind->second;
}

absl::StatusOr<int> TryParsingFile(std::string* notation) {
  if (notation == nullptr || notation->empty()) {
    return absl::InvalidArgumentError("cannot parse file from empty notation");
  }
  const char first_character = (*notation)[0];
  if (first_character < 'a' || first_character > 'h') {
    return absl::InvalidArgumentError(
        absl::StrFormat("Invalid square file: \'%c\'", first_character));
  }
  *notation = notation->substr(1);
  return first_character - 'a';
}

absl::StatusOr<int> TryParsingRank(std::string* notation) {
  if (notation == nullptr || notation->empty()) {
    return absl::InvalidArgumentError("cannot parse file from empty notation");
  }
  const char first_character = (*notation)[0];
  if (first_character < '1' || first_character > '8') {
    return absl::InvalidArgumentError(
        absl::StrFormat("Invalid square rank: \'%c\'", first_character));
  }
  *notation = notation->substr(1);
  return first_character - '1';
}

bool TryParsingCapture(std::string* notation) {
  if (notation == nullptr || notation->empty()) {
    return false;
  }
  if ((*notation)[0] == 'x' || (*notation)[0] == ':') {
    *notation = notation->substr(1);
    return true;
  }
  return false;
}

absl::StatusOr<std::pair<int, int>> TryParsingSquare(std::string* notation) {
  if (notation == nullptr) {
    return absl::InvalidArgumentError(
        "cannot parse square from empty notation");
  }
  if (notation->length() < 2) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "cannot parse square from notation: \"%s\"", *notation));
  }
  // Make a copy of notation to prevent modifications before we're sure the
  // square can be parsed.
  std::unique_ptr<std::string> notation_copy =
      std::make_unique<std::string>(*notation);
  absl::StatusOr<int> file_or = TryParsingFile(notation_copy.get());
  absl::StatusOr<int> rank_or = TryParsingRank(notation_copy.get());
  if (file_or.ok() && rank_or.ok()) {
    *notation = notation->substr(2);
    return std::make_pair(*file_or, *rank_or);
  }
  return absl::InvalidArgumentError(
      absl::StrFormat("cannot parse square from notation: \"%s\"", *notation));
}

} // namespace

absl::StatusOr<Move>
ParseAlgebraicNotation(const std::string& original_notation, Color color,
                       const Position& position) {
  std::string notation = original_notation;
  absl::StatusOr<Kind> kind_or = TryParsingKind(&notation);
  if (!kind_or.ok()) {
    return kind_or.status();
  }

  Kind kind = *kind_or;
  const std::vector<std::pair<int, int>> possible_initial_squares =
      position.FindPieces(Piece(kind, color));
  if (possible_initial_squares.empty()) {
    // TODO: print kind and color nicely.
    return absl::InvalidArgumentError(
        absl::StrFormat("No pieces found: %d, %d", kind, color));
  }

  // These might either disambiguate source square or denote destination square.
  absl::StatusOr<int> file_or = TryParsingFile(&notation);
  absl::StatusOr<int> rank_or = TryParsingRank(&notation);

  bool is_a_capture = TryParsingCapture(&notation);

  absl::StatusOr<std::pair<int, int>> square_or = TryParsingSquare(&notation);
  if (!square_or.ok()) {
    if (file_or.ok() && rank_or.ok()) {
      square_or = std::make_pair(*file_or, *rank_or);
      file_or = absl::UnavailableError("File uninitialized");
      rank_or = absl::UnavailableError("Rank uninitialized");
    } else {
      return square_or.status();
    }
  }

  std::vector<Move> possible_moves;
  for (const std::pair<int, int>& initial_square : possible_initial_squares) {
    Move move = Move(&position, initial_square.first, initial_square.second,
                     square_or->first, square_or->second);
    if (MoveIsValid(position, move)) {
      if (file_or.ok() && move.From().first != *file_or) {
        continue;
      }
      if (rank_or.ok() && move.From().second != *rank_or) {
        continue;
      }
      possible_moves.push_back(move);
    }
  }
  // TODO: print position to the status message.
  if (possible_moves.empty()) {
    return absl::InvalidArgumentError(
        absl::StrFormat("No legal moves could be found for notation \"%s\"",
                        original_notation));
  }
  if (possible_moves.size() > 1) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Multiple legal moves found for notation \"%s\"", original_notation));
  }

  if (is_a_capture && !possible_moves[0].IsACapture()) {
    return absl::InvalidArgumentError(absl::StrFormat(
        "Notation \"%s\" is a capture, but parsed move \"%s\" is not",
        original_notation, possible_moves[0].ToAlgebraicNotation()));
  }

  if (!notation.empty()) {
    return absl::InvalidArgumentError(
        absl::StrFormat("Unparsed notation left: \"%s\" from parsing \"%s\"",
                        notation, original_notation));
  }

  return possible_moves[0];
}
