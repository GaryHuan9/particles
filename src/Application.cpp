#include "Application.hpp"

#include <raylib.h>

namespace ptl
{
	Application::Application()
	{
		InitWindow(1920, 1080, "particles");

//		SetTargetFPS(60);
		BeginDrawing();
		ClearBackground(BLACK);
	}

	Application::~Application()
	{
		CloseWindow();
	}

	bool Application::update(float& delta_time) const
	{
		EndDrawing();
		delta_time = GetFrameTime();

		if (WindowShouldClose()) return false;

		BeginDrawing();
		ClearBackground(BLACK);

		auto fps = static_cast<uint32_t>(1.0f / delta_time);
		DrawText(std::to_string(fps).c_str(), 0, 0, 16, LIGHTGRAY);

		return true;
	}

	void Application::draw_circle(float x, float y, float radius)
	{
		DrawCircleGradient(std::lround(x), std::lround(y), radius, WHITE, (Color){ 255, 255, 255, 24 });
	}

	bool Application::last_click(int32_t& x, int32_t& y)
	{
		if (!IsMouseButtonPressed(0)) return false;

		x = static_cast<int32_t>(GetMouseX());
		y = static_cast<int32_t>(GetMouseY());
		return true;
	}
}
