#include <iostream>
#include <string>
using namespace std;
#define MAX_ID 1024

struct symbol_info{
	string name;
	int type;
	bool defined = false;
} ;

class symbol_table{

public:
    symbol_info table[MAX_ID];
	int size;

	int lookup(string name);
	int insert(string name, int type);
	//int gettoken(string name);
	string &getname(int pos);
	int set_type(int pos, int type);
	int get_type(int pos);
};
