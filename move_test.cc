#include "chess/move.h"

#include "chess/position.h"

#include <gtest/gtest.h>

TEST(ToAlgebraicNotation, NoPosition) {
  Move move(E, TWO, E, FOUR);
  const std::string algebraic_notation = move.ToAlgebraicNotation();

  EXPECT_EQ(algebraic_notation, "?e2-e4");
}

TEST(ToAlgebraicNotation, KnownPositionPawn) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);

  Move move(&position, E, TWO, E, FOUR);
  const std::string algebraic_notation = move.ToAlgebraicNotation();

  EXPECT_EQ(algebraic_notation, "e2-e4");
}

TEST(ToAlgebraicNotation, KnownPositionKnight) {
  Position position;
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), B, ONE);

  Move move(&position, B, ONE, C, THREE);
  const std::string algebraic_notation = move.ToAlgebraicNotation();

  EXPECT_EQ(algebraic_notation, "Nb1-c3");
}
