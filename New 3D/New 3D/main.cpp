#include<bits/stdc++.h>
#include<graphics.h>
#include<windows.h>
using namespace std;
struct Vec2i
{
	int x, y;
	Vec2i() : x(0), y(0){}
	Vec2i(int xx, int yy) : x(xx), y(yy) {}
};
struct Vec2f
{
	float x, y;
	Vec2f() : x(0), y(0){}
	Vec2f(float xx, float yy) : x(xx), y(yy) {}
};
template<typename T>
class Vec3
{
public:
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	Vec3 operator + (Vec3 v) {return Vec3(x + v.x, y + v.y, z + v.z);}
	Vec3 operator - (Vec3 v){return Vec3(x - v.x, y - v.y, z - v.z);}
	Vec3 operator * (float r){return Vec3(x * r, y * r, z * r);}
	Vec3 operator * (Vec3 &v){return Vec3(x * v.x, y * v.y, z * v.z);}
	/*绕x旋转*/void rotate_point3d_x(float R) {T new_y = y * cos(R) - z * sin(R);T new_z = y * sin(R) + z * cos(R);y = new_y;z = new_z;}
	/*绕y旋转*/void rotate_point3d_y(float R) {T new_x = x * cos(R) + z * sin(R);T new_z = -x * sin(R) + z * cos(R);x = new_x;z = new_z;}
	/*点乘*/T dotProduct(Vec3<T> &v){return x * v.x + y * v.y + z * v.z;}
	Vec3& operator *= (T &r){x *= r, y *= r, z *= r;return *this;}
	/*叉乘*/Vec3 crossProduct(Vec3<T> &v){return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);}
	/*模长平方*/T norm(){return x * x + y * y + z * z;}
	/*模长*/T length(){return sqrt(norm());}
	/*下标访问*/const T& operator [] (uint8_t i) const{return (&x)[i];}
	T& operator [] (uint8_t i){return (&x)[i];}
	/*归一化*/Vec3& normalize(){T n = norm();if (n > 0){T factor = 1 / sqrt(n);x *= factor, y *= factor, z *= factor;}return *this;}
	friend Vec3 operator * (const T &r, const Vec3 &v){return Vec3<T>(v.x * r, v.y * r, v.z * r);}
	friend Vec3 operator / (const T &r, const Vec3 &v){return Vec3<T>(r / v.x, r / v.y, r / v.z);}
	friend std::ostream& operator << (std::ostream &s, const Vec3<T> &v){return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';}
	T x, y, z;
};
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
struct Cube
{
	Vec3i Center;
	Vec3f p[8];
	int flag[8],type;
	void Reset()
	{
		int cnt=0;
		for(int i=-8;i<=8;i+=16)
			for(int j=-8;j<=8;j+=16)
				for(int k=-8;k<=8;k+=16)	
					p[cnt].x=Center.x*16+i,p[cnt].y=Center.y*16+j,p[cnt].z=Center.z*16+k,cnt++;
	}
	void make_cube(int x,int y,int z,int t)
	{
		Center.x=x;Center.y=y;Center.z=z;type=t;
		Reset();
	}
};
map<Vec3i,bool>Is_Block;
struct Living
{
	float TowardsX=0,TowardsY=0;
	Vec3f pos,camera;
}p;
vector<Vec3f>Render_Point;
struct Rect
{
	int flag[4],type;
	int BelongCube,RectCnt;//RectCnt:面的方向
	float dist()
	{
		Vec3f tmp=(Render_Point[flag[0]]+Render_Point[flag[1]]+Render_Point[flag[2]]+Render_Point[flag[3]])*0.25f-p.camera;
		return tmp.length();
	}
};
bool operator<(Rect a,Rect b){return a.dist()<b.dist();}
bool operator<(Vec3i a,Vec3i b){return a.x!=b.x?a.x<b.x:a.y!=b.y?a.y<b.y:a.z<b.z;}
Rect make_rect(int cube,int cnt,int a,int b,int c,int d,int t)
{
	Rect x;x.flag[0]=a;x.flag[1]=b;x.flag[2]=c;x.flag[3]=d;x.type=t;x.BelongCube=cube;x.RectCnt=cnt;
	return x;
}
template<typename T>
class Matrix44
{
public:
	T x[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	Matrix44() {}
	Matrix44 (T a, T b, T c, T d, T e, T f, T g, T h,T i, T j, T k, T l, T m, T n, T o, T p){x[0][0] = a;x[0][1] = b;x[0][2] = c;x[0][3] = d;x[1][0] = e;x[1][1] = f;x[1][2] = g;x[1][3] = h;x[2][0] = i;x[2][1] = j;x[2][2] = k;x[2][3] = l;x[3][0] = m;x[3][1] = n;x[3][2] = o;x[3][3] = p;}
	const T* operator [] (uint8_t i) const{return x[i];}
	T* operator [] (uint8_t i){return x[i];}
	Matrix44 operator * (const Matrix44& v) const{Matrix44 tmp;multiply (*this, v, tmp);return tmp;}
	/*矩阵乘法*/static void multiply(const Matrix44<T> &a, const Matrix44& b, Matrix44 &c){const T * __restrict ap = &a.x[0][0];const T * __restrict bp = &b.x[0][0];T * __restrict cp = &c.x[0][0];T a0, a1, a2, a3;a0 = ap[0];a1 = ap[1];a2 = ap[2];a3 = ap[3];cp[0]  = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];cp[1]  = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];cp[2]  = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];cp[3]  = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];a0 = ap[4];a1 = ap[5];a2 = ap[6];a3 = ap[7];cp[4]  = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];cp[5]  = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];cp[6]  = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];cp[7]  = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];a0 = ap[8];a1 = ap[9];a2 = ap[10];a3 = ap[11];cp[8]  = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];cp[9]  = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];cp[10] = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];cp[11] = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];a0 = ap[12];a1 = ap[13];a2 = ap[14];a3 = ap[15];cp[12] = a0 * bp[0]  + a1 * bp[4]  + a2 * bp[8]  + a3 * bp[12];cp[13] = a0 * bp[1]  + a1 * bp[5]  + a2 * bp[9]  + a3 * bp[13];cp[14] = a0 * bp[2]  + a1 * bp[6]  + a2 * bp[10] + a3 * bp[14];cp[15] = a0 * bp[3]  + a1 * bp[7]  + a2 * bp[11] + a3 * bp[15];}
	/*转置矩阵*/Matrix44 transposed()const{return Matrix44(x[0][0],x[1][0],x[2][0],x[3][0],x[0][1],x[1][1],x[2][1],x[3][1],x[0][2],x[1][2],x[2][2],x[3][2],x[0][3],x[1][3],x[2][3],x[3][3]);}
	template<typename S>
	/*矩阵乘向量*/void multVecMatrix(const Vec3<S> &src, Vec3<S> &dst) const{S a, b, c, w;a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];dst.x = a / w;dst.y = b / w;dst.z = c / w;}
	template<typename S>
	void multDirMatrix(const Vec3<S> &src, Vec3<S> &dst) const{S a, b, c;a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];dst.x = a;dst.y = b;dst.z = c;}
	/*矩阵的逆*/Matrix44 inverse() const{int i, j, k;Matrix44 s;Matrix44 t (*this);for (i = 0; i < 3 ; i++){int pivot = i;T pivotsize = t[i][i];if (pivotsize < 0)pivotsize = -pivotsize;for (j = i + 1; j < 4; j++){T tmp = t[j][i];if (tmp < 0)tmp = -tmp;if (tmp > pivotsize){pivot = j;pivotsize = tmp;}}if (pivotsize == 0){return Matrix44();}if (pivot != i){for (j = 0; j < 4; j++){T tmp;tmp = t[i][j];t[i][j] = t[pivot][j];t[pivot][j] = tmp;tmp = s[i][j];s[i][j] = s[pivot][j];s[pivot][j] = tmp;}}for (j = i + 1; j < 4; j++){T f = t[j][i] / t[i][i];for (k = 0; k < 4; k++){t[j][k] -= f * t[i][k];s[j][k] -= f * s[i][k];}}}for (i = 3; i >= 0; --i){T f;if ((f = t[i][i]) == 0){return Matrix44();}for (j = 0; j < 4; j++){t[i][j] /= f;s[i][j] /= f;}for (j = 0; j < i; j++){f = t[j][i];for (k = 0; k < 4; k++){t[j][k] -= f * t[i][k];s[j][k] -= f * s[i][k];}}}return s;}
};
typedef Matrix44<float> Matrix44f;
priority_queue<Rect>Render_Rect;
vector<Cube>Render_Cube;
//vector<Vec3f>Render_Point;
Matrix44f worldToCamera;
vector<Vec2i>Screen_Point;
vector<float>Point_Z;
float imageHeight=1000,imageWidth=2000,canvasWidth=1,canvasHeight=0.5;
bool LD=0,RD=0,ld=0,rd=0,md=0,key[512];
void Solve_Camera()
{
	Vec3f X(2,0,0),Y(0,2,0),Z(0,0,2);
	X.rotate_point3d_y(p.TowardsY);X.rotate_point3d_x(p.TowardsX);
	Y.rotate_point3d_y(p.TowardsY);Y.rotate_point3d_x(p.TowardsX);
	Z.rotate_point3d_y(p.TowardsY);Z.rotate_point3d_x(p.TowardsX);
	Matrix44f cameraToWorld=Matrix44f(X.x, Y.x, Z.x, 0, X.y, Y.y, Z.y, 0, X.z, Y.z, Z.z, 0, p.camera.x, p.camera.y, p.camera.z, 1);
	worldToCamera = cameraToWorld.inverse();
}
void Get_Rander_Point()
{
	Render_Point.clear();
	int cnt=0;
	for(Cube &i:Render_Cube)
	{
		for(int j=0;j<8;j++)
		{
			Render_Point.push_back(i.p[j]);
			i.flag[j]=cnt++;
		}
	}
}
void Get_Rander_Rect()
{
	while(!Render_Rect.empty())Render_Rect.pop();
	int I=0;
	for(Cube i:Render_Cube)
	{
		if(!Is_Block.count(i.Center+Vec3i(-1,0,0))&&p.camera.x<i.Center.x*16)
			Render_Rect.push(make_rect(I,0,i.flag[0],i.flag[1],i.flag[3],i.flag[2],i.type));
		if(!Is_Block.count(i.Center+Vec3i(1,0,0))&&p.camera.x>i.Center.x*16)
			Render_Rect.push(make_rect(I,1,i.flag[4],i.flag[5],i.flag[7],i.flag[6],i.type));
		if(!Is_Block.count(i.Center+Vec3i(0,-1,0))&&p.camera.y<i.Center.y*16)
			Render_Rect.push(make_rect(I,2,i.flag[1],i.flag[5],i.flag[4],i.flag[0],i.type));
		if(!Is_Block.count(i.Center+Vec3i(0,1,0))&&p.camera.y>i.Center.y*16)
			Render_Rect.push(make_rect(I,3,i.flag[2],i.flag[3],i.flag[7],i.flag[6],i.type));
		if(!Is_Block.count(i.Center+Vec3i(0,0,-1))&&p.camera.z<i.Center.z*16)
			Render_Rect.push(make_rect(I,4,i.flag[0],i.flag[2],i.flag[6],i.flag[4],i.type));
		if(!Is_Block.count(i.Center+Vec3i(0,0,1))&&p.camera.z>i.Center.z*16)
			Render_Rect.push(make_rect(I,5,i.flag[1],i.flag[3],i.flag[7],i.flag[5],i.type));
		I++;
	}
}
int mx,my;Rect MouseToward;bool Is_Toward;int Type_Choosed;
PIMAGE img[10];
void Calc()
{
	Screen_Point.clear();
	Point_Z.clear();
	for(int i=0;i<Render_Point.size();i++)
	{
		Vec3f pCamera;
		worldToCamera.multVecMatrix(Render_Point[i],pCamera);
		//if(pCamera.z>0)cout<<"Out\n";
		Vec2f pScreen;
		pScreen.x = pCamera.x / -pCamera.z;
		pScreen.y = pCamera.y / -pCamera.z;
		Vec2f pNDC;
		pNDC.x = (pScreen.x + canvasWidth * 0.5) / canvasWidth;
		pNDC.y = (pScreen.y + canvasHeight * 0.5) / canvasHeight;
		Vec2i pRaster;
		pRaster.x = (int)(pNDC.x * imageWidth);
		pRaster.y = (int)((1 - pNDC.y) * imageHeight);
		Screen_Point.push_back(pRaster);
		//cout<<p.x<<","<<p.y<<endl;
		Point_Z.push_back(pCamera.z);
	}
}
color_t texture(int t,int x,int y)
{
	if(x<0||y<0||x>=16||y>=16)return BLACK;
	if(t==0)return (x+y)%2==0?BROWN:WHITE;
	else
	{
		color_t* tmp=getbuffer(img[t]);
		return tmp[x*16+y];
	}
}
bool OutOfWindow(Vec2i a){return a.x<0||a.y<0||a.x>imageWidth||a.y>imageHeight;}
void Print_Tri(Vec2i a,Vec2i b,Vec2i c,Vec2i A,Vec2i B,Vec2i C,float za,float zb,float zc,int type)
//A,B,C表示纹理点坐标,a,b,c表示屏幕坐标,z用于矫正透视,type是纹理编号
{
	if (b.y < a.y) swap(a, b),swap(A, B),swap(za,zb);
	if (c.y < a.y) swap(a, c),swap(C, A),swap(zc,za);
	if (c.y < b.y) swap(b, c),swap(C, B),swap(zc,zb);
	if(a.y==b.y)a.y--;if(b.y==c.y)c.y++;
	float S = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	for(int y=max(0,a.y);y<=min(b.y,(int)imageHeight);y++)
		if((b.y-a.y)!=0&&(b.y-c.y)!=0&&(c.y-a.y)!=0)
		{
			int l=(y-a.y)*b.x/(b.y-a.y)+(b.y-y)*a.x/(b.y-a.y);
			int r=(y-a.y)*c.x/(c.y-a.y)+(c.y-y)*a.x/(c.y-a.y);
			if(r<l)swap(l,r);
			l=max(l,0);
			r=min(r,(int)imageWidth);
			for(int x=l;x<=r;x++)
				if(r!=l)
				{
					float alpha = ((b.y - c.y) * (x - c.x) + (c.x - b.x) * (y - c.y)) / S;
					float beta = ((c.y - a.y) * (x - a.x) + (a.x - c.x) * (y - a.y)) / S;
					float gamma = 1.0f - alpha - beta;
					float Z=alpha/za+beta/zb+gamma/zc;Z=1/Z;
					Vec2f Real(alpha*A.x/za+beta*B.x/zb+gamma*C.x/zc,alpha*A.y/za+beta*B.y/zb+gamma*C.y/zc);
					Real.x=Z*Real.x;Real.y=Z*Real.y;
					putpixel(x,y,texture(type,Real.x,Real.y));
				}
		}
	
	for(int y=max(0,b.y);y<=min(c.y,(int)imageHeight);y++)
		if((b.y-a.y)!=0&&(b.y-c.y)!=0&&(c.y-a.y)!=0)
		{
			int l=(y-b.y)*c.x/(c.y-b.y)+(c.y-y)*b.x/(c.y-b.y);
			int r=(y-a.y)*c.x/(c.y-a.y)+(c.y-y)*a.x/(c.y-a.y);
			l=max(l,0);
			r=min(r,(int)imageWidth);
			if(r<l)swap(l,r);
			for(int x=l;x<=r;x++)
				if(r!=l)
				{
					float alpha = ((b.y - c.y) * (x - c.x) + (c.x - b.x) * (y - c.y)) / S;
					float beta = ((c.y - a.y) * (x - a.x) + (a.x - c.x) * (y - a.y)) / S;
					float gamma = 1.0f - alpha - beta;
					float Z=alpha/za+beta/zb+gamma/zc;Z=1/Z;
					Vec2f Real(alpha*A.x/za+beta*B.x/zb+gamma*C.x/zc,alpha*A.y/za+beta*B.y/zb+gamma*C.y/zc);
					Real.x=Z*Real.x;Real.y=Z*Real.y;
					putpixel(x,y,texture(type,Real.x,Real.y));
				}
		}
}
bool inpoly(int x, int y, Vec2i A,Vec2i B,Vec2i C,Vec2i D)
{
	int count = 0,a[8]={A.x,A.y,B.x,B.y,C.x,C.y,D.x,D.y};
	for (int i = 0; i < 8; i += 2)
	{
		int x1 = a[i], y1 = a[i + 1], x2 = a[(i + 2) % 8], y2 = a[(i + 3) % 8];
		if ((y1 < y && y2 >= y) || (y1 >= y && y2 < y))
		{
			int cross_x = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
			if (cross_x < x)count++;
		}
	}
	return count % 2 != 0;
}
void Print()
{
	Vec2i A=Vec2i(0,0),B=Vec2i(16,0),C=Vec2i(16,16),D=Vec2i(0,16);
	Is_Toward=0;
	while(!Render_Rect.empty())
	{
		Rect i=Render_Rect.top();Render_Rect.pop();
		Vec2i a,b,c,d;float az,bz,cz,dz;
		a=Screen_Point[i.flag[0]];b=Screen_Point[i.flag[1]];
		c=Screen_Point[i.flag[2]];d=Screen_Point[i.flag[3]];
		az=Point_Z[i.flag[0]];bz=Point_Z[i.flag[1]];
		cz=Point_Z[i.flag[2]];dz=Point_Z[i.flag[3]];
		if(az>0&&bz>0&&cz>0&&dz>0)continue;
		if(OutOfWindow(a)&&OutOfWindow(b)&&OutOfWindow(c)&&OutOfWindow(d))continue;
		if(inpoly(mx,my,a,b,c,d))MouseToward=i,Is_Toward=1;
		Print_Tri(d,a,b,D,A,B,dz,az,bz,i.type);
		Print_Tri(c,b,d,C,B,D,cz,bz,dz,i.type);
	}
}
void Print_Sky()
{
	for(int i=0;i<imageHeight;i++)
	{
		int col=255*(1-i/imageHeight-sin(p.TowardsX));
		col=max(0,col);col=min(255,col);
		setcolor(EGEGRAY(col));
		line(0,i,imageWidth,i);
	}
}
void Add_Cube(int x,int y,int z,int type)
{
	Cube _;_.make_cube(x,y,z,type);
	Render_Cube.push_back(_);
	Is_Block[Vec3i(x,y,z)]=1;
}
void updata()
{
	ld=0,rd=0,md=0;
	while(mousemsg())
	{
		mouse_msg m=getmouse();
		if(m.is_move())
		{
			p.TowardsX=m.y*PI/imageHeight-PI/2;
			p.TowardsY=m.x*PI/imageWidth+PI/2;
			mx=m.x;my=m.y;
		}
		if(m.is_down()&&m.is_left())LD=1,ld=1;
		if(m.is_down()&&m.is_mid())md=1;
		if(m.is_down()&&m.is_right())RD=1,rd=1;
		if(m.is_up())LD=0,RD=0;
	}
	while(kbmsg())
	{
		key_msg t=getkey();
		if(t.msg==key_msg_down)key[t.key]=1;
		if(t.msg==key_msg_up)key[t.key]=0;
	}
	Vec3f FORWARD(worldToCamera[0][2],0.0,worldToCamera[2][2]);
	Vec3f LEFT(worldToCamera[0][0],0.0,worldToCamera[2][0]);
	if(key['W'])p.camera=p.camera-FORWARD*2;
	if(key['S'])p.camera=p.camera+FORWARD*2;
	if(key['A'])p.camera=p.camera-LEFT;
	if(key['D'])p.camera=p.camera+LEFT;
	if(key[key_space])p.camera=p.camera+Vec3f(0,1,0);
	if(key[key_shift])p.camera=p.camera-Vec3f(0,1,0);
}
void SetBlock()
{
	if(!Is_Toward)return;
	Rect i=MouseToward;int cnt=i.RectCnt;
	Vec2i a,b,c,d;
	a=Screen_Point[i.flag[0]];b=Screen_Point[i.flag[1]];
	c=Screen_Point[i.flag[2]];d=Screen_Point[i.flag[3]];
	setcolor(WHITE);
	line(a.x,a.y,b.x,b.y);line(c.x,c.y,b.x,b.y);line(c.x,c.y,d.x,d.y);line(a.x,a.y,d.x,d.y);
	if(ld)
	{
		Is_Block.erase(Render_Cube[i.BelongCube].Center);
		Render_Cube.erase(Render_Cube.begin()+i.BelongCube);
		Get_Rander_Point();
	}
	else if(rd)
	{
		int x=cnt==0?-1:cnt==1?1:0;
		int y=cnt==2?-1:cnt==3?1:0;
		int z=cnt==4?-1:cnt==5?1:0;
		Vec3i tmp=Render_Cube[i.BelongCube].Center;
		Add_Cube(tmp.x+x,tmp.y+y,tmp.z+z,Type_Choosed);
		Get_Rander_Point();
	}
	else if(md)Type_Choosed=Render_Cube[i.BelongCube].type;
}
int main()
{
	initgraph(imageWidth,imageHeight);setrendermode(RENDER_MANUAL);
	for(int i=0;i<=50;i++)
		for(int j=0;j<=50;j++)
		{
			Add_Cube(i,0,j,1);
			if(rand()%50==0)
			{
				Add_Cube(i,1,j,2);Add_Cube(i,2,j,2);Add_Cube(i,3,j,2);
				Add_Cube(i,4,j,3);Add_Cube(i-1,3,j,3);Add_Cube(i+1,3,j,3);Add_Cube(i,3,j-1,3);Add_Cube(i,3,j+1,3);
			}
		}
	p.camera=Vec3f(0,100,0);
	Get_Rander_Point();
	img[1]=newimage();getimage(img[1],"texture1.png");
	img[2]=newimage();getimage(img[2],"texture2.png");
	img[3]=newimage();getimage(img[3],"texture3.png");
	while(1)
	{
		Solve_Camera();
		Calc();
		Get_Rander_Rect();
		Print_Sky();
		Print();
		setcolor(WHITE);
		outtextxy(0,0,("Fps:"+to_string(getfps())+";Blocks:"+to_string(Render_Cube.size())+";Rects:"+to_string(Render_Rect.size())).c_str());
		SetBlock();
		delay_fps(40);
		cleardevice();
		updata();
	}
}
