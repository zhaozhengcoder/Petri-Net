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
							  //gai
const float fix_phase_time = 30;  //固定相位的绿灯长度是30s
const int phase_num = 4;        // 有4个相位
const int road_num = 4;	//道路数

						//gai :10改为了1
const int add_car_time = 5;   //计算的周期是10s，10s调用一次addcar，
const int second_per_minutes = 60;  //1分钟有60s

const float token_per_car = 0.1;  //每个token代表的车辆数

								  //float flux2[road_num][net_run_time_minutes] = {
								  //	{ 15,15,15,15,15,15,15,15,15,15 },  //road1 
								  //	{ 15,15,15,15,15,15,15,15,15,15 },  //road2
								  //	{ 15,15,15,15,15,15,15,15,15,15 },
								  //	{ 15,15,15,15,15,15,15,15,15,15 },
								  //};


float flux2[road_num][net_run_time_minutes] = {
	{ 11,15,3,16,13,23,4,23,17,23 },  //road1  人民中路（江海-公园) 2012-11-11 09:42~10:41
	{ 9,19,6,18,3,19,18,12,8,17 },  //road2
	{ 16,17,18,6,18,8,18,13,9,17 },
	{ 14,11,8,23,9,23,10,18,8,19 },
};

//记录每一个相位运行的时候，获取输入的次数
int flux_times[8] = { 0,0,0,0,0,0,0,0 };

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
		capacity = 10000;
		nextplace = NULL;
	}
	Place(Place * n) {
		capacity = 10000;
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

	//激发一个的时间是0.1s 所以rate=0.1的情况下，激发一次只能通过一辆车
	//调用一次transmit 表示激发一次
	void transmit() {
		// 变迁的当前车辆
		if (IsEnable() == true) {
			Token * t;
			t = contain.front();
			contain.pop_front();
			nextplace->contain.push_back(t);

			//对于没有离开库所的车辆 延时++
			for (auto it = contain.begin(); it != contain.end(); it++) {
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
		float delay_sum_time = 0.0;
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

	//给当前库所里面的所有变迁增加10秒钟的延时
	void place_add_cartime_delay() {
		for (int i = 0; i < add_car_time*frequency; i++) {
			// 这里一个库所里面的所有变迁 增加0.1秒的延时
			for (auto it = contain.begin(); it != contain.end(); it++) {
				(*it)->add_delay_time();
			}
		}
	}
};

class Graph {
	//p00 表示的phase0 的dir=0方向
	//p01 表示的phase0 的dir=1方向
	Place p00;
	Place p01;
	Place p10;
	Place p11;

	Place p20;
	Place p21;
	Place p30;
	Place p31;

	Place out00;   //out 在这里
	Place out01;
	Place out10;
	Place out11;
	Place out20;
	Place out21;
	Place out30;
	Place out31;
public:
	vector<Place *> p;   //p在这里
	vector<Place *> pout;

	//p00(&out00) 表示的是：p00当前库所的下一个库所out00
	Graph() :p00(&out00), p01(&out01), p10(&out10), p11(&out11), p20(&out20), p21(&out21), p30(&out30), p31(&out31) {
		p.push_back(&p00);
		p.push_back(&p01);
		p.push_back(&p10);
		p.push_back(&p11);
		p.push_back(&p20);
		p.push_back(&p21);
		p.push_back(&p30);
		p.push_back(&p31);

		pout.push_back(&out00);
		pout.push_back(&out01);
		pout.push_back(&out10);
		pout.push_back(&out11);
		pout.push_back(&out20);
		pout.push_back(&out21);
		pout.push_back(&out30);
		pout.push_back(&out31);
	}

};

class Config {
public:
	//flux_00 表示左转，flux_01 表示直行
	//将路的输入，转化为一个对应的vector，将四条路每分钟的输入，转换为四条路对应的每分钟的直行和左转的车辆
	vector<float> flux_00;
	vector<float> flux_01;

	vector<float> flux_10;
	vector<float> flux_11;

	vector<float> flux_20;
	vector<float> flux_21;

	vector<float> flux_30;
	vector<float> flux_31;

	//将分钟为单位 转换为以add_car_time为单位的时间
	vector<float> flux_00_add_car_time;
	vector<float> flux_01_add_car_time;

	vector<float> flux_10_add_car_time;
	vector<float> flux_11_add_car_time;

	vector<float> flux_20_add_car_time;
	vector<float> flux_21_add_car_time;

	vector<float> flux_30_add_car_time;
	vector<float> flux_31_add_car_time;

	//将路转换为相位
	vector<float> &phase_flux_00_add_car_time;
	vector<float> &phase_flux_01_add_car_time;

	vector<float> & phase_flux_10_add_car_time;
	vector<float> & phase_flux_11_add_car_time;

	vector<float> & phase_flux_20_add_car_time;
	vector<float> & phase_flux_21_add_car_time;

	vector<float> & phase_flux_30_add_car_time;
	vector<float> & phase_flux_31_add_car_time;

	//相位对应的输入的集合
	vector<vector<float>> phase_flux_add_car_time;


	Config() :phase_flux_00_add_car_time(flux_01_add_car_time), phase_flux_01_add_car_time(flux_21_add_car_time), phase_flux_10_add_car_time(flux_00_add_car_time), phase_flux_11_add_car_time(flux_20_add_car_time), phase_flux_20_add_car_time(flux_11_add_car_time), phase_flux_21_add_car_time(flux_31_add_car_time), phase_flux_30_add_car_time(flux_10_add_car_time), phase_flux_31_add_car_time(flux_30_add_car_time) {
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
	void convert(vector<float> &source, vector<float> & des, float div) {
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
		convert(flux_00, flux_00_add_car_time, div);
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
		//static int  times = 0; 这里要修改 吃完饭再来改

		int index = phasenum * 2 + direction;
		int times = flux_times[index];

		//从vector里面读取输入
		vector<float> &ref = c.phase_flux_add_car_time[index];
		//现在这里只能获得到5s的车流量
		float flux_per_addcartime = ref[times];

		g.p[index]->addcar(flux_per_addcartime);
		flux_times[index]++;
	}


	// 每隔addcartime(5s)都会addcar一次，但是只有绿灯的时候，才会transmit，激发10(频率) *5s=50次
	void act_transmit(int phasenum, int direction, Graph &g, Config & c) {
		int index = phasenum * 2 + direction;
		//5s 会循环50次，调用一次transmit表示激发一次
		for (int i = 0; i < add_car_time * frequency; i++) {
			g.p[index]->transmit();
		}
	}


	void act_transmit_with_losttime2s(int phasenum, int direction, Graph &g, Config & c) {
		int index = phasenum * 2 + direction;
		//5s 损失1s
		for (int i = 0; i < add_car_time*frequency - 10; i++) {
			g.p[index]->transmit();
		}
	}


	//一个addcartime时长(5s),没有带有切换相位的损失时间
	void act_run_phase_nowith_losttime(int phasenum, int direction, Graph &g, Config & c) {
		act_per_add_car_time(phasenum, direction, g, c);
		// 切换相位会损失一部分的时间
		act_transmit(phasenum, direction, g, c);
	}

	//一个addcartime时长(5s),并且带有切换相位的损失时间
	void act_run_phase(int phasenum, int direction, Graph &g, Config & c) {
		act_per_add_car_time(phasenum, direction, g, c);
		// 切换相位会损失一部分的时间
		act_transmit_with_losttime2s(phasenum, direction, g, c);
	}



	//一个addcartime时长(5s)
	void act_wait_phase(int phasenum, int direction, Graph &g, Config & c) {
		act_per_add_car_time(phasenum, direction, g, c);
	}

	//30s 
	void act_phase(int phasenum, Graph &g, Config & c) {
		//phase0,run ;phase 1-3 wait
		int loops = fix_phase_time / add_car_time;
		for (int i = 0; i < loops; i++) {
			if (phasenum == 0) {
				//run
				act_run_phase(0, 0, g, c);
				act_run_phase(0, 1, g, c);

				//wait
				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);

				g.p[2]->place_add_cartime_delay();
				g.p[3]->place_add_cartime_delay();
				g.p[4]->place_add_cartime_delay();
				g.p[5]->place_add_cartime_delay();
				g.p[6]->place_add_cartime_delay();
				g.p[7]->place_add_cartime_delay();


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
				g.p[0]->place_add_cartime_delay();
				g.p[1]->place_add_cartime_delay();
				g.p[4]->place_add_cartime_delay();
				g.p[5]->place_add_cartime_delay();
				g.p[6]->place_add_cartime_delay();
				g.p[7]->place_add_cartime_delay();
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
				g.p[0]->place_add_cartime_delay();
				g.p[1]->place_add_cartime_delay();
				g.p[2]->place_add_cartime_delay();
				g.p[3]->place_add_cartime_delay();
				g.p[6]->place_add_cartime_delay();
				g.p[7]->place_add_cartime_delay();

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
				g.p[0]->place_add_cartime_delay();
				g.p[1]->place_add_cartime_delay();
				g.p[2]->place_add_cartime_delay();
				g.p[3]->place_add_cartime_delay();
				g.p[4]->place_add_cartime_delay();
				g.p[5]->place_add_cartime_delay();
			}
		}

	}

	//通过loops定义次数，这个是延时相位调用的，所以不需要有切换相位的损失时间
	void act_phase(int phasenum, Graph &g, Config & c, int loops) {

		//循环一次是5s
		for (int i = 0; i < loops; i++) {
			if (phasenum == 0) {
				//run
				act_run_phase_nowith_losttime(0, 0, g, c);
				act_run_phase_nowith_losttime(0, 1, g, c);

				//wait
				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);

				g.p[2]->place_add_cartime_delay();
				g.p[3]->place_add_cartime_delay();
				g.p[4]->place_add_cartime_delay();
				g.p[5]->place_add_cartime_delay();
				g.p[6]->place_add_cartime_delay();
				g.p[7]->place_add_cartime_delay();


			}
			if (phasenum == 1) {
				act_run_phase_nowith_losttime(1, 0, g, c);
				act_run_phase_nowith_losttime(1, 1, g, c);

				act_wait_phase(0, 0, g, c);
				act_wait_phase(0, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);
				g.p[0]->place_add_cartime_delay();
				g.p[1]->place_add_cartime_delay();
				g.p[4]->place_add_cartime_delay();
				g.p[5]->place_add_cartime_delay();
				g.p[6]->place_add_cartime_delay();
				g.p[7]->place_add_cartime_delay();
			}
			if (phasenum == 2) {
				act_run_phase_nowith_losttime(2, 0, g, c);
				act_run_phase_nowith_losttime(2, 1, g, c);
				act_wait_phase(0, 0, g, c);
				act_wait_phase(0, 1, g, c);
				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(3, 0, g, c);
				act_wait_phase(3, 1, g, c);
				g.p[0]->place_add_cartime_delay();
				g.p[1]->place_add_cartime_delay();
				g.p[2]->place_add_cartime_delay();
				g.p[3]->place_add_cartime_delay();
				g.p[6]->place_add_cartime_delay();
				g.p[7]->place_add_cartime_delay();

			}
			if (phasenum == 3) {
				act_run_phase_nowith_losttime(3, 0, g, c);
				act_run_phase_nowith_losttime(3, 1, g, c);
				act_wait_phase(0, 0, g, c);
				act_wait_phase(0, 1, g, c);
				act_wait_phase(1, 0, g, c);
				act_wait_phase(1, 1, g, c);
				act_wait_phase(2, 0, g, c);
				act_wait_phase(2, 1, g, c);
				g.p[0]->place_add_cartime_delay();
				g.p[1]->place_add_cartime_delay();
				g.p[2]->place_add_cartime_delay();
				g.p[3]->place_add_cartime_delay();
				g.p[4]->place_add_cartime_delay();
				g.p[5]->place_add_cartime_delay();
			}
		}

	}

	void clear_out_place(Graph &g) {
		for (int i = 0; i < 8; i++) {
			g.pout[i]->contain.clear();
		}
	}

	void print_delay(Graph & g) {
		for (int i = 0; i<8; i++)
		{
			int phase_number = i / 2;
			int phase_dir = i % 2;

			float delay_time_sum1 = 0.0;
			float car_num1 = g.pout[i]->contain.size()*token_per_car;
			for (auto it = g.pout[i]->contain.begin(); it != g.pout[i]->contain.end(); it++) {
				delay_time_sum1 += (*it)->get_delay_time();
			}
			delay_time_sum1 = delay_time_sum1*token_per_car;

			cout << "phase " << phase_number << " dir=" << phase_dir << " 的延时 ： " << setw(10) << delay_time_sum1 << " | 车辆数 ： " << setw(10) << car_num1 << " | 平均延时： " << delay_time_sum1 / car_num1 << endl;
		}

		clear_out_place(g);
	}

	void print_delay2(Graph &g) {
		for (int i = 0; i<8; i++)
		{
			//dir0
			int phase_number = i / 2;
			//int phase_dir = i % 2;

			float delay_time_sum1 = 0.0;
			float car_num1 = g.pout[i]->contain.size()*token_per_car;

			for (auto it = g.pout[i]->contain.begin(); it != g.pout[i]->contain.end(); it++) {
				delay_time_sum1 += (*it)->get_delay_time();
			}
			delay_time_sum1 = delay_time_sum1*token_per_car;

			cout << "phase " << phase_number << " dir=" << 0 << " 的延时 ： " << setw(10) << delay_time_sum1 << " | 车辆数 ： " << setw(10) << car_num1 << " | 平均延时： " << delay_time_sum1 / car_num1 << endl;

			//dir1
			i++;
			float delay_time_sum2 = 0.0;
			float car_num2 = g.pout[i]->contain.size()*token_per_car;

			for (auto it = g.pout[i]->contain.begin(); it != g.pout[i]->contain.end(); it++) {
				delay_time_sum2 += (*it)->get_delay_time();
			}
			delay_time_sum2 = delay_time_sum2*token_per_car;

			cout << "phase " << phase_number << " dir=" << 0 << " 的延时 ： " << setw(10) << delay_time_sum2 << " | 车辆数 ： " << setw(10) << car_num2 << " | 平均延时： " << delay_time_sum2 / car_num2 << "   |  ";

			cout << (delay_time_sum1 + delay_time_sum2) / (car_num1 + car_num2) << endl;

		}
	}

	int max(const vector<float> &waitcar) {
		int index = 0;
		int maxvalue = waitcar[0];

		for (int i = 0; i < waitcar.size(); i++) {
			if (waitcar[i] > maxvalue) {
				index = i;
				maxvalue = waitcar[i];
			}
		}

		return index;
	}

	//返回一个下一次需要执行的相位
	int IsNeedRunPhase(int phasenum, Graph & g, Config & c) {
		//当前相位的车辆数大于7，那么当前相位继续执行，返回true；否则在候选相位里面选择一个等待队列最长的执行
		int index1 = phasenum * 2;
		int index2 = phasenum * 2 + 1;

		if (g.p[index1]->contain.size() + g.p[index2]->contain.size()>140) {
			return phasenum;
		}
		else {
			//从候选相位里面选一个最大的，为了简化代码，就直接从phase0—3所有的相位里面选一个最大的，因为只要是候选相位的车流量往往远大于当前相位
			vector<float> waitcar;
			for (int i = 0; i < phase_num; i++) {
				int wait_index1 = i * 2;
				int wait_index2 = i * 2 + 1;

				waitcar.push_back(g.p[wait_index1]->contain.size() + g.p[wait_index2]->contain.size());
			}
			int max_phasenum = max(waitcar);
			return max_phasenum;
		}
	}

	int ChangetoWaitPhase( Graph & g, Config & c) {
		//int index1 = phasenum * 2;
		//int index2 = phasenum * 2 + 1;

		vector<float> waitcar;
		for (int i = 0; i < phase_num; i++) {
			int wait_index1 = i * 2;
			int wait_index2 = i * 2 + 1;

			waitcar.push_back(g.p[wait_index1]->contain.size() + g.p[wait_index2]->contain.size());
		}
		int max_phasenum = max(waitcar);
		return max_phasenum;
	}

	void print_act_phase_number(vector<int> &act_phase_number) {
		cout << "相位执行的顺序 > " << endl;
		for (int i = 0; i < act_phase_number.size(); i++) {
			cout << act_phase_number[i] << "  ";
		}
		cout << endl;
	}

	//网运行一次
	void act(Graph &g, Config & c) {

		//int phase_run_times[4] = { 0,0,0,0 };
		vector<int> act_phase_number;
		int phase_run_times = 0;

		int cur_phase = 0;
		act_phase(cur_phase, g, c);
		//++phase_run_times[cur_phase];
		act_phase_number.push_back(cur_phase);
		int act_times = 17;  //一次表示10秒，35次表示 360s，也就是6分钟
		int delay_times = 0;
		while (act_times > 0) {
			int next_phase = IsNeedRunPhase(cur_phase, g, c);
			if (next_phase != cur_phase) {
				//下一个相位和当前的相位不同
				////有损失时间的切换相位
				act_phase(next_phase, g, c);
				act_times--;
				phase_run_times = 0;
			}
			//next_phase == cur_phase 继续执行当前的相位
			else{
				//下一个相位和当前执行的相位相同
				//延时10s
				if (phase_run_times>=3) {
					//如果这个相位执行到了最长的绿灯时间，那么在其他的候选相位里面选一个
					int changetophase=ChangetoWaitPhase(g, c);
					next_phase = changetophase;

					//有损失时间的切换相位
					act_phase(next_phase, g, c);
					act_times--;
					phase_run_times = 0;
				}
				else {
					//没有执行到最长的绿灯时间
					int time_10s = 2;
					// 没有损失的切换相位
					act_phase(next_phase, g, c, time_10s);
					delay_times++;
					//如果延时了3次，也就意味着30s，那么act_times--
					if (delay_times % 3 == 1) {
						act_times--;
					}
					phase_run_times++;
				}
			}
			act_phase_number.push_back(next_phase);
			cur_phase = next_phase;

		}
		print_delay2(g);
		print_act_phase_number(act_phase_number);
	}
};

//将流量放大一个陪数
void changeflux(int multi) {
	for (int j = 0; j < road_num; j++) {
		for (int i = 0; i < net_run_time_minutes; i++) {
			flux2[j][i] = flux2[j][i] * multi;
		}
	}
}

int main() {
	//相比较上一个版本 加入了切换相位的损失时间

	changeflux(3);
	Graph g;
	//已经改成了以5秒为单位的输入
	Config c;
	c.convert_to_by_add_car_time();

	//to-do
	Phase phase;


	//网运行一次
	phase.act(g, c);

	system("pause");
	return 0;
}
