#include <iostream>
#include <string>

#include "absl/status/statusor.h"

#include "engine/game.h"
#include "engine/move.h"
#include "engine/notation_parser.h"

namespace {

void PrintGameState(const Game& game) {
  std::cout << "Turn " << game.Turn() << " ("
            << (game.ActivePlayerColor() == Color::BLACK ? "Black" : "White")
            << ")" << std::endl;
  std::cout << game.Position().ToString() << std::endl;
}

bool ParseInputAndReact(const std::string& input, Game& game) {
  if (input.empty()) {
    return false;
  }
  if (input == "exit") {
    return true;
  }
  if (input == "print") {
    std::cout << game.Position().ToString() << std::endl;
    return false;
  }

  absl::StatusOr<Move> move_or =
      ParseAlgebraicNotation(input, game.ActivePlayerColor(), game.Position());
  if (move_or.ok()) {
    game.MakeMove(*move_or);
    return false;
  }

  std::cout << "Couldn't parse move: " << move_or.status() << std::endl;
  return false;
}

} // namespace

int main() {
  Game game;
  while (true) {
    PrintGameState(game);
    std::string line;
    std::getline(std::cin, line);
    if (std::cin.eof()) {
      break;
    }
    if (ParseInputAndReact(line, game)) {
      break;
    }
  }

  return 0;
}