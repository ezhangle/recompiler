#pragma once

namespace native
{
	class IThread;

	enum
	{
		TimeoutInfinite = 0xFFFFFFFF,
	};

	enum class WaitResult
	{
		Success,
		IOCompletion,
		Timeout,
		Failed,
		Abandoned,
	};

	enum class ThreadPriority
	{
		Lowest,
		BelowNormal,
		Normal,
		AboveNormal,
		Highest,
	};

	class LAUNCHER_API IRunnable
	{
	public:
		virtual ~IRunnable();
		virtual int Run(IThread& thread) = 0;
		virtual void ProcessAPC() = 0;
	};

	/// critical section
	class LAUNCHER_API ICriticalSection
	{
	public:
		virtual ~ICriticalSection();

		virtual void Acquire() = 0;
		virtual void Release() = 0;
	};

	/// event
	class LAUNCHER_API IEvent
	{
	public:
		virtual ~IEvent();

		virtual bool Set() = 0;
		virtual bool Pulse() = 0;
		virtual bool Reset() = 0;

		virtual WaitResult Wait(const uint32 timeout, const bool alertable) = 0;
	};

	/// semaphore
	class LAUNCHER_API ISemaphore
	{
	public:
		virtual ~ISemaphore();

		virtual uint32 Release(const uint32 count) = 0;
		virtual WaitResult Wait(const uint32 timeout, const bool alertable) = 0;
	};

	/// thread
	class LAUNCHER_API IThread
	{
	public:
		virtual ~IThread();

		virtual WaitResult Wait(const uint32 timeout, const bool alertable) = 0;

		virtual bool Pause() = 0;
		virtual bool Resume() = 0;

		virtual int32 SetPriority(const ThreadPriority priority) = 0;
		virtual uint32 SetAffinity(const uint32 affinity) = 0;

		virtual WaitResult Sleep(const uint32 ms, const bool alertable) = 0;

		virtual void ScheduleAPC() = 0; // 			QueueUserAPC( (PAPCFUNC)&DeliverAPCs, m_physicalThread, (ULONG_PTR)this );
	};

	/// kernel
	class LAUNCHER_API IKernel
	{
	public:
		virtual ~IKernel();

		/// create a native (host) critical section
		virtual ICriticalSection* CreateCriticalSection() = 0;

		/// create a native (host) event
		virtual IEvent* CreateEvent(const bool manualReset, const bool initialState) = 0;

		/// create a native (host) semaphore
		virtual ISemaphore* CreateSemaphore(const uint32 initialCount, const uint32 maxCount) = 0;

		/// create a native (host) thread running given code
		virtual IThread* CreateThread(IRunnable* runnable) = 0;
	};

} // native