#include <iostream>
#include "board.hpp"

int main() {
	// LE TODOS OS INPUTS
	struct board board = board::from_stream(std::cin);
	board.run();
}
