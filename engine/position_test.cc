#include "engine/position.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::UnorderedElementsAre;

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

TEST(FindPieces, FindsPiecesAsExpected) {
  Position position = StartingPosition();
  const std::vector<std::pair<int, int>> squares =
      position.FindPieces(Piece(Kind::BISHOP, Color::BLACK));
  EXPECT_THAT(squares,
              UnorderedElementsAre(std::make_pair<int, int>(C, EIGHT),
                                   std::make_pair<int, int>(F, EIGHT)));
}

TEST(FindPieces, FindsExpectedNumberOfPawns) {
  Position position = StartingPosition();
  const std::vector<std::pair<int, int>> squares =
      position.FindPieces(Piece(Kind::PAWN, Color::WHITE));
  EXPECT_EQ(squares.size(), 8);
}
