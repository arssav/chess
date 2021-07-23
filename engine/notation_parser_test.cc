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

TEST(ParseAlgebraicNotation, UnparsedTextGivesError) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), C, ONE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Bf4extra", Color::WHITE, position);

  EXPECT_FALSE(move.ok());
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

TEST(ParseAlgebraicNotation, DisambiguationByFileIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), C, FOUR);
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), D, FIVE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("exd5", Color::WHITE, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, E, FOUR, D, FIVE));
}

TEST(ParseAlgebraicNotation, DisambiguationByRankIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), E, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), E, SIX);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("R1xe4", Color::BLACK, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, E, ONE, E, FOUR));
}

TEST(ParseAlgebraicNotation, DisambiguationByRankAndFileIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), C, ONE);
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), A, THREE);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Qa1c3", Color::WHITE, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, A, ONE, C, THREE));
}

TEST(ParseAlgebraicNotation, NoDisambiguationWhenItsNeededCausesError) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), E, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), E, SIX);
  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("Rxe4", Color::BLACK, position);

  EXPECT_FALSE(move.ok());
}

TEST(ParseAlgebraicNotation, ShortCastlingIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), H, EIGHT);

  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("0-0", Color::BLACK, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, {E, EIGHT}, {G, EIGHT}));
}

TEST(ParseAlgebraicNotation, LongCastlingIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);

  absl::StatusOr<Move> move =
      ParseAlgebraicNotation("0-0-0", Color::WHITE, position);

  EXPECT_TRUE(move.ok());
  EXPECT_EQ(*move, Move(&position, {E, ONE}, {C, ONE}));
}
