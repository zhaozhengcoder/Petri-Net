#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

//直行 4代表四个相位，5代表5分钟
//int flux2[4][5] = {
//	{ 10,5,10,3,3 },
//	{ 7,8,13,9,4 },
//	{ 12,7,6,9,8 },
//	{ 7,8,9,12,11 }
//};

const int row = 10;
const int list = 4;

float rate = 5;   // 每一个激发周期里面 离开路口的车的数量 rate=10，表示1s 离开路口的车的数量是1辆，一个周期有10s，所以rate=10
float frequency = 10;   // 
// 
int flux2[list][row]= {
	{ 10,10,10,10,10,10,10,10,10,10 },  //road1 
	{ 10,10,10,10,10,10,10,10,10,10 },  //road2
	{ 10,10,10,10,10,10,10,10,10,10 },
	{ 10,10,10,10,10,10,10,10,10,10 },
};

//flux2[0][0] =10 表示进入road1的车辆 每分钟是10辆，50%进入 p13，30% 进入p12；一个激发周期是10s，所以一个周期内进入p13的是 0.8辆，进入p12 的是0.5辆
//如果rate=10，也就意味着一个激发的周期，要带走10辆车，也就是说，一个周期进入p13的车辆是0.8，带走的是10


//如果rate=5，那么对于phase1来说，从phase1~phase4 需要2分钟，其中有30s是绿灯，可以带走15辆车，在两分钟内来的车辆数目是10辆（如果 flux2[0][0] =10 的话）


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
	int minutes = 5;  // 网运行的时间
	vector<float> flux_12;
	vector<float > flux_13;

	vector<float> flux_22;
	vector<float> flux_23;
	
	vector<float> flux_32;
	vector<float> flux_33;
	
	vector<float> flux_42;
	vector<float> flux_43;

	Config() {
		//flux f12,f13
		float straight = 0.5;
		float left = 0.3;

		for (int i = 0; i < this->minutes; i++) {
			float flux = flux2[0][i];
			flux_12.push_back(flux*left);
			flux_13.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i <  this->minutes; i++) {
			float flux = flux2[1][i];
			flux_22.push_back(flux*left);
			flux_23.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i <  this->minutes; i++) {
			float flux = flux2[2][i];
			flux_32.push_back(flux*left);
			flux_33.push_back(flux*straight);
		}

		//flux f12,f13
		for (int i = 0; i < this->minutes; i++) {
			float flux = flux2[3][i];
			flux_42.push_back(flux*left);
			flux_43.push_back(flux*straight);
		}
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

void transmit(Place &pre, Place &post, float rate) {
	//先判断一下能否enable
	if (IsPreEnable(pre, rate) == true && IsPostEnable(post, rate) == true) {
		pre.volume = pre.volume - rate;
		post.volume = post.volume + rate;
	}
	else {
		//前集的变迁太少
		if (IsPreEnable(pre, rate) == false) {
			post.volume = post.volume + pre.volume;
			pre.volume = 0;
			return;
		}
		//后集的变迁可能要超出容量
		if (IsPostEnable(post, rate) == false) {
			pre.volume = pre.volume - (post.capacity - post.volume);
			post.volume = post.capacity;
			return;
		}
	}
}

//opt 指的是对应的这个相位，每分钟产生的车辆
float well_distributed(float opt) {
	float ret = opt / 6.0;
	return ret;
}

//每一次激发时间内 进入的车辆 这里是10s ,opt 指的是对应的这个相位，每分钟产生的车辆
void add_car(Place & place,float opt) {
	place.volume = place.volume + well_distributed(opt);
	//cout << "add car : " << well_distributed(opt) << endl;
}


//10s 激发一次，一次进入 离开的车辆的数目
void run_phase1(Graph & g,Config & config,int index) {
	//rate= ，指的是车辆离开路口1秒钟通过1辆
	

	float  opt1 = config.flux_13[index]; //30s的流量
	float  opt2 = config.flux_33[index];

	// 进入
	add_car(g.P13, opt1);
	//离开
	transmit(g.P13,g.out13,rate);

	//进入
	add_car(g.P33, opt2);
	// 离开
	transmit(g.P33, g.out33, rate);
}

void wait_phase1(Graph & g, Config & config, int index) {
	float  opt1 = config.flux_13[index];
	float  opt2 = config.flux_33[index];
	add_car(g.P13, opt1);
	add_car(g.P33, opt2);
}

void run_phase2(Graph & g, Config & config, int index) {
	//int rate = 5;
	float opt1 = config.flux_12[index];
	float opt2 = config.flux_32[index];

	add_car(g.P12, opt1);
	transmit(g.P12, g.out12, rate);

	add_car(g.P32, opt2);
	transmit(g.P32, g.out32, rate);
}

void wait_phase2(Graph & g, Config & config, int index) {
	float opt1= config.flux_12[index]; //30s的流量
	float opt2 = config.flux_32[index];
	add_car(g.P12, opt1);
	add_car(g.P32, opt2);
}

void run_phase3(Graph & g, Config & config, int index) {
	float opt1 = config.flux_23[index];
	float opt2 = config.flux_43[index];
	add_car(g.P23, opt1);
	transmit(g.P23, g.out23, rate);

	add_car(g.P43, opt2);
	transmit(g.P43, g.out43, rate);
}

void wait_phase3(Graph & g, Config & config, int index) {
	float opt1 = config.flux_23[index];
	float opt2 = config.flux_43[index];
	add_car(g.P23, opt1);
	add_car(g.P43, opt2);
}

void run_phase4(Graph & g, Config & config, int index) {
	float opt1 = config.flux_22[index];
	float opt2 = config.flux_42[index];


	add_car(g.P22, opt1);
	transmit(g.P22, g.out22, rate);

	add_car(g.P42, opt2);
	transmit(g.P42, g.out42, rate);
}

void wait_phase4(Graph & g, Config & config, int index) {
	float opt1 = config.flux_23[index];
	float opt2 = config.flux_43[index];

	add_car(g.P22, opt1);
	add_car(g.P42, opt2);
}

// 执行phase1 
void act_phase1(Graph & g,Config &config,int times=3) {
	//一次10s 循环三次 30s
	static int num = 0;
	//cout << "phase1 执行次数: " << num << endl;
	int index = num / 2;

	// 激发三次，每次10s
	for (int i = 0; i < times; i++) {
		run_phase1(g,config,index);
		wait_phase2(g, config, index);
		wait_phase3(g, config, index);
		wait_phase4(g, config, index);
	}
	cout << "phase1 第" << num << "次 执行完毕后" << endl;
	g.print();
	num++;
}

void act_phase2(Graph & g,Config & config,int times = 3) {
	static int num = 0;
	//cout << "phase2 执行次数: " << num+1 << endl;
	int index = num / 2;

	for (int i = 0; i < times; i++) {
		run_phase2(g, config, index);
		wait_phase1(g, config, index);
		wait_phase3(g, config, index);
		wait_phase4(g, config, index);
	}
	cout << "phase2 第" << num << "次 执行完毕后" << endl;
	g.print();
	num++;
}

void act_phase3(Graph & g, Config & config,int times = 3) {
	static int num = 0;
	//cout << "phase3 执行次数: " << num+1 << endl;
	int index = num / 2;
	for (int i = 0; i < times; i++) {
		run_phase3(g, config, index);
		wait_phase1(g, config, index);
		wait_phase2(g, config, index);
		wait_phase4(g, config, index);
	}
	cout << "phase3 第" << num << "次 执行完毕后" << endl;
	g.print();
	num++;
}

void act_phase4(Graph & g, Config & config,int times = 3) {
	static int num = 0;
	//cout << "phase4 执行次数: " << num +1<< endl;
	int index = num / 2;
	for (int i = 0; i < times; i++) {
		run_phase4(g, config,index);
		wait_phase1(g, config, index);
		wait_phase2(g, config, index);
		wait_phase3(g, config, index);
	}
	cout << "phase4 第" << num << "次 执行完毕后" << endl;
	g.print();
	num++;
}

//opt 指的是对应的这个相位，每分钟产生的车辆
//每个相位30秒，4个相位依次执行一次，所以执行一次act的时间是2分钟
void act(Graph & g, Config & config, int times = 3) {
	act_phase1(g, config);
	act_phase2(g, config);
	act_phase3(g, config);
	act_phase4(g, config);
}

int main() {
	Graph g;
	Config c;
	g.print();

	for (int i = 0; i <10; i++) {
		act(g, c);
	}
	

	system("pause");
	return 0;
}