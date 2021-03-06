#include "engine/game_engine.h"

#include <cmath>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
// The following static methods return possible move directions for Bishop, Rook
// and Queen, and move deltas for the Knights.
// The underlying vectors are only initialized once thanks to Meyers singleton
// pattern: static variables with block scope are only initialized once and are
// thread-safe.
// We are using dynamic initialization instead of compile-time hardcoded list to
// avoid possible typos and to brag about our design pattern knowledge.
static const std::vector<std::pair<int, int>>& GetBishopMoveDirections() {
  // Mind the lambda: while a static variable initialization only happens once
  // when in a scoped context, any further modificatioins to said variable may
  // happen any time this function is called. By initializing at declaration
  // time, we ensure that the lambda below is only called once.
  static const std::vector<std::pair<int, int>> move_directions = []() {
    std::vector<std::pair<int, int>> move_directions;
    for (int dx = -1; dx <= 1; dx += 2) {
      for (int dy = -1; dy <= 1; dy += 2) {
        move_directions.push_back({dx, dy});
      }
    }
    return move_directions;
  }();
  return move_directions;
}

static const std::vector<std::pair<int, int>>& GetRookMoveDirections() {
  static const std::vector<std::pair<int, int>> move_directions = []() {
    std::vector<std::pair<int, int>> move_directions;
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (std::abs(dx) + std::abs(dy) == 1) {
          move_directions.push_back({dx, dy});
        }
      }
    }
    return move_directions;
  }();
  return move_directions;
}

static const std::vector<std::pair<int, int>>& GetQueenMoveDirections() {
  static const std::vector<std::pair<int, int>> move_directions = []() {
    std::vector<std::pair<int, int>> move_directions;
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (dx != 0 || dy != 0) {
          move_directions.push_back({dx, dy});
        }
      }
    }
    return move_directions;
  }();
  return move_directions;
}

static const std::vector<std::pair<int, int>>& GetKnightMoves() {
  static const std::vector<std::pair<int, int>> moves = []() {
    std::vector<std::pair<int, int>> moves;
    for (int dx = -2; dx <= 2; ++dx) {
      for (int dy = -2; dy <= 2; ++dy) {
        if (std::abs(dx) + std::abs(dy) == 3) {
          moves.push_back({dx, dy});
        }
      }
    }
    return moves;
  }();
  return moves;
}

bool IsValidCoordinate(int x, int y) {
  return x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE;
}

void AddMovesInDirection(const Position& position, int start_x, int start_y,
                         int direction_x, int direction_y,
                         std::vector<Move>* moves) {
  int x = start_x + direction_x;
  int y = start_y + direction_y;
  while (IsValidCoordinate(x, y)) {
    if (!position.HasPiece(x, y)) {
      moves->push_back(Move(&position, start_x, start_y, x, y));
      x += direction_x;
      y += direction_y;
      continue;
    }
    if (position.GetPiece(x, y).Color() !=
        position.GetPiece(start_x, start_y).Color()) {
      moves->push_back(Move(&position, start_x, start_y, x, y));
    }
    break;
  }
}

std::pair<int, int> FindKingOfColor(const Position& position, Color color) {
  for (int x = 0; x < BOARD_SIZE; ++x) {
    for (int y = 0; y < BOARD_SIZE; ++y) {
      if (position.HasPiece(x, y)) {
        const Piece& piece = position.GetPiece(x, y);
        if (piece.Kind() == Kind::KING && piece.Color() == color) {
          return std::make_pair(x, y);
        }
      }
    }
  }
  // Must never happen
  return std::make_pair(-1, -1);
}

template <typename T, typename Key>
bool Contains(const T& set_or_map, const Key& key) {
  return set_or_map.find(key) != set_or_map.end();
}

std::vector<Move> GenerateNonCastlingMovesForAKing(
    const Position& position, int x, int y,
    const std::unordered_set<Square>& squares_under_attack) {
  std::vector<Move> moves;
  const Color king_color = position.GetPiece(x, y).Color();
  const std::pair<int, int> enemy_king_position = FindKingOfColor(
      position, king_color == Color::BLACK ? Color::WHITE : Color::BLACK);
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      if (!IsValidCoordinate(x + dx, y + dy)) {
        continue;
      }
      if (position.HasPiece(x + dx, y + dy) &&
          position.GetPiece(x + dx, y + dy).Color() == king_color) {
        continue;
      }
      if (Contains(squares_under_attack, Square{x + dx, y + dy})) {
        continue;
      }
      if (std::abs(x + dx - enemy_king_position.first) <= 1 &&
          std::abs(y + dy - enemy_king_position.second) <= 1) {
        continue;
      }
      moves.push_back(Move(&position, x, y, x + dx, y + dy));
    }
  }
  return moves;
}

std::vector<Move> GenerateMovesForAKing(const Position& position, int x,
                                        int y) {
  const Color king_color = position.GetPiece(x, y).Color();
  const std::unordered_set<Square> squares_under_attack =
      GetSquaresUnderAttack(position, OppositeColor(king_color));
  std::vector<Move> moves =
      GenerateNonCastlingMovesForAKing(position, x, y, squares_under_attack);

  // Castling.
  const int starting_rank = (king_color == Color::WHITE ? ONE : EIGHT);
  if (x != E || y != starting_rank) {
    return moves;
  }

  if (Contains(squares_under_attack, Square{x, y})) {
    return moves;
  }
  // Short castling
  int route_x = x;
  if (position.ShortCastlingPossible(king_color)) {
    do {
      if (!position.HasPiece(H, starting_rank) ||
          (position.GetPiece(H, starting_rank) !=
           Piece(Kind::ROOK, king_color))) {
        break;
      }
      ++route_x;
      if (position.HasPiece(Square{route_x, y})) {
        break;
      }
      if (Contains(squares_under_attack, Square{route_x, y})) {
        break;
      }
      if (route_x == G) {
        moves.push_back(Move(&position, x, y, route_x, y));
      }
    } while (route_x < G);
  }
  // Long castling
  if (position.LongCastlingPossible(king_color) &&
      !position.HasPiece(Square{G, y})) {
    route_x = x;
    do {
      if (!position.HasPiece(A, starting_rank) ||
          (position.GetPiece(A, starting_rank) !=
           Piece(Kind::ROOK, king_color))) {
        break;
      }
      --route_x;
      if (position.HasPiece(Square{route_x, y})) {
        break;
      }
      if (Contains(squares_under_attack, Square{route_x, y})) {
        break;
      }
      if (route_x == C) {
        moves.push_back(Move(&position, x, y, route_x, y));
      }
    } while (route_x > C);
  }

  return moves;
}

std::vector<Move> GenerateMovesForAKnight(const Position& position, int x,
                                          int y) {
  std::vector<Move> moves;
  const Color knight_color = position.GetPiece(x, y).Color();

  for (const std::pair<int, int>& move : GetKnightMoves()) {
    int to_x = x + move.first;
    int to_y = y + move.second;

    if (IsValidCoordinate(to_x, to_y) &&
        (!position.HasPiece(to_x, to_y) ||
         position.GetPiece(to_x, to_y).Color() != knight_color)) {
      moves.push_back(Move(&position, x, y, to_x, to_y));
    }
  }

  return moves;
}

bool PawnHasNeverMoved(const Position& position, int x, int y) {
  if (position.GetPiece(x, y).Color() == Color::WHITE) {
    return y == 1;
  } else {
    return y == BOARD_SIZE - 2;
  }
}

std::vector<Move> GenerateMovesForAPawn(const Position& position, int x,
                                        int y) {
  std::vector<Move> moves;
  const Color pawn_color = position.GetPiece(x, y).Color();

  const int vertical_move_direction = pawn_color == Color::WHITE ? 1 : -1;
  if (!position.HasPiece(x, y + vertical_move_direction)) {
    moves.push_back(Move(&position, x, y, x, y + vertical_move_direction));
    if (PawnHasNeverMoved(position, x, y) &&
        !position.HasPiece(x, y + 2 * vertical_move_direction)) {
      moves.push_back(
          Move(&position, x, y, x, y + 2 * vertical_move_direction));
    }
  }

  if (x < BOARD_SIZE - 1 &&
      position.HasPiece(x + 1, y + vertical_move_direction) &&
      position.GetPiece(x + 1, y + vertical_move_direction).Color() !=
          pawn_color) {
    moves.push_back(Move(&position, x, y, x + 1, y + vertical_move_direction));
  }
  if (x > 0 && position.HasPiece(x - 1, y + vertical_move_direction) &&
      position.GetPiece(x - 1, y + vertical_move_direction).Color() !=
          pawn_color) {
    moves.push_back(Move(&position, x, y, x - 1, y + vertical_move_direction));
  }

  // TODO: Implement en passant.

  return moves;
}

} // namespace

std::vector<Move> GenerateMovesForAPiece(const Position& position, int x,
                                         int y) {
  const Piece piece = position.GetPiece(x, y);
  Kind kind = piece.Kind();
  std::vector<Move> moves;
  switch (kind) {
  case Kind::PAWN:
    return GenerateMovesForAPawn(position, x, y);
  case Kind::BISHOP:
    for (const std::pair<int, int>& direction : GetBishopMoveDirections()) {
      AddMovesInDirection(position, x, y, direction.first, direction.second,
                          &moves);
    }
    break;
  case Kind::ROOK:
    for (const std::pair<int, int>& direction : GetRookMoveDirections()) {
      AddMovesInDirection(position, x, y, direction.first, direction.second,
                          &moves);
    }
    break;
  case Kind::QUEEN:
    for (const std::pair<int, int>& direction : GetQueenMoveDirections()) {
      AddMovesInDirection(position, x, y, direction.first, direction.second,
                          &moves);
    }
    break;
  case Kind::KING:
    return GenerateMovesForAKing(position, x, y);
  case Kind::KNIGHT:
    return GenerateMovesForAKnight(position, x, y);
  case Kind::NONE:
  default:
    break;
  }
  return moves;
}

bool MoveIsValid(const Position& position, const Move& move) {
  for (const Move& possible_move :
       GenerateMovesForAPiece(position, move.From().file, move.From().rank)) {
    if (move.To() == possible_move.To()) {
      return true;
    }
  }

  return false;
}

std::unordered_set<Square> GetSquaresUnderAttack(const Position& position,
                                                 Color attacking_color) {
  std::unordered_set<Square> squares_under_attack;
  for (int x = 0; x < BOARD_SIZE; ++x) {
    for (int y = 0; y < BOARD_SIZE; ++y) {
      if (position.HasPiece(x, y) &&
          position.GetPiece(x, y).Color() == attacking_color) {
        const Piece& piece = position.GetPiece(x, y);
        if (piece.Kind() == Kind::PAWN) {
          int to_x = x - 1;
          int to_y = y + (piece.Color() == Color::WHITE ? 1 : -1);
          if (IsValidCoordinate(to_x, to_y) &&
              (!position.HasPiece(to_x, to_y) ||
               position.GetPiece(to_x, to_y).Color() != piece.Color())) {
            squares_under_attack.insert(Square{to_x, to_y});
          }
          to_x = x + 1;
          if (IsValidCoordinate(to_x, to_y) &&
              (!position.HasPiece(to_x, to_y) ||
               position.GetPiece(to_x, to_y).Color() != piece.Color())) {
            squares_under_attack.insert(Square{to_x, to_y});
          }
        } else if (piece.Kind() == Kind::KING) {
          for (const Move& move :
               GenerateNonCastlingMovesForAKing(position, x, y, {})) {
            squares_under_attack.insert(move.To());
          }
        } else {
          for (const Move& move : GenerateMovesForAPiece(position, x, y)) {
            squares_under_attack.insert(move.To());
          }
        }
      }
    }
  }
  return squares_under_attack;
}
