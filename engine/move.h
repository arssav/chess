#ifndef ENGINE_MOVE_H_
#define ENGINE_MOVE_H_

#include <ostream>
#include <string>
#include <utility>

#include "engine/position.h"

// Represents a move of a piece, storing both source and destination
// coordinates.
// TODO: Consider adding a more compact class with only destination coordinates
// to save on memory.
class Move {
 public:
  Move(int from_x, int from_y, int to_x, int to_y)
      : from_x_(from_x), from_y_(from_y), to_x_(to_x), to_y_(to_y){};
  Move(const Position* position, int from_x, int from_y, int to_x, int to_y)
      : position_(position), from_x_(from_x), from_y_(from_y), to_x_(to_x),
        to_y_(to_y){};

  bool operator==(const Move& other) const;

  std::string ToAlgebraicNotation() const;

  std::pair<int, int> From() const { return {from_x_, from_y_}; }
  std::pair<int, int> To() const { return {to_x_, to_y_}; }

  bool IsACapture() const;

  friend class Position;

 private:
  std::string ToLongAlgebraicNotation() const;

  // Pointer to a position this turn refers to. Can be nullptr if position is
  // known from context. Unowned.
  const Position* position_ = nullptr;

  int from_x_;
  int from_y_;
  int to_x_;
  int to_y_;
};

// Prints turn in FIDE algebraic notation. This is particularly useful in tests
// which use this operator to print difering values.
std::ostream& operator<<(std::ostream& out, const Move& move);

#endif // ENGINE_MOVE_H_
