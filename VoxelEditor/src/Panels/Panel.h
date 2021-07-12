#pragma once

#include "Voxel/Events/Event.h"
#include "Voxel/Core/Timestep.h"
#include <string>

namespace Voxel {

	class Panel
	{
	public:
		Panel(const char* name)
			: m_Name(name) 
		{
			m_ID = rand() % UINT32_MAX;
		}

		virtual ~Panel() = default;

		virtual void OnImGuiRender() = 0;
		virtual void OnUpdate(Timestep& timestep) = 0;
		virtual void OnEvent(Event& event) = 0;

		const char*& GetName() { return m_Name; }
		const int GetID() { return m_ID; }

	protected:
		std::string GetImGuiID()
		{
			return std::string(m_Name).append("##").append(std::to_string(m_ID));
		}

	private:
		const char* m_Name = "";
		uint32_t m_ID = 0;
	};

}