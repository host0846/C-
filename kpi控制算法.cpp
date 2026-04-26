#include<iostream>
#include<windows.h>
#include<fstream>
#include<ctime>
#include<random>
using namespace std;
int main(){
	ofstream dataFile("U_t.csv");
	dataFile << "U_t值"<< endl;
	double h=10000;
	double K_p=0.5;
	double K_i=0.1;
	double K_d=0.1;//k_d<0.2
	double U_t=0;
	double err=0;
	double sum_err=0;
	double miner_err=0;
	double err_all[1000];
	int i=0;
	while(i!=50){
		sum_err=0;
		miner_err=0;
		err=h-U_t;
		err_all[i]=err;
		for(int j=0;j<=i;j++){
			sum_err=sum_err+err_all[j];//误差总和
		}
		for(int j=0;j<=i;j++){
			miner_err=miner_err-err_all[j];//误差总差
		}
		U_t+=K_p*err+K_i*sum_err+K_d*miner_err;//PID控制算法
		U_t-=rand() % 2,19;
		cout<<U_t<<" ";
		dataFile <<U_t << endl;
		i++;
	}
	cout<<"已保存于D:,"<<"李承羲的文件夹,"<<"公开"<<endl;
	return 0;
}
