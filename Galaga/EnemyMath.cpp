#include "EnemyMath.h"
#include <iostream>

BezierInfo ReadBezier::m_Info;

BezierPath::BezierPath()
{

}

void BezierPath::AddCurve(std::vector<BezierCurve> curve, int samples)
{
	for(auto& data:curve)m_Curve.emplace_back(data);
	m_Samples.emplace_back(static_cast<int>(samples * curve.size()));
}

std::vector<std::pair<glm::fvec2, float>> BezierPath::Sample()
{
	std::vector<std::pair<glm::fvec2, float>> sampledPath;
	for (int index{}; index < m_Curve.size(); ++index)
	{
		for (float time{}; time <= 1.f; time += (1.f / m_Samples[0]))
		{
			sampledPath.emplace_back(m_Curve[index].CalcCurvePoint(time), 0.f);
		}
	}

	for (int index{}; index < sampledPath.size(); ++index)
	{
		if (index + 1 != sampledPath.size())
		{
			if (GetAngle(sampledPath[index].first, sampledPath[index + 1].first) == 0)
			{
				sampledPath[index].second = sampledPath[index - 1].second;
			}

			else 
			{
				sampledPath[index].second = GetAngle(sampledPath[index].first, sampledPath[index + 1].first);
			}
			
		}
	}
	return sampledPath;
}

void ReadBezier::GetBezierDataFromFile(const std::string& path)
{
	std::filesystem::path data_location = "./Data/";
	if (!std::filesystem::exists(data_location))
		data_location = "../Data/";

	m_Info.path = data_location/path;
	if (std::filesystem::exists(m_Info.path))
	{
		m_Info.file = std::fstream(m_Info.path);
		std::cout << "found file\n---------------------------------------------------------\n\n";
	}
	else
	{
		std::cout << "File not found\n---------------------------------------------------------\n\n";
		return;
	}
	m_Info.curves.resize(10);
	int BezierCount{};
	int RowCount{};
	while (!m_Info.file.eof())
	{
		std::string line{};
		std::string number{};
		BezierCurve curve{};
		size_t pos{};
		std::vector<float> nums;
		std::vector<float> gridPos;

		std::getline(m_Info.file, line);
		if (line == "")
		{
			++BezierCount;
			continue;
		}
		if (line.starts_with("B"))
		{
			line = line.substr(2);
			for (int index{}; index < 8; ++index)
			{

				pos = line.find(" ");
				number = line.substr(0, pos);
				line = line.substr(pos + 1);
				nums.emplace_back(std::stof(number));

				std::cout << number << std::endl;
			}
			curve.startPoint = { nums[0],nums[1] };
			curve.controlPoint1 = { nums[2],nums[3] };
			curve.endPoint = { nums[4],nums[5] };
			curve.controlPoint2 = { nums[6],nums[7] };
			m_Info.curves[BezierCount].emplace_back(curve);
		}
		if (line.starts_with("G"))
		{
			line = line.substr(2);
			for (int index{}; index < 2; ++index)
			{
				pos = line.find(" ");
				number = line.substr(0, pos);
				line = line.substr(pos + 1);
				gridPos.emplace_back(std::stof(number));
				std::cout << number << std::endl;
			}
			if (m_Info.gridPos.empty())
			{
				m_Info.gridPos.resize(5);
				glm::vec2 value = { gridPos[0], gridPos[1] };
				m_Info.gridPos[RowCount].emplace_back(value);
			}
			else if(m_Info.gridPos[RowCount][0].y == gridPos[1])
			{
				glm::vec2 value = { gridPos[0], gridPos[1] };
				m_Info.gridPos[RowCount].emplace_back(value);
			}
			else
			{
				glm::vec2 value = { gridPos[0], gridPos[1] };
				m_Info.gridPos[++RowCount].emplace_back(value);
			}
		
		}
		
	}
	
}
