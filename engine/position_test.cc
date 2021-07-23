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

TEST(MakeMove, ShortCastlingMovesBothPieces) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, ONE);
  position.MakeMove({E, ONE}, {G, ONE});

  EXPECT_FALSE(position.HasPiece(E, ONE));
  EXPECT_FALSE(position.HasPiece(H, ONE));

  EXPECT_EQ(position.GetPiece(G, ONE), Piece(Kind::KING, Color::WHITE));
  EXPECT_EQ(position.GetPiece(F, ONE), Piece(Kind::ROOK, Color::WHITE));
}

TEST(MakeMove, LongCastlingMovesBothPieces) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.MakeMove({E, EIGHT}, {C, EIGHT});

  EXPECT_FALSE(position.HasPiece(E, EIGHT));
  EXPECT_FALSE(position.HasPiece(A, EIGHT));

  EXPECT_EQ(position.GetPiece(C, EIGHT), Piece(Kind::KING, Color::BLACK));
  EXPECT_EQ(position.GetPiece(D, EIGHT), Piece(Kind::ROOK, Color::BLACK));
}

TEST(CastlingPossible, CastlingIsImpossibleAfterKingHasMoved) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, ONE);
  position.MakeMove({E, ONE}, {E, TWO});
  position.MakeMove({E, TWO}, {E, ONE});

  EXPECT_FALSE(position.ShortCastlingPossible(Color::WHITE));
  EXPECT_FALSE(position.LongCastlingPossible(Color::WHITE));
  EXPECT_TRUE(position.ShortCastlingPossible(Color::BLACK));
}

TEST(CastlingPossible, CastlingIsImpossibleAfterRookHasMoved) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.MakeMove({A, EIGHT}, {A, SEVEN});
  position.MakeMove({A, SEVEN}, {A, EIGHT});

  EXPECT_FALSE(position.LongCastlingPossible(Color::BLACK));
  EXPECT_TRUE(position.ShortCastlingPossible(Color::BLACK));
  EXPECT_TRUE(position.LongCastlingPossible(Color::WHITE));
}

TEST(FindPieces, FindsPiecesAsExpected) {
  Position position = StartingPosition();
  const std::vector<Square> squares =
      position.FindPieces(Piece(Kind::BISHOP, Color::BLACK));
  EXPECT_THAT(squares,
              UnorderedElementsAre(Square{C, EIGHT}, Square{F, EIGHT}));
}

TEST(FindPieces, FindsExpectedNumberOfPawns) {
  Position position = StartingPosition();
  const std::vector<Square> squares =
      position.FindPieces(Piece(Kind::PAWN, Color::WHITE));
  EXPECT_EQ(squares.size(), 8);
}
