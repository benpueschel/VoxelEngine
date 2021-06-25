#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

using namespace std::chrono;

namespace Voxel {

	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = high_resolution_clock::now();

			long long start = time_point_cast<microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = time_point_cast<microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		time_point<high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#if VOXEL_PROFILE

	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define VOXEL_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define VOXEL_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
		#define VOXEL_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define VOXEL_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define VOXEL_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define VOXEL_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define VOXEL_FUNC_SIG __func__
	#else
		#define VOXEL_FUNC_SIG "HZ_FUNC_SIG unknown!"
	#endif

	#define PROFILE_BEGIN_SESSION(name, path) :: Voxel::Instrumentor::Get().BeginSession(name, path)
	#define PROFILE_END_SESSION() :: Voxel::Instrumentor::Get().EndSession()
	#define PROFILE_SCOPE(name) :: Voxel::InstrumentationTimer timer##__LINE__(name);
	#define PROFILE_FUNCTION() PROFILE_SCOPE(VOXEL_FUNC_SIG)
#else
	#define PROFILE_BEGIN_SESSION(name, path)
	#define PROFILE_END_SESSION()
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif