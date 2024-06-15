#include "riscv.h"

Register_physical reg[2][num_registers];
map<type_label, int>map_global_register;//ȫ�ּĴ���������ŵ�ӳ��
map<int, Register_virtual*>map_global_register_position;//ȫ�ּĴ�����ŵ�ָ���ӳ��
map<int, Register_virtual*>map_local_register_position;//�ֲ�����Ĵ�����ŵ�ָ���ӳ��
map<int, std::string>map_physical_reg_name[2];//����Ĵ�����ŵ����Ƶ�ӳ��
vector<bool>physical_reg_usable[2];//����Ĵ����ܷ�ʹ��
vector<int>physical_reg_order[2];//���Ƿ�������Ĵ�����˳��
int total_register = 0;

variable_table* global = new variable_table, * global_tail;
int total_global = 0;//�洢����������
map<type_variables, int>map_global;//����������ŵ�ӳ��
map<int, int>map_register_local;//�Ĵ�����ŵ�������ŵ�ӳ��

instruction* start = new instruction;//����ȫ�ֵ�ָ��
map<std::string, int>ins_num, cond_num;
int tot_instructions = 0;//�ܵ�ָ����
set<int>ins_definied = { 1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,24,25 };//�ᶨ������Ĵ�����ָ��(��call)
set<int>ins_used = { 1,4,24,25 };
set<int>ins_valuate = { 1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,24,25 };//���Rd��ֵ��ָ��
map<int, instruction*>map_instruction_position;//ָ���ŵ�ָ���ӳ��
map<int, pair<int, int> >map_ins_copy;//��¼copyָ���λ��


map<type_label, int>label_num;
map<int, int>label_ins;//label��ָ���ӳ��
vector<basic_block*>label_bb;//label��Ŷ�Ӧ��bb���
int tot_label = 0;

functions* func_head = new functions, * func_tail = new functions;
map<type_label, int>map_function;
int tot_functions = 0;//�ܵĺ�����

void init_definition()//��ʼ������
{
	ins_num = {
		{"global",0},{"load",1},{"store",2},{"alloca",3},{"getelementptr",4},
		{"add",5},{"fadd",6},{"sub",7},{"fsub",8},{"mul",9},{"fmul",10},{"sdiv",11},{"fdiv",12},
		{"srem",13},{"frem",14},{"fneg",15},{"icmp",16}, {"fcmp",17},{"br",18},{"define",19},
		{"call",20},{"ret",21},{"label",22},{"unreachable",23},{"sitofp",24},{"fptosi",25}
	};
	cond_num = {
		{"eq",0},{"ne",1},{"ugt",2},{"sgt",3},{"ule",4},
		{"sle",5},{"uge",6},{"sge",7},{"ult",8},{"slt",9},
		{"oeq",10},{"ogt",11},{"oge",12}, {"olt",13},{"ole",14},{"une",15}
	};
	map_physical_reg_name[0] = {
		{0,"zero"},{1,"ra"},{2,"sp"},{3,"gp"},{4,"tp"},{5,"t0"},{6,"t1"},{7,"t2"},
		{8,"s0"},{9,"s1"},{10,"a0"},{11,"a1"},{12,"a2"},{13,"a3"},{14,"a4"},{15,"a5"},
		{16,"a6"},{17,"a7"},{18,"s2"},{19,"s3"},{20,"s4"},{21,"s5"},{22,"s6"},{23,"s7"},
		{24,"s8"},{25,"s9"},{26,"s10"},{27,"s11"},{28,"t3"},{29,"t4"},{30,"t5"},{31,"t6"}
	};
	map_physical_reg_name[1] = {
		{32,"ft0"},{33,"ft1"},{34,"ft2"},{35,"ft3"},{36,"ft4"},{37,"ft5"},{38,"ft6"},{39,"ft7"},
		{40,"fs0"},{41,"fs1"},{42,"fa0"},{43,"fa1"},{44,"fa2"},{45,"fa3"},{46,"fa4"},{47,"fa5"},
		{48,"fa6"},{49,"fa7"},{50,"fs2"},{51,"fs3"},{52,"fs4"},{53,"fs5"},{54,"fs6"},{55,"fs7"},
		{56,"fs8"},{57,"fs9"},{58,"fs10"},{59,"fs11"},{60,"ft8"},{61,"ft9"},{62,"ft10"},{63,"ft11"}
	};
	physical_reg_order[0] = {
		t0,t1,t2
		/*,t3,t4,t5,t6,s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,
		a7,a6,a5,a4,a3,a2,a1,a0,zero,ra,sp,gp,tp*/
	};
	physical_reg_order[1] = {
		ft0,ft1,ft2
		/*,ft3,ft4,ft5,ft6,ft7,ft8,ft9,ft10,ft11,
		fs0,fs1,fs2,fs3,fs4,fs5,fs6,fs7,fs8,fs9,fs10,fs11,
		fa7,fa6,fa5,fa4,fa3,fa2,fa1,fa0*/
	};
	physical_reg_usable[0] = {
		false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true
	};
	physical_reg_usable[1] = {
		true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true
	};
	//��ʼ������ͷβָ��
	func_tail = func_head;
	//��ʼ��ȫ�ֱ���βָ��
	global_tail = global;
}

int main()
{
	FILE* output_stream;
	freopen_s(&output_stream, "output.out", "w", stdout);
	init_definition();
	std::ifstream file("test_f.ll");//���ļ�
	if (!file.is_open())
	{
		std::cerr << "�޷����ļ�" << std::endl;
		return 1;
	}
	std::string line;
	std::string s;
	functions* lst_func = func_head, * now_func = NULL;//ָ����һ��������ָ��ǰ����
	bool in_func = 0;//�Ƿ��ں����ڲ�����ʼ��Ϊ��
	/*���ҳ�����ȫ�ֱ���*/
	while(getline(file,line))//���ж���
	{
		if (line[0] == ';')
			continue;
		std::istringstream word(line);
		while (word >> s)
		{
			if (check_label(s))//ȡ��label
			{
				type_label ret = get_label(s);
				tot_label++;
				label_num[ret] = tot_label;
			}
			int option = -1;//������û��ʶ���ָ����Ϊ-1
			if (ins_num.count(s) == 0)
				continue;
			else option = ins_num[s];
			if (option == 0)
			{
				read(option, line, now_func, in_func);
				break;
			}
		}
	}
	label_bb.resize(tot_label + 1);
	std::ifstream file_2("test_f.ll");
	while (getline(file_2, line))//���ж���
	{
		std::istringstream word(line);
		while (word >> s)
		{
			if (s[0] == ';')
				break;
			if (s[0] == '}')//���ﺯ�������ĩβ
			{
				get_basic_block(now_func);//�ҳ�basic blocks
				get_live_interval(now_func);//�ҳ�ÿ������Ĵ����Ļ�Ծ����
				register_allocate(now_func);//�Ĵ�������
				end_function(now_func);//���㺯������Ŀռ��С
				in_func = 0;
			}
			if (check_label(s))//��������label
			{
				type_label ret = get_label(s);
				new_label(ins_label, ret, now_func);
				line = get_new_line(line);
			}
			int option = -1;//������û��ʶ���ָ����Ϊ-1
			if (ins_num.count(s) == 0)
				continue;
			else option = ins_num[s];
			if (option == ins_define)//�����������������
			{
				functions* ret = new_function(line);
				lst_func->next = ret;
				lst_func = ret;
				now_func = ret;
				in_func = 1;
				break;
			}
			else if (option != 0)
			{
				read(option, line, now_func, in_func);
				break;
			}
		}
	}
	/*�ҳ�ȫ�ֱ����Ļ�Ծ����*/
	/*for (auto it : map_global_register_position)
	{
		Register_virtual* now_reg = it.second;
		resize_live_interval(now_reg);//����Ĵ����Ļ�Ծ����(���ϲ�)
		check_live_interval(now_reg);
	}*/
	return 0;
}