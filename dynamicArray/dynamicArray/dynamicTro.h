#ifndef _DYNAMIC_H
#define _DYNAMIC_H
#include<list>
#include<vector>
#include<iostream>

typedef struct dynamic
{
	int *row;
}Row;
//���崫�������X ��Y �� theta ��eta[6]
typedef struct _eta_path
{
  double begin_x;
  double begin_y;
  double begin_st;
  double end_x;
  double end_y;
  double end_st;
  double eta[6];
}eta_path;//��ȷ����ֹλ�ú��eta����·��
typedef struct _vel_wheel
{
    double v_l;double v_r;//������������
}vel_wheel
typedef vector <_vel_wheel> contrl_buff;
typedef list <_eta_path> queue_eta_path;

//��������vector����contrl_buff-----eta_buff�����ٶ�
//��������list����queue_eta_path--------tra_path��ֹλ����Ϣ
class tro
{
public:
  double shijian;
  contrl_buff vel_buff;//
  queue_eta_path eta_tro_buff;
  int flag;
public:
      tro();
      virtual ~tro();
      double    Cal_Tra();//���vel_buff�����ٶ�

      void AddTask(int taskid,double tra_info[]);//���tra_path��ֹλ����Ϣ list
      void CleanTask();
      //��ȡ
      void GetPeriodRef(int & returnflag,double ref_value[],double cur_info[]);
  };

#endif