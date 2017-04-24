#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

using namespace std;

const int net_run_times = 5;  //�����еĴ���
const int net_run_time_minutes = net_run_times * 2;	// �����е�ʱ�� ����λ�Ƿ��ӣ�
const int net_run_time_second = net_run_time_minutes * 60;//�����е�ʱ�� ��λ����

const int frequency = 10; // 1���Ӽ���10��
						  //const int transmit_times = net_run_time_second / 10;  //��Ǩ�����Ĵ���

const float rate = 0.1;       // 0.1�� ����ͨ��0.1����
const float fix_phase_time = 30;  //�̶���λ���̵Ƴ�����30s
const int phase_num = 4;        // ��4����λ
const int road_num = 4;	//��·��
const int add_car_time = 10;   //�����������10s��10s����һ��addcar��
const int second_per_minutes = 60;  //1������60s

//float flux2[road_num][net_run_time_minutes] = {
//	{ 15,15,15,15,15,15,15,15,15,15 },  //road1 
//	{ 15,15,15,15,15,15,15,15,15,15 },  //road2
//	{ 15,15,15,15,15,15,15,15,15,15 },
//	{ 15,15,15,15,15,15,15,15,15,15 },
//};


float flux2[road_num][net_run_time_minutes] = {
	{ 11,15,3,16,13,23,4,23,17,23 },  //road1  ������·������-��԰) 2012-11-11 09:42~10:41
	{ 9,19,6,18,3,19,18,12,8,17 },  //road2
	{ 16,17,18,6,18,8,18,13,9,17 },
	{ 14,11,8,23,9,23,10,18,8,19 },
};

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

	//����һ����ʱ����0.1s ����rate=0.1������£�����һ��ֻ��ͨ��һ����
	//����һ��transmit ��ʾ����һ��
	void transmit() {
		// ��Ǩ�ĵ�ǰ����
		if (IsEnable() == true) {
			Token * t;
			t = contain.front();
			contain.pop_front();
			nextplace->contain.push_back(t);

			//����û���뿪�����ĳ��� ��ʱ++
			for (auto it = contain.begin(); it != contain.end(); it++) {
				(*it)->add_delay_time();
			}
		}
		else {
			//û�г����Լ���
		}
	}

	//10s �ڿ��Խ���ĳ��� ������0.5������ôflux_per_add_car_time=0.5
	void addcar(float flux_per_add_car_time) {
		//0.1������ʾһ��token
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

	//����ǰ������������б�Ǩ����10���ӵ���ʱ
	void place_add_10s_delay() {
		for (int i = 0; i < add_car_time*frequency; i++) {
			// ����һ��������������б�Ǩ ����0.1�����ʱ
			for (auto it = contain.begin(); it != contain.end(); it++) {
				(*it)->add_delay_time();
			}
		}
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
public:
	Place out00;   //out ������
	Place out01;
	Place out10;
	Place out11;
	Place out20;
	Place out21;
	Place out30;
	Place out31;

	vector<Place> p;   //p������
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
	//flux_00 ��ʾ��ת��flux_01 ��ʾֱ��
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

	//�Է���Ϊ��λ ת���� ��add_car_timeΪ��λ
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
	//phasenum ��ʾ��λ�ţ�direction ��ʾľһ����λ���������ķ���times��ʾ���ǵڼ���ִ��
	void act_per_add_car_time(int phasenum, int direction, Graph &g, Config & c) {
		//static int  times = 0; ����Ҫ�޸� ���극������

		int index = phasenum * 2 + direction;
		int times = flux_times[index];

		//��vector�����ȡ����
		vector<float> &ref = c.phase_flux_add_car_time[index];
		float flux_10s = ref[times];

		g.p[index].addcar(flux_10s);
		flux_times[index]++;
	}


	// ÿ��10s����addcarһ�Σ�����ֻ���̵Ƶ�ʱ�򣬲Ż�transmit������10(Ƶ��) *10s=100��
	void act_transmit(int phasenum, int direction, Graph &g, Config & c) {
		int index = phasenum * 2 + direction;
		//10s ��ѭ��100�Σ�����һ��transmit��ʾ����һ��
		for (int i = 0; i < add_car_time * frequency; i++) {
			g.p[index].transmit();
		}
	}

	//10s
	void act_run_phase(int phasenum, int direction, Graph &g, Config & c) {
		act_per_add_car_time(phasenum, direction, g, c);
		act_transmit(phasenum, direction, g, c);
	}

	//10s
	void act_wait_phase(int phasenum, int direction, Graph &g, Config & c) {
		act_per_add_car_time(phasenum, direction, g, c);
	}


	//����һ���������ʱ10s
	void add_phase(int phasaenum, Graph & g, Config & c) {

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

	//����һ����һ����Ҫִ�е���λ
	int IsNeedRunPhase(int phasenum, Graph & g, Config & c) {
		//��ǰ��λ�ĳ���������7����ô��ǰ��λ����ִ�У�����true�������ں�ѡ��λ����ѡ��һ���ȴ��������ִ��
		int index1 = phasenum * 2;
		int index2 = phasenum * 2 + 1;

		if (g.p[index1].contain.size()>70||g.p[index2].contain.size()>70 ) {
			return phasenum;
		}
		else {
			//�Ӻ�ѡ��λ����ѡһ�����ģ�Ϊ�˼򻯴��룬��ֱ�Ӵ�phase0��3���е���λ����ѡһ�����ģ���ΪֻҪ�Ǻ�ѡ��λ�ĳ���������Զ���ڵ�ǰ��λ
			vector<float> waitcar;
			for (int i = 0; i < phase_num; i++) {
				int wait_index1 = i * 2;
				int wait_index2 = i * 2 + 1;
				
				waitcar.push_back(g.p[wait_index1].contain.size() + g.p[wait_index2].contain.size());
			}

			int max_phasenum = max(waitcar);
			return max_phasenum;
		}
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

				//g.p[0].place_add_10s_delay();
				//g.p[1].place_add_10s_delay();
				g.p[2].place_add_10s_delay();
				g.p[3].place_add_10s_delay();
				g.p[4].place_add_10s_delay();
				g.p[5].place_add_10s_delay();
				g.p[6].place_add_10s_delay();
				g.p[7].place_add_10s_delay();


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

				g.p[0].place_add_10s_delay();
				g.p[1].place_add_10s_delay();
				//g.p[2].place_add_10s_delay();
				//g.p[3].place_add_10s_delay();
				g.p[4].place_add_10s_delay();
				g.p[5].place_add_10s_delay();
				g.p[6].place_add_10s_delay();
				g.p[7].place_add_10s_delay();
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

				g.p[0].place_add_10s_delay();
				g.p[1].place_add_10s_delay();
				g.p[2].place_add_10s_delay();
				g.p[3].place_add_10s_delay();
				//g.p[4].place_add_10s_delay();
				//g.p[5].place_add_10s_delay();
				g.p[6].place_add_10s_delay();
				g.p[7].place_add_10s_delay();

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

				g.p[0].place_add_10s_delay();
				g.p[1].place_add_10s_delay();
				g.p[2].place_add_10s_delay();
				g.p[3].place_add_10s_delay();
				g.p[4].place_add_10s_delay();
				g.p[5].place_add_10s_delay();
				//g.p[6].place_add_10s_delay();
				//g.p[7].place_add_10s_delay();
			}


			//g.p[0].place_add_10s_delay();
			//g.p[1].place_add_10s_delay();
			//g.p[2].place_add_10s_delay();
			//g.p[3].place_add_10s_delay();
			//g.p[4].place_add_10s_delay();
			//g.p[5].place_add_10s_delay();
			//g.p[6].place_add_10s_delay();
			//g.p[7].place_add_10s_delay();

		}

	}

	void clear_out_place(Graph &g) {
		g.out00.contain.clear();
		g.out01.contain.clear();
		g.out10.contain.clear();
		g.out11.contain.clear();

		g.out20.contain.clear();
		g.out21.contain.clear();
		g.out30.contain.clear();
		g.out31.contain.clear();
	}

	void print_delay(Graph & g) {
		//phase 1
		float delay_time_sum1 = 0.0;
		float car_num1 = g.out00.contain.size()*0.1;
		for (auto it = g.out00.contain.begin(); it != g.out00.contain.end(); it++) {
			delay_time_sum1 += (*it)->get_delay_time();
		}
		delay_time_sum1 = delay_time_sum1*0.1;

		float delay_time_sum2 = 0.0;
		float car_num2 = g.out01.contain.size()*0.1;
		for (auto it = g.out01.contain.begin(); it != g.out01.contain.end(); it++) {
			delay_time_sum2 += (*it)->get_delay_time();
		}
		delay_time_sum2 = delay_time_sum2*0.1;
		cout << "phase 1 dir=0 ����ʱ �� " << setw(10) << delay_time_sum1 << " | ������ �� " << setw(10) << car_num1 << " | ƽ����ʱ�� " << delay_time_sum1 / car_num1 << endl;
		cout << "phase 1 dir=1 ����ʱ �� " << setw(10) << delay_time_sum2 << " | ������ �� " << setw(10) << car_num2 << " | ƽ����ʱ�� " << delay_time_sum2 / car_num2 << endl;

		//phase2 
		float delay_time_sum3 = 0.0;
		float car_num3 = g.out10.contain.size()*0.1;
		for (auto it = g.out10.contain.begin(); it != g.out10.contain.end(); it++) {
			delay_time_sum3 += (*it)->get_delay_time();
		}
		delay_time_sum3 = delay_time_sum3 *0.1;

		float delay_time_sum4 = 0.0;
		float car_num4 = g.out11.contain.size()*0.1;
		for (auto it = g.out11.contain.begin(); it != g.out11.contain.end(); it++) {
			delay_time_sum4 += (*it)->get_delay_time();
		}
		delay_time_sum4 = delay_time_sum4 *0.1;
		cout << "phase 2 dir=0 ����ʱ �� " << setw(10) << delay_time_sum3 << " | ������ �� " << setw(10) << car_num3 << " | ƽ����ʱ�� " << delay_time_sum3 / car_num3 << endl;
		cout << "phase 2 dir=1 ����ʱ �� " << setw(10) << delay_time_sum4 << " | ������ �� " << setw(10) << car_num4 << " | ƽ����ʱ�� " << delay_time_sum4 / car_num4 << endl;


		//phase3
		float delay_time_sum5 = 0.0;
		float car_num5 = g.out20.contain.size()*0.1;
		for (auto it = g.out20.contain.begin(); it != g.out20.contain.end(); it++) {
			delay_time_sum5 += (*it)->get_delay_time();
		}
		delay_time_sum5 = delay_time_sum5 *0.1;

		float delay_time_sum6 = 0.0;
		float car_num6 = g.out21.contain.size()*0.1;
		for (auto it = g.out21.contain.begin(); it != g.out21.contain.end(); it++) {
			delay_time_sum6 += (*it)->get_delay_time();
		}
		delay_time_sum6 = delay_time_sum6 *0.1;

		cout << "phase 3 dir=0 ����ʱ �� " << setw(10) << delay_time_sum5 << " | ������ �� " << setw(10) << car_num5 << " | ƽ����ʱ�� " << delay_time_sum5 / car_num5 << endl;
		cout << "phase 3 dir=1 ����ʱ �� " << setw(10) << delay_time_sum6 << " | ������ �� " << setw(10) << car_num6 << " | ƽ����ʱ�� " << delay_time_sum6 / car_num6 << endl;

		//phase4
		float delay_time_sum7 = 0.0;
		float car_num7 = g.out30.contain.size()*0.1;
		for (auto it = g.out30.contain.begin(); it != g.out30.contain.end(); it++) {
			delay_time_sum7 += (*it)->get_delay_time();
		}
		delay_time_sum7 = delay_time_sum7 *0.1;

		float delay_time_sum8 = 0.0;
		float car_num8 = g.out31.contain.size()*0.1;
		for (auto it = g.out31.contain.begin(); it != g.out31.contain.end(); it++) {
			delay_time_sum8 += (*it)->get_delay_time();
		}
		delay_time_sum8 = delay_time_sum8 *0.1;

		cout << "phase 4 dir=0 ����ʱ �� " << setw(10) << delay_time_sum7 << " | ������ �� " << setw(10) << car_num7 << " | ƽ����ʱ�� " << delay_time_sum7 / car_num7 << endl;
		cout << "phase 4 dir=1 ����ʱ �� " << setw(10) << delay_time_sum8 << " | ������ �� " << setw(10) << car_num8 << " | ƽ����ʱ�� " << delay_time_sum8 / car_num8 << endl;

		clear_out_place(g);
		cout << endl;
	}

	//������һ��
	void act(Graph &g, Config & c) {
		//phase1
		//act_phase(0, g, c);
		////phase2
		//act_phase(1, g, c);
		////phase3
		//act_phase(2, g, c);
		////phase4
		//act_phase(3, g, c);

		int cur_phase = 0;
		act_phase(cur_phase, g, c);

		int act_times = 11;
		while (act_times > 0) {
			int next_phase = IsNeedRunPhase(cur_phase, g, c);
			act_phase(next_phase, g, c);
			act_times--;
			
		}
		print_delay(g);
	}
};

int main() {
	Graph g;
	Config c;
	Phase phase;
	c.convert_to_by_add_car_time();

	// ������һ��
	//for (int i = 0; i <5; i++) {
	phase.act(g, c);
	//}





	system("pause");
	return 0;
}