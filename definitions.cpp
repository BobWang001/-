#include "riscv.h"

extern Register reg[num_registers];

extern variable_table* global, * global_tail;
extern int total_global;//�洢ȫ�ֱ���������
extern map<type_variables, int>map_global;//����������ŵ�ӳ��

extern instruction* start;//����ȫ�ֵ�ָ��
extern map<std::string, int>ins_num;

extern functions* func_head, * func_tail;
extern map<type_label, int>map_function;