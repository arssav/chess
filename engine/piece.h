#ifndef CHESS_PIECE_H_
#define CHESS_PIECE_H_

// Using scoped enums here to prevent implicit casting to int and name
// collisions.
enum class Color : char { BLACK, WHITE };
enum class Kind : char { NONE, PAWN, KING, QUEEN, BISHOP, KNIGHT, ROOK };

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

 private:
  enum ::Kind kind_;
  bool is_black_;
};

#endif // CHESS_PIECE_H_