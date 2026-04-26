#include<bits/stdc++.h>
#include<graphics.h>
using namespace std;
float Rotate_x,Rotate_y,g,Light_UPD=1;
float canvasWidth = 4, canvasHeight = 2;
int imageWidth = 1024, imageHeight = 512;
int ld=0,rd=0,md=0,LD,mx,my,shd,spd,key[256],moved,wheel,Time;
int recnum,DayTime;
int dighash,digtime,hand,Use;
int hard[50],lev[50],TOT,BLK,NOB,ATKTIME,Dim=0;map<string,int>toint;
float health=20;
bool show[114514];
struct ITEM
{
	int num=0,type=0,nj=20;
}bag[20][10];
PIMAGE texture[100],des[20];
//#include"noise.h"
#include"math.h"
Vec3i respawn;
Vec3f player(0,50,0),playerMove;vector<Vec3f>point;vector<Vec2i>campoint;map<Vec3i,int>Light;
struct Rect
{
	int ind[4],k;string type;Vec3i bl;
	Rect(Vec3i b,int A,int B,int C,int D,int K,string t) : ind({A,B,C,D}),k(K),bl(b),type(t) {}
	float dist()
	{
		Vec3f ta=(point[ind[0]]+point[ind[1]]+point[ind[2]]+point[ind[3]])*0.25f-player;
		return ta.lenth();
	}
	bool operator<(Rect b){return dist()==b.dist()?0:dist()>b.dist();}
};
struct Noob
{
	Vec3i pos,to;
	float health;int type,hurttime=0,lasthurt,AI=0;
	map<string,int>mem;
};
vector<Noob>noob;
map<Vec3i,int>world[5];
map<string,int>rule;
vector<Rect> rects;
Matrix44 worldToplayer;
int d(Vec2i a,Vec2i b)
{
	a=a-b;
	return sqrt(a.x*a.x+a.y*a.y);
}
bool wt(Vec3i pos)
{
	if(world[Dim].count(pos)&&world[0][pos]!=toint["water"]&&world[0][pos]!=toint["water_flow"])return 1;
	return 0;
}
void mk_cube(Vec3i pos,int k)
{
	int IND=point.size(),x=pos.x,y=pos.y,z=pos.z;
	for(int t1=-1;t1<=1;t1+=2)
		for(int t2=-1;t2<=1;t2+=2)	
			for(int t3=-1;t3<=1;t3+=2)
				point.push_back(Vec3f(pos.x*16+8*t1,pos.y*16+8*t2,pos.z*16+8*t3));
	//--- --+ -+- -++ +-- +-+ ++- +++
	Vec3i bl(x,y,z);
	if(!wt({x-1,y,z}))
		rects.push_back(Rect(bl,IND+0,IND+1,IND+3,IND+2,k,"x-"));
	if(!wt({x+1,y,z}))
		rects.push_back(Rect(bl,IND+4,IND+5,IND+7,IND+6,k,"x+"));
	if(!wt({x,y-1,z}))
		rects.push_back(Rect(bl,IND+0,IND+1,IND+5,IND+4,k,"y-"));
	if(!wt({x,y+1,z}))
		rects.push_back(Rect(bl,IND+2,IND+3,IND+7,IND+6,k,"y+"));
	if(!wt({x,y,z-1}))
		rects.push_back(Rect(bl,IND+0,IND+2,IND+6,IND+4,k,"z-"));
	if(!wt({x,y,z+1}))
		rects.push_back(Rect(bl,IND+1,IND+3,IND+7,IND+5,k,"z+"));
}
void slove_light(int x, int y, int z, int LT)
{
	Light[Vec3i(x, y, z)] = LT;
	Vec3i Minp;
	int Min = 1e9,U=100;
	queue<Vec3i>q;
	map<Vec3i, int>vis;
	q.push(Vec3i(x,y, z));
	vis[Vec3i(x, y, z)] = LT;
	while (!q.empty())
	{
		Vec3i tmp = q.front();
		q.pop();
		if (vis[tmp]<=0)continue;
		Light[tmp]=vis[tmp];
		for (int dx = -1; dx <= 1; dx++)
			for (int dy = -1; dy <= 1; dy++)
				for (int dz = -1; dz <= 1; dz++)
					if (abs(dx) + abs(dz) + abs(dy) == 1)
					{
						if (world[Dim].count(tmp + Vec3i(dx, dy, dz)))continue;
						if (vis.count(tmp + Vec3i(dx, dy, dz))||Light[tmp+ Vec3i(dx, dy, dz)]>=Light[tmp])continue;
						vis[tmp + Vec3i(dx, dy, dz)] = vis[tmp] - 1;
						q.push(tmp + Vec3i(dx, dy, dz));
					}
	}
	return;
}
void calc()
{
	int PRC=10;
	if(DayTime>=12000&&DayTime<=14000)PRC=-7*DayTime/2000 + 52;
	if(DayTime>=14000&&DayTime<=22000)PRC=3;
	if(DayTime>=22000&&DayTime<=24000)PRC=7*DayTime/2000 + 3-77;
	if(Time%2==0||Light_UPD)
	{
		if(Time%20==0)Light.clear();
		for(int i=f45(player.x/16.0)-rule["Rander_Dist"];i<=f45(player.x/16.0)+rule["Rander_Dist"];i++)
			for(int j=f45(player.z/16.0)-rule["Rander_Dist"];j<=f45(player.z/16.0)+rule["Rander_Dist"];j++)
			{
				int T=min(255,f45(player.y/16.0)+rule["Rander_Dist"]);
				while(!world[Dim].count(Vec3i(i,T,j))&&T>f45(player.y/16.0)-rule["Rander_Dist"])
					T--;
				if(rule["Light_Change"])slove_light(i,T+1,j,PRC);
				else slove_light(i,T+1,j,10);
			}
		Vec3i Pl=Vec3i(f45(player.x/16.0),f45(player.y/16.0),f45(player.z/16.0));
		for(map<Vec3i,int>::iterator it=world[Dim].begin();it!=world[Dim].end();it++)
			if(it->second==toint["torch"]&&abs(it->first.x-Pl.x)+abs(it->first.y-Pl.y)+abs(it->first.z-Pl.z)<=rule["Rander_Dist"])
			slove_light(it->first.x,it->first.y,it->first.z,8);
		Light_UPD=0;
	}
	Vec3f X(1,0,0),Y(0,1,0),Z(0,0,1);
	X.rotate_y(Rotate_y);Y.rotate_y(Rotate_y);Z.rotate_y(Rotate_y);
	X.rotate_x(Rotate_x);Y.rotate_x(Rotate_x);Z.rotate_x(Rotate_x);
	Matrix44 playerToWorld(X.x, Y.x, Z.x, 0, X.y, Y.y, Z.y, 0, X.z, Y.z, Z.z, 0, player.x, player.y, player.z, 1);
	worldToplayer = playerToWorld.inverse();
	campoint.clear();memset(show,1,sizeof(show));
	for(auto i:point)
	{
		Vec3f tmp;
		worldToplayer.multVecMatrix(i,tmp);
		if(tmp.z>0)show[campoint.size()]=0;
		tmp.x = tmp.x / -tmp.z;
		tmp.y = tmp.y / -tmp.z;
		tmp.x=(tmp.x/canvasWidth+0.5)*imageWidth;
		tmp.y=(0.5-tmp.y/canvasHeight)*imageHeight;
		campoint.push_back({tmp.x,tmp.y});
	}
}
void line(Vec2i a,Vec2i b){line(a.x,a.y,b.x,b.y);}
void outtextxy(int x,int y,string t){outtextxy(x,y,t.c_str());}
int get_noob(Vec3i pos)
{
	int I=0;
	for(Noob i:noob)
	{
		if(i.pos==pos)return I;
		I++;
	}
	return -1;
}
color_t rgb(int r,int g,int b)
{
	return EGERGB(max(0,min(255,r)),max(0,min(255,g)),max(0,min(255,b)));
}
void updata()
{
	rects.clear();point.clear();
	Vec3i Pl=Vec3i(f45(player.x/16.0),f45(player.y/16.0),f45(player.z/16.0));
	for(map<Vec3i,int>::iterator it=world[Dim].begin();it!=world[Dim].end();it++)
		if(it->second!=0&&abs(it->first.x-Pl.x)+abs(it->first.y-Pl.y)+abs(it->first.z-Pl.z)<=rule["Rander_Dist"])mk_cube(it->first,it->second);
	for(auto i:noob)mk_cube(i.pos,i.type);
}
Noob mk_noob(Vec3i pos,int type)
{
	Noob tmp;
	tmp.pos=pos;tmp.type=type;tmp.AI=0;
	Vec3i Pl=Vec3i(f45(player.x/16.0),f45(player.y/16.0),f45(player.z/16.0));
	tmp.mem["dis_pl"]=abs(Pl.x-pos.x)+abs(Pl.y-pos.y)+abs(Pl.z-pos.z);
	if(type==toint["pig"])tmp.health=10,tmp.AI=1;
	if(type==toint["cow"])tmp.health=15,tmp.AI=1;
	if(type==toint["sheep"])tmp.health=10,tmp.AI=1;
	if(type==toint["creeper"])tmp.health=20;
	if(type==toint["zombie"])tmp.health=20;
	return tmp;
}
void ky()
{
	while(kbmsg())
	{
		key_msg t=getkey();
		if(t.msg==key_msg_down)
		{
			key[t.key]=1;
			if(t.key==key_esc)rule["config_mode"]=!rule["config_mode"];
			if(t.key==key_shift)shd=1;
			if(t.key==key_space)spd=1;
		}
		if(t.msg==key_msg_up)
		{
			key[t.key]=0;
			if(t.key==key_shift)shd=0;
			if(t.key==key_space)spd=0;
		}
	}
}
bool inb(Vec3i p)
{
	int x=p.x,y=p.y,z=p.z;
	if(world[Dim].count({x,y,z})&&world[Dim][{x,y,z}]!=0&&world[Dim][{x,y,z}]!=toint["nether"]||get_noob({x,y,z})!=-1)return 1;
	return 0;
}
bool inb(Vec3f p)
{
	return inb(Vec3i(f45(p.x/16.0),f45(p.y/16.0),f45(p.z/16.0)));
}
void putimg(int x, int y,PCIMAGE f,int bl=2)
{
	int bo=getpixel(0, 0, f)==EGERGB(0,255,0);
	for (int i = 0; i < getwidth(f)*bl; i++)
		for (int j = 0; j < getheight(f)*bl; j++)
		{
			color_t col = getpixel(i/bl, j/bl, f);
			if(bo?col!=EGERGB(0,255,0):col!=EGERGB(255,255,255))putpixel(x + i, j + y, col);
		}
}
void putimg(int x, int y,PCIMAGE f,string light)
{
	int bo=getpixel(0, 0, f)==EGERGB(0,255,0);
	for (int i = 0; i < getwidth(f); i++)
		for (int j = 0; j < getheight(f); j++)
		{
			color_t col = getpixel(i, j, f);
			if(bo?col!=EGERGB(0,255,0):col!=EGERGB(255,255,255))
			{
				if(col==BLACK&&light=="1")putpixel(x + i, j + y, WHITE);	
					else putpixel(x + i, j + y, col);	
			}
		}
}
void dead()
{
	PIMAGE f=newimage();
	getimage(f,0,0,1024,512);
	LOGFONTW tmp;
	getfont(&tmp);
	setfont(64,0,"Fixedsys");
	while(1)
	{
		for (int i = 0; i < getwidth(f); i++)
			for (int j = 0; j < getheight(f); j++)
			{
				color_t col = getpixel(i, j, f);
				putpixel(i+rand()%10-5, j+rand()%10-5, rgb(EGEGET_R(col)+50,EGEGET_G(col)-50,EGEGET_B(col)-50));
			}
		setcolor(WHITE);
		outtextxy(364,169,"You Dead");
		outtextxy(364,269,"点击R重生");
		ky();
		if(key['R'])break;
		delay_ms(100);
	}
	noob.clear();
	setfont(&tmp);
	delimage(f);
}
void bad(int pr)
{
	PIMAGE f=newimage();
	getimage(f,0,0,1024,512);
	
	int T=10;if(pr==0)pr=1;else pr=-1;
	while(T<255)
	{
		for (int i = 0; i < getwidth(f); i++)
			for (int j = 0; j < getheight(f); j++)
			{
				color_t col = getpixel(i, j, f);
				putpixel(i+rand()%(T/10)-(T/20), j+rand()%(T/10)-(T/20), rgb(EGEGET_R(col)-T*pr,EGEGET_G(col)-T*pr,EGEGET_B(col)-T*pr));
			}
		ky();T+=3;
		delay_ms(100);
	}
	delimage(f);
}
void Atk(float d)
{
	if(d<0)return;
	ATKTIME=15;
	float v=0;
	health-=max(0.5,d*(1.0-min(20.0,0.2*v)/25.0));
}
bool line_check(Vec3f a,Vec3f b=player)
{
	Vec3f p=b-a;int PR=f45(p.lenth()/16.0);
	p.normalize();p=p*16;
	while(PR--)
	{
		a=a+p;
		if(inb(a))return 1;
	}
	return 0;
}
void print()
{
	cleardevice();
	Sort(rects,0,rects.size()-1);
	if(key['Q'])system("cls");
	recnum=0;
	for(Rect i:rects)
	{
		if(show[i.ind[0]]+show[i.ind[1]]+show[i.ind[2]]+show[i.ind[3]]==0)continue;
		if(i.type=="x-"&&point[i.ind[0]].x<player.x)continue;
		if(i.type=="x+"&&point[i.ind[0]].x>player.x)continue;
		if(i.type=="y-"&&point[i.ind[0]].y<player.y)continue;
		if(i.type=="y+"&&point[i.ind[0]].y>player.y)continue;
		if(i.type=="z-"&&point[i.ind[0]].z<player.z)continue;
		if(i.type=="z+"&&point[i.ind[0]].z>player.z)continue;
		if(rule["Rander"]&&line_check(point[i.ind[0]])&&line_check(point[i.ind[1]])&&line_check(point[i.ind[2]])&&line_check(point[i.ind[3]]))continue;
		int a[8];
		for(int j=0;j<4;j++)a[j*2]=campoint[i.ind[j]].x,a[j*2+1]=campoint[i.ind[j]].y;
		int x=i.bl.x*16-8,y=i.bl.y*16-8,z=i.bl.z*16-8,f=(i.type[1]=='+');
		Vec2i pos[17][17];
		int zl=1;
		if(rule["Rander_Level"]==0)zl=1;
		if(rule["Rander_Level"]==1)
		{
			zl=2;
			if(i.dist()>60)zl=16;
			if(i.dist()>20)zl=4;
		}
		if(rule["Rander_Level"]==2)
		{
			if(i.dist()>60)zl=16;
			if(i.dist()>30)zl=4;
		}
		if(rule["Rander_Level"]==3)
		{
			if(i.dist()>100)zl=16;
			if(i.dist()>60)zl=16;
			if(i.dist()>40)zl=2;
		}
		
		for(int I=0;I<=16/zl;I++)
			for(int J=0;J<=16/zl;J++)
				{
					Vec3f tmp,Tmp;
					if(i.type[0]=='x')tmp=Vec3f(x+16*f,y+I*zl,z+J*zl);
					if(i.type[0]=='y')tmp=Vec3f(x+I*zl,y+16*f,z+J*zl);
					if(i.type[0]=='z')tmp=Vec3f(x+I*zl,y+J*zl,z+16*f);
					worldToplayer.multVecMatrix(tmp,Tmp);
					if(Tmp.z>0)pos[I][J]=Vec2i(0,0);
					else if(abs(Tmp.z)>0.001)
					{
						Tmp.x = Tmp.x / -Tmp.z;
						Tmp.y = Tmp.y / -Tmp.z;
						pos[I][J]=Vec2i((Tmp.x/canvasWidth+0.5)*imageWidth,(0.5-Tmp.y/canvasHeight)*imageHeight);
					}
					
				}
		color_t* k;
		k=getbuffer(texture[i.k]);
		for(int I=1;I<=16/zl;I++)
			for(int J=1;J<=16/zl;J++)
				if(pos[I-1][J-1].x&&pos[I-1][J].x&&pos[I][J].x&&pos[I][J-1].x)
				{
					int poly[]={pos[I-1][J-1].x,pos[I-1][J-1].y,pos[I-1][J].x,pos[I-1][J].y,pos[I][J].x,pos[I][J].y,pos[I][J-1].x,pos[I][J-1].y};
					color_t col;
					if(i.k>=0)col=k[(J*zl-1) * 16 + I*zl-1];
					else col=WHITE;
					if(get_noob(i.bl)!=-1&&noob[get_noob(i.bl)].hurttime>0)col=rgb(EGEGET_R(col)+80,EGEGET_G(col),EGEGET_B(col));
					if(get_noob(i.bl)!=-1&&noob[get_noob(i.bl)].mem["boom_time"]>0&&Time%4<=2)col=WHITE;
					if(i.bl.x*33+i.bl.y*114514+i.bl.z*2245+1919810==dighash)
					{
						int tmp=getpixel(I*zl-1,J*zl-1,des[min(9,digtime*10/hard[i.k])]);
						if(tmp!=WHITE)col=tmp;
					}
					int L=min(pos[I-1][J-1].x,pos[I][J].x),R=max(pos[I-1][J-1].x,pos[I][J].x);
					int U=min(pos[I-1][J-1].y,pos[I][J].y),D=max(pos[I-1][J-1].y,pos[I][J].y);
					if(i.k==toint["water"]||i.k==toint["water_flow"])
					{
						color_t c=getpixel((L+R)/2,(U+D)/2);
						col=rgb((EGEGET_R(col)+EGEGET_R(c))/2,(EGEGET_G(col)+EGEGET_G(c))/2,(EGEGET_B(col)+EGEGET_B(c))/2);
					}
					int det=(i.type[1]=='+')*2-1,tmp;
					if(i.type[0]=='x')tmp=Light[Vec3i(i.bl.x+det,i.bl.y,i.bl.z)];
					if(i.type[0]=='y')tmp=Light[Vec3i(i.bl.x,i.bl.y+det,i.bl.z)];
					if(i.type[0]=='z')tmp=Light[Vec3i(i.bl.x,i.bl.y,i.bl.z+det)];
					col=rgb(EGEGET_R(col)-(10-tmp)*15,EGEGET_G(col)-(10-tmp)*15,EGEGET_B(col)-(10-tmp)*15);
					setcolor(col);setfillcolor(col);
					if(rule["Rander_Level"]==0)fillpoly(4,poly);
					else
					{
						int rad=max(d(pos[I-1][J-1],pos[I][J]),d(pos[I][J-1],pos[I-1][J]));
						rad=min(20,rad);
						if(R-L+D-U<10)fillellipse((L+R)/2,(U+D)/2,rad/2,rad/2);
						else fillpoly(4,poly);
					}
					
				}
		
		recnum++;
		if(rule["Show_Line"])
		{
			setcolor(BLACK);
			drawpoly(4,a);
		}
	}
	setcolor(WHITE);
	if(rule["Debug_Mode"])
	{
		//setfillcolor(GRAY);
		fillrect(0,0,300,100);
		outtextxy(0,0,"Fps:"+to_string(getfps()));
		outtextxy(0,20,"Time:"+to_string(DayTime/20.0/60.0));
		outtextxy(0,40,"Rect:"+to_string(recnum)+"/"+to_string(rects.size()));
		outtextxy(0,60,"Pos:"+to_string(player.x)+","+to_string(player.y)+","+to_string(player.z));
		outtextxy(0,80,"P/A:"+to_string(rule["Print_T"])+"/"+to_string(rule["All_T"]));
	}
	setlinewidth(2);
	line(imageWidth/2-10,imageHeight/2,imageWidth/2+10,imageHeight/2);
	line(imageWidth/2,imageHeight/2-10,imageWidth/2,imageHeight/2+10);
	for(int i=330;i<=790;i++)
		for(int j=390;j<=460;j++)
		{
			color_t col=getpixel(i,j);
			putpixel(i,j,rgb(EGEGET_R(col)-50,EGEGET_G(col)-50,EGEGET_B(col)-50));
		}
	
	if(bag[3][hand].type!=0)
	{
		int bo=getpixel(0, 0, texture[bag[3][hand].type])==EGERGB(0,255,0),bt=3;
		if(LD&&Use%5<=2)bt=-10;
		if(!LD)Use=0;
		else Use++;
		for(int i=0;i<256;i++)
			for(int j=0;j<256;j++)
			if(15-(j-i/bt)/16<=15)
			{
				color_t col=getpixel(15-(j-i/bt)/16,i/16,texture[bag[3][hand].type]);
				if(bo?col!=EGERGB(0,255,0):col!=EGERGB(255,255,255))putpixel(750+i,290+j+(bt==-10)*40,col);
			}
	}
	
	for(int i=0;i<9;i++)
	{
		outtextxy(imageWidth/2-9*25+i*50+50,400,to_string(i+1));
		if(bag[3][i].nj<=0)bag[3][i]={0,0,20};
		if(bag[3][i].type>0)putimg(imageWidth/2-9*25+i*50+50,420,texture[bag[3][i].type]);
		if(hand==i)
		{
			setlinewidth(2);setcolor(WHITE);
			rectangle(imageWidth/2-9*25+i*50+50,420,imageWidth/2-9*25+i*50+50+32,420+32);
			setlinewidth(1);
		}
		if(bag[3][i].nj==20)outtextxy(imageWidth/2-9*25+i*50+50+30,420+30,to_string(bag[3][i].num));
		else 
		{
			setcolor(bag[3][i].nj>10?GREEN:bag[3][i].nj>3?YELLOW:RED);
			setlinewidth(3);
			line(imageWidth/2-9*25+i*50+50,420+35,imageWidth/2-9*25+i*50+50+bag[3][i].nj*1.5,420+35);
			setlinewidth(1);
			setcolor(WHITE);
		}
	}
	if(health<=0)
	{
		dead();
		player=Vec3f(respawn.x*16,respawn.y*16,respawn.z*16);health=20;
		if(rule["keep_inventory"]!=1)for(int i=0;i<=5;i++)for(int j=0;j<9;j++)bag[i][j].num=0,bag[i][j].type=0;
	}
	if(ATKTIME>0)ATKTIME--;
	string tmp="0";
	if(ATKTIME>0&&ATKTIME%4<=2)tmp="1";
	for (int i = 1; i <= f45(health)/2; i++)putimg(470 + i * 20, 350, des[11],tmp);
	for (int i = f45(health)/ 2 + 1; i <= 10; i++)putimg(470 + i * 20, 350, des[10],tmp);
	if (f45(health) % 2 >=1)putimg(470 + (f45(health) + 1) / 2 * 20, 350, des[12],tmp);
}
void ms()
{
	ld=0,rd=0,md=0,wheel=0;
	RECT r1;
	GetWindowRect(getHWnd(),&r1);
	int winX=r1.left,winY=r1.top;
	while(mousemsg())
	{
		mouse_msg m=getmouse();
		if(m.is_move())
		{
			mx=m.x;my=m.y;
			POINT pt; 
			GetCursorPos(&pt);
			if(pt.x<=winX+20)SetCursorPos(winX+imageWidth-30,pt.y);
			else if(pt.x>=winX+imageWidth-20)SetCursorPos(winX+30,pt.y);
			else if(pt.y<winY+30)SetCursorPos(pt.x,winY+30);
			else if(pt.y>=winY+imageHeight-20)SetCursorPos(pt.x,winY+imageHeight-20);
			Rotate_x=m.y*3.1415/imageHeight-3.1415/2;
			Rotate_y=m.x*3.1415*2/imageWidth-3.1415;
		}
		if(m.is_wheel())wheel=m.wheel;
		if(m.is_down()&&m.is_left())ld=1,LD=1;
		if(m.is_down()&&m.is_right())rd=1;
		if(m.is_down()&&m.is_mid())md=1;
		if(m.is_up()&&m.is_left())LD=0;
	}
}
void bagg()
{
	showmouse(1);
	setcolor(WHITE);setlinewidth(1);
	ITEM choose={0,0};
	while(1)
	{
		ms();
		setcolor(GRAY_BRUSH);
		fillrect(imageWidth/2-9*25+40,50,imageWidth/2-9*25+10*50+20,480);setfillcolor(WHITE);
		for(int i=-4;i<4;i++)
			for(int j=0;j<9;j++)
			if(i>=0||(i<-2&&j<=-i-2))
			{
				int I=i;
				if(i<0)I+=10;
				int L=imageWidth/2-9*25+j*50+50;
				if(mx>=L&&mx<=L+32&&my>=50*i+270&&my<=50*i+302)
				{
					//setcolor(GRAY);
					if(ld)
					{
						if(choose.type&&i==6&&j==2)continue;
						if(I==6&&j==2)
						{
							bag[6][0].num--,bag[6][1].num--,bag[7][1].num--,bag[7][0].num--;
							if(bag[6][0].num<=0)bag[6][0]={0,0};
							if(bag[6][1].num<=0)bag[6][1]={0,0};
							if(bag[7][1].num<=0)bag[7][1]={0,0};
							if(bag[7][0].num<=0)bag[7][0]={0,0};
						}
						if(choose.type==bag[I][j].type)
						{
							if(I==6&&j==2)choose.num+=bag[I][j].num,bag[I][j]={0,0};
							else bag[I][j].num+=choose.num,choose={0,0};
						}
						else swap(choose,bag[I][j]);
					}
					if(rd&&choose.type==0)
					{
						choose=bag[I][j];
						choose.num/=2;
						bag[I][j].num-=choose.num;
					}
				}
				else setcolor(WHITE);
				fillrect(L-1,50*i+270-1,L+32+1,50*i+302+1);
				if(bag[I][j].type>0&&bag[I][j].num>0)putimg(L,50*i+270,texture[bag[I][j].type]);
				setcolor(WHITE);outtextxy(L+32,50*i+270+32,to_string(bag[I][j].num));
			}
		bag[6][2].type=0;bag[6][2].num=0;
		ifstream fin("craft1.txt");
		while(1)
		{
			string A,B,C,D,E;int F=0;
			fin>>A>>B>>C>>D>>E>>F;
			if(F<=0)break;
			if(!toint.count(A)||!toint.count(B)||!toint.count(C)||!toint.count(D))continue;
			if(toint[A]==bag[6][0].type&&toint[B]==bag[6][1].type&&toint[C]==bag[7][0].type&&toint[D]==bag[7][1].type)
			{
				bag[6][2].type=toint[E];bag[6][2].num=F;
			}
		}
		if(choose.type>0)putimg(mx,my,texture[choose.type]);
		while(kbmsg())
		{
			key_msg t=getkey();
			if(t.msg==key_msg_down&&t.key==key_E)
			{showmouse(0);
				for(int i=5;i<20;i++)for(int j=0;j<9;j++)bag[i][j].num=0,bag[i][j].type=0;
				return;
			}
		}
		delay_ms(50);
	}
	
}
void craf()
{
	showmouse(1);
	setcolor(WHITE);setlinewidth(1);
	ITEM choose={0,0};
	while(1)
	{
		ms();
		setcolor(GRAY_BRUSH);
		fillrect(imageWidth/2-9*25+40,50,imageWidth/2-9*25+10*50+20,480);setfillcolor(WHITE);
		for(int i=-4;i<4;i++)
			for(int j=0;j<9;j++)
				if(i>=0||i<=-2&&j<=(i==-4?3:2))
				{
					int I=i;
					if(i<0)I+=10;
					int L=imageWidth/2-9*25+j*50+50;
					if(mx>=L&&mx<=L+32&&my>=50*i+270&&my<=50*i+302)
					{
						setcolor(GRAY_BRUSH);
						if(ld)
						{
							if(choose.type&&i==6&&j==2)continue;
							if(I==6&&j==3)
							{
								for(int q=6;q<=8;q++)
									for(int w=0;w<=2;w++)
									{
										bag[q][w].num--;
										if(bag[q][w].num<=0)bag[q][w]={0,0};
									}
							}
							if(choose.type==bag[I][j].type)
							{
								if(I==6&&j==3)choose.num+=bag[I][j].num,bag[I][j]={0,0};
								else bag[I][j].num+=choose.num,choose={0,0};
							}
							else swap(choose,bag[I][j]);
						}
						if(rd&&choose.type==0)
						{
							choose=bag[I][j];
							choose.num/=2;
							bag[I][j].num-=choose.num;
						}
					}
					else setcolor(WHITE);
					fillrect(L-1,50*i+270-1,L+32+1,50*i+302+1);
					if(bag[I][j].type>0&&bag[I][j].num>0)putimg(L,50*i+270,texture[bag[I][j].type]);
					setcolor(WHITE);outtextxy(L+32,50*i+270+32,to_string(bag[I][j].num));
				}
		bag[6][3].type=0;bag[6][3].num=0;
		ifstream fin("craft2.txt");
		while(1)
		{
			string crt,TO;int F=0,ok=1;
			for(int i=0;i<3;i++)
				for(int j=0;j<3;j++)
				{
					fin>>crt;
					if(!toint.count(crt))ok=0;
					else if(toint[crt]!=bag[6+i][j].type)ok=0;
				}
			fin>>TO>>F;
			if(F<=0)break;
			if(!ok)continue;
			bag[6][3].type=toint[TO];bag[6][3].num=F;
		}
		if(choose.type>0)putimg(mx,my,texture[choose.type]);
		while(kbmsg())
		{
			key_msg t=getkey();
			if(t.msg==key_msg_down&&t.key==key_E)
			{
				showmouse(0);
				for(int i=5;i<20;i++)for(int j=0;j<9;j++)bag[i][j].num=0,bag[i][j].type=0;
				return;
			}
		}
		delay_ms(50);
	}
	
}
void furn()
{
	showmouse(1);
	setcolor(WHITE);setlinewidth(1);
	ITEM choose={0,0};int LASTT=0;
	while(1)
	{
		ms();
		setcolor(GRAY_BRUSH);
		fillrect(imageWidth/2-9*25+40,50,imageWidth/2-9*25+10*50+20,480);setfillcolor(WHITE);
		setfillcolor(WHITE);
		setcolor(WHITE);
		if(LASTT)fillrect(480,180,520-LASTT/5,190);
		for(int i=-4;i<4;i++)
			for(int j=0;j<9;j++)
				if(i>=0||i==-2&&(j==4||j==1||j==2))
				{
					int I=i;
					if(i<0)I+=10;
					int L=imageWidth/2-9*25+j*50+50;
					if(mx>=L&&mx<=L+32&&my>=50*i+270&&my<=50*i+302)
					{
						//setcolor(GRAY);
						if(ld)
						{
							if(choose.type&&i==6&&j==2)continue;
							if(choose.type==bag[I][j].type)
							{
								if(I==6&&j==3)choose.num+=bag[I][j].num,bag[I][j]={0,0};
								else bag[I][j].num+=choose.num,choose={0,0};
							}
							else swap(choose,bag[I][j]);
						}
						if(rd&&choose.type==0)
						{
							choose=bag[I][j];
							choose.num/=2;
							bag[I][j].num-=choose.num;
						}
					}
					else setcolor(WHITE);
					fillrect(L-1,50*i+270-1,L+32+1,50*i+302+1);
					if(bag[I][j].type>0&&bag[I][j].num>0)putimg(L,50*i+270,texture[bag[I][j].type]);
					setcolor(WHITE);outtextxy(L+32,50*i+270+32,to_string(bag[I][j].num));
				}
		if(bag[8][2].num>0&&(bag[8][2].type==toint["coal"]||bag[8][2].type==toint["log"]||bag[8][2].type==toint["planks"]))
		{
			ifstream fin("furnace.txt");
			while(1)
			{
				string sb,TO;int F=0,ok=1,rp=1;
				fin>>sb;F=toint[sb];
				fin>>TO;ok=toint[TO];
				if(ok<=0)break;
				if(bag[8][2].type==toint["coal"])rp=rand()%8==0;
				if(bag[8][2].type==toint["log"])rp=rand()%2==0;
				if(bag[8][1].type==F&&(bag[8][4].type==toint[TO]||bag[8][4].type==0))
				{
					if(LASTT==1)
					{
						bag[8][4].type=toint[TO];
						bag[8][1].num--;bag[8][2].num-=rp;
						if(bag[8][1].num<=0)bag[8][1].type=0,bag[8][1].num=0;
						if(bag[8][2].num<=0)bag[8][2].type=0,bag[8][2].num=0;
						bag[8][4].num++;
						LASTT=0;
					}
					else if(LASTT>1)LASTT--;
					else LASTT=200;
					break;
				}
				
			}
		}
		if(choose.type>0)putimg(mx,my,texture[choose.type]);
		while(kbmsg())
		{
			key_msg t=getkey();
			if(t.msg==key_msg_down&&t.key==key_E)
			{
				showmouse(0);
				for(int i=5;i<20;i++)for(int j=0;j<9;j++)bag[i][j].num=0,bag[i][j].type=0;
				return;
			}
		}
		delay_ms(50);
	}
	
}
void chk()
{
	Vec3f S(worldToplayer[0][2],0.0,worldToplayer[2][2]);
	Vec3f D(worldToplayer[0][0],0.0,worldToplayer[2][0]);
	Vec3f U(0,1,0);
	S.normalize();D.normalize();
	Vec3f det;
	if(key['W'])det=Vec3f(0,0,0)-S;
	if(key['S'])det=S;
	if(key['A'])det=Vec3f(0,0,0)-D;
	if(key['D'])det=D;
	det=det+playerMove;
	playerMove=playerMove*0.5;
	if(rule["creative"]==0)
	{
		for(int T=1;T<=10;T++)if(!inb(player+det*0.2)&&!inb(player+det*0.2-U*22))player=player+det*0.1;
		if(g==0&&inb(player)||inb(player-U*22))
		{
			for(int dx=-5;dx<=5;dx+=5)
				for(int dy=-5;dy<=5;dy+=5)
					if(!inb(player+Vec3f(dx,0,dy))&&!inb(player+Vec3f(dx,0,dy)-U*22))
					{
						player=player+Vec3f(dx,0,dy);
						break;	
					}
		}
		if(spd&&g==0&&inb(player-U*25))g=6;
		if(!inb(player-U*25))g--;
		while(inb(player-U*25)&&inb(player-U*24))player=player+U;
		if(g!=0)
		{
			int bo=-1;
			if(g<0)for(int i=0;i>=g;i--)if(inb(player+U*i-U*25)||inb(player+U*i)){bo=i;break;}
			if(g>0)for(int i=0;i<=g;i++)if(inb(player+U*i+U*5)){bo=i;break;}
			if(bo==-1)player=player+U*g;
			else 
			{
				if(g<-10)Atk(abs(g)/2.0);
				player=player+U*bo,g=0;
			}
		}
		if(shd&&!inb(player-U*26))player=player-U;
	}
	else
	{
		if(spd)player=player+U;
		if(shd)player=player-U;
		player=player+det;
	}
	if(key['X'])exit(0);
	if(key['E'])
	{
		bagg();
		key['E']=0;
	}
	for(int i=1;i<=9;i++)if(key[i+'0'])hand=i-1;
	if(!LD)digtime=0,dighash=0;
}
void put(int x,int y,int z)
{
	if(bag[3][hand].type==0)return;
	if(bag[3][hand].type==toint["water_bucket"])world[Dim][{x,y,z}]=toint["water"];
	else if(bag[3][hand].type==toint["lava_bucket"])world[Dim][{x,y,z}]=toint["lava"];
	else world[Dim][{x,y,z}]=bag[3][hand].type;
	if(bag[3][hand].type==toint["water_bucket"])bag[3][hand].type=toint["bucket"];
	if(bag[3][hand].type==toint["lava_bucket"])bag[3][hand].type=toint["bucket"];
	if(bag[3][hand].type!=toint["bucket"])bag[3][hand].num--;if(bag[3][hand].num==0)bag[3][hand].type=0;
	ofstream fout("sb.world",ios::app);
	fout<<x<<" "<<y<<" "<<z<<" "<<bag[3][hand].type<<endl;
	fout.close();
}
void give(int t)
{
	for(int i=3;i>=0;i--)
		for(int j=0;j<=9;j++)
		if(bag[i][j].num==0||(bag[i][j].type==t&&bag[i][j].num<64))
		{
			bag[i][j].num++;
			bag[i][j].type=t;
			return;
		}
}
void dest(int x,int y,int z)
{
	for(map<Vec3i,int>::iterator it=world[Dim].begin();it!=world[Dim].end();it++)
		if(it->first.x==x&&it->first.y==y&&it->first.z==z)
		{
			world[Dim].erase(it);
			break;
		}
	ofstream fout("sb.world",ios::app);
	fout<<x<<" "<<y<<" "<<z<<" "<<0<<endl;
	fout.close();
}
void broke(int x,int y,int z)
{
	if(world[Dim][{x,y,z}]==toint["lava"]||world[Dim][{x,y,z}]==toint["water_flow"]||world[Dim][{x,y,z}]==toint["water"])return;
	dighash=x*33+y*114514+z*2245+1919810;
	int notok=1,dig_level=0,bl=1;
	if(bag[3][hand].type==toint["wood_pickaxe"])dig_level=1,bl=2;
	if(bag[3][hand].type==toint["stone_pickaxe"])dig_level=2,bl=4;
	if(bag[3][hand].type==toint["iron_pickaxe"])dig_level=3,bl=6;
	if(bag[3][hand].type==toint["diamond_pickaxe"])dig_level=4,bl=8;
	if(lev[world[Dim][{x,y,z}]]>dig_level)notok=2,bl=1;
	if(rule["creative"])notok=1,bl=100;
	if(Time%notok==0)digtime+=bl;
	if(digtime>hard[world[Dim][{x,y,z}]])
	{
		if(bag[3][hand].type==toint["wood_pickaxe"]&&rand()%2==0)bag[3][hand].nj--;
		if(bag[3][hand].type==toint["stone_pickaxe"]&&rand()%4==0)bag[3][hand].nj--;
		if(bag[3][hand].type==toint["iron_pickaxe"]&&rand()%6==0)bag[3][hand].nj--;
		if(bag[3][hand].type==toint["diamond_pickaxe"]&&rand()%12==0)bag[3][hand].nj--;
		int GV=world[Dim][{x,y,z}];
		if(GV==toint["coal_ore"])GV=toint["coal"];
		if(GV==toint["diamond_ore"])GV=toint["diamond"];
		if(GV==toint["grass"])GV=toint["dirt"];
		if(GV==toint["leaf"])GV=rand()%3==0?toint["stick"]:rand()%2==0?toint["tree"]:0;
		if(GV&&notok==1)give(GV);
		dighash=0;digtime=0;
		Light_UPD=1;
		if(GV==toint["torch"])Light_UPD=2;
		dest(x,y,z);
	}
}
void atk(Vec3i tmp)
{
	for(Noob &i:noob)
		if(i.pos==tmp)
		{
			float PR=1;
			if(bag[3][hand].type==toint["stone_sword"]&&i.lasthurt<=Time-20)PR=5;
			if(bag[3][hand].type==toint["iron_sword"]&&i.lasthurt<=Time-20)PR=6;
			if(bag[3][hand].type==toint["diamond_sword"]&&i.lasthurt<=Time-20)PR=7;
			if(bag[3][hand].type==toint["stone_axe"]&&i.lasthurt<=Time-40)PR=8;
			if(bag[3][hand].type==toint["iron_axe"]&&i.lasthurt<=Time-35)PR=8;
			if(bag[3][hand].type==toint["diamond_axe"]&&i.lasthurt<=Time-30)PR=8;
			if(g!=0)PR*=1.5;
			i.lasthurt=Time;
			i.health-=PR;
			if(i.health<0)
			{
				if(i.type==toint["pig"])give(toint["beef_raw"]);
				if(i.type==toint["zombie"])give(toint["beef_raw"]);
				if(i.type==toint["sheep"])
				{
					for(int t=1;t<=rand()%2;t++)give(toint["beef_raw"]);
					for(int t=1;t<=rand()%2+1;t++)give(toint["wool"]);
				}
				if(i.type==toint["cow"])for(int t=1;t<=rand()%3+1;t++)give(toint["beef_raw"]);
			}
			i.hurttime=10;
		}
}
void pler()
{
	reverse(rects.begin(), rects.end());
	for(Rect i:rects)
	{
		if(i.dist()>50)continue;
		if(show[i.ind[0]]+show[i.ind[1]]+show[i.ind[2]]+show[i.ind[3]]<4)continue;
		int a[8];
		for(int j=0;j<4;j++)a[j*2]=campoint[i.ind[j]].x,a[j*2+1]=campoint[i.ind[j]].y;
		if(inpoly(imageWidth/2,imageHeight/2,a))
		{
			setcolor(BLACK);drawpoly(4,a);
			if(LD&&world[Dim].count(Vec3i(i.bl.x,i.bl.y,i.bl.z)))broke(i.bl.x,i.bl.y,i.bl.z);
			if(ld&&get_noob(i.bl)!=-1)atk(Vec3i(i.bl.x,i.bl.y,i.bl.z));
			if(rd&&world[Dim][i.bl]==toint["crafting_table"])craf();
			else if(rd&&world[Dim][i.bl]==toint["water"]&&bag[3][hand].type==toint["bucket"])dest(i.bl.x,i.bl.y,i.bl.z),bag[3][hand].type=toint["water_bucket"];
			else if(rd&&world[Dim][i.bl]==toint["lava"]&&bag[3][hand].type==toint["bucket"])dest(i.bl.x,i.bl.y,i.bl.z),bag[3][hand].type=toint["lava_bucket"];
			else if(rd&&world[Dim][i.bl]==toint["furnace"])furn();
			else if(rd&&world[Dim][i.bl]==toint["deb"]&&DayTime>=3000&&DayTime<=11000)bad(0),DayTime=17000,respawn=i.bl+Vec3i(0,2,0);
			else if(rd&&world[Dim][i.bl]==toint["bed"]&&DayTime>=18000&&DayTime<=23000)bad(1),DayTime=0,respawn=i.bl+Vec3i(0,2,0);
			else if(rd&&(bag[3][hand].type<=BLK||bag[3][hand].type==toint["lava_bucket"]||bag[3][hand].type==toint["water_bucket"]))
			{
				int det=(i.type[1]=='+')*2-1;
				if(i.type[0]=='x')put(i.bl.x+det,i.bl.y,i.bl.z);
				if(i.type[0]=='y')put(i.bl.x,i.bl.y+det,i.bl.z);
				if(i.type[0]=='z')put(i.bl.x,i.bl.y,i.bl.z+det);
			}
			return;
		}
	}
}
void getfile()
{
	system("dir block /b >list.txt");
	ifstream fin("list.txt");
	string str;
	while(getline(fin,str))
	{
		texture[++TOT]=newimage(16,16);toint[str.substr(0,str.find('.'))]=TOT;
		getimage(texture[TOT],("block\\"+str).c_str());
		hard[TOT]=45;
		lev[TOT]=0;
	}
	lev[toint["stone"]]=1;
	lev[toint["coal_ore"]]=1;
	lev[toint["iron_ore"]]=2;
	lev[toint["diamond_ore"]]=3;
	lev[toint["obsidian"]]=4;
	hard[toint["obsidian"]]=50*30;
	hard[toint["stone"]]=45;
	hard[toint["iron_ore"]]=90;
	hard[toint["diamond_ore"]]=90;
	hard[toint["coal_ore"]]=90;
	hard[toint["dirt"]]=30;
	hard[toint["grass"]]=30;
	hard[toint["leaf"]]=15;
	fin.close();
	system("dir item /b >list.txt");
	fin.open("list.txt");BLK=TOT;
	while(getline(fin,str))
	{
		texture[++TOT]=newimage(16,16);toint[str.substr(0,str.find('.'))]=TOT;
		getimage(texture[TOT],("item\\"+str).c_str());
	}
	fin.close();
	system("dir noob /b >list.txt");
	fin.open("list.txt");NOB=TOT;
	while(getline(fin,str))
	{
		texture[++TOT]=newimage();toint[str.substr(0,str.find('.'))]=TOT;
		getimage(texture[TOT],("noob\\"+str).c_str());
	}
	for(int i=0;i<13;i++)
	{
		des[i]=newimage();
		getimage(des[i],("destroy\\"+to_string(i)+".png").c_str());
	}
}
void initworld_main()
{
	srand(time(0));
	int DETX=rand(),DETY=rand();
	map<pair<int,int>,int>hi;
	int ws=rule["world_size"];
	for(int i=-ws;i<=ws;i++)
	for(int j=-ws;j<=ws;j++)hi[{i,j}]=64+(rand()%5?0:1);
	int k=1;
	while(inb(Vec3i(0,hi[{0,0}]+k,0)))k++;
	respawn=Vec3i(0,hi[{0,0}]+k,0);
	for(int o=1;o<=ws/5;o++)
	{
		int lakeX=rand()%(ws*2-2)-(ws-1),lakeY=rand()%(ws*2-2)-(ws-1);
		hi[{lakeX,lakeY}]=63;
		for(int i=-1;i<=1;i++)
			for(int j=-1;j<=1;j++)
			{
				if(rand()%2)
				{
					hi[{lakeX+i,lakeY+j}]=64;
					world[0][{lakeX+i,64,lakeY+j}]=toint["water"];
					if(rand()%2)hi[{lakeX+i,lakeY+j}]=63,world[0][{lakeX+i,64,lakeY+j}]=toint["water"];
				}
			}
	}
	for(int i=-ws;i<=ws;i++)
		for(int j=-ws;j<=ws;j++)
		{
			int T=hi[{i,j}],Tr=rand()%3+2;
			for(int k=T-Tr;k<T;k++)world[0][{i,k,j}]=toint["dirt"];
			for(int k=1;k<T-Tr;k++)world[0][{i,k,j}]=toint["stone"];
			world[0][{i,T,j}]=toint["grass"];
			world[0][{i,0,j}]=toint["bedrock"];
		}
	int T=2*(ws/5);
	while(T--)
	{
		int x=rand()%(ws*2)-ws,z=rand()%(ws*2)-ws,y=hi[{x,z}];
		if(y<=64)
		{
			T++;
			continue;
		}
		world[0][Vec3i(x,y,z)]=toint["log"];
		world[0][Vec3i(x,y+1,z)]=toint["log"];world[0][Vec3i(x,y+2,z)]=toint["log"];
		world[0][Vec3i(x,y+3,z)]=toint["log"];world[0][Vec3i(x,y+4,z)]=toint["leaf"];
		world[0][Vec3i(x,y+3,z+1)]=toint["leaf"];world[0][Vec3i(x+1,y+3,z)]=toint["leaf"];
		world[0][Vec3i(x,y+3,z-1)]=toint["leaf"];world[0][Vec3i(x-1,y+3,z)]=toint["leaf"];
	}
	T=8*(ws/5)*(ws/5)*rule["Mine_Number"];
	while(T-->0)
	{
		Vec3i tmp=Vec3i(rand()%(ws*2)-ws,rand()%60+1,rand()%(ws*2)-ws);
		world[0][tmp]=toint["iron_ore"];
		for(int x=tmp.x-1;x<=tmp.x+1;x++)
		for(int y=tmp.y-1;y<=tmp.y+1;y++)
		for(int z=tmp.z-1;z<=tmp.z+1;z++)
			if(rand()%5==0)world[0][Vec3i(x,y,z)]=toint["iron_ore"],T--;
	}
	T=2*(ws/5)*(ws/5)*rule["Mine_Number"];
	while(T-->0)
	{
		Vec3i tmp=Vec3i(rand()%(ws*2)-ws,rand()%32+1,rand()%(ws*2)-ws);
		world[0][tmp]=toint["diamond_ore"];
		for(int x=tmp.x-1;x<=tmp.x+1;x++)
			for(int y=tmp.y-1;y<=tmp.y+1;y++)
				for(int z=tmp.z-1;z<=tmp.z+1;z++)
					if(rand()%10==0)world[0][Vec3i(x,y,z)]=toint["diamond_ore"],T--;
	}
	T=16*(ws/5)*(ws/5)*rule["Mine_Number"];
	while(T-->0)
	{
		Vec3i tmp=Vec3i(rand()%(ws*2)-ws,rand()%25+32,rand()%(ws*2)-ws);
		world[0][tmp]=toint["coal_ore"];
		for(int x=tmp.x-1;x<=tmp.x+1;x++)
			for(int y=tmp.y-1;y<=tmp.y+1;y++)
				for(int z=tmp.z-1;z<=tmp.z+1;z++)
					if(rand()%3==0)world[0][Vec3i(x,y,z)]=toint["coal_ore"],T--;
	}
	for(int o=1;o<=ws/5;o++)
	{
		int x=rand()%(ws*2-4)-(ws-2),y=25,z=rand()%(ws*2-4)-(ws-2),len=rand()%10+10,flot=10;
		while(len--)
		{
			flot--;
			if(flot>0)y--;
			else
			{
				if(rand()%2==0)y--;
				else y++;
			}
			if(rand()%2==0)x--;
			else x++;
			if(rand()%2==0)z--;
			else z++;
			int tc=rand()%3;
			for(int i=x-rand()%3;i<=x+rand()%3;i++)
				for(int j=y-tc;j<=y+tc;j++)
					for(int k=z-rand()%3;k<=z+rand()%3;k++)
						if(abs(i)<=5&&abs(k)<=5)
						{
							if(j>20)dest(i,j,k);
							else world[0][{i,j,k}]=toint["lava"];
						}
		}
	}
	
}
void initworld_down()
{
	Dim=1;
	int DETX=rand(),DETY=rand();
	map<pair<int,int>,int>hi;
	int ws=rule["world_size"];
	/*for(int i=-ws;i<=ws;i++)
		for(int j=-ws;j<=ws;j++)hi[{i,j}]=noise((i+DETX)/10.0,(j+DETY)/10.0)*20+64;*/
	for(int i=-ws;i<=ws;i++)
		for(int j=-ws;j<=ws;j++)
		{
			int T=hi[{i,j}];
			for(int k=1;k<T;k++)world[Dim][{i,k,j}]=toint["netherrack"];
			world[Dim][{i,0,j}]=toint["bedrock"];
			for(int k=T;k<=63;k++)world[Dim][{i,k,j}]=toint["lava"];
		}
	int T=16*(ws/5)*(ws/5)*rule["Mine_Number"];
	while(T-->0)
	{
		Vec3i tmp=Vec3i(rand()%(ws*2)-ws,rand()%20+50,rand()%(ws*2)-ws);
		if(tmp.y<=hi[{tmp.x,tmp.z}])world[Dim][tmp]=toint["gold_ore"];
		for(int x=tmp.x-1;x<=tmp.x+1;x++)
			for(int y=tmp.y-1;y<=tmp.y+1;y++)
				for(int z=tmp.z-1;z<=tmp.z+1;z++)
					if(rand()%3==0&&y<=hi[{x,z}])world[Dim][Vec3i(x,y,z)]=toint["gold_ore"],T--;
	}
	
}
pair<Vec3i,int> getway(Vec3i s,Vec3i t)
{
	Vec3i Minp;int Min=1e9;
	if(s==t)return make_pair(Minp,1);
	for(int dx=-1;dx<=1;dx++)
		for(int dy=0;dy<=1;dy++)
			for(int dz=-1;dz<=1;dz++)
				if(abs(dx)+abs(dz)==1&&!inb(s+Vec3i(dx,dy,dz))&&get_noob(s+Vec3i(dx,dy,dz))==-1)
				{
					queue<Vec3i>q;
					map<Vec3i,int>vis;
					q.push(s+Vec3i(dx,dy,dz));vis[s+Vec3i(dx,dy,dz)]=1;
					while(!q.empty())
					{
						Vec3i tmp=q.front();
						q.pop();
						if(tmp==t)
						{
							if(Min>vis[tmp])Min=vis[tmp],Minp={dx,dy,dz};
							break;
						}
						for(int dx=-1;dx<=1;dx++)
							for(int dz=-1;dz<=1;dz++)
									if(abs(dx)+abs(dz)<=1)
									{
										int dy=world[Dim].count(tmp+Vec3i(0,-1,0))==0?-1:0;
										if(abs(tmp.x+dx)>5||abs(tmp.z+dz)>5||tmp.y+dy<0||tmp.y+dy>128)continue;
										if(world[Dim].count(tmp+Vec3i(dx,dy,dz))||get_noob(tmp+Vec3i(dx,dy,dz))!=-1)continue;
										if(vis.count(tmp+Vec3i(dx,dy,dz)))continue;
										vis[tmp+Vec3i(dx,dy,dz)]=vis[tmp]+1;
										q.push(tmp+Vec3i(dx,dy,dz));
									}
					}
				}
	if(Min>=5000)return make_pair(Minp,-1);
	else return make_pair(Minp,0);
}
void nbai()
{
	for(auto i=noob.begin();i!=noob.end();)
		if(i->health<=0)noob.erase(i);
		else i++;
	vector<Vec3i>Rnd;
	for(int x=-5;x<=5;x++)
		for(int y=0;y<=128;y++)
			for(int z=-5;z<=5;z++)
				if(!inb(Vec3i(x,y,z))&&inb(Vec3i(x,y-1,z)))Rnd.push_back(Vec3i(x,y,z));
	mt19937 sed(rand());
	shuffle(Rnd.begin(),Rnd.end(),sed);
	if(DayTime>=12700&&DayTime%400==0&&noob.size()<=5)
	{
		noob.push_back(mk_noob(Rnd.back(),rand()%3==0?toint["creeper"]:toint["zombie"]));
		Rnd.pop_back();
	}
	if((DayTime%800==0||DayTime==0)&&noob.size()<=2)
	{
		noob.push_back(mk_noob(Rnd.back(),rand()%3==0?toint["pig"]:rand()%3==0?toint["cow"]:toint["sheep"]));
		Rnd.pop_back();
	}
	Vec3i Pl=Vec3i(f45(player.x/16.0),f45(player.y/16.0),f45(player.z/16.0));
	for(Noob &i:noob)
	{
		if(i.hurttime>0)i.hurttime--;
		if(!inb(i.pos+Vec3i(0,-1,0)))
		{
			i.pos=i.pos+Vec3i(0,-1,0);
			continue;
		}
		if(i.AI==0)
		{
			if(i.lasthurt>Time-100)i.AI=1;
			if(Vec3f(player-Vec3f(i.pos.x*16,i.pos.y*16,i.pos.z*16)).lenth()<=3*16)i.AI=1;
			if(abs(Pl.x-i.pos.x)+abs(Pl.y-i.pos.y)+abs(Pl.z-i.pos.z)<i.mem["dis_pl"]-2)i.AI=1;
			continue;
		}
		if(i.type==toint["pig"]||i.type==toint["cow"]||i.type==toint["sheep"])
		{
			if(rand()%(i.lasthurt>Time-100?5:50)==0)
			{
				pair<Vec3i,int> t=getway(i.pos,i.to);
				if(t.second==1||t.second==-1)i.to=Rnd[rand()%Rnd.size()];
				else i.pos=i.pos+t.first;
			}
		}
		else if(i.type==toint["creeper"])
		{
			if(i.mem["boom_time"]>=30)
			{
				i.health=0;
				for(int dx=-3;dx<=3;dx++)
					for(int dy=-3;dy<=3;dy++)
						for(int dz=-3;dz<=3;dz++)
							if(abs(dx)+abs(dy)+abs(dz)<=3)dest(i.pos.x+dx,i.pos.y+dy,i.pos.z+dz);
				Vec3f tmp=player-Vec3f(i.pos.x*16,i.pos.y*16,i.pos.z*16);
				Atk((400.0-sqrt(tmp.lenth()))/40.0);
				tmp.normalize();
				playerMove=tmp*(20.0/sqrt(tmp.lenth()));
			}
			else
			{
				if(abs(i.pos.x-Pl.x)+abs(i.pos.z-Pl.z)<=3||i.mem["boom_time"]>=25)i.mem["boom_time"]++;
				else i.mem["boom_time"]=0;
				if(Time%15==0&&i.mem["boom_time"]<25)
				{
					int MIN=1000;
					for(auto T:Rnd)
						if(abs(T.x-Pl.x)+abs(T.y-Pl.y)+abs(T.z-Pl.z)<MIN)
							MIN=abs(T.x-Pl.x)+abs(T.y-Pl.y)+abs(T.z-Pl.z),i.to=T;
					pair<Vec3i,int> t=getway(i.pos,i.to);
					if(t.second==-1)i.to=Rnd[rand()%Rnd.size()];
					else i.pos=i.pos+t.first;
				}
			}
		}
		else if(i.type==toint["zombie"])
		{
			if(abs(i.pos.x-Pl.x)+abs(i.pos.z-Pl.z)==1&&abs(i.pos.y-Pl.y)<2)
			{
				if(Time%15==0)
				{
					Vec3f tmp=player-Vec3f(i.pos.x*16,i.pos.y*16,i.pos.z*16);
					tmp.normalize();
					Atk(3);
					playerMove=tmp*10;
				}
			}
			else if(Time%15==0)
			{
				for(auto T:Rnd)
					if(abs(T.x-Pl.x)+abs(T.z-Pl.z)==1&&abs(T.y-Pl.y)<2)i.to=T;
				pair<Vec3i,int> t=getway(i.pos,i.to);
				if(t.second==-1)i.to=Rnd[rand()%Rnd.size()];
				else i.pos=i.pos+t.first;
			}
		}
	}
}
void menu()
{
	rule["world_size"]=5;rule["Rander"]=1;
	rule["creative"]=0;
	rule["Rander_Level"]=3;
	rule["Light_Change"]=1;
	rule["random_tick"]=1;
	rule["Rander_Dist"]=10;
	rule["keep_inventory"]=0;
	rule["Mine_Number"]=4;
	setbkcolor(WHITE);setcolor(BLACK);
	while(1)
	{
		cleardevice();
		LOGFONTW ft;
		getfont(&ft);
		setfont(50,0,"宋体");
		outtextxy(0,0,"Mine Craft Beta by EndC");
		setfont(&ft);
		outtextxy(0,60,"点击左键生成世界");
		outtextxy(0,80,"生成设置(使用滚轮调节)");
		outtextxy(0,100,"大小(半径)"+to_string(rule["world_size"]*2));
		outtextxy(0,120,"Keep_Inventory:"+to_string(rule["keep_inventory"]));
		outtextxy(0,140,"矿物比例(4为正常值):"+to_string(rule["Mine_Number"]));
		outtextxy(0,160,"创造模式:"+to_string(rule["creative"]));
		if(wheel)
		{
			if(my<=120)
			{
				if(rule["world_size"]==5)rule["world_size"]=10;
				else if(rule["world_size"]==10)rule["world_size"]=20;
				else if(rule["world_size"]==20)rule["world_size"]=50;
				else if(rule["world_size"]==50)rule["world_size"]=5;
			}
			else if(my<=140)rule["keep_inventory"]=!rule["keep_inventory"];
			else if(my<=160)rule["Mine_Number"]+=wheel/100;
			else rule["creative"]=!rule["creative"];
		}
		ms();
		if(LD)
		{
			cleardevice();
			getfont(&ft);
			setfont(50,0,"宋体");
			outtextxy(0,0,"Mine Craft Beta by EndC");
			outtextxy(0,100,"正在生成世界");
			setfont(&ft);delay_ms(10);
			return;
		}
		delay_ms(10);
	}
}
map<Vec3i,Vec3i>Water_From;
void block()
{
	vector<Vec3i>WPR;
	for(map<Vec3i,int>::iterator it=world[Dim].begin();it!=world[Dim].end();it++)
		if(it->second==toint["water_flow"]&&(!Water_From.count(it->first)||!Water_From.count(Water_From[it->first])))world[Dim].erase(it--);
	for(map<Vec3i,int>::iterator it=world[Dim].begin();it!=world[Dim].end();it++)
		if(it->second==toint["nether"])world[Dim].erase(it--);
	for(auto it=Water_From.begin();it!=Water_From.end();it++)
		if(!world[Dim].count(it->first)||world[Dim][it->first]!=toint["water"])Water_From.erase(it--);
	for(auto it=Water_From.begin();it!=Water_From.end();it++)
		if(!Water_From.count(it->first))Water_From.erase(it--);
	for(auto &it:world[Dim])
		if(it.second==toint["water"])
		{
			Vec3i tmp=it.first;
			Water_From[tmp]=Vec3i(0,0,0);
			for(int dx=-1;dx<=1;dx++)
				for(int dy=-1;dy<=1;dy++)	
					for(int dz=-1;dz<=1;dz++)
						if(abs(dx)+abs(dy)+abs(dz)==1&&world[Dim].count(tmp+Vec3i(dx,dy,dz))&&world[Dim][tmp+Vec3i(dx,dy,dz)]==toint["lava"])world[Dim][tmp+Vec3i(dx,dy,dz)]=toint["obsidian"];
			for(int dx=-1;dx<=1;dx++)
				for(int dy=-1;dy<=0;dy++)	
					for(int dz=-1;dz<=1;dz++)
						if(abs(dx)+abs(dy)+abs(dz)==1&&!inb(tmp+Vec3i(dx,dy,dz)))
						{
							Water_From[tmp+Vec3i(dx,dy,dz)]=it.first;
							WPR.push_back(tmp+Vec3i(dx,dy,dz));
						}
		}
	for(auto &it:world[Dim])
		if(it.second==toint["obsidian"])
		{
			Vec3i tmp=it.first,chk[]=
			{Vec3i(-1,0,0),Vec3i(1,1,0),Vec3i(1,2,0),Vec3i(1,3,0),Vec3i(0,4,0),Vec3i(-1,4,0),Vec3i(-2,1,0),Vec3i(-2,2,0),Vec3i(-2,3,0)};
			Vec3i tc[]={Vec3i(0,1,0),Vec3i(0,2,0),Vec3i(0,3,0),Vec3i(-1,1,0),Vec3i(-1,2,0),Vec3i(-1,3,0)};
			for(int U=1;U<=2;U++)
			{
				if(U==2)
				{
					for(int i=0;i<9;i++)chk[i]=Vec3i(chk[i].z,chk[i].y,chk[i].x);
					for(int i=0;i<6;i++)tc[i]=Vec3i(tc[i].z,tc[i].y,tc[i].x);
				}
				int bo=1;
				for(int i=0;i<9;i++)
					if(!world[Dim].count(tmp+chk[i])||world[Dim][tmp+chk[i]]!=toint["obsidian"])bo=0;
				if(bo)
				{
					Vec3i Pl=Vec3i(f45(player.x/16.0),f45(player.y/16.0),f45(player.z/16.0));
					for(int i=0;i<6;i++)
						if(!(Pl==tmp+tc[i]||Pl==tmp+tc[i]+Vec3i(0,-1,0)))world[Dim][tmp+tc[i]]=toint["nether"];
						else bo=2;
				}
				if(bo==2)
				{
					Dim=(1-Dim);
					for(int i=0;i<9;i++)world[Dim][tmp+chk[i]]=toint["obsidian"];
				}
			}
		}
	for(auto &it:world[Dim])
		if(it.second==toint["water_flow"])
		{
			Vec3i tmp=it.first;
			for(int dx=-1;dx<=1;dx++)
				for(int dy=-1;dy<=1;dy++)	
					for(int dz=-1;dz<=1;dz++)
						if(abs(dx)+abs(dy)+abs(dz)==1&&world[Dim].count(tmp+Vec3i(dx,dy,dz))&&world[Dim][tmp+Vec3i(dx,dy,dz)]==toint["lava"])it.second=toint["stone"];
			if(it.second==toint["stone"])continue;
			for(int dx=-1;dx<=1;dx++)
				for(int dy=-1;dy<=0;dy++)	
					for(int dz=-1;dz<=1;dz++)
			if(abs(dx)+abs(dy)+abs(dz)==1&&!inb(tmp+Vec3i(dx,dy,dz))&&
			abs(tmp.x+dx-Water_From[tmp].x)+abs(tmp.z+dx-Water_From[tmp].z)<=2&&tmp.y+dy-Water_From[tmp].y>0)
							Water_From[tmp+Vec3i(dx,dy,dz)]=Water_From[tmp],WPR.push_back(tmp+Vec3i(dx,dy,dz));
		}
	for(Vec3i tmp:WPR)world[Dim].insert({tmp,toint["water_flow"]});
}
int main()
{
	initgraph(imageWidth,imageHeight);setrendermode(RENDER_MANUAL);
	menu();
	getfile();setbkmode(1);
	toint["empty"]=0;
	for(int i=0;i<20;i++)
		for(int j=0;j<9;j++)bag[i][j]={0,0};
	initworld_main(); 
	initworld_down();
	player=Vec3f(respawn.x*16,respawn.y*16,respawn.z*16);
	Dim=0;
	while(1)
	{
		ms();ky();showmouse(0);
		rule["All_S"]=getseconds();
		if(rule["config_mode"]==1)
		{
			showmouse(1);
			setfillcolor(WHITE);
			fillrect(50,50,950,450);
			setcolor(BLACK);
			int t=rule["Rander_Level"];
			string TS=t==3?"正常":t==2?"一般":t==1?"流畅":"最佳";
			outtextxy(400,70,"设置与调节 请用滚轮调节 ESC以退出");
			outtextxy(100,100,("画质调节:"+TS).c_str());
			outtextxy(100,150,("渲染距离:"+to_string(rule["Rander_Dist"])).c_str());
			outtextxy(100,200,("渲染优化(如果要透视方块，请关闭):"+to_string(rule["Rander"])).c_str());
			if(wheel)
			{
				if(my<=150)
				{
					rule["Rander_Level"]++;
					rule["Rander_Level"]%=4;
				}
				else  if(my<=200)
				{
					rule["Rander_Dist"]+=wheel/100;
					rule["Rander_Dist"]=max(5,rule["Rander_Dist"]);
				}
				else
				{
					rule["Rander"]=!rule["Rander"];
				}
			}
		}
		else
		{
			if(DayTime>=14000&&DayTime<=22000)setbkcolor(BLACK);
			else if(DayTime<=11000)setbkcolor(WHITE);
			else if(DayTime>=11000&&DayTime<=14000)setbkcolor(EGEGRAY(255-((DayTime-11000)*255/3000)));
			else if(DayTime>=22000)setbkcolor(EGEGRAY((DayTime-22000)*255/2000));
			DayTime%=(24000);
			updata();calc();
			rule["Print_S"]=getseconds();print();
			rule["Print_T"]=getseconds()-rule["Print_S"];
			if(Time%20==0)block();
			chk();
			moved=0;
			pler();
			nbai();
		}
		rule["All_T"]=getseconds()-rule["All_S"];
		delay_fps(20);
		Time++;DayTime+=rule["random_tick"];if(player.y<0&&Time%10==0)Atk(1);
	}
}
