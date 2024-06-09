#include "riscv.h"

extern int total_global;//�洢ȫ�ֱ���������
extern variable_table* global, * global_tail;

int floatToBinary(float num)//��������ת��Ϊ��Ӧ�Ķ�������
{
	// ʹ��union������float��λ��ʾ
	union
	{
		float f;
		unsigned int i;
	}b;
	b.f = num;
	std::bitset<32> bs(b.i); // ����һ��32λ��bitset
	int val = 0;
	for (int i = 0; i < 31; ++i)
		val += bs[i] * (1 << i);
	// ��ӡ���
	return val;
}

void new_global_type(variable_table* new_global, std::string word)
{
	int len = word.length();
	for (int p = 0; p < len; )
	{
		if (word[p] == '[' || word[p] == ']' || word[p]=='x' || word[p] == ' ')
		{
			p++;
			continue;
		}
		std::string ret;
		ret.push_back(word[p]);
		p++;
		while (word[p] != ' ' && word[p] != ']')
		{
			ret.push_back(word[p]);
			p++;
		}
		if (ret == "i32")
			new_global->type = 0;
		else if (ret == "float")
			new_global->type = 1;
		else
		{
			int val = atoi(ret.c_str());//��ά�ȵĴ�С
			new_global->dim++;
			new_global->size.push_back(val);
			new_global->cnt *= val;
		}
		p++;
	}
}

void new_global_value(variable_table* new_global, std::string word)
{
	int len = word.length();
	for (int p = 0; p < len; )
	{
		if (word[p] == '[' || word[p] == ']' || word[p] == 'x' || word[p] == ' ')
		{
			p++;
			continue;
		}
		std::string ret;
		ret.push_back(word[p]);
		p++;
		while (word[p] != ' ' && word[p] != ']')
		{
			ret.push_back(word[p]);
			p++;
		}
		if (ret == "i32" || ret == "float")
		{
			bool find_num = 0;
			std::string num;
			while (word[p] < '0' || word[p] > '9')
			{
				if (word[p] == ']')
				{
					p++;
					break;
				}
				else if (word[p] == ' ')
					p++;
			}
			find_num = 1;//�ҵ�һ��ֵ
			while ((word[p] >= '0' && word[p] <= '9') || word[p] == '.')
			{
				num.push_back(word[p]);
				p++;
			}
			if (!find_num)
				continue;
			if (new_global->type == 0)
				new_global->val.push_back(atoi(num.c_str()));
			else
				new_global->val.push_back(floatToBinary(atof(num.c_str())));
		}
		p++;
	}
}

void new_global(std::string line)
{
	variable_table* new_global = new variable_table;
	new_global->num = ++total_global;//��¼���
	bool name = 0, size_type = 0, val = 0;//����Ƿ��Ѿ��ҵ���ȫ�ֱ��������ƣ���С���ͣ���ʼֵ
	int cnt = 0;
	int len = line.length();
	for (int p = 0; p < len; )
	{
		if (line[p] == ' ')
		{
			p++;
			continue;
		}
		/*��õ��ʣ����˴��������Ž�������ƥ�����⣬������Ϊ�����ո�ֹͣ*/
		std::string word;
		word.push_back(line[p]);
		switch (line[p])
		{
			case '[': 
			{
				int top = 1;
				while (top)
				{
					p++;
					word.push_back(line[p]);
					if (line[p] == '[')
						top++;
					else if (line[p] == ']')
						top--;
				}
				break;
			}
			default:
			{
				p++;
				while (p < len && line[p] != ' ')
					word.push_back(line[p]);
				break;
			}
		}
		p++;
		if (word == "global")
			continue;
		if (word == "=")
			continue;
		if (!name)
		{
			new_global->name = word;
			name = 1;
		}
		else if (!size_type)
		{
			if (word[0] != '[')
				new_global->type = (word == "i32") ? 0 : 1;
			else
				new_global_type(new_global, word);
			size_type = 1;
		}
		else if (!val)
		{
			if (word[0] != '[')
			{
				if (new_global->type == 0)//����ֱ�Ӹ�ֵ
					new_global->val.push_back(atoi(word.c_str()));
				else//��������Ҫת��һ��
					new_global->val.push_back(floatToBinary(atof(word.c_str())));
			}
			else
				new_global_value(new_global, word);
			val = 1;
		}
	}
	new_global->next = NULL;
	global_tail->next = new_global;
	global_tail = new_global;
}

void read(int option, std::string line, functions* num, bool in_func)//����
{
	switch (option)
	{
		case 0://global���
		{
			new_global(line);//�½�һ��ȫ�ֱ���
			break;
		}
	}
}