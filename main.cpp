#include "riscv.h"

variable_table* global = new variable_table, * global_tail;
int total_global;//存储全局变量的数量
map<type_variables, int>map_global;//变量名到编号的映射

instruction* start = new instruction;//属于全局的指令
map<std::string, int>ins_num;

functions* func_head = new functions, * func_tail = new functions;
map<type_label, int>map_function;

void init_definition()//初始化定义
{
	total_global = 0;
	ins_num = {
	{"global",0},{"load",1},{"store",2},{"alloca",3},{"getelementptr",4},
	{"add",5},{"sub",6},{"mul",7},{"sdiv",8},{"and",9},{"or",10},{"xor",11},
	{"icmp",12}, {"fcmp",13},{"br",14},{"define",15},{"call",16},{"ret",17}
	};
	//初始化函数头尾指针
	func_head->pre = NULL; func_head->next = func_tail;
	func_tail->pre = func_head; func_tail->next = NULL;
	//初始化全局变量尾指针
	global_tail = global;
}

int main()
{
	init_definition();
	std::ifstream file("test.ll");//打开文件
	if (!file.is_open())
	{
		std::cerr << "无法打开文件" << std::endl;
		return 1;
	}
	std::string line;
	std::string s;
	functions* lst_func = func_head, * now_func = NULL;//指向上一个函数，指向当前函数
	bool in_func = 0;//是否在函数内部，初始化为否
	while(getline(file,line))//逐行读入
	{
		std::istringstream word(line);
		while (word >> s)
		{
			int option = -1;//操作，没有识别出指令视为-1
			if (ins_num.count(s) == 0)
				continue;
			else option = ins_num[s];
			read(option, line, now_func, in_func);
		}
	}
	return 0;
}