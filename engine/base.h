#ifndef ENGINE_BASE_H_
#define ENGINE_BASE_H_

// Using scoped enums here to prevent implicit casting to int and name
// collisions.
enum class Color : char { BLACK, WHITE };
enum class Kind : char { NONE, PAWN, KING, QUEEN, BISHOP, KNIGHT, ROOK };

struct Square {
  Square(int file, int rank) : file(file), rank(rank) {}

  bool operator==(const Square& other) const {
    return file == other.file && rank == other.rank;
  }

  int file;
  int rank;
};

#endif // ENGINE_BASE_H_