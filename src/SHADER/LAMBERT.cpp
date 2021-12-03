#include<tchar.h>
#include"common.h"
#include"graphic.h"
#include"VECTOR.h"
#include"MATRIX.h"
#include"FILE_BUFFER.h"
#include"SHADER/LAMBERT.h"

LAMBERT::LAMBERT() :
    VertexShader(0),
    InputLayout(0),
    VertexShaderSkin(0),
    InputLayoutSkin(0),
    PixelShader(0),
    CBProjView(0),
    CBWorld(0),
    CBLight(0),
    CBDiffuse(0) {
    init();
}

LAMBERT::~LAMBERT(){
    SAFE_RELEASE( CBDiffuse );
    SAFE_RELEASE( CBLight );
    SAFE_RELEASE( CBProjView );
    SAFE_RELEASE( CBWorld );

    SAFE_RELEASE( PixelShader );
    SAFE_RELEASE( InputLayoutSkin );
    SAFE_RELEASE( InputLayout );
    SAFE_RELEASE( VertexShaderSkin );
    SAFE_RELEASE( VertexShader );
}

void LAMBERT::init(){
    Device = device();
    ImmediateContext = context();

	//--------------------------------------------------------------------------------------------------------
	//シェーダー生成
    createVertexShaderAndInputLayoutFromRes(_T("LAMBERT_VS"), &VertexShader, VERTEX_TYPE::PNT, &InputLayout);
    createVertexShaderAndInputLayoutFromRes(_T("LAMBERT_SKIN_VS"), &VertexShaderSkin, VERTEX_TYPE::PNTWW, &InputLayoutSkin);
    createPixelShaderFromRes(_T("LAMBERT_PS"), &PixelShader);

	//--------------------------------------------------------------------------------------------------------
    //コンスタントバッファ生成
    HRESULT hr;
    hr = createConstantBuffer( sizeof(LIGHT), &CBLight );
    WARNING( FAILED( hr ), "Error", "コンスタントバッファ・CBLightが生成できません" );
    hr = createConstantBuffer( sizeof(COLOR), &CBDiffuse );
    WARNING( FAILED( hr ), "Error", "コンスタントバッファ・CBDiffuseが生成できません" );
    hr = createConstantBuffer( sizeof(MATRIX), &CBProjView );
    WARNING( FAILED( hr ), "Error", "コンスタントバッファ・CBProjViewが生成できません" );
    hr = createConstantBuffer( sizeof(MATRIX), &CBWorld );
    WARNING( FAILED( hr ), "Error", "コンスタントバッファ・CBWorldが生成できません" );
}

void LAMBERT::beginPath( int path ){
}

void LAMBERT::endPath(){
}

void LAMBERT::setShader(){
    ImmediateContext->VSSetShader( VertexShader, NULL, 0 );
    ImmediateContext->PSSetShader( PixelShader, NULL, 0 );
    ImmediateContext->IASetInputLayout( InputLayout );
}

void LAMBERT::setShaderSkin(){
    ImmediateContext->VSSetShader( VertexShaderSkin, NULL, 0 );
    ImmediateContext->PSSetShader( PixelShader, NULL, 0 );
    ImmediateContext->IASetInputLayout( InputLayoutSkin );
}

void LAMBERT::setLightPosition( const VECTOR& lightPosition ){
    Light.pos = normalize( lightPosition );
    ImmediateContext->UpdateSubresource( CBLight, 0, NULL, &Light, 0, 0 );
    ImmediateContext->VSSetConstantBuffers( CBID_LIGHT, 1, &CBLight );
}

void LAMBERT::setLightAmbient( float ambient ){
    Light.ambient = ambient;
    ImmediateContext->UpdateSubresource( CBLight, 0, NULL, &Light, 0, 0 );
    ImmediateContext->VSSetConstantBuffers( CBID_LIGHT, 1, &CBLight );
}

void LAMBERT::setDiffuse( const COLOR& diffuse ){
    ImmediateContext->UpdateSubresource( CBDiffuse, 0, NULL, &diffuse, 0, 0 );
    ImmediateContext->VSSetConstantBuffers( CBID_DIFFUSE, 1, &CBDiffuse );
}

void LAMBERT::setProjView( const MATRIX& projView ){
    ImmediateContext->UpdateSubresource( CBProjView, 0, NULL, &projView, 0, 0 );
    ImmediateContext->VSSetConstantBuffers( CBID_PROJ_VIEW, 1, &CBProjView );
}

void LAMBERT::setWorld( const MATRIX& world ){
    ImmediateContext->UpdateSubresource( CBWorld, 0, NULL, &world, 0, 0 );
    ImmediateContext->VSSetConstantBuffers( CBID_WORLD, 1, &CBWorld );
}

void LAMBERT::setWorldArray( ID3D11Buffer* cbWorldArray, const MATRIX* worldArray ){
    ImmediateContext->UpdateSubresource( cbWorldArray, 0, NULL, worldArray, 0, 0 );	
    ImmediateContext->VSSetConstantBuffers( CBID_WORLD_ARRAY, 1, &cbWorldArray );
}

