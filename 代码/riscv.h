#pragma once
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>
#include <stack>
#include <queue>
/*type of variables*/
#define type_variables std::string
#define type_registers int
#define type_label std::string
/*type of spilting weight*/
#define type_spliting_weight long long
/*type of register typies*/
#define i32 0
#define float32 1
#define i64 2
/*type of instructions*/
#define ins_global 0
#define ins_load 1
#define ins_store 2
#define ins_alloca 3
#define ins_getelementptr 4
#define ins_add 5
#define ins_fadd 6
#define ins_sub 7
#define ins_fsub 8
#define ins_mul 9
#define ins_fmul 10
#define ins_sdiv 11
#define ins_fdiv 12
#define ins_srem 13
#define ins_frem 14
#define ins_fneg 15
#define ins_icmp 16
#define ins_fcmp 17
#define ins_br 18
#define ins_define 19
#define ins_call 20
#define ins_ret 21
#define ins_label 22
#define ins_unreachable 23
#define ins_sitofp 24
#define ins_fptosi 25

/*number the registers*/
#define num_registers 32
/*����*/
#define x0 0
#define x1 1
#define x2 2
#define x3 3
#define x4 4
#define x5 5
#define x6 6
#define x7 7
#define x8 8
#define x9 9
#define x10 10
#define x11 11
#define x12 12
#define x13 13
#define x14 14
#define x15 15
#define x16 16
#define x17 17
#define x18 18
#define x19 19
#define x20 20
#define x21 21
#define x22 22
#define x23 23
#define x24 24
#define x25 25
#define x26 26
#define x27 27
#define x28 28
#define x29 29
#define x30 30
#define x31 31

#define zero x0
#define ra x1
#define sp x2
#define gp x3
#define tp x4
#define t0 x5
#define t1 x6
#define t2 x7
#define s0 x8
#define s1 x9
#define a0 x10
#define a1 x11
#define a2 x12
#define a3 x13
#define a4 x14
#define a5 x15
#define a6 x16
#define a7 x17
#define s2 x18
#define s3 x19
#define s4 x20
#define s5 x21
#define s6 x22
#define s7 x23
#define s8 x24
#define s9 x25
#define s10 x26
#define s11 x27
#define t3 x28
#define t4 x29
#define t5 x30
#define t6 x31
/*������*/
#define f0 0
#define f1 1
#define f2 2
#define f3 3
#define f4 4
#define f5 5
#define f6 6
#define f7 7
#define f8 8
#define f9 9
#define f10 10
#define f11 11
#define f12 12
#define f13 13
#define f14 14
#define f15 15
#define f16 16
#define f17 17
#define f18 18
#define f19 19
#define f20 20
#define f21 21
#define f22 22
#define f23 23
#define f24 24
#define f25 25
#define f26 26
#define f27 27
#define f28 28
#define f29 29
#define f30 30
#define f31 31

#define ft0 f0
#define ft1 f1
#define ft2 f2
#define ft3 f3
#define ft4 f4
#define ft5 f5
#define ft6 f6
#define ft7 f7
#define fs0 f8
#define fs1 f9
#define fa0 f10
#define fa1 f11
#define fa2 f12
#define fa3 f13
#define fa4 f14
#define fa5 f15
#define fa6 f16
#define fa7 f17
#define fs2 f18
#define fs3 f19
#define fs4 f20
#define fs5 f21
#define fs6 f22
#define fs7 f23
#define fs8 f24
#define fs9 f25
#define fs10 f26
#define fs11 f27
#define ft8 f28
#define ft9 f29
#define ft10 f30
#define ft11 f31
using namespace std;

/*register*/
struct Register_physical//����Ĵ���
{
	bool used,modified;//��Чλ,�޸�λ
	int num;//��Ӧ����Ĵ������
	vector<pair<int, int> >occupied;//��ռ�õ�����
	Register_physical()
	{
		occupied.clear();
	}
};

struct Register_virtual//����Ĵ���
{
	type_label name;//����
	int num,reg_phisical, is_splited_from;//���,���䵽������Ĵ������,���ĸ��Ĵ���splited����
	type_spliting_weight spliting_weight, prod;//prodָ�����ȶ����ڵ����ȼ�
	int type;//����
	bool used, is_splited, is_spilled;//��Чλ,����,�Ƿ��Ѿ���spilted����,�Ƿ��·ŵ��ڴ�
	vector < pair<int, int> >live_interval;//��Ծ����
	Register_virtual* next;
	Register_virtual()
	{
		live_interval.clear();
		reg_phisical = -1;
		is_splited = false;
		is_spilled = false;
		next = NULL;
	}
};

/*���ű�*/
struct variable_table
{
	int num, dim, num_reg, cnt;//���,ά��,��Ӧ�ļĴ������,����������
	int type;//����(i32,float,i64)
	type_variables name;//������
	vector<unsigned int>size, val;//ÿ��ά�ȵĴ�С,ÿ����λ��ֵ
	variable_table* next;
	variable_table()
	{
		dim = 0;
		cnt = 1;
		num_reg = -1;
		next = NULL;
	}
};

/*ָ���*/
struct instruction
{
	int num;//ָ����
	int op;//ָ������
	int Rd, Rs1, Rs2;//������ţ�Ŀ�ģ�Դ1��Դ2
	bool fRd, fRs1, fRs2;//�������ͣ��ֲ���ȫ��
	int tRd, tRs1, tRs2;//�������ͣ�i32��i64��������
	bool fimm, fimm1, fimm2;//�Ƿ���ƫ����
	unsigned int imm, imm1, imm2;//ƫ����������GEPָ��,storeָ��,����ָ��
	/*���xcmp���*/
	int cond;
	/*���br���*/
	bool branch_flag;//�Ƿ�Ϊ��������ת
	int L1, L2;//��ת��Ŷ�Ӧ���
	/*���call,ret���*/
	int tot_formal,type_ret;//�β���������������
	type_label name;//����������
	int num_bb;//����bb���
	vector<bool>formal_type;//�洢�βε�����
	vector<int>formal_num;//�洢�βεı��
	instruction* next;
	instruction()
	{
		Rd = Rs1 = Rs2 = -1;
		fimm = fimm1 = fimm2 = 0;
		branch_flag = 0;
		tot_formal = 0;
		next = NULL;
	}
};

/*basic block*/
struct basic_block
{
	int num;//bb���
	int l, r;//��ָֹ����
	bool is_loop;//�Ƿ�Ϊѭ��ָ��
	instruction* ins_head;//��ʼָ��ָ��
	vector<basic_block*>edge;//����
	basic_block* next;
	basic_block()
	{
		ins_head = new instruction;
		next = NULL;
		is_loop = false;
	}
};

/*functions*/
struct functions
{
	//�������,ָ����,bb��,ʵ������,�β�����,���õ��β��������ֵ,��������,�����ջ�ռ��С,����ֵ����
	int num, cnt_ins, cnt_bb, total_actual, total_formal, max_formal, tot_arg, size, type;
	type_label name;//��������
	vector<bool>args;//��������
	instruction* ins_head, * ins_tail;//ָ��ͷβָ��
	basic_block* bb_head, * bb_tail;//bbͷβָ��
	variable_table* local_head, * local_tail;//�ֲ�������
	map<type_variables, int>map_local;//����������ŵ�ӳ��
	map<int, int>imm_local;//������ŵ���ַƫ������ӳ��
	Register_virtual* reg_head, * reg_tail;//����Ĵ���
	map<type_label, int>map_local_register;//����Ĵ���������ŵ�ӳ��
	map<int, Register_virtual*>map_local_register_position;//����Ĵ�����ŵ�ָ���ӳ��
	functions* next;
	functions()
	{
		cnt_ins = 0, cnt_bb = 0; total_actual = 0; total_formal = 0; max_formal = 0; tot_arg = 0; size = 0;
		ins_head = new instruction;
		ins_tail = ins_head;
		bb_head = new basic_block;
		bb_tail = bb_head;
		local_head = new variable_table;
		local_tail = local_head;
		reg_head = new Register_virtual;
		reg_tail = reg_head;
		next = NULL;
	}
};

/*definitions*/
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

extern map<type_label, int>label_num;//label������ŵ�ӳ��
extern map<int, int>label_ins;//label��ָ���ӳ��
extern vector<basic_block*>label_bb;//label��Ŷ�Ӧ��bb���
extern int tot_label;//label����

extern functions* func_head, * func_tail;
extern map<type_label, int>map_function;
extern int tot_functions;//�ܵĺ�����

/*function names*/
unsigned int floatToBinary(float num);
void read(int option, std::string line, functions* num, bool in_func);
functions* new_function(std::string line);
void end_function(functions* now_function);
int check_label(std::string s);
std::string get_label(std::string s);
void new_label(int op, type_label label, functions* num_func);
std::string get_new_line(std::string line);
void get_basic_block(functions* now_func);
void get_live_interval(functions* now_func);
void resize_live_interval(Register_virtual* now_reg);//����Ĵ����Ļ�Ծ����(���ϲ�)
void check_live_interval(Register_virtual* now_reg);
void register_allocate(functions* now_func);