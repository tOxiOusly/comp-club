#include <fstream>
#include <iostream>
#include "Time.h"
#include "Club.h"

int main(int argc, char* argv[])
{
	if (argc < 2) return 1;

	std::ifstream in(argv[1]);
	if (!in) return 2;
	int table;
	if (!(in >> table))
	{
		return 0;
	}
	std::string open;
	std::string close;
	if (!(in >> open >> close))
	{
		return 0;
	}
	int openInt = timeInMin(open);
	int closeInt = timeInMin(close);
	if (openInt == -1 || closeInt == -1)
	{
		std::cout << open << " " << close << std::endl;
		return 0;
	}
	int price;
	if (!(in >> price))
	{
		return 0;
	}


	Club club(table, openInt, closeInt, price);

	club.begin();

	std::string input;
	std::getline(in, input);

	while (std::getline(in, input))
	{
		if (input.empty()) continue;
		club.getEvent(input);
	}

	club.end();
}