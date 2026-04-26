#include<bits/stdc++.h>
using namespace std;
struct Vec2i
{
	Vec2i() : x(0), y(0) {}
	Vec2i(int X, int Y): x(X), y(Y) {};
	Vec2i operator - (Vec2i b)
	{
		return Vec2i(x - b.x, y - b.y);
	}
	int x, y;
};
struct Vec3i
{
	int x, y, z;
	Vec3i(int X, int Y, int Z): x(X), y(Y), z(Z) {};
	Vec3i() : x(0), y(0), z(0) {};
	bool operator<(const Vec3i b) const
	{
		return x == b.x ? (y == b.y ? z < b.z : y < b.y) : x < b.x;
	}
	Vec3i operator * (int r)
	{
		return Vec3i(x * r, y * r, z * r);
	}
	Vec3i operator + (Vec3i v)
	{
		return Vec3i(x + v.x, y + v.y, z + v.z);
	}
	bool operator==(const Vec3i b) const
	{
		return x == b.x && y == b.y && z == b.z;
	}
};
Vec2i findIntersection(Vec2i a, Vec2i b, Vec2i c, Vec2i d)
{
	float x1 = a.x, y1 = a.y;
	float x2 = b.x, y2 = b.y;
	float x3 = c.x, y3 = c.y;
	float x4 = d.x, y4 = d.y;
	float x, y;
	float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (den == 0)return Vec2i(0.25 * (x1 + x2 + x3 + x4), 0.25 * (y1 + y2 + y3 + y4));
	x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / den;
	y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / den;
	return Vec2i(x, y);
}
struct Vec3f
{
	Vec3f() : x(0), y(0), z(0) {}
	Vec3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vec3f operator + (Vec3f v)
	{
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}
	Vec3f operator - (Vec3f v)
	{
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}
	Vec3f operator * (float r)
	{
		return Vec3f(x * r, y * r, z * r);
	}
	float lenth()
	{
		return sqrt(x * x + y * y + z * z);
	}
	bool operator<(const Vec3f b) const
	{
		return x == b.x ? (y == b.y ? z < b.z : y < b.y) : x < b.x;
	}
	void rotate_x(float R)
	{
		float new_y = y * cos(R) - z * sin(R);
		float new_z = y * sin(R) + z * cos(R);
		y = new_y;
		z = new_z;
	}
	void rotate_y(float R)
	{
		float new_x = x * cos(R) + z * sin(R);
		float new_z = -x * sin(R) + z * cos(R);
		x = new_x;
		z = new_z;
	}
	const float& operator [] (uint8_t i) const
	{
		return (&x)[i];
	}
	Vec3f& normalize()
	{
		float factor = 1 / lenth() ;
		x *= factor, y *= factor, z *= factor;
		return *this;
	}
	float x, y, z;
};
struct Matrix44
{
	float x[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	Matrix44() {}
	int rad = rand() % 2;
	Matrix44 (float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
	{
		x[0][0] = a;
		x[0][1] = b;
		x[0][2] = c;
		x[0][3] = d;
		x[1][0] = e;
		x[1][1] = f;
		x[1][2] = g;
		x[1][3] = h;
		x[2][0] = i;
		x[2][1] = j;
		x[2][2] = k;
		x[2][3] = l;
		x[3][0] = m;
		x[3][1] = n;
		x[3][2] = o;
		x[3][3] = p;
	}
	const float* operator [] (uint8_t i) const
	{
		return x[i];
	}
	float* operator [] (uint8_t i)
	{
		return x[i];
	}
	void multVecMatrix(const Vec3f &src, Vec3f &dst) const
	{
		float a, b, c, w;
		a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0] + x[3][0];
		b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1] + x[3][1];
		c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2] + x[3][2];
		w = src[0] * x[0][3] + src[1] * x[1][3] + src[2] * x[2][3] + x[3][3];
		dst.x = a / w;
		dst.y = b / w;
		dst.z = c / w;
	}
	void multDirMatrix(const Vec3f &src, Vec3f &dst) const
	{
		float a, b, c;
		a = src[0] * x[0][0] + src[1] * x[1][0] + src[2] * x[2][0];
		b = src[0] * x[0][1] + src[1] * x[1][1] + src[2] * x[2][1];
		c = src[0] * x[0][2] + src[1] * x[1][2] + src[2] * x[2][2];
		dst.x = a;
		dst.y = b;
		dst.z = c;
	}
	Matrix44 inverse()
	{
		int i, j, k;
		Matrix44 s;
		Matrix44 t (*this);
		for (i = 0; i < 3 ; i++)
		{
			int pivot = i;
			float pivotsize = t[i][i];
			if (pivotsize < 0)pivotsize = -pivotsize;
			for (j = i + 1; j < 4; j++)if (abs(t[j][i]) > pivotsize)pivot = j, pivotsize = abs(t[j][i]);
			if (pivotsize == 0)return Matrix44();
			if (pivot != i)for (j = 0; j < 4; j++)swap(t[i][j], t[pivot][j]), swap(s[i][j], s[pivot][j]);
			for (j = i + 1; j < 4; j++)
			{
				float f = t[j][i] / t[i][i];
				for (k = 0; k < 4; k++)t[j][k] -= f * t[i][k], s[j][k] -= f * s[i][k];
			}
		}
		for (i = 3; i >= 0; --i)
		{
			float f;
			if ((f = t[i][i]) == 0)return Matrix44();
			for (j = 0; j < 4; j++)t[i][j] /= f, s[i][j] /= f;
			for (j = 0; j < i; j++)
			{
				f = t[j][i];
				for (k = 0; k < 4; k++)t[j][k] -= f * t[i][k], s[j][k] -= f * s[i][k];
			}
		}
		return s;
	}
};
bool inpoly(int x, int y, int a[])
{
	int count = 0;
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
template <typename T>
void Sort(std::vector<T>& arr, int low, int high)
{
	if (low < high)
	{
		int pivotIndex = partition(arr, low, high);
		Sort(arr, low, pivotIndex - 1);
		Sort(arr, pivotIndex + 1, high);
	}
}
template <typename T>
int partition(std::vector<T>& arr, int low, int high)
{
	T pivot = arr[high];
	int i = low - 1;
	for (int j = low; j <= high - 1; j++)
	{
		if (arr[j] < pivot)
		{
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return i + 1;
}
#include <chrono>
long long getseconds()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	return nanos.time_since_epoch().count();
}
int f45 (double n)
{
	if (n > 0)return n - int(n) >= 0.5 ? int(n) + 1 : int(n);
	else return -n - int(-n) >= 0.5 ? -(int(-n) + 1) : -int(-n);
}
