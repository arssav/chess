#include "chess/move.h"

#include <unordered_map>

#include "absl/strings/str_format.h"

namespace {
static const std::unordered_map<Kind, std::string> PIECE_NOTATIONS = {
    {Kind::KING, "K"},   {Kind::QUEEN, "Q"}, {Kind::BISHOP, "B"},
    {Kind::KNIGHT, "N"}, {Kind::ROOK, "R"},  {Kind::PAWN, ""},
};

char GetFile(int x) { return 'a' + x; }

char GetRank(int y) { return '0' + y + 1; }

} // namespace

bool Move::operator==(const Move& other) const {
  return from_x_ == other.from_x_ && from_y_ == other.from_y_ &&
         to_x_ == other.to_x_ && to_y_ == other.to_y_;
}

std::string Move::ToAlgebraicNotation() const {
  std::string piece_notation =
      position_ == nullptr
          ? "?"
          : PIECE_NOTATIONS.at(position_->GetPiece(from_x_, from_y_).Kind());
  return absl::StrFormat("%s%c%c-%c%c", piece_notation, GetFile(from_x_),
                         GetRank(from_y_), GetFile(to_x_), GetRank(to_y_));
}

std::ostream& operator<<(std::ostream& out, const Move& move) {
  out << move.ToAlgebraicNotation();
  return out;
}

std::string Move::ToLongAlgebraicNotation() const { return "UNIMPLEMENTED"; }
