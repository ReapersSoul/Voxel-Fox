#pragma once


// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
#include <GL/GLU.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <string>
#include <thread>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <Windows.h>

//kinect
#include <NuiApi.h>
//lua 
#include "Lua.h"

//DevIL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

namespace VoxelFox {
	wchar_t* GetWC(std::string c)
	{
		size_t newsize = strlen(c.c_str()) + 1;
		wchar_t* wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, c.c_str(), _TRUNCATE);

		return wcstring;
	}

	namespace Math {
		template <typename T>
		T MapRange(T input, T input_start, T input_end, T output_start, T output_end) {
			return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
		}

		float DegreeToRadians(float v) {
			return v * M_PI / 180;
		}

		template<typename T>
		struct Vec2 : public Lua::ClassTable{

			T x, y;

			void FromTable(Lua::Table t) override {
				x = t.find("x")->second.number;
				y = t.find("y")->second.number;
			}
			Lua::Table ToTable() override {
				Lua::LuaTableBuilder ret;
				ret.AddNumber("x", x);
				ret.AddNumber("y", y);
				return ret.GetTable();
			}

			Vec2(T x, T y) {
				this->x = x;
				this->y = y;
			}
			Vec2(T x) {
				this->x = x;
				this->y = x;
			}
			Vec2() {
				x = 0;
				y = 0;
			}

			void Rotate(float angle) {
				float sin_t = sin(angle);
				float cos_t = cos(angle);
				Math::Vec2<float> old = *this;

				x = (old.x * cos_t) - (old.y * sin_t);
				y = (old.x * sin_t) + (old.y * cos_t);
			}

			Vec2<T> operator+ (Vec2<T> v) {
				Vec2<T> ret;
				ret.x = this->x + v.x;
				ret.y = this->y + v.y;
				return ret;
			}
			Vec2<T> operator- (Vec2<T> v) {
				Vec2<T> ret;
				ret.x = this->x - v.x;
				ret.y = this->y - v.y;
				return ret;
			}
			Vec2<T> operator* (Vec2<T> v) {
				Vec2<T> ret;
				ret.x = this->x * v.x;
				ret.y = this->y * v.y;
				return ret;
			}
			Vec2<T> operator/ (Vec2<T> v) {
				Vec2<T> ret;
				ret.x = this->x / v.x;
				ret.y = this->y / v.y;
				return ret;
			}
			
			template<typename l>
			Vec2<l> reType() {
				Vec2<l> ret;
				ret.x = this->x;
				ret.y = this->y;
				return ret;
			}
		};
		template<typename T>
		struct Vec3 : public Lua::ClassTable {

			void FromTable(Lua::Table t) override {
				x = t.find("x")->second.number;
				y = t.find("y")->second.number;
				z = t.find("z")->second.number;
			}
			Lua::Table ToTable() override {
				Lua::LuaTableBuilder ret;
				ret.AddNumber("x", x);
				ret.AddNumber("y", y);
				ret.AddNumber("z", z);
				return ret.GetTable();
			}

			void Print() {
				std::cout << "Vec3: x:" << x << " y:" << y << " z:" << z << std::endl;
			}

			Vec3(T x, T y, T z) {
				this->x = x;
				this->y = y;
				this->z = z;
			}
			Vec3(T x) {
				this->x = x;
				this->y = x;
				this->z = x;
			}
			Vec3() {
				x = 0;
				y = 0;
				z = 0;
			}

			T x, y, z;

			Vec3<T> Normalize() {
				Vec3<T> ret=*this;
				T tmp = x + y + z;

				ret.x = ret.x / tmp;
				ret.y = ret.y / tmp;
				ret.z = ret.z / tmp;

				return ret;
			}
			Vec3<T> Cross(Vec3<T> v) {
				Vec3<T> ret;

				ret.x = (this->y*v.z)-(v.y*this->z);
				ret.y = (this->z*v.x)-(v.z*this->x);
				ret.z = (this->x*v.y)-(v.x*this->y);

				return ret;
			}

			void RotateZAxis(float angle) {
				float sin_t = sin(angle);
				float cos_t = cos(angle);
				Math::Vec3<float> old = *this;

				x = (old.x * cos_t) - (old.y * sin_t);
				y = (old.x * sin_t) + (old.y * cos_t);
			}
			void RotateYAxis(float angle) {
				float sin_t = sin(angle);
				float cos_t = cos(angle);
				Math::Vec3<float> old = *this;
				x = (cos_t * old.x) + (sin_t * old.z);
				z = (-sin_t * old.x) + (cos_t * old.z);
			}
			void RotateXAxis(float angle) {
				float sin_t = sin(angle);
				float cos_t = cos(angle);
				Math::Vec3<float> old = *this;
				y = (old.y * cos_t) - (sin_t * old.z);
				z = (old.y * sin_t) + (cos_t * old.z);
			}

			void Rotate(Math::Vec3<float> rot) {
				RotateXAxis(rot.x);
				RotateYAxis(rot.y);
				RotateZAxis(rot.z);
			}

			Vec3<T> operator+ (Vec3<T> v) {
				Vec3<T> ret;
				ret.x = this->x + v.x;
				ret.y = this->y + v.y;
				ret.z = this->z + v.z;
				return ret;
			}
			Vec3<T> operator- (Vec3<T> v) {
				Vec3<T> ret;
				ret.x = this->x - v.x;
				ret.y = this->y - v.y;
				ret.z = this->z - v.z;
				return ret;
			}
			Vec3<T> operator/ (Vec3<T> v) {
				Vec3<T> ret;
				ret.x = this->x / v.x;
				ret.y = this->y / v.y;
				ret.z = this->z / v.z;
				return ret;
			}
			Vec3<T> operator* (Vec3<T> v) {
				Vec3<T> ret;
				ret.x = this->x * v.x;
				ret.y = this->y * v.y;
				ret.z = this->z * v.z;
				return ret;
			}
			Vec3<T> operator- () {
				x = -x;
				y = -y;
				z = -z;
				return *this;
			}
			T Dot(Vec3<T> v) {
				T ret = (this->x * v.x) + (this->y * v.y) + (this->z + v.z);
				return ret;
			}
			Vec3<T> operator/ (double v) {
				Vec3<T> ret;
				ret.x = this->x / v;
				ret.y = this->y / v;
				ret.z = this->z / v;
				return ret;
			}
			Vec3<T> operator* (double v) {
				Vec3<T> ret;
				ret.x = this->x * v;
				ret.y = this->y * v;
				ret.z = this->z * v;
				return ret;
			}
			Vec3<T> operator+ (double v) {
				Vec3<T> ret;
				ret.x = this->x + v;
				ret.y = this->y + v;
				ret.z = this->z + v;
				return ret;
			}
			Vec3<T> operator- (double v) {
				Vec3<T> ret;
				ret.x = this->x - v;
				ret.y = this->y - v;
				ret.z = this->z - v;
				return ret;
			}

			Vec3<T> negate() { Vec3<T> v = *this; v.x = -v.x; v.y = -v.y; v.z = -v.z; return v; }



		};


		template<typename T>
		struct Vec4 : public Lua::ClassTable {

			void FromTable(Lua::Table t) override {
				x = t.find("x")->second.number;
				y = t.find("y")->second.number;
				z = t.find("z")->second.number;
				w = t.find("w")->second.number;
			}
			Lua::Table ToTable() override {
				Lua::LuaTableBuilder ret;
				ret.AddNumber("x", x);
				ret.AddNumber("y", y);
				ret.AddNumber("z", z);
				ret.AddNumber("w", w);
				return ret.GetTable();
			}

			Vec4(T x, T y, T z, T w) {
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;
			}
			Vec4(T x) {
				this->x = x;
				this->y = x;
				this->z = x;
				this->w = w;
			}
			Vec4() {
				x = 0;
				y = 0;
				z = 0;
				w = 0;
			}

			T x, y, z,w;

			void Normalize() {
				T tmp = x + y + z+w;

				x = x / tmp;
				y = y / tmp;
				z = z / tmp;
				w = w / tmp;
			}
			Vec4<T> Normalized() {
				T tmp = x + y + z+w;
				Vec4<T> ret;
				ret.x = x / tmp;
				ret.y = y / tmp;
				ret.z = z / tmp;
				ret.w = w / tmp;
				return ret;
			}

			Vec4<T> operator+ (Vec4<T> v) {
				Vec4<T> ret;
				ret.x = this->x + v.x;
				ret.y = this->y + v.y;
				ret.z = this->z + v.z;
				ret.w = this->w + v.w;
				return ret;
			}
			Vec4<T> operator- (Vec4<T> v) {
				Vec4<T> ret;
				ret.x = this->x - v.x;
				ret.y = this->y - v.y;
				ret.z = this->z - v.z;
				ret.w = this->w - v.w;
				return ret;
			}
			Vec4<T> operator- () {
				x = -x;
				y = -y;
				z = -z;
				w = -w;
				return *this;
			}
			T Dot(Vec4<T> v) {
				T ret = (this.x * v.x) + (this.y * v.y) + (this.z * v.z)+ (this.w * v.wz);
				return ret;
			}
			Vec4<T> operator* (Vec4<T> v) {
				Vec4<T> ret;
				ret.x = this->x * v.x;
				ret.y = this->y * v.y;
				ret.z = this->z * v.z;
				ret.w = this->w * v.w;
				return ret;
			}
			template <typename O>
			Vec4<T> operator/ (O v) {
				Vec4<T> ret;
				ret.x = this->x / v;
				ret.y = this->y / v;
				ret.z = this->z / v;
				ret.w = this->w / v;
				return ret;
			}
			template <typename O>
			Vec4<T> operator* (O v) {
				Vec4<T> ret;
				ret.x = this->x * v;
				ret.y = this->y * v;
				ret.z = this->z * v;
				ret.w = this->w * v;
				return ret;
			}
			template <typename O>
			Vec4<T> operator+ (O v) {
				Vec4<T> ret;
				ret.x = this->x + v;
				ret.y = this->y + v;
				ret.z = this->z + v;
				ret.w = this->w + v;
				return ret;
			}
			template <typename O>
			Vec4<T> operator- (O v) {
				Vec4<T> ret;
				ret.x = this->x - v;
				ret.y = this->y - v;
				ret.z = this->z - v;
				ret.w = this->w - v;
				return ret;
			}

			Vec4<T> negate() { 
				Vec4<T> v = *this;
				v.x = -v.x;
				v.y = -v.y;
				v.z = -v.z;
				v.w = -v.w;
				return v;
			}



		};


		template <typename T>
		Vec3<T> Difference(Vec3<T>one, Vec3<T>two) {
			Vec3<T> ret;
			ret.x = one.x - two.x;
			ret.y = one.y - two.y;
			ret.z = one.z - two.z;
			return ret;
		}
		template <typename T>
		Vec3<T> tripleProduct(Vec3<T> a, Vec3<T> b, Vec3<T> c) {

			Vec3<T> r;

			float ac = a.Dot(c); // perform a.dot(c)
			float bc = b.Dot(c);// perform b.dot(c)

			// perform b * a.dot(c) - a * b.dot(c)
			r.x = b.x * ac - a.x * bc;
			r.y = b.y * ac - a.y * bc;
			r.z = b.z * ac - a.z * bc;
			return r;
		}

		template <typename T>
		Vec3<T> averagePoint(std::vector<Vec3<T>> vertices) {
			Vec3<T> avg = Vec3<T>(0);
			for (size_t i = 0; i < vertices.size(); i++) {
				avg.x += vertices[i].x;
				avg.y += vertices[i].y;
			}
			avg.x = avg.x / vertices.size();
			avg.y = avg.y/ vertices.size();
			return avg;
		}

		template <typename T>
		size_t indexOfFurthestPoint(std::vector<Vec3<T>> vertices, Vec3<T> d) {

			float maxProduct = d.Dot(vertices[0]);
			size_t index = 0;
			for (size_t i = 1; i < vertices.size(); i++) {
				float product = d.Dot(vertices[i]);
				if (product > maxProduct) {
					maxProduct = product;
					index = i;
				}
			}
			return index;
		}

		template <typename T>
		Vec3<T> Support(std::vector<Vec3<T>> vertices1, std::vector<Vec3<T>> vertices2, Vec3<T> d) {

			// get furthest point of first body along an arbitrary direction
			size_t i = indexOfFurthestPoint(vertices1,  d);

			// get furthest point of second body along the opposite direction
			size_t j = indexOfFurthestPoint(vertices2,  d.negate());

			// subtract (Minkowski sum) the two points to see if bodies 'overlap'
			return vertices1[i] - vertices2[j];
		}

		template <typename T>
		struct Simplex {
		private:
			std::array<Vec3<T>, 4> m_points;
			unsigned m_size;

		public:
			Simplex()
				: m_points({ 0, 0, 0, 0 })
				, m_size(0)
			{}

			Simplex& operator=(std::initializer_list<Vec3<T>> list) {
				for (auto v = list.begin(); v != list.end(); v++) {
					m_points[std::distance(list.begin(), v)] = *v;
				}
				m_size = list.size();

				return *this;
			}

			void push_front(Vec3<T> point) {
				m_points = { point, m_points[0], m_points[1], m_points[2] };
				m_size = min(m_size + 1, 4u);
			}

			Vec3<T>& operator[](unsigned i) { return m_points[i]; }
			unsigned size() const { return m_size; }

			auto begin() const { return m_points.begin(); }
			auto end()   const { return m_points.end() - (4 - m_size); }
		};

		template <typename T>
		bool SameDirection(
			const Vec3<T>& direction,
			const Vec3<T>& ao)
		{
			return direction.Dot(ao) > 0;
		}

		template <typename T>
		bool Line(
			Simplex<T>& points,
			Vec3<T>& direction)
		{
			Vec3<T> a = points[0];
			Vec3<T> b = points[1];

			Vec3<T> ab = b - a;
			Vec3<T> ao = -a;

			if (SameDirection(ab, ao)) {
				direction = ab.cross(ao).cross(ab);
			}

			else {
				points = { a };
				direction = ao;
			}

			return false;
		}

		template <typename T>
		bool Triangle(
			Simplex<T>& points,
			Vec3<T>& direction)
		{
			Vec3<T> a = points[0];
			Vec3<T> b = points[1];
			Vec3<T> c = points[2];

			Vec3<T> ab = b - a;
			Vec3<T> ac = c - a;
			Vec3<T> ao = -a;

			Vec3<T> abc = ab.cross(ac);

			if (SameDirection(abc.cross(ac), ao)) {
				if (SameDirection(ac, ao)) {
					points = { a, c };
					direction = ac.cross(ao).cross(ac);
				}

				else {
					return Line(points = { a, b }, direction);
				}
			}

			else {
				if (SameDirection(ab.cross(abc), ao)) {
					return Line(points = { a, b }, direction);
				}

				else {
					if (SameDirection(abc, ao)) {
						direction = abc;
					}

					else {
						points = { a, c, b };
						direction = -abc;
					}
				}
			}

			return false;
		}

		template <typename T>
		bool Tetrahedron(
			Simplex<T>& points,
			Vec3<T>& direction)
		{
			Vec3<T> a = points[0];
			Vec3<T> b = points[1];
			Vec3<T> c = points[2];
			Vec3<T> d = points[3];

			Vec3<T> ab = b - a;
			Vec3<T> ac = c - a;
			Vec3<T> ad = d - a;
			Vec3<T> ao = -a;

			Vec3<T> abc = ab.cross(ac);
			Vec3<T> acd = ac.cross(ad);
			Vec3<T> adb = ad.cross(ab);

			if (SameDirection(abc, ao)) {
				return Triangle(points = { a, b, c }, direction);
			}

			if (SameDirection(acd, ao)) {
				return Triangle(points = { a, c, d }, direction);
			}

			if (SameDirection(adb, ao)) {
				return Triangle(points = { a, d, b }, direction);
			}

			return true;
		}

		template <typename T>
		bool NextSimplex(
			Simplex<T>& points,
			Vec3<T>& direction)
		{
			switch (points.size()) {
			case 2: return Line(points, direction);
			case 3: return Triangle(points, direction);
			case 4: return Tetrahedron(points, direction);
			}

			// never should be here
			return false;
		}

		template <typename T>
		bool gjk(std::vector<Vec3<T>> vertices1,
			std::vector<Vec3<T>> vertices2) {

			Vec3<T> support = Support(vertices1, vertices2, Vec3<T>(1, 0, 0));

			Simplex points;
			points.push_front(support);

			Vec3<T> direction = -support;

			while (true) {
				support = Support(vertices1, vertices2, direction);

				if (support.dot(direction) <= 0) {
					return false; // no collision
				}

				points.push_front(support);
				if (NextSimplex(points, direction)) {
					return true;
				}
			}
		}

		template<typename T>
		struct CollisionPoints {
			Vec3<T> A;
			Vec3<T> B;
			Vec3<T> Normal;
			float PenetrationDepth;
			bool HasCollision;
		};

		template<typename T>
		std::pair<std::vector<Vec4<T>>, size_t> GetFaceNormals(
			const std::vector<Vec3<T>>& polytope,
			const std::vector<size_t>& faces)
		{
			std::vector<Vec4<T>> normals;
			size_t minTriangle = 0;
			float  minDistance = FLT_MAX;

			for (size_t i = 0; i < faces.size(); i += 3) {
				Vec3<T> a = polytope[faces[i]];
				Vec3<T> b = polytope[faces[i + 1]];
				Vec3<T> c = polytope[faces[i + 2]];

				Vec3<T> normal = (b - a).Cross(c - a).Normalize();
				float distance = normal.Dot(a);

				if (distance < 0) {
					normal *= -1;
					distance *= -1;
				}

				normals.emplace_back(normal, distance);

				if (distance < minDistance) {
					minTriangle = i / 3;
					minDistance = distance;
				}
			}

			return { normals, minTriangle };
		}

		template<typename T>
		void AddIfUniqueEdge(
			std::vector<std::pair<size_t, size_t>>& edges,
			const std::vector<size_t>& faces,
			size_t a,
			size_t b)
		{
			auto reverse = std::find(               //      0--<--3
				edges.begin(),                     //     / \ B /   A: 2-0
				edges.end(),                       //    / A \ /    B: 0-2
				std::make_pair(faces[b], faces[a]) //   1-->--2
			);

			if (reverse != edges.end()) {
				edges.erase(reverse);
			}

			else {
				edges.emplace_back(faces[a], faces[b]);
			}
		}

		template<typename T>
		CollisionPoints<T> EPA(Simplex<T>& simplex, std::vector<Vec3<T>> vertices1, std::vector<Vec3<T>> vertices2) {
			std::vector<Vec3<T>> polytope(simplex.begin(), simplex.end());
			std::vector<int> faces = {
				0,1,2,
				0,3,1,
				0,2,3,
				1,3,2
			};

			auto [normals, minFace] = GetFaceNormals<T>(polytope, faces);

			Vec3<T> minNormal;
			float minDistance = FLT_MAX;

			while (minDistance ==FLT_MAX)
			{
				minNormal = normals[minFace].xyz();
				minDistance = normals[minFace].w;

				Vec3<T> support = Support<T>(vertices1, vertices2, minNormal);
				float sDistance = minNormal.Dot(support);

				if (abs(sDistance - minDistance) > 0.001f) {
					minDistance = FLT_MAX;

					std::vector<std::pair<int, int>> uniqueEdges;

					for (int i = 0; i < normals.size(); i++) {
						if (SameDirection<T>(normals[i], support)) {
							int f = i * 3;

							AddIfUniqueEdge<T>(uniqueEdges, faces, f, f + 1);
							AddIfUniqueEdge<T>(uniqueEdges, faces, f + 1, f + 2);
							AddIfUniqueEdge<T>(uniqueEdges, faces, f + 2, f);

							faces[f + 2] = faces.back(); faces.pop_back();
							faces[f + 1] = faces.back(); faces.pop_back();
							faces[f] = faces.back(); faces.pop_back();

							normals[i] = normals.back(); normals.pop_back();

							i--;
						}
					}

					std::vector<size_t> newFaces;
					for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
						newFaces.push_back(edgeIndex1);
						newFaces.push_back(edgeIndex2);
						newFaces.push_back(polytope.size());
					}

					polytope.push_back(support);

					auto [newNormals, newMinFace] = GetFaceNormals<T>(polytope, newFaces);

					float oldMinDistance = FLT_MAX;
					for (size_t i = 0; i < normals.size(); i++) {
						if (normals[i].w < oldMinDistance) {
							oldMinDistance = normals[i].w;
							minFace = i;
						}
					}

					if (newNormals[newMinFace].w < oldMinDistance) {
						minFace = newMinFace + normals.size();
					}

					faces.insert(faces.end(), newFaces.begin(), newFaces.end());
					normals.insert(normals.end(), newNormals.begin(), newNormals.end());
				}
			}
			CollisionPoints points;

			points.Normal = minNormal;
			points.PenetrationDepth = minDistance + 0.001f;
			points.HasCollision = true;

			return points;
		}

	}
	
	namespace Kinect {
		typedef Math::Vec4<double> Joint;

		struct Bone {
			Joint Start;
			Joint End;
			void Update(Joint s, Joint e) {
				Start = s; End = e;
			}
			Joint Normalize() {
				Joint ret = End - Start;
				//ret = ret.Normalize();
				return ret.negate();
			}
		};

		typedef Joint BoneNormal;

		struct Skelly {
			bool Tracked = false;
			Joint HandL;
			Joint HandR;
			Joint WristL;
			Joint WristR;
			Joint ElbowL;
			Joint ElbowR;
			Joint ShoulderL;
			Joint ShoulderR;
			Joint ShoulderC;
			Joint Spine;
			Joint HipL;
			Joint HipR;
			Joint HipC;
			Joint KneeL;
			Joint KneeR;
			Joint AnkleL;
			Joint AnkleR;
			Joint FootL;
			Joint FootR;
			Joint Head;
			Bone WristHandL;
			Bone WristHandR;
			Bone ElbowWristL;
			Bone ElbowWristR;
			Bone ShoulderElbowL;
			Bone ShoulderElbowR;
			Bone ShoulderCShoulderL;
			Bone ShoulderCShoulderR;
			Bone ShoulderCHead;
			Bone SpineShoulderC;
			Bone SpineHipC;
			Bone HipCHipL;
			Bone HipCHipR;
			Bone HipKneeL;
			Bone HipKneeR;
			Bone KneeAnkleL;
			Bone KneeAnkleR;
			Bone AnkleFootL;
			Bone AnkleFootR;
			BoneNormal WristHandL_Normal;
			BoneNormal WristHandR_Normal;
			BoneNormal ElbowWristL_Normal;
			BoneNormal ElbowWristR_Normal;
			BoneNormal ShoulderElbowL_Normal;
			BoneNormal ShoulderElbowR_Normal;
			BoneNormal ShoulderCShoulderL_Normal;
			BoneNormal ShoulderCShoulderR_Normal;
			BoneNormal ShoulderCHead_Normal;
			BoneNormal SpineShoulderC_Normal;
			BoneNormal SpineHipC_Normal;
			BoneNormal HipCHipL_Normal;
			BoneNormal HipCHipR_Normal;
			BoneNormal HipKneeL_Normal;
			BoneNormal HipKneeR_Normal;
			BoneNormal KneeAnkleL_Normal;
			BoneNormal KneeAnkleR_Normal;
			BoneNormal AnkleFootL_Normal;
			BoneNormal AnkleFootR_Normal;

			Bone* GetBonesArray() {
				Bone ret[19];
				ret[0] = WristHandL;
				ret[1] = WristHandR;
				ret[2] = ElbowWristL;
				ret[3] = ElbowWristR;
				ret[4] = ShoulderElbowL;
				ret[5] = ShoulderElbowR;
				ret[6] = ShoulderCShoulderL;
				ret[7] = ShoulderCShoulderR;
				ret[8] = ShoulderCHead;
				ret[9] = SpineShoulderC;
				ret[10] = SpineHipC;
				ret[11] = HipCHipL;
				ret[12] = HipCHipR;
				ret[13] = HipKneeL;
				ret[14] = HipKneeR;
				ret[15] = KneeAnkleL;
				ret[16] = KneeAnkleR;
				ret[17] = AnkleFootL;
				ret[18] = AnkleFootR;
				return ret;
			}

			void UpdateSkelly(NUI_SKELETON_DATA& data) {
				//joints
				HandL=Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].w);
				HandR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].w);
				WristL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].w);
				WristR=Joint(data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z,data.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].w);
				ElbowL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].w);
				ElbowR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].w);
				ShoulderL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].w);
				ShoulderR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].w);
				ShoulderC=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].w);
				Spine=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].x, data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].y, data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].z, data.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].w);
				HipL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].w);
				HipR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].w);
				HipC=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].w);
				KneeL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].w);
				KneeR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].w);
				AnkleL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT].w);
				AnkleR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT].w);
				FootL=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].w);
				FootR=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].x, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].y, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].z, data.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].w);
				Head=Joint( data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x, data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y, data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z, data.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].w);
				//bones
				WristHandL.Update(WristL, HandL);
				WristHandR.Update(WristR, HandR);
				ElbowWristL.Update(ElbowL, WristL);
				ElbowWristR.Update(ElbowR, WristR);
				ShoulderElbowL.Update(ShoulderL, ElbowL);
				ShoulderElbowR.Update(ShoulderR, ElbowR);
				ShoulderCShoulderL.Update(ShoulderC, ShoulderL);
				ShoulderCShoulderR.Update(ShoulderC, ShoulderR);
				ShoulderCHead.Update(ShoulderC, Head);
				SpineShoulderC.Update(Spine, ShoulderC);
				SpineHipC.Update(Spine, HipC);
				HipCHipL.Update(HipC, HipL);
				HipCHipR.Update(HipC, HipR);
				HipKneeL.Update(HipL, KneeL);
				HipKneeR.Update(HipR, KneeR);
				KneeAnkleL.Update(KneeL, AnkleL);
				KneeAnkleR.Update(KneeR, AnkleR);
				AnkleFootL.Update(AnkleL, FootL);
				AnkleFootR.Update(AnkleR, FootR);
				//bone direction/normal
				WristHandL_Normal = WristHandL.Normalize();
				WristHandR_Normal = WristHandR.Normalize();
				ElbowWristL_Normal = ElbowWristL.Normalize();
				ElbowWristR_Normal = ElbowWristR.Normalize();
				ShoulderElbowL_Normal = ShoulderElbowL.Normalize();
				ShoulderElbowR_Normal = ShoulderElbowR.Normalize();
				ShoulderCShoulderL_Normal = ShoulderCShoulderL.Normalize();
				ShoulderCShoulderR_Normal = ShoulderCShoulderR.Normalize();
				ShoulderCHead_Normal = ShoulderCHead.Normalize();
				SpineShoulderC_Normal = SpineShoulderC.Normalize();
				SpineHipC_Normal = SpineHipC.Normalize();
				HipCHipL_Normal = HipCHipL.Normalize();
				HipCHipR_Normal = HipCHipR.Normalize();
				HipKneeL_Normal = HipKneeL.Normalize();
				HipKneeR_Normal = HipKneeR.Normalize();
				KneeAnkleL_Normal = KneeAnkleL.Normalize();
				KneeAnkleR_Normal = KneeAnkleR.Normalize();
				AnkleFootL_Normal = AnkleFootL.Normalize();
				AnkleFootR_Normal = AnkleFootR.Normalize();
			}
		};

		NUI_SKELETON_FRAME ourframe;
		Skelly People[6];

		void Init() {
			NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		}

		void UpdateSkeletons() {
			NuiSkeletonGetNextFrame(0, &ourframe); //Get a frame and stuff it into ourframe
			for (int i = 0; i < 6; i++) {
				People[i].Tracked = ourframe.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED;
				if (People[i].Tracked) {
					People[i].UpdateSkelly(ourframe.SkeletonData[i]);
				}
				//std::cout << "Person[" << i << "]: x:" << People[i].HandR.x << " y:" << People[i].HandR.y << " z:" << People[i].HandR.z<<std::endl;
			}
			//system("cls");
		}
	}

	namespace Graphics {
		VoxelFox::Math::Vec3<float> HSVtoRGB(float H, float S, float V) {
			if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
				return VoxelFox::Math::Vec3<float>();
			}
			float s = S / 100;
			float v = V / 100;
			float C = s * v;
			float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
			float m = v - C;
			float r, g, b;
			if (H >= 0 && H < 60) {
				r = C, g = X, b = 0;
			}
			else if (H >= 60 && H < 120) {
				r = X, g = C, b = 0;
			}
			else if (H >= 120 && H < 180) {
				r = 0, g = C, b = X;
			}
			else if (H >= 180 && H < 240) {
				r = 0, g = X, b = C;
			}
			else if (H >= 240 && H < 300) {
				r = X, g = 0, b = C;
			}
			else {
				r = C, g = 0, b = X;
			}
			float R = VoxelFox::Math::MapRange<float>((r + m) * 255, 0, 255, 0, 1);
			float G = VoxelFox::Math::MapRange<float>((g + m) * 255, 0, 255, 0, 1);
			float B = VoxelFox::Math::MapRange<float>((b + m) * 255, 0, 255, 0, 1);
			return VoxelFox::Math::Vec3<float>(R, G, B);
		}

		struct Texture {
			GLuint TexID;
			std::string Path;
			GLuint WrapingMode;
			GLuint Interpolation;

			Texture() {
				TexID = 0;
				WrapingMode = 0;
				Interpolation = 0;
				Path = "";
			}
			Texture(GLuint _TexID, std::string _Path, GLuint _WrapingMode, GLuint _Interpolation) {
				TexID = _TexID;
				WrapingMode = _WrapingMode;
				Interpolation = _Interpolation;
				Path = _Path;
			}
		};

		static std::map<std::string, Texture> Textures;

		int lastTexture = 0;
		Texture LoadTexture(std::string theFileName, GLint wrapingType, GLint Interpolation)
		{
			ILuint imageID;                // Create an image ID as a ULuint
			GLuint TextureID = lastTexture;
			lastTexture++;

			ILboolean success;            // Create a flag to keep track of success/failure

			ILenum error;                // Create a flag to keep track of the IL error state

			ilGenImages(1, &imageID);         // Generate the image ID

			ilBindImage(imageID);             // Bind the image

			success = ilLoadImage(GetWC(theFileName));     // Load the image file

			// If we managed to load the image, then we can start to do things with it...
			if (success)
			{
				std::cout << "Texture " << theFileName << " load successful." << std::endl;
				// The Image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
				iluFlipImage();

				// Convert the image into a suitable format to work with
				// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
				success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

				// Quit out if we failed the conversion
				if (!success)
				{
					error = ilGetError();
					std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
					exit(-1);
				}

				// Generate a new texture
				glGenTextures(1, &TextureID);

				// Bind the texture to a name
				glBindTexture(GL_TEXTURE_2D, TextureID);
				// Set texture clamping method
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapingType);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapingType);

				// Set texture interpolation method to use linear interpolation (no MIPMAPS)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Interpolation);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Interpolation);

				// Specify the texture specification
				glTexImage2D(GL_TEXTURE_2D,                 // Type of texture
					0,                // Pyramid level (for mip-mapping) - 0 is the top level
					ilGetInteger(IL_IMAGE_BPP),    // Image colour depth
					ilGetInteger(IL_IMAGE_WIDTH),    // Image width
					ilGetInteger(IL_IMAGE_HEIGHT),    // Image height
					0,                // Border width in pixels (can either be 1 or 0)
					ilGetInteger(IL_IMAGE_FORMAT),    // Image format (i.e. RGB, RGBA, BGR etc.)
					GL_UNSIGNED_BYTE,        // Image data type
					ilGetData());            // The actual image data itself
			}
			else // If we failed to open the image file in the first place...
			{
				error = ilGetError();
				std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
				exit(-1);
			}

			ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

			std::cout << "Texture creation successful." << std::endl;

			return Texture(TextureID, theFileName, wrapingType, Interpolation);
		}

		struct Material {
			Math::Vec4<float> Color;
			Texture ColorMap;
		};

		static std::map<std::string, Material> Materials;

		void LoadMaterials(std::string path,std::string FileName) {
			std::ifstream input(path + "/"+FileName);
			std::string token;
			std::string tmp;
			Material Mat;
			std::string MatName;
			bool BuildingMat = false;
			while (std::getline(input, token)) {
				if (token[0] == '#') {
					continue;
				}
				if (token == ""&&BuildingMat) {
					Materials.insert(std::pair<std::string, Material>(MatName,Mat));
					BuildingMat = false;
					Mat = Material();
					MatName = "";
				}

				std::stringstream TokenStream(token);
				std::string Identifier;
				std::getline(TokenStream, Identifier,' ');
				if (Identifier == "newmtl") {
						BuildingMat = true;
						std::getline(TokenStream, tmp,' ');
						MatName = tmp;
				}
				if (Identifier == "Kd"&&BuildingMat) {
						float r, g, b;
						std::getline(TokenStream, tmp, ' ');
						r = atof(tmp.c_str());
						std::getline(TokenStream, tmp, ' ');
						g = atof(tmp.c_str());
						std::getline(TokenStream, tmp, ' ');
						b = atof(tmp.c_str());
						Mat.Color=Math::Vec4<float>(r, g, b,1);
				}
				if (Identifier == "map_Kd"&&BuildingMat) {
						std::getline(TokenStream, tmp, ' ');
						Mat.ColorMap = LoadTexture((path + "/"+tmp).c_str(), GL_CLAMP_TO_EDGE,GL_NEAREST);
				}

			}
		}

		void BindTexture(Texture t) {
			glBindTexture(GL_TEXTURE_2D, t.TexID);
			glEnable(GL_TEXTURE_2D);
		}

		namespace Geometry {

			class Bone {
			protected:
				Bone * Parent;
				Math::Vec3<float> Rotation;
				Math::Vec3<float> Pos;
				float scale;
				std::map<int, float> weights;
			public:
				Math::Vec3<float> GetGlobalPos() {
					if (Parent != nullptr) {
						return Parent->GetGlobalPos() + Pos;
					}
					else {
						return Pos;
					}
				}

				Math::Vec3<float> GetPos() {
					return Pos;
				}

				float GetWeight(int i) {
					return weights.find(i)->second;
				}

				Bone() {

				}
				~Bone() {

				}
			};

			class Armature {
			public:
				std::vector<Bone> bones;
				std::map<int, std::map<int, float>> weights;
			};

			class Mesh {
			public:
				struct Face {
					int Tex = 0;
					std::vector<Math::Vec3<int>> face;
				};
				enum Mode {
					quad,
					tri,
					triFan,
					line,
					curve,
					closedcurve,
					loop,
					loaded
				};
			protected:
				Mode mode=tri;
				Math::Vec3<float> pos;
				Math::Vec3<float> rot;
				Math::Vec3<float> scl;
				std::vector<Math::Vec3<float>> data;
				std::vector<Math::Vec3<float>> normals;
				std::vector<std::string> MaterialIDs;
				std::vector<Math::Vec2<float>> TextureCoords;
				std::vector<Face> Faces;
				Armature arm;
			public:
				Mesh() {
					scl = Math::Vec3<float>(1);
				}

				std::vector<Math::Vec3<float>> * getData() {
					return &data;
				}
				std::vector<Math::Vec3<float>> * getNormals() {
					return &normals;
				}
				std::vector<Math::Vec3<float>> getDrawableNormals() {
					return normals;
				}
				std::vector<std::string> * getMatIDS() {
					return &MaterialIDs;
				}
				std::vector<std::string> getDrawableMatIDS() {
					return MaterialIDs;
				}
				std::vector<Math::Vec2<float>> * getTextureCoords() {
					return &TextureCoords;
				}
				std::vector<Face> * getFaces() {
					return &Faces;
				}
				std::vector<Face> getDrawableFaces() {
					return Faces;
				}

				std::vector<Math::Vec3<float>> getDrawableData() {
					std::vector<Math::Vec3<float>> ret = data;
					if (arm.bones.empty()||arm.weights.empty()) {
						for (int i = 0; i < ret.size(); i++)
						{
							ret[i].Rotate(rot);
							ret[i] = ret[i] * scl;
							ret[i] = ret[i] + pos;
						}
					}
					else {
						for (int i = 0; i < ret.size(); i++)
						{
							Math::Vec3<float> tmppoint=Math::Vec3<float>(0);
							int total=0;
							for (int j = 0; j < arm.bones.size(); j++)
							{
								if (arm.weights[i][j]!=0) {
									total++;
									tmppoint = tmppoint + (arm.bones[j].GetGlobalPos() * arm.weights[i][j]) + ret[i];
								}
							}
							tmppoint = tmppoint / total;
							ret[i].Rotate(rot);
							ret[i] = ret[i] * scl;
							ret[i] = ret[i] + pos;
						}
					}
					return ret;
				}

				void setRot(Math::Vec3<float> rotation) {
					rot = rotation;
				}
				Math::Vec3<float> getRot() {
					return rot;
				}
				void setMode(Mesh::Mode m) {
					mode = m;
				}
				Mesh::Mode getMode() {
					return mode;
				}
				void setPos(Math::Vec3<float> position) {
					pos = position;
				}
				Math::Vec3<float> getPos() {
					return pos;
				}
				void setScl(Math::Vec3<float> scale) {
					scl = scale;
				}
				Math::Vec3<float> getScl() {
					return scl;
				}
			};

			std::map<std::string, Mesh> Meshes;

			static class MeshLoader {

			public:
				static Mesh LoadFBX(std::string name) {
				
				}
				static Mesh LoadDAE(std::string name) {
				
				}

				static Mesh LoadOBJ(std::string path,std::string name) {
					//std::vector<Mesh> retVec;
					Mesh ret;
					ret.setMode(Graphics::Geometry::Mesh::loaded);

					std::ifstream infile(path+"/"+ name);
					if (!infile.is_open()) {
						std::cout << "Couldnt find obj to load: " << path + "/" + name << std::endl;
					}
					std::string line;
					int textToUse = 0;
					while (std::getline(infile, line)){
						std::stringstream lineStream(line);
						std::string token;
						std::getline(lineStream, token,' ');
						if (token == "mtllib") {
							std::getline(lineStream, token, ' ');
							LoadMaterials(path,token);
						}
						if (token == "o") {
							continue;
						}
						if (token == "usemtl") {
							std::getline(lineStream, token, ' ');
							ret.getMatIDS()->push_back(token);
							textToUse = ret.getMatIDS()->size() - 1;
						}
						if (token == "v") {
							std::getline(lineStream, token, ' ');
							float x= atof(token.c_str());
							std::getline(lineStream, token, ' ');
							float y = atof(token.c_str());
							std::getline(lineStream, token, ' ');
							float z = atof(token.c_str());
							ret.getData()->push_back(Math::Vec3<float>(x,y,z));
						}
						if (token == "vt") {
							std::getline(lineStream, token, ' ');
							float x = atof(token.c_str());
							std::getline(lineStream, token, ' ');
							float y = atof(token.c_str());
							ret.getTextureCoords()->push_back(Math::Vec2<float>(x, y));
						}
						if (token == "vn") {
							std::getline(lineStream, token, ' ');
							float x = atof(token.c_str());
							std::getline(lineStream, token, ' ');
							float y = atof(token.c_str());
							std::getline(lineStream, token, ' ');
							float z = atof(token.c_str());
							ret.getNormals()->push_back(Math::Vec3<float>(x, y, z));
						}
						if (token == "f") {
							Mesh::Face face;
							while (std::getline(lineStream, token, ' ')) {
								Math::Vec3<int> vals;
								std::stringstream facestream(token);
								std::string tmps;
								std::getline(facestream, tmps,'/');
								vals.x = atoi(tmps.c_str())-1;
								std::getline(facestream, tmps,'/');
								vals.y = atoi(tmps.c_str())-1;
								std::getline(facestream, tmps,'/');
								vals.z = atoi(tmps.c_str())-1;
								face.Tex = textToUse;
								face.face.push_back(vals);
							}
							ret.getFaces()->push_back(face);
						}
					}
					return ret;
				}
			};

			void LoadMeshesFromDir(std::string path) {
				for (auto const& dir_entry : std::filesystem::directory_iterator{ std::filesystem::path{path} }) {
					if (dir_entry.path().extension() == ".obj") {
						Meshes.insert(std::pair<std::string, Mesh>(dir_entry.path().filename().string(), MeshLoader::LoadOBJ(path, dir_entry.path().filename().string())));
					}
				}
			}

			class Mesh2D {
			public:
				enum Mode {
					quad,
					tri,
					triFan,
					line,
					curve,
					closedcurve,
					loop
				};
			protected:
				Mode mode=tri;
				Math::Vec2<float> pos;
				float rot;
				Math::Vec2<float> scl;
				std::vector<Math::Vec2<float>> data;
			public:
				Mesh2D() {
					scl = Math::Vec2<float>(1);
				}

				std::vector<Math::Vec2<float>> * getData() {
					return &data;
				}

				std::vector<Math::Vec2<float>> getDrawableData() {
					std::vector<Math::Vec2<float>> ret=data;
					for (int i = 0; i < ret.size(); i++)
					{
						ret[i].Rotate(rot);
						ret[i] = ret[i] * scl;
						ret[i]=ret[i] + pos;
					}
					return ret;
				}

				void setRot(float rotation) {
					rot = rotation;
				}
				Math::Vec2<float> getRot() {
					return rot;
				}
				void setMode(Mesh2D::Mode m) {
					mode = m;
				}
				Mesh2D::Mode getMode() {
					return mode;
				}
				void setPos(Math::Vec2<float> position) {
					pos = position;
				}
				Math::Vec2<float> getPos() {
					return pos;
				}
				void setScl(Math::Vec2<float> scale) {
					scl = scale;
				}
				Math::Vec2<float> getScl() {
					return scl;
				}
			};

			class Circle2D :public Mesh2D {
				
			public:
				float radius;
				Circle2D(Math::Vec2<float> center, float radius,int smoothness) {
					pos = center;
					this->radius = radius;
					mode = closedcurve;
					for (int ii = 0; ii < smoothness; ii++)
					{
						float theta = 2.0f * 3.1415926f * float(ii) / float(smoothness);//get the current angle

						float x = radius * cosf(theta);//calculate the x component
						float y = radius * sinf(theta);//calculate the y component

						data.push_back(Math::Vec2<float>(x, y));//output vertex

					}
				}
				Circle2D() {
					mode = closedcurve;
					for (int ii = 0; ii < 20; ii++)
					{
						float theta = 2.0f * 3.1415926f * float(ii) / float(20);//get the current angle

						float x = 1 * cosf(theta);//calculate the x component
						float y = 1 * sinf(theta);//calculate the y component

						data.push_back(Math::Vec2<float>(x + 0, y + 0));//output vertex

					}
				}
			};

			class Line :public Mesh {
			public:
				Line(Math::Vec3<float> pos1, Math::Vec3<float> pos2) {
					Mesh::Mesh();
					mode = line;
					data.push_back(pos1);
					data.push_back(pos2);
				}
				Line() {
					Mesh::Mesh();
					mode = line;
					data.push_back(Math::Vec3<float>(0,0,0));
					data.push_back(Math::Vec3<float>(1,0,0));
				}
			};
			class Line2D :public Mesh2D {
			public:
				Line2D(Math::Vec2<float> pos1, Math::Vec2<float> pos2) {
					Mesh::Mesh();
					mode = line;
					data.push_back(pos1);
					data.push_back(pos2);
				}
				Line2D() {
					Mesh::Mesh();
					mode = line;
					data.push_back(Math::Vec2<float>(0,0));
					data.push_back(Math::Vec2<float>(1,0));
				}
			};

			template<typename T>
			T Lerp(T start_value, T end_value, T pct)
			{
				return (start_value + (end_value - start_value) * pct);
			}

			template<typename T>
			Math::Vec2<T> Lerp(Math::Vec2<T> point1, Math::Vec2<T> point2, T x) {
				Math::Vec2<T> ret;
				ret.x = Lerp<T>(point1.x, point2.x, x);
				ret.y = Lerp<T>(point1.y, point2.y, x);
				return ret;
			}

			template<typename T>
			Math::Vec2<T> BezierCurveFunct(std::vector<Math::Vec2<T>> points, T x) {
				std::vector<Math::Vec2<T>> Holder = points;

				while (Holder.size() > 1) {
					std::vector<Math::Vec2<T>> tmp;
					for (int i = 0; i < Holder.size() - 1; i++) {
						tmp.push_back(Lerp<T>(Holder[i], Holder[i + 1], x));
					}
					Holder = tmp;
				}
				return Holder[0];
			}

			template<typename T>
			Math::Vec3<T> Lerp(Math::Vec3<T> point1, Math::Vec3<T> point2, T x) {
				Math::Vec3<T> ret;
				ret.x = Lerp<T>(point1.x, point2.x, x);
				ret.y = Lerp<T>(point1.y, point2.y, x);
				ret.z = Lerp<T>(point1.z, point2.z, x);
				return ret;
			}

			template<typename T>
			Math::Vec3<T> BezierCurveFunct(std::vector<Math::Vec3<T>> points, T x) {
				std::vector<Math::Vec3<T>> Holder = points;

				while (Holder.size() > 1) {
					std::vector<Math::Vec3<T>> tmp;
					for (int i = 0; i < Holder.size() - 1; i++) {
						tmp.push_back(Lerp<T>(Holder[i], Holder[i + 1], x));
					}
					Holder = tmp;
				}
				return Holder[0];
			}

			class BezierCurve : public Mesh {
			public:
				BezierCurve(std::vector<Math::Vec3<float>> points, int smoothness) {
					mode = curve;
					float pointsAlongLine = smoothness;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>(points, (1.0 / pointsAlongLine) * i));
					}
				}
				BezierCurve() {
					mode = curve;
					float pointsAlongLine = 12;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>({ Math::Vec3<float>(0,0,0),Math::Vec3<float>(0,1,0),Math::Vec3<float>(0,0,1) }, (1.0 / pointsAlongLine) * i));
					}
				}
			};

			class ClosedBezierCurve : public Mesh {
			public:
				ClosedBezierCurve(std::vector<Math::Vec3<float>> points, int smoothness) {
					mode = curve;
					float pointsAlongLine = smoothness;
					std::vector<Math::Vec3<float>> input = points;
					input.push_back(points[0]);
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>(input, (1.0 / pointsAlongLine) * i));
					}
				}
				ClosedBezierCurve() {
					mode = closedcurve;
					float pointsAlongLine = 12;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>({ Math::Vec3<float>(0,0,0),Math::Vec3<float>(0,1,0),Math::Vec3<float>(0,0,1),Math::Vec3<float>(0,0,0) }, (1.0 / pointsAlongLine) * i));
					}
				}
			};

			class BezierCurve2D : public Mesh2D {
			public:
				BezierCurve2D(std::vector<Math::Vec2<float>> points, int smoothness) {
					mode = curve;
					rot = 0;
					float pointsAlongLine = smoothness;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>(points, (1.0 / pointsAlongLine) * i));
					}
				}
				BezierCurve2D() {
					mode = curve;
					rot = 0;
					float pointsAlongLine = 12;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>({ Math::Vec2<float>(0,0),Math::Vec2<float>(1,0),Math::Vec2<float>(1,1),Math::Vec2<float>(0,1) }, (1.0 / pointsAlongLine) * i));
					}
				}
			};

			class ClosedBezierCurve2D : public Mesh2D {
			public:
				ClosedBezierCurve2D(std::vector<Math::Vec2<float>> points, int smoothness) {
					mode = closedcurve;
					float pointsAlongLine = smoothness;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>(points, (1.0 / pointsAlongLine) * i));
					}
				}
				ClosedBezierCurve2D() {
					mode = closedcurve;
					float pointsAlongLine = 12;
					for (int i = 0; i < pointsAlongLine; i++)
					{
						data.push_back(BezierCurveFunct<float>({ Math::Vec2<float>(0,0),Math::Vec2<float>(0,1),Math::Vec2<float>(1,1),Math::Vec2<float>(1,0) }, (1.0 / pointsAlongLine) * i));
					}
				}
			};

			class Tri : public Mesh {
			public:
				Tri(Math::Vec3<float> pos1,Math::Vec3<float> pos2,Math::Vec3<float> pos3) {
					Mesh::Mesh();
					data.push_back(pos1);
					data.push_back(pos2);
					data.push_back(pos3);
				}
				Tri() {
					Mesh::Mesh();
					data.push_back(Math::Vec3<float>(0, 0, 0));
					data.push_back(Math::Vec3<float>(0, 1, 0));
					data.push_back(Math::Vec3<float>(1, 1, 0));
				}
			};

			class Quad :public Mesh {
			public:
				Quad(Math::Vec3<float> pos1, Math::Vec3<float> pos2, Math::Vec3<float> pos3, Math::Vec3<float> pos4) {
					Mesh::Mesh();
					mode = quad;
					data.push_back(pos1);
					data.push_back(pos2);
					data.push_back(pos3);
					data.push_back(pos4);
				}
				Quad() {
					Mesh::Mesh();
					mode = quad;
					data.push_back(Math::Vec3<float>(0, 0, 0));
					data.push_back(Math::Vec3<float>(1, 0, 0));
					data.push_back(Math::Vec3<float>(1, 1, 0));
					data.push_back(Math::Vec3<float>(0, 1, 0));
				}
			};

			class Circle :public Mesh {
				float radius;
				float resolution;
			public:
				Circle(float rad,float res) {
					Mesh::Mesh();
					int i;
					resolution = res; //# of triangles used to draw circle

					//GLfloat radius = 0.8f; //radius
					GLfloat twicePi = 2.0f * M_PI;

					mode = triFan;
					data.push_back(Math::Vec3<float>(pos.x, pos.y,0)); // center of circle
					for (i = 0; i <= resolution; i++) {
						data.push_back(Math::Vec3<float>(
							pos.x + (radius * cos(i * twicePi / resolution)),
							pos.y + (radius * sin(i * twicePi / resolution)),0
						));
					}
				}
				Circle() {
					Mesh::Mesh();
					int i;
					radius = 1;
					resolution = 20; //# of triangles used to draw circle

					//GLfloat radius = 0.8f; //radius
					GLfloat twicePi = 2.0f * M_PI;

					mode = triFan;
					data.push_back(Math::Vec3<float>(pos.x, pos.y,0)); // center of circle
					for (i = 0; i <= resolution; i++) {
						data.push_back(Math::Vec3<float>(
							pos.x + (radius * cos(i * twicePi / resolution)),
							pos.y + (radius * sin(i * twicePi / resolution)),0
						));
					}
				}
			};

			class Cube : public Mesh {
			public:
				Cube() {
					Mesh::Mesh();
					mode = quad;
					//front
					data.push_back(Math::Vec3<float>(0 , 0 , 0 ));
					data.push_back(Math::Vec3<float>(0 , (0 + 1) , 0 ));
					data.push_back(Math::Vec3<float>((0 + 1) , (0 + 1) , 0 ));
					data.push_back(Math::Vec3<float>((0 + 1) , 0 , 0 ));
					//back
					data.push_back(Math::Vec3<float>(0 , 0 , (0 + 1) ));
					data.push_back(Math::Vec3<float>(0 , (0 + 1) , (0 + 1) ));
					data.push_back(Math::Vec3<float>((0 + 1) , (0 + 1) , (0 + 1) ));
					data.push_back(Math::Vec3<float>((0 + 1) , 0 , (0 + 1) ));
					//left
					data.push_back(Math::Vec3<float>(0 , 0 , (0 + 1) ));
					data.push_back(Math::Vec3<float>(0 , 0 , 0 ));
					data.push_back(Math::Vec3<float>(0 , (0 + 1) , 0 ));
					data.push_back(Math::Vec3<float>(0 , (0 + 1) , (0 + 1) ));
					//right
					data.push_back(Math::Vec3<float>((0 + 1) , 0 , (0 + 1) ));
					data.push_back(Math::Vec3<float>((0 + 1) , 0 , 0 ));
					data.push_back(Math::Vec3<float>((0 + 1) , (0 + 1) , 0 ));
					data.push_back(Math::Vec3<float>((0 + 1) , (0 + 1) , (0 + 1) ));
					//bottom
					data.push_back(Math::Vec3<float>((0 + 1) , 0 , 0 ));
					data.push_back(Math::Vec3<float>(0 , 0 , 0 ));
					data.push_back(Math::Vec3<float>(0 , 0 , (0 + 1) ));
					data.push_back(Math::Vec3<float>((0 + 1) , 0 , (0 + 1) ));
					//top
					data.push_back(Math::Vec3<float>((0 + 1) , (0 + 1) , 0 ));
					data.push_back(Math::Vec3<float>(0 , (0 + 1) , 0 ));
					data.push_back(Math::Vec3<float>(0 , (0 + 1) , (0 + 1) ));
					data.push_back(Math::Vec3<float>((0 + 1) , (0 + 1) , (0 + 1) ));
				}
			};

		}

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3
#define BUTTON_LB 4
#define BUTTON_RB 5
#define BUTTON_SELECT 6
#define BUTTON_START 7
#define BUTTON_LSTICK 8
#define BUTTON_RSTICK 9
#define BUTTON_DPADU 10
#define BUTTON_DPADR 11
#define BUTTON_DPADD 12
#define BUTTON_DPADL 13
#define STICK_1 0
#define STICK_2 1
#define TRIGGERS 2

		class Controller {
			int AxisCount;
			const float* Axis;
			int ButtonCount;
			const unsigned char* Buttons;
			int id;
		public:
			Controller(int controller_id) {
				id = controller_id;
				Axis = glfwGetJoystickAxes(controller_id, &AxisCount);
				Buttons = glfwGetJoystickButtons(controller_id, &ButtonCount);
				//std::cout << "Num axes: " << AxisCount << std::endl;
				//for (int i = 0; i < AxisCount; i++)
				//{
				//	std::cout << Axis[i] << " ";
				//}
				//std::cout << std::endl;

				//std::cout << (bool)Buttons[BUTTON_A] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_B] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_X] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_Y] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_LB] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_RB] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_SELECT] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_START] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_LSTICK] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_RSTICK] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_DPADU] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_DPADR] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_DPADD] << std::endl;
				//std::cout << (bool)Buttons[BUTTON_DPADL] << std::endl;
			}

			void Update() {
				Axis = glfwGetJoystickAxes(id, &AxisCount);
				Buttons = glfwGetJoystickButtons(id, &ButtonCount);
			}

			bool ButtonPressed(int button) {
				return (bool)Buttons[button];
			}

			Math::Vec2<float> GetStick(int stick) {
				Math::Vec2<float> ret;
				ret.x = Axis[0+(2*stick)];
				ret.y = Axis[1+(2*stick)];
				return ret;
			}
		};

		struct InputHandler {
			struct Binding {
				enum BindingType {
					ControllerButton,
					KeyboardButton,
					MouseButton,
					MousePos,
					ControllerAxis
				};
				int controller=NULL;
				int stick=NULL;
				int keyButton=NULL;
				BindingType bt=ControllerButton;
				bool ButtonPressed=false;
				Math::Vec2<double> AxisPos=Math::Vec2<double>(0);
			};

			std::map<std::string, Binding> Inputs;
			std::map<int, Controller> controllers;
			bool SettingInput = false;
			std::string name = "";

			void AddNextInput(std::string Name) {
				name = name;
			}

			void AddUpdateInput(std::string name, Binding binding) {
				std::map<std::string, Binding>::iterator it = Inputs.find(name);
				if (it != Inputs.end()) {
					it->second = binding;
				}
				else {
					Inputs.insert(std::pair<std::string, Binding>(name, binding));
				}
			}

			Binding GetInput(std::string name) {
				std::map<std::string, Binding>::iterator it = Inputs.find(name);
				if (it != Inputs.end()) {
					return it->second;
				}
				else {

				}
			}

			void CheckControllers() {
				for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++) {
					if (glfwJoystickPresent(i)) {
						if (controllers.find(i) == controllers.end()) {
							controllers.insert(std::pair<int, Controller>(i, Controller(i)));
						}
						controllers.find(i)->second.Update();
					}
					else {
						controllers.erase(i);
					}
				}
			}

			void Update() {
				for (std::map<int, Controller>::iterator it = controllers.begin(); it != controllers.end(); it++) {
					it->second.Update();
				}
				for (std::map<std::string, Binding>::iterator it = Inputs.begin(); it != Inputs.end(); it++) {
					if (controllers.size() > 0) {
						if (it->second.bt == Binding::ControllerButton) {

							it->second.ButtonPressed = controllers.find(it->second.controller)->second.ButtonPressed(it->second.keyButton);
						}

						if (it->second.bt == Binding::ControllerAxis) {
							Math::Vec2<double> val = controllers.find(it->second.controller)->second.GetStick(it->second.stick).reType<double>();
							it->second.AxisPos = val;
						}
					}
				}
			}

		};

		class Camera : public Lua::ClassTable{
			Math::Vec3<float> Pos;
			Math::Vec3<float> Rot;
			Math::Vec3<float> EyePos;
			float NearPlane;
			float FarPlane;
			float FOV;
		public:

			void FromTable(Lua::Table t) override {
				Pos.FromTable(t.find("Pos")->second.table);
				EyePos.FromTable(t.find("EyePos")->second.table);
				NearPlane=t.find("NearPlane")->second.number;
				FarPlane=t.find("FarPlane")->second.number;
				FOV=t.find("FOV")->second.number;
			};
			Lua::Table ToTable() override{
				Lua::LuaTableBuilder ret;
				ret.AddTable("Pos", Pos.ToTable());
				ret.AddTable("EyePos", EyePos.ToTable());
				ret.AddNumber("NearPlane", NearPlane);
				ret.AddNumber("FarPlane", FarPlane);
				ret.AddNumber("FOV", FOV);
				return ret.GetTable();
			};

			Camera() {
				Pos = Math::Vec3<float>(0);
				EyePos = Math::Vec3<float>(0);
				NearPlane = .001;
				FarPlane = 1000;
				FOV = 90;
			}
			void SetNearPlane(float NP) {
				NearPlane = NP;
			}
			void SetFarPlane(float FP) {
				FarPlane = FP;
			}
			float GetNearPlane() {
				return NearPlane;
			}
			void SetFOV(float Fov) {
				FOV = Fov;
			}
			float GetFOV() {
				return FOV;
			}
			float GetFarPlane() {
				return FarPlane;
			}

			Math::Vec3<float> GetAppliedEyePos() {
				Math::Vec3<float> ret = EyePos;
				ret.Rotate(Rot);
				return ret;
			}

			Math::Vec3<float> GetEyePos(){
				return EyePos;
			}
			Math::Vec3<float>* GetEyePosPoint(){
				return &EyePos;
			}
			Math::Vec3<float> GetPos(){
				return Pos;
			}
			Math::Vec3<float> GetRot(){
				return Rot;
			}
			Math::Vec3<float>* GetPosPoint() {
				return &Pos;
			}
			void SetEyePos(Math::Vec3<float> v) {
				EyePos = v;
			}
			void SetPos(Math::Vec3<float> v) {
				Pos = v;
			}
			void SetRot(Math::Vec3<float> v) {
				Rot = v;
			}
		};

		class VRCamera : public Camera {
			float eyeDist;
		};

		static InputHandler inputHandler;

		void KeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (inputHandler.SettingInput) {
				InputHandler::Binding binding;
				binding.bt = InputHandler::Binding::KeyboardButton;
				binding.keyButton = key;
				if (action == GLFW_PRESS) {
					binding.ButtonPressed = true;
				}
				else {
					binding.ButtonPressed = false;
				}
				inputHandler.AddUpdateInput(inputHandler.name, binding);
				return;
			}

			for (std::map<std::string, InputHandler::Binding>::iterator it = inputHandler.Inputs.begin(); it != inputHandler.Inputs.end(); it++) {
				if (it->second.bt == InputHandler::Binding::KeyboardButton && key == it->second.keyButton) {
					if (action == GLFW_PRESS) {
						it->second.ButtonPressed = true;
					}
					else {
						it->second.ButtonPressed = false;
					}
				}
			}
		}
		//glfwSetCursorPosCallback(window, cursor_position_callback);
		void MousePosHandler(GLFWwindow* window, double xpos, double ypos)
		{
			if (inputHandler.SettingInput) {
				InputHandler::Binding binding;
				binding.bt = InputHandler::Binding::MousePos;
				binding.AxisPos = Math::Vec2<double>(xpos, ypos);
				inputHandler.AddUpdateInput(inputHandler.name, binding);
				return;
			}
			for (std::map<std::string, InputHandler::Binding>::iterator it = inputHandler.Inputs.begin(); it != inputHandler.Inputs.end(); it++) {
				if (it->second.bt == InputHandler::Binding::MousePos) {
					it->second.AxisPos = Math::Vec2<double>(xpos, ypos);
				}
			}
		}
		//glfwSetMouseButtonCallback(window, mouse_button_callback);
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			if (inputHandler.SettingInput) {
				InputHandler::Binding binding;
				binding.bt = InputHandler::Binding::MouseButton;
				binding.keyButton = button;
				if (action == GLFW_PRESS) {
					binding.ButtonPressed = true;
				}
				else {
					binding.ButtonPressed = false;
				}
				inputHandler.AddUpdateInput(inputHandler.name, binding);
				return;
			}
			for (std::map<std::string, InputHandler::Binding>::iterator it = inputHandler.Inputs.begin(); it != inputHandler.Inputs.end(); it++) {
				if (it->second.bt == InputHandler::Binding::MouseButton) {
					if (action == GLFW_PRESS) {
						it->second.ButtonPressed = true;
					}
					else {
						it->second.ButtonPressed = false;
					}
				}
			}
		}

		class Window {
			typedef void(*UIFunction)(Window* window);
			typedef void(*DrawFunction)(Window* window);
			typedef void(*UpdateFunction)(Window* window);
			typedef void(*FinalFunction)(Window* window);
			typedef void(*SetupFunction)(Window* window);

			GLFWwindow* window;
			std::string title;
			float ratio;
			int width, height;
			int fps = 0;
			bool isOpen;
			bool isFocus;
			bool fillObject;

			std::thread DrawThread;
			std::vector<Camera> cams;
			int selectedCam = 0;

			UIFunction UILoop;
			DrawFunction DrawLoop;
			UpdateFunction UpdateLoop;
			FinalFunction FinalLoop;
			SetupFunction Setup;

			void DrawFunct() {

				if (!glfwInit())
					return;

				/* Create a windowed mode window and its OpenGL context */
				window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
				if (!window)
				{
					glfwTerminate();
				}
				/* Make the window's context current */
				glfwMakeContextCurrent(window);
				static bool tmpfocus = false;
				glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
					{
						if (focused)
						{
							tmpfocus = true;
						}
						else
						{
							tmpfocus = false;
						}
					});
				glfwSetKeyCallback(window, KeyHandler);
				glfwSetCursorPosCallback(window, MousePosHandler);
				glfwSetMouseButtonCallback(window, mouse_button_callback);
				static int w = 0;
				static int h = 0;
				glfwSetFramebufferSizeCallback(window, [](GLFWwindow* wind, int width, int Height) {
					w = width;
					h = Height;
					});
				Setup(this);
				/* Loop until the user closes the window */
				isOpen = true;

				while (!glfwWindowShouldClose(window))
				{
					inputHandler.CheckControllers();
					inputHandler.Update();
					isFocus = tmpfocus;
					width = w;
					height = h;
					/* Render here */
					if (UpdateLoop!=nullptr) {
						UpdateLoop(this);
					}
					if (width != 0 && height != 0) {
						gluPerspective(cams[selectedCam].GetFOV(), width / height, cams[selectedCam].GetNearPlane(), cams[selectedCam].GetFarPlane());
					}
					else {
						gluPerspective(cams[selectedCam].GetFOV(), 1, cams[selectedCam].GetNearPlane(), cams[selectedCam].GetFarPlane());
					}
					gluLookAt(cams[selectedCam].GetAppliedEyePos().x, cams[selectedCam].GetAppliedEyePos().y, cams[selectedCam].GetAppliedEyePos().z, cams[selectedCam].GetPos().x, cams[selectedCam].GetPos().y, cams[selectedCam].GetPos().z, 0, 1, 0);
					if (DrawLoop != nullptr) {
						DrawLoop(this);
					}
					if (UILoop != nullptr) {
						UILoop(this);
					}
					if (FinalLoop != nullptr) {
						FinalLoop(this);
					}
					/* Swap front and back buffers */
					glfwSwapBuffers(window);

					/* Poll for and process events */
					glfwPollEvents();
					//limit frames
					if (fps > 0) {
						std::this_thread::sleep_for(std::chrono::milliseconds((1000 / fps)));
					}
				}
				isOpen = false;

				glfwTerminate();
			}
		public:

			Math::Vec2<double> GetSize() {
				return Math::Vec2<double>(width,height);
			}

			void Fill(bool b) {
				fillObject = b;
			}

			Camera* GetCamera(int i) {
				return &cams[i];
			}

			int NumCams() {
				return cams.size();
			}

			int GetFps() {
				return fps;
			}

			void SetFps(int _fps) {
				fps=_fps;
			}

			void AddCam(Camera c) {
				cams.push_back(c);
			}

			void SelectCam(int i) {
				selectedCam = i;
			}
			int GetSelectedCam() {
				return selectedCam;
			}

			GLFWwindow* GetWindow() {
				return window;
			}

			float getRatio() {
				return ratio;
			}
			void Init(std::string title, int x, int y, UpdateFunction UFunct, DrawFunction DFunct, SetupFunction setup, UIFunction ui, FinalFunction finalf) {
				Setup = setup;
				UpdateLoop = UFunct;
				DrawLoop = DFunct;
				UILoop = ui;
				FinalLoop = finalf;
				this->width = x;
				this->height = y;
				this->title = title;
				isOpen = true;
				DrawThread = std::thread(&Window::DrawFunct, this);
				cams.push_back(Camera());
			}

			bool IsOpen() {
				return isOpen;
			}
			bool IsFocused() {
				return isFocus;
			}

			void Draw(Geometry::Mesh m) {
				if (!fillObject) {
					std::vector<Math::Vec3<float>> data = m.getDrawableData();
					std::vector<Math::Vec3<float>> normals = m.getDrawableNormals();
					std::vector<Math::Vec2<float>> TexCoords = *m.getTextureCoords();
					std::vector<Geometry::Mesh::Face> Faces = m.getDrawableFaces();
					std::map<std::string, Material>::iterator it;
					int texindex= -1;

					switch (m.getMode())
					{
					case Geometry::Mesh::loaded:
						for (int i = 0; i < Faces.size(); i++)
						{
							if (texindex != Faces[i].Tex) {
								texindex = Faces[i].Tex;
								it = Materials.find(m.getDrawableMatIDS()[Faces[i].Tex]);
								if (it != Materials.end()) {
									glColor4f(it->second.Color.x, it->second.Color.y, it->second.Color.z, it->second.Color.w);
									BindTexture(it->second.ColorMap);
								}
							}
							if (Faces[i].face.size() == 3) {
								glBegin(GL_TRIANGLES);
							}
							if (Faces[i].face.size() == 4) {
								glBegin(GL_QUADS);
							}
							for (int j = 0; j < Faces[i].face.size(); j++) {
								if (normals.size() > 0) {
									glNormal3f(normals[Faces[i].face[j].z].x, normals[Faces[i].face[j].z].y, normals[Faces[i].face[j].z].z);
								}
								if (TexCoords.size() > 0) {
									glTexCoord2f(TexCoords[Faces[i].face[j].y].x, TexCoords[Faces[i].face[j].y].y);
								}
								glVertex3f(data[Faces[i].face[j].x].x, data[Faces[i].face[j].x].y, data[Faces[i].face[j].x].z);
							}
							glEnd();
						}
						glDisable(GL_TEXTURE_2D);
						break;
					case Geometry::Mesh::quad:
						glBegin(GL_QUADS);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex3f(data[i].x, data[i].y, data[i].z);
						}
						glEnd();
						break;
					case Geometry::Mesh::tri:
						glBegin(GL_TRIANGLES);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex3f(data[i].x, data[i].y, data[i].z);
						}
						glEnd();
						break;
					case Geometry::Mesh::triFan:
						glBegin(GL_TRIANGLE_FAN);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex3f(data[i].x, data[i].y, data[i].z);
						}
						glEnd();
						break;
					case Geometry::Mesh::line:
						glBegin(GL_LINE_STRIP);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex3f(data[i].x, data[i].y, data[i].z);
						}
						glEnd();
						break;
					case Geometry::Mesh::curve:
						for (int i = 0; i < data.size() - 1; i++)
						{
							glBegin(GL_LINE_STRIP);
							glVertex3f(data[i].x, data[i].y, data[i].z);
							glVertex3f(data[i + 1].x, data[i + 1].y, data[i + 1].z);
							glEnd();
						}
						break;
					case Geometry::Mesh::closedcurve:
						for (int i = 0; i < data.size() - 1; i++)
						{
							glBegin(GL_LINE_LOOP);
							glVertex3f(data[i].x, data[i].y, data[i].z);
							glVertex3f(data[i + 1].x, data[i + 1].y, data[i + 1].z);
							glEnd();
						}
						break;
					case Geometry::Mesh::loop:
						glBegin(GL_LINE_LOOP);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex3f(data[i].x, data[i].y, data[i].z);
						}
						glEnd();
						break;
					default:
						break;
					}
				}
				else {

				}
			}
			void Draw(Geometry::Mesh2D m) {
				if (!fillObject) {
					std::vector<Math::Vec2<float>> data = m.getDrawableData();
					switch (m.getMode())
					{
					case Geometry::Mesh2D::quad:
						glBegin(GL_QUADS);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex2f(data[i].x, data[i].y);
						}
						glEnd();
						break;
					case Geometry::Mesh2D::tri:
						glBegin(GL_TRIANGLES);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex2f(data[i].x, data[i].y);
						}
						glEnd();
						break;
					case Geometry::Mesh2D::triFan:
						glBegin(GL_TRIANGLE_FAN);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex2f(data[i].x, data[i].y);
						}
						glEnd();
						break;
					case Geometry::Mesh2D::line:
						glBegin(GL_LINE_STRIP);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex2f(data[i].x, data[i].y);
						}
						glEnd();
						break;
					case Geometry::Mesh2D::curve:
						for (int i = 0; i < data.size() - 1; i++)
						{
							glBegin(GL_LINE_STRIP);
							glVertex2f(data[i].x, data[i].y);
							glVertex2f(data[i + 1].x, data[i + 1].y);
							glEnd();
						}
						break;
					case Geometry::Mesh2D::closedcurve:
						for (int i = 0; i < data.size() - 1; i++)
						{
							glBegin(GL_LINE_STRIP);
							glVertex2f(data[i].x, data[i].y);
							glVertex2f(data[i + 1].x, data[i + 1].y);
							glEnd();
						}
						glBegin(GL_LINE_STRIP);
						glVertex2f(data[0].x, data[0].y);
						glVertex2f(data[data.size() - 1].x, data[data.size() - 1].y);
						glEnd();
						break;
					case Geometry::Mesh2D::loop:
						glBegin(GL_LINE_LOOP);
						for (int i = 0; i < data.size(); i++)
						{
							glVertex2f(data[i].x, data[i].y);
						}
						glEnd();
						break;
					default:
						break;
					}
				}
				else {
					std::vector<Math::Vec2<float>> data = m.getDrawableData();
					Math::Vec2<float> avg;
					for (int i = 0; i < data.size(); i++)
					{
						avg = avg + data[i];
					}
					avg = avg / data.size();
					glBegin(GL_TRIANGLE_FAN);
					for (int i = 0; i < data.size(); i++)
					{
						glVertex2f(data[i].x, data[i].y);
					}
					glVertex2f(data[0].x, data[0].y);
					glEnd();
				}
			}

			~Window() {
				DrawThread.join();
			}
		};
	}

}
