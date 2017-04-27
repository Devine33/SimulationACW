#include "Direct_X.h"
#include <string>
#include "../Tracer/Trace.hpp"
#include <AntTweakBar.h>


Direct_X::Direct_X(): m_Factory(nullptr), m_Adapter(nullptr), m_AdapterOutput(nullptr), m_DisplayModeList(nullptr), m_NumModes(0), i(0), m_Numerator(0), m_Denominator(0), m_FeatureLevel()
{
}


Direct_X::~Direct_X()
{
}

//Starts the window and initializes all the Direct_X elements then starts the scene
void Direct_X::StartWindowing(int cmd, WNDPROC Wndproc)
{	//seperate window 
	window.WindowCreation(cmd,Wndproc);
	InitializeFactory(window.GetScreenWidth(), window.GetScreenHeight());
	InitializeSwapChain(window.GetScreenWidth(), window.GetScreenHeight(), window.GetHandle());
	InitializeResources(window.GetScreenWidth(), window.GetScreenHeight());
	BeginScene();
}

void Direct_X::CreateFeatureList()
{
	m_FeatureLevels.push_back(D3D_FEATURE_LEVEL_11_0);
	/*m_FeatureLevels.push_back(D3D_FEATURE_LEVEL_10_1);
	m_FeatureLevels.push_back(D3D_FEATURE_LEVEL_10_0);
	m_FeatureLevels.push_back(D3D_FEATURE_LEVEL_9_3);*/

	TRACE(L"Feature List Populate \n");
}
//Initializes Factory, Adapter & Output
void Direct_X::InitializeFactory(int ScreenWidth,int ScreenHeight)
{
	HRESULT result;
	CreateFeatureList();
	TRACE(L"Direct_X::InitializeFactory \n");

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(m_Factory.GetAddressOf()));
	if (FAILED(result))
	{
		TRACE(L"Test");
	}
	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = m_Factory->EnumAdapters(0, m_Adapter.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"Test");
	}
	result = m_Adapter->EnumOutputs(0, m_AdapterOutput.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"Test");
	}
	result = m_AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_NumModes, nullptr);
	if (FAILED(result))
	{
		TRACE(L"Test");
	}
	m_DisplayModeList = new DXGI_MODE_DESC[m_NumModes];
	if(!m_DisplayModeList)
	{		
		TRACE(L"Test");		
	}
	result = m_AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_NumModes, m_DisplayModeList);
	if (FAILED(result))
	{
		TRACE(L"Test");
	}
	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < m_NumModes; i++)
	{
		if (m_DisplayModeList[i].Width == (ScreenWidth))
		{
			if (m_DisplayModeList[i].Height == static_cast<unsigned int>(ScreenHeight))
			{			
				m_Numerator = m_DisplayModeList[i].RefreshRate.Numerator;
				m_Denominator = m_DisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	

	delete[] m_DisplayModeList;
	
	//Factory Is Finished
}
//Initializes the device and swapchain - you can change these to seperate the two.
void Direct_X::InitializeSwapChain(int ScreenWidth, int ScreenHeight,HWND hwnd)
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	// If the project is in a debug build, enable the debug layer.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	TRACE(L"SwapChain Started \n");
	ZeroMemory(m_SwapChain.GetAddressOf(), sizeof(m_SwapChainDesc));
	//BufferDesc
	m_SwapChainDesc.BufferDesc.Width = ScreenWidth;
	m_SwapChainDesc.BufferDesc.Height = ScreenHeight;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//DXGI_FORMAT_R8G8B8A8_UNORM << only works with this one find out something about this
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_R32G32B32A32_UINT;
	m_SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	m_SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//SamplerDesc
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.SampleDesc.Quality = 0;
	//BufferUsage
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//BufferCount
	m_SwapChainDesc.BufferCount = 1;
	//Output Window
	m_SwapChainDesc.OutputWindow = hwnd;
	//Windowed allow fullscreen later
	m_SwapChainDesc.Windowed = true;
	//DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL causes crash
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	m_SwapChainDesc.Flags = 0;

	//SORT FULLSCREEN LATER
	
	HRESULT result;
	//D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_11_0;
	//cycle the features list later
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, //adapter , drivertype,software
									0,&m_FeatureLevels.front() , m_FeatureLevels.size(), //flags ,*pfeature featurelevels,
									D3D11_SDK_VERSION, &m_SwapChainDesc, m_SwapChain.GetAddressOf(), //sdkversion *pSwapChaindesc,**swapchain
									m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf()); 
	
	if (FAILED(result))
	{
		TRACE(L"ASDSA");
	}

	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_SwapChainBuffer.GetAddressOf()));
	if (FAILED(result))
	{
		TRACE(L"m_SwapChain->GetBuffer::FAIL");
	}
	TRACE(L"swapchain ended \n");
}

void Direct_X::InitializeResources(int ScreenWidth, int ScreenHeight)
{
	TRACE(L"Resource Initialization Started \n");
	//Acquire texture interface from swapchain
	
	HRESULT result;

	//createa a rendering target view to allow binding the swapchain teture to the pipeline for rendering
	result  = m_Device->CreateRenderTargetView(m_SwapChainBuffer.Get(), nullptr, m_RenderTarget.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"FAIL");
	}
	// Release pointer to the back buffer as we no longer need it.
	
	
	ZeroMemory(&m_DepthDesc, sizeof(D3D11_TEXTURE2D_DESC));
	m_DepthDesc.Width = ScreenWidth;
	m_DepthDesc.Height = ScreenHeight;
	m_DepthDesc.MipLevels = 1;
	m_DepthDesc.ArraySize = 1;
	//DXGI_FORMAT_D32_FLOAT see multi-pass rendering 2 real-time also DXGI_FORMAT_D24_UNORM_S8_UINT //r32_typess for shader and depthstencil resource
	m_DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_D32_FLOAT;//DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_DepthDesc.SampleDesc.Count = 1;
	m_DepthDesc.SampleDesc.Quality = 0;
	//d3d11_usage_default could be an option
	//D3D11 USAGE DYNAMIC CAUSES CRASH
	m_DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	m_DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_DepthDesc.CPUAccessFlags = 0;
	m_DepthDesc.MiscFlags = 0;

	result = m_Device->CreateTexture2D(&m_DepthDesc, nullptr, m_DepthStencilBuffer.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"ASDSA");
	}
	
	m_DepthStencilBufferDescription.DepthEnable = true;
	m_DepthStencilBufferDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_DepthStencilBufferDescription.DepthFunc = D3D11_COMPARISON_LESS;
	m_DepthStencilBufferDescription.StencilEnable = true;
	m_DepthStencilBufferDescription.StencilReadMask = 0xFF;
	m_DepthStencilBufferDescription.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	m_DepthStencilBufferDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencilBufferDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	m_DepthStencilBufferDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencilBufferDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	m_DepthStencilBufferDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencilBufferDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	m_DepthStencilBufferDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_DepthStencilBufferDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_Device.Get()->CreateDepthStencilState(&m_DepthStencilBufferDescription, m_DepthStencilState.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"ASDSA");
	}
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
	

	//
	

	//DXGI_FORMAT_D32_FLOAT_S8X24_UINT
	ZeroMemory(&m_DepthStencilViewDesc, sizeof(m_DepthStencilViewDesc));
	m_DepthStencilViewDesc.Format = m_DepthDesc.Format;
	m_DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_DepthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_Device.Get()->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &m_DepthStencilViewDesc, m_DepthStencilView.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"ASDSA");
	}
	m_DeviceContext.Get()->OMSetRenderTargets(1, m_RenderTarget.GetAddressOf(), m_DepthStencilView.Get());

	m_Rasterizer = {};
	m_Rasterizer.FillMode = D3D11_FILL_SOLID;
	m_Rasterizer.CullMode = D3D11_CULL_NONE;
	m_Rasterizer.FrontCounterClockwise = false;
	m_Rasterizer.DepthBias = 0;
	m_Rasterizer.SlopeScaledDepthBias = 0.0f;
	m_Rasterizer.DepthBiasClamp = 0.0f;
	m_Rasterizer.DepthClipEnable = true;
	m_Rasterizer.ScissorEnable = false;
	m_Rasterizer.MultisampleEnable = false;
	m_Rasterizer.AntialiasedLineEnable = false;

	result = m_Device.Get()->CreateRasterizerState(&m_Rasterizer, m_RasterizerState.GetAddressOf());
	if (FAILED(result))
	{
		TRACE(L"failed");
	}

	m_DeviceContext.Get()->RSSetState(m_RasterizerState.Get());

	
	m_Viewport.Width = static_cast<float>(m_DepthDesc.Width);
	m_Viewport.Height = static_cast<float>(m_DepthDesc.Height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;

	m_DeviceContext->RSSetViewports(1, &m_Viewport);
	//attempt to break this up even more and SETUP
	TRACE(L"Resource Initialization Finished \n");
}

//ID3D11ShaderResourceView* Direct_X::CreateShaderResourceView(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc)
//{
//	ID3D11ShaderResourceView* pView = nullptr;
//	HRESULT hr = m_Device.Get()->CreateShaderResourceView(pResource, pDesc, &pView);
//}
//Move ALL PARTS OF INITIALIZATION IN HERE AND CALL 
void Direct_X::DirectXInitialize(const HWND hwnd, const int ScreenWidth, const int ScreenHeight, const bool Fullscreen)
{
	/*HRESULT Result = S_OK;*/
}

void Direct_X::BeginScene() const
{
	std::vector<float> colour(4);
	float color[4];
	const float red = (0.1 * 0.5) * 0.25 + 0.5f;
	const float green = 1.0;
	const float blue = 1.0f;
	const float alpha = 1.0f;
	
	// Setup the color to clear the buffer to.
	colour.push_back(red);
	colour.push_back(green);
	colour.push_back(blue);
	colour.push_back(alpha);

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	// Clear the back buffer.
	m_DeviceContext.Get()->ClearRenderTargetView(m_RenderTarget.Get(),color);
	
	// Clear the depth buffer.
	m_DeviceContext.Get()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct_X::EndScene() const
{	
	// Present the back buffer to the screen since rendering is complete
	// Lock to screen refresh rate.
	m_SwapChain.Get()->Present(1, 0);
}

ID3D11Device* Direct_X::GetDevice() const
{
	return m_Device.Get();
}

ID3D11DeviceContext* Direct_X::GetDeviceContext() const
{
	return m_DeviceContext.Get();
}

HWND Direct_X::GetHandle() const
{
	return window.GetHandle();
}

float Direct_X::GetScreenWidth() const
{
	return window.GetScreenWidth();
}

 float Direct_X::GetScreenHeight() const
{
	return window.GetScreenHeight();
}
