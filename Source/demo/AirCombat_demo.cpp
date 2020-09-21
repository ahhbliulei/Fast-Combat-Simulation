
#include "../CombatSimulation/UnitDefine.h"

#include "../TacView/TacViewOutput.h"
#include "../Tools/JoySticks.h"


#include <iostream>
#include <conio.h>
using namespace std;
using namespace TacView;
using namespace Eigen;
using namespace CombatSimulation;

int main() {
	
	//ѡ�������
	int control_method = 0;
	std::cout << "Choose control method:" << endl
		<< "1-JoySticks      2-Keyboard" << endl;
	do {
		std::cout << "Input 1 or 2 :" << endl;
		cin >> control_method;
	} while (control_method != 1 && control_method != 2);
	std::cout << "Begin:" << endl;
	//
	UJoysticks* JS1 = new UJoysticks;
	UJoysticks* JS2 = new UJoysticks;
	DIJOYSTATE JSstate, JSs2tate;

	if (control_method == 1) {
		JS1->Init();
		JS2->Init(1);
		//
	}

	Battlefield_C battlefield;

	battlefield.InitCoordinate(126.0, 30.0, 1000.0);

	//�ɻ�״̬��ʼ��
	battlefield.aircraft_count = 2;
	battlefield.aircraft_list[0].Init(10000001, "F-16",1 , 127.0, 30.0, 20000.0, 0, 5, -90, -0, -80., 0.0);
	battlefield.aircraft_list[1].Init(10000002, "F-16",2 , 127.01, 30.001, 20000.0, 0, 5, -90, 80, 0., 0.0);
	//��ʼ���ɻ����Ʋ���
	battlefield.aircraft_list[0].craft_handle << 0, 0, 0, 40;
	battlefield.aircraft_list[1].craft_handle << 0, 0, 0, 40;

	//����TacView����
	TacViewOutput tacview_show;
	tacview_show.InitServer();

	//���͵�һ֡�ɻ�����
	for (int i = 0; i < battlefield.aircraft_count; i++) {
		string color,pilot;
		if (battlefield.aircraft_list[i].base_team == 1) {
			color = "Red";
			pilot = "AI";
		}
		else if (battlefield.aircraft_list[i].base_team == 2) {
			color = "Blue";
			pilot = "Human";
		}
		tacview_show.InitOneObject(i, battlefield.aircraft_list[i].Sim_id, battlefield.aircraft_list[i].base_name,
			battlefield.aircraft_list[i].base_type, pilot,color,15.5, 9.7, 4.8,
			battlefield.aircraft_list[i].coordinate_longitude, battlefield.aircraft_list[i].coordinate_latitude, battlefield.aircraft_list[i].coordinate_altitude,
			battlefield.aircraft_list[i].coordinate_roll, battlefield.aircraft_list[i].coordinate_pitch, battlefield.aircraft_list[i].coordinate_yaw);
	}
	tacview_show.SendOneFrame(battlefield.time);



	double dt = 0.1;
	int ShootFlag = 0;
	while (1) {
		//*****�л�
		if (battlefield.aircraft_list[0].base_live == 0) {
			battlefield.aircraft_list[0].craft_handle << 0, 0, 0, 0;
		}

		//�ɻ�״̬����
		battlefield.aircraft_list[0].Run(dt);
		//�����ʾ
		tacview_show.OneFrameFlightState(0, battlefield.aircraft_list[0].Sim_id, battlefield.aircraft_list[0].base_live, 
			battlefield.aircraft_list[0].coordinate_longitude, battlefield.aircraft_list[0].coordinate_latitude, battlefield.aircraft_list[0].coordinate_altitude,
			battlefield.aircraft_list[0].coordinate_roll, battlefield.aircraft_list[0].coordinate_pitch, battlefield.aircraft_list[0].coordinate_yaw,
			0, -1, 0, 0);

		//*****�˿ػ�
		//���ռ��̻���ݸ��ź�

		//**���̲���˵����
		//* w: �����Թ̶���������
		//* s: �����Թ̶������¸�
		//* a: �����Թ̶��������
		//* d: �����Թ̶������ҹ�
		//* f: �����Ժ��ΪĿ�귢�䵼��
		char ch;
		if (_kbhit()) {//����а������£���_kbhit()����������
			ch = _getch();//ʹ��_getch()������ȡ���µļ�ֵ
			if (control_method == 2) {
				switch (ch)
				{
				case 'w':battlefield.aircraft_list[1].craft_handle(1) += 0.05; break;
				case 's':battlefield.aircraft_list[1].craft_handle(1) -= 0.05; break;
				case 'a':battlefield.aircraft_list[1].craft_handle(0) -= 0.05; break;
				case 'd':battlefield.aircraft_list[1].craft_handle(0) += 0.05; break;
				case 'f':ShootFlag = 1; break;//��F�������䵼��
				}
			}
			if (ch == 27) { break; }//������ESCʱ����ѭ����ESC���ļ�ֵʱ27.
		}

		//*���Ƹ˲�����
		if (control_method == 1) {
			//
			JS1->getState(&JSstate);
			battlefield.aircraft_list[1].craft_handle(3) = -(JSstate.lZ - 1000) / 25.0;//*����
			JS2->getState(&JSs2tate);

			battlefield.aircraft_list[1].craft_handle(1) = (JSs2tate.lY) / 5000.0;//*����������
			battlefield.aircraft_list[1].craft_handle(0) = (JSs2tate.lX) / 5000.0;//*���������
		}

		//�ɻ�״̬����
		battlefield.aircraft_list[1].Run(dt);
		//�����ʾ
		tacview_show.OneFrameFlightState(1, battlefield.aircraft_list[1].Sim_id, battlefield.aircraft_list[1].base_live,
			battlefield.aircraft_list[1].coordinate_longitude, battlefield.aircraft_list[1].coordinate_latitude, battlefield.aircraft_list[1].coordinate_altitude,
			battlefield.aircraft_list[1].coordinate_roll, battlefield.aircraft_list[1].coordinate_pitch, battlefield.aircraft_list[1].coordinate_yaw,
			0, -1, 0, 0);

		if (ShootFlag == 1) {//���������ж�
			battlefield.MissileFire(battlefield.aircraft_list[1], battlefield.aircraft_list[0]);

			ShootFlag = 0;
		}
		//�������м���
		for (int i = 0; i < battlefield.missile_count; i++) {
			battlefield.missile_list[i].Run(dt);
			string color;
			if (battlefield.missile_list[i].base_team == 1) {
				color = "Red";
			}
			else if (battlefield.missile_list[i].base_team == 2) {
				color = "Blue";
			}
			//������ʾ
			tacview_show.OneFrameMissileState(battlefield.aircraft_count + i, battlefield.missile_list[i].Sim_id, color,
				battlefield.missile_list[i].missile_live, battlefield.missile_list[i].coordinate_longitude, 
				battlefield.missile_list[i].coordinate_latitude, battlefield.missile_list[i].coordinate_altitude, 
				battlefield.missile_list[i].coordinate_roll, battlefield.missile_list[i].coordinate_pitch, 
				battlefield.missile_list[i].coordinate_yaw, battlefield.missile_list[i].p_target_air->Sim_id);
		}

		battlefield.time += dt;
		tacview_show.SendOneFrame(battlefield.time);		
		Sleep(100);
	}


	return 0;
}