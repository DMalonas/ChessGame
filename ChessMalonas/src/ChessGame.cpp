#include <limits>
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>


#include "../HeaderFiles/ChessGame.hpp"

#define INPUTWAY 1

using namespace std;

Square::Square()
{
	piece = EMPTY;
	color = NONE;
}


void Square::setSpace(Square* space)
{
	color = space->getColor();
	piece = space->getPiece();
}

void Square::setEmpty()
{
	color = NONE;
	piece = EMPTY;
}

Piece Square::getPiece()
{
	return piece;
}

Color Square::getColor()
{
	return color;
}

void Square::setPieceAndColor(Piece p, Color c)
{
	piece = p;
	color = c;

}


/*
 * print board
 */
void Board::printBoard() {
	using namespace std;
	cout << " y:  0  1  2  3  4  5  6  7 " << endl << "x:" << endl; //Print the number of columns
	//8X8 double loop to populate the square[][] with info for the content of each spot on the board
	for (int i = 0; i < 8; i++) {
		cout << " " << i << "   "; //Print number of each line
		for (int j = 0; j < 8; j++) {
			Piece p = square[i][j].getPiece(); //Get type of piece
			Color c = square[i][j].getColor();

			if (KING == p) {
				if (WHITE == c) {
					cout << " K ";
				} else {
					cout << " k ";
				}

			} else if (QUEEN == p) {
				if(WHITE == c) {
					cout << " Q ";
				} else {
					cout << " q ";
				}
			} else if (BISHOP == p) {
				if(WHITE == c) {
					cout << " B ";
				} else {
					cout << " b ";
				}
			} else if (KNIGHT == p) {
				if(WHITE == c) {
					cout << " H ";
				} else {
					cout << " h ";
				}
			} else if (ROOK == p) {
				if(WHITE == c) {
					cout << " R ";
				} else {
					cout << " r ";
				}
			} else if (PAWN == p) {
				if(WHITE == c) {
					cout << " P ";
				} else {
					cout << " p ";
				}
			} else if(EMPTY == p) {
				cout << " " << "_" << " ";
			} else {
				cout << "Wrong";
			}
		//}

/*
			switch (p)
			{
			case KING: (c == WHITE) ? cout << " K " : cout << " k ";
				break;
			case QUEEN: (c == WHITE) ? cout << " Q " : cout << " q ";
				break;
			case BISHOP:(c == WHITE) ? cout << " B " : cout << " b ";
				break;
			case KNIGHT:(c == WHITE) ? cout << " H " : cout << " h ";
				break;
			case ROOK: (c == WHITE) ? cout << " R " : cout << " r ";
				break;
			case PAWN: (c == WHITE) ? cout << " P " : cout << " p ";
				break;
			case EMPTY: cout << " " << "_" << " "; // ASCII kwdikos gia to symvolo twn tetragwnwn
				break;
			default: cout << "XXX";
				break;
			}
*/
		}
		cout << endl;
	}


}

/**
 * Experimental method for
 * selection control with
 * the keyboard arrows
 */
void loop(int &x, int &y) {
	using namespace std;
	int columnCounter = 0;
	int rowCounter = 0;
	while(1	){
		system ("cls");
		//system("pause");
		if(GetAsyncKeyState(VK_DOWN)) {
			rowCounter++;
			cout << "VK-DOWN" << endl;
		}
		if(GetAsyncKeyState(VK_UP)){
			cout << "VK-UP" << endl;
			rowCounter--;
		}
		if(GetAsyncKeyState(VK_LEFT)) {
			cout << "VK-LEFT" << endl;
			columnCounter--;
		}
		if(GetAsyncKeyState(VK_RIGHT)) {
			cout << "VK-RIGHT" << endl;
			columnCounter++;
		}
		if (GetAsyncKeyState('\r')) {
			break;
		}
		cout << "\nx = " << rowCounter << "\ty = " << columnCounter << endl;
	}
	x = rowCounter;
	y = columnCounter;
}






/*
 * The doMove method calculates the
 * coordinates of the initial and
 * final positions
 */
bool Board::doMove() {
	using namespace std;
	string move;
	int x1, x2, y1, y2;
	bool stop = false;

	do {
		if (WHITE == turn) {
			 cout << "White move" << endl;
		} else {
			cout << "Black move" << endl;
		}

		cout << "Give 4 input string (e.g. 0103): " << endl;


		//gist.github.com/jpkrause/4a1aa400d45197ca3253
		//cin.clear();
		//cin.ignore(numeric_limits<streamsize>::max(), '\n');

		//http://www.cplusplus.com/articles/4z18T05o/
		system("CLS");
		/*
		 * The integer value of ASCII 0 is 48.
		 * The ASCII values inside the string, move,
		 * will be between '0' and '9'.
		 * The integer value of '1' is 49 so
		 * 49 - 48 = 1
		 */
		if (INPUTWAY) {
			cin >> move; //Read keyboard input
			x1 = move[0] - 48;
			y1 = move[1] - 48;
			x2 = move[2] - 48;
			y2 = move[3] - 48;
		} else if (!INPUTWAY) {
			loop(x1, y1);
			system ("cls");
			system("pause");
			loop(x2, y2);
		}




		//Check if the player picked the right color to play.
		if (getSquare(x1, y1)->getColor() == turn)
		{

			//check if the move is valid.
			if (makeMove(x1, y1, x2, y2) == false)
			{
				cout << "Wrong move" << endl;
			}
			else
			{
				Square* src = getSquare(x1, y1);
				Square* dest = getSquare(x2, y2);
				//Check if it is a check mate
				if (getSquare(x2, y2)->getPiece() == KING)
				{
					if (getSquare(x1, y1)->getColor() == WHITE)
					{
						cout << "white wins" << endl;
						return false;
					}
					else if (getSquare(x1, y1)->getColor() == BLACK)
					{
						cout << "black wins" << endl;
						return false;
					}
				}
				dest->setSpace(src);
				src->setEmpty();
				stop = true;
				saveToBinFile("./record.dat");
			}
		}
		else
			cout << "Wrong piece, please try again" << endl;
	} while (!stop); //while(true)



	if (turn == BLACK)
		turn = WHITE;
	else
		turn = BLACK;

	return true;

}

/*
 * setBoard() sets the values
 * of the Square elements inside
 * square[][]
 */
void Board::setBoard()
{
	//Instantiate all Square objects that are of white color that are not pawns or empty.
	square[0][0].setPieceAndColor(ROOK, WHITE);
	square[1][0].setPieceAndColor(KNIGHT, WHITE);
	square[2][0].setPieceAndColor(BISHOP, WHITE);
	square[3][0].setPieceAndColor(QUEEN, WHITE);
	square[4][0].setPieceAndColor(KING, WHITE);
	square[5][0].setPieceAndColor(BISHOP, WHITE);
	square[6][0].setPieceAndColor(KNIGHT, WHITE);
	square[7][0].setPieceAndColor(ROOK, WHITE);

	//Instantiate all the Square objects of black color that are not pawns or empty.
	square[0][7].setPieceAndColor(ROOK, BLACK);
	square[1][7].setPieceAndColor(KNIGHT, BLACK);
	square[2][7].setPieceAndColor(BISHOP, BLACK);
	square[3][7].setPieceAndColor(QUEEN, BLACK);
	square[4][7].setPieceAndColor(KING, BLACK);
	square[5][7].setPieceAndColor(BISHOP, BLACK);
	square[6][7].setPieceAndColor(KNIGHT, BLACK);
	square[7][7].setPieceAndColor(ROOK, BLACK);

	//Instantiate all Square objects that are pawns
	for (int i = 0; i < 8; i++)
	{
		square[i][1].setPieceAndColor(PAWN, WHITE); //white soldiers
		square[i][6].setPieceAndColor(PAWN, BLACK); //black soldiers

	}
	//Instantiate all Square objects that are piece = EMPTY AND color = NONE
	for (int i = 2; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
			square[j][i].setPieceAndColor(EMPTY, NONE);

	}

	//Set x and y coordinates for each Square object
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			square[i][j].setX(i);
			square[i][j].setY(j);
		}

}

/*
 * playGame calls the methods for
 * printing the board and executing a move
 */
bool Board::playGame()
{
	system("cls"); //clear screen everytime we make move
	printBoard(); //print boards
	return doMove();
}

/*

 * The method moveKing() ensures that the
 * king will move exactly one step
 * in x AND exactly one step step in y
 * or, exactly one step in x AND 0
 * in y, or exactly one step in Y AND 0
 * in x
 */
bool Board::checkIfKing(Square* thisKing, Square* thatSpace) {

	if (abs(thatSpace->getX() - thisKing->getX()) == 1 || abs(thatSpace->getX() - thisKing->getX()) == 0 )
		if (abs(thatSpace->getY() - thisKing->getY()) == 1 || abs(thatSpace->getY() - thisKing->getY()) == 0)
		{
			return true;
		} else return false;
	else return false;
}

/*
 * Check if we have a legal queen move.
 * The queen is a combination of a rook
 * and a bishop
 */
bool Board::checkIfQueen(Square* thisQueen, Square* thatSpace) {

	if (thisQueen->getX() != thatSpace->getX() || thisQueen->getY() != thatSpace->getY())
	{

		if ((thisQueen->getX() == thatSpace->getX()) || (thisQueen->getY() == thatSpace->getY()))
		{
			return checkIfRook(thisQueen, thatSpace);
		}
		else if (abs(thisQueen->getX() - thatSpace->getX()) == abs(thisQueen->getY() - thatSpace->getY()))
		{
			return checkIfBishop(thisQueen, thatSpace);
		}
		else
			return false;
	}
	return true;
}

/*
 * Check if we have a legal the bishop move
 * thisBishop : Square object, the Square object in the start spot
 * thatSpace : Square object, the Square object in the final spot
 *
 */
bool Board::checkIfBishop(Square* thisBishop, Square* thatSpace)
{
	/*
	 * The rook moves in such a way, that the absolute
	 * value of the difference between the starting line
	 * and the destination line is always equal with the
	 * absolute value of the difference of the starting
	 * column minus the destination column.
	 */
	if (abs(thisBishop->getX() - thatSpace->getX()) == abs(thisBishop->getY() - thatSpace->getY()))
	{
			if (false == moveDiagonal(thisBishop,thatSpace))
				return false;
	}
	else
	{
		return false;
	}
	return true;
}

/*
 * checkIfKnight checks if we have a legal knight piece
 * thisKnight : Square object, the Square object in the starting position
 * thatSpace : Square object, the Square object in the destination position
 * return bool
 *
 */
bool Board::checkIfKnight(Square* thisKnight, Square* thatSpace)
{
	/*
	 * (If the absolute value of the difference between the starting
	 * line minus the destination line is 2 AND the absolute value
	 * of the difference between the starting and destination column
	 * is 1) OR (the absolute value of the difference between the
	 * starting and destination line is 1 AND the absolute value between
	 * the starting and destination column is 2) then move the horse
	 * else return false
	 */
	if ((abs(thisKnight->getX() - thatSpace->getX()) == 2 && abs(thisKnight->getY() - thatSpace->getY()) == 1) ||
		(abs(thisKnight->getX() - thatSpace->getX()) == 1 && abs(thisKnight->getY() - thatSpace->getY()) == 2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Move diagonally
 */
bool Board::moveDiagonal(Square *src, Square *dest)
{
	int xIncrement = (dest->getX() - src->getX()) / (abs(dest->getX() - src->getX()));
	int yIncrement = (dest->getY() - src->getY()) / (abs(dest->getY() - src->getY()));

	for (int i = 1; i < abs(src->getX() - dest->getX()); i++)
	{
		if (square[src->getX() + xIncrement*i][src->getY() + yIncrement*i].getColor() != NONE)
			return false;
	}
	return true;
}


/**
 * Move backwards
 */
bool Board::moveBackwards(Square *src, Square *dest)
{
	int yDecrement = -1;
	for (int i = src->getY() + yDecrement; i != dest->getY(); i += yDecrement)
		{

			if (square[src->getX()][i].getColor() != NONE)
				return false;
		}
		return true;
}



/**
 * Move forward
 */
bool Board::moveForward(Square *src, Square *dest)
{
	int yIncrement = 1;
	for (int i = src->getY() + yIncrement; i != dest->getY(); i += yIncrement)
		{

			if (square[src->getX()][i].getColor() != NONE)
				return false;
		}
		return true;
}


/**
 * Move forward or backwards
 */
bool Board::moveForwardOrBackwards(Square *src, Square *dest)
{
	int yIncrement = (dest->getY() - src->getY()) / (abs(dest->getY() - src->getY()));
	if (-1 == yIncrement)
	{
		return moveBackwards(src, dest);
	}
	return moveForward(src, dest);

}


/**
 * Move up
 */
bool Board::moveUp(Square *src, Square *dest)
{
	int xDecrement = -1;
	for (int i = src->getX() + xDecrement; i != dest->getX(); i += xDecrement)
	{
		if (square[i][dest->getY()].getColor() != NONE)
			return false;
	}
	return true;
}


/**
 * Move down
 */
bool Board::moveDown(Square *src, Square *dest)
{
	int xIncrement = 1;
	for (int i = src->getX() + xIncrement; i != dest->getX(); i += xIncrement)
	{
		if (square[i][dest->getY()].getColor() != NONE)
			return false;
	}
	return true;
}


/**
 * Move up or down
 */
bool Board::moveUpDown(Square *src, Square *dest)
{
	int xIncrement = (dest->getX() - src->getX()) / (abs(dest->getX() - src->getX()));
	if (-1 == xIncrement)
	{
		return moveUp(src, dest);
	}
	return moveDown(src, dest);
}








/**
 * Check if we have a legal rook move.
 * thisRook : Square object in the starting position
 * thatSpace : Square object in the destination position
 * The method returns a bool value. The method checks
 * that we stay in the same row AND change column(s)
 * OR that we stay in the same column AND change row(s)
 */
bool Board::checkIfRook(Square* thisRook, Square* thatSpace)
{
	if (((thisRook->getX() == thatSpace->getX()) && (thisRook->getY() != thatSpace->getY())))
	{
		if (false == moveForwardOrBackwards(thisRook, thatSpace))
			return false;
	}
	else if ((thisRook->getY() == thatSpace->getY()) && (thisRook->getX() != thatSpace->getX()))
	{
		if (false == moveUpDown(thisRook, thatSpace))
			return false;
	}
	else
		return false;
	return true;
}



/**
 *Check if we have a legal pawn move
 */
bool Board::checkIfPawn(Square* thisPawn, Square* thatSpace) {

	//check if white pawn
	if (thisPawn->getColor() == WHITE)
	{
		/**
		 * move forward only in empty spot 1, or two spots ahead of starting position
		 * if the starting row is equal with the destination row, namely x1 =x2
		 * AND
		 * ((the destination column is equal to the the starting column + 1, namely y2 = y1 + 1)
		 * OR ((the destination column is equal to the starting column + 2) AND (the starting column is equal to 1))
		 * AND the destination square is empty
		 * then the white pawn has a legal move
		 */
		if ((thisPawn->getX() == thatSpace->getX()) &&
		    ((thatSpace->getY() == thisPawn->getY() + 1) || ((thatSpace->getY() == thisPawn->getY() + 2) && (thisPawn->getY() == 1))) &&
			(thatSpace->getColor() == NONE))
		{
			return true;
		}



		/**
		 * white pawn takes black piece.
		 * (If the destination row is equal to the starting row + 1, namely x2 = x1 + 1 (one down)
		 * OR if the destination row is equal to the starting row - 1, namely x2 = x1 - 1 (one up))
		 * AND (the destination square has color black) then we can take the piece
		 */
		else if ((thisPawn->getX() + 1 == thatSpace->getX() || thisPawn->getX() - 1 == thatSpace->getX()) && thisPawn->getY() + 1 == thatSpace->getY()  && thatSpace->getColor() == BLACK)
		{
			return true;
		}
		else
			return false;
	}
	else
		if (thisPawn->getColor() == BLACK)
		{
			if  ((thisPawn->getX() == thatSpace->getX()) &&
				    ((thatSpace->getY() == thisPawn->getY() - 1) || ((thatSpace->getY() == thisPawn->getY() - 2) && (thisPawn->getY() == 6))) &&
					(thatSpace->getColor() == NONE))
			{
				return true;
			}
			else
				if ((thisPawn->getX() + 1 == thatSpace->getX() || thisPawn->getX() - 1 == thatSpace->getX()) && thisPawn->getY() - 1 == thatSpace->getY()  && thatSpace->getColor() == WHITE)
				{
					return true;
				}
				else
					return false;
		}
		else
			return false;
}



/**
 * makeMove controls if the user's move is within
 * the board's bounds and if it is, it calls
 * the appropriate method to complete the
 * move given.
 */
bool Board::makeMove(int x1, int y1, int x2, int y2) {
	//Checking for turns will be done previous to this
	using namespace std;
	if (x1 < 0 || x1>7 || y1 < 0 || y1>7 || x2 < 0 || x2>7 || y2 < 0 || y2>8)
	{
		std::cout << "Out of limits move" << std::endl;
		return false;
	}

	Square* src = getSquare(x1, y1);
	Square* dest = getSquare(x2, y2);

	//Cannot move to spot already occupied by another same colour piece.
	if (src->getColor() == dest->getColor()) // && dest->getColor() != NONE)
	{
		std::cout << "Same color pieces" << std::endl;
		return false;
	}


	switch (src->getPiece())
	{
	case KING: return checkIfKing(src, dest);
		break;
	case QUEEN: return checkIfQueen(src, dest);
		break;
	case BISHOP: return checkIfBishop(src, dest);
		break;
	case KNIGHT: return checkIfKnight(src, dest);
		break;
	case ROOK: return checkIfRook(src, dest);
		break;
	case PAWN: return checkIfPawn(src, dest);
		break;
	case EMPTY: std::cout << "You do not have a piece there" << std::endl;  return false;
		break;
	default: std::cerr << "Something went wrong in the switch statement in makeMove()" << std::endl;
		break;
	}
	return false;
}


void Board :: saveToBinFile(string strFile)
{
	using namespace std;
	ofstream fs;	//The ofstream class provides a member function named write that allows for information to be written in binary form to the stream. http://www.eecs.umich.edu/courses/eecs380/HANDOUTS/cppBinaryFileIO-2.html
	fs.open(strFile, ios::out | ios::binary);
	if (!fs.is_open()) {
		cout << "cannot open file" << strFile << endl;
	} else {
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				fs.write(reinterpret_cast<char*>(&square[i][j]), sizeof(square));
			}


		}
	}
	fs.close();
}


void Board::readFromBinFile(string strFile)
{
	ifstream binaryFile;
	int size = 0;

	binaryFile.open(strFile, ios::in | ios::binary);
	binaryFile.seekg(0, ios::end); // check where the file ends
	size = (int)binaryFile.tellg();
	binaryFile.seekg(0, ios::beg); //position pointer in the beginning of the file

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (binaryFile.tellg() >= size)
			{
				break;
			}
			binaryFile.read((char*)(&square[i][j]), sizeof(square));
		}
		if (binaryFile.tellg() >= size)
		{
			break;
		}
	}
	binaryFile.close();
}
