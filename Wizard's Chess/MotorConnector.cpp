#include "stdafx.h"
#include <iostream>
#include <string>
#include <math.h>

#include "MotorConnector.h"

using namespace System;
using namespace System::IO::Ports;



Point::Point()
 : x(0), y(0) {
};

Point::Point(int a, int b)
 : x(a), y(b) {
};

void Point::operator= (Point p){
	x = p.x;
	y = p.y;
}

Board::Board(){
	
	
	//System::Console::WriteLine("Type in a port name and hit ENTER");
	std::cout << "Type in a port name and hit ENTER\n";
	std::getline(std::cin,nameOfPort);
	std::cout << nameOfPort;

	discardSpaces[32];
	currentDiscardIndex = 0;
	int t = 0;

	for (int p = 17; p > 15; p--)
	{
		for (int q = 15; q >= 0; q--)
		{
			discardSpaces[t].x = p;
			discardSpaces[t].y = q;
			t++;
		}
	}

	int z = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			spacesTaken[z].x = i;
			spacesTaken[z].y = j;
			z++;
		}
	}
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 6; j < 8; j++)
		{
			spacesTaken[z].x = i;
			spacesTaken[z].y = j;
			z++;
		}
	}

	for (int d = 0; d < 32; d++)
	{
		printf("SPACES TAKEN %d: (%d, %d)\n",d,spacesTaken[d].x,spacesTaken[d].y);
	}
	for (int d = 0; d < 32; d++)
	{
		printf("DISCARD SPACES %d: (%d, %d)\n", d, discardSpaces[d].x, discardSpaces[d].y);
	}
};

void Board::makeMotion(std::string inputCode){
	String^ answer;
	String^ portName;
	int baudRate = 9600;
	// arduino settings
	std::string codeExample;
	String^ cExample;
	portName = gcnew String(nameOfPort.c_str());
	SerialPort^ arduino;
	arduino = gcnew SerialPort(portName, baudRate);
	// open port
	try
	{
		arduino->Open();
		std::cout << "The full code is: " << inputCode << std::endl;
		cExample = gcnew String(inputCode.c_str());
		arduino->WriteLine(cExample);
		arduino->Close();
	}
	catch (IO::IOException^ e)
	{
		Console::WriteLine(e->GetType()->Name + ": Port is not ready");
	}
	catch (ArgumentException^ e)
	{
		Console::WriteLine(e->GetType()->Name + ": incorrect port name syntax, must start with COM/com");
	}
	return;


};

std::string Board::getCode(bool magnetUp, int deltaX, int deltaY){

	//char firstChar;
	std::string firstChar;
	std::string sX;
	std::string sY;

	
	if (!magnetUp)
	{
		if (deltaX < 0 && deltaY < 0){
			firstChar = '0';
		}
		else if (deltaX < 0 && deltaY >= 0){
			firstChar = '1';
		}
		else if (deltaX >= 0 && deltaY < 0){
			firstChar = '2';
		}
		else if (deltaX >= 0 && deltaY >= 0){
			firstChar = '3';
		}
	}
	else{
		if (deltaX < 0 && deltaY < 0){
			firstChar = '4';
		}
		else if (deltaX < 0 && deltaY >= 0){
			firstChar = '5';
		}
		else if (deltaX >= 0 && deltaY < 0){
			firstChar = '6';
		}
		else if (deltaX >= 0 && deltaY >= 0){
			firstChar = '7';
		}
	}

	std::cout << "The first character is: " + firstChar + '\n';

	int dX = abs(deltaX);
	int dY = abs(deltaY);

	if (dX <= 9)
	{
		sX = "0" + std::to_string(dX);
	}
	else
	{
		sX = std::to_string(dX);
	}

	if (dY <= 9)
	{
		sY = "0" + std::to_string(dY);
	}
	else
	{
		sY = std::to_string(dY);
	}

	std::cout << "The delta X is: " + sX + '\n';
	std::cout << "The delta Y is: " + sY + '\n';

	std::string combinedString = firstChar + sX + sY;

	std::cout << "The combined string is: " + combinedString + '\n';

	return combinedString;
}

bool Board::destinationOccupied(int Row, int Col){
	for (int i = 0; i < 32; i++){
		if (spacesTaken[i].x == Row && spacesTaken[i].y == Col){
			std::cout << "DESTINATION OCCUPIED CHECK: X is " << spacesTaken[i].x << " ROW is " << Row << std::endl;
			std::cout << "DESTINATION OCCUPIED CHECK: Y is " << spacesTaken[i].y << " COL is " << Col << std::endl;
			return true;
		}
	}
	return false;
}

void Board::updateSpacesOccupied(bool isDiscarded, int startRow, int startCol, int endRow, int endCol){

	int endIndex;
	int startIndex;

	for (int i = 0; i < 32; i++){
		if (spacesTaken[i].x == startRow && spacesTaken[i].y == startCol)
		{
			startIndex = i;
		}
	}

	spacesTaken[startIndex].x = endRow;
	spacesTaken[startIndex].y = endCol;

	if (!isDiscarded){

		return;
	}

	for (int j = 0; j < 32; j++){
		if (spacesTaken[j].x == endRow && spacesTaken[j].y == endCol)
		{
			endIndex = j;
		}
	}

	

	spacesTaken[endIndex].x = discardSpaces[currentDiscardIndex].x; 
	spacesTaken[endIndex].y = discardSpaces[currentDiscardIndex].y;

	currentDiscardIndex++;

		return;


};	
Point Board::convertChessToGrid(int Row, int Col){
	Point g;
	g.x = (Row - 1) * 2;
	//g.x = (Row-1) * 2+1;
	g.y = (Col-1) * 2;
	//g.y = (Col - 1) * 2 + 1;
	return g;
}

void Board::moveStartingPiece(int startRow, int startCol, int endRow, int endCol){
	Point convertedStart = convertChessToGrid(startRow, startCol);
	Point convertedEnd = convertChessToGrid(endRow, endCol);
	std::cout << "The start position is (" << startRow << "," << startCol << ")\n";
	std::cout << "The end position is (" << endRow << "," << endCol << ")\n";

	std::cout << "The converted start position is (" << convertedStart.x << "," << convertedStart.y << ")\n";
	std::cout << "The converted end position is (" << convertedEnd.x << "," << convertedEnd.y << ")\n";

	std::string toPiece = getCode(false, convertedStart.x, convertedStart.y);
	makeMotion(toPiece);
	std::string diagStartOffset = getCode(true, 1, 1);
	makeMotion(diagStartOffset);
	std::string xMotion = getCode(true, convertedEnd.x-convertedStart.x, 0);
	//std::string xMotion = getCode(true, convertedEnd.x - convertedStart.x - 1, 0);
	//std::string xMotion = getCode(true, convertedEnd.x - convertedStart.x - 2, 0);
	makeMotion(xMotion);
	std::string yMotion = getCode(true, 0, convertedEnd.y - convertedStart.y);
	//std::string yMotion = getCode(true, 0, convertedEnd.y - convertedStart.y - 1);
	//std::string yMotion = getCode(true, 0, convertedEnd.y - convertedStart.y - 2);
	makeMotion(yMotion);
	std::string diagEndOffset = getCode(true, -1, -1);
	//std::string diagEndOffset = getCode(true, -2, -2);
	makeMotion(diagEndOffset);
	std::string sendHome = getCode(false, -convertedEnd.x, -convertedEnd.y);
	//std::string sendHome = getCode(false, -convertedEnd.x+1, -convertedEnd.y+1);
	makeMotion(sendHome);


}

void Board::discardPiece(int Row, int Col){
	Point converted = convertChessToGrid(Row, Col);
	int x = discardSpaces[currentDiscardIndex].x;
	int y = discardSpaces[currentDiscardIndex].y;


	std::cout << "The converted start position is (" << converted.x << "," << converted.y << ")\n";
	std::cout << "The converted end position is (" << x << "," << y << ")\n";

	std::string toPiece = getCode(false, converted.x, converted.y);
	makeMotion(toPiece);
	std::string diagStartOffset = getCode(true, 1, 1);
	makeMotion(diagStartOffset);
	std::string xMotion = getCode(true, 14 - converted.x, 0);
	makeMotion(xMotion);
	std::string yMotion = getCode(true, x-15, y - converted.y-1);
	makeMotion(yMotion);
	std::string sendHome = getCode(false, -x, -y);
	makeMotion(sendHome);


}

void Board::moveDirector(int startRow, int startCol, int endRow, int endCol)
{
	//startRow += 1;
	//startCol += 1;
	//endRow += 1;
	//endCol += 1;
	std::cout << "(" << startRow << "," << startCol << ") to (" << endRow << "," << endCol << ")";
	std::cout << "Success\n";

	if (destinationOccupied(endRow-1, endCol-1)){
		discardPiece(endRow, endCol);
		std::cout << "Destination Occupied\n";
		//moveStartingPiece(startRow,startCol,endRow,endCol);
		updateSpacesOccupied(true,startRow-1, startCol-1, endRow-1, endCol-1);
		moveStartingPiece(startRow, startCol, endRow, endCol);
	}
	else{
		updateSpacesOccupied(false, startRow - 1, startCol - 1, endRow - 1, endCol - 1);
		moveStartingPiece(startRow, startCol, endRow, endCol);
	}
	return;
}

