#include<cmath>
#include<d3d11.h>
#include"common.h"
#include"window.h"
#include"graphic.h"
#include"MAT.h"
void createDepthStencielState();
void createBlendState();
void createRasterizerState();
void createConstantBuffer();
void createShaderFromRes();
void createRectVertexPosBuffer();
void createCircleVertexPosBuffer();

static float BaseWidth = 0;
static float BaseHeight = 0;
D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* Dev = 0;
ID3D11DeviceContext* ImmediateContext = 0;
IDXGISwapChain* SwapChain = 0;
ID3D11RenderTargetView* RenderTargetView = 0;
ID3D11DepthStencilView* DepthStencilView = 0;
ID3D11DepthStencilState* DepthStencilState = 0;
//
ID3D11BlendState* BlendState = NULL;
ID3D11RasterizerState* RasterizerState = NULL;
//line,rect,circle用シェーダ
ID3D11VertexShader* ShapeVertexShader = NULL;
ID3D11InputLayout* ShapeVertexLayout = NULL;
ID3D11PixelShader* ShapePixelShader = NULL;
//image,text用シェーダ
ID3D11VertexShader* ImageVertexShader = NULL;
ID3D11InputLayout* ImageVertexLayout = NULL;
ID3D11PixelShader* ImagePixelShader = NULL;
ID3D11SamplerState* SamplerLinear = NULL;
//シェーダ用コンスタントバッファオブジェクト
ID3D11Buffer* CbWorld = NULL;
ID3D11Buffer* CbProj = NULL;
ID3D11Buffer* CbColor = NULL;
//頂点オブジェクト
ID3D11Buffer* RectVertexPosBuffer = NULL;
ID3D11Buffer* CircleVertexPosBuffer = NULL;
ID3D11Buffer* TexCoordBuffer = NULL;

MAT Proj;
MAT World;
COLOR StrokeColor(0.0f, 0.0f, 0.0f, 1.0f);
COLOR FillColor(1.0f, 1.0f, 1.0f, 1.0f);
COLOR MeshColor(1.0f, 1.0f, 1.0f, 1.0f);
float StrokeWeight = 1;
int TextSize = 20;

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
    //SAFE_RELEASE(SamplerLinear);
    //SAFE_RELEASE(TexCoordBuffer);

    SAFE_RELEASE(RectVertexPosBuffer);
    SAFE_RELEASE(CircleVertexPosBuffer);
    SAFE_RELEASE(CbColor);
    SAFE_RELEASE(CbWorld);
    SAFE_RELEASE(CbProj);

    SAFE_RELEASE(ShapeVertexShader);
    SAFE_RELEASE(ShapeVertexLayout);
    SAFE_RELEASE(ShapePixelShader);
    SAFE_RELEASE(ImageVertexShader);
    SAFE_RELEASE(ImageVertexLayout);
    SAFE_RELEASE(ImagePixelShader);

    SAFE_RELEASE(BlendState);
    SAFE_RELEASE(RasterizerState);
    SAFE_RELEASE(DepthStencilState);
    SAFE_RELEASE(DepthStencilView);
    SAFE_RELEASE(RenderTargetView);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(ImmediateContext);
    SAFE_RELEASE(Dev);
}

void initGraphic(int baseWidth, int baseHeight) {
    //基準となる幅と高さ
    BaseWidth = (float)baseWidth;
    BaseHeight = (float)baseHeight;
    
    createDevice();
    createRenderTarget();
    setViewport();

    createDepthStencielState();
    createBlendState();
    createRasterizerState();

    createShaderFromRes();
    createConstantBuffer();

    createCircleVertexPosBuffer();
    createRectVertexPosBuffer();
}

void createDevice() {
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
    swapChainDesc.Windowed = true;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        hr = D3D11CreateDeviceAndSwapChain(
            NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &Dev, &FeatureLevel, &ImmediateContext);
        if (SUCCEEDED(hr)) {
            break;
        }
    }
    WARNING(FAILED(hr), "グラフィックデバイスが作れません", "");
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

        SwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        createRenderTarget();
        setViewport();
    }
}

void clear(float* clearColor){
    // Clear the back buffer
    ImmediateContext->ClearRenderTargetView(RenderTargetView, clearColor);
    // Clear the depth buffer to 1.0 (max depth)
    ImmediateContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void clear(const COLOR& c) {
    // Clear the back buffer
    float clearColor[4] = { c.r, c.g, c.b, c.a };
    clear(clearColor);
}
void clear(float r, float g, float b) {
    // Clear the back buffer
    float clearColor[4] = { r/255, g/255, b/255, 1.0f };
    clear(clearColor);
}

void present() {
    SwapChain->Present(1, 0);
}

void createDepthStencielState(){
    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable = FALSE;
    HRESULT hr = Dev->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
    WARNING(FAILED(hr), "Error", "");
    ImmediateContext->OMSetDepthStencilState(DepthStencilState, 0);
}
void createBlendState() {
    //ブレンディングステート生成
    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    for (int i = 0; i < 1; i++) {
        blendDesc.RenderTarget[i].BlendEnable = TRUE;
        blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    Dev->CreateBlendState(&blendDesc, &BlendState);
    float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
    ImmediateContext->OMSetBlendState(BlendState, blendFactor, 0xffffffff);
}
void createRasterizerState() {
    D3D11_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    //rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = TRUE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    Dev->CreateRasterizerState(&rasterizerDesc, &RasterizerState);
    ImmediateContext->RSSetState(RasterizerState);
}

void createConstantBuffer()
{
    HRESULT hr;

    // Create the constant buffers
    // CbWorld
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.ByteWidth = sizeof(MAT);
    hr = Dev->CreateBuffer(&bd, NULL, &CbWorld);
    WARNING(FAILED(hr), "Create Constant Buffer CbWorld","");
    // CbProj
    hr = Dev->CreateBuffer(&bd, NULL, &CbProj);
    WARNING(FAILED(hr), "Create Constant Buffer Proj","");
    // CbColor
    bd.ByteWidth = sizeof(COLOR);
    hr = Dev->CreateBuffer(&bd, NULL, &CbColor);
    WARNING(FAILED(hr), "Create Constant Buffer CbColor","");

    // Set Proj to constant buffer
    Proj.identity();
    Proj.ortho(0, BaseWidth, BaseHeight, 0, -1, 1);
    ImmediateContext->UpdateSubresource(CbProj, 0, NULL, &Proj, 0, 0);
    ImmediateContext->VSSetConstantBuffers(2, 1, &CbProj);
}
enum class VERTEX_TYPE { PNT, PNTWW, PT, P };
void createVertexShaderAndInputLayoutFromRes(
    LPCTSTR resName,
    ID3D11VertexShader** vertexShader,
    VERTEX_TYPE vertexType,
    ID3D11InputLayout** inputLayout
) {
    HINSTANCE hInst = GetModuleHandle(0);
    HRSRC hRes = FindResource(hInst, resName, _T("SHADER"));
    WARNING(hRes == 0, "リソースがみつからない","");
    // リソースのサイズを取得する 
    DWORD sizeOfRes = SizeofResource(hInst, hRes);
    WARNING(sizeOfRes == 0, "リソースのサイズがゼロ","");
    // 取得したリソースをメモリにロードする
    HGLOBAL HMem = LoadResource(hInst, hRes);
    WARNING(HMem == 0, "リソースがロードできない","");
    // ロードしたリソースデータのアドレスを取得する
    unsigned char* mem = (unsigned char*)LockResource(HMem);
    if (mem == 0) {
        FreeResource(HMem);
        WARNING(true, "リソースのアドレスが取得できない", "");
    }
    //vertexShaderをつくる
    HRESULT hr = Dev->CreateVertexShader(mem, sizeOfRes, NULL, vertexShader);
    WARNING(FAILED(hr), "cannot CreateVertexShader", "");

    switch (vertexType) {
    case VERTEX_TYPE::PNT: {
        //インプットレイアウト
        D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        };
        UINT numElements = ARRAYSIZE(inputElementDesc);
        hr = Dev->CreateInputLayout(inputElementDesc, numElements, mem, sizeOfRes, inputLayout);
        break; }
    case VERTEX_TYPE::PNTWW: {
        //スキンインプットレイアウト
        D3D11_INPUT_ELEMENT_DESC inputElementDescSkin[] = {
            {"POSITION",    0,DXGI_FORMAT_R32G32B32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"NORMAL",      0,DXGI_FORMAT_R32G32B32_FLOAT,   0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",    0,DXGI_FORMAT_R32G32_FLOAT,      0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"BLENDINDICES",0,DXGI_FORMAT_R16G16B16A16_SINT, 0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"BLENDWEIGHT", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        };
        UINT numElementsSkin = ARRAYSIZE(inputElementDescSkin);
        hr = Dev->CreateInputLayout(inputElementDescSkin, numElementsSkin, mem, sizeOfRes, inputLayout);
        break; }
    case VERTEX_TYPE::PT: {
        //イメージインプットレイアウト生成
        D3D11_INPUT_ELEMENT_DESC inputElementDescImage[] = {
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   1,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        };
        UINT numElementsImage = ARRAYSIZE(inputElementDescImage);
        hr = Dev->CreateInputLayout(inputElementDescImage, numElementsImage, mem, sizeOfRes, inputLayout);
        break; }
    case VERTEX_TYPE::P: {
        //シェイプインプットレイアウト生成
        D3D11_INPUT_ELEMENT_DESC inputElementDescLine[] = {
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        };
        UINT numElementsLine = ARRAYSIZE(inputElementDescLine);
        hr = Dev->CreateInputLayout(inputElementDescLine, numElementsLine, mem, sizeOfRes, inputLayout);
        break; }
    }
    WARNING(FAILED(hr), "InputLayoutを生成できません", "");
    //開放
    FreeResource(HMem);
}

void createPixelShaderFromRes(LPCTSTR resName, ID3D11PixelShader** pixelShader) {
    HINSTANCE hInst = GetModuleHandle(0);
    HRSRC hRes = FindResource(hInst, resName, _T("SHADER"));
    WARNING(hRes == 0, "リソースがみつからない", "PixcelShader");
    // リソースのサイズを取得する 
    DWORD sizeOfRes = SizeofResource(hInst, hRes);
    WARNING(sizeOfRes == 0, "リソースのサイズがゼロ", "PixcelShader");
    // 取得したリソースをメモリにロードする
    HGLOBAL hMem = LoadResource(hInst, hRes);
    WARNING(hMem == 0, "ビットマップリソースがロードできない", "PixcelShader");
    // ロードしたリソースデータのアドレスを取得する
    unsigned char* mem = (unsigned char*)LockResource(hMem);
    if (mem == 0) {
        FreeResource(hMem);
        WARNING(true, "リソースのアドレスが取得できない", "PixcelShader");
    }
    //pixelShaderをつくる
    HRESULT hr = Dev->CreatePixelShader(mem, sizeOfRes, NULL, pixelShader);
    WARNING(FAILED(hr), "PixelShaderを生成できません", "PixcelShader");
    //開放
    FreeResource(hMem);
}

void createShaderFromRes()
{
    createVertexShaderAndInputLayoutFromRes(_T("SHAPE_VS"), &ShapeVertexShader, VERTEX_TYPE::P, &ShapeVertexLayout);
    createPixelShaderFromRes(_T("SHAPE_PS"), &ShapePixelShader);
    createVertexShaderAndInputLayoutFromRes(_T("IMAGE_VS"), &ImageVertexShader, VERTEX_TYPE::PT, &ImageVertexLayout);
    createPixelShaderFromRes(_T("IMAGE_PS"), &ImagePixelShader);
}

struct VERTEX_P {
    VECTOR3 pos;
};
void createRectVertexPosBuffer()
{
    //vertex for rect or line
    //rectと兼用だがline用に最適化した位置を設定する
    VERTEX_P vertices[] =  {
        VECTOR3(0.0f,  0.5f, 0.0f),
        VECTOR3(0.0f, -0.5f, 0.0f),
        VECTOR3(1.0f,  0.5f, 0.0f),
        VECTOR3(1.0f, -0.5f, 0.0f),
    };
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_P) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HRESULT hr = Dev->CreateBuffer(&bd, &InitData, &RectVertexPosBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Rect","");
}
void createCircleVertexPosBuffer()
{
    const int num = 64;
    float rad = 3.141592f * 2 / num;
    VERTEX_P circleVertices[num];
    circleVertices[0].pos = VECTOR3(0.5f, 0, 0.0f);
    for (int i = 1, j = 1; i <= num / 2 - 1; i++){
        circleVertices[j++].pos = VECTOR3(cosf(rad * i) * 0.5f, sinf(rad * i) * 0.5f, 0.0f);
        circleVertices[j++].pos = VECTOR3(cosf(rad * i) * 0.5f, -sinf(rad * i) * 0.5f, 0.0f);
    }
    circleVertices[num - 1].pos = VECTOR3(-0.5f, 0, 0.0f);
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_P) * (num);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = circleVertices;
    HRESULT hr = Dev->CreateBuffer(&bd, &InitData, &CircleVertexPosBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Circle","");
}
void stroke(float r, float g, float b, float a)
{
    StrokeColor.r = r / 255;
    StrokeColor.g = g / 255;
    StrokeColor.b = b / 255;
    StrokeColor.a = a / 255;
}
void fill(float r, float g, float b, float a)
{
    FillColor.r = r / 255;
    FillColor.g = g / 255;
    FillColor.b = b / 255;
    FillColor.a = a / 255;
}
void meshColor(float r, float g, float b, float a)
{
    MeshColor.r = r / 255;
    MeshColor.g = g / 255;
    MeshColor.b = b / 255;
    MeshColor.a = a / 255;
}
void strokeWeight(float weight)
{
    StrokeWeight = weight;
}
void line(float sx, float sy, float ex, float ey)
{
    float dx = ex - sx;
    float dy = ey - sy;
    float length = sqrt(dx * dx + dy * dy);
    float r = atan2(dy, dx);
    World.identity();
    World.mulTranslate(sx, sy, 0);
    World.mulRotateZ(r);
    World.mulScale(length, StrokeWeight, 1);

    // Update constant buffer
    ImmediateContext->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    ImmediateContext->UpdateSubresource(CbColor, 0, NULL, &StrokeColor, 0, 0);
    ImmediateContext->VSSetConstantBuffers(0, 1, &CbWorld);
    ImmediateContext->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P), offset = 0;
    ImmediateContext->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    ImmediateContext->IASetInputLayout(ShapeVertexLayout);
    ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
    ImmediateContext->VSSetShader(ShapeVertexShader, NULL, 0);
    ImmediateContext->PSSetShader(ShapePixelShader, NULL, 0);
    ImmediateContext->Draw(4, 0);

    point(sx, sy);
    point(ex, ey);
}
void point(float x, float y)
{
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulScale(StrokeWeight, StrokeWeight, 1);

    // Update variables that change once per frame
    ImmediateContext->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    ImmediateContext->UpdateSubresource(CbColor, 0, NULL, &StrokeColor, 0, 0);
    ImmediateContext->VSSetConstantBuffers(0, 1, &CbWorld);
    ImmediateContext->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P), offset = 0;
    ImmediateContext->IASetVertexBuffers(0, 1, &CircleVertexPosBuffer, &stride, &offset);
    ImmediateContext->IASetInputLayout(ShapeVertexLayout);

    ImmediateContext->VSSetShader(ShapeVertexShader, NULL, 0);
    ImmediateContext->PSSetShader(ShapePixelShader, NULL, 0);
    ImmediateContext->Draw(64, 0);
}