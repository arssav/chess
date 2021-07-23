#include "engine/game_engine.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::UnorderedElementsAre;

// Pawn Tests.

TEST(GenerateMovesForAPiece, WhitePawnOnStartingPosition) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, TWO);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, TWO, E, THREE),
                                          Move(&position, E, TWO, E, FOUR)));
}

TEST(GenerateMovesForAPiece, BlackPawnOnRankTwo) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), E, TWO);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, TWO);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, TWO, E, ONE)));
}

TEST(GenerateMovesForAPiece, PawnWithSameColorPieceBlockingItsMove) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), E, THREE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, TWO);

  EXPECT_THAT(moves, UnorderedElementsAre());
}

TEST(GenerateMovesForAPiece, PawnWithOppositeColorPieceBlockingItsMove) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), E, THREE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, TWO);

  EXPECT_THAT(moves, UnorderedElementsAre());
}

TEST(GenerateMovesForAPiece, PawnWithAPieceBlockingTwoSquaresAdvance) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), E, FOUR);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, TWO);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, TWO, E, THREE)));
}

TEST(GenerateMovesForAPiece, PawnTakingAPiece) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), D, FIVE);
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), F, FIVE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, FOUR);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, FOUR, E, FIVE),
                                          Move(&position, E, FOUR, D, FIVE),
                                          Move(&position, E, FOUR, F, FIVE)));
}

TEST(GenerateMovesForAPiece, PawnCannotTakeSameColorPiece) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), D, FIVE);
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), F, FIVE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, FOUR);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, FOUR, E, FIVE)));
}

TEST(GenerateMovesForAPiece, PawnOnFirstFileDoesNotCrash) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), A, THREE);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), B, FOUR);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, THREE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, A, THREE, A, FOUR),
                                          Move(&position, A, THREE, B, FOUR)));
}

// Bishop tests.

TEST(GenerateMovesForAPiece, BishopCornered) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), A, ONE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, ONE);

  EXPECT_EQ(moves.size(), 7);
}

TEST(GenerateMovesForAPiece, BishopMoveDirectionsAreInitializedOnce) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), A, ONE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, ONE);

  // Checking that the move directions singleton vector is actually only
  // initialized once. If it isn't, moves in the same direction would be
  // added twice on a second call.
  // Note that this is a test anti-pattern: we're testing the internals of a
  // function as opposed to its interface here. However, it's better to
  // catch any possible errors early.
  EXPECT_EQ(moves.size(), 7);
  EXPECT_EQ(moves.size(), 7);
}

TEST(GenerateMovesForAPiece, BishopInAMiddleOfAnEmptyBoard) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), E, FOUR);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, FOUR);

  EXPECT_THAT(
      moves,
      UnorderedElementsAre(
          Move(&position, E, FOUR, D, THREE), Move(&position, E, FOUR, C, TWO),
          Move(&position, E, FOUR, B, ONE), Move(&position, E, FOUR, D, FIVE),
          Move(&position, E, FOUR, C, SIX), Move(&position, E, FOUR, B, SEVEN),
          Move(&position, E, FOUR, A, EIGHT),
          Move(&position, E, FOUR, F, THREE), Move(&position, E, FOUR, G, TWO),
          Move(&position, E, FOUR, H, ONE), Move(&position, E, FOUR, F, FIVE),
          Move(&position, E, FOUR, G, SIX),
          Move(&position, E, FOUR, H, SEVEN)));
}

TEST(GenerateMovesForAPiece, BishopIsBlockedByOppositeColorPieces) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), G, SIX);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), F, THREE);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), D, FIVE);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), D, THREE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, FOUR);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, FOUR, F, FIVE),
                                          Move(&position, E, FOUR, G, SIX),
                                          Move(&position, E, FOUR, F, THREE),
                                          Move(&position, E, FOUR, D, FIVE),
                                          Move(&position, E, FOUR, D, THREE)));
}

TEST(GenerateMovesForAPiece, BishopIsBlockedBySameColorPieces) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), G, SIX);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), F, THREE);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), D, FIVE);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), D, THREE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, FOUR);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, FOUR, F, FIVE)));
}

TEST(GenerateMovesForAPiece, SameColorBishopsInOppositeCorners) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), H, EIGHT);
  const std::vector<Move> first_moves =
      GenerateMovesForAPiece(position, A, ONE);
  const std::vector<Move> second_moves =
      GenerateMovesForAPiece(position, H, EIGHT);

  EXPECT_EQ(first_moves.size(), 6);
  EXPECT_EQ(second_moves.size(), 6);
}

TEST(GenerateMovesForAPiece, OppositeColorBishopsInOppositeCorners) {
  Position position;
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), H, EIGHT);
  const std::vector<Move> first_moves =
      GenerateMovesForAPiece(position, A, ONE);
  const std::vector<Move> second_moves =
      GenerateMovesForAPiece(position, H, EIGHT);

  EXPECT_EQ(first_moves.size(), 7);
  EXPECT_EQ(second_moves.size(), 7);
}

// Knight Tests.

TEST(GenerateMovesForAPiece, KnightInAMiddleOfAnEmptyBoard) {
  Position position;
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), C, SIX);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, C, SIX);

  EXPECT_THAT(
      moves,
      UnorderedElementsAre(
          Move(&position, C, SIX, B, EIGHT), Move(&position, C, SIX, D, EIGHT),
          Move(&position, C, SIX, E, SEVEN), Move(&position, C, SIX, E, FIVE),
          Move(&position, C, SIX, D, FOUR), Move(&position, C, SIX, B, FOUR),
          Move(&position, C, SIX, A, FIVE), Move(&position, C, SIX, A, SEVEN)));
}

TEST(GenerateMovesForAPiece, KnightOnAStartingPosition) {
  Position position = StartingPosition();
  const std::vector<Move> moves = GenerateMovesForAPiece(position, B, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, B, ONE, A, THREE),
                                          Move(&position, B, ONE, C, THREE)));
}

TEST(GenerateMovesForAPiece, KnightDoesntCareForBeingBlocked) {
  Position position;
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), C, SIX);
  for (int x = B; x < D; ++x) {
    for (int y = FIVE; y <= SEVEN; ++y) {
      if (x != C || y != SIX) {
        position.AddPiece(Piece(Kind::ROOK, Color::BLACK), x, y);
      }
    }
  }
  const std::vector<Move> moves = GenerateMovesForAPiece(position, C, SIX);

  EXPECT_THAT(
      moves,
      UnorderedElementsAre(
          Move(&position, C, SIX, B, EIGHT), Move(&position, C, SIX, D, EIGHT),
          Move(&position, C, SIX, E, SEVEN), Move(&position, C, SIX, E, FIVE),
          Move(&position, C, SIX, D, FOUR), Move(&position, C, SIX, B, FOUR),
          Move(&position, C, SIX, A, FIVE), Move(&position, C, SIX, A, SEVEN)));
}

TEST(GenerateMovesForAPiece, KnightCanTakeOppositeColorPieces) {
  Position position;
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), B, THREE);
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), C, TWO);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, A, ONE, B, THREE),
                                          Move(&position, A, ONE, C, TWO)));
}

TEST(GenerateMovesForAPiece, KnightCannotTakeSameColorPieces) {
  Position position;
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), B, THREE);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), C, TWO);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre());
}

// King tests.

TEST(GenerateMovesForAPiece, KingInAMiddleOfAnEmptyBoard) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), C, SIX);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, C, SIX);

  EXPECT_EQ(moves.size(), 8);
}

TEST(GenerateMovesForAPiece, KingOnAStartingPosition) {
  Position position = StartingPosition();
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre());
}

TEST(GenerateMovesForAPiece, KingCannotMoveAdjacentToEnemyKing) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), A, ONE);
  position.AddPiece(Piece(Kind::KING, Color::WHITE), C, TWO);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, A, ONE, A, TWO)));
}

TEST(GenerateMovesForAPiece, KingCanTakePieces) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), A, ONE);
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), B, TWO);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, A, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, A, ONE, B, TWO)));
}

TEST(GenerateMovesForAPiece, KingCanCastle) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, ONE);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, ONE, E, TWO),
                                          Move(&position, E, ONE, D, ONE),
                                          Move(&position, E, ONE, F, ONE),
                                          Move(&position, E, ONE, D, TWO),
                                          Move(&position, E, ONE, F, TWO),
                                          Move(&position, E, ONE, G, ONE)));
}

TEST(GenerateMovesForAPiece, KingCannotCastleAfterMoving) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), D, TWO);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), F, TWO);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, ONE);
  position.MakeMove({E, ONE}, {D, ONE});
  position.MakeMove({D, ONE}, {E, ONE});
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, ONE, D, ONE),
                                          Move(&position, E, ONE, F, ONE)));
}

TEST(GenerateMovesForAPiece, KingCannotCastleAfterRookHasMoved) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::WHITE), E, ONE);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), D, TWO);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), E, TWO);
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), F, TWO);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  position.MakeMove({A, ONE}, {B, ONE});
  position.MakeMove({B, ONE}, {A, ONE});
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, ONE);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, ONE, D, ONE),
                                          Move(&position, E, ONE, F, ONE)));
}

TEST(GenerateMovesForAPiece, KingCannotCastleWhenUnderCheck) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), H, EIGHT);
  position.AddPiece(Piece(Kind::QUEEN, Color::WHITE), E, FOUR);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, EIGHT);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, EIGHT, D, EIGHT),
                                          Move(&position, E, EIGHT, F, EIGHT),
                                          Move(&position, E, EIGHT, D, SEVEN),
                                          Move(&position, E, EIGHT, F, SEVEN)));
}

TEST(GenerateMovesForAPiece,
     KingCannotMoveThroughAttackedSquaresWhileCastling) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), H, FOUR);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, SEVEN);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, EIGHT);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, EIGHT, F, EIGHT)));

  // Protect castling route
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), F, SIX);
  const std::vector<Move> new_moves =
      GenerateMovesForAPiece(position, E, EIGHT);
  EXPECT_THAT(new_moves,
              UnorderedElementsAre(Move(&position, E, EIGHT, F, EIGHT),
                                   Move(&position, E, EIGHT, D, EIGHT),
                                   Move(&position, E, EIGHT, C, EIGHT)));
}

TEST(GenerateMovesForAPiece, KingCannotCastleToAttackedSquare) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.AddPiece(Piece(Kind::BISHOP, Color::WHITE), H, THREE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, SEVEN);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, EIGHT);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, EIGHT, D, EIGHT),
                                          Move(&position, E, EIGHT, F, EIGHT)));

  // Protect castling destination
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), E, SIX);
  const std::vector<Move> new_moves =
      GenerateMovesForAPiece(position, E, EIGHT);
  EXPECT_THAT(new_moves,
              UnorderedElementsAre(Move(&position, E, EIGHT, D, EIGHT),
                                   Move(&position, E, EIGHT, F, EIGHT),
                                   Move(&position, E, EIGHT, C, EIGHT)));
}

TEST(GenerateMovesForAPiece, RookCanMoveThroughAttackedSquaresWhileCastling) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), B, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, SEVEN);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, EIGHT);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, EIGHT, F, EIGHT),
                                          Move(&position, E, EIGHT, D, EIGHT),
                                          Move(&position, E, EIGHT, C, EIGHT)));
}

TEST(GenerateMovesForAPiece, RookCanCastleWhenAttacked) {
  Position position;
  position.AddPiece(Piece(Kind::KING, Color::BLACK), E, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::BLACK), A, EIGHT);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), A, ONE);
  position.AddPiece(Piece(Kind::ROOK, Color::WHITE), H, SEVEN);
  const std::vector<Move> moves = GenerateMovesForAPiece(position, E, EIGHT);

  EXPECT_THAT(moves, UnorderedElementsAre(Move(&position, E, EIGHT, F, EIGHT),
                                          Move(&position, E, EIGHT, D, EIGHT),
                                          Move(&position, E, EIGHT, C, EIGHT)));
}

// GetSquaresUnderAttack()

TEST(GetSquaresUnderAttack, PawnAttacksEmptySquares) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::WHITE), B, THREE);
  const std::unordered_set<Square> squares =
      GetSquaresUnderAttack(position, Color::WHITE);

  EXPECT_THAT(squares, UnorderedElementsAre(Square{A, FOUR}, Square{C, FOUR}));
}

TEST(GetSquaresUnderAttack, PawnAttacksEnemyOccupiedSquares) {
  Position position;
  position.AddPiece(Piece(Kind::PAWN, Color::BLACK), B, TWO);
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), C, ONE);
  position.AddPiece(Piece(Kind::BISHOP, Color::BLACK), A, ONE);
  const std::unordered_set<Square> squares =
      GetSquaresUnderAttack(position, Color::BLACK);

  EXPECT_THAT(squares, UnorderedElementsAre(Square{C, ONE}));
}

TEST(GetSquaresUnderAttack, KnightInTheMiddleAttacksEightSquares) {
  Position position;
  position.AddPiece(Piece(Kind::KNIGHT, Color::WHITE), E, FOUR);
  position.AddPiece(Piece(Kind::KNIGHT, Color::BLACK), G, FIVE);
  const std::unordered_set<Square> squares =
      GetSquaresUnderAttack(position, Color::WHITE);

  EXPECT_EQ(squares.size(), 8);
}
