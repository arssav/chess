#ifndef ENGINE_PIECE_H_
#define ENGINE_PIECE_H_

#include "engine/base.h"

// Represents a figure on a board.
class Piece {
 public:
  Piece(::Kind kind, ::Color color)
      : kind_(kind), is_black_(color == ::Color::BLACK){};

  enum ::Kind Kind() const { return kind_; };
  enum ::Color Color() const {
    return is_black_ ? ::Color::BLACK : ::Color::WHITE;
  };

  bool operator==(const Piece& rhs) const {
    return kind_ == rhs.kind_ && is_black_ == rhs.is_black_;
  }

  bool operator!=(const Piece& rhs) const { return !(*this == rhs); }

  // Represents a piece as a char, lowercase for black, uppercase for white.
  char ToChar() const;

 private:
  enum ::Kind kind_;
  bool is_black_;
};

#endif // ENGINE_PIECE_H_