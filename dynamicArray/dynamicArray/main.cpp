#include<iostream>
#include<list>
#include<vector>
#include"dynamicTro.h"
using namespace std;
#define pi 3.1415926


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
      ~tro();
      double    Cal_Tra();//求出vel_buff车轮速度

      void AddTask(int taskid,double tra_info[]);//添加tra_path起止位置信息 list
      void CleanTask();
      //获取
      void GetPeriodRef(int & returnflag,double ref_value[],double cur_info[]);
};
tro::tro()
{
	flag=0;shijian=0;
}
tro::~tro()
{
	vel_buff.clear();
	eta_tro_buff.clear();
}
void tro::CleanTask()
{
	vel_buff.clear();
	eta_tro_buff.clear();
}

void tro::AddTask(int taskid,double tra_info[])
{
      _eta_path temp_path;
      temp_path.begin_x=tra_info[0];
      temp_path.begin_y=tra_info[1];
      temp_path.begin_st=tra_info[2];
      temp_path.end_x=tra_info[3];
      temp_path.end_y=tra_info[4];
      temp_path.end_st=tra_info[5];
      for(int i=0;i<6;i++)
        temp_path.eta[i]=tra_info[6+i];
      //将路径点添加到eta_tro_buff
      eta_tro_buff.push_back(temp_path);
}
double tro::Cal_Tra()
{
	_eta_path temp;//Cal_Tra(temp);
  list<_eta_path>::iterator eta_itor;//eta_path的迭代器

  double xa,ya,t_a,xb,yb,t_b,/*n1,n2,n3,n4,n5,n6,*/ka=0,d_ka=0,kb=0,d_kb=0,n[6];
  double a0,a1,a2,a3,a4,a5,a6,a7;
  double b0,b1,b2,b3,b4,b5,b6,b7;
  //定义计算需要的机器人参数，车轮半径、车轮最大转速、轮间距、最大车速、最大离心加速度、最大车的加速度
  double radius_wheel=0.195,wheel_rotation_speed_max=3,dis_2_wheels=0.245*2;
  double velocity_max=0.5,assumed_centrifugal_acceleration_maximum=0.1,abs_vel_acc_max=0.2;
  //定义在运算中用到的变量
  int size=eta_tro_buff.size();
  ////////////////////////////////////////////////////////
  //list<double> x=new list<double>();
  //typedef list <double>  x,y,theta_path,curvature,phi;
  double *x,*y,*theta_path,*curvature,*phi,*rho_candidate_1,*rho_candidate_2,*rho_candidate_3,*temp_candidate,*velocity_maximum,*omega_maximum,*s;
  x=new double [1000*size];y=new double [1000*size];theta_path=new double [1000*size];curvature=new double [1000*size];phi=new double [1000*size];
  rho_candidate_1=new double [1000*size];rho_candidate_2=new double [1000*size];rho_candidate_3=new double [1000*size];
  temp_candidate=new double [1000*size];velocity_maximum=new double [1000*size];omega_maximum=new double [1000*size],s=new double [1000*size];
  //double rho_candidate_1[1000*size],rho_candidate_2[1000*size],rho_candidate_3[1000*size];
  //double temp_candidate[1000*size],velocity_maximum[1000*size],omega_maximum[1000*size];
  double last_point_vel=0,last_point_acc=0;//s[1000*size],;;
  //double *phi,*temp_candidate,*velocity_maximum;
  //phi=(double *)malloc(1001*size*sizeof(double));
  //temp_candidate=(double *)malloc(1001*size*sizeof(double));
  //velocity_maximum=(double *)malloc(1001*size*sizeof(double));
  double d_x,d_y,dd_x,dd_y,last_arclength=0;
  double *vel_acc_max;
  vel_acc_max=new double [1001*size];
  //double *x,*y;
  int total=0;
  int i=total;
  //求出初始的参数并要保存start_point_x(y,theta中 )
  double start_point_x,start_point_y,start_point_theta;
  cout<<"the size of eta_tro_buff:"<<size<<endl;
  eta_itor=eta_tro_buff.begin();
  temp=*eta_itor;
  start_point_x=temp.begin_x;
  start_point_y=temp.begin_y;
  start_point_theta=temp.begin_st;
  //开启循环球出每一段的速度约束、离心力约束下的最大速度
  eta_itor=eta_tro_buff.begin();
  while(eta_itor!=eta_tro_buff.end())
  {
      temp=*eta_itor;
      xa=temp.begin_x;
      ya=temp.begin_y;
      t_a=temp.begin_st;
      xb=temp.end_x;
      yb=temp.end_y;
      t_b=temp.end_st;
      //n1=temp.eta[0];n2=temp.eta[1];n3=temp.eta[2];n4=temp.eta[3];n5=temp.eta[4];n6=temp.eta[5];
      for(int ni=0;ni<6;ni++)
        n[ni]=temp.eta[ni];
      a0=xa;
      a1=n[0]*cos(t_a);
      a2=0.5*n[2]*cos(t_a)-0.5*n[0]*n[0]*ka*sin(t_a);
      a3=(float)1/6*n[4]*cos(t_a)-(float)1/6*(n[0]*n[0]*n[0]*d_ka+3*n[0]*n[2]*ka)*sin(t_a);
      a4=35*(xb-xa)-(20*n[0]+5*n[2]+(float)2/3*n[4])*cos(t_a)+(5*n[0]*n[0]*ka+(float)2/3*n[0]*n[0]*n[0]*d_ka+2*n[0]*n[2]*ka)*sin(t_a)-(15*n[1]-2.5*n[3]+(float)1/6*n[5])*cos(t_b)-(2.5*n[1]*n[1]*kb-(float)1/6*pow(n[1],3)*d_kb-0.5*n[1]*n[3]*kb)*sin(t_b);
      a5=-84*(xb-xa)+(45*n[0]+10*n[2]+n[4])*cos(t_a)-(10*n[0]*n[0]*ka+n[0]*n[0]*n[0]*d_ka+3*n[0]*n[2]*ka)*sin(t_a)+(39*n[1]-7*n[3]+0.5*n[5])*cos(t_b)+(7*n[1]*n[1]*kb-0.5*pow(n[1],3)*d_kb-1.5*n[1]*n[3]*kb)*sin(t_b);
      a6=70*(xb-xa)-(36*n[0]+7.5*n[2]+(float)2/3*n[4])*cos(t_a)+(7.5*n[0]*n[0]*ka+(float)2/3*n[0]*n[0]*n[0]*d_ka+2*n[0]*n[2]*ka)*sin(t_a)-(34*n[1]-6.5*n[3]+0.5*n[5])*cos(t_b)-(6.5*n[1]*n[1]*kb-0.5*n[1]*n[1]*n[1]*d_kb-1.5*n[1]*n[3]*kb)*sin(t_b);
      a7=-20*(xb-xa)+(10*n[0]+2*n[2]+(float)1/6*n[4])*cos(t_a)-(2*n[0]*n[0]*ka+(float)1/6*n[0]*n[0]*n[0]*d_ka+0.5*n[0]*n[2]*ka)*sin(t_a)+(10*n[1]-2*n[3]+(float)1/6*n[5])*cos(t_b)+(2*n[1]*n[1]*kb-(float)1/6*n[1]*n[1]*n[1]*d_kb-0.5*n[1]*n[3]*kb)*sin(t_b);
      b0=ya;
      b1=n[0]*sin(t_a);
      b2=0.5*n[2]*sin(t_a)+0.5*n[0]*n[0]*ka*cos(t_a);
      b3=(float)1/6*n[4]*sin(t_a)+(float)1/6*(pow(n[0],3)*d_ka+3*n[0]*n[2]*ka)*cos(t_a);
      b4=35*(yb-ya)-(20*n[0]+5*n[2]+(float)2/3*n[4])*sin(t_a)-(5*n[0]*n[0]*ka+(float)2/3*n[0]*n[0]*n[0]*d_ka+2*n[0]*n[2]*ka)*cos(t_a)-(15*n[1]-2.5*n[3]+(float)1/6*n[5])*sin(t_b)+(2.5*n[1]*n[1]*kb-(float)1/6*n[1]*n[1]*n[1]*d_kb-0.5*n[1]*n[3]*kb)*cos(t_b);
      b5=-84*(yb-ya)+(45*n[0]+10*n[2]+n[4])*sin(t_a)+(10*n[0]*n[0]*ka+n[0]*n[0]*n[0]*d_ka+3*n[0]*n[2]*ka)*cos(t_a)+(39*n[1]-7*n[3]+0.5*n[5])*sin(t_b)-(7*n[1]*n[1]*kb-0.5*n[1]*n[1]*n[1]*d_kb-1.5*n[1]*n[3]*kb)*cos(t_b);
      b6=70*(yb-ya)-(36*n[0]+7.5*n[2]+(float)2/3*n[4])*sin(t_a)-(7.5*n[0]*n[0]*ka+(float)2/3*n[0]*n[0]*n[0]*d_ka+2*n[0]*n[2]*ka)*cos(t_a)-(34*n[1]-6.5*n[3]+0.5*n[5])*sin(t_b)+(6.5*n[1]*n[1]*kb-0.5*n[1]*n[1]*n[1]*d_kb-1.5*n[1]*n[3]*kb)*cos(t_b);
      b7=-20*(yb-ya)+(10*n[0]+2*n[2]+(float)1/6*n[4])*sin(t_a)+(2*n[0]*n[0]*ka+(float)1/6*n[0]*n[0]*n[0]*d_ka+0.5*n[0]*n[2]*ka)*cos(t_a)+(10*n[1]-2*n[3]+(float)1/6*n[5])*sin(t_b)-(2*n[1]*n[1]*kb-(float)1/6*n[1]*n[1]*n[1]*d_kb-0.5*n[1]*n[3]*kb)*cos(t_b);
      i=total;
      cout<<"a"<<a0<<"  "<<a1<<endl;
      cout<<"b"<<b0<<"  "<<b1<<endl;
      double t=0;
      while(t<=1)
      {
        if(t==0.999)
        {
          cout<<"I am one"<<endl;
        }
        x[i]=a0+a1*t+a2*pow(t,2)+a3*pow(t,3)+a4*pow(t,4)+a5*pow(t,5)+a6*pow(t,6)+a7*pow(t,7);
        y[i]=b0+b1*t+b2*pow(t,2)+b3*pow(t,3)+b4*pow(t,4)+b5*pow(t,5)+b6*pow(t,6)+b7*pow(t,7);

        //       s(i)=sqrt(x(i)*x(i)+y(i)*y(i));
        d_x=a1+2*a2*t+3*a3*pow(t,2)+4*a4*pow(t,3)+5*a5*pow(t,4)+6*a6*pow(t,5)+7*a7*pow(t,6);
        d_y=b1+2*b2*t+3*b3*pow(t,2)+4*b4*pow(t,3)+5*b5*pow(t,4)+6*b6*pow(t,5)+7*b7*pow(t,6);
        dd_x=2*a2+6*a3*t+12*a4*pow(t,2)+20*a5*pow(t,3)+30*a6*pow(t,4)+42*a7*pow(t,5);
        dd_y=2*b2+6*b3*t+12*b4*pow(t,2)+20*b5*pow(t,3)+30*b6*pow(t,4)+42*b7*pow(t,5);
        theta_path[i]=atan2(d_y,d_x);

        curvature[i]=(d_x*dd_y-dd_x*d_y)/pow(pow(d_x,2)+pow(d_y,2),1.5);
        phi[i]=atan2((d_x*dd_y-dd_x*d_y),pow(pow(d_x,2)+pow(d_y,2),1.5) );
        rho_candidate_1[i]=fabs(velocity_max/cos(phi[i]));
        rho_candidate_2[i]=sqrt(2*assumed_centrifugal_acceleration_maximum/fabs(sin(2*phi[i])));
        rho_candidate_3[i]=fabs(radius_wheel*wheel_rotation_speed_max/(dis_2_wheels*sin(phi[i])+cos(phi[i])));
        //选择最小的pho_candidate
        if (rho_candidate_1[i]<=rho_candidate_2[i])
             temp_candidate[i]=rho_candidate_1[i];
             if (rho_candidate_1[i]>rho_candidate_3[i])
                  temp_candidate[i]=rho_candidate_3[i];
        if (rho_candidate_1[i]>rho_candidate_2[i])
             temp_candidate[i]=rho_candidate_2[i];
             if (rho_candidate_2[i]>rho_candidate_3[i])
                   temp_candidate[i]=rho_candidate_3[i];
        //线速度velocity
        velocity_maximum[i]=temp_candidate[i]*cos(phi[i]);
        //角速度omega
        omega_maximum[i]=temp_candidate[i]*sin(phi[i]);
        double time=0,dis=0;
        while(time<=t)
        {
          d_x=a1+2*a2*time+3*a3*pow(time,2)+4*a4*pow(time,3)+5*a5*pow(time,4)+6*a6*pow(time,5)+7*a7*pow(time,6);
          d_y=b1+2*b2*time+3*b3*pow(time,2)+4*b4*pow(time,3)+5*b5*pow(time,4)+6*b6*pow(time,5)+7*b7*pow(time,6);
          dis+=sqrt(pow(d_x,2)+pow(d_y,2));
          time+=0.001;
        }
        s[i]=dis+last_arclength;
        t=t+0.001;
        i=i+1;
      }
      total=i;
      cout<<"total:"<<total<<endl;
      last_arclength=s[total-1];
      cout<<"last_arclength"<<"   "<<s[999]<<endl;
      eta_itor++;
  }
  //寻找开始阶段，全力加速到达rho_maximum的位置，在该位置之前全力加速
  i=0;
  double temp_vel=0;
  int point_num=total-1,speed_up_num;
  double *time,*delta_s,*delta_t;
  time=new double [1000*size];delta_s=new double [1000*size];delta_t=new double [1000*size];
  cout<<"point_num:"<<point_num<<endl;
  while (i<=point_num)//此时point_num=1000*size-1
  {
      if (i==0)
      {
          velocity_maximum[i]=last_point_vel;
          temp_candidate[i]=velocity_maximum[i]/cos(phi[i]);
          vel_acc_max[i]=last_point_acc;
          time[i]=0;
          //cout<<"time"<<time[i]<<endl;
      }
      if (i!=0)//全力加速
      {
          vel_acc_max[i]=abs_vel_acc_max;
          delta_s[i]=sqrt(pow(x[i]-x[i-1],2)+pow(y[i]-y[i-1],2));
          delta_t[i]=(-velocity_maximum[i-1]+sqrt(pow(velocity_maximum[i-1],2)+2*vel_acc_max[i]*delta_s[i]))/vel_acc_max[i];
          temp_vel=velocity_maximum[i-1]+vel_acc_max[i]*delta_t[i];
          time[i]=delta_t[i]+time[i-1];
          cout<<"time:i"<<i<<"  "<<time[i-1]<<endl;
          if (temp_vel>=velocity_maximum[i])
          {
              speed_up_num=i;
              i=point_num;
          }
          if (temp_vel<velocity_maximum[i])
          {
              velocity_maximum[i]=temp_vel;
              temp_candidate[i]=velocity_maximum[i]/cos(phi[i]);
              omega_maximum[i]=temp_candidate[i]*sin(phi[i]);
          }
      }

      i=i+1;

  }
  //寻找结束阶段，有rho_maximum全力减速到0的位置，在该位置之后全力减速
  //此时的point_num，1001*size-1

  velocity_maximum[point_num]=0.0;
  temp_candidate[point_num]=0.0;
  phi[point_num]=0.0;
  int  num=point_num;
  int j,speed_down_num;
  while (num>=1)//因为下面有j-1 所以num的最小值为1
  {
      j=num;
      delta_s[j-1]=sqrt(pow(x[j]-x[j-1],2)+pow(y[j]-y[j-1],2));
      delta_t[j-1]=(-velocity_maximum[j]+sqrt(pow(velocity_maximum[j],2)+2*abs_vel_acc_max*delta_s[j-1]))/abs_vel_acc_max;
      temp_vel=velocity_maximum[j]+abs_vel_acc_max*delta_t[j-1];
      if (temp_vel>=velocity_maximum[j-1])
      {
          speed_down_num=j-1;
          num=1;
      }
      if (temp_vel<velocity_maximum[j-1])
      {
          velocity_maximum[j-1]=temp_vel;
          temp_candidate[j-1]=velocity_maximum[j-1]/cos(phi[j-1]);
          omega_maximum[j-1]=temp_candidate[j-1]*sin(phi[j]-1);
      }
      num=num-1;
  }
  cout<<"speed_down_num:"<<speed_down_num<<endl;
  //对rho_maximum加入始末状态为0的约束，根据可达的最大线速度对rho_maximum进行优化
  for(i=1;i<=point_num;i++)
  {
      //从起点的加速过程
      if( velocity_maximum[i-1]<velocity_maximum[i])
      {
          vel_acc_max[i]=abs_vel_acc_max;
          delta_s[i]=sqrt(pow(x[i]-x[i-1],2)+pow(y[i]-y[i-1],2));
          delta_t[i]=(-velocity_maximum[i-1]+sqrt(pow(velocity_maximum[i-1],2)+2*vel_acc_max[i]*delta_s[i]))/vel_acc_max[i];
          temp_vel=velocity_maximum[i-1]+vel_acc_max[i]*delta_t[i];
          if (temp_vel<=velocity_maximum[i])
          {
              velocity_maximum[i]=temp_vel;
              temp_candidate[i]=velocity_maximum[i]/cos(phi[i]);
              omega_maximum[i]=temp_candidate[i]*sin(phi[i]);
          }
      }
  }
  i=point_num;
  while(i>=1)
  {
      //从终点的加速过程
      if (velocity_maximum[i-1]>velocity_maximum[i])
      {
          vel_acc_max[i-1]=abs_vel_acc_max;
          delta_s[i-1]=sqrt(pow(x[i]-x[i-1],2)+pow(y[i]-y[i-1],2));
          delta_t[i-1]=(-velocity_maximum[i]+sqrt(pow(velocity_maximum[i],2)+2*vel_acc_max[i-1]*delta_s[i-1]))/vel_acc_max[i-1];
          temp_vel=velocity_maximum[i]+vel_acc_max[i-1]*delta_t[i-1];
          if (temp_vel<=velocity_maximum[i-1])
          {
              velocity_maximum[i-1]=temp_vel;
              temp_candidate[i-1]=velocity_maximum[i-1]/cos(phi[i-1]);
              omega_maximum[i-1]=temp_candidate[i-1]*sin(phi[i-1]);
          }
      }
      i=i-1;
  }
  //对velocity_maximum,omega_maximum以及temp_candidate进行小数点后保留4位处理
  for (i=0;i<=point_num;i++)
  {
      velocity_maximum[i]=velocity_maximum[i]*10000;
      velocity_maximum[i]=round(velocity_maximum[i])/10000;
      temp_candidate[i]=temp_candidate[i]*10000;
      temp_candidate[i]=round(temp_candidate[i])/10000;
      omega_maximum[i]=omega_maximum[i]*10000;
      omega_maximum[i]=round(omega_maximum[i])/10000;
  }
  //对全力加速段进行积分
  //首先重新计算在速度曲线优化后的总时间
  time[0]=0;
  double temp_t=0;
  for (i=1;i<=point_num;i++)
  {
      if (velocity_maximum[i]>velocity_maximum[i-1])//加速段 注意不是每一段的加速度都是最大加速度，有的段不是
      {
          delta_s[i]=sqrt(pow(x[i]-x[i-1],2)+pow(y[i]-y[i-1],2));
          delta_t[i]=2*delta_s[i]/(velocity_maximum[i]+velocity_maximum[i-1]);
          temp_t=delta_t[i]*1000;
          delta_t[i]=round(temp_t)/1000;
          vel_acc_max[i]=(velocity_maximum[i]-velocity_maximum[i-1])/delta_t[i];
          time[i]=delta_t[i]+time[i-1];
      }
      if (velocity_maximum[i]-velocity_maximum[i-1]==0)//匀速段
      {
          vel_acc_max[i]=0;
          delta_s[i]=sqrt(pow(x[i]-x[i-1],2)+pow(y[i]-y[i-1],2));
          delta_t[i]=delta_s[i]/velocity_maximum[i];
          temp_t=delta_t[i]*1000;
          delta_t[i]=round(temp_t)/1000;
          time[i]=delta_t[i]+time[i-1];
      }

      if (velocity_maximum[i]<velocity_maximum[i-1])///减速段
      {
          delta_s[i]=sqrt(pow(x[i]-x[i-1],2)+pow(y[i]-y[i-1],2));
          delta_t[i]=2*delta_s[i]/(velocity_maximum[i]+velocity_maximum[i-1]);
          temp_t=delta_t[i]*1000;
          delta_t[i]=round(temp_t)/1000;
          vel_acc_max[i]=(velocity_maximum[i]-velocity_maximum[i-1])/delta_t[i];
          time[i]=delta_t[i]+time[i-1];
      }
  }
  double total_time=time[point_num],time_consumed=0,start=0;
  double dt=0.001,omega_acc,road_traveled;//1ms
  i=0;j=1;

  double *total_time_consumed,temp_speed,temp_omega,last_temp_speed,last_temp_omega;
  total_time_consumed=new double [1000*size];
  double p_x,p_y,theta,road,last_theta,last_road;
  double right_wheel,left_wheel,last_p_x,last_p_y;
  int m;
  //fid=fopen('planned_trajectory.txt','wt');
  //定义一个wel_wheel的变量用来向eta_buff中添加数据
  _vel_wheel my_wheel;
  while (i<=point_num)
  {
      if (i==0)
      {
          last_temp_speed=0;
          last_temp_omega=0;
          last_p_x=start_point_x;
          last_p_y=start_point_y;
          last_theta=start_point_theta;
          last_road=0;
          total_time_consumed[i]=0;
          m=1;
      }
      else
      {
      //temp_time_consumed=delta_t(i)*1000;
      //time_consumed=round(temp_time_consumed)/1000;
          time_consumed=delta_t[i];
          start=0.001;
          //for (t=start:dt:time_consumed)
          double t=start;
          while(t<=time_consumed)
          {
              temp_speed=last_temp_speed+vel_acc_max[i]*dt;
              omega_acc=(omega_maximum[i]-omega_maximum[i-1])/time_consumed;
              temp_omega=last_temp_omega+omega_acc*dt;
              right_wheel=(2*temp_speed+temp_omega*dis_2_wheels)/(2*radius_wheel);
              my_wheel.v_r=right_wheel;
              left_wheel=(2*temp_speed-temp_omega*dis_2_wheels)/(2*radius_wheel);
              my_wheel.v_l=left_wheel;
              vel_buff.push_back(my_wheel);
              theta=last_theta+(last_temp_omega*dt+0.5*omega_acc*dt*dt);
              p_x=last_p_x+(temp_speed*dt+0.5*vel_acc_max[i]*dt*dt)*cos(theta);
              p_y=last_p_y+(temp_speed*dt+0.5*vel_acc_max[i]*dt*dt)*sin(theta);
              road_traveled=temp_speed*dt+0.5*vel_acc_max[i]*dt*dt;
              road=last_road+road_traveled;
              //fprintf(fid,'%f %f\n',p_x(j),p_y(j));
              last_temp_speed=temp_speed;
              last_temp_omega=temp_omega;
              last_theta=theta;
              last_p_x=p_x;
              last_p_y=p_y;
              last_road=road;
              t=t+dt;
          }
          total_time_consumed[i]=total_time_consumed[i-1]+time_consumed;
      }
      shijian=total_time_consumed[i];
      i=i+1;

      //cout<<"shijian"<<shijian<<endl;
  }

  return shijian;
}
void tro::GetPeriodRef(int &returnflag,double ref_info[],double cur_info[])
{
    if((unsigned int)flag<vel_buff.size())
      {
          _vel_wheel temp=vel_buff[flag];
          cur_info[0]=temp.v_l;
          cur_info[1]=temp.v_r;
          flag++;
          returnflag=0;
          return ;
      }
      else
      {
          flag=0;
          vel_buff.clear();
          if(eta_tro_buff.empty())
          {
              returnflag=1;
              return ;
          }
          else
          {
              //eta_path temp=eta_tro_buff.front();
              Cal_Tra();
              //tra_path.pop_front();
              returnflag=2;
              return ;
          }
      }
}
int main()
{
	//Row.row=new int [10];
	tro tro1();
	tro *tro12;
	tro12=new tro();
	int task=1;
	double tra_info[12];tra_info[0]=1;tra_info[1]=0;tra_info[2]=0;tra_info[3]=3;tra_info[4]=3;tra_info[5]=pi/2;tra_info[6]=1;tra_info[7]=1;
	for(int i=2;i<6;i++)
	    tra_info[6+i]=1;
	//添加第一段路径信息
	tro12->AddTask(task,tra_info);
	tra_info[0]=3;tra_info[1]=3;tra_info[2]=pi/2;tra_info[3]=5;tra_info[4]=0;tra_info[5]=0;tra_info[6]=1;tra_info[7]=1;
	for(int i=2;i<6;i++)
	    tra_info[6+i]=0;
	//添加第二段路径信息
	tro12->AddTask(task,tra_info);
	cout<<endl<<tro12->Cal_Tra()<<endl;
	int flag=-1;double myref[2],mycur[2];
	tro12->GetPeriodRef(flag,myref,mycur);
	cout<<"myflag="<<flag<<endl;
    cout<<mycur[0]<<" "<<mycur[1]<<endl;
    cout<<1<<tro12->vel_buff.size()<<endl;
	getchar();
}