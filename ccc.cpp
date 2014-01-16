/* knightPaths
* © Owen Miller 2014
* This code implements Dijkstra's algorithm to find the shortest path for a
* knight to travel to a particular place on an empty chessboard
*/

#include <tuple>
#include <deque>
#include <memory>
#include <unordered_map>
#include <cstdint>
#include <assert.h>

//have a hash map of board points, then each one has neighbour links. The hashmap will therefore only be filled
//with the current fringe

//shared BoardPoint pointers kept in neighbourhood address books of other BoardPoints, as well as in the hash map

typedef std::tuple<char,int> pos2D;

struct posHash {
	std::size_t operator()(const pos2D p) const {
		size_t hash = std::get<0>(p) - 'A';
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
	std::shared_ptr<BoardPoint> parent; //this BoardPoint only exists logically as part of a path
	pos2D position;
	std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison>* const universe;
public:
	std::deque<std::shared_ptr<BoardPoint>> getNeighbours(void) {
		//initialise neighbours, then return
		return this->neighbours;
	}
	BoardPoint(pos2D myPosition,
		std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison>* const myUniverse)
		: position(myPosition), universe(myUniverse) {
			//parent is NULL by default
	}
	BoardPoint(pos2D myPosition,
		std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison>* const myUniverse,
		std::shared_ptr<BoardPoint> myParent)
		: position(myPosition), universe(myUniverse), parent(myParent) {
			//everything initialised
	}
};

typedef std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison> ChessBoard;

inline pos2D posTypify(const char pos[]) {
	assert(strlen(pos) == 2); //2D position with 26 bins for x, 10 for y
	assert(pos[0] >= 'A' && pos[0] <= 'H'); //board extremeties
	assert(pos[1] >= '1' && pos[1] <= '8'); //board extremeties
	pos2D returnPos = pos2D(static_cast<char>(pos[0]),static_cast<char>(pos[1]) - '0');
	return returnPos;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		//in Windows, code page must be changed to accomodate Unicode − chcp 1250
		printf("This program will return an optimally short path for a knight (♞) travelling from one"
			" chess-board position to another − Please specify these positions when running the program"
			", eg \n >>knightPaths A8 B7\n >>C7 B5 D6 B7\n");
	} else {
		//correct number of input arguments
		printf("calculating path from %s to %s…\n",argv[1],argv[2]);
		pos2D startPos = posTypify(argv[1]);
		pos2D endPos = posTypify(argv[2]);
	}
	return 0;
}