#pragma once
#include "Windowing.h"
#include <d3d11.h>
#include <vector>
#include <wrl/client.h>


//what can i abstract from this?
//feature list of D3Dmodes e.g. create feature list
//Will Initialize Direct_X and Associate with Rendering
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
class Direct_X
{

public:
	Direct_X();
	~Direct_X();
	void StartWindowing(int cmd);
	void CreateFeatureList();
	void InitializeFactory(int ScreenWidth, int ScreenHeight);
	void InitializeSwapChain(int ScreenWidth,int ScreenHeight,HWND hwnd);
	void InitializeResources(int ScreenWidth, int ScreenHeight);
	//ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc);

	void DirectXInitialize(const HWND hwnd, const int ScreenWidth, const int ScreenHeight, const bool Fullscreen);

	////Move these to graphicsEngine Maybe
	void BeginScene() const;
	void EndScene() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	 float GetScreenWidth() const;
	 float GetScreenHeight() const;
private:
	
	Windowing window;
	std::vector<D3D_FEATURE_LEVEL> m_FeatureLevels;
#pragma region Factory,Adapter & Output
	Microsoft::WRL::ComPtr<IDXGIFactory>					m_Factory;
	Microsoft::WRL::ComPtr<IDXGIAdapter>					m_Adapter;
	Microsoft::WRL::ComPtr<IDXGIOutput>						m_AdapterOutput;
	DXGI_MODE_DESC*											m_DisplayModeList;
	/*Microsoft::WRL::ComPtr<DXGI_MODE_DESC> m_DisplayModeList;*/
#pragma endregion 
	
#pragma region SwapChain,DeviceContext,Device
	unsigned int											m_NumModes, i, m_Numerator, m_Denominator;
	DXGI_SWAP_CHAIN_DESC									m_SwapChainDesc;
	D3D_FEATURE_LEVEL										m_FeatureLevel;
	Microsoft::WRL::ComPtr<IDXGISwapChain>					m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>					m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>				m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGIAdapter>					M_Adapter;
#pragma endregion 

#pragma region Resources
	D3D11_RENDER_TARGET_VIEW_DESC							m_RenderTargetViewDesc;
	Microsoft::WRL::ComPtr < ID3D11RenderTargetView>		m_RenderTarget;

	Microsoft::WRL::ComPtr < ID3D11Texture2D>				m_SwapChainBuffer;

#pragma region Depth
	D3D11_TEXTURE2D_DESC									m_DepthDesc;
	Microsoft::WRL::ComPtr < ID3D11Texture2D>				m_DepthStencilBuffer;
	D3D11_DEPTH_STENCIL_DESC								m_DepthStencilBufferDescription;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState>		m_DepthStencilState;
	D3D11_DEPTH_STENCIL_VIEW_DESC							m_DepthStencilViewDesc;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView>			m_DepthStencilView;
#pragma endregion 

#pragma region Rasterizer
	D3D11_RASTERIZER_DESC m_Rasterizer;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState>			m_RasterizerState;
#pragma endregion 
#pragma region ViewPort

	D3D11_VIEWPORT											m_Viewport;
#pragma endregion 
	
#pragma endregion 

};

