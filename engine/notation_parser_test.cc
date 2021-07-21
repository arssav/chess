#include "engine/notation_parser.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(ParseAlgebraicNotation, PawnMoveIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("e4", Color::WHITE, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, E, TWO, E, FOUR));
}

TEST(ParseAlgebraicNotation, MoveWithExplicitKindIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), C, ONE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Bf4", Color::WHITE, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, C, ONE, F, FOUR));
}

TEST(ParseAlgebraicNotation, MoveWithWrongKindGivesError) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), C, ONE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Nf4", Color::WHITE, position);

  EXPECT_FALSE(move.ok());
}

TEST(ParseAlgebraicNotation, IllegalMoveGivesError) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), C, ONE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Nc2", Color::WHITE, position);

  EXPECT_FALSE(move.ok());
}

TEST(ParseAlgebraicNotation, CaptureIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), D, FOUR);
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), D, FIVE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Kxd5", Color::BLACK, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, D, FOUR, D, FIVE));
}
