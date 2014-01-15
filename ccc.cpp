/* knightPaths
* © Owen Miller 2014
* This code implements Dijkstra's algorithm to find the shortest path for a
* knight to travel to a particular place on an empty chessboard
*/

#include <tuple>
#include <deque>
#include <memory>
#include <unordered_map>

//have a hash map of board points, then each one has neighbour links. The hashmap will therefore only be filled
//with the current fringe

typedef std::tuple<char,int> pos2D;

struct posHash {
	std::size_t operator()(const pos2D p) const {
		size_t hash = std::get<0>(p) - 'a';
		hash = hash*8;
		hash += std::get<1>(p) - 1; //base at min integer
		return hash;
	}
};

struct placeComparison {
	bool operator()(const pos2D p1, const pos2D p2) const {
		if(p1 == p2)
			return true;
		else
			return false;
	}
};

class BoardPoint {
private:
	std::deque<std::shared_ptr<BoardPoint>> neighbours;
	std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison>* const universe;
public:
	std::deque<std::shared_ptr<BoardPoint>> getNeighbours(void) {
		//initialise neighbours
		return this->neighbours;
	}
};

typedef std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison> ChessBoard;

int main(int argc, char** argv) {

	return 0;
}