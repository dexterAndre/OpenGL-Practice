#pragma once

#include "Test.h"

namespace test
{
	class TestUniformColor : public Test
	{
	public:
		TestUniformColor();
		~TestUniformColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender();

	private:
		float m_UniformColor[4];
	};
}