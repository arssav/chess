#ifndef ENGINE_POSITION_H_
#define ENGINE_POSITION_H_

#include <string>
#include <vector>

#include "engine/base.h"
#include "engine/piece.h"

enum File { A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7 };

// Represents square horizontal, added for readability reasons, as chess
// algebraic notation is 1-based.
enum Rank {
  ONE = 0,
  TWO = 1,
  THREE = 2,
  FOUR = 3,
  FIVE = 4,
  SIX = 5,
  SEVEN = 6,
  EIGHT = 7
};

class Position {
 public:
  Position();

  bool HasPiece(int x, int y) const;
  bool HasPiece(const Square& square) const;
  Piece GetPiece(int x, int y) const;
  Piece GetPiece(const Square& square) const;
  void AddPiece(const Piece& piece, int x, int y);
  void AddPiece(const Piece& piece, const Square& square);
  void RemovePiece(int x, int y);
  void RemovePiece(const Square& square);

  // Returns false if the King of corresponding color has moved, or if the
  // correesponding rook has moved. Doesn't check if king or any of the squares
  // on the castling routes is under check.
  bool ShortCastlingPossible(Color color) const;
  bool LongCastlingPossible(Color color) const;

  std::vector<Square> FindPieces(const Piece& piece) const;

  // Not passing a Move object to avoid circular dependencies, as Move stores a
  // pointer to its position.
  void MakeMove(const Square& from, const Square& to);

  std::string ToString() const;

 private:
  // Stores all the board cells state, flattened into a vector.
  // TODO: Add the necessary fields for turn validity evaluation:
  //   * Whether the castling is possible (kings and rooks haven't moved)
  //   * Whether a pawn moved two squares last turn (for en passant)
  std::vector<Piece> cells_;

  // A single byte of data storing all the necessary bits required to get
  // whether each one of 4 castling kinds is possible.
  char castling_bits_ = 0;
};

Position StartingPosition();

#endif // ENGINE_POSITION_H_