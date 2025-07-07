#pragma once
#include "Singleton.h"


namespace dae {
	class Time: public Singleton<Time>
	{
	public:
		void SetDeltaTime(float time) { 
			m_DeltaTime = time / 1000000; 
		};
		float GetDeltaTime() const { return m_DeltaTime; };
		float GetFps() { return 1 / m_DeltaTime; };
	private:
		friend class Singleton<Time>;
		Time() = default;
		Time(const Time& other) = delete;
		Time(Time&& other) = delete;
		Time& operator=(const Time& other) = delete;
		Time& operator=(Time&& other) = delete;

		float m_DeltaTime{};
	};

}

