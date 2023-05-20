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

	static struct board from_stream(std::istream& stream);
#ifdef DEBUG
	void print_state() const;
#else
	void print_state() const { return ;};
#endif

	void run();

private:
	std::vector<std::vector<enum pos_state>> map;
	struct pos pacman_pos;
	std::deque<enum direction> pacman_moves;
	struct pos ghost_pos;
	std::deque<enum direction> ghost_moves;
	std::vector<std::vector<int>> bfs_map;

	void correct_pos(struct pos& p);
	void move_ghost();
	bool move_pacman();
	void calc_bfs();
	bool dont_bfs;
	std::vector<struct pos> find_neighbors(const struct pos& curr) const;
};
