#include "riscv.h"

variable_table* global, * global_tail;
int total_global;//�洢ȫ�ֱ���������
map<type_variables, int>map_global;//����������ŵ�ӳ��

instruction* start;//����ȫ�ֵ�ָ��
map<std::string, int>ins_num;

functions* func_head, * func_tail;
map<type_label, int>map_function;

void init_definition()//��ʼ������
{
	total_global = 0;
	ins_num = {
	{"global",0},{"load",1},{"store",2},{"alloca",3},{"getelementptr",4},
	{"add",5},{"sub",6},{"mul",7},{"sdiv",8},{"and",9},{"or",10},{"xor",11},
	{"icmp",12}, {"fcmp",13},{"br",14},{"define",15},{"call",16},{"ret",17}
	};
	//��ʼ������ͷβָ��
	func_head->pre = NULL; func_head->next = func_tail;
	func_tail->pre = func_head; func_tail->next = NULL;
}

int main()
{
	init_definition();
	std::ifstream file("test.ll");//���ļ�
	if (!file.is_open())
	{
		std::cerr << "�޷����ļ�" << std::endl;
		return 1;
	}
	std::string line;
	std::string s;
	functions* lst_func = func_head, * now_func = NULL;//ָ����һ��������ָ��ǰ����
	bool in_func = 0;//�Ƿ��ں����ڲ�����ʼ��Ϊ��
	while(getline(file,line))//���ж���
	{
		std::istringstream word(line);
		while (word >> s)
		{
			int option = -1;//������û��ʶ���ָ����Ϊ-1
			if (ins_num.count(s) == 0)
				continue;
			else option = ins_num[s];
			read(option, line, now_func, in_func);
		}
	}
	return 0;
}