#include "engine/notation_parserh"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::status::IsOkAndHolds;

TEST(ParseAlgebraicNotation, PawnMoveIsParsed) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  absl::StatusOr<Move> move = ParseAlgebraicNotation("e4", Color::WHITE, position);

  EXPECT_THAT(move, IsOkAndHolds(Move(&position, E, TWO, E, FOUR)));
}
