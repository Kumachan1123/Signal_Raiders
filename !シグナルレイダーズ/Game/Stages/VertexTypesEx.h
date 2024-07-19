#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

namespace DirectX
{
	struct VertexPositionNormalTextureTangent
	{
		VertexPositionNormalTextureTangent() = default;

		VertexPositionNormalTextureTangent(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& normal,
			const DirectX::XMFLOAT2& textureCoordinate,
			const DirectX::XMFLOAT4& tangent)
			: position(position),
			normal(normal),
			textureCoordinate(textureCoordinate),
			tangent(tangent) {}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 textureCoordinate;
		DirectX::XMFLOAT4 tangent;

		static const int InputElementCount = 4;
		static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
	};
}
