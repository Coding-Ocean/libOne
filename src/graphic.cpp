#include<vector>
#include<map>
#include<string>
#include<d3d11.h>
#include"common.h"
#include"window.h"
#include"MATRIX.h"
#include"VECTOR.h"
#include"stb_image_reader.h"
#include"package.h"
#include"mathUtil.h"
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
void createTriangleVertexBuffer();
void createTexCoordBuffer();

extern float Width = 0;
extern float Height = 0;
DXGI_SAMPLE_DESC MSAA = { 0,0 };
ID3D11Device* Device = 0;
ID3D11DeviceContext* Context = 0;
IDXGISwapChain* SwapChain = 0;
ID3D11RenderTargetView* RenderTargetView = 0;
ID3D11DepthStencilView* DepthStencilView = 0;
//各種ステートオブジェクト
ID3D11DepthStencilState* DepthStencilState = 0;
ID3D11BlendState* BlendState = 0;
ID3D11RasterizerState* RasterizerStateWire = 0;
ID3D11RasterizerState* RasterizerStateCullBack = 0;
ID3D11RasterizerState* RasterizerStateCullNone = 0;
//line,rect,circle用シェーダ
ID3D11VertexShader* ShapeVertexShader = 0;
ID3D11InputLayout* ShapeVertexLayout = 0;
ID3D11PixelShader* ShapePixelShader = 0;
//image,text用シェーダ
ID3D11VertexShader* ImageVertexShader = 0;
ID3D11InputLayout* ImageVertexLayout = 0;
ID3D11PixelShader* ImagePixelShader = 0;
ID3D11SamplerState* SamplerStateClamp = 0;
ID3D11SamplerState* SamplerStateWrap = 0;
//triangle用シェーダ
ID3D11VertexShader* TriangleVertexShader = 0;
ID3D11InputLayout* TriangleVertexLayout = 0;
ID3D11PixelShader* TrianglePixelShader = 0;
//シェーダ用コンスタントバッファオブジェクト
ID3D11Buffer* CbWorld = 0;
ID3D11Buffer* CbProj = 0;
ID3D11Buffer* CbColor = 0;
//頂点オブジェクト
ID3D11Buffer* RectVertexPosBuffer = 0;
ID3D11Buffer* CircleSVertexPosBuffer = 0;
ID3D11Buffer* CircleMVertexPosBuffer = 0;
ID3D11Buffer* CircleLVertexPosBuffer = 0;
ID3D11Buffer* CircleLLVertexPosBuffer = 0;
ID3D11Buffer* TriangleVertexBuffer = 0;
ID3D11Buffer* TexCoordBuffer = 0;

//共通使用マトリックス
MATRIX Proj;
MATRIX World;
//デフォルト値
COLOR StrokeColor(0.0f, 0.0f, 0.0f, 1.0f);
COLOR FillColor(1.0f, 1.0f, 1.0f, 1.0f);
COLOR MeshColor(1.0f, 1.0f, 1.0f, 1.0f);
float StrokeWeight = 1;
int TextSize = 20;
COLOR_MODE ColorMode = RGB;
RECT_MODE RectMode = CORNER;
TEXT_MODE TextMode = BOTTOM;

struct CNTNR {
    //テクスチャ
    struct TEXTURE {
        ID3D11ShaderResourceView* obj;
        ID3D11Buffer* texCoord;
        float width;
        float height;
        TEXTURE(ID3D11ShaderResourceView* obj, float width, float height, ID3D11Buffer* texCoord) {
            this->obj = obj; this->width = width; this->height = height; this->texCoord = texCoord;
        }
    };
    std::vector<TEXTURE> textures;
    std::map<std::string, int> textureMap;

    //フォント
    struct FONT {
        ID3D11ShaderResourceView* texObj;
        GLYPHMETRICS gm;
        int size;
        FONT(ID3D11ShaderResourceView* texObj, GLYPHMETRICS gm, int size) {
            this->texObj = texObj; this->gm = gm; this->size = size;
        }
    };
    struct CURRENT_FONT {
        std::string name;
        int id;
    };
    class KEY {
    public:
        int fontId;
        int fontSize;
        wchar_t c;
        bool operator<(const KEY& key) const {
            bool b;
            if (this->fontId < key.fontId)
                b = true;
            else if (this->fontId > key.fontId)
                b = false;
            else if (this->fontSize < key.fontSize)
                b = true;
            else if (this->fontSize > key.fontSize)
                b = false;
            else if (this->c < key.c)
                b = true;
            else
                b = false;

            return b;
        }
    };
    CURRENT_FONT currentFont;
    std::vector<FONT> fonts;
    std::map<std::string, int> fontIdMap;
    std::map<KEY, int> fontMap;

    //カスタムシェイプ
    struct SHAPE {
        ID3D11Buffer* shapeVertexPosBuffer;
        int numShapeVertices;
        ID3D11Buffer* shapeIndexBuffer;
        int numShapeIndices;
        VECTOR* shapeVertices;
        SHAPE(
            ID3D11Buffer* shapeVertexPosBuffer,
            int numShapeVertices,
            ID3D11Buffer* shapeIndexBuffer,
            int numShapeIndices,
            VECTOR* shapeVertices
        ) {
            this->shapeVertexPosBuffer = shapeVertexPosBuffer;
            this->numShapeVertices = numShapeVertices;
            this->shapeIndexBuffer = shapeIndexBuffer;
            this->numShapeIndices = numShapeIndices;
            this->shapeVertices = shapeVertices;
        }
    };
    std::vector<SHAPE> shapes;

    ~CNTNR() {
        if (textures.size() > 0) {
            for (int i = (int)textures.size() - 1; i >= 0; i--) {
                if (textures.at(i).texCoord == 0)textures.at(i).obj->Release();
                if (textures.at(i).texCoord)textures.at(i).texCoord->Release();
                textures.pop_back();
            }
        }
        if (fonts.size() > 0) {
            for (int i = (int)fonts.size() - 1; i >= 0; i--) {
                fonts.at(i).texObj->Release();
                fonts.pop_back();
            }
        }
        if (shapes.size() > 0) {
            for (int i = (int)shapes.size() - 1; i >= 0; i--) {
                shapes.at(i).shapeVertexPosBuffer->Release();
                shapes.at(i).shapeIndexBuffer->Release();
                delete[] shapes.at(i).shapeVertices;
                shapes.pop_back();
            }
        }
    }
};
CNTNR* Cntnr = 0;
void refreshCntnr() {
    SAFE_DELETE(Cntnr);
    Cntnr = new CNTNR;
}
void freeGraphic() {
    if (Context) Context->ClearState();

    SAFE_DELETE(Cntnr);
    
    SAFE_RELEASE(SamplerStateWrap);
    SAFE_RELEASE(SamplerStateClamp);
    SAFE_RELEASE(TexCoordBuffer);
    SAFE_RELEASE(RectVertexPosBuffer);
    SAFE_RELEASE(CircleLLVertexPosBuffer);
    SAFE_RELEASE(CircleLVertexPosBuffer);
    SAFE_RELEASE(CircleMVertexPosBuffer);
    SAFE_RELEASE(CircleSVertexPosBuffer);
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
    SAFE_RELEASE(RasterizerStateWire);
    SAFE_RELEASE(RasterizerStateCullNone);
    SAFE_RELEASE(RasterizerStateCullBack);
    SAFE_RELEASE(DepthStencilState);
    SAFE_RELEASE(DepthStencilView);
    SAFE_RELEASE(RenderTargetView);
    SAFE_RELEASE(SwapChain);
    SAFE_RELEASE(Context);
    SAFE_RELEASE(Device);
}
void initGraphic(int baseWidth, int baseHeight) {
    //基準となる幅と高さ
    if (baseWidth == 0 || baseHeight == 0) {
        Width = (float)ClientWidth;
        Height = (float)ClientHeight;
    }
    else {
        Width = (float)baseWidth;
        Height = (float)baseHeight;
    }

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
    createTriangleVertexBuffer();
    createTexCoordBuffer();

    Cntnr = new CNTNR;
    font("BIZ UDGothic");
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
        hr = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality);
        if (SUCCEEDED( hr ) ){
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
    hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    hDXGISwapChainDesc.OutputWindow = HWnd;
    hDXGISwapChainDesc.Windowed = TRUE;
    hDXGISwapChainDesc.SampleDesc = MSAA;
    hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#ifndef _DEBUG//リリースモードの時
    if (Windowed == false) {
        hDXGISwapChainDesc.Windowed = FALSE;
    }
#endif
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

    // Create a depth stencil texture
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

    // Create a depth stencil view
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
    rasterizerDesc.FrontCounterClockwise = TRUE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    Device->CreateRasterizerState(&rasterizerDesc, &RasterizerStateCullNone);
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    Device->CreateRasterizerState(&rasterizerDesc, &RasterizerStateCullBack);
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    Device->CreateRasterizerState(&rasterizerDesc, &RasterizerStateWire);
    Context->RSSetState(RasterizerStateCullNone);
}
void setRasterizerCullNone() {
    Context->RSSetState(RasterizerStateCullNone);
}
void setRasterizerCullBack() {
    Context->RSSetState(RasterizerStateCullBack);
}
void setRasterizerWire() {
    Context->RSSetState(RasterizerStateWire);
}
void createSamplerState(){
    HRESULT hr;
    //サンプラーステートClamp生成～設定
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = Device->CreateSamplerState(&samplerDesc, &SamplerStateClamp);
    WARNING(FAILED(hr), "Error", "テクスチャサンプラーClampが生成できません");
    Context->PSSetSamplers(0, 1, &SamplerStateClamp);
    //サンプラーステートWrap生成のみ
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = Device->CreateSamplerState(&samplerDesc, &SamplerStateWrap);
    WARNING(FAILED(hr), "Error", "テクスチャサンプラーWrapが生成できません");
}
void setTextureSamplerClamp() {
    Context->PSSetSamplers(0, 1, &SamplerStateClamp);
}
void setTextureSamplerWrap() {
    Context->PSSetSamplers(0, 1, &SamplerStateWrap);
}
void clear(float* clearColor){
    // Clear the back buffer
    Context->ClearRenderTargetView(RenderTargetView, clearColor);
    // Clear the depth buffer to 1.0 (max depth)
    Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void present() {
    SwapChain->Present(1, 0);
}
void createConstantBuffer(){
    HRESULT hr;

    // Create the constant buffers
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    //CbWorld
    bd.ByteWidth = sizeof(MATRIX);
    hr = Device->CreateBuffer(&bd, NULL, &CbWorld);
    WARNING(FAILED(hr), "Can't Create Constant Buffer CbWorld","");
    //CbProj
    bd.ByteWidth = sizeof(MATRIX);
    hr = Device->CreateBuffer(&bd, NULL, &CbProj);
    WARNING(FAILED(hr), "Can't Create Constant Buffer Proj","");
    //CbColor
    bd.ByteWidth = sizeof(COLOR);
    hr = Device->CreateBuffer(&bd, NULL, &CbColor);
    WARNING(FAILED(hr), "Can't Create Constant Buffer CbColor","");

    // Set Proj to constant buffer
    Proj.identity();
    Proj.ortho(0, Width, Height, 0, -1, 1);
    Context->UpdateSubresource(CbProj, 0, NULL, &Proj, 0, 0);
    Context->VSSetConstantBuffers(2, 1, &CbProj);
}
HRESULT createConstantBuffer(unsigned byteWidth, ID3D11Buffer** buffer) {
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.ByteWidth = byteWidth;
    return Device->CreateBuffer(&bufferDesc, NULL, buffer);
}
void createVertexShaderAndInputLayoutFromRes(
    LPCTSTR resName,
    ID3D11VertexShader** vertexShader,
    VERTEX_TYPE vertexType,
    ID3D11InputLayout** inputLayout) {
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
    //inputLayoutをつくる
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
    case VERTEX_TYPE::PC: {
        //トライアングルインプットレイアウト生成
        D3D11_INPUT_ELEMENT_DESC inputElementDescTriangle[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElementsLine = ARRAYSIZE(inputElementDescTriangle);
        hr = Device->CreateInputLayout(inputElementDescTriangle, numElementsLine, mem, sizeOfRes, inputLayout);
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
void createShaderFromRes(){
    createVertexShaderAndInputLayoutFromRes(_T("SHAPE_VS"), &ShapeVertexShader, VERTEX_TYPE::P, &ShapeVertexLayout);
    createPixelShaderFromRes(_T("SHAPE_PS"), &ShapePixelShader);
    createVertexShaderAndInputLayoutFromRes(_T("IMAGE_VS"), &ImageVertexShader, VERTEX_TYPE::PT, &ImageVertexLayout);
    createPixelShaderFromRes(_T("IMAGE_PS"), &ImagePixelShader);
    createVertexShaderAndInputLayoutFromRes(_T("TRIANGLE_VS"), &TriangleVertexShader, VERTEX_TYPE::PC, &TriangleVertexLayout);
    createPixelShaderFromRes(_T("TRIANGLE_PS"), &TrianglePixelShader);
}
void createRectVertexPosBuffer(){
    //vertex for rect or line
    //rectと兼用だがline用に最適化した位置を設定する
    VECTOR vertices[] = {
        VECTOR(0.0f, -0.5f, 0.0f),
        VECTOR(0.0f,  0.5f, 0.0f),
        VECTOR(1.0f, -0.5f, 0.0f),
        VECTOR(1.0f,  0.5f, 0.0f),
    };
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VECTOR) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &RectVertexPosBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Rect","");
}
void createCircleVertexPosBuffer(){
    {
        const int num = 8;
        float rad = 3.141592f * 2 / num;
        VECTOR circleVertices[num];
        circleVertices[0] = VECTOR(0.5f, 0, 0.0f);
        for (int i = 1, j = 1; i <= num / 2 - 1; i++) {
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, -sinf(rad * i) * 0.5f, 0.0f);
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, sinf(rad * i) * 0.5f, 0.0f);
        }
        circleVertices[num - 1] = VECTOR(-0.5f, 0, 0.0f);
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VECTOR) * (num);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = circleVertices;
        HRESULT hr = Device->CreateBuffer(&bd, &InitData, &CircleSVertexPosBuffer);
        WARNING(FAILED(hr), "CreateVertexBuffer Circle", "");
    }
    {
        const int num = 16;
        float rad = 3.141592f * 2 / num;
        VECTOR circleVertices[num];
        circleVertices[0] = VECTOR(0.5f, 0, 0.0f);
        for (int i = 1, j = 1; i <= num / 2 - 1; i++) {
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, -sinf(rad * i) * 0.5f, 0.0f);
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, sinf(rad * i) * 0.5f, 0.0f);
        }
        circleVertices[num - 1] = VECTOR(-0.5f, 0, 0.0f);
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VECTOR) * (num);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = circleVertices;
        HRESULT hr = Device->CreateBuffer(&bd, &InitData, &CircleMVertexPosBuffer);
        WARNING(FAILED(hr), "CreateVertexBuffer Circle", "");
    }
    {
        const int num = 32;
        float rad = 3.141592f * 2 / num;
        VECTOR circleVertices[num];
        circleVertices[0] = VECTOR(0.5f, 0, 0.0f);
        for (int i = 1, j = 1; i <= num / 2 - 1; i++) {
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, -sinf(rad * i) * 0.5f, 0.0f);
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, sinf(rad * i) * 0.5f, 0.0f);
        }
        circleVertices[num - 1] = VECTOR(-0.5f, 0, 0.0f);
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VECTOR) * (num);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = circleVertices;
        HRESULT hr = Device->CreateBuffer(&bd, &InitData, &CircleLVertexPosBuffer);
        WARNING(FAILED(hr), "CreateVertexBuffer Circle", "");
    }
    {
        const int num = 64;
        float rad = 3.141592f * 2 / num;
        VECTOR circleVertices[num];
        circleVertices[0] = VECTOR(0.5f, 0, 0.0f);
        for (int i = 1, j = 1; i <= num / 2 - 1; i++) {
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, -sinf(rad * i) * 0.5f, 0.0f);
            circleVertices[j++] = VECTOR(cosf(rad * i) * 0.5f, sinf(rad * i) * 0.5f, 0.0f);
        }
        circleVertices[num - 1] = VECTOR(-0.5f, 0, 0.0f);
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VECTOR) * (num);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = circleVertices;
        HRESULT hr = Device->CreateBuffer(&bd, &InitData, &CircleLLVertexPosBuffer);
        WARNING(FAILED(hr), "CreateVertexBuffer Circle", "");
    }
}
//3D勉強用
struct VERTEX_PC {
    VECTOR pos;
    COLOR color;
};
struct VERTEX_PC Vertices[3];
void createTriangleVertexBuffer() {
    // Vertex Buffer for drawTriangle
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.ByteWidth = sizeof(VERTEX_PC) * 3;
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = Vertices;
    HRESULT hr = Device->CreateBuffer(&bd, &initData, &TriangleVertexBuffer);
    WARNING(FAILED(hr), "CreateTriangleVertexBuffer", "");
}
//カスタムシェープの頂点バッファを作る。トライアングルファンインデックス
int createShape(struct SHAPE_VERTEX* vertices, int numVertices) {
    ID3D11Buffer* shapeVertexPosBuffer = 0;
    ID3D11Buffer* shapeIndexBuffer = 0;
    VECTOR* shapeVertices = 0;
    //頂点バッファをつくる
    shapeVertices = new VECTOR[numVertices];
    for (int i = 0; i < numVertices; i++) {
        shapeVertices[i].x = vertices[i].x;
        shapeVertices[i].y = vertices[i].y;
        shapeVertices[i].z = 0;
    }
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VECTOR) * (numVertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = shapeVertices;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &shapeVertexPosBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Circle", "");
    //インデックスバッファをつくる
    //　インデックス
    const int numTriangles = numVertices - 2;
    const int numIndices = numTriangles * 3;
    long* indices = new long[numIndices];
    for (int i = 0; i < numTriangles; i++) {
        int idx = i * 3;
        indices[idx + 0] = 0;
        indices[idx + 1] = i + 1;
        indices[idx + 2] = i + 2;
    }
    ZeroMemory(&bd, sizeof(bd));
    bd.ByteWidth = sizeof(long) * numIndices;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    hr = Device->CreateBuffer(&bd, &InitData, &shapeIndexBuffer);
    Cntnr->shapes.emplace_back(
        shapeVertexPosBuffer, numVertices,
        shapeIndexBuffer, numIndices,
        shapeVertices);
    delete[] indices;
    return int(Cntnr->shapes.size() - 1);
}
int createShape(struct SHAPE_VERTEX* vertices, int numVertices,
    long* indices, int numIndices) {
    ID3D11Buffer* shapeVertexPosBuffer = 0;
    ID3D11Buffer* shapeIndexBuffer = 0;
    VECTOR* shapeVertices = 0;
    //頂点バッファをつくる
    shapeVertices = new VECTOR[numVertices];
    for (int i = 0; i < numVertices; i++) {
        shapeVertices[i].x = vertices[i].x;
        shapeVertices[i].y = vertices[i].y;
        shapeVertices[i].z = 0;
    }
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VECTOR) * (numVertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = shapeVertices;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &shapeVertexPosBuffer);
    WARNING(FAILED(hr), "CreateVertexBuffer Circle", "");
    //インデックスバッファをつくる
    ZeroMemory(&bd, sizeof(bd));
    bd.ByteWidth = sizeof(long) * numIndices;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    hr = Device->CreateBuffer(&bd, &InitData, &shapeIndexBuffer);
    Cntnr->shapes.emplace_back(
        shapeVertexPosBuffer, numVertices,
        shapeIndexBuffer, numIndices,
        shapeVertices);
    return int(Cntnr->shapes.size() - 1);
}

void createTexCoordBuffer(){
    VECTOR2 vertices[] =  {
        VECTOR2(0.0f, 0.0f),
        VECTOR2(0.0f, 1.0f),
        VECTOR2(1.0f, 0.0f),
        VECTOR2(1.0f, 1.0f),
    };
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VECTOR2) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &TexCoordBuffer);
    WARNING(FAILED(hr), "CreateTexCoordBuffe","");
}

float ColorDenominator = 255;
void colorMode(COLOR_MODE mode, float colorDenominator) {
    ColorMode = mode;
    ColorDenominator = colorDenominator;
}
COLOR hsv2rgb(float h, float s, float v, float a) {
    if (AngleMode == RADIANS) h *= TO_DEG;
    //0～360の値に変換
    if (h < 0) h = h - int(h / 360) * 360 + 360;
    if (h > 360) h = h - int(h / 360) * 360;
    h /= 360;
    s /= ColorDenominator;
    v /= ColorDenominator;
    float r = v;
    float g = v;
    float b = v;
    if (s > 0.0f) {
        h *= 6.0f;
        int i = (int)h;
        float f = h - (float)i;
        switch (i) {
        default:
        case 0:
            g *= 1 - s * (1 - f);
            b *= 1 - s;
            break;
        case 1:
            r *= 1 - s * f;
            b *= 1 - s;
            break;
        case 2:
            r *= 1 - s;
            b *= 1 - s * (1 - f);
            break;
        case 3:
            r *= 1 - s;
            g *= 1 - s * f;
            break;
        case 4:
            r *= 1 - s * (1 - f);
            g *= 1 - s;
            break;
        case 5:
            g *= 1 - s;
            b *= 1 - s * f;
            break;
        }
    }
    return COLOR(r, g, b, a / ColorDenominator);
}

void clear(const COLOR& c) {
    float clearColor[4];
    if (ColorMode == RGB) {
        clearColor[0] = c.r / ColorDenominator;
        clearColor[1] = c.g / ColorDenominator;
        clearColor[2] = c.b / ColorDenominator;
        clearColor[3] = 1.0f;
    }
    else if (ColorMode == HSV) {
        COLOR hsv = hsv2rgb(c.r, c.g, c.b, ColorDenominator);
        clearColor[0] = hsv.r;
        clearColor[1] = hsv.g;
        clearColor[2] = hsv.b;
        clearColor[3] = 1.0f;
    }
    clear(clearColor);
}
void clear(float r, float g, float b) {
    float clearColor[4];
    if (ColorMode == RGB) {
        clearColor[0] = r / ColorDenominator;
        clearColor[1] = g / ColorDenominator;
        clearColor[2] = b / ColorDenominator;
        clearColor[3] = 1.0f;
    }
    else if (ColorMode == HSV) {
        COLOR c = hsv2rgb(r, g, b, ColorDenominator);
        clearColor[0] = c.r;
        clearColor[1] = c.g;
        clearColor[2] = c.b;
        clearColor[3] = 1.0f;
    }
    clear(clearColor);
}
void clear(float c) {
    c /= ColorDenominator;
    float clearColor[4] = { c, c, c, 1.0f };
    clear(clearColor);
}

void stroke(float r, float g, float b) {
    if (ColorMode == RGB) {
        StrokeColor.r = r / ColorDenominator;
        StrokeColor.g = g / ColorDenominator;
        StrokeColor.b = b / ColorDenominator;
        StrokeColor.a = 1.0f;
    }
    else if (ColorMode == HSV) {
        StrokeColor = hsv2rgb(r, g, b, ColorDenominator);
    }
}
void stroke(float r, float g, float b, float a){
    if (ColorMode == RGB) {
        StrokeColor.r = r / ColorDenominator;
        StrokeColor.g = g / ColorDenominator;
        StrokeColor.b = b / ColorDenominator;
        StrokeColor.a = a / ColorDenominator;
    }
    else if (ColorMode == HSV) {
        StrokeColor = hsv2rgb(r, g, b, a);
    }
}
void stroke(const COLOR& c) {
    if (ColorMode == RGB) {
        StrokeColor.r = c.r / ColorDenominator;
        StrokeColor.g = c.g / ColorDenominator;
        StrokeColor.b = c.b / ColorDenominator;
        StrokeColor.a = c.a / ColorDenominator;
    }
    else if (ColorMode == HSV) {
        StrokeColor = hsv2rgb(c.r, c.g, c.b, c.a);
    }
}
void stroke(float c) {
    c /= ColorDenominator;
    StrokeColor.r = c;
    StrokeColor.g = c;
    StrokeColor.b = c;
    StrokeColor.a = 1;
}
void fill(float r, float g, float b) {
    if (ColorMode == RGB) {
        FillColor.r = r / ColorDenominator;
        FillColor.g = g / ColorDenominator;
        FillColor.b = b / ColorDenominator;
        FillColor.a = 1.0f;
    }
    else if (ColorMode == HSV) {
        FillColor = hsv2rgb(r, g, b, ColorDenominator);
    }
}
void fill(float r, float g, float b, float a){
    if (ColorMode == RGB) {
        FillColor.r = r / ColorDenominator;
        FillColor.g = g / ColorDenominator;
        FillColor.b = b / ColorDenominator;
        FillColor.a = a / ColorDenominator;
    }
    else if (ColorMode == HSV) {
        FillColor = hsv2rgb(r, g, b, a);
    }
}
void fill(const COLOR& c) {
    if (ColorMode == RGB) {
        FillColor.r = c.r / ColorDenominator;
        FillColor.g = c.g / ColorDenominator;
        FillColor.b = c.b / ColorDenominator;
        FillColor.a = c.a / ColorDenominator;
    }
    else if (ColorMode == HSV) {
        FillColor = hsv2rgb(c.r, c.g, c.b, c.a);
    }
}
void fill(float c) {
    c /= ColorDenominator;
    FillColor.r = c;
    FillColor.g = c;
    FillColor.b = c;
    FillColor.a = 1;
}
void noFill() {
    FillColor.r = 0;
    FillColor.g = 0;
    FillColor.b = 0;
    FillColor.a = 0;
}
void imageColor(float r, float g, float b) {
    if (ColorMode == RGB) {
        MeshColor.r = r / ColorDenominator;
        MeshColor.g = g / ColorDenominator;
        MeshColor.b = b / ColorDenominator;
        MeshColor.a = 1.0f;
    }
    else if (ColorMode == HSV) {
        MeshColor = hsv2rgb(r, g, b, ColorDenominator);
    }
}
void imageColor(float r, float g, float b, float a){
    if (ColorMode == RGB) {
        MeshColor.r = r / ColorDenominator;
        MeshColor.g = g / ColorDenominator;
        MeshColor.b = b / ColorDenominator;
        MeshColor.a = a / ColorDenominator;
    }
    else if (ColorMode == HSV) {
        MeshColor = hsv2rgb(r, g, b, a);
    }
}
void imageColor(const COLOR& c) {
    if (ColorMode == RGB) {
        MeshColor.r = c.r / ColorDenominator;
        MeshColor.g = c.g / ColorDenominator;
        MeshColor.b = c.b / ColorDenominator;
        MeshColor.a = c.a / ColorDenominator;
    }
    else if (ColorMode == HSV) {
        MeshColor = hsv2rgb(c.r, c.g, c.b, c.a);
    }
}
void imageColor(float c){
    c /= ColorDenominator;
    MeshColor.r = c;
    MeshColor.g = c;
    MeshColor.b = c;
    MeshColor.a = 1;
}



void strokeWeight(float weight){
    StrokeWeight = weight;
}
void noStroke() {
    StrokeWeight = 0;
}

bool DrawEndPointFlag = true;
void line(float sx, float sy, float ex, float ey){
    float dx = ex - sx;
    float dy = ey - sy;
    float length = sqrtf(dx * dx + dy * dy);
    float r = Atan2(dy, dx);
    World.identity();
    World.mulTranslate(sx, sy, 0);
    World.mulRotateZ(r);
    World.mulScaling(length, StrokeWeight, 1);
    // Update constant buffer
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &StrokeColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);
    // Set vertex buffer
    UINT stride = sizeof(VECTOR), offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->Draw(4, 0);
    if (StrokeWeight > 2) 
    {
        point(sx, sy);
        if (DrawEndPointFlag)point(ex, ey);
    }
}
void arrow(float sx, float sy, float ex, float ey, float size) {
    if (sx == ex && sy == ey)return;
    float ang = Atan2(ey - sy, ex - sx);
    float c = Cos(ang);
    float s = Sin(ang);
    float sizeY = size / 3;
    float ax, ay;
    //線
    line(sx, sy, ex, ey);
    //矢印１
    ax = -size * c + sizeY * -s;
    ay = -size * s + sizeY * c;
    line(ex, ey, ex + ax, ey + ay);
    //矢印２
    ax = -size * c + -sizeY * -s;
    ay = -size * s + -sizeY * c;
    line(ex, ey, ex + ax, ey + ay);
}
void arc(float px, float py, float startAngle, float angle, float radius) {
    float div = 5;
    if (AngleMode == RADIANS)div *= TO_RAD;
    int num = int(Abs(angle) / div) + 1;
    float divAngle = angle / num;

    float sx = Cos(startAngle) * radius;
    float sy = -Sin(startAngle) * radius;
    line(px, py, px + sx, py + sy);
    float ex = 0, ey = 0;
    for (int i = 0; i < num; i++) {
        ex = Cos(startAngle + divAngle * (i + 1)) * radius;
        ey = -Sin(startAngle + divAngle * (i + 1)) * radius;
        line(px + sx, py + sy, px + ex, py + ey);
        sx = ex;
        sy = ey;
    }
    line(px, py, px + ex, py + ey);
}
void point(float x, float y){
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulScaling(StrokeWeight, StrokeWeight, 1);

    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &StrokeColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VECTOR), offset = 0;
    Context->IASetInputLayout(ShapeVertexLayout);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    if (StrokeWeight <=4) {
        Context->IASetVertexBuffers(0, 1, &CircleSVertexPosBuffer, &stride, &offset);
        Context->Draw(8, 0);
    }
    else if (StrokeWeight<=10) {
        Context->IASetVertexBuffers(0, 1, &CircleMVertexPosBuffer, &stride, &offset);
        Context->Draw(16, 0);
    }
    else if (StrokeWeight <= 20) {
        Context->IASetVertexBuffers(0, 1, &CircleLVertexPosBuffer, &stride, &offset);
        Context->Draw(32, 0);
    }
    else {
        Context->IASetVertexBuffers(0, 1, &CircleLLVertexPosBuffer, &stride, &offset);
        Context->Draw(64, 0);
    }
}
void rectMode(RECT_MODE mode){
    RectMode = mode;
}
void rect(float x, float y, float w, float h) {
    if (RectMode == CORNER) {
        World.identity();
        World.mulTranslate(x, y, 0);
        World.mulScaling(w, h, 1);
        World.mulTranslate(0, 0.5, 0);
    }
    else {
        World.identity();
        World.mulTranslate(x, y, 0);
        World.mulScaling(w, h, 1);
        World.mulTranslate(-0.5f, 0, 0);
    }
    if (FillColor.a > 0) {
        // Update variables that change once per frame
        Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
        Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
        Context->VSSetConstantBuffers(0, 1, &CbWorld);
        Context->PSSetConstantBuffers(3, 1, &CbColor);
        // Set vertex buffer
        UINT stride = sizeof(VECTOR), offset = 0;
        Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
        Context->IASetInputLayout(ShapeVertexLayout);
        Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        Context->VSSetShader(ShapeVertexShader, NULL, 0);
        Context->PSSetShader(ShapePixelShader, NULL, 0);
        Context->Draw(4, 0);
    }
    //draw stroke
    if (StrokeWeight > 0) {
        DrawEndPointFlag = false;
        if (RectMode == CORNER) {
            float r = x + w;
            float b = y + h;
            line(x, y, x, b);
            line(x, b, r, b);
            line(r, b, r, y);
            line(r, y, x, y);
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
        DrawEndPointFlag = true;
    }
}
void rect(float x, float y, float w, float h, float r) {
    World.identity();
    if (RectMode == CORNER) {
        World.mulTranslate(x + w / 2, y + h / 2, 0);
    }
    else {
        World.mulTranslate(x, y, 0);
    }
    World.mulRotateZ(r);
    World.mulScaling(w, h, 1);
    World.mulTranslate(-0.5f, 0, 0);
    if (FillColor.a > 0) {
        // Update variables that change once per frame
        Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
        Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
        Context->VSSetConstantBuffers(0, 1, &CbWorld);
        Context->PSSetConstantBuffers(3, 1, &CbColor);
        // Set vertex buffer
        UINT stride = sizeof(VECTOR), offset = 0;
        Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
        Context->IASetInputLayout(ShapeVertexLayout);
        Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        Context->VSSetShader(ShapeVertexShader, NULL, 0);
        Context->PSSetShader(ShapePixelShader, NULL, 0);
        Context->Draw(4, 0);
    }
    if (StrokeWeight > 0) {
        DrawEndPointFlag = false;
        VECTOR lt = World * VECTOR(0.0f, 0.5f, 0.0f);
        VECTOR lb = World * VECTOR(0.0f, -0.5f, 0.0f);
        VECTOR rt = World * VECTOR(1.0f, 0.5f, 0.0f);
        VECTOR rb = World * VECTOR(1.0f, -0.5f, 0.0f);
        line(lt.x, lt.y, lb.x, lb.y);
        line(lb.x, lb.y, rb.x, rb.y);
        line(rb.x, rb.y, rt.x, rt.y);
        line(rt.x, rt.y, lt.x, lt.y);
        DrawEndPointFlag = true;
    }
}
void circle(float x, float y, float diameter){
    int numAngles = 0;
    if (FillColor.a > 0) {
        World.identity();
        World.mulTranslate(x, y, 0);
        World.mulScaling(diameter, diameter, 1);

        // Update variables that change once per frame
        Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
        Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
        Context->VSSetConstantBuffers(0, 1, &CbWorld);
        Context->PSSetConstantBuffers(3, 1, &CbColor);

        // Set vertex buffer
        UINT stride = sizeof(VECTOR), offset = 0;
        Context->IASetInputLayout(ShapeVertexLayout);
        Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        Context->VSSetShader(ShapeVertexShader, NULL, 0);
        Context->PSSetShader(ShapePixelShader, NULL, 0);

        if (diameter <= 100) {
            Context->IASetVertexBuffers(0, 1, &CircleLVertexPosBuffer, &stride, &offset);
            numAngles = 32;
            Context->Draw(numAngles, 0);
        }
        else {
            Context->IASetVertexBuffers(0, 1, &CircleLLVertexPosBuffer, &stride, &offset);
            numAngles = 64;
            Context->Draw(numAngles, 0);
        }
    }
    //輪郭
    if (StrokeWeight > 0) {
        if (diameter <= 100) {
            numAngles = 32;
        }
        else if (diameter < 900) {
            numAngles = 64;
        }
        else {
            numAngles = 120;
        }
        float rad = 3.141592f * 2 / numAngles;
        float radius = diameter / 2;
        DrawEndPointFlag = false;
        float sx = x + radius;
        float sy = y;
        float ex, ey;
        for (int i = 0; i < numAngles; i++) {
            ex = x + cosf(rad * (i + 1)) * radius;
            ey = y + sinf(rad * (i + 1)) * radius;
            line(sx, sy, ex, ey);
            sx = ex;
            sy = ey;
        }
        DrawEndPointFlag = true;
    }
}
void point(const VECTOR& p, const COLOR& c, float sw){
    stroke(c);
    strokeWeight(sw);
    point(p.x, p.y);
}
void line(const VECTOR& p1, const VECTOR& p2, const COLOR& c, float sw){
    stroke(c);
    strokeWeight(sw);
    line(p1.x, p1.y, p2.x, p2.y);
}
void arrow(const VECTOR& s, const VECTOR& e, const COLOR& strokeColor, float strokeW) {
    stroke(strokeColor);
    strokeWeight(strokeW);
    arrow(s.x, s.y, e.x, e.y);
}
void circle(const VECTOR& p, float diameter, const COLOR& fc, const COLOR& sc, float sw){
    fill(fc);
    stroke(sc);
    strokeWeight(sw);
    circle(p.x, p.y, diameter);
}
void rect(const VECTOR& p, float w, float h, float angle, 
    RECT_MODE mode, const COLOR& fc, const COLOR& sc, float sw){
    rectMode(mode);
    fill(fc);
    stroke(sc);
    strokeWeight(sw);
    rect(p.x, p.y, w, h, angle);
}
void shape(int idx, float x, float y, float r, float size) {
    WARNING((size_t)idx >= Cntnr->shapes.size(), "shape番号オーバー", "");
    CNTNR::SHAPE& sh = Cntnr->shapes.at(idx);
    World.identity();
    World.mulTranslate(x, y, 0);
    World.mulRotateZ(r);
    World.mulScaling(size, size, 1);
    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);
    // Set vertex buffer
    UINT stride = sizeof(VECTOR), offset = 0;
    Context->IASetVertexBuffers(0, 1, &sh.shapeVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);
    Context->IASetIndexBuffer(sh.shapeIndexBuffer, DXGI_FORMAT_R32_UINT, 0); 
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->DrawIndexed(sh.numShapeIndices, 0,0);
    // Outline of shape
    if (StrokeWeight > 0) {
        DrawEndPointFlag = false;
        //line内でWorldを変更するためローカルコピーを使用
        MATRIX world = World;
        VECTOR o, s, e;
        o = world * sh.shapeVertices[0];
        s = o;
        for (int i = 0; i < sh.numShapeVertices-1; i++) {
            e = world * sh.shapeVertices[i+1];
            line(s.x, s.y, e.x, e.y);
            s = e;
        }
        line(s.x, s.y, o.x, o.y);
        DrawEndPointFlag = true;
    }
}
void shape(int idx, const class MATRIX& world) {
    WARNING((size_t)idx >= Cntnr->shapes.size(), "shape番号オーバー", "");
    CNTNR::SHAPE& sh = Cntnr->shapes.at(idx);
    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &world, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);
    // Set vertex buffer
    UINT stride = sizeof(VECTOR), offset = 0;
    Context->IASetVertexBuffers(0, 1, &sh.shapeVertexPosBuffer, &stride, &offset);
    Context->IASetInputLayout(ShapeVertexLayout);
    Context->IASetIndexBuffer(sh.shapeIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->VSSetShader(ShapeVertexShader, NULL, 0);
    Context->PSSetShader(ShapePixelShader, NULL, 0);
    Context->DrawIndexed(sh.numShapeIndices, 0, 0);
    // Outline of shape
    if (StrokeWeight > 0) {
        DrawEndPointFlag = false;
        VECTOR o, s, e;
        o = world * sh.shapeVertices[0];
        s = o;
        for (int i = 0; i < sh.numShapeVertices - 1; i++) {
            e = world * sh.shapeVertices[i + 1];
            line(s.x, s.y, e.x, e.y);
            s = e;
        }
        line(s.x, s.y, o.x, o.y);
        DrawEndPointFlag = true;
    }
}

int loadImage(const char* filename){
    //重複ロードを避ける
    auto it = Cntnr->textureMap.find(filename);
    if (it != Cntnr->textureMap.end()) {
        return it->second;
    }
    //ロード
    unsigned char* pixels = 0;
    int texWidth = 0;
    int texHeight = 0;
    int numBytePerPixel = 0;
    if (packageDataExists()==false) {
        //パッケージがなければ、普通にファイル読み込みする
        pixels = stbi_load(filename, &texWidth, &texHeight, &numBytePerPixel, 4);
        WARNING(!pixels, filename, "Load error");
    }
    else {
        //パッケージデータから読み込む
        int size;
        unsigned char* mem = getPackageData(filename, &size);
        pixels = stbi_load_from_memory(mem, size, &texWidth, &texHeight, &numBytePerPixel, 4);
        WARNING(!pixels, filename, "Load error from package");
    }
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

    Cntnr->textures.emplace_back(obj, texWidth, texHeight, texCoord);
    int textureIdx = int(Cntnr->textures.size()) - 1;
    Cntnr->textureMap.emplace(filename, textureIdx);
    return textureIdx;
}
int loadImageFromRes(const char* str) {
    //size_t l = strlen(str);
    //int num = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, (DWORD)l, 0, 0);
    //wchar_t resName[128] = L"";
    //int ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, (DWORD)l, resName, num);

    HINSTANCE hInst = GetModuleHandle(0);
    //HRSRC hRes = FindResource(hInst, resName, _T("IMAGE"));
    HRSRC hRes = FindResourceA(hInst, str, "IMAGE");
    WARNING(hRes == 0, "リソースがみつからない", "loadImageFromRes");
    // リソースのサイズを取得する 
    DWORD sizeOfRes = SizeofResource(hInst, hRes);
    WARNING(sizeOfRes == 0, "リソースのサイズがゼロ", "loadImageFromRes");
    // 取得したリソースをメモリにロードする
    HGLOBAL hMem = LoadResource(hInst, hRes);
    WARNING(hMem == 0, "リソースがロードできない", "loadImageFromRes");
    // ロードしたリソースデータのアドレスを取得する
    unsigned char* mem = (unsigned char*)LockResource(hMem);
    if (mem == 0) {
        FreeResource(hMem);
        WARNING(true, "リソースのアドレスが取得できない", "loadImageFromRes");
    }
    //stb_imageで読み込んでResourceViewをつくる
    unsigned char* pixels = 0;
    int texWidth = 0;
    int texHeight = 0;
    int numBytePerPixel = 0;
    pixels = stbi_load_from_memory(mem, sizeOfRes, &texWidth, &texHeight, &numBytePerPixel, 4);
    WARNING(!pixels, str, "Load error");

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
    WARNING(FAILED(hr), "resourceView", "");

    //解放
    pTexture->Release();
    stbi_image_free(pixels);
    FreeResource(hMem);
    ID3D11Buffer* texCoord = 0;
    Cntnr->textures.emplace_back(obj, texWidth, texHeight, texCoord);
    return int(Cntnr->textures.size()) - 1;
}
int cutImage(int idx, int left, int top, int w, int h){
    WARNING((size_t)idx >= Cntnr->textures.size(), "画像番号オーバー", "");
    ID3D11ShaderResourceView* texObj = Cntnr->textures.at(idx).obj;
    float texWidth = Cntnr->textures.at(idx).width;
    float texHeight = Cntnr->textures.at(idx).height;

    float l = left / texWidth;
    float t = top / texHeight;
    float r = (left + w) / texWidth;
    float b = (top + h) / texHeight;
    VECTOR2 vertices[] = {
        VECTOR2(l, t),
        VECTOR2(l, b),
        VECTOR2(r, t),
        VECTOR2(r, b),
    };
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VECTOR2) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    ID3D11Buffer* texCoord;
    HRESULT hr = Device->CreateBuffer(&bd, &InitData, &texCoord);
    WARNING(FAILED(hr), "CreateVertexBuffer Image","");

    Cntnr->textures.emplace_back(texObj, w, h, texCoord);
    return int(Cntnr->textures.size()) - 1;
}
void divideImage(int img, int cols, int rows, int w, int h, int* imgs) {
    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
            imgs[j * cols + i] = cutImage(img, i * w, j * h, w, h);
        }
    }
}
void image(int idx, float x, float y, float a, float s) {
    WARNING((size_t)idx >= Cntnr->textures.size(), "画像番号オーバー", "");

    CNTNR::TEXTURE& texture = Cntnr->textures.at(idx);

    World.identity();
    if (RectMode == CORNER) {
        World.mulTranslate(x + texture.width *s/ 2, 
            y + texture.height*s / 2, 0);
    }
    else {
        World.mulTranslate(x, y, 0);
    }
    World.mulRotateZ(a);
    World.mulScaling(texture.width*s, texture.height*s, 1);
    World.mulTranslate(-0.5f, 0, 0);

    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &MeshColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);

    // Set vertex buffer
    UINT stride = sizeof(VECTOR); UINT offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    stride = sizeof(VECTOR2);
    if (texture.texCoord == 0)
        Context->IASetVertexBuffers(1, 1, &TexCoordBuffer, &stride, &offset);
    else
        Context->IASetVertexBuffers(1, 1, &texture.texCoord, &stride, &offset);
    Context->IASetInputLayout(ImageVertexLayout);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // Set texture
    Context->PSSetShaderResources(0, 1, &texture.obj);

    Context->VSSetShader(ImageVertexShader, NULL, 0);
    Context->PSSetShader(ImagePixelShader, NULL, 0);
    Context->Draw(4, 0);
}
void image(int idx, const VECTOR& p, float angle, float size, RECT_MODE mode)
{
    rectMode(mode);
    image(idx, p.x, p.y, angle, size);
}
void triangle3D(const VECTOR& p0, const VECTOR& p1, const VECTOR& p2, 
    const COLOR& c0, const COLOR& c1, const COLOR& c2) {
    Vertices[0].pos = p0;
    Vertices[1].pos = p1;
    Vertices[2].pos = p2;
    if (ColorMode == RGB) {
        float ce = 1 / ColorDenominator;
        Vertices[0].color = COLOR(c0.r * ce, c0.g * ce, c0.b * ce, c0.a * ce);
        Vertices[1].color = COLOR(c1.r * ce, c1.g * ce, c1.b * ce, c1.a * ce);
        Vertices[2].color = COLOR(c2.r * ce, c2.g * ce, c2.b * ce, c2.a * ce);
    }
    else {
        Vertices[0].color = hsv2rgb(c0.r, c0.g, c0.b, c0.a);
        Vertices[1].color = hsv2rgb(c1.r, c1.g, c1.b, c1.a);
        Vertices[2].color = hsv2rgb(c2.r, c2.g, c2.b, c2.a);
    }

    Context->UpdateSubresource(TriangleVertexBuffer, 0, 0, Vertices, 0, 0);
    UINT stride = sizeof(VERTEX_PC);
    UINT offset = 0;
    Context->IASetVertexBuffers(0, 1, &TriangleVertexBuffer, &stride, &offset);
    Context->IASetInputLayout(TriangleVertexLayout);
    Context->VSSetShader(TriangleVertexShader, NULL, 0);
    Context->PSSetShader(TrianglePixelShader, NULL, 0);
    Context->Draw(3, 0);
}

//----------------------------------------------------------------------------------------------------
//１文字のフォントテクスチャを作る
int createFont(const wchar_t* c)
{
    // フォントの生成
    int fontWeight = 0;
    LOGFONTA lf = { TextSize, 0, 0, 0, fontWeight, 0, 0, 0,
        SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, 0 };
    strcpy_s(lf.lfFaceName, Cntnr->currentFont.name.c_str());
    HFONT hFont = CreateFontIndirectA(&lf);
    // デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
    HDC hdc = GetDC(NULL);
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
    // フォントビットマップ取得
    UINT code = (UINT)c[0];
    const int gradFlag = GGO_GRAY4_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
    UINT grad = 0; // 階調の最大値
    switch (gradFlag) {
    case GGO_GRAY2_BITMAP: grad = 4; break;
    case GGO_GRAY4_BITMAP: grad = 16; break;
    case GGO_GRAY8_BITMAP: grad = 64; break;
    }
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    GLYPHMETRICS gm;
    CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
    DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
    BYTE* pMono = new BYTE[size];
    GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);
    // デバイスコンテキストとフォントハンドルはもういらないので解放
    SelectObject(hdc, oldFont);
    ReleaseDC(NULL, hdc);

    // テクスチャ作成
    gm.gmBlackBoxX = (gm.gmBlackBoxX + 3) / 4 * 4;
    D3D11_TEXTURE2D_DESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.Width = gm.gmBlackBoxX;
    desc.Height = gm.gmBlackBoxY;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// RGBA(255,255,255,255)タイプ
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;			// 動的（書き込みするための必須条件）
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// シェーダリソースとして使う
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUからアクセスして書き込みOK
    ID3D11Texture2D* pTexture;
    Device->CreateTexture2D(&desc, 0, &pTexture);

    // フォントの書き込み
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = Context->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    BYTE* pData = (BYTE*)mapped.pData;
    DWORD iBmp_w = gm.gmBlackBoxX;
    DWORD iBmp_h = gm.gmBlackBoxY;
    DWORD x, y;
    DWORD Alpha, Color;
    for (y = 0; y < iBmp_h; y++) {
        for (x = 0; x < iBmp_w; x++) {
            Alpha = pMono[x + iBmp_w * y] * 255 / grad;
            if (code == 0x20 || code == 0x3000)//半角全角スペースは左下に線が入るので、Colorを0にする
                Color = 0x0;
            else
                Color = 0x00ffffff | (Alpha << 24);
            memcpy((BYTE*)pData + mapped.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
        }
    }
    Context->Unmap(pTexture, 0);
    gm.gmptGlyphOrigin.y += tm.tmDescent;
    
    ID3D11ShaderResourceView* obj = 0;
    Device->CreateShaderResourceView(pTexture, 0, &obj);
    Cntnr->fonts.emplace_back(obj, gm, TextSize);
    pTexture->Release();
    delete[] pMono;

    return int(Cntnr->fonts.size() - 1);
}
void textMode(TEXT_MODE mode) {
    TextMode = mode;
}
TEXT_MODE getTextMode() {
    return TextMode;
}
//FONTデータを使って、１文字表示する
void drawFont(CNTNR::FONT* font, float x, float y) {
    World.identity();
    if (TextMode == TOP)
        World.mulTranslate(x + (float)font->gm.gmptGlyphOrigin.x,
            y - (float)font->gm.gmptGlyphOrigin.y + font->size, 0.0f);
    else if (TextMode == BOTTOM)
        World.mulTranslate(x + (float)font->gm.gmptGlyphOrigin.x,
            y - (float)font->gm.gmptGlyphOrigin.y, 0.0f);
    else if (TextMode == BCENTER)
        World.mulTranslate(x + (float)font->gm.gmptGlyphOrigin.x - font->size * 0.25f,
            y - (float)font->gm.gmptGlyphOrigin.y + font->size * 0.5f, 0.0f);
    else if (TextMode == MBCENTER)
        World.mulTranslate(x + (float)font->gm.gmptGlyphOrigin.x - font->size * 0.5f,
            y - (float)font->gm.gmptGlyphOrigin.y + font->size * 0.5f, 0.0f);
    World.mulScaling((float)font->gm.gmBlackBoxX, (float)font->gm.gmBlackBoxY, 1.0f);
    World.mulTranslate(0, 0.5f, 0);
    // Update variables that change once per frame
    Context->UpdateSubresource(CbWorld, 0, NULL, &World, 0, 0);
    Context->UpdateSubresource(CbColor, 0, NULL, &FillColor, 0, 0);
    Context->VSSetConstantBuffers(0, 1, &CbWorld);
    Context->PSSetConstantBuffers(3, 1, &CbColor);
    // Set vertex buffer
    UINT stride = sizeof(VECTOR); UINT offset = 0;
    Context->IASetVertexBuffers(0, 1, &RectVertexPosBuffer, &stride, &offset);
    stride = sizeof(VECTOR2);
    Context->IASetVertexBuffers(1, 1, &TexCoordBuffer, &stride, &offset);
    Context->IASetInputLayout(ImageVertexLayout);
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Context->PSSetShaderResources(0, 1, &font->texObj);
    Context->VSSetShader(ImageVertexShader, NULL, 0);
    Context->PSSetShader(ImagePixelShader, NULL, 0);
    Context->Draw(4, 0);
}

int FontIdCnt = -1;
void font(const char* fontname) {
    auto itr = Cntnr->fontIdMap.find(fontname);
    if (itr != Cntnr->fontIdMap.end()) {
        Cntnr->currentFont.name = fontname;
        Cntnr->currentFont.id = itr->second;
    }
    else {
        FontIdCnt++;
        Cntnr->fontIdMap[fontname] = FontIdCnt;
        Cntnr->currentFont.name = fontname;
        Cntnr->currentFont.id = FontIdCnt;
    }
}
void textSize(float size) {
    TextSize = (int)size;
}
void text(const char* str, float x, float y){
    size_t l = strlen(str);
    int num = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, (DWORD)l, 0, 0);
    wchar_t c[128] = L"";
    int ret = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, (DWORD)l, c, num);

    CNTNR::KEY key;
    key.fontId = Cntnr->currentFont.id;
    key.fontSize = TextSize;
    for (int i = 0; i < num; i++) {
        key.c = c[i];
        //keyでFONTを検索してあったら表示、なかったらFONTを作って表示
        auto itr = Cntnr->fontMap.find(key);
        if (itr != Cntnr->fontMap.end()) {
            itr->second;
            drawFont(&Cntnr->fonts.at(itr->second), x, y);
            x += Cntnr->fonts.at(itr->second).gm.gmCellIncX;
        }
        else {
            int j = createFont(&c[i]);
            Cntnr->fontMap[key] = j;
            drawFont(&Cntnr->fonts.at(j), x, y);
            x += Cntnr->fonts.at(j).gm.gmCellIncX;
        }
    }
}
void text(double n, float x, float y){
    int w = (int)n;
    char str[128];
    if (w == n) {
        sprintf_s(str, "%d", w);
    }
    else {
        sprintf_s(str, "%f", n);
    }
    text(str, x, y);
}
void text(int n, float x, float y) {
    char str[128];
    sprintf_s(str, "%d", n);
    text(str, x, y);
}
void text(let l, float x, float y) {
    if (l.str())
        text(l.str(), x, y);
    else
        text((double)l, x, y);
}
void text(let l, const VECTOR& p, TEXT_MODE mode, const COLOR& c, float size) {
    textMode(mode);
    fill(c);
    textSize(size);
    if (l.str())
        text(l.str(), p.x, p.y);
    else
        text((double)l, p.x, p.y);
}
//print function
float PrintSize = 50;
float PrintPosX = 0;
float PrintPosY = 0;
void printStart() {
    PrintPosY = 0;
}
void printSize(float size) { 
    PrintSize = size;
}
void printPosX(float x) { 
    PrintPosX = x;
}
void printPosY(float y) { 
    PrintPosY = y;
}
void printColor(float r, float g, float b) { 
    fill(r, g, b);
}
void print(let textInfo) {
    textMode(TOP);
    textSize(PrintSize);
    text(textInfo, PrintPosX, PrintPosY);
    PrintPosY += PrintSize;
    textMode(BOTTOM);
    textSize((float)TextSize);
}
void print(let textInfo, const COLOR& c, float size) {
    textMode(TOP);
    PrintSize = size;
    textSize(PrintSize);
    fill(c);
    text(textInfo, PrintPosX, PrintPosY);
    PrintPosY += size+size*0.1f;
    textMode(BOTTOM);
    textSize((float)TextSize);
}
//extention
#include "CONTAINER/VERTEX_FORMATS.h"
#include "CONTAINER/VERTEX_BUFFER.h"
#include "CONTAINER/INDEX_BUFFER.h"
#include "CONTAINER/BATCH.h"
#include "CONTAINER/TEXTURE.h"

void createVertexBuffer(unsigned byteWidth, void* vertices, ID3D11Buffer** obj)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = byteWidth;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = vertices;

    HRESULT hr = Device->CreateBuffer(&bufferDesc, &initData, obj);
}

void createIndexBuffer(unsigned num, unsigned short* indices, ID3D11Buffer** dxObj) {
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(unsigned short) * num;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = indices;

    HRESULT hr = Device->CreateBuffer(&bufferDesc, &initData, dxObj);
    WARNING(FAILED(hr), "Error", "インデックスバッファが生成できません");
}

void createTexture(const char* fileName, TEXTURE_OBJ** dxObj,
    int* texWidth, int* texHeight) {
    void* pixels = 0;
    //bool tgaFlag = isTGA(fileName);
    //if (tgaFlag) {
    //    pixels = tga_load(fileName, texWidth, texHeight);
    //}
    //else {
        pixels = stbi_load(fileName, texWidth, texHeight, 0, 4);
    //}
    WARNING(!pixels, fileName, "テクスチャが読み込めません");

    D3D11_SUBRESOURCE_DATA sd;
    sd.pSysMem = pixels;
    sd.SysMemPitch = (UINT)(*texWidth * 4);
    sd.SysMemSlicePitch = (UINT)(*texWidth * *texHeight * 4);
    D3D11_TEXTURE2D_DESC td;
    td.Width = *texWidth;
    td.Height = *texHeight;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_IMMUTABLE;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    ID3D11Texture2D* texture = 0;
    Device->CreateTexture2D(&td, &sd, &texture);
    Device->CreateShaderResourceView(texture, 0, dxObj);
    texture->Release();

    //if (tgaFlag) {
    //    tga_image_free();
    //}
    //else {
        stbi_image_free(pixels);
    //}
}

void draw3D(VERTEX_BUFFER* vb, INDEX_BUFFER* ib, SUBSET* subsets, int numSubsets, unsigned stride) {
    unsigned offset = 0;
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->IASetVertexBuffers(0, 1, vb->obj(), &stride, &offset);
    Context->IASetIndexBuffer(ib->dxObj(), DXGI_FORMAT_R16_UINT, 0);
    for (int i = 0; i < numSubsets; i++) {
        Context->PSSetShaderResources(0, 1, subsets[i].texture->obj());
        Context->DrawIndexed(subsets[i].numDrawPolygons * 3, subsets[i].startIdx, 0);
    }
}

//debug
void draw3D(VERTEX_BUFFER* vb, INDEX_BUFFER* ib, TEXTURE* t, unsigned stride) {
    unsigned offset = 0;
    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Context->IASetVertexBuffers(0, 1, vb->obj(), &stride, &offset);
    Context->IASetIndexBuffer(ib->dxObj(), DXGI_FORMAT_R16_UINT, 0);
    Context->PSSetShaderResources(0, 1, t->obj());
    Context->DrawIndexed(ib->num(), 0, 0);
}

ID3D11Device* device() { return Device; }
ID3D11DeviceContext* context() { return Context; }


VECTOR2 getTextureSize(int idx) {

    CNTNR::TEXTURE& texture = Cntnr->textures.at(idx);

    return VECTOR2(texture.width, texture.height);
}
