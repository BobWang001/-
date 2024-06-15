#include "riscv.h"

extern Register_physical reg[2][num_registers];
extern map<type_label, int>map_global_register;//ȫ�ּĴ���������ŵ�ӳ��
extern map<int, Register_virtual*>map_global_register_position;//ȫ�ּĴ�����ŵ�ָ���ӳ��
extern map<int, Register_virtual*>map_local_register_position;//�ֲ�����Ĵ�����ŵ�ָ���ӳ��
extern map<int, std::string>map_physical_reg_name[2];//����Ĵ�����ŵ����Ƶ�ӳ��
extern vector<bool>physical_reg_usable[2];//����Ĵ����ܷ�ʹ��
extern vector<int>physical_reg_order[2];//���Ƿ�������Ĵ�����˳��
extern int total_register;

extern variable_table* global, * global_tail;
extern int total_global;//�洢����������
extern map<type_variables, int>map_global;//����������ŵ�ӳ��
extern map<int, int>map_register_local;//�Ĵ�����ŵ�������ŵ�ӳ��

extern instruction* start;//����ȫ�ֵ�ָ��
extern map<std::string, int>ins_num, cond_num;
extern int tot_instructions;//�ܵ�ָ����
extern set<int>ins_definied;//�ᶨ������Ĵ�����ָ��(��call)
extern set<int>ins_used;//��ʹ������Ĵ�����ָ��
extern set<int>ins_valuate;//���Rd��ֵ��ָ��
extern map<int, instruction*>map_instruction_position;//ָ���ŵ�ָ���ӳ��
extern map<int, pair<int, int> >map_ins_copy;//��¼copyָ���λ��

extern map<type_label, int>label_num;
extern map<int, int>label_ins;//label��ָ���ӳ��
extern vector<basic_block*>label_bb;//label��Ŷ�Ӧ��bb���
extern int tot_label;

extern functions* func_head, * func_tail;
extern map<type_label, int>map_function;
extern int tot_functions;//�ܵĺ�����