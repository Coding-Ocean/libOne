#include <d3d11.h>
#include"common.h"
#include"window.h"
#include"graphic.h"

static float BaseWidth = 0;
static float BaseHeight = 0;
D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* Dev = 0;
ID3D11DeviceContext* ImmediateContext = 0;
IDXGISwapChain* SwapChain = 0;
ID3D11RenderTargetView* RenderTargetView = 0;
ID3D11DepthStencilView* DepthStencilView = 0;
ID3D11DepthStencilState* DepthStencilState = 0;

void initGraphic(int baseWidth, int baseHeight, bool windowed) {
    //基準となる幅と高さ
    BaseWidth = (float)baseWidth;
    BaseHeight = (float)baseHeight;

    HRESULT hr = S_OK;

    // Create Direct3D device and swap chain
    HWND hWnd = FindWindow(CLASS_NAME, 0);

    UINT createDeviceFlags = 0;

    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = ClientWidth;
    swapChainDesc.BufferDesc.Height = ClientHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = windowed;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        hr = D3D11CreateDeviceAndSwapChain(
            NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &Dev, &FeatureLevel, &ImmediateContext);
        if (SUCCEEDED(hr)) {
            break;
        }
    }
    WARNING(FAILED(hr), "グラフィックデバイスが作れません", "");

    createRenderTarget();

    // Setup the viewport
    setViewport();
}

void createRenderTarget() {
    // Create a render target view
    ID3D11Texture2D* backBuffer = NULL;
    HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    WARNING(FAILED(hr), "can't Get BackBuffer", "");
    hr = Dev->CreateRenderTargetView(backBuffer, NULL, &RenderTargetView);
    backBuffer->Release();
    WARNING(FAILED(hr), "can't Create RenderTargetView", "");

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
    ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
    depthStencilTextureDesc.Width = ClientWidth;
    depthStencilTextureDesc.Height = ClientHeight;
    depthStencilTextureDesc.MipLevels = 1;
    depthStencilTextureDesc.ArraySize = 1;
    depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilTextureDesc.SampleDesc.Count = 1;
    depthStencilTextureDesc.SampleDesc.Quality = 0;
    depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilTextureDesc.CPUAccessFlags = 0;
    depthStencilTextureDesc.MiscFlags = 0;
    ID3D11Texture2D* depthStencilTexture = 0;
    hr = Dev->CreateTexture2D(&depthStencilTextureDesc, NULL, &depthStencilTexture);
    WARNING(FAILED(hr), "can't Create depth stencil texture", "");

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = depthStencilTextureDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    hr = Dev->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &DepthStencilView);
    WARNING(FAILED(hr), "can't Create DepthStencilView", "");
    SAFE_RELEASE(depthStencilTexture);

    // Set RenderTarget
    ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable = FALSE;
    hr = Dev->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
    WARNING(FAILED(hr), "Error", "");
    ImmediateContext->OMSetDepthStencilState(DepthStencilState, 0);
}

void setViewport() {
    D3D11_VIEWPORT vp;
    float aspect = BaseWidth / BaseHeight;
    if ((float)ClientWidth / ClientHeight >= aspect) {
        float width = ClientHeight * aspect;
        float left = (ClientWidth - width) / 2.0f;
        vp.Width = width;
        vp.Height = (float)ClientHeight;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = left;
        vp.TopLeftY = 0;
        ImmediateContext->RSSetViewports(1, &vp);
    }
    else {
        float height = ClientWidth / aspect;
        float top = (ClientHeight - height) / 2.0f;
        vp.Width = (float)ClientWidth;
        vp.Height = height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = top;
        ImmediateContext->RSSetViewports(1, &vp);
    }
}

void changeSize() {
    if (ImmediateContext) {
        ID3D11RenderTargetView* renderTargetView = NULL;
        ImmediateContext->OMSetRenderTargets(1, &renderTargetView, NULL);
        SAFE_RELEASE(RenderTargetView);
        SAFE_RELEASE(DepthStencilView);
        SAFE_RELEASE(DepthStencilState);

        SwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

        createRenderTarget();
        setViewport();
    }
}

void freeGraphic() {
    //if (Images) {
    //    for (unsigned i = 0; i < Images->size(); i++) {
    //        SAFE_RELEASE(Images->at(i).vertexBuffer);
    //        if (!Images->at(i).dividedImageFlag) {
    //            SAFE_RELEASE(Images->at(i).texture);
    //        }
    //    }
    //    SAFE_DELETE(Images);
    //}

    //SAFE_DELETE(UltraBasicImg);
    //SAFE_DELETE(FontMap);
    //SAFE_RELEASE(VertexBuffer2);
    //SAFE_RELEASE(BlendState);
    //SAFE_RELEASE(RasterizerState);
    //SAFE_RELEASE(SamplerLinear);
    //SAFE_RELEASE(CBDiffuse);
    //SAFE_RELEASE(CBWorld);

    SAFE_RELEASE(DepthStencilState);
    SAFE_RELEASE(DepthStencilView);
    SAFE_RELEASE(RenderTargetView);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(ImmediateContext);
    SAFE_RELEASE(Dev);
}

void clearTarget(const COLOR& c) {
    // Clear the back buffer
    float clearColor[4] = { c.r, c.g, c.b, c.a };
    ImmediateContext->ClearRenderTargetView(RenderTargetView, clearColor);
    // Clear the depth buffer to 1.0 (max depth)
    ImmediateContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void present() {
    SwapChain->Present(1, 0);
}