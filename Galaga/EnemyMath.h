#pragma once
#include <cmath>
#include <vector>
#include <glm.hpp>
#include <numbers>
#include <fstream>
#include <string>
#include <filesystem>
#include <numeric>
// I did get most of the logic for beziers from ather omar on yt
//https://youtu.be/qoIYkCwWLkA?si=yUKUKsOsaZnlnpqR
// he coincidently also made galaga but I wont just take his work.they are good videos.
// 

struct BezierCurve
{
	glm::fvec2 startPoint;
	glm::fvec2 controlPoint1;
	glm::fvec2 endPoint;
	glm::fvec2 controlPoint2;

	glm::fvec2 CalcCurvePoint(float time)
	{
		float time2{ time * time };
		float time3{ time2 * time };
		float u{ 1.0f - time };
		float u2{ u * u };
		float u3{ u2 * u };

		glm::fvec2 point = (u3 * startPoint) + (3 * u2 * time * controlPoint1) + (3 * u * time2 * endPoint) + (time3 * controlPoint2);
		//point.x = std::round(point.x);
		//point.y = std::round(point.y);

		return point;
	}
};


class BezierPath
{
public:
	BezierPath();

	void AddCurve(std::vector<BezierCurve> curve, int samples);
	std::vector<std::pair<glm::fvec2,float>> Sample();
	int GetSamples()const { return std::accumulate(m_Samples.begin(),m_Samples.end(),0); }
	float GetAngle(glm::fvec2 p1, glm::fvec2 p2)
	{
		float radians = atan2(p2.y - p1.y, p2.x - p1.x);
		return static_cast<float>(radians * (180 / std::numbers::pi) + 90);
	}
	

private:
	
	std::vector<BezierCurve> m_Curve;
	std::vector<int> m_Samples;

};

struct BezierInfo
{
	bool read{ false };
	std::filesystem::path path;
	std::fstream file;
	std::vector<std::vector<BezierCurve>> curves;
	std::vector<std::vector<glm::vec2>> gridPos;
};

struct ReadBezier {
	
	static BezierInfo m_Info;
	void GetBezierDataFromFile(const std::string& path);

	
};

