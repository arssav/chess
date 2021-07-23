#include "engine/position.h"

#include "engine/piece.h"

namespace {

static const size_t x_mask = (1 << BOARD_SIZE_LOG) - 1;
static const size_t y_mask = x_mask << BOARD_SIZE_LOG;

int GetFlattenedIndex(int x, int y) { return y << BOARD_SIZE_LOG | x; }

int GetX(int flattened_index) { return flattened_index & x_mask; }

int GetY(int flattened_index) {
  return (flattened_index & y_mask) >> BOARD_SIZE_LOG;
}

} // namespace

Position::Position() {
  cells_ = std::vector<Piece>(BOARD_SIZE * BOARD_SIZE,
                              Piece(Kind::NONE, Color::BLACK));
}

bool Position::HasPiece(int x, int y) const {
  return cells_[GetFlattenedIndex(x, y)].Kind() != Kind::NONE;
};

bool Position::HasPiece(const Square& square) const {
  return cells_[GetFlattenedIndex(square.file, square.rank)].Kind() !=
         Kind::NONE;
};

Piece Position::GetPiece(int x, int y) const {
  return cells_[GetFlattenedIndex(x, y)];
}

Piece Position::GetPiece(const Square& square) const {
  return cells_[GetFlattenedIndex(square.file, square.rank)];
}

void Position::AddPiece(const Piece& piece, int x, int y) {
  cells_[GetFlattenedIndex(x, y)] = piece;
}

void Position::AddPiece(const Piece& piece, const Square& square) {
  cells_[GetFlattenedIndex(square.file, square.rank)] = piece;
}

void Position::RemovePiece(int x, int y) {
  cells_[GetFlattenedIndex(x, y)] = Piece(Kind::NONE, Color::BLACK);
}

void Position::RemovePiece(const Square& square) {
  cells_[GetFlattenedIndex(square.file, square.rank)] =
      Piece(Kind::NONE, Color::BLACK);
}

namespace {
static constexpr char BLACK_KING_MOVED_MASK = 1;
static constexpr char WHITE_KING_MOVED_MASK = 1 << 1;
static constexpr char BLACK_ROOK_A_MOVED_MASK = 1 << 2;
static constexpr char BLACK_ROOK_H_MOVED_MASK = 1 << 3;
static constexpr char WHITE_ROOK_A_MOVED_MASK = 1 << 4;
static constexpr char WHITE_ROOK_H_MOVED_MASK = 1 << 5;
} // namespace

bool Position::ShortCastlingPossible(Color color) const {
  const char mask =
      (color == Color::BLACK ? BLACK_KING_MOVED_MASK | BLACK_ROOK_H_MOVED_MASK
                             : WHITE_KING_MOVED_MASK | WHITE_ROOK_H_MOVED_MASK);
  return (castling_bits_ & mask) == 0;
}

bool Position::LongCastlingPossible(Color color) const {
  const char mask =
      (color == Color::BLACK ? BLACK_KING_MOVED_MASK | BLACK_ROOK_A_MOVED_MASK
                             : WHITE_KING_MOVED_MASK | WHITE_ROOK_A_MOVED_MASK);
  return (castling_bits_ & mask) == 0;
}

std::vector<Square> Position::FindPieces(const Piece& piece) const {
  std::vector<Square> squares;
  for (int i = 0; i < cells_.size(); ++i) {
    if (cells_[i] == piece) {
      squares.push_back({GetX(i), GetY(i)});
    }
  }
  return squares;
}

void Position::MakeMove(const Square& from, const Square& to) {
  // TODO: Implement en passant.
  const Piece piece = GetPiece(from);
  // Castling
  if (piece.Kind() == Kind::KING) {
    castling_bits_ |= (piece.Color() == Color::WHITE ? WHITE_KING_MOVED_MASK
                                                     : BLACK_KING_MOVED_MASK);
  }
  if (piece.Kind() == Kind::ROOK && from.file == A) {
    castling_bits_ |= (piece.Color() == Color::WHITE ? WHITE_ROOK_A_MOVED_MASK
                                                     : BLACK_ROOK_A_MOVED_MASK);
  }
  if (piece.Kind() == Kind::ROOK && from.file == H) {
    castling_bits_ |= (piece.Color() == Color::WHITE ? WHITE_ROOK_H_MOVED_MASK
                                                     : BLACK_ROOK_H_MOVED_MASK);
  }
  if (piece.Kind() == Kind::KING && std::abs(from.file - to.file) > 1) {
    if (piece.Color() == Color::WHITE) {
      castling_bits_ |= WHITE_KING_MOVED_MASK;
      if (to.file == C) {
        castling_bits_ |= WHITE_ROOK_A_MOVED_MASK;
        RemovePiece({A, ONE});
        AddPiece(Piece{Kind::ROOK, Color::WHITE}, {D, ONE});
      } else {
        castling_bits_ |= WHITE_ROOK_H_MOVED_MASK;
        RemovePiece({H, ONE});
        AddPiece(Piece{Kind::ROOK, Color::WHITE}, {F, ONE});
      }
    } else {
      castling_bits_ |= BLACK_KING_MOVED_MASK;
      if (to.file == C) {
        castling_bits_ |= BLACK_ROOK_A_MOVED_MASK;
        RemovePiece({A, EIGHT});
        AddPiece(Piece{Kind::ROOK, Color::BLACK}, {D, EIGHT});
      } else {
        castling_bits_ |= BLACK_ROOK_H_MOVED_MASK;
        RemovePiece({H, EIGHT});
        AddPiece(Piece{Kind::ROOK, Color::BLACK}, {F, EIGHT});
      }
    }
  }

  AddPiece(piece, to);
  RemovePiece(from);
}

Position StartingPosition() {
  Position position;

  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), D, ONE);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), C, ONE);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), F, ONE);
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), B, ONE);
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), G, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, ONE);
  for (int x = 0; x < BOARD_SIZE; ++x) {
    position.AddPiece(Piece(Kind::PAWN, Color::WHITE), x, TWO);
  }

  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::QUEEN, Color::BLACK), D, EIGHT);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), C, EIGHT);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), F, EIGHT);
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), B, EIGHT);
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), G, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), H, EIGHT);
  for (int y = 0; y < BOARD_SIZE; ++y) {
    position.AddPiece(Piece(Kind::PAWN, Color::BLACK), y, SEVEN);
  }

  return position;
}