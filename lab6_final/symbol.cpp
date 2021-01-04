#include "symbol.h"
using namespace std;

int symbol_table::lookup(string name)
{
	for (int i = 0; i < size; i++)
		if (table[i].name == name)
			return i;
	return -1;
}

int symbol_table::insert(string name, int type)
{
	if (size >= MAX_ID)
		return -1;
	table[size].name = name;
	table[size].type = type;
	size++;
	return size - 1;
}


string& symbol_table::getname(int pos)
{
	return table[pos].name;
}

int symbol_table::set_type(int pos, int type)
{
	if (pos < 0 || pos >= size)
	{
		cerr << "Bad identifier" << endl;
		return -1;
	}

	table[pos].type = type;
	return 0;
}

int symbol_table::get_type(int pos)
{
	if (pos < 0 || pos >= size)
	{
		cerr << "Bad identifier" << endl;
		return -1;
	}

	return table[pos].type;
}