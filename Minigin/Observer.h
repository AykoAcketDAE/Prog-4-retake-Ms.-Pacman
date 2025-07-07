#pragma once
#include "Events.h"
#include "GameObject.h"
namespace dae
{
	class Observer
	{
		friend class Subject;
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(Event event, const GameObject& go) = 0;
	private:
		Observer* m_Next{nullptr};
	};

	class Subject
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void Notify(const GameObject& go, Event event);

	private:
		Observer * m_Head{nullptr};
	};
}

