#include<d3d11.h>
#include"VECTOR.h"
#include"SHADER.h"

class LAMBERT : public SHADER{
public:
    LAMBERT();
    ~LAMBERT();
    int numPath(){ return 1; }
    void beginPath( int path );
    void endPath();
    void setShader();
    void setShaderSkin();
    void setWorld( const MATRIX& world );
    void setWorldArray( ID3D11Buffer* cbWorldArray, const MATRIX* worldArray );
	void setLightPosition( const VECTOR& lightPosition );
	void setLightAmbient( float ambient );
	void setDiffuse( const COLOR& diffuse );
    void setProjView( const MATRIX& projView );
private:
    ID3D11Device* Device;
    ID3D11DeviceContext* ImmediateContext;

    void init();
	ID3D11VertexShader* VertexShader;
	ID3D11InputLayout*  InputLayout;
	ID3D11VertexShader* VertexShaderSkin;
	ID3D11InputLayout*  InputLayoutSkin;
	ID3D11PixelShader*  PixelShader;
	ID3D11Buffer*       CBLight;
	ID3D11Buffer*       CBDiffuse;
	ID3D11Buffer*       CBProjView;
	ID3D11Buffer*       CBWorld;
    enum CONSTANT_BUFFER_ID{ 
        CBID_LIGHT, CBID_DIFFUSE, dummy2, dummy3, dummy4, dummy5, 
        CBID_PROJ_VIEW, CBID_WORLD, CBID_WORLD_ARRAY 
    };
	struct LIGHT{
		VECTOR pos;
		float ambient=1;
		LIGHT(){}
	}Light;
};