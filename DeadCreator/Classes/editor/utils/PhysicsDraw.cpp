#define _ENABLE_ATOMIC_ALIGNMENT_FIX

#include "PhysicsDraw.h"
#include "Game.hpp"
#include "SimulatorLayer.hpp"
#include "DummyScene.hpp"
#include "RenderingSystem.hpp"

using namespace cocos2d;
using namespace realtrick;
using namespace realtrick::editor;
using namespace realtrick::client;

std::unique_ptr<PhysicsDraw> PhysicsDraw::instance = nullptr;

void PhysicsDraw::staticInit(SimulatorLayer* simulatorLayer)
{
	instance.reset(new PhysicsDraw(simulatorLayer));
}


PhysicsDraw::PhysicsDraw(SimulatorLayer* simulatorLayer)
	:_simulatorLayer(simulatorLayer)
{}


PhysicsDraw::~PhysicsDraw()
{}


/// Draw a closed polygon provided in CCW order.
void PhysicsDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	auto origin = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
	auto drawList = ImGui::GetWindowDrawList();

	for (size_t i = 0; i<vertexCount; i++)
	{
		size_t j = (i + 1) % vertexCount;

		auto a = _simulatorLayer->worldToLocal(origin, toVec(vertices[i]));
		auto b = _simulatorLayer->worldToLocal(origin, toVec(vertices[j]));
		drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 0.0, 0.5)));
	}
}

/// Draw a solid closed polygon provided in CCW order.
void PhysicsDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);
}

/// Draw a circle.
void PhysicsDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	auto origin = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
	auto drawList = ImGui::GetWindowDrawList();
	auto rad = radius * _simulatorLayer->getGameLayer()->getGame()->getRenderingSysetm()->getZoomScale().x;
	auto pos = _simulatorLayer->worldToLocal(origin, toVec(center));
	drawList->AddCircle(ImVec2(pos.x, pos.y), rad, ImColor(ImVec4(1.0, 0.0, 1.0, 0.7)), 20, 0.0f);
}

/// Draw a solid circle.
void PhysicsDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	DrawCircle(center, radius, color);
}

/// Draw a line segment.
void PhysicsDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	auto origin = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
	auto drawList = ImGui::GetWindowDrawList();
	auto a = _simulatorLayer->worldToLocal(origin, toVec(p1));
	auto b = _simulatorLayer->worldToLocal(origin, toVec(p2));
	drawList->AddLine(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImColor(ImVec4(1.0, 1.0, 0.0, 0.5)));
}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void PhysicsDraw::DrawTransform(const b2Transform& xf)
{}

/// Draw a point.
void PhysicsDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	auto origin = Vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
	auto drawList = ImGui::GetWindowDrawList();
	Vec2 rectOrigin = _simulatorLayer->worldToLocal(origin, toVec(p));
	Vec2 rectDest = _simulatorLayer->worldToLocal(origin, Vec2(p.x + size, p.y + size));

	drawList->AddRectFilled(ImVec2(rectOrigin.x, rectOrigin.y),
		ImVec2(rectDest.x, rectDest.y),
		ImColor(ImVec4(0.2, 0.2, 0.8, 0.2)));
}