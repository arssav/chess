#include "chess/position.h"

#include <gtest/gtest.h>

TEST(HasPiece, EmptyBoard) {
  Position position;
  EXPECT_FALSE(position.HasPiece(E, TWO));
}

TEST(HasPiece, ActuallyHasAPiece) {
  Position position;
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  EXPECT_TRUE(position.HasPiece(A, ONE));
}

TEST(HasPiece, HasAPieceOnADifferentSquare) {
  Position position;
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  EXPECT_TRUE(position.HasPiece(A, ONE));
}

TEST(RemovePiece, ActuallyRemovesThePiece) {
  Position position;
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  position.RemovePiece(A, ONE);
  EXPECT_FALSE(position.HasPiece(A, ONE));
}

TEST(MakeMove, PieceIsMoved) {
  Position position;
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  position.MakeMove({A, ONE}, {A, THREE});
  EXPECT_FALSE(position.HasPiece(A, ONE));
  EXPECT_EQ(position.GetPiece(A, THREE), Piece(Kind::ROOK, Color::WHITE));
}
