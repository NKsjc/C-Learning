#ifndef _DYNAMIC_H
#define _DYNAMIC_H
#include<list>
#include<vector>
#include<iostream>

typedef struct dynamic
{
	int *row;
}Row;
//定义传入的数据X 、Y 、 theta 、eta[6]
typedef struct _eta_path
{
  double begin_x;
  double begin_y;
  double begin_st;
  double end_x;
  double end_y;
  double end_st;
  double eta[6];
}eta_path;//由确定起止位置后的eta决定路径
typedef struct _vel_wheel
{
    double v_l;double v_r;//定义左右轮速
}vel_wheel
typedef vector <_vel_wheel> contrl_buff;
typedef list <_eta_path> queue_eta_path;

//包含变量vector类型contrl_buff-----eta_buff车轮速度
//包含变量list类型queue_eta_path--------tra_path起止位置信息
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
      double    Cal_Tra();//求出vel_buff车轮速度

      void AddTask(int taskid,double tra_info[]);//添加tra_path起止位置信息 list
      void CleanTask();
      //获取
      void GetPeriodRef(int & returnflag,double ref_value[],double cur_info[]);
  };

#endif