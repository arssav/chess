#ifndef ENGINE_POSITION_H_
#define ENGINE_POSITION_H_

#include <vector>

#include "engine/piece.h"

static constexpr int BOARD_SIZE = 8;

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
  Piece GetPiece(int x, int y) const;
  void AddPiece(const Piece& piece, int x, int y);
  void RemovePiece(int x, int y);

  std::vector<std::pair<int, int>> FindPieces(const Piece& piece) const;

  // Not passing a Move object to avoid circular dependencies, as Move stores a
  // pointer to its position.
  void MakeMove(const std::pair<int, int>& from, const std::pair<int, int>& to);

 private:
  // Stores all the board cells state, flattened into a vector.
  // TODO: Add the necessary fields for turn validity evaluation:
  //   * Whether the castling is possible (kings and rooks haven't moved)
  //   * Whether a pawn moved two squares last turn (for en passant)
  std::vector<Piece> cells_;
};

Position StartingPosition();

#endif // ENGINE_POSITION_H_