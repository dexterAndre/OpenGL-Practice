#pragma once

#include "Test.h"

namespace test
{
	class TestCamera : public Test
	{
	public:
		TestCamera();
		~TestCamera();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender();

	private:
		float m_ClearColor[4];
	};
}
