#if( _DEBUG )
#include<string>
#include<vector>
#include"fbxCommon.h"
#include"MATRIX.h"

class CONVERT_MA{
public:
    CONVERT_MA();
    ~CONVERT_MA(){};
    void fbxToTree(const char* fbxFileName, const char* outFileName, const char* textureFilePath="Assets/textures/dummy/");
    void fbxToAnim(const char* fbxFileName, const char* outFileName);
    void fbxToTreeAnim(const char* fbxFileName, const char* outFileName, const char* textureFilePath);
    void fbxToCollisionData(const char* fbxFileName, const char* outFileName);
private:
    void ExtractNodeInfo(FbxScene* scene);
        void CountNumNodes(FbxNode* pNode);
        void ExtractNodeNames(FbxNode* pNode);
            void ExtractNodeName(FbxNode* pNode);
    void ExtractContent(FbxScene* scene);
        void ExtractContent(FbxNode* node);
            void ExtractMesh( FbxNode* node );
            void ExtractPivotPoint(FbxNode* node);
            void ExtractBone(FbxMesh* fbxMesh);
                void ExtractPolygons( FbxMesh* fbxMesh ); //vertices
                void ExtractTextureName( FbxGeometry* pGeometry ); //textures
                    void FindAndExtractTextureInfoByProperty( FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
                        void ExtractTextureInfo( FbxTexture* pTexture, int pBlendMode );
                void ExtractMaterialMapping( FbxMesh* fbxMesh ); //batch
    void ExtractGenericInfo( FbxScene* scene ); //tree
        void ExtractGenericInfo( FbxNode* node, int depth );
            void ExtractProperties( FbxNode* node );
            //void GetMatrixToOrigin(FbxNode* node);
    void ExtractAnimation( FbxScene* scene );
        //void CountNodes( FbxNode* node );
        void ExtractAnimation( FbxAnimStack* pAnimStack, FbxNode* pNode );
            void ExtractAnimation( FbxAnimLayer* pAnimLayer, FbxNode* pNode );
                void ExtractChannels( FbxNode* pNode, FbxAnimLayer* pAnimLayer );
                    enum CURVE_TYPE{ TX, TY, TZ, RX, RY, RZ, SX, SY, SZ };
                    void ExtractCurveKeys(FbxAnimCurve* pCurve, CURVE_TYPE curveType);
    void ExtractCollisionInfo( FbxScene* scene );
        void ExtractCollisionInfo( FbxNode* node );
            void ExtractCollisionNode( FbxNode* node );
    void GetFileName( const char* outFileName );

 
    struct NODE{
        FbxString name;
        bool setFlag=0;
        MATRIX matrix;
    };
    NODE* Nodes=0;
    int NumNodes=0;
    int NodeIdx=0;
    
    struct TEMP_SKIN{
        int worldIdx[6];
        float weight[6];
        TEMP_SKIN(){
            for (int i = 0; i < 6; i++){
                worldIdx[i] = -1;
                weight[i] = 0.0f;
            }
        }
    };
    TEMP_SKIN* TempSkin=0;
    int SkinCount=0;

    struct VERTEX{
        float position[ 3 ];
        float normal[3];
        float textureCoord[2];
        int worldIdx[4] = { 0 };
        float weight[4] = { 0 };
        VERTEX(){
            position[0] = position[1] = position[2] = 0;
            normal[0] = normal[1] = normal[2] = 0;
            textureCoord[0] = textureCoord[1] = 0;
        }
    };

    MATRIX Matrix;//ピボットポイントに座標変換するマトリックス
    char* MeshName;
    FILE* FP;
    int NumTextures;
    char** TextureName;
    char** TextureHandler;
    int TextureIdx;
    char FileName[ 128 ];//ツリー名、アニメーション名にする
    int SkinMeshNodeFlag;
    const char* TextureFilePath;
    std::vector<std::string>* Names;
    int TextureCnt;

    //アニメーション
    long StartTime;
    long EndTime;

};
#endif