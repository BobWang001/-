#pragma once
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <fstream>
/*type of variables*/
#define type_variables std::string
#define type_registers int
#define type_label std::string
/*number the registers*/
#define num_registers 64
#define zero 0
#define ra 1
#define sp 2
#define t0 3
#define t1 4
#define t2 5
#define t3 6
#define t4 7
#define t5 8
#define t6 9
#define s0 10
#define s1 11
#define s2 12
#define s3 13
#define s4 14
#define s5 15
#define s6 16
#define s7 17
#define s8 18
#define s9 19
#define s10 20
#define s11 21
#define a0 22
#define a1 23
#define a2 24
#define a3 25
#define a4 26
#define a5 27
#define a6 28
#define a7 29
#define ft0 30
#define ft1 31
#define ft2 32
#define ft3 33
#define ft4 34
#define ft5 35
#define ft6 36
#define ft7 37
#define fs0 38
#define fs1 39
#define fa0 40
#define fa1 41
#define fa2 42
#define fa3 43
#define fa4 44
#define fa5 45
#define fa6 46
#define fa7 47
#define fs2 48
#define fs3 49
#define fs4 50
#define fs5 51
#define fs6 52
#define fs7 53
#define fs8 54
#define fs9 55
#define fs10 56
#define fs11 57
#define ft8 58
#define ft9 59
#define ft10 60
#define ft11 61
using namespace std;

/*register*/
struct Register
{
	bool uesd,modified;//�޸�λ
	type_variables name;//��Ӧ������
}reg[num_registers];

/*���ű�*/
struct variable_table
{
	int num,num_reg;//���,��Ӧ�ļĴ������
	bool type;//����(int,float)
	type_variables name;//������
	variable_table* next;
}*local,*global;//�ֲ�,ȫ��
int total_global=0;//�洢ȫ�ֱ���������
map<type_variables, int>rev_local, rev_global;

/*ָ���*/
struct instruction
{
	int num;//ָ����
	int op;//ָ������
	int Rd,Rs1,Rs2;//������ţ�Ŀ�ģ�Դ1��Դ2
	bool fRd,fRs1,fRs2;//�������ͣ��ֲ���ȫ��
	type_label L1, L2;//��ת���
	int imm;//ƫ����������GEPָ��
	instruction* next;
};

/*basic block*/
struct basic_block
{
	int num;//bb���
	int l, r;//��ָֹ����
	vector<int>edge;//����
	basic_block* next;
};