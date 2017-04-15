#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

using namespace std;

const int net_run_times = 5;  //网运行的次数
const int net_run_time_minutes = net_run_times * 2;	// 网运行的时间 （单位是分钟）
const int net_run_time_second = net_run_time_minutes * 60;//网运行的时间 单位是秒

const int frequency = 10; // 1秒钟激发10次
//const int transmit_times = net_run_time_second / 10;  //变迁激发的次数

const float rate = 0.1;       // 0.1秒 可以通过0.1辆车
const float fix_phase_time = 30;  //固定相位的绿灯长度是30s
const int phase_num = 4;        // 有4个相位
const int road_num = 4;	//道路数
const int add_car_time = 10;   //计算的周期是10s，10s调用一次addcar，
const int second_per_minutes = 60;  //1分钟有60s

float flux[road_num][net_run_time_minutes] = {
	{ 10,10,10,10,10,10,10,10,10,10 },  //road1 
	{ 10,10,10,10,10,10,10,10,10,10 },  //road2
	{ 10,10,10,10,10,10,10,10,10,10 },
	{ 10,10,10,10,10,10,10,10,10,10 },
};


float flux2[phase_num][net_run_time_minutes] = {
	{ 10,10,10,10,10,10,10,10,10,10 },  //road1 
	{ 10,10,10,10,10,10,10,10,10,10 },  //road2
	{ 10,10,10,10,10,10,10,10,10,10 },
	{ 10,10,10,10,10,10,10,10,10,10 },
};

class Token {
public:
	float delay_time;

	Token(float delay_t = 0) {
		delay_time = delay_t;
	}

	void add_delay_time() {
		delay_time += 1.0 / frequency;
	}

	float get_delay_time() {
		return delay_time;
	}
};

class Place {
public:
	list<Token *> contain;
	float capacity;
	Place * nextplace;

	Place() {
		capacity = 1000;
		nextplace = NULL;
	}
	Place(Place * n) {
		capacity = 1000;
		nextplace = n;
	}
	float getnowcar() {
		return contain.size()*0.1;
	}

	bool IsEnable() {
		float nowcar = getnowcar();
		if (nowcar >= rate) {
			return true;
		}
		return false;
	}
	void transmit() {
		// 变迁的当前车辆
		if (IsEnable() == true) {
			Token * t;
			t = contain.front();
			contain.pop_front();
			nextplace->contain.push_back(t);

			//对于没有离开库所的车辆 延时++
			for (auto it = contain.begin(); it != contain.end();it++) {
				(*it)->add_delay_time();
			}
		}
		else {
			//没有车可以激发
		}
	}

	//10s 内可以进入的车辆 比如是0.5辆，那么flux_per_add_car_time=0.5
	void addcar(float flux_per_add_car_time) {
		//0.1辆车表示一个token
		int tokennum = flux_per_add_car_time * 10;
		Token * t = new Token[tokennum];
		for (int i = 0; i < tokennum; i++) {
			contain.push_back(&t[i]);
		}
	}

	void calu_delay() {
		float delay_sum_time=0.0;
		float car_num = 0.0;
		for (auto it = contain.begin(); it != contain.end(); it++) {
			delay_sum_time = (*it)->delay_time;
		}

		car_num = getnowcar();
		delay_sum_time = delay_sum_time*0.1;
		cout << "delay_sum_time : " << delay_sum_time << endl;
		cout << "car_num : " << car_num << endl;
		cout << "avg time :" << delay_sum_time / car_num << endl;
	}
};

class Graph {

	Place p00;
	Place p01;
	Place p10;
	Place p11;

	Place p20;
	Place p21;
	Place p30;
	Place p31;

	Place out00;
	Place out01;
	Place out10;
	Place out11;
	Place out20;
	Place out21;
	Place out30;
	Place out31;
public:
	vector<Place> p;
	vector<Place> pout;

	Graph() :p00(&out00), p01(&out01), p10(&out10), p11(&out11), p20(&out20), p21(&out21), p30(&out30), p31(&out31) {
		p.push_back(p00);
		p.push_back(p01);
		p.push_back(p10);
		p.push_back(p11);

		p.push_back(p20);
		p.push_back(p21);
		p.push_back(p30);
		p.push_back(p31);

		pout.push_back(out00);
		pout.push_back(out01);
		pout.push_back(out10);
		pout.push_back(out11);

		pout.push_back(out20);
		pout.push_back(out21);
		pout.push_back(out30);
		pout.push_back(out31);
	}

};

class Config {
public:
	//flux_00 表示左转，flux_01 表示直行
	vector<float> flux_00;
	vector<float> flux_01;

	vector<float> flux_10;
	vector<float> flux_11;

	vector<float> flux_20;
	vector<float> flux_21;

	vector<float> flux_30;
	vector<float> flux_31;

	vector<float> flux_00_add_car_time;
	vector<float> flux_01_add_car_time;

	vector<float> flux_10_add_car_time;
	vector<float> flux_11_add_car_time;

	vector<float> flux_20_add_car_time;
	vector<float> flux_21_add_car_time;

	vector<float> flux_30_add_car_time;
	vector<float> flux_31_add_car_time;


	vector<float> & phase_flux_00_add_car_time;
	vector<float> &phase_flux_01_add_car_time;

	vector<float> & phase_flux_10_add_car_time;
	vector<float> & phase_flux_11_add_car_time;

	vector<float> & phase_flux_20_add_car_time;
	vector<float> & phase_flux_21_add_car_time;

	vector<float> & phase_flux_30_add_car_time;
	vector<float> & phase_flux_31_add_car_time;

	vector<vector<float>> phase_flux_add_car_time;
	

	Config():phase_flux_00_add_car_time(flux_01_add_car_time), phase_flux_01_add_car_time(flux_21_add_car_time) , phase_flux_10_add_car_time(flux_00_add_car_time) , phase_flux_11_add_car_time(flux_20_add_car_time) , phase_flux_20_add_car_time(flux_11_add_car_time) , phase_flux_21_add_car_time(flux_31_add_car_time) , phase_flux_30_add_car_time(flux_10_add_car_time) , phase_flux_31_add_car_time(flux_30_add_car_time) {
		float straight = 0.5;
		float left = 0.3;

		for (int i = 0; i < net_run_time_minutes; i++) {
			float flux = flux2[0][i];
			flux_00.push_back(flux*left);
			flux_01.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < net_run_time_minutes; i++) {
			float flux = flux2[1][i];
			flux_10.push_back(flux*left);
			flux_11.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < net_run_time_minutes; i++) {
			float flux = flux2[2][i];
			flux_20.push_back(flux*left);
			flux_21.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < net_run_time_minutes; i++) {
			float flux = flux2[3][i];
			flux_30.push_back(flux*left);
			flux_31.push_back(flux*straight);
		}
	}

	//以分钟为单位 转换成 以add_car_time为单位
	void convert(vector<float> &source, vector<float> & des,float div) {
		for (int i = 0; i < source.size(); i++) {
			float flux = source[i];
			float flux_per_addcartime = flux / div;
			for (int j = 0; j < div; j++) {
				des.push_back(flux_per_addcartime);
			}
		}
	}


	void convert_to_by_add_car_time() {
		//to-do
		float div = (second_per_minutes / add_car_time);
		convert(flux_00, flux_00_add_car_time,div);
		convert(flux_01, flux_01_add_car_time, div);

		convert(flux_10, flux_10_add_car_time, div);
		convert(flux_11, flux_11_add_car_time, div);

		convert(flux_20, flux_20_add_car_time, div);
		convert(flux_21, flux_21_add_car_time, div);

		convert(flux_30, flux_30_add_car_time, div);
		convert(flux_31, flux_31_add_car_time, div);

		phase_flux_add_car_time.push_back(phase_flux_00_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_01_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_10_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_11_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_20_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_21_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_30_add_car_time);
		phase_flux_add_car_time.push_back(phase_flux_31_add_car_time);
	}

	void print_phase_flux_add_car_time() {
		for (int i = 0; i < phase_flux_add_car_time.size(); i++) {
			vector<float> & ref = phase_flux_add_car_time[i];

			for (int j = 0; j<ref.size(); j++) {
				cout << ref[j] << "  ";
			}
			cout << endl;
		}
	}

};

class Phase {
public:
	//phasenum 表示相位号，direction 表示木一个相位里面正反的方向，times表示这是第几次执行
	void act_per_add_car_time(int phasenum, int direction, Graph &g, Config & c) {
		static int  times = 0;
		int index = phasenum * 2 + direction;
		vector<float> &ref=c.phase_flux_add_car_time[index];
		float flux_10s = ref[times];
		g.p[index].addcar(flux_10s);
		times++;
	}


	// 每隔10s都会addcar一次，但是只有绿灯的时候，才会transmit，激发10(频率) *10s=100次
	void act_transmit(int phasenum, int direction, Graph &g, Config & c){
		int index = phasenum * 2 + direction;
		for (int i = 0; i < add_car_time * frequency; i++) {
			g.p[index].transmit();
		}
	}

	//10s
	void act_run_phase(int phasenum, int direction, Graph &g, Config & c) {
		act_per_add_car_time(phasenum, direction, g, c);
		act_transmit(phasenum, direction, g, c);
		// //loops =3;
		//int loops = fix_phase_time / add_car_time;
		//for (int i = 0; i < loops; i++) {
		//	act_per_add_car_time(phasenum, direction, g, c);
		//	act_transmit(phasenum, direction, g, c);
		//}
	}

	//10s
	void act_wait_phase(int phasenum, int direction, Graph &g, Config & c) {
		act_transmit(phasenum, direction, g, c);
		//int loops = fix_phase_time / add_car_time;
		//for (int i = 0; i < loops; i++) {
		//	//act_per_add_car_time(phasenum, direction, g, c);
		//	act_transmit(phasenum, direction, g, c);
		//}
	}


	void act_phase(int phasenum,Graph &g, Config & c) {
		//30s 
		//phase0,run ;phase 1-3 wait

		int loops = fix_phase_time / add_car_time;
		for (int i = 0; i < loops; i++) {
			if (phasenum == 0) {
				act_run_phase(0, 0, g, c);
				act_run_phase(0, 1, g, c);

				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);
			}
			if (phasenum == 1) {
				act_run_phase(1, 0, g, c);
				act_run_phase(1, 1, g, c);

				act_wait_phase(0, 0, g, c);
				act_wait_phase(0, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);
			}
			if (phasenum == 2) {
				act_run_phase(2, 0, g, c);
				act_run_phase(2, 1, g, c);

				act_wait_phase(0, 0, g, c);
				act_wait_phase(0, 1, g, c);
				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);

			}
			if (phasenum == 3) {
				act_run_phase(3, 0, g, c);
				act_run_phase(3, 1, g, c);

				act_wait_phase(0, 0, g, c);
				act_wait_phase(0, 1, g, c);
				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
			}
		}
		
	}


	//网运行一次
	void act(Graph &g, Config & c) {
		//phase1
		act_phase(0, g, c);
		//phase2
		act_phase(1, g, c);
		//phase3
		act_phase(2, g, c);
		//phase4
		act_phase(3, g, c);
	}
};

int main() {
	Graph g;
	Config c;
	Phase phase;
	c.convert_to_by_add_car_time();
	// 网运行一次
	c.print_phase_flux_add_car_time();

	phase.act(g, c);

	



	system("pause");
	return 0;
}