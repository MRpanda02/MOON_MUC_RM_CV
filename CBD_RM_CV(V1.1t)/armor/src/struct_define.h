#pragma once
#ifndef STRUCT_DEFINE_H
#define STRUCT_DEFINE_H
#include<opencv2/opencv.hpp>
#include<iostream>


struct WindmillParamFlow {
	int RED_GRAY_THRESH;//�з���ɫʱ����ֵ
	int BLUE_GRAY_THRESH;//�з���ɫʱ����ֵ
	float armor_contour_area_max;//װ�װ�����ɸѡ����
	float armor_contour_area_min;
	float armor_contour_length_max;
	float armor_contour_length_min;
	float armor_contour_width_max;
	float armor_contour_width_min;
	float armor_contour_hw_ratio_max;
	float armor_contour_hw_ratio_min;

	float flow_strip_fan_contour_area_max;//������������Ҷ�����ɸѡ����
	float flow_strip_fan_contour_area_min;
	float flow_strip_fan_contour_length_max;
	float flow_strip_fan_contour_length_min;
	float flow_strip_fan_contour_width_max;
	float flow_strip_fan_contour_width_min;
	float flow_strip_fan_contour_hw_ratio_max;
	float flow_strip_fan_contour_hw_ratio_min;
	float flow_strip_fan_contour_area_ratio_max;
	float flow_strip_fan_contour_area_ratio_min;

	float Strip_Fan_Distance_max;//��������װ�װ�������
	float Strip_Fan_Distance_min;

	float flow_strip_contour_area_max;//��������ز���ɸѡ
	float flow_strip_contour_area_min;
	float flow_strip_contour_length_max;
	float flow_strip_contour_length_min;
	float flow_strip_contour_width_max;
	float flow_strip_contour_width_min;
	float flow_strip_contour_hw_ratio_max;
	float flow_strip_contour_hw_ratio_min;
	float flow_strip_contour_area_ratio_min;
	float flow_strip_contour_intersection_area_min;

	long target_intersection_contour_area_min;

	float twin_point_max;

	float Center_R_Control_area_max;//����R����ز���ɸѡ
	float Center_R_Control_area_min;
	float Center_R_Control_length_max;
	float Center_R_Control_length_min;
	float Center_R_Control_width_max;
	float Center_R_Control_width_min;
	float Center_R_Control_radio_max;
	float Center_R_Control_radio_min;
	float Center_R_Control_area_radio_min;
	float Center_R_Control_area_intersection_area_min;

	float flow_area_max;//��Ҷ��ز���ɸѡ
	float flow_area_min;
	float flow_length_max;
	float flow_length_min;
	float flow_width_max;
	float flow_width_min;
	float flow_aim_max;
	float flow_aim_min;
	float flow_area_ratio_min;
};

struct McuData {
	float curr_yaw;      // ��ǰ��̨yaw�Ƕ�
	float curr_pitch;    // ��ǰ��̨pitch��
	uint8_t state;       // ��ǰ״̬������-���-С��
	uint8_t mark;        // ��̨�Ƕȱ��λ
	uint8_t anti_top;    // �Ƿ�Ϊ������ģʽ
	uint8_t enemy_color; // �з���ɫ
	int delta_x;         // ��������x�Ჹ����
	int delta_y;         // ��������y�Ჹ����
};












#endif