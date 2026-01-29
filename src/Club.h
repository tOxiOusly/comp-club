#pragma once

#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

struct Client
{
	std::string name;
	int table = -1;
	int time = -1;
};

struct Table
{
	bool isBusy = false;
	std::string client;
	int busyFrom = -1;
	int time = 0;
	int money = 0;
};

class Club
{
private:
	int numTables;
	int openTime;
	int closeTime;
	int hourPrice;

	std::vector<Table> tables;
	std::unordered_map<std::string, Client> clients;
	std::queue<std::string> q;
public:
	Club(int tables, int open, int close, int price) :
		numTables(tables), openTime(open), closeTime(close), hourPrice(price), 
		tables(tables) {};

	void getEvent(std::string& event);

	void throwError(int id, const std::string& err, int time);

	void generateEvent(int id, const std::string& err, int time);

	void begin();

	void end();
};