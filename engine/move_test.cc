#include "engine/move.h"

#include "engine/position.h"

#include <gtest/gtest.h>

TEST(IsACapture, IsNotACapture) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  Move move(&position, E, TWO, E, THREE);

  EXPECT_TRUE(move.IsACapture());
}

TEST(IsACapture, IsActuallyACapture) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), D, THREE);
  Move move(&position, E, TWO, D, THREE);

  EXPECT_TRUE(move.IsACapture());
}

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
