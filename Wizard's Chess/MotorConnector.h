class Point{
public:
	int x;
	int y;

	Point();
	Point(int, int);
private:
	void	operator= (Point p);
};

class Board{
	public:

	Point spacesTaken[32];
	Point discardSpaces[32];
	int currentDiscardIndex;
	bool servoState;
	Point currentMotorPosition;
	std::string nameOfPort;
	Board();

	void makeMotion(std::string);
	void moveDirector(int, int, int, int);
	std::string getCode(bool, int, int);
	bool destinationOccupied(int Row, int Col);
	void updateSpacesOccupied(bool,int,int,int,int);
	Point convertChessToGrid(int,int);
	void moveStartingPiece(int startRow, int startCol, int endRow, int endCol);
	void discardPiece(int,int);

};