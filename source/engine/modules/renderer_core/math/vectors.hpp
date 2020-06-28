#pragma once

namespace ray::renderer_core_api::math
{
	struct Vector2
	{
		Vector2(float x, float y) : _x(x), _y(y) {}
		Vector2(const Vector2& other) {}
		Vector2(Vector2&& other) {}

		float _x, _y;
	};

	struct Vector3
	{
		Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
		Vector3(const Vector3& other) {}
		Vector3(Vector3&& other) {}
		
		float _x, _y, _z;
	};

	struct Vector4
	{
		Vector4(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w) {}
		Vector4(const Vector4& other) {}
		Vector4(Vector4&& other) {}
		
		float _x, _y, _z, _w;
	};
}