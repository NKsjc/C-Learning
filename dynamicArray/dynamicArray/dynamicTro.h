#ifndef _DYNAMIC_H
#define _DYNAMIC_H
#include<list>
#include<vector>
#include<iostream>
#include<math.h>
typedef struct dynamic
{
	int *row;
}Row;
//���崫�������X ��Y �� theta ��eta[6]
struct _eta_path
{
  double begin_x;
  double begin_y;
  double begin_st;
  double end_x;
  double end_y;
  double end_st;
  double eta[6];
};//��ȷ����ֹλ�ú��eta����·��
struct _vel_wheel
{
    double v_l;double v_r;//������������
};
double round(double r)
{
	return(r>0.0)? floor(r+0.5):ceil(r-0.5);
}
#define contrl_buff vector <_vel_wheel> 
#define queue_eta_path list <_eta_path> 
#endif