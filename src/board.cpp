#include "board.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
#include <algorithm>
#include <memory>
#include <cstring>

/**
 * @brief  Função que pega uma direção do enum direction e
 * transforma em string
 *
 * @param dir direção do enum direction
 * @return std::string string relativa a direção entrada
 */
std::string direction2string(enum direction dir) {
		std::string res;
		switch (dir) {
		case UP:
			res = "UP";
			break;
		case DOWN:
			res = "DOWN";
			break;
		case LEFT:
			res = "LEFT";
			break;
		case RIGHT:
			res = "RIGHT";
			break;
		case INVALID:
			res = "INVALID";
			break;
		}
	return res;
}


struct board board::from_stream(std::istream &stream) {
	struct board result;

	size_t n;
	result.dont_bfs = false;

	stream >> n;

	result.map.reserve(n);
	result.bfs_map.reserve(n);
	for (size_t y = 0; y < n; y++) {
		result.map.push_back(std::vector<enum pos_state>());
		result.bfs_map.push_back(std::vector<int>());
		for (size_t x = 0; x < n; x++) {
			int state;
			stream >> state;
			result.map[y].push_back(state ? WALL : PATH);
			result.bfs_map[y].push_back(-1);
		}
	}

	stream >> result.pacman_pos.y;
	stream >> result.pacman_pos.x;

	stream >> result.ghost_pos.y;
	stream >> result.ghost_pos.x;

	stream >> n;

	for (size_t i = 0; i < n; i++) {
		char read_move;
		enum direction move;
		stream >> read_move;
		switch (read_move) {
		case 'U':
			move = UP;
			break;
		case 'D':
			move = DOWN;
			break;
		case 'L':
			move = LEFT;
			break;
		case 'R':
			move = RIGHT;
			break;
		default:
			continue;
		}
		if(stream.eof())
			break;
		result.ghost_moves.push_back(move);
	}
	return result;
}

/**
 * Sobrecarga de operadores para deixar as operações de envolvendo as posições do pacman e fanstama
 * mais legíveis.
 */

pos operator+(const pos& a, const pos& b) {
	return {
		a.x + b.x,
		a.y + b.y
	};
}

pos operator-(const pos& a, const pos& b) {
	return {
		a.x - b.x,
		a.y - b.y
	};
}

bool operator==(const pos& a, const pos& b) {
	return a.x == b.x && a.y == b.y;
}

bool operator!=(const pos& a, const pos& b) {
	return a.x != b.x || a.y != b.y;
}

struct pos operator+=(pos& a, const pos& b) {
	return a = a + b;
}

/**
 * @brief Função que pega uma direção do enum direction e
 * transforma em uma posição que ou somar, a nova posição
 * será para a direção desejada
 *
 * @param dir direção do enum
 * @return struct pos posição para a direção desejada
 */
struct pos direction2pos(enum direction dir) {
		struct pos res;
		switch (dir) {
		case UP:
			res = {0,-1};
			break;
		case DOWN:
			res = {0,1};
			break;
		case LEFT:
			res = {-1,0};
			break;
		case RIGHT:
			res = {1,0};
			break;
		case INVALID:
			res = {0,0};
			break;
		}
	return res;
}

/**
 * @brief Função que pega uma variação na posição e
 * transforma em um enum direction
 *
 * @param ds variação na posição
 * @return enum direction direção do enum
 */
enum direction pos2direction(struct pos ds) {
	if(ds.x == 1 && ds.y == 0)
		return RIGHT;
	if(ds.x == 0 && ds.y == 1)
		return DOWN;
	if(ds.x == -1 && ds.y == 0)
		return LEFT;
	if(ds.x == 0 && ds.y == -1)
		return UP;
	return INVALID;
}

void board::run() {
	while(pacman_pos != ghost_pos) {

		// IF QUE VERIFICA SE AINDA É POSSÍVEL DO PACMAN CHEGAR NO FANTASMA
		if((ghost_moves.empty() && map[ghost_pos.y][ghost_pos.x] == WALL) || !move_pacman() ){
			std::cout << "Não foi possível achar um caminho\n";
			return;
		}

		if(pacman_pos == ghost_pos) break;

		move_ghost();

	}

	std::cout << "Número de passos: "  << pacman_moves.size() << "\n";
	std::cout << "Movimentos para cima: " << std::count(std::begin(pacman_moves), std::end(pacman_moves), UP) << "\n";
	std::cout << "Movimentos para baixo: " << std::count(std::begin(pacman_moves), std::end(pacman_moves), DOWN) << "\n";
	std::cout << "Movimentos para esquerda: " << std::count(std::begin(pacman_moves), std::end(pacman_moves), LEFT) << "\n";
	std::cout << "Movimentos para direita: " << std::count(std::begin(pacman_moves), std::end(pacman_moves), RIGHT) << "\n";
}

void board::correct_pos(struct pos& p) {
	if(p.x < 0)
		p.x = 0;
	if(p.y < 0)
		p.y = 0;
	if(p.x >= (int)map.size())
		p.x = map.size() - 1;
	if(p.y >= (int)map.size())
		p.y = map.size() - 1;
}

void board::move_ghost() {
	if(ghost_moves.empty()) return;
	ghost_pos += direction2pos(ghost_moves.front());
	correct_pos(ghost_pos);
	ghost_moves.pop_front();
}


std::vector<struct pos> board::find_neighbors(const struct pos& curr) const {
	std::vector<struct pos> neighbors;

	struct pos neighbors_directions[] = {
		{1,0},
		{-1,0},
		{0,-1},
		{0,1},
	};

	// For que pega todos os 4 vizinhos do curr mas só coloca no vetor os que estiverem dentro do mapa
	for (auto& dir : neighbors_directions) {
		auto neighbor = curr + dir;
		if(neighbor.x >= (int)map.size() || neighbor.y >= (int)map.size() ||
		   neighbor.x < 0 || neighbor.y < 0)
			continue;

		neighbors.push_back(neighbor);
	}

	return neighbors;
}

void board::calc_bfs() {
	if(dont_bfs)
		return;
	if(!bfs_map.empty() && ghost_moves.empty())
		dont_bfs = true;

	auto visited = std::make_unique<char[]>(map.size() * map.size());
	std::memset(visited.get(), 0, map.size() * map.size());
	std::queue<struct pos> queue;

	queue.push(ghost_pos);
	visited[ghost_pos.y * map.size() + ghost_pos.x] = 1;
	bfs_map[ghost_pos.y][ghost_pos.x] = 0;

	while (!queue.empty()) {
		struct pos current = queue.front();
		queue.pop();

		for (auto& neighbor : find_neighbors(current)) {
			if(map[neighbor.y][neighbor.x] == WALL)
				continue;
			if(visited[neighbor.y * map.size() + neighbor.x] == 1)
				continue;

			visited[neighbor.y * map.size() + neighbor.x] = 1;
			queue.push(neighbor);
			bfs_map[neighbor.y][neighbor.x] = bfs_map[current.y][current.x] + 1;
		}
	}
}

bool board::move_pacman() {

	// SE O FANTASMA ESTIVER DENTRO DA PAREDE, O PACMAN NAO SE MOVE
	if(map[ghost_pos.y][ghost_pos.x] == WALL)
		return true;

	if(pacman_pos == ghost_pos)
		return true;

	calc_bfs();

	// SE O FANTASMA NAO TIVER MAIS MOVIMENTOS, E NÃO FOR POSSÍVEL CHEGAR NELE, NAO É POSSÍVEL CHEGAR
	if(ghost_moves.empty() && bfs_map[pacman_pos.y][pacman_pos.x] == -1){
		return false;
	}

	// SE NÃO FOR POSSÍVEL CHEGAR NO FANTASMA, MAS ELE AINDA TIVER MOVIMENTOS, CONTINUA
	if(bfs_map[pacman_pos.y][pacman_pos.x] == -1)
		return true;


	auto neighbors = find_neighbors(pacman_pos);
	std::vector<int> distances;

	// REMOVE TODOS OS VIZINHOS QUE FOREM UMA PAREDE
	neighbors.erase(std::remove_if(std::begin(neighbors), std::end(neighbors),
								   [&](auto n){ return map[n.y][n.x] == WALL;}), std::end(neighbors));

	for (auto neighbor : neighbors) {
		distances.push_back(bfs_map[neighbor.y][neighbor.x]);
	}

	if(distances.empty())
		return true;

	// MOVE O PACMAN PARA O VIZINHO QUE ESTIVER A UMA DISTANCIA MENOR AO FANTASMA
	int min_dist = distances[0];
	int min_dist_i = 0;
	for (size_t i = 0; i < distances.size(); i++) {
		if(min_dist > distances[i]) {
			min_dist = distances[i];
			min_dist_i = i;
		}
	}

	auto prev_pos = pacman_pos;

	pacman_pos = neighbors[min_dist_i];
	correct_pos(pacman_pos);
	pacman_moves.push_back(pos2direction(pacman_pos - prev_pos));
	return true;
}
