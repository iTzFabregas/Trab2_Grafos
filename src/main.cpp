#include <iostream>
#include "board.hpp"

int main() {
	struct board board = board::from_stream(std::cin);
	board.run();
}
