#include <raylib.h>
#include "Application.hpp"
#include "Renderer.hpp"

#define RAYGUI_IMPLEMENTATION

#include <raygui/raygui.h>

#undef RAYGUI_IMPLEMENTATION

ptl::Application::Application()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1920, 1080, "particles");
	renderer = new Renderer();

//	SetTargetFPS(60);
	BeginDrawing();
	ClearBackground(BLACK);
}

ptl::Application::~Application()
{
	delete static_cast<Renderer*>(renderer);
	CloseWindow();
}

bool ptl::Application::update(float& delta_time)
{
	EndDrawing();
	delta_time = GetFrameTime();

	if (WindowShouldClose()) return false;

	BeginDrawing();
	ClearBackground(BLACK);

	x_current = y_current = GapSize;
	return true;
}

void ptl::Application::text(const char* text)
{
	DrawText(text, (int)x_current, (int)y_current, ElementHeight, LIGHTGRAY);
	y_current += ElementHeight + GapSize;
}

bool ptl::Application::button(const char* text)
{
	Rectangle rectangle{ (float)x_current, (float)y_current, ElementWidth, ElementHeight };

	bool result = GuiButton(rectangle, text);
	y_current += ElementHeight + GapSize;

	return result;
}

bool ptl::Application::slider(const char* text, float& value, float min, float max)
{
	Rectangle rectangle{ (float)x_current, (float)y_current, ElementWidth, ElementHeight };

	float result = GuiSlider(rectangle, nullptr, text, value, min, max);
	y_current += ElementHeight + GapSize;

	bool changed = result != value;
	value = result;
	return changed;
}

bool ptl::Application::slider(const char* text, int32_t& value, int32_t min, int32_t max)
{
	Rectangle rectangle{ (float)x_current, (float)y_current, ElementWidth, ElementHeight };

	int32_t result = std::lround(GuiSlider(rectangle, nullptr, text, (float)value, (float)min, (float)max));
	y_current += ElementHeight + GapSize;

	bool changed = result != value;
	value = result;
	return changed;
}

void ptl::Application::draw(size_t length, const float* x_positions, const float* y_positions, const float* radii) const
{
	static_cast<Renderer*>(renderer)->draw(length, x_positions, y_positions, radii);
}

void ptl::Application::get_center(float& center_x, float& center_y)
{
	center_x = (float)GetScreenWidth() / 2.0f;
	center_y = (float)GetScreenHeight() / 2.0f;
}

bool ptl::Application::last_click(int32_t& x, int32_t& y)
{
	if (!IsMouseButtonPressed(0)) return false;

	x = static_cast<int32_t>(GetMouseX());
	y = static_cast<int32_t>(GetMouseY());
	return true;
}
