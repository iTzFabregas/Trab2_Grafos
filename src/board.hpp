#include <istream>
#include <sys/types.h>
#include <vector>
#include <deque>

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	INVALID
};

enum pos_state {
	PATH,
	WALL
};

struct pos {
	int x,y;
};

struct board {

public:
	/**
	 * @brief Função que lê todos os inputs do teclado e monta o mapa
	 *
	 * @param stream modo de leitura
	 * @return struct board mapa criado com todas as informações
	 */
	static struct board from_stream(std::istream& stream);

	/**
	 * @brief Função que inicia a busca pelo mapa
	 *
	 */
	void run();

private:

	std::vector<std::vector<enum pos_state>> map;
	struct pos pacman_pos;
	std::deque<enum direction> pacman_moves;
	struct pos ghost_pos;
	std::deque<enum direction> ghost_moves;
	std::vector<std::vector<int>> bfs_map;
	bool dont_bfs;

	/**
	 * @brief Função que verifica se  a nova posição do pacman ou fantasma está dentro do mapa ou não
	 *
	 * @param p posição
	 */
	void correct_pos(struct pos& p);

	/**
	 * @brief Função que move o fantasma baseado nos movimentos do input
	 *
	 */
	void move_ghost();

	/**
	 * @brief Função que move o pacman, nela é feito o BFS para encontrar qual a melhor direção possível para o pacman
	 *
	 * @return true se ainda é ou foi possível achar o caminho até o fantasma
	 * @return false se não é possível achar o caminho até o fantasma
	 */
	bool move_pacman();

	/**
	 * @brief Função que calcula o BFS propriamente, obtendo a distancia de cada PATH do mapa até o fantasma
	 *
	 */
	void calc_bfs();

	/**
	 * @brief Função que encontra os 4 espaços vizinhos de um ponto
	 *
	 * @param curr ponto a se encontrar os vizinhos
	 * @return std::vector<struct pos>  vector com a posição dos vizinhos
	 */
	std::vector<struct pos> find_neighbors(const struct pos& curr) const;
};
