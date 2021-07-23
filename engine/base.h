#ifndef ENGINE_BASE_H_
#define ENGINE_BASE_H_

#include <functional>
#include <ostream>

static constexpr int BOARD_SIZE = 8;
static constexpr int BOARD_SIZE_LOG = 3;

// Using scoped enums here to prevent implicit casting to int and name
// collisions.
enum class Color : char { BLACK, WHITE };
enum class Kind : char { NONE, PAWN, KING, QUEEN, BISHOP, KNIGHT, ROOK };

Color OppositeColor(Color color) {
  return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}

struct Square {
  Square(int file, int rank) : file(file), rank(rank) {}

  bool operator==(const Square& other) const {
    return file == other.file && rank == other.rank;
  }

  int file;
  int rank;
};

template <> struct std::hash<Square> {
  size_t operator()(const Square& square) const {
    return std::hash<int>{}(square.file | (square.rank << BOARD_SIZE_LOG));
  }
};

std::ostream& operator<<(std::ostream& stream, const Square& square) {
  stream << static_cast<char>('a' + static_cast<char>(square.file))
         << 1 + square.rank;
  return stream;
}

#endif // ENGINE_BASE_H_