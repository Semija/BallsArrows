#define OLC_PGE_APPLICATION
#include "Includings\olcPixelGameEngine.h"


class Window : public olc::PixelGameEngine
{
public: 
	class Point { public: int x, y,radius; };

	static const int pointsCount = 3;
	Point pointsMatrix[pointsCount];
	Point* selectedPoint = NULL;

	Window() { sAppName = "BallsArrows"; }

	bool IsInsideRadius(int X, int Y, int objX, int objY, int objR) {
		return std::sqrt((X - objX) * (X - objX) + (Y - objY) * (Y - objY)) < (objR + 1);
	}

	bool IsMousePointsAnyPoint() {
		bool answer = 0;
		for (int i = 0; i < pointsCount; i++) {
			if (IsInsideRadius(GetMouseX(), GetMouseY(), pointsMatrix[i].x, pointsMatrix[i].y, pointsMatrix[i].radius)) {
				answer = 1;
				break;
			}
		}
		return answer;
	}
	
	Point* WhatMousePoints() {
		Point* answer = 0;
		for (int i = 0; i < pointsCount; i++) {
			if (IsInsideRadius(GetMouseX(), GetMouseY(), pointsMatrix[i].x, pointsMatrix[i].y, pointsMatrix[i].radius)) {
				answer = &pointsMatrix[i];
				break;
			}
		}
		return answer;
	}

	void FillRegion(int X1, int Y1, int X2, int Y2, int offset, olc::Pixel Color) {
		int start = 10;
			int tmp = 0;
		if (X1 > X2) {
			tmp = X1;
			X1 = X2;
			X2 = tmp;
		}
		if (Y1 > Y2) {
			tmp = Y1;
			Y1 = Y2;
			Y2 = tmp;
		}
		for (int DrawY = Y1; DrawY < Y2; DrawY++) {
			for (int DrawX = X1 + start; DrawX < X2; DrawX += offset) {
				if (DrawX >= X1)
					Draw(DrawX, DrawY, Color);
			}
			start--;
		}
		DrawLine(X1,Y1,X1,Y2, Color);
		DrawLine(X2, Y1, X2, Y2, Color);
	}

	// Called once at the start
	bool OnUserCreate() override {
		pointsMatrix[0].x = 50; pointsMatrix[0].y = 50; pointsMatrix[0].radius = 3;
		pointsMatrix[1].x = 100; pointsMatrix[1].y = 100; pointsMatrix[1].radius = 3;
		pointsMatrix[2].x = 50; pointsMatrix[2].y = 150; pointsMatrix[2].radius = 3;
		return true;
	}

	// Called once per frame
	bool OnUserUpdate(float fElapsedTime) override { 

		// exit
		if (GetKey(olc::Key::ESCAPE).bPressed) return false;

		// select point
		if (GetMouse(0).bPressed) {
			if (IsMousePointsAnyPoint()) {
				selectedPoint = WhatMousePoints();
			}
			else {
				selectedPoint = nullptr;
			}
		}
		if (selectedPoint != 0) {
			// drag selected point
			if (GetMouse(0).bHeld) {
				selectedPoint->x = GetMouseX();
				selectedPoint->y = GetMouseY();
			}
			// change size of selected point
			if (GetMouseWheel() > 0) selectedPoint->radius += 1;
			if (GetMouseWheel() < 0) selectedPoint->radius -= 1;
			if (selectedPoint->radius < 0) selectedPoint->radius = 0;
		}

		Clear(olc::BLACK);

		// main algorythm--------------------------------------------------------------------------------------------------
		unsigned int arrows = 0;
		bool* sign = new bool[pointsCount];
		for (int i = 0; i < pointsCount; i++) { sign[i] = 0; }
		int xSi = 0, xEi = 0, xSj = 0, xEj = 0;

		for (int i = 0; i < pointsCount; i++) {
			if (!sign[i]) {
				sign[i] = true;
				xSi = pointsMatrix[i].x - pointsMatrix[i].radius;
				xEi = pointsMatrix[i].x + pointsMatrix[i].radius;

				for (int j = i; j < pointsCount; j++) {
					if (!sign[j]) {
						xSj = pointsMatrix[j].x - pointsMatrix[j].radius;
						xEj = pointsMatrix[j].x + pointsMatrix[j].radius;

						if (xSi <= xSj && xSj <= xEi) {
							xSi = xSj;
							sign[j] = true;
						}
						if (xSi <= xEj && xEj <= xEi) 
						{
							xEi = xEj;
							sign[j] = true;
						}
						if (xSj <= xSi && xEi <= xEj) {
							sign[j] = true;
						}
					}
				}
			    FillRegion(xSi, 0, xEi, ScreenHeight(), 20, olc::DARK_RED);
				arrows++;
			}
		}
		delete[] sign;
		// --------------------------------------------------------------------------------------------------


		// render selection
		if (selectedPoint != NULL) {
			DrawCircle(selectedPoint->x, selectedPoint->y, selectedPoint->radius + 3, olc::DARK_GREEN);
		}

		// render points
		for (int i = 0; i < pointsCount; i++) {
			FillCircle(pointsMatrix[i].x, pointsMatrix[i].y, pointsMatrix[i].radius, olc::GREEN);
		}

		// render text
		if (selectedPoint != NULL) {
			std::string SelectedPointStr = std::to_string(selectedPoint->x) + "." + std::to_string(selectedPoint->y);
			int selectedPointStringX = selectedPoint->x + selectedPoint->radius + 2;
			int selectedPointStringY = selectedPoint->y - selectedPoint->radius/2 - 10;
			if ((selectedPointStringX + SelectedPointStr.length()*8) > ScreenWidth()) 
				selectedPointStringX = selectedPoint->x - selectedPoint->radius - SelectedPointStr.length()*8;
			if ((selectedPointStringY) < 0)
				selectedPointStringY = selectedPoint->y + selectedPoint->radius/2 + 2;
			DrawString(selectedPointStringX, selectedPointStringY, SelectedPointStr);
		}
		std::string Str = "Arrows:" + std::to_string(arrows);
		DrawString(3, 3, Str);
		Str = "FPS:" + std::to_string(GetFPS());
		DrawString(3, 13, Str);



		return true;
	}
};



int main() {

	Window Game;
	if (Game.Construct(400, 250, 4, 4, true))
		Game.Start();

	return 0;
}
