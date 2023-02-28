#pragma once

#ifndef HZN_TIMESTEP_H
#define HZN_TIMESTEP_H

namespace Hzn
{
	class TimeStep
	{
	public:
		/// <summary>
		/// Timestep class
		/// </summary>
		/// <param name="time"></param>
		TimeStep(float time = 0.0f) : m_Time(time) {}
		
		operator float() const
		{
			return m_Time;
		}

	private:

		float m_Time;
	};
}

#endif