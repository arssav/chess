#include "chess/notation_parser.h"

#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include <memory>
#include <unordered_map>

#include "chess/piece.h"

namespace {

const std::unordered_map<char, Kind> NOTATION_TO_KIND = {
    {'K', Kind::KING},   {'Q', Kind::QUEEN}, {'B', Kind::BISHOP},
    {'N', Kind::KNIGHT}, {'R', Kind::ROOK},
};

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
ParseAlgebraicNotation(const std::string& original_notation,
                       const Position& position) {
  std::string notation = original_notation;
  absl::StatusOr<Kind> kind_or = TryParsingKind(&notation);
  if (!kind_or.ok()) {
    return kind_or.status();
  }

  absl::StatusOr<std::pair<int, int>> square_or = TryParsingSquare(&notation);

  if (square_or.ok()) {
    return Move(&position, 0, 0, square_or->first, square_or->second);
  }
  return absl::InvalidArgumentError(
      absl::StrFormat("cannot parse notation: \"%s\"", original_notation));
}
