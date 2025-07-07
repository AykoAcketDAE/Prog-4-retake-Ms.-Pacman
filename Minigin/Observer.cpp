#include "Observer.h"

void dae::Subject::AddObserver(Observer* observer)
{
	observer->m_Next = m_Head;
	m_Head = observer;
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	if (m_Head == observer)
	{
		m_Head = observer->m_Next;
		observer->m_Next = nullptr;
		return;
	}
	Observer* current{m_Head};
	while (current != nullptr)
	{
		if (current->m_Next == observer)
		{
			current->m_Next = observer->m_Next;
			observer->m_Next = nullptr;
			return;
		}

		current = current->m_Next;
	}
}

void dae::Subject::Notify(const GameObject& go, Event event)
{
	Observer* observer = m_Head;
	while (observer != NULL)
	{
		observer->OnNotify(event,go);
		observer = observer->m_Next;
	}
}
