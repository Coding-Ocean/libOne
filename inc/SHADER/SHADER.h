#ifndef SHADER_HD
#define SHADER_HD

class MATRIX;
class VECTOR;
struct COLOR;
struct ID3D11Buffer;

class SHADER{
public:
    virtual ~SHADER(){};
	virtual int numPath() = 0;
	virtual void beginPath( int path )=0;
	virtual void endPath()=0;
    virtual void setShader(){}
    virtual void setShaderSkin(){}
	virtual void setWorld( const MATRIX& world ){}
    virtual void setWorldArray( ID3D11Buffer* cbWorldArray, const MATRIX* worldArray ){}
	virtual void setLightPosition( const VECTOR& lightPosition ){}
	virtual void setLightAmbient( float ambient ){}
	virtual void setDiffuse( const COLOR& diffuse ){}
	virtual void setProjView( const MATRIX& projView ){}
	virtual void setLight_ProjView( const MATRIX& light_projView ){}
};


#endif