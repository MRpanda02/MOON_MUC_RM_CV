#pragma once
#ifndef ENERGY_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
#include"struct_define.h"

using namespace std;
using namespace cv;

#define PI 3.14;
#define ENEMY_BLUE 0//�ҷ���ɫΪ��ɫ
#define ENEMY_RED  1//�з���ɫΪ��ɫ

class Energy {
public:
	Energy();//���캯��
	~Energy();//��������

	bool is_big;//���ģʽΪtrue
	bool is_small;//С��ģʽΪtrue
	void run(cv::Mat& src);//��������
	void showData(cv::Point target_point, cv::Point predict_point, cv::Point circle_center_point);//��ʾ����
	
private:
	WindmillParamFlow _flow;
	McuData mcu_data;

	bool show_energy; //�Ƿ���ʾͼ��
	bool show_process;//�Ƿ���ʾ���Թ���
	bool show_wrong;//�Ƿ���ʾ����
	bool show_data;//�Ƿ���ʾ����

	void initEnergy();//�������س�ʼ��
	void initEnergyPartParam();//�������ز�����ʼ��

	void clearAll();//�����������vector
	void initImage(cv::Mat& src);//ͼ��Ԥ����
	void ArmorDilate(cv::Mat& src);//��װ�װ�ĸ�ʴ������

	void showArmors(std::string windows_name, const cv::Mat& src); //��ʾͼ��������װ�װ�

	std::vector<cv::RotatedRect> fans;//ͼ����������Ҷ
	std::vector<cv::RotatedRect> target_armors;//���ܵ�Ŀ��װ�װ�
	std::vector<cv::RotatedRect> armors;//ͼ�������п���װ�װ�

	cv::RotatedRect target_armor;//Ŀ��װ�װ�

	cv::Point target_point;//Ŀ��װ�װ���������
	cv::Point predict_point;//Ԥ��Ļ��������

	bool findCenterROI(const cv::Mat& src);//��ȡ����R��ѡ��
	bool findTargetInFlowStripFan();//���ѷ��ֵ�������������Ѱ�Ҵ�����װ�װ�
	int  findFans(const cv::Mat& src);//

	bool isValidArmorContour(const vector<cv::Point>& armor_contour);//װ�װ���γߴ�Ҫ��

	int clockwise_rotation_init_cnt;//װ�װ�˳ʱ����ת����
	int anticlockwise_rotation_init_cnt;//װ�װ���ʱ����ת����
	double pointDistance(cv::Point point_1, cv::Point point_2);//�����������
	void getTargetPolarAngle();//���Ŀ��װ�װ弫����Ƕ�
	void getPredictPoint(cv::Point target_point);//��ȡԤ�������
	void rotate(cv::Point target_point);// ����Ԥ��Ļ��������
	void showPredictPoint(std::string windows_name, const cv::Mat& src);//��ʾԤ���
	float predict_rad;//Ԥ����ǰ��
	float predict_rad_norm;//Ԥ����ǰ�ǵľ���ֵ
	float target_polar_angle;//������װ�װ�ļ�����Ƕ�
	float last_target_polar_angle_judge_rotation;//��һ֡������װ�װ�ļ�����Ƕȣ������ж�����


};



#endif //ENERGY_H