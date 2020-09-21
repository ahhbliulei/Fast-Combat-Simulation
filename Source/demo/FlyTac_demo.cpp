#include "../FlyTac/aircraft.h"
#include "../FlyTac/missile.h"
#include "../TacView/TacViewOutput.h"
#include "../Tools/JoySticks.h"


#include <iostream>
#include <conio.h>
using namespace std;
using namespace TacView;
using namespace Eigen;

int main()
{

	

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

	//�ɻ�״̬��ʼ��
	Matrix4d craft_state[2];
	double xn0, yn0, zn0;
	earth_to_navigation(&xn0, &yn0, &zn0, 127.0, 30.0, 20000.0, 126.0, 30.0, 1000.0);
	Vector4d qbn0;
	euler_to_quaternion_bn(&qbn0, 0, 5, -90);

	craft_state[0] << xn0, yn0, zn0, 0.0,
		-0, -80., 0., 0.,
		qbn0(0), qbn0(1), qbn0(2), qbn0(3),
		0, 0, 0, 0;

	

	double xn1, yn1, zn1;
	earth_to_navigation(&xn1, &yn1, &zn1, 127.01, 30.001, 20000.0, 126.0, 30.0, 1000.0);
	Vector4d qbn1;
	euler_to_quaternion_bn(&qbn1, 0, 5, -90);

	craft_state[1] << xn1, yn1, zn1, 0.0,
		80, 0., 0., 0.,
		qbn1(0), qbn1(1), qbn1(2), qbn1(3),
		0, 0, 0, 0;

	//����TacView����
	TacViewOutput tacview_show;
	tacview_show.InitServer();

	//���͵�һ֡�ɻ�����
	tacview_show.InitOneObject(0, 10000001, "F-16", "Air+FixedWing", "ai_gaoyang", "Red",
		15.5, 9.7, 4.8, 127.0, 30.0, 20000.0, 0.0, 5.0, -90.0);
	tacview_show.InitOneObject(1, 10000002, "F-16", "Air+FixedWing", "gaoyang", "Blue",
		15.5, 9.7, 4.8, 127.01, 30.001, 20000.0, 0.0, 5.0, -90.0);

	tacview_show.SendOneFrame(0.0);

	//��ʼ���ɻ����Ʋ���
	Vector4d handle0,handle1;
	handle0 << 0, 0, 0, 40;
	handle1 << 0, 0, 0, 50;
	double errA0 = 0, errP0 = 0, errAsum0=0 ,errPsum0=0,errR0=0;
	double errA1 = 0, errP1 = 0, errAsum1 = 0, errPsum1 = 0, errR1 = 0;


	//��ʼ��ս��״̬����
	double index = 0;
	float dt = 0.1;
	int ShootFlag = 0;
	int missileNum = 0;
	Matrix4d missile_state[max_object];
	Vector4d missile_handle[max_object];
	int missile_life[max_object] = { 1 };
	int craft_life[max_object] ;
	fill(craft_life, craft_life + max_object, 1);
	double missile_errA[max_object] = { 0 }, missile_errP[max_object] = { 0 }, missile_errR[max_object] = { 0 },
		missile_errAsum[max_object] = { 0 }, missile_errPsum[max_object] = { 0 };
	
	while (1) {
		// ÿһ֡

		//*****�л�
		if (craft_life[0] == 0) {
			handle0 << 0, 0, 0, 0;
		}
		//�ɻ�״̬����
		Flight(&craft_state[0], craft_state[0], dt, handle0);
		//����ת��
		double lon0 = 126.0, lat0 = 30.0, alt0 = 10000;
		navigation_to_earth(&lon0, &lat0, &alt0, craft_state[0](0, 0), craft_state[0](0, 1),
			craft_state[0](0, 2), 126.0, 30.0, 1000.0);
		double roll0 = 0, pitch0 = 0, yaw0 = 90;
		quaternion_bn_to_euler(&roll0, &pitch0, &yaw0, craft_state[0].row(2));
		//�����ʾ
		tacview_show.OneFrameFlightState(0, 10000001, craft_life[0], lon0,
			lat0, alt0, roll0, pitch0, yaw0, 0, -1, 0, 0);


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
				case 'w':handle1(1) += 0.05; break;
				case 's':handle1(1) -= 0.05; break;
				case 'a':handle1(0) -= 0.05; break;
				case 'd':handle1(0) += 0.05; break;
				case 'f':ShootFlag = 1; break;//��F�������䵼��
				}
			}
			if (ch == 27) { break; }//������ESCʱ����ѭ����ESC���ļ�ֵʱ27.
		}

		if (control_method == 1) {
			//
			JS1->getState(&JSstate);
			handle1(3) = -(JSstate.lZ - 1000) / 25.0;
			JS2->getState(&JSs2tate);

			handle1(1) = (JSs2tate.lY) / 5000.0;
			handle1(0) = (JSs2tate.lX) / 5000.0;
		}


		//�ɻ�״̬����
		Flight(&craft_state[1], craft_state[1], dt, handle1);

		//����ת��
		double lon1= 127.0, lat1= 30.0, alt1= 10000;
		navigation_to_earth(&lon1, &lat1, &alt1, craft_state[1](0, 0), craft_state[1](0, 1),
			craft_state[1](0, 2), 126.0, 30.0, 1000.0);
		double roll1=0, pitch1=0, yaw1=270;
		quaternion_bn_to_euler(&roll1, &pitch1, &yaw1, craft_state[1].row(2));
		//�����ʾ
		tacview_show.OneFrameFlightState(1, 10000002, craft_life[1], lon1,
			lat1, alt1, roll1, pitch1, yaw1, 0, -1, 0, 0);

		int object_id = 0;
		if (ShootFlag == 1) {//���������ж�
			object_id = 2 + missileNum;

			int missile_id = 20000001 + object_id;

			tacview_show.OneFrameMissileState(object_id, missile_id, "Blue", 1,
				lon1, lat1,	alt1, roll1, pitch1, yaw1, 10000001);

			missile_state[missileNum] = craft_state[1];
			missile_life[missileNum] = 1;
			missile_handle[missileNum] << 0, 0, 0, 90;

			missileNum++;
			
			ShootFlag = 0;
		}

		//����
		for (int i = 0; i < missileNum; i++) {
			if (missile_life[i] != 0) {
				object_id = 2 + i;

				Vector3d TargetMissile;
				
				//������Ŀ����루�ף�
				double distance = (craft_state[0].row(0) - missile_state[i].row(0)).norm();

				//�����˶�Ŀ�������
				double K_target = craft_state[0].row(1).norm() / (0.001 * (distance));
				if (distance <= 250)
					K_target = 195 * craft_state[0].row(1).norm() / missile_state[i].row(1).norm() ;
				TargetMissile << craft_state[0](0, 0) + (craft_state[0](1, 0) / craft_state[0].row(1).norm())* K_target,
					craft_state[0](0, 1) + (craft_state[0](1, 1) / craft_state[0].row(1).norm())* K_target,
					craft_state[0](0, 2) + (craft_state[0](1, 2) / craft_state[0].row(1).norm())* K_target;

				//���������ж�
				if (distance <= 250) {

					missile_life[i] = 0;

					craft_life[0] = 0;

					cout << "!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				Flight_find_point(&missile_handle[i], &missile_errA[i], &missile_errP[i], &missile_errR[i],
					&missile_errAsum[i], &missile_errPsum[i], missile_state[i], 90,0.1, TargetMissile);
				if (missile_life[i] == 0) {
					missile_handle[i] << 0, 0, 0, 0;
				}else{
					if (missile_life[i]++ >= 3 * 60 / dt) {
						missile_life[i] = 0;
					}
				}

				//�������м���
				missile_Flight(&missile_state[i], missile_state[i], dt, missile_handle[i]);
				//����ת��
				double lonM = 127.0, latM = 30.0, altM = 10000;
				navigation_to_earth(&lonM, &latM, &altM, missile_state[i](0, 0), missile_state[i](0, 1),
					missile_state[i](0, 2), 126.0, 30.0, 1000.0);
				double rollM = 0, pitchM = 0, yawM = 270;
				quaternion_bn_to_euler(&rollM, &pitchM, &yawM, missile_state[i].row(2));

				//������ʾ
				int missile_id = 20000001 + object_id;
				tacview_show.OneFrameMissileState(object_id, missile_id, "Blue", missile_life[i],
					lonM, latM, altM, rollM, pitchM, yawM, 10000001);
			}
		}

		tacview_show.SendOneFrame(index);
		index+=dt;
		Sleep(100);
	}

	std::cout << "end" << endl;

	return 0;
}