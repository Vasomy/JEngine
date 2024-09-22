#pragma once
#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include<Windows.h>
#include<runtime/core/JCores.h>
class WindowTimer
{
public:
	WindowTimer();

	float TotalTime()const; // in seconds
	float DeltaTime()const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double mSecondsPerCount =0;
	double mDeltaTime = 0;

	__int64 mBaseTime = 0;
	__int64 mPausedTime = 0;
	__int64 mStopTime = 0;
	__int64 mPrevTime = 0;
	__int64 mCurrTime = 0;

	bool mStopped;
};
class WindowSystem
{
public:
	WindowSystem() = default;
	bool Init();
	bool InitMainWindow();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	float32 Tick(){
		mTimer.Tick();
		CalculateFrameStats();
		return mTimer.DeltaTime();
	}
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	bool IsRenderSytemRun = false;

	bool IsWindowShouldClose() { return msg.message == WM_QUIT; }
	MSG msg = { 0 };

	HINSTANCE HInstance = nullptr;
	HWND	  MainWnd = nullptr;
	Vec2Int WindowSize = { 1600,960 };
private:
	void OnResize();

	void CalculateFrameStats();

	WindowTimer mTimer;

	bool OnResizing = false;
	std::wstring WindowName = L"JEngine";
	bool IsMaximized = false;
	bool IsMinimized = false;

};

#endif // !WINDOWSYSTEM_H
