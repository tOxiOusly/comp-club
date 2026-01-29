#include "Club.h"
#include "Time.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void Club::throwError(int id, const std::string& err, int time)
{
	std::cout << minInTime(time) << " " << id << " " << err << std::endl;
}

void Club::generateEvent(int id, const std::string& ev, int time)
{
	std::cout << minInTime(time) << " " << id << " " << ev << std::endl;
}

void Club::getEvent(std::string& event)
{
	std::istringstream iss(event);
	int id;
	std::cout << event << std::endl;
	std::string time_str;
	if (!(iss >> time_str >> id)) std::exit(0);

	if (timeInMin(time_str) == -1) std::exit(0);
	int time_int = timeInMin(time_str);

	switch (id)
	{
	case 1:
	{
		std::string name_user;
		if (!(iss >> name_user)) std::exit(0);
		if (time_int < openTime || time_int >= closeTime)
		{
			throwError(13, "NotOpenYet", time_int);
			return;
		}
		if (clients.find(name_user) != clients.end())
		{
			throwError(13, "YouShallNotPass", time_int);
			return;
		}
		Client client;
		client.name = name_user;
		clients[name_user] = client;
		break;
	}
	case 2:
	{
		std::string name_user;
		int table_number;
		if (!(iss >> name_user >> table_number)) std::exit(0);
		if (clients.find(name_user) == clients.end())
		{
			throwError(13, "ClientUnknown", time_int);
			return;
		}
		if (tables[table_number - 1].isBusy)
		{
			throwError(13, "PlaceIsBusy", time_int);
			return;
		}

		Client& client = clients[name_user];
		if (client.table != -1)
		{
			int duration = time_int - tables[client.table].busyFrom;
			tables[client.table].time += duration;

			int h = (duration + 59) / 60;
			tables[client.table].money += h * hourPrice;

			tables[client.table].isBusy = false;
			tables[client.table].client.clear();
			tables[client.table].busyFrom = -1;
		}

		tables[table_number - 1].isBusy = true;
		tables[table_number - 1].client = name_user;
		tables[table_number - 1].busyFrom = time_int;
		client.table = table_number - 1;
		break;
	}
	case 3:
	{
		std::string name_user;
		if (!(iss >> name_user)) std::exit(0);
		if (clients.find(name_user) == clients.end())
		{
			throwError(13, "ClientUnknown", time_int);
			return;
		}
		bool vacantPlace = false;
		for (Table& t : tables) vacantPlace = vacantPlace || !t.isBusy;
		if (vacantPlace)
		{
			throwError(13, "ICanWaitNoLonger!", time_int);
			return;
		}
		if (q.size() >= numTables)
		{
			generateEvent(11, name_user, time_int);
			clients.erase(name_user);
			return;
		}
		q.push(name_user);
		break;
	}
	case 4:
	{
		std::string name_user;
		if (!(iss >> name_user)) std::exit(0);
		if (clients.find(name_user) == clients.end())
		{
			throwError(13, "ClientUnknown", time_int);
			return;
		}
		Client& client = clients[name_user];
		if (client.table != -1)
		{
			Table& table = tables[client.table];
			int duration = time_int - table.busyFrom;
			table.time += duration;

			int h = (duration + 59) / 60;
			table.money += h * hourPrice;

			tables[client.table].isBusy = false;
			tables[client.table].client.clear();
			tables[client.table].busyFrom = -1;

			if (!q.empty())
			{
				std::string next = q.front();
				q.pop();
				table.isBusy = true;
				table.busyFrom = time_int;
				table.client = next;

				clients[next].table = client.table;
				generateEvent(12, next + " " + std::to_string(client.table + 1), time_int);
			}
		}
		clients.erase(name_user);
		break;
	}
	default:
		std::exit(0);
	}
}

void Club::end()
{
	std::vector<std::string> remain;

	for (auto& t : tables)
	{
		if (t.isBusy)
		{
			int duration = closeTime - t.busyFrom;
			t.time += duration;
			int hours = (duration + 59) / 60;
			t.money += hours * hourPrice;
			t.isBusy = false;
			t.client.clear();
			t.busyFrom = -1;
		}
	}

	for (auto& [name, c] : clients)
	{
		remain.push_back(name);

	}

	std::sort(remain.begin(), remain.end());

	for (auto& name : remain)
	{
		generateEvent(11, name, closeTime);
	}

	std::cout << minInTime(closeTime) << std::endl;

	for (int i = 0; i < numTables; ++i)
	{
		std::cout << (i + 1) << " " << tables[i].money << " " << minInTime(tables[i].time) << std::endl;
	}
}

void Club::begin()
{
	std::cout << minInTime(openTime) << std::endl;
}
