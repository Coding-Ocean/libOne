#pragma once

#include"common.h"
#include"COLOR.h"
#include"let.h"
#include"VECTOR2.h"

//for users
extern float Width;
extern float Height;
#define width Width
#define height Height

enum COLOR_MODE { RGB, HSV };
void colorMode(COLOR_MODE mode, float colorDenominator=255);
enum RECT_MODE { CENTER, CORNER };
void rectMode(RECT_MODE mode);
enum TEXT_MODE { BOTTOM, TOP, BCENTER, MBCENTER };
void textMode(TEXT_MODE mode);
TEXT_MODE getTextMode();

void clear(const struct COLOR& c);
void clear(float r, float g, float b);
void clear(float c=0);

void stroke(const struct COLOR& c);
void stroke(float r, float g, float b);
void stroke(float r, float g, float b, float a);
void stroke(float c);
void strokeWeight(float weight);
void noStroke();

void fill(const struct COLOR& c);
void fill(float r, float g, float b);
void fill(float r, float g, float b, float a);
void fill(float c);
void noFill();

void point(float x, float y);
void line(float sx, float sy, float ex, float ey);
void arrow(float sx, float sy, float ex, float ey, float size = 30.0f);
void arc(float x, float y, float startAngle, float angle, float radius);
void rect(float x, float y, float w, float h);
void rect(float x, float y, float w, float h, float angle);
void circle(float x, float y, float diameter);
void point(const class VECTOR& p,
	const struct COLOR& strokeColor, float strokeW);
void line(const class VECTOR& p1, const class VECTOR& p2,
	const struct COLOR& strokeColor, float strkWeight);
void arrow(const class VECTOR& s, const class VECTOR& e,
	const struct COLOR& strokeColor, float strokeW);
void circle(const class VECTOR& p, float diameter,
	const struct COLOR& fillColor, const struct COLOR& strokeColor, float strokeW);
void rect(const class VECTOR& p, float w, float h, float angle,
	RECT_MODE mode, 
	const struct COLOR& fillColor, const struct COLOR& strokeColor, float strokeW);

struct SHAPE_VERTEX { float x, y; };
int createShape(struct SHAPE_VERTEX* vertices, int numVertices);
int createShape(struct SHAPE_VERTEX* vertices, int numVertices, long* indices, int numIndices);
void shape(int idx, float x, float y, float angle = 0, float size = 1);
void shape(int idx, const class MATRIX& m);

int loadImage(const char* filename);
int loadImageFromRes(const char* resname);
int cutImage(int idx, int left, int top, int w, int h);
void divideImage(int img, int cols, int rows, int w, int h, int* imgs);
void imageColor(const struct COLOR& c);
void imageColor(float r, float g, float b);
void imageColor(float r, float g, float b, float a);
void imageColor(float c);
void image(int idx, float x, float y, float angle = 0, float size = 1);
void image(int idx, const class VECTOR& p, float angle=0, float size=1, RECT_MODE mode=CORNER);

void triangle3D(const class VECTOR& p0, const class VECTOR& p1, const class VECTOR& p2,
            const COLOR& c0=COLOR(), const COLOR& c1=COLOR(), const COLOR& c2=COLOR());

void font(const char* fontname);
void textSize(float size);
void text(const char* str, float x, float y);
void text(double n, float x, float y);
void text(int n, float x, float y);
void text(let n, float x, float y);
void text(let l, const VECTOR& p, TEXT_MODE mode, const COLOR& c, float size);

void printStart();
void printSize(float size);
void printPosX(float x);
void printPosY(float y);
void printColor(float r, float g, float b);
void print(let textInfo);
void print(let textInfo, const COLOR& c, float size);

//for framework
void initGraphic(int baseWidth, int baseHeight);
void freeGraphic();
void changeSize();
void present();
void refreshCntnr();

//extention
#define VERTEX_BUFFER_OBJ struct ID3D11Buffer
#define TEXTURE_OBJ struct ID3D11ShaderResourceView
enum class VERTEX_TYPE { PNT, PNTWW, PT, P, PC };
void createVertexBuffer(unsigned byteWidth, void* vertices, struct ID3D11Buffer** obj);
void createIndexBuffer(unsigned num, unsigned short* indices, struct ID3D11Buffer** obj);
void createTexture(const char* fileName, TEXTURE_OBJ** obj, int* texWidth, int* texHeight);
void draw3D(class VERTEX_BUFFER* vb, class INDEX_BUFFER* ib, struct SUBSET* subsets, int numSubsets, unsigned stride);
void draw3D( class VERTEX_BUFFER* vb, class INDEX_BUFFER* ib, class TEXTURE* t, unsigned stride);

struct ID3D11Device* device();
struct ID3D11DeviceContext* context();
void createVertexShaderAndInputLayoutFromRes(
    LPCTSTR resName, struct ID3D11VertexShader** vertexShader,
    VERTEX_TYPE vertexType, struct ID3D11InputLayout** inputLayout);
void createPixelShaderFromRes(LPCTSTR resName, struct ID3D11PixelShader** pixelShader);
HRESULT createConstantBuffer(unsigned size, struct ID3D11Buffer** buffer);

#define BEGIN_SHADER_PATH() \
for (int path = 0; path < shader->numPath(); path++) {\
shader->beginPath(path);

#define END_SHADER_PATH() \
shader->endPath();\
}

void setTextureSamplerClamp();
void setTextureSamplerWrap();
void setRasterizerCullBack(); 
void setRasterizerCullNone(); 
void setRasterizerWire();

VECTOR2 getTextureSize(int idx);
