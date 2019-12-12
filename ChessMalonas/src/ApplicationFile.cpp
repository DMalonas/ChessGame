//============================================================================
// Name        : ChessMalonas.cpp
// Author      : Dimitrios Malwnas
// Version     : 4
// Copyright   : Your copyright notice
// Description : A chess programm in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <limits>

#include "../HeaderFiles/ChessGame.hpp" // https://stackoverflow.com/questions/13298550/eclipse-c-including-header-file-from-my-source-folder

using namespace std;





int main()
{
	Board b; //dhmiourghse Board object vale to turn = WHITE
	string s;
	bool newgame = true;
	cout << "Chess\n" << endl;
	cout << "Press key: " << endl;
	// www.cplusplus.com/forum/beginner/2624/
	int i;
	cin >> i;
	do {
		if (1 == i) {
			cout << "Load previous game\n"<<endl;
			b.setBoard();
			b.readFromBinFile("./record.dat");
		} else {
			b.setBoard();
		}
		cin.clear();
		//cin.ignore(numeric_limits<streamsize>::max(), '\n');
		for (;b.playGame(););
		cout << "Play again? (y|Y|yes|Yes : continue) , (Press any other key to exit)";
		cin >> s;
		//www.cplusplus.com/reference/string/string/compare/
		if (s.compare("y") != 0 && s.compare("Y") != 0 && s.compare("yes") != 0 && s.compare("Yes") != 0)
			newgame = false;
	} while(newgame);


	return 0;
}
