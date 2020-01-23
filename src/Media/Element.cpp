/*
*
* Copyright (C) 2016 OtherCrashOverride@users.noreply.github.com.
* All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
*/

#include "Element.h"

	Element::Element() {}

	void Element::Initialize() {}

	void Element::DoWork()
	{
		Log("Element (%s) DoWork exited.\n", name.c_str());
	}

	void Element::Terminating() {}

	ExecutionStateEnum Element::GetExecutionState()
	{
		ExecutionStateEnum result;

		pthread_mutex_lock(&executionWaitMutex);
		result = executionState;
		pthread_mutex_unlock(&executionWaitMutex);

		return result;
	}

	void Element::SetExecutionState(ExecutionStateEnum newState)
	{
		bool isOperationInvalid = false;

		// Reading current state, validating then setting new state must be atomic
		pthread_mutex_lock(&executionWaitMutex);
		
		if (newState != executionState)
		{
			switch (executionState)
			{
				case ExecutionStateEnum::WaitingForExecute:
					if (newState != ExecutionStateEnum::Initializing)
					{
						isOperationInvalid = true;
					}
					break;

				case ExecutionStateEnum::Initializing:
					if (newState != ExecutionStateEnum::Idle)
					{
						isOperationInvalid = true;
					}
					break;

				case ExecutionStateEnum::Executing:
					if (newState != ExecutionStateEnum::Idle &&
						newState != ExecutionStateEnum::Terminating)
					{
						isOperationInvalid = true;
					}
					break;

				case ExecutionStateEnum::Idle:
					if (newState != ExecutionStateEnum::Executing &&
						newState != ExecutionStateEnum::Terminating)
					{
						isOperationInvalid = true;
					}
					break;

				case ExecutionStateEnum::Terminating:
					if (newState != ExecutionStateEnum::WaitingForExecute)
					{
						isOperationInvalid = true;
					}
					break;

				default:
					break;
			}

			// if the operation is valid then set the new state
			if (!isOperationInvalid) {
				executionState = newState;
				Log("Element: %s Set ExecutionState=%d\n", name.c_str(), (int)newState);
			}
		}

		if (pthread_cond_broadcast(&executionWaitCondition) != 0)
		{
			throw Exception("Element::SignalExecutionWait - pthread_cond_broadcast failed.");
		}

		pthread_mutex_unlock(&executionWaitMutex);

		if (isOperationInvalid) {
			throw InvalidOperationException();
		}
	}

	void Element::ExecutingRunLoop()
	{
		while (isRunning)
		{
			if (GetState() == MediaState::Play)
			{
				if (GetExecutionState() != ExecutionStateEnum::Executing)
				{
					SetExecutionState(ExecutionStateEnum::Executing);
					Log("Element (%s) InternalWorkThread woke.\n", name.c_str());
				}

				DoWork();
			}

			if (!isRunning)
				break;


			pthread_mutex_lock(&waitMutex);

			while (canSleep)
			{
				if (GetExecutionState() != ExecutionStateEnum::Idle)
				{
					Log("Element (%s) InternalWorkThread sleeping.\n", name.c_str());
					SetExecutionState(ExecutionStateEnum::Idle);
				}

				pthread_cond_wait(&waitCondition, &waitMutex);
			}

			canSleep = true;

			pthread_mutex_unlock(&waitMutex);
		}
	}

	void Element::InternalWorkThread()
	{
		Log("Element (%s) InternalWorkThread entered.\n", name.c_str());

		Thread::SetCancelTypeDeferred(false);
		Thread::SetCancelEnabled(true);

		SetExecutionState(ExecutionStateEnum::Initializing);
		Initialize();

		SetExecutionState(ExecutionStateEnum::Idle);
		ExecutingRunLoop();

		Log("Element (%s) InternalWorkThread - Terminating\n", name.c_str());
		SetExecutionState(ExecutionStateEnum::Terminating);
		Terminating();

		SetExecutionState(ExecutionStateEnum::WaitingForExecute);

		Log("Element (%s) InternalWorkThread exited.\n", name.c_str());
	}

	bool Element::IsExecuting()
	{
		return GetState() == MediaState::Play && GetExecutionState() == ExecutionStateEnum::Executing;
	}

	void Element::Execute()
	{
		if (GetExecutionState() != ExecutionStateEnum::WaitingForExecute)
		{
			throw InvalidOperationException();
		}

		isRunning = true;
		thread.Start();

		Log("Element (%s) Execute.\n", name.c_str());
	}

	void Element::Wake()
	{
		pthread_mutex_lock(&waitMutex);

		canSleep = false;

		if (pthread_cond_broadcast(&waitCondition) != 0)
		{
			throw Exception("Element::Wake - pthread_cond_broadcast failed.");
		}

		pthread_mutex_unlock(&waitMutex);

		Log("Element (%s) Wake.\n", name.c_str());
	}

	void Element::Terminate()
	{
		if (GetExecutionState() != ExecutionStateEnum::Executing && GetExecutionState() != ExecutionStateEnum::Idle)
		{
			throw InvalidOperationException();
		}

		SetState(MediaState::Pause);
		WaitForExecutionState(ExecutionStateEnum::Idle);

		Flush();

		isRunning = false;

		Wake();

		WaitForExecutionState(ExecutionStateEnum::WaitingForExecute);

		Log("Element (%s) thread.Join().\n", name.c_str());

		thread.Join();

		Log("Element (%s) Terminate.\n", name.c_str());
	}

	void Element::ChangeState(MediaState oldState, MediaState newState)
	{
		state = newState;

		Wake();

		Log("Element (%s) ChangeState oldState=%d newState=%d.\n", name.c_str(), (int)oldState, (int)newState);
	}

	void Element::WaitForExecutionState(ExecutionStateEnum stateToWait)
	{
		pthread_mutex_lock(&executionWaitMutex);

		while (executionState != stateToWait)
		{
			pthread_cond_wait(&executionWaitCondition, &executionWaitMutex);
		}

		pthread_mutex_unlock(&executionWaitMutex);

	}

	void Element::Flush()
	{
		if (GetState() != MediaState::Pause)
			throw InvalidOperationException();

		WaitForExecutionState(ExecutionStateEnum::Idle);

		inputs.Flush();
		outputs.Flush();

		Log("Element (%s) Flush exited.\n", name.c_str());
	}

	void Element::AddInputPin(InPinSPTR pin)
	{
		inputs.Add(pin);
	}

	void Element::ClearInputPins()
	{
		inputs.Clear();
	}

	void Element::AddOutputPin(OutPinSPTR pin)
	{
		outputs.Add(pin);
	}

	void Element::ClearOutputPins()
	{
		outputs.Clear();
	}

	InPinCollection* Element::GetInputs()
	{
		return &inputs;
	}

	OutPinCollection* Element::GetOutputs()
	{
		return &outputs;
	}

	std::string Element::GetName() const
	{
		return name;
	}

	void Element::SetName(std::string name)
	{
		this->name = name;
	}

	bool Element::GetLogEnabled() const
	{
		return logEnabled;
	}

	void Element::SetLogEnabled(bool value)
	{
		logEnabled = value;
	}

	MediaState Element::GetState() const
	{
		return state;
	}

	void Element::SetState(MediaState value)
	{
		if (state != value)
		{
			ChangeState(state, value);
		}
	}

	Element::~Element()
	{
		Log("Element %s destructed.\n", name.c_str());
	}

	// DEBUG
	void Element::Log(const char* message, ...)
	{
		if (logEnabled)
		{
			struct timeval tp;
			gettimeofday(&tp, NULL);
			double ms = tp.tv_sec + tp.tv_usec * 0.0001;

			char text[1024];
			sprintf(text, "[%s : %f] %s", GetName().c_str(), ms, message);


			va_list argptr;
			va_start(argptr, message);
			vfprintf(stderr, text, argptr);
			va_end(argptr);
		}
	}
