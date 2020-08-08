#include<vector>
#include<cmath>
#include<d3d11.h>
#include"common.h"
#include"window.h"
#include"MAT.h"
#include"VECTOR2.h"
#include"stb_image_reader.h"
#include"graphic.h"
void createDevice();
void createRenderTarget();
void setViewport();
void createDepthStencielState();
void createBlendState();
void createRasterizerState();
void createSamplerState();
void createConstantBuffer();
void createShaderFromRes();
void createRectVertexPosBuffer();
void createCircleVertexPosBuffer();
void createTexCoordBuffer();

extern float Width = 0;
extern float Height = 0;
DXGI_SAMPLE_DESC MSAA = { 0,0 };
ID3D11Device* Device = 0;
ID3D11DeviceContext* Context = 0;
IDXGISwapChain* SwapChain = 0;
ID3D11RenderTargetView* RenderTargetView = 0;
ID3D11DepthStencilView* DepthStencilView = 0;
//
ID3D11DepthStencilState* DepthStencilState = 0;
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

struct VERTEX_T {
    VECTOR2 texCoord;
};
struct TEXTURE {
    ID3D11ShaderResourceView* obj;
    ID3D11Buffer* texCoord;
    float width;
    float height;
    TEXTURE(ID3D11ShaderResourceView* obj, float width, float height, ID3D11Buffer* texCoord)
    {
        this->obj = obj; this->width = width; this->height = height; this->texCoord = texCoord;
    }
};
std::vector<TEXTURE>* Textures = NULL;

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
    SAFE_RELEASE(SamplerLinear);
    SAFE_RELEASE(TexCoordBuffer);
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
    SAFE_RELEASE(Context);
    SAFE_RELEASE(Device);
}

void initGraphic(int baseWidth, int baseHeight) {
    //基準となる幅と高さ
    Width = (float)baseWidth;
    Height = (float)baseHeight;
    
    createDevice();
    createRenderTarget();
    setViewport();

    createDepthStencielState();
    createBlendState();
    createRasterizerState();
    createSamplerState();

    createShaderFromRes();
    createConstantBuffer();

    createCircleVertexPosBuffer();
    createRectVertexPosBuffer();
    createTexCoordBuffer();

    Textures = new std::vector<TEXTURE>;

}


void createDevice() {
    //マルチサンプリング対応
    //デバイスの生成
    HRESULT hr;
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
        0, NULL, 0, D3D11_SDK_VERSION, &Device, NULL, &Context);
    WARNING(FAILED(hr), "D3D11CreateDevice","");

    //使用可能なMSAAを取得
    for (int i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++) {
        UINT Quality;
        if (SUCCEEDED( Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality) ) ){
            if (0 < Quality) {
                MSAA.Count = i;
                MSAA.Quality = Quality - 1;
            }
        }
    }

    //インターフェース取得
    IDXGIDevice1* hpDXGI = NULL;
    hr = Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI);
    WARNING(FAILED(hr), "QueryInterface","");
    //アダプター取得
    IDXGIAdapter* hpAdapter = NULL;
    hr = hpDXGI->GetAdapter(&hpAdapter);
    WARNING(FAILED(hr), "GetAdapter","");
    //ファクトリー取得
    IDXGIFactory* hpDXGIFactory = NULL;
    hr = hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);
    WARNING(FAILED(hr), "GetParent","");
    //スワップチェイン作成
    DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
    hDXGISwapChainDesc.BufferCount = 1;
    hDXGISwapChainDesc.BufferDesc.Width = ClientWidth;
    hDXGISwapChainDesc.BufferDesc.Height = ClientHeight;
    hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    hDXGISwapChainDesc.OutputWindow = HWnd;
    hDXGISwapChainDesc.Windowed = TRUE;
    hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    hDXGISwapChainDesc.SampleDesc = MSAA;
    hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    hr = hpDXGIFactory->CreateSwapChain(Device, &hDXGISwapChainDesc, &SwapChain);
    WARNING(FAILED(hr), "CreateSwapChain","");

    hpDXGI->Release();
    hpAdapter->Release();
    hpDXGIFactory->Release();
}
void createRenderTarget() {
    //マルチサンプリング対応
    // Create a render target view
    ID3D11Texture2D* backBuffer = NULL;
    HRESULT hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    WARNING(FAILED(hr), "can't Get BackBuffer", "");
    hr = Device->CreateRenderTargetView(backBuffer, NULL, &RenderTargetView);
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
    depthStencilTextureDesc.SampleDesc = MSAA;
    depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilTextureDesc.CPUAccessFlags = 0;
    depthStencilTextureDesc.MiscFlags = 0;
    ID3D11Texture2D* depthStencilTexture = 0;
    hr = Device->CreateTexture2D(&depthStencilTextureDesc, NULL, &depthStencilTexture);
    WARNING(FAILED(hr), "can't Create depth stencil texture", "");

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = depthStencilTextureDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    hr = Device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &DepthStencilView);
    WARNING(FAILED(hr), "can't Create DepthStencilView", "");
    SAFE_RELEASE(depthStencilTexture);

    // Set RenderTarget
    Context->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);
}

void setViewport() {
    D3D11_VIEWPORT vp;
    float aspect = Width / Height;
    if ((float)ClientWidth / ClientHeight >= aspect) {
        float width = ClientHeight * aspect;
        float left = (ClientWidth - width) / 2.0f;
        vp.Width = width;
        vp.Height = (float)ClientHeight;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = left;
        vp.TopLeftY = 0;
        Context->RSSetViewports(1, &vp);
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
        Context->RSSetViewports(1, &vp);
    }
}

void changeSize() {
    if (Context) {
        ID3D11RenderTargetView* renderTargetView = NULL;
        Context->OMSetRenderTargets(1, &renderTargetView, NULL);
        SAFE_RELEASE(RenderTargetView);
        SAFE_RELEASE(DepthStencilView);

        SwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        createRenderTarget();
        setViewport();
    }
}
void createDepthStencielState(){
    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable = FALSE;
    HRESULT hr = Device->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
    WARNING(FAILED(hr), "Error", "");
    Context->OMSetDepthStencilState(DepthStencilState, 0);
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
    Device->CreateBlendState(&blendDesc, &BlendState);
    float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
    Context->OMSetBlendState(BlendState, blendFactor, 0xffffffff);
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
    Device->CreateRasterizerState(&rasterizerDesc, &RasterizerState);
    Context->RSSetState(RasterizerState);
}
void createSamplerState()
{
    HRESULT hr;
    // Create the sample state
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = Device->CreateSamplerState(&samplerDesc, &SamplerLinear);
    WARNING(FAILED(hr), "cannot Create sampler state","");
    // Set the sample state
    Context->PSSetSamplers(0, 1, &SamplerLinear);

    // Set primitive topology
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
void clear(float* clearColor){
    // Clear the back buffer
    Context->ClearRenderTargetView(RenderTargetView, clearColor);
    // Clear the depth buffer to 1.0 (max depth)
    Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
    hr = Device->CreateBuffer(&bd, NULL, &CbWorld);
    WARNING(FAILED(hr), "Create Constant Buffer CbWorld","");
    // CbProj
    hr = Device->CreateBuffer(&bd, NULL, &CbProj);
    WARNING(FAILED(hr), "Create Constant Buffer Proj","");
    // CbColor
    bd.ByteWidth = sizeof(COLOR);
    hr = Device->CreateBuffer(&bd, NULL, &CbColor);
    WARNING(FAILED(hr), "Create Constant Buffer CbColor","");

    // Set Proj to constant buffer
    Proj.identity();
    Proj.ortho(0, Width, Height, 0, -1, 1);
    Context->UpdateSubresource(CbProj, 0, NULL, &Proj, 0, 0);
    Context->VSSetConstantBuffers(2, 1, &CbProj);
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
    HRESULT hr = Device->CreateVertexShader(mem, sizeOfRes, NULL, vertexShader);
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
        hr = Device->CreateInputLayout(inputElementDesc, numElements, mem, sizeOfRes, inputLayout);
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
        hr = Device->CreateInputLayout(inputElementDescSkin, numElementsSkin, mem, sizeOfRes, inputLayout);
        break; }
    case VERTEX_TYPE::PT: {
        //イメージインプットレイアウト生成
        D3D11_INPUT_ELEMENT_DESC inputElementDescImage[] = {
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
            {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,   1,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        };
        UINT numElementsImage = ARRAYSIZE(inputElementDescImage);
        hr = Device->CreateInputLayout(inputElementDescImage, numElementsImage, mem, sizeOfRes, inputLayout);
        break; }
    case VERTEX_TYPE::P: {
        //シェイプインプットレイアウト生成
        D3D11_INPUT_ELEMENT_DESC inputElementDescLine[] = {
            {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        };
        UINT numElementsLine = ARRAYSIZE(inputElementDescLine);
        hr = Device->CreateInputLayout(inputElementDescLine, numElementsLine, mem, sizeOfRes, inputLayout);
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
    HRESULT hr = Device->CreatePixelShader(mem, sizeOfRes, NULL, pixelShader);
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
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &RectVertexPosBuffer);
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
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &CircleVertexPosBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Circle","");
}
void createTexCoordBuffer()
{
    VERTEX_T vertices[] =
    {
        VECTOR2(0.0f, 1.0f),
        VECTOR2(0.0f, 0.0f),
        VECTOR2(1.0f, 1.0f),
        VECTOR2(1.0f, 0.0f),
    };
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_T) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &TexCoordBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Image","");
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
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &StrokeColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P), offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->Draw(4, 0);

    point(sx, sy);
    point(ex, ey);
}
void point(float x, float y)
{
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulScale(StrokeWeight, StrokeWeight, 1);

    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &StrokeColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P), offset = 0;
    Context->IASetVertexBuffers(0, 1, &CircleVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);

    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->Draw(64, 0);
}
RECT_MODE RectMode = LEFTTOP;
void rectMode(RECT_MODE mode)
{
    RectMode = mode;
}
void rect(float x, float y, float w, float h)
{
    if (RectMode == LEFTTOP) {
        World.identity();
        World.mulTranslate(x, y, 0);
        World.mulScale(w, h, 1);
        World.mulTranslate(0, 0.5, 0);
    }
    else {
        World.identity();
        World.mulTranslate(x, y, 0);
        World.mulScale(w, h, 1);
        World.mulTranslate(-0.5f, 0, 0);
    }

    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P), offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);

    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->Draw(4, 0);

    if (RectMode == LEFTTOP) {
        float r = x + w;
        float b = y + h;
        line(x, y, x, b);
        line(x, b, r, b);
        line(r, b, r, y);
        line(x, y, r, y);
    }
    else {
        float l = x - w / 2;
        float t = y - h / 2;
        float r = x + w / 2;
        float b = y + h / 2;
        line(l, t, l, b);
        line(l, b, r, b);
        line(r, b, r, t);
        line(r, t, l, t);
    }
}
void circle(float x, float y, float diameter)
{
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulScale(diameter, diameter, 1);

    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P), offset = 0;
    Context->IASetVertexBuffers(0, 1, &CircleVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);

    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->Draw(64, 0);

    int numAngles = 64;
    float rad = 3.141592f * 2 / numAngles;
    float radius = diameter / 2;
    for (int i = 0; i < numAngles; i++) {
        float sx = x + cosf(rad * i) * radius;
        float sy = y + sinf(rad * i) * radius;
        float ex = x + cosf(rad * (i + 1)) * radius;
        float ey = y + sinf(rad * (i + 1)) * radius;
        line(sx, sy, ex, ey);
    }
}

int loadImage(const char* filename)
{
    //stb_imageで画像をメモリに読み込む
    unsigned char* pixels = 0;
    int texWidth = 0;
    int texHeight = 0;
    int numBytePerPixel = 0;
    pixels = stbi_load(filename, &texWidth, &texHeight, &numBytePerPixel, 4);
    WARNING(!pixels, filename, "Load error");

    //テクスチャーとビューを創る
    D3D11_TEXTURE2D_DESC td;
    td.Width = texWidth;
    td.Height = texHeight;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_IMMUTABLE;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA sd;
    sd.pSysMem = (void*)pixels;
    sd.SysMemPitch = (UINT)(texWidth * 4);
    sd.SysMemSlicePitch = (UINT)(texWidth * texHeight * 4);
    ID3D11Texture2D* pTexture = 0;
    HRESULT hr;
    hr = Device->CreateTexture2D(&td, &sd, &pTexture);
    ID3D11ShaderResourceView* obj = 0;
    hr = Device->CreateShaderResourceView(pTexture, 0, &obj);
    WARNING(FAILED(hr), "resourceView","");

    //解放
    pTexture->Release();
    stbi_image_free(pixels);
    ID3D11Buffer* texCoord = 0;
    Textures->emplace_back(obj, texWidth, texHeight, texCoord);
    return int(Textures->size()) - 1;
}
int divideImage(int img, float left, float top, float width, float height)
{
    ID3D11ShaderResourceView* texObj = Textures->at(img).obj;
    float texWidth = Textures->at(img).width;
    float texHeight = Textures->at(img).height;

    float l = left / texWidth;
    float t = top / texHeight;
    float r = (left + width) / texWidth;
    float b = (top + height) / texHeight;
    VERTEX_T vertices[] =
    {
        VECTOR2(l, b),
        VECTOR2(l, t),
        VECTOR2(r, b),
        VECTOR2(r, t),
    };
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_T) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    ID3D11Buffer* texCoord;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &texCoord);
    WARNING(FAILED(hr), "CreateVertexBuffer Image","");

    Textures->emplace_back(texObj, width, height, texCoord);
    return int(Textures->size()) - 1;
}
void image(int img, float x, float y)
{
    WARNING((size_t)img >= Textures->size(), "画像番号オーバー","");

    TEXTURE& texture = Textures->at(img);

    //World = XMMatrixTranspose(
    //    XMMatrixTranslation(0.0f, 0.5f, 0)
    //    * XMMatrixScaling(texture.width, texture.height, 1)
    //    * XMMatrixTranslation(x, y, 0));
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulScale(texture.width, texture.height, 1);
    World.mulTranslate(0, 0.5f, 0);

    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &MeshColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P); UINT offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    stride = sizeof(VERTEX_T);
    if (texture.texCoord == 0)
        Context->IASetVertexBuffers(1, 1, &TexCoordBuffer, &stride, &offset);
    else
        Context->IASetVertexBuffers(1, 1, &texture.texCoord, &stride, &offset);
    Context->IASetInputLayout(ImageVertexLayout);

    Context->PSSetShaderResources(0, 1, &texture.obj);

    Context->VSSetShader(ImageVertexShader, NULL, 0);
    Context->PSSetShader(ImagePixelShader, NULL, 0);
    Context->Draw(4, 0);
}
void image(int img, float x, float y, float r)
{
    WARNING( (size_t)img >= Textures->size(), "画像番号オーバー", "" );

    TEXTURE& texture = Textures->at(img);

    //World = XMMatrixTranspose(
    //    XMMatrixTranslation(-0.5f, 0.0f, 0)
    //    * XMMatrixScaling(texture->width, texture->height, 1)
    //    * XMMatrixRotationZ(r)
    //    * XMMatrixTranslation(x, y, 0));
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulRotateZ(r);
    World.mulScale(texture.width, texture.height, 1);
    World.mulTranslate(-0.5f, 0, 0);
    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &MeshColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VERTEX_P); UINT offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    stride = sizeof(VERTEX_T);
    if (texture.texCoord == 0)
        Context->IASetVertexBuffers(1, 1, &TexCoordBuffer, &stride, &offset);
    else
        Context->IASetVertexBuffers(1, 1, &texture.texCoord, &stride, &offset);
    Context->IASetInputLayout(ImageVertexLayout);

    // Set texture
    Context->PSSetShaderResources(0, 1, &texture.obj);

    Context->VSSetShader(ImageVertexShader, NULL, 0);
    Context->PSSetShader(ImagePixelShader, NULL, 0);
    Context->Draw(4, 0);
}