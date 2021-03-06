/* knightPaths
* © Owen Miller 2014
* This code implements Dijkstra's algorithm to find the shortest path for a
* knight to travel to a particular place on an empty chessboard
*/

#include <tuple>
#include <list>
#include <memory>
#include <unordered_map>
#include <cstdint>
#include <assert.h>

#define MAX_X 'H'
#define MAX_Y 8

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

std::string posToString(const pos2D& position) {
	char posString[3]; //NULL termination
	sprintf_s(posString,"%c%d",std::get<0>(position),std::get<1>(position));
	return posString; //implicit conversion on return
}

class BoardPoint : public std::enable_shared_from_this<BoardPoint> {
private:
	std::shared_ptr<BoardPoint> parent; //this BoardPoint only exists logically as part of a path
	pos2D position;
protected:
	std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison>* const universe;
	std::list<std::shared_ptr<BoardPoint>> neighbours;
public:
	virtual std::list<std::shared_ptr<BoardPoint>> getNeighbours(void) = 0;

	std::shared_ptr<BoardPoint> getptr() {
		return shared_from_this();
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
	pos2D getPosition(void) const {
		return this->position;
	}
	std::shared_ptr<BoardPoint> getParent() {
		if(this->parent == nullptr) {
			//printf(" parent is null ");
			return nullptr;
		} else {
			return this->parent;
		}
	}
};

typedef std::unordered_map<pos2D,std::shared_ptr<BoardPoint>,posHash,placeComparison> ChessBoard;

class KnightPoint : public BoardPoint {
public:
	KnightPoint(pos2D myPosition, ChessBoard* const myUniverse)
		: BoardPoint(myPosition, myUniverse) {
			//forward to superclass
	}
	KnightPoint(pos2D myPosition, ChessBoard* const myUniverse, std::shared_ptr<BoardPoint> myParent)
		: BoardPoint(myPosition, myUniverse, myParent) {
			//forward to superclass
	}
	std::list<std::shared_ptr<BoardPoint>> getNeighbours(void) {
		//initialise neighbours and place them on the board, then return those new neighbours
		if(neighbours.size() == 0) {
			pos2D& myPos = this->getPosition(); //maintain encapsulation
			//printf("preparing neighbours for %s\n",posToString(myPos));
			for(int ang = 0; ang < 4; ++ang) {
				for(int j = 0; j < 2; ++j) {
					pos2D p;
					char x = std::get<0>(myPos);
					int y = std::get<1>(myPos);
					if(ang % 2) {
						//move 2 in x
						if(ang < 2) {
							x -= 2;
							if(x < 'A')
								break;
						} else {
							x += 2;
							if(x > MAX_X)
								break;
						}
						if(j % 2) {
							//move up 1 in y
							y++;
							if(y > MAX_Y)
								continue;
						} else {
							//move down 1 in y
							y--;
							if(y < 1)
								continue;
						}
					} else {
						//move 2 in y
						if(ang < 2)  {
							y -= 2;
							if(y < 1)
								break;
						} else {
							y += 2;
							if(y > MAX_Y)
								break;
						}
						if(j % 2) {
							//move left in x
							x--;
							if(x < 'A')
								continue;
						} else {
							//move right in x
							x++;
							if(x > MAX_X)
								continue;
						}
					}
					p = pos2D(x,y);
					if(this->universe->find(p) != this->universe->end()) {
						//this spot is not empty
						continue;
					} else {
						//add to list of neighbours
						this->neighbours.push_back(std::shared_ptr<BoardPoint>(
							new KnightPoint(p,this->universe,this->getptr())
							)
						);
						universe->emplace(p,neighbours.back());
						//printf("added %s to fringe; there are now %d neighbours\n",posToString(p),neighbours.size());
					}
				}
			}	
		}
		return this->neighbours;
	}
};


inline pos2D posTypify(const char pos[]) {
	assert(strlen(pos) == 2); //2D position with 26 bins for x, 10 for y
	assert(pos[0] >= 'A' && pos[0] <= MAX_X); //board extremeties
	assert(pos[1] >= '1' && pos[1] <= (MAX_Y + '0')); //board extremeties
	pos2D returnPos = pos2D(static_cast<char>(pos[0]),static_cast<char>(pos[1]) - '0');
	return returnPos;
}

inline void tracePath(std::shared_ptr<BoardPoint>& boardNode, bool ignore=true) {
	//visit the parents and print the path
	if(!ignore) {
		printf("%s ",posToString(boardNode->getPosition()));
		//dangling space for last move − not a big deal
	}
	if(boardNode->getParent() != nullptr) {
		tracePath(boardNode->getParent(), false);
	} //else finished printing path
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		//in Windows, code page must be changed to accomodate Unicode − chcp 1201
		printf("This program will return an optimally short path for a knight (♞) travelling from one"
			" chess-board position to another − Please specify these positions when running the program"
			", eg \n >>knightPaths A8 B7\n >>C7 B5 D6 B7\n");
	} else {
		//correct number of input arguments
		//printf("calculating path from %s to %s…\n",argv[1],argv[2]);
		pos2D startPos = posTypify(argv[1]);
		pos2D endPos = posTypify(argv[2]);
		ChessBoard* searchBoard = new ChessBoard();

		std::list<std::shared_ptr<BoardPoint>> fringe;
		fringe.push_back(std::shared_ptr<BoardPoint>(new KnightPoint(endPos,searchBoard)));
		//searching the reverse path saves us from reorganising the path at the end of the search

		do {
			//check if front of queue is the goal
			if(fringe.empty()) {
				printf("unable to find path after exhausting search space\n");
				break;
			} else if(fringe.front()->getPosition() == startPos) {
				//printf("goal found\n");
				tracePath(fringe.front());
				break;
			} else {
				//printf("considering %s\n",posToString(fringe.front()->getPosition()));
				//printf(" with parent %s…\n",posToString(fringe.front()->getParent()->getPosition()));
				fringe.splice(fringe.end(),fringe.front()->getNeighbours()); //combine lists in O(1) time
				//printf("popping front element (%s) off %d-long list\n",posToString(fringe.front()->getPosition()),fringe.size());
				fringe.pop_front(); //no need for that anymore
			}
		} while(true);

		delete searchBoard;
	}
	return 0;
}