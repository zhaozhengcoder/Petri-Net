#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

//ֱ�� 4�����ĸ���λ��5����5����


int minutes = 5;  // �����е�ʱ��

const int row = 10;
const int list = 4;

float rate = 5;   // ÿһ�������������� �뿪·�ڵĳ������� rate=10����ʾ1s �뿪·�ڵĳ���������1����һ��������10s������rate=10
float frequency = 10;   // һ������������10s
int transmit_per_period = 12;  // ��������һ��2���ӣ�120s������12��
int avg_wait_time = 45;
 
float flux2[list][row]= {
	{ 10,10,10,10,10,10,10,10,10,10 },  //road1 
	{ 10,10,10,10,10,10,10,10,10,10 },  //road2
	{ 10,10,10,10,10,10,10,10,10,10 },
	{ 10,10,10,10,10,10,10,10,10,10 },
};

//flux2[0][0] =10 ��ʾ����road1�ĳ��� ÿ������10����50%���� p13��30% ����p12��һ������������10s������һ�������ڽ���p13���� 0.8��������p12 ����0.5��
//���rate=10��Ҳ����ζ��һ�����������ڣ�Ҫ����10������Ҳ����˵��һ�����ڽ���p13�ĳ�����0.8�����ߵ���10

//���rate=5����ô����phase1��˵����phase1~phase4 ��Ҫ2���ӣ�������30s���̵ƣ����Դ���15�������������������ĳ�����Ŀ��10������� flux2[0][0] =10 �Ļ���


class Place {
public:
	Place() {
		this->volume = 0;
		this->capacity = 100;
	}
	Place(float v, float c=100) {
		this->volume = v;
		this->capacity = c;
	}
	float volume;
	float capacity;
};

class Graph {
public:
	Graph(float f12,float f13, float f22, float f23, float f32, float f33, float f42, float f43):P12(f12),P13(f13), P22(f22), P23(f23) , P32(f32), P33(f33) , P42(f42), P43(f43) {	}
	Graph(float f12):P12(f12){	}
	Graph() {}
	Place  P12, P13, P22, P23, P32, P33, P42, P43;
	Place out12, out13, out22, out23, out32, out33, out42, out43;
	void print() {
		cout << "road1  >  " << "p12 : " << setw(8) << P12.volume << "  ,  p13 :" << setw(8) << P13.volume << "   |  out12 : " << setw(8) << out12.volume << "  , out13: " << setw(8) << out13.volume << endl;
		cout << "road2  >  " << "p22 : " << setw(8) << P22.volume << "  ,  p23 :" << setw(8) << P23.volume << "   |  out22 : " << setw(8) << out22.volume << "  , out23: " << setw(8) << out23.volume << endl;
		cout << "road3  >  " << "p32 : " << setw(8) << P32.volume << "  ,  p33 :" << setw(8) << P33.volume << "   |  out32 : " << setw(8) << out32.volume << "  , out33: " << setw(8) << out33.volume << endl;
		cout << "road4  >  " << "p42 : " << setw(8) << P42.volume << "  ,  p43 :" << setw(8) << P43.volume << "   |  out42 : " << setw(8) << out42.volume << "  , out43: " << setw(8) << out43.volume << endl;
		cout << "+++++++++++++++++" << endl;
	}
};

class Config {
public:
	
	vector<float> flux_12;
	vector<float > flux_13;

	vector<float> flux_22;
	vector<float> flux_23;
	
	vector<float> flux_32;
	vector<float> flux_33;
	
	vector<float> flux_42;
	vector<float> flux_43;

	Config() {
		
		float straight = 0.5;
		float left = 0.3;

		for (int i = 0; i < minutes; i++) {
			float flux = flux2[0][i];
			flux_12.push_back(flux*left);
			flux_13.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < minutes; i++) {
			float flux = flux2[1][i];
			flux_22.push_back(flux*left);
			flux_23.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < minutes; i++) {
			float flux = flux2[2][i];
			flux_32.push_back(flux*left);
			flux_33.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < minutes; i++) {
			float flux = flux2[3][i];
			flux_42.push_back(flux*left);
			flux_43.push_back(flux*straight);
		}
	}
};

class Time {
public:
	//in
	vector<float> In_phase1_p13;
	vector<float> In_phase1_p33;

	vector<float> In_phase2_p12;
	vector<float> In_phase2_p32;

	vector<float> In_phase3_p23;
	vector<float> In_phase3_p43;

	vector<float> In_phase4_p22;
	vector<float> In_phase4_p42;


	//out
	vector<float> Out_phase1_p13;
	vector<float> Out_phase1_p33;
	
	vector<float> Out_phase2_p12;
	vector<float> Out_phase2_p32;
	
	vector<float> Out_phase3_p23;
	vector<float> Out_phase3_p43;

	vector<float> Out_phase4_p22;
	vector<float> Out_phase4_p42;

	//������
	vector<float> In_phase1_pure_p13;
	vector<float> In_phase1_pure_p33;

	vector<float> In_phase2_pure_p12;
	vector<float> In_phase2_pure_p32;

	vector<float> In_phase3_pure_p23;
	vector<float> In_phase3_pure_p43;

	vector<float> In_phase4_pure_p22;
	vector<float> In_phase4_pure_p42;


	void calu_pure_in() {
		// �ȳ�ʼ��
		In_phase1_pure_p13 = In_phase1_p13;
		In_phase1_pure_p33 = In_phase1_p33;

		In_phase2_pure_p12= In_phase2_p12;
		In_phase2_pure_p32= In_phase2_p32;

		In_phase3_pure_p23 = In_phase3_p23;
		In_phase3_pure_p43= In_phase3_p43;

		In_phase4_pure_p22 = In_phase4_p22;
		In_phase4_pure_p42 = In_phase4_p42;

		//phase1
		for (int i = 0; i < Out_phase1_p13.size(); i++) {
			if (Out_phase1_p13[i] != 0) {
				//��0
				In_phase1_pure_p13[i] = 0;
				In_phase1_pure_p33[i] = 0;
			}
		}
		//phase2
		for (int i = 0; i < Out_phase2_p12.size(); i++) {
			if (Out_phase2_p12[i] != 0) {
				In_phase2_pure_p12[i] = 0;
				In_phase2_pure_p32[i] = 0;
			}
		}
		//phase3
		for (int i = 0; i < Out_phase3_p23.size(); i++) {
			if (Out_phase3_p23[i] != 0) {
				In_phase3_pure_p23[i] = 0;
				In_phase3_pure_p43[i] = 0;
			}
		}
		//phase4
		for (int i = 0; i < Out_phase4_p22.size(); i++) {
			if (Out_phase4_p22[i] != 0) {
				In_phase4_pure_p22[i] = 0;
				In_phase4_pure_p42[i] = 0;
			}
		}
	}

	void calu_avg_delay_phase1(int beg, int end) {
		int weight[] = { 0,0,0,90,80,70,60,50,40,30,20,10 };
		float sum_time = 0;
		float sum_car = 0;
		for (int i = beg; i <= end; i++) {
			sum_time = sum_time + In_phase1_pure_p13[i] * weight[i%12];
			sum_time = sum_time + In_phase1_pure_p33[i] * weight[i%12];
			sum_car = sum_car + In_phase1_pure_p13[i];
			sum_car = sum_car + In_phase1_pure_p33[i];
		}
		cout << "phase1 ƽ���ȴ�ʱ�� ��" << sum_time / sum_car << endl;
	}

	void calu_avg_delay_phase2(int beg, int end) {
		int weight[] = { 30,20,10,0,0,0,90,80,70,60,50,40, };
		float sum_time = 0;
		float sum_car = 0;
		for (int i = beg; i <= end; i++) {
			sum_time = sum_time + In_phase2_pure_p12[i] * weight[i%12];
			sum_time = sum_time + In_phase2_pure_p32[i] * weight[i%12];
			sum_car = sum_car + In_phase2_pure_p12[i];
			sum_car = sum_car + In_phase2_pure_p32[i];
		}
		cout << "phase2 ƽ���ȴ�ʱ�� ��" << sum_time / sum_car << endl;
	}

	void calu_avg_delay_phase3(int beg, int end) {
		int weight[] = { 60,50,40,30,20,10,0,0,0,90,80,70 };
		float sum_time = 0;
		float sum_car = 0;
		for (int i = beg; i <= end; i++) {
			sum_time = sum_time + In_phase3_pure_p23[i] * weight[i%12];
			sum_time = sum_time + In_phase3_pure_p43[i] * weight[i%12];
			sum_car = sum_car + In_phase3_pure_p23[i];
			sum_car = sum_car + In_phase3_pure_p43[i];
		}
		cout << "phase3 ƽ���ȴ�ʱ�� ��" << sum_time / sum_car << endl;
	}

	void calu_avg_delay_phase4(int beg, int end) {
		int weight[] = { 90,80,70,60,50,40,30,20,10,0,0,0 };
		float sum_time = 0;
		float sum_car = 0;
		for (int i = beg; i <= end; i++) {
			sum_time = sum_time + In_phase4_pure_p22[i] * weight[i%12];
			sum_time = sum_time + In_phase4_pure_p42[i] * weight[i%12];
			sum_car = sum_car + In_phase4_pure_p22[i];
			sum_car = sum_car + In_phase4_pure_p42[i];
		}
		cout << "phase4 ƽ���ȴ�ʱ�� ��" << sum_time / sum_car << endl;
	}

	void calu_avg_delay(int run_times) {
		// ����run_times ��ƽ���ȴ�ʱ��
		int beg = run_times*transmit_per_period;
		int end = beg + (transmit_per_period - 1);
		calu_pure_in();

		calu_avg_delay_phase1(beg, end);
		calu_avg_delay_phase2(beg, end);
		calu_avg_delay_phase3(beg, end);
		calu_avg_delay_phase4(beg, end);
	}

};

bool IsPreEnable(Place & pre, float rate) {
	if (pre.volume < rate) {
		return false;
	}
	return true;
}

bool IsPostEnable(Place & post, float & rate) {
	if (post.volume + rate > post.capacity) {
		return false;
	}
	return true;
}


//�����ķ���ֵ ָ���� һ�μ������ߵĳ�����Ŀ
float transmit(Place &pre, Place &post, float rate) {
	//���ж�һ���ܷ�enable
	float ret;
	if (IsPreEnable(pre, rate) == true && IsPostEnable(post, rate) == true) {
		pre.volume = pre.volume - rate;
		post.volume = post.volume + rate;
		ret = rate;
		return ret;
	}
	else {
		//ǰ���ı�Ǩ̫��
		if (IsPreEnable(pre, rate) == false) {
			ret = pre.volume;
			post.volume = post.volume + pre.volume;
			pre.volume = 0;
			return ret;
		}
		//�󼯵ı�Ǩ����Ҫ��������
		if (IsPostEnable(post, rate) == false) {
			ret = post.capacity - post.volume;
			pre.volume = pre.volume - (post.capacity - post.volume);
			post.volume = post.capacity;
			return ret;
		}
	}
}

//opt ָ���Ƕ�Ӧ�������λ��ÿ���Ӳ����ĳ���
float well_distributed(float opt) {
	float ret = opt / 6.0;
	return ret;
}

//ÿһ�μ���ʱ���� ����ĳ��� ������10s ,opt ָ���Ƕ�Ӧ�������λ��ÿ���Ӳ����ĳ���
float add_car(Place & place,float opt) {
	float ret = well_distributed(opt);
	place.volume = place.volume + ret;

	//cout << "add car : " << well_distributed(opt) << endl;
	return ret;
}





void run_phase1(Graph & g, Config & config, int index, vector<float> & Out_phase1_p13, vector<float> & Out_phase1_p33, vector<float> & In_phase1_p13, vector<float> & In_phase1_p33) {
	//rate= ��ָ���ǳ����뿪·��1����ͨ��1��	
	float ret1;
	float ret2;
	float in1;
	float in2;

	float  opt1 = config.flux_13[index]; //30s������
	float  opt2 = config.flux_33[index];

	//����
	in1=add_car(g.P13, opt1);
	//�뿪
	ret1 = transmit(g.P13, g.out13, rate);

	//����
	in2=add_car(g.P33, opt2);
	//�뿪
	ret2 = transmit(g.P33, g.out33, rate);

	Out_phase1_p13.push_back(ret1);
	Out_phase1_p33.push_back(ret2);
	In_phase1_p13.push_back(in1);
	In_phase1_p33.push_back(in2);
}

void wait_phase1(Graph & g, Config & config, int index, vector<float> & In_phase1_p13, vector<float> & In_phase1_p33) {
	float  opt1 = config.flux_13[index];
	float  opt2 = config.flux_33[index];

	float in1;
	float in2;

	in1=add_car(g.P13, opt1);
	in2=add_car(g.P33, opt2);

	In_phase1_p13.push_back(in1);
	In_phase1_p33.push_back(in2);
}

void run_phase2(Graph & g, Config & config, int index, vector<float> & Out_phase2_p12,vector<float> & Out_phase2_p32,vector<float> & In_phase2_p12, vector<float> & In_phase2_p32) {
	//int rate = 5;
	float ret1;
	float ret2;
	float in1;
	float in2;

	float opt1 = config.flux_12[index];
	float opt2 = config.flux_32[index];

	in1=add_car(g.P12, opt1);
	ret1=transmit(g.P12, g.out12, rate);

	in2=add_car(g.P32, opt2);
	ret2=transmit(g.P32, g.out32, rate);

	Out_phase2_p12.push_back(ret1);
	Out_phase2_p32.push_back(ret2);
	In_phase2_p12.push_back(in1);
	In_phase2_p32.push_back(in2);
}

void wait_phase2(Graph & g, Config & config, int index, vector<float> & In_phase2_p12, vector<float> & In_phase2_p32) {
	float opt1 = config.flux_12[index]; //30s������
	float opt2 = config.flux_32[index];
	float in1;
	float in2;

	in1=add_car(g.P12, opt1);
	in2=add_car(g.P32, opt2);

	In_phase2_p12.push_back(in1);
	In_phase2_p32.push_back(in2);
}

void run_phase3(Graph & g, Config & config, int index, vector<float> & Out_phase3_p23, vector<float> & Out_phase3_p43, vector<float> & In_phase3_p23, vector<float> & In_phase3_p43) {
	float ret1;
	float ret2;
	float in1;
	float in2;

	float opt1 = config.flux_23[index];
	float opt2 = config.flux_43[index];
	in1=add_car(g.P23, opt1);
	ret1=transmit(g.P23, g.out23, rate);

	in2=add_car(g.P43, opt2);
	ret2=transmit(g.P43, g.out43, rate);

	Out_phase3_p23.push_back(ret1);
	Out_phase3_p43.push_back(ret2);
	In_phase3_p23.push_back(in1);
	In_phase3_p43.push_back(in2);
}

void wait_phase3(Graph & g, Config & config, int index, vector<float> & In_phase3_p23, vector<float> & In_phase3_p43) {
	float opt1 = config.flux_23[index];
	float opt2 = config.flux_43[index];
	float in1;
	float in2;

	in1=add_car(g.P23, opt1);
	in2=add_car(g.P43, opt2);

	In_phase3_p23.push_back(in1);
	In_phase3_p43.push_back(in2);
}

void run_phase4(Graph & g, Config & config, int index, vector<float> & Out_phase4_p22, vector<float> &  Out_phase4_p42, vector<float> & In_phase4_p22, vector<float> & In_phase4_p42) {
	float opt1 = config.flux_22[index];
	float opt2 = config.flux_42[index];
	float ret1;
	float ret2;
	float in1;
	float in2;

	in1=add_car(g.P22, opt1);
	ret1=transmit(g.P22, g.out22, rate);

	in2=add_car(g.P42, opt2);
	ret2=transmit(g.P42, g.out42, rate);

	Out_phase4_p22.push_back(ret1);
	Out_phase4_p42.push_back(ret2);
	In_phase4_p22.push_back(in1);
	In_phase4_p42.push_back(in2);
}

void wait_phase4(Graph & g, Config & config, int index, vector<float> & In_phase4_p22, vector<float> & In_phase4_p42) {
	float opt1 = config.flux_22[index];
	float opt2 = config.flux_42[index];
	float in1;
	float in2;


	in1=add_car(g.P22, opt1);
	in2=add_car(g.P42, opt2);

	In_phase4_p22.push_back(in1);
	In_phase4_p42.push_back(in2);
}


//tag1 ��ʾrun_phase ��1
void push_back_null(Time & t,int tag) {
	if (tag == 1) {

		t.Out_phase2_p12.push_back(0);
		t.Out_phase2_p32.push_back(0);

		t.Out_phase3_p23.push_back(0);
		t.Out_phase3_p43.push_back(0);

		t.Out_phase4_p22.push_back(0);
		t.Out_phase4_p42.push_back(0);
	}

	if (tag == 2) {
		t.Out_phase1_p13.push_back(0);
		t.Out_phase1_p33.push_back(0);

		t.Out_phase3_p23.push_back(0);
		t.Out_phase3_p43.push_back(0);

		t.Out_phase4_p22.push_back(0);
		t.Out_phase4_p42.push_back(0);
	}

	if (tag == 3) {
		t.Out_phase1_p13.push_back(0);
		t.Out_phase1_p33.push_back(0);

		t.Out_phase2_p12.push_back(0);
		t.Out_phase2_p32.push_back(0);

		t.Out_phase4_p22.push_back(0);
		t.Out_phase4_p42.push_back(0);
	}

	if (tag == 4) {
		t.Out_phase1_p13.push_back(0);
		t.Out_phase1_p33.push_back(0);

		t.Out_phase2_p12.push_back(0);
		t.Out_phase2_p32.push_back(0);

		t.Out_phase3_p23.push_back(0);
		t.Out_phase3_p43.push_back(0);
	}
}


// ִ��phase1 
void act_phase1(Graph & g,Config &config,Time &t,int run_times,int times=3) {
	//һ��10s ѭ������ 30s
	static int num = 0;
	static int transmit_times = 0;
	//cout << "phase1 ִ�д���: " << num << endl;
	int index = num / 2;
	// �������Σ�ÿ��10s

	//���ڵ��뷨�� ������ȴ�phase���ó�����Ŀ

	for (int i = 0; i < times; i++) {
		run_phase1( g,  config,index,t.Out_phase1_p13,t.Out_phase1_p33,t.In_phase1_p13,t.In_phase1_p33);
		wait_phase2(g, config, index,t.In_phase2_p12,t.In_phase2_p32);
		wait_phase3(g, config, index,t.In_phase3_p23,t.In_phase3_p43);
		wait_phase4(g, config, index,t.In_phase4_p22,t.In_phase4_p42);

		push_back_null(t, 1);
	}
	cout << "phase1 ��" << num << "�� ִ����Ϻ�" << endl;
	g.print();
	num++;
}

void act_phase2(Graph & g, Config &config, Time &t, int run_times, int times = 3) {
	static int num = 0;
	//cout << "phase2 ִ�д���: " << num+1 << endl;
	int index = num / 2;

	for (int i = 0; i < times; i++) {
		run_phase2(g, config, index, t.Out_phase2_p12, t.Out_phase2_p32, t.In_phase2_p12, t.In_phase2_p32);
		wait_phase1(g, config, index, t.In_phase1_p13, t.In_phase1_p33);
		wait_phase3(g, config, index, t.In_phase3_p23, t.In_phase3_p43);
		wait_phase4(g, config, index, t.In_phase4_p22, t.In_phase4_p42);

		push_back_null(t, 2);
	}
	cout << "phase2 ��" << num << "�� ִ����Ϻ�" << endl;
	g.print();
	num++;
}

void act_phase3(Graph & g, Config &config, Time &t, int run_times, int times = 3) {
	static int num = 0;
	//cout << "phase3 ִ�д���: " << num+1 << endl;
	int index = num / 2;
	for (int i = 0; i < times; i++) {
		run_phase3(g, config, index,t.Out_phase3_p23,t.Out_phase3_p43,t.In_phase3_p23,t.In_phase3_p43);
		wait_phase1(g, config, index, t.In_phase1_p13, t.In_phase1_p33);
		wait_phase2(g, config, index, t.In_phase2_p12, t.In_phase2_p32);
		wait_phase4(g, config, index, t.In_phase4_p22, t.In_phase4_p42);

		push_back_null(t, 3);
	}
	cout << "phase3 ��" << num << "�� ִ����Ϻ�" << endl;
	g.print();
	num++;
}

void act_phase4(Graph & g, Config &config, Time &t, int run_times, int times = 3) {
	static int num = 0;
	//cout << "phase4 ִ�д���: " << num +1<< endl;
	int index = num / 2;
	for (int i = 0; i < times; i++) {
		run_phase4(g, config,index,t.Out_phase4_p22,t.Out_phase4_p42,t.In_phase4_p22,t.In_phase4_p42);
		wait_phase1(g, config, index, t.In_phase1_p13, t.In_phase1_p33);
		wait_phase2(g, config, index, t.In_phase2_p12, t.In_phase2_p32);
		wait_phase3(g, config, index, t.In_phase3_p23, t.In_phase3_p43);

		push_back_null(t, 4);
	}
	cout << "phase4 ��" << num << "�� ִ����Ϻ�" << endl;
	g.print();
	num++;
}



//opt ָ���Ƕ�Ӧ�������λ��ÿ���Ӳ����ĳ���
//ÿ����λ30�룬4����λ����ִ��һ�Σ�����ִ��һ��act��ʱ����2����
void act(Graph & g, Config & config,Time &t,int run_times, int times = 3) {
	act_phase1(g, config, t, run_times);
	act_phase2(g, config, t, run_times);
	act_phase3(g, config, t, run_times);
	act_phase4(g, config, t, run_times);

	t.calu_avg_delay(run_times);
}

int main() {
	Graph g;
	Config c;
	Time t;
	g.print();

	//i ָ���������������еĴ���
	for (int i = 0; i <10; i++) {
		int run_times = i;
		act(g, c,t,run_times);
	}
	

	system("pause");
	return 0;
}