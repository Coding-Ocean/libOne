#if( _DEBUG )

#pragma warning( disable : 4996 )

#include"common.h"
#include"VECTOR.h"
#include"GRAPHIC.h"
#include"CONVERTER\\convertUtil.h"
#include"CONVERTER\\CONVERT_MA.h"

CONVERT_MA::CONVERT_MA()
{
    clear();
    fill(255);
    text("Converting FBX to Asset",20,70);
}

void CONVERT_MA::fbxToCollisionData(const char* fbxFileName, const char* outFileName){
    FbxManager* fbxManager = 0;
    FbxScene* fbxScene = 0;
    bool result;
    InitializeSdkObjects( fbxManager, fbxScene );
    result = LoadScene( fbxManager, fbxScene, fbxFileName );
    WARNING( result == false, "FBXファイルを開けません", fbxFileName );

    FP = fopen( outFileName, "wt" );
    ExtractCollisionInfo( fbxScene );
    fclose( FP );

    DestroySdkObjects( fbxManager, result );
}

void CONVERT_MA::fbxToTree(const char* fbxFileName, const char* outFileName, const char* textureFilePath){
    Names = new std::vector<std::string>;//テクスチャファイル名の重複出力をさけるために使用
    TextureFilePath = textureFilePath;//このパスがテクスチャファイル名の前に付加される
    //outFileNameからFileNameを取り出しTREE名とする。
    GetFileName( outFileName );

    //TREEデータ抽出
    FbxManager* fbxManager = 0;
    FbxScene* fbxScene = 0;
    bool result;
    InitializeSdkObjects(fbxManager, fbxScene);
    result = LoadScene(fbxManager, fbxScene, fbxFileName);
    WARNING(result == false, "FBXファイルを開けません", fbxFileName);
    FP = fopen( outFileName, "wt" );
    ExtractNodeInfo(fbxScene);
    ExtractContent( fbxScene );//VERTICIES、BATCHS、TEXTURES
    ExtractGenericInfo( fbxScene );//TREE
    fclose( FP );

    SAFE_DELETE_ARRAY( Nodes );
    DestroySdkObjects( fbxManager, result );
    SAFE_DELETE( Names );
}

void CONVERT_MA::fbxToAnim(const char* fbxFileName, const char* outFileName){
    //outFileNameからFileNameを取り出しANIMATION名とする
    GetFileName( outFileName );

    //ANIMATIONデータ抽出
    FbxManager* fbxManager = 0;
    FbxScene* fbxScene = 0;
    bool result;
    InitializeSdkObjects( fbxManager, fbxScene );
    result = LoadScene( fbxManager, fbxScene, fbxFileName );
    WARNING( result == false, "FBXファイルを開けません", fbxFileName );
    FP = fopen( outFileName, "wt" );
    ExtractNodeInfo( fbxScene );
    ExtractAnimation( fbxScene );//ANIMATION
    fclose( FP );
    SAFE_DELETE_ARRAY(Nodes);
    DestroySdkObjects(fbxManager, result);
}

void CONVERT_MA::fbxToTreeAnim( const char* fbxFileName, const char* outFileName, const char* textureFilePath ){
    Names = new std::vector<std::string>;
    TextureFilePath = textureFilePath;
    //outFileNameからFileNameを取り出しTREE名およびANIMATION名とする。
    GetFileName( outFileName );

    //TREEおよびANIMATIONデータ抽出
    FbxManager* fbxManager = 0;
    FbxScene* fbxScene = 0;
    bool result;
    InitializeSdkObjects( fbxManager, fbxScene );
	result = LoadScene( fbxManager, fbxScene, fbxFileName );
    WARNING( result == false, "FBXファイルを開けません", fbxFileName );
    FP = fopen( outFileName, "wt" );
    ExtractNodeInfo(fbxScene);
    ExtractContent( fbxScene );//VERTICIES、BATCHS、TEXTURES
    ExtractGenericInfo( fbxScene );//TREE
    ExtractAnimation( fbxScene );//ANIMATION
    fclose( FP );
    SAFE_DELETE_ARRAY(Nodes);
    DestroySdkObjects( fbxManager, result );
    SAFE_DELETE( Names );
}

void CONVERT_MA::GetFileName( const char* outFileName ){
    size_t len = strlen( outFileName ) + 1;
    char* tempFileName = new char[ len ];
    strcpy_s( tempFileName, len, outFileName );

    char* p = 0;
    //拡張子の前の'.'をNULLに置き換える
    p = strrchr( tempFileName, '.' );
    if( p ) *p = 0; else WARNING( 1, "ファイル名に拡張子がない", outFileName );
    //パス名を除いたファイル名の先頭アドレスを取得
    p = strrchr( tempFileName, '/' );
    if( p ) p++; else WARNING( 1, "パス名を指定していない", outFileName );

    FileName[ 0 ] = 0;
    strcpy_s( FileName, 128, p );

    SAFE_DELETE_ARRAY( tempFileName );
}

//次の呼び出し構造で、Node名をNodes構造体型配列に格納する
//void ExtractNodeInfo(FbxScene* scene);
//  void CountNumNodes(FbxNode* node);
//  void ExtractNodeNames(FbxNode* node);
//      void ExtractNodeName(FbxNode* node);
void CONVERT_MA::ExtractNodeInfo(FbxScene* scene){
    //ボーンidxを得るためにする
    NumNodes = 0;
    CountNumNodes(scene->GetRootNode());
    ExtractNodeNames(scene->GetRootNode());
    //スキンウェイトおよびボーンマトリックスをいったんバッファへ
    //GetBone(node->GetChild(0));
}
void CONVERT_MA::CountNumNodes(FbxNode* node){
    NumNodes++;
    for (int i = 0; i < node->GetChildCount(); i++){
        CountNumNodes(node->GetChild(i));
    }
}
void CONVERT_MA::ExtractNodeNames(FbxNode* node){
    Nodes = new NODE[NumNodes];
    NodeIdx = 0;
    ExtractNodeName(node);
//debug
    //for (int i = 0; i < NumNodes; i++){
    //    printf("%s\n", Nodes[i].name);
    //}
}
void CONVERT_MA::ExtractNodeName(FbxNode* node){
    Nodes[NodeIdx].name = node->GetName();
    NodeIdx++;
    for (int i = 0; i < node->GetChildCount(); i++){
        ExtractNodeName(node->GetChild(i));
    }
}



void CONVERT_MA::ExtractContent(FbxScene* scene){
    FbxNode* rootNode = scene->GetRootNode();

    for (int i = 0; i < rootNode->GetChildCount(); i++){
        ExtractContent(rootNode->GetChild(i));
    }
}
void CONVERT_MA::ExtractContent(FbxNode* node){
    if( node->GetNodeAttribute() ){
        FbxNodeAttribute::EType attributeType = node->GetNodeAttribute()->GetAttributeType();
        switch ( attributeType ){
        case FbxNodeAttribute::eMesh:
            ExtractMesh(node);
            break;
        }
    }
    
    for( int i = 0; i < node->GetChildCount(); i++){
        ExtractContent( node->GetChild(i) );
    }
}
void CONVERT_MA::ExtractMesh( FbxNode* node ){
    size_t len = strlen( node->GetName() ) + 1;
    MeshName = new char[ len ];
    strcpy_s( MeshName, len, node->GetName() );

    ExtractPivotPoint(node);
    FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();
    ExtractBone(fbxMesh);
    ExtractPolygons(fbxMesh);
    ExtractTextureName( fbxMesh );
    ExtractMaterialMapping( fbxMesh );

    SAFE_DELETE_ARRAY( MeshName );
    SAFE_DELETE_ARRAY(TempSkin);
}
//ピボットポイントを原点に持ってくるように頂点位置を座標変換するマトリックスを作る
void CONVERT_MA::ExtractPivotPoint(FbxNode* node){

    FbxVector4 lTmpVector;

    Matrix.identity();

    // Translation
    //lTmpVector = node->GetRotationOffset(FbxNode::eSourcePivot);
    //Matrix.mulTranslate((float)lTmpVector[0], (float)lTmpVector[1], (float)lTmpVector[2]);
    lTmpVector = node->GetRotationPivot(FbxNode::eSourcePivot);
    //lTmpVector = node->GetGeometricTranslation(FbxNode::eSourcePivot);
    Matrix.mulTranslate(-(float)lTmpVector[0], -(float)lTmpVector[1], -(float)lTmpVector[2]);

    //// Rotation
    //lTmpVector = node->GetGeometricRotation(FbxNode::eSourcePivot);
    //Matrix.mulRotateY(float(lTmpVector[1] * 3.141592 / 180.0));
    //Matrix.mulRotateZ(float(lTmpVector[2] * 3.141592 / 180.0));
    //Matrix.mulRotateX(float(lTmpVector[0] * 3.141592 / 180.0));

    //// Scaling
    //lTmpVector = node->GetGeometricScaling(FbxNode::eSourcePivot);
    //Matrix.mulScale((float)lTmpVector[0], (float)lTmpVector[1], (float)lTmpVector[2]);
}
void CONVERT_MA::ExtractBone(FbxMesh* fbxMesh){
    int lControlPointsCount = fbxMesh->GetControlPointsCount();
    SkinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    TempSkin = 0;
    if (SkinCount){
        TempSkin = new TEMP_SKIN[lControlPointsCount];
        int lClusterCount = ((FbxSkin *)fbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetClusterCount();
        for (int j = 0; j != lClusterCount; ++j){

            FbxCluster* lCluster = ((FbxSkin *)fbxMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(j);

            //このボーンのindexを調べておく
            int idx;
            for (idx = 0; idx < NumNodes; idx++){
                FbxString nodeName = lCluster->GetLink()->GetName();
                if (nodeName.Compare(Nodes[idx].name) == 0){
                    break;
                }
            }

            //頂点ごとのworldIdxとweightをTempSkin配列にとっておく
            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();
            for (k = 0; k < lIndexCount; k++)
            {
                int l;
                for (l = 0; l < 6; l++){
                    if (TempSkin[lIndices[k]].worldIdx[l] == -1){
                        TempSkin[lIndices[k]].worldIdx[l] = idx;
                        TempSkin[lIndices[k]].weight[l] = (float)lWeights[k];
                        break;
                    }
                }
                if (l >= 3){
                    for (int ii = 0; ii < 5; ii++){
                        for (int jj = ii+1; jj < 6; jj++){
                            if (TempSkin[lIndices[k]].weight[ii] < TempSkin[lIndices[k]].weight[jj]){
                                int tempIdx = TempSkin[lIndices[k]].worldIdx[ii];
                                TempSkin[lIndices[k]].worldIdx[ii] = TempSkin[lIndices[k]].worldIdx[jj];
                                TempSkin[lIndices[k]].worldIdx[jj] = tempIdx;
                                float tempWeight = TempSkin[lIndices[k]].weight[ii];
                                TempSkin[lIndices[k]].weight[ii] = TempSkin[lIndices[k]].weight[jj];
                                TempSkin[lIndices[k]].weight[jj] = tempWeight;
                            }

                        }
                    }
                }
            }

            //このボーンが影響する頂点をピボットポイントへ移動するmatrixを対応するNodesにセットしておく
            FbxAMatrix matrix;
            FbxVector4 vector4;
            if (Nodes[idx].setFlag == 0){
                matrix = lCluster->GetTransformLinkMatrix(matrix);

                vector4 = matrix.GetS();
                Nodes[idx].matrix.mulScaling(float(1.0/vector4[0]), float(1.0/vector4[1]), float(1.0/vector4[2]));

                vector4 = matrix.GetR();
                Nodes[idx].matrix.mulRotateX((float)(vector4[0] * 3.141592 / 180.0));
                Nodes[idx].matrix.mulRotateZ((float)(vector4[2] * 3.141592 / 180.0));
                Nodes[idx].matrix.mulRotateY((float)(vector4[1] * 3.141592 / 180.0));

                vector4 = matrix.GetT();
                Nodes[idx].matrix.mulTranslate((float)-vector4[0], (float)-vector4[1], -(float)vector4[2]);

                Nodes[idx].setFlag = 1;
            }

             //このメッシュの頂点をデフォルトの位置に座標変換するマトリックスを作っておく
            matrix = lCluster->GetTransformMatrix(matrix);
            vector4 = matrix.GetT();
            Matrix.identity();
            Matrix.mulTranslate((float)vector4[0], (float)vector4[1], (float)vector4[2]);

            vector4 = matrix.GetR();
            Matrix.mulRotateY((float)(vector4[1] * 3.141592 / 180.0));
            Matrix.mulRotateZ((float)(vector4[2] * 3.141592 / 180.0));
            Matrix.mulRotateX((float)(vector4[0] * 3.141592 / 180.0));

            vector4 = matrix.GetS();
            Matrix.mulScaling((float)vector4[0], (float)vector4[1], (float)vector4[2]);
        }
        //使わないTempSkin[i].worldIdx[j]0クリア
        for (int i = 0; i < lControlPointsCount; i++){
            for (int j = 0; j < 6; j++){
                if (TempSkin[i].worldIdx[j] == -1){
                    TempSkin[i].worldIdx[j] = 0;
                }
            }
        }
    }
}
void CONVERT_MA::ExtractPolygons( FbxMesh* fbxMesh )
{
    //三角ポリゴンかチェック
    int numPolygons = fbxMesh->GetPolygonCount();
    for( int i = 0; i < numPolygons; i++ ){
        int polygonSize = fbxMesh->GetPolygonSize( i );
        WARNING( polygonSize != 3, "三角形分割してください", "" );
    }

    //Common variables
    FbxLayerElement::EMappingMode	mapMode;
    FbxLayerElement::EReferenceMode	refMode;

    //インデックスをoIndicesに抽出する(ＵＶのインデックスを頂点のインデックスとする)
    int numIndices = numPolygons * 3;
    int* oIndices = new int[ numIndices ]();
    FbxGeometryElementUV* iUV;
    int ofstIdx = 0;
    int k = 0;
    int numUVs = fbxMesh->GetElementUVCount();
    for( int i = 0; i < numUVs; i++ ){
        iUV = fbxMesh->GetElementUV( i );
        //mapMode = iUV->GetMappingMode();
        //refMode = iUV->GetReferenceMode();
        const char* uvName = iUV->GetName();
        int indexNum = iUV->GetIndexArray().GetCount();
        for( int j = 0; j < indexNum; j++ ){
            int uvIdx = iUV->GetIndexArray().GetAt( j );
            if( uvIdx != -1 ){
                int outIdx = uvIdx + ofstIdx;
                oIndices[ k++ ] = outIdx;
            }
        }
        int numUV = iUV->GetDirectArray().GetCount();
        ofstIdx += numUV;
    }

    //頂点情報をoVerticesに抽出する
    FbxVector4* iPositions = fbxMesh->GetControlPoints();
    FbxGeometryElementNormal* iNormal = fbxMesh->GetElementNormal();
    int* posIdxs = fbxMesh->GetPolygonVertices();
    int numVertices = ofstIdx;    
    VERTEX* oVertices = new VERTEX[ numVertices ];
    for( int i = 0; i < numIndices; i++ ){
        int vIdx = oIndices[ i ];
        int pIdx = posIdxs[ i ];
        //位置
        oVertices[ vIdx ].position[ 0 ] = (float)iPositions[ pIdx ][ 0 ]+Matrix._14;
        oVertices[ vIdx ].position[ 1 ] = (float)iPositions[ pIdx ][ 1 ]+Matrix._24;
        oVertices[ vIdx ].position[ 2 ] = (float)iPositions[ pIdx ][ 2 ]+Matrix._34;
        //法線
        mapMode = iNormal->GetMappingMode();
        refMode = iNormal->GetReferenceMode();
        VECTOR normal;
        if( mapMode == FbxGeometryElement::eByPolygonVertex ){
            if( refMode == FbxGeometryElement::eDirect ){
                normal.x = (float)(iNormal->GetDirectArray().GetAt( i )[ 0 ]);
                normal.y = (float)(iNormal->GetDirectArray().GetAt( i )[ 1 ]);
                normal.z = (float)(iNormal->GetDirectArray().GetAt( i )[ 2 ]);
            }
        }
        else if( mapMode == FbxGeometryElement::eByControlPoint ){
            if( refMode == FbxGeometryElement::eDirect ){
                normal.x = (float)(iNormal->GetDirectArray().GetAt( pIdx )[ 0 ]);
                normal.y = (float)(iNormal->GetDirectArray().GetAt( pIdx )[ 1 ]);
                normal.z = (float)(iNormal->GetDirectArray().GetAt( pIdx )[ 2 ]);
            }
        }
        MATRIX mr = Matrix;
        mr._14 = mr._24 = mr._34 = 0.0f;
        normal = mr * normal;
        oVertices[ vIdx ].normal[ 0 ] = normal.x;
        oVertices[ vIdx ].normal[ 1 ] = normal.y;
        oVertices[ vIdx ].normal[ 2 ] = normal.z;
        //ウェイト
        if (SkinCount){
            for (int l = 0; l < 4; l++){
                oVertices[ vIdx ].worldIdx[ l ] = TempSkin[ pIdx ].worldIdx[ l ];
                oVertices[ vIdx ].weight[l] = TempSkin[pIdx].weight[l];
            }
        }
    }
    //テクスチャ座標
    for( int i = 0; i < numUVs; i++ ){
        iUV = fbxMesh->GetElementUV( i );
        const char* uvName = iUV->GetName();
        mapMode = iUV->GetMappingMode();
        refMode = iUV->GetReferenceMode();
        int indexNum = iUV->GetIndexArray().GetCount();
        for( int j = 0; j < indexNum; j++ ){
            int uvIdx = iUV->GetIndexArray().GetAt( j );
            if( uvIdx != -1 ){
                int vIdx = oIndices[ j ];
                oVertices[ vIdx ].textureCoord[ 0 ] = (float)iUV->GetDirectArray().GetAt( uvIdx )[ 0 ];
                oVertices[ vIdx ].textureCoord[ 1 ] = 1 - (float)iUV->GetDirectArray().GetAt( uvIdx )[ 1 ];
            }
        }
    }

    //頂点出力
    if (SkinCount){
        fprintf(FP, "v %s pntww %d\n", MeshName, numVertices);
    }
    else{
        fprintf(FP, "v %s pnt %d\n", MeshName, numVertices);
    }
    for( int i = 0; i < numVertices; i++ ){
        for( int j = 0; j < 3; j++ ){
            fprintNonZero( FP, oVertices[ i ].position[ j ] );
        }
        fprintf(FP, " ");
        for( int j = 0; j < 3; j++ ){
            fprintNonZero( FP, oVertices[ i ].normal[ j ] );
        }
        fprintf(FP, " ");
        for (int j = 0; j < 2; j++){
            fprintNonZero( FP, oVertices[ i ].textureCoord[ j ] );
        }
        fprintf(FP, " ");
        if (SkinCount){
            for (int j = 0; j < 4; j++){
                fprintf( FP, "%i ", oVertices[i].worldIdx[j] );
            }
            fprintf(FP, " ");
            for (int j = 0; j < 4; j++){
                fprintNonZero(FP, oVertices[i].weight[j] );
            }
        }
        fprintf( FP, "\n" );
    }
    //インデックス出力
    fprintf( FP, "i %s %d\n", MeshName, numIndices );
    for( int i = 0; i < numIndices; i++ ){
        fprintf( FP, "%d ", oIndices[ i ] );
        if( (i+1) % 3 == 0 )fprintf( FP, "\n" );
    }
        
    SAFE_DELETE_ARRAY( oVertices )
    SAFE_DELETE_ARRAY( oIndices )

/*
//if ( mappingMode == KFbxLayerElement::eBY_POLYGON_VERTEX ) {
//    if ( refMode == KFbxLayerElement::eDIRECT ) {
//        // 直接取得
//        for ( int i = 0; i < size; ++i ) {
//            buffer[ i ].x = (float)elem->GetDirectArray().GetAt( i )[ 0 ];
//            buffer[ i ].y = (float)elem->GetDirectArray().GetAt( i )[ 1 ];
//        }
//    }
//    else if ( refMode == KFbxLayerElement::eINDEX_TO_DIRECT ) {
//        // インデックスから取得
//        for ( int i = 0; i < size; ++i ) {
//            int index = elem->GetIndexArray().GetAt( i );
//            buffer[ i ].x = (float)elem->GetDirectArray().GetAt( index )[ 0 ];
//            buffer[ i ].y = (float)elem->GetDirectArray().GetAt( index )[ 1 ];
//        }
//    }
//}
    //インデックス抽出(「テクスチャ座標へのインデックス」を「頂点のインデックス」とする！！！！！！)
    int numPolygons = fbxMesh->GetPolygonCount();
    int numIndices = numPolygons * 3;
    int* indices = new int[ numIndices ];
    fprintf( FP, "i %s %d\n", MeshName, numIndices );
    for (int i = 0; i < numPolygons; i++){
        int polygonSize = fbxMesh->GetPolygonSize(i);
        WARNING( polygonSize != 3, "三角形分割してください", "");
        for ( int j = 0; j < polygonSize; j++){
            FbxGeometryElementUV* eUV = fbxMesh->GetElementUV();
            switch (eUV->GetMappingMode())
            {
                case FbxGeometryElement::eByPolygonVertex:
                {
                    int textureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
                    switch (eUV->GetReferenceMode())
                    {
                        case FbxGeometryElement::eDirect:
                        case FbxGeometryElement::eIndexToDirect:
                            fprintf( FP, "%d ", textureUVIndex );
                            indices[ i * 3 + j ] = textureUVIndex;
                            break;
                        default:
                            break; // other reference modes not shown here!
                    }
                }
                case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
                case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
                case FbxGeometryElement::eNone:      // doesn't make much sense for UVs
                    break;
            }
        } // for polygonSize
        fprintf( FP, "\n" );
    } // for numPolygons


    //頂点データ抽出
    //numVertices++;
    //VERTEX* vertices = new VERTEX[ numVertices ];
    FbxVector4* lControlPoints = fbxMesh->GetControlPoints();
    FbxGeometryElementNormal* leNormal = fbxMesh->GetElementNormal();
    FbxGeometryElementUV* leUV = fbxMesh->GetElementUV();
    int numVertices = leUV->GetDirectArray().GetCount();
    VERTEX* vertices = new VERTEX[ numVertices ];
    for ( int i = 0; i < numPolygons; i++)
    {
        int polygonSize = fbxMesh->GetPolygonSize(i);
        for ( int j = 0; j < polygonSize; j++)
        {
            int vertexId = i * 3 + j;
            int k = indices[ vertexId ];
            int lControlPointIndex = fbxMesh->GetPolygonVertex( i, j );

            //位置・ピボットポイントに座標変換する
            VECTOR3 pos;
            pos.x = (float)lControlPoints[lControlPointIndex][0];
            pos.y = (float)lControlPoints[lControlPointIndex][1];
            pos.z = (float)lControlPoints[lControlPointIndex][2];
            pos = Matrix * pos;
            vertices[ k ].position[ 0 ] = pos.x;
            vertices[ k ].position[ 1 ] = pos.y;
            vertices[ k ].position[ 2 ] = pos.z;
            if (SkinCount){
                for (int l = 0; l < 4; l++){
                    vertices[k].worldIdx[l] = TempSkin[lControlPointIndex].worldIdx[l];
                    vertices[k].weight[l] = TempSkin[lControlPointIndex].weight[l];
                }
            }
            
            //法線
            //FbxGeometryElementNormal* leNormal = fbxMesh->GetElementNormal();
            switch( leNormal->GetMappingMode())
            {
            case FbxGeometryElement::eByPolygonVertex:
                switch (leNormal->GetReferenceMode())
                {
                    case FbxGeometryElement::eDirect:
                    {
                        VECTOR3 normal;
                        normal.x = (float)(leNormal->GetDirectArray().GetAt( vertexId )[ 0 ]);
                        normal.y = (float)(leNormal->GetDirectArray().GetAt( vertexId )[ 1 ]);
                        normal.z = (float)(leNormal->GetDirectArray().GetAt( vertexId )[ 2 ]);
                        MATRIX mr = Matrix;
                        mr._14 = mr._24 = mr._34 = 0.0f;
                        normal = mr * normal;

                        vertices[k].normal[0] = normal.x;
                        vertices[k].normal[1] = normal.y;
                        vertices[k].normal[2] = normal.z;
                        break;
                    }
                    case FbxGeometryElement::eIndexToDirect:
                    {
                        //int id = leNormal->GetIndexArray().GetAt(i);
                        //Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
                        break;
                    }
                    default:
                        break; // other reference modes not shown here!
                }
                break;
            case FbxGeometryElement::eByControlPoint:
                switch( leNormal->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                    {
                        VECTOR3 normal;
                        normal.x = (float)(leNormal->GetDirectArray().GetAt( lControlPointIndex )[ 0 ]);
                        normal.y = (float)(leNormal->GetDirectArray().GetAt( lControlPointIndex )[ 1 ]);
                        normal.z = (float)(leNormal->GetDirectArray().GetAt( lControlPointIndex )[ 2 ]);
                        MATRIX mr = Matrix;
                        mr._14 = mr._24 = mr._34 = 0.0f;
                        normal = mr * normal;

                        vertices[ k ].normal[ 0 ] = normal.x;
                        vertices[ k ].normal[ 1 ] = normal.y;
                        vertices[ k ].normal[ 2 ] = normal.z;
                        break;
                    }
                    case FbxGeometryElement::eIndexToDirect:
                    {
                        //int id = leNormal->GetIndexArray().GetAt( i );
                        //Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
                        break;
                    }
                    default:
                        break; // other reference modes not shown here!
                }
                break;
            }

            //テクスチャ座標
            //FbxGeometryElementUV* leUV = fbxMesh->GetElementUV();
            switch( leUV->GetMappingMode() )
            {
            case FbxGeometryElement::eByControlPoint:
                switch( leUV->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                        //Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
                        break;
                    case FbxGeometryElement::eIndexToDirect:
                    {
                        //int id = leUV->GetIndexArray().GetAt( lControlPointIndex );
                        //Display2DVector(header, leUV->GetDirectArray().GetAt(id));
                        break;
                    }
                    default:
                        break; // other reference modes not shown here!
                }
                break;
            case FbxGeometryElement::eByPolygonVertex:
                int lTextureUVIndex = fbxMesh->GetTextureUVIndex( i, j );
                switch( leUV->GetReferenceMode() )
                {
                    case FbxGeometryElement::eDirect:
                    case FbxGeometryElement::eIndexToDirect:
                        // printf( "         %2d:", lTextureUVIndex );
                        //Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));//ここを通っているようだ
                        vertices[ k ].textureCoord[ 0 ] = (float)(leUV->GetDirectArray().GetAt( lTextureUVIndex )[ 0 ]);
                        vertices[ k ].textureCoord[ 1 ] = 1.0f - (float)(leUV->GetDirectArray().GetAt( lTextureUVIndex )[ 1 ]);
                        break;
                    case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
                    case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
                        break;
                    default:
                        break; // other reference modes not shown here!
                }
                break;
            }
        } // for polygonSize
    } // for numPolygons


    if (SkinCount){
        fprintf(FP, "v %s pntww %d\n", MeshName, numVertices);
    }
    else{
        fprintf(FP, "v %s pnt %d\n", MeshName, numVertices);
    }
    for( int i = 0; i < numVertices; i++ ){
        for( int j = 0; j < 3; j++ ){
            fprintNonZero( FP, vertices[ i ].position[ j ] );
        }
        fprintf(FP, " ");
        for( int j = 0; j < 3; j++ ){
            fprintNonZero( FP, vertices[ i ].normal[ j ] );
        }
        fprintf(FP, " ");
        for (int j = 0; j < 2; j++){
            fprintNonZero( FP, vertices[ i ].textureCoord[ j ] );
        }
        fprintf(FP, " ");
        if (SkinCount){
            for (int j = 0; j < 4; j++){
                fprintf( FP, "%i ", vertices[i].worldIdx[j] );
            }
            fprintf(FP, " ");
            for (int j = 0; j < 4; j++){
                fprintNonZero(FP, vertices[i].weight[j] );
            }
        }
        fprintf( FP, "\n" );
    }

    SAFE_DELETE_ARRAY(vertices);
    SAFE_DELETE_ARRAY(indices);
    */
}



//テクスチャファイル名を取り出すための関数３つ（次のような呼び出し構造）
//void CONVERT_MA::ExtractTextureName(FbxGeometry* pGeometry)
//    void CONVERT_MA::FindAndExtractTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
//        void CONVERT_MA::ExtractTextureInfo(FbxTexture* pTexture, int pBlendMode);
void CONVERT_MA::ExtractTextureInfo(FbxTexture* pTexture, int pBlendMode)
{
	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
	FbxProceduralTexture *lProceduralTexture = FbxCast<FbxProceduralTexture>(pTexture);

    //DisplayString("            Name: \"", (char *) pTexture->GetName(), "\"");
	if (lFileTexture)
	{
        if( TextureIdx < NumTextures ){
            //DisplayString("            Type: File Texture");
            //DisplayString("            File Name: \"", (char *) lFileTexture->GetFileName(), "\"");

            //fbxが持つパス名を除いたファイル名をTextureNameにつくる
            size_t len = strlen( lFileTexture->GetFileName() );
            char* tempName = new char[len+1];
            strcpy_s( tempName, len+1, lFileTexture->GetFileName() );
            size_t i;
            for( i = len-1; tempName[ i ] != '\\' && tempName[ i ] != '/' && i > 0; i-- );
            len = strlen( tempName + i + 1 );
            TextureName[ TextureIdx ] = new char[ len + 1 ];
            TextureHandler[ TextureIdx ] = new char[ len + 1 ];
            strcpy_s( TextureName[ TextureIdx ], len+1, tempName + i + 1 );
            strcpy_s( TextureHandler[ TextureIdx ], len+1, tempName + i + 1 );
            for( i = 0; TextureName[ TextureIdx ][ i ] != '.' && TextureName[ TextureIdx ][ i ] != '\0'; i++ );
            TextureHandler[ TextureIdx ][ i ] = '\0';
            delete[] tempName;

            //すでに出力したファイル名かチェックする
            int flag = 0;
            for( size_t i = 0; i < Names->size() && flag == 0; i++ ){
                if( Names->at( i ) == TextureName[ TextureIdx ] ){
                    flag = 1;
                }
            }
            //新規時は出力
            if( flag == 0 ){
                Names->push_back( TextureName[ TextureIdx ] );
                fprintf( FP, "x " );
                fprintf( FP, "%s", TextureHandler[ TextureIdx ] );
                fprintf( FP, " %s%s\n", TextureFilePath, TextureName[ TextureIdx ] );
            }

            TextureIdx++;
        }
	}
}
void CONVERT_MA::FindAndExtractTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex){

    if( pProperty.IsValid() )
    {
		int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();

		for (int j = 0; j < lTextureCount; ++j)
		{
			//Here we have to check if it's layeredtextures, or just textures:
			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			if (lLayeredTexture)
			{
                //DisplayInt("    Layered Texture: ", j);
                FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
                int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
                for(int k =0; k<lNbTextures; ++k)
                {
                    FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
                    if(lTexture)
                    {

                        if(pDisplayHeader){                    
                            //DisplayInt("    Textures connected to Material ", pMaterialIndex);
                            pDisplayHeader = false;
                        }

                        //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                        //Why is that?  because one texture can be shared on different layered textures and might
                        //have different blend modes.

                        FbxLayeredTexture::EBlendMode lBlendMode;
                        lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
                        //DisplayString("    Textures for ", pProperty.GetName());
                        //DisplayInt("        aTexture ", k);  
                        ExtractTextureInfo(lTexture, (int) lBlendMode);   
                    }

                }
            }
			else
			{
				//no layered texture simply get on the property                                         //ここを通っているようだ
                FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
                if(lTexture)
                {
                    //display connected Material header only at the first time
                    if(pDisplayHeader){                    
                        //DisplayInt("    Textures connected to Material ", pMaterialIndex);
                        pDisplayHeader = false;
                    }             

                    //DisplayString("    Textures for ", pProperty.GetName());
                    //DisplayInt("        bTexture ", j);  
                    ExtractTextureInfo(lTexture, -1);
                }
            }
        }
    }//end if pProperty

}
void CONVERT_MA::ExtractTextureName(FbxGeometry* pGeometry)
{
    if(pGeometry->GetNode()==0)
        return;

    int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();

    TextureName = new char*[lNbMat];
    TextureName[0] = 0;
    TextureHandler = new char*[ lNbMat ];
    TextureHandler[0] = 0;
    NumTextures = lNbMat;
    TextureIdx = 0;

    FbxProperty lProperty;
    for (int lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
        FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
        bool lDisplayHeader = true;

        //go through all the possible textures
        if(lMaterial){

            int lTextureIndex;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
                lProperty = lMaterial->FindProperty( FbxLayerElement::sTextureChannelNames[ lTextureIndex ] );
                FindAndExtractTextureInfoByProperty( lProperty, lDisplayHeader, lMaterialIndex );
            }
        }//end if(lMaterial)

    }// end for lMaterialIndex 

    //テクスチャの無いモデル
    if( TextureName[0] == 0 ){
        int flag = 0;
        for( unsigned i = 0; i < Names->size() && flag == 0; i++ ){
            if( Names->at(i) == "dummy.bmp" ){
                flag = 1;
            }
        }
        //新規時は出力
        if( flag == 0 ){
            Names->push_back( "dummy.bmp" );
            fprintf( FP, "x dummy %sdummy.bmp\n", TextureFilePath );
        }
        NumTextures = 0;
        SAFE_DELETE_ARRAY( TextureName );
        SAFE_DELETE_ARRAY( TextureHandler );
    }
}



//batch出力
void CONVERT_MA::ExtractMaterialMapping(FbxMesh* fbxMesh){
    if( NumTextures == 0 ){
        fprintf( FP, "b %s %s %s %d\n", MeshName, MeshName, MeshName, 1 );
        fprintf(FP, "%d %d %s\n", 0, fbxMesh->GetPolygonCount(), "dummy");
        return;
    }

    int*  numDrawPolygons = new int[ NumTextures ];
    for( int i = 0; i < NumTextures; i++ ){
        numDrawPolygons[ i ] = 0;
    }

    const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
    const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct"};

    int lMtrlCount = 0;
    FbxNode* lNode = 0;
    if(fbxMesh){
        lNode = fbxMesh->GetNode();
        if(lNode)
            lMtrlCount = lNode->GetMaterialCount();    
    }

    for (int l = 0; l < fbxMesh->GetElementMaterialCount(); l++)
    {
        FbxGeometryElementMaterial* leMat = fbxMesh->GetElementMaterial( l);
        if (leMat)
        {
            char header[100];
            FBXSDK_sprintf(header, 100, "    Material Element %d: ", l); 
            //DisplayString(header);
            //DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
            //DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

            int lMaterialCount = 0;
            FbxString lString;

            if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
                leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
            {
                lMaterialCount = lMtrlCount;
            }

            if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
                leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
            {
                int i;

                lString = "           Indices: ";

                int lIndexArrayCount = leMat->GetIndexArray().GetCount(); 
                for (i = 0; i < lIndexArrayCount; i++)
                {
                    numDrawPolygons[ leMat->GetIndexArray().GetAt(i) ] += 1;



                    lString += leMat->GetIndexArray().GetAt(i);

                    if (i < lIndexArrayCount - 1)
                    {
                        lString += ", ";
                    }
                }

                lString += "\n";

                //fprintf( FP,lString);

                fprintf( FP, "b %s %s %s %d\n", MeshName, MeshName, MeshName, NumTextures );
                if( NumTextures == 1 ){//テクスチャ１枚だけ
                        fprintf(FP, "%d %d %s\n", 0, fbxMesh->GetPolygonCount(), TextureHandler[0]);
                }
                else{
                    int startDrawIndex = 0;
                    for( int i = 0; i < NumTextures; i++ ){
                        fprintf( FP, "%d %d %s\n", startDrawIndex, numDrawPolygons[ i ], TextureHandler[ i ] );
                        startDrawIndex += numDrawPolygons[ i ] * 3;
                    }
                }
            }
        }
    }
    
    SAFE_DELETE_ARRAY( numDrawPolygons );

    for( int i = 0; i < NumTextures; i++ ){
        SAFE_DELETE_ARRAY( TextureName[ i ] );
        SAFE_DELETE_ARRAY( TextureHandler[ i ] );
    }

    SAFE_DELETE_ARRAY(  TextureName );
    SAFE_DELETE_ARRAY(  TextureHandler );

}



//tree出力
//void CONVERT_MA::ExtractGenericInfo( FbxScene* scene );
//    void CONVERT_MA::ExtractGenericInfo( FbxNode* node, int depth );
//        void CONVERT_MA::ExtractProperties( FbxObject* pObject );

void CONVERT_MA::ExtractGenericInfo( FbxScene* scene ){

    FbxNode* rootNode = scene->GetRootNode();

    fprintf( FP, "t %s\n", FileName );
    fprintf( FP, "{ n %s\n", (const char*)rootNode->GetName());
    
    NodeIdx = 1;
    for( int i = 0; i < rootNode->GetChildCount(); i++ ){
        ExtractGenericInfo( rootNode->GetChild(i), 1 );
    }

    fprintf(FP, "}\n");
}
void CONVERT_MA::ExtractGenericInfo( FbxNode* node, int depth ){

    int i;

    for(i = 0; i < depth; i++){
        fprintf( FP, " " );
    }

    fprintf( FP, "{ n " );

    fprintf( FP, "%s ", node->GetName() );
    SkinMeshNodeFlag = 0;
    if( node->GetNodeAttribute() ){
        FbxNodeAttribute::EType attributeType = node->GetNodeAttribute()->GetAttributeType();
        switch ( attributeType ){
        case FbxNodeAttribute::eMesh:
            fprintf( FP, "b %s ", node->GetName() );
            FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();
            SkinMeshNodeFlag = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
            break;
        }
    }

    //Display generic info about that Node
    ExtractProperties( node );
    NodeIdx++;

    if( node->GetChildCount()==0 ){
        fprintf( FP, " }\n" );
    }
    else{
        fprintf( FP, "\n" );
        for(i = 0; i < node->GetChildCount(); i++){
            ExtractGenericInfo( node->GetChild(i), depth + 1 );
        }
        for(i = 0; i < depth; i++){
            fprintf( FP, " " );
        }
        fprintf( FP, "}\n");
    }
}
void CONVERT_MA::ExtractProperties(FbxNode* node){
    FbxVector4 tmpVector; 
    tmpVector = node->GetRotationPivot( FbxNode::eSourcePivot )
            - node->GetParent()->GetRotationPivot( FbxNode::eSourcePivot );

    FbxObject* pObject = node;
    FbxDouble3 value;
    FbxProperty property = pObject->FindProperty("Lcl Translation");
    if (property.IsValid()){
        //printf("%s\n", (const char*)property.GetName());
        value = property.Get<FbxDouble3>();
        value[ 0 ] += tmpVector[ 0 ];
        value[ 1 ] += tmpVector[ 1 ];
        value[ 2 ] += tmpVector[ 2 ];
        if (value[0] != 0.0 || value[1] != 0.0 || value[2] != 0.0){
            fprintf(FP, "t ");
            fprintNonZero(FP, (float)value[0]);
            fprintNonZero(FP, (float)value[1]);
            fprintNonZero(FP, (float)value[2]);
        }
    }

    property = pObject->GetNextProperty(property);//普通"Lcl Translation"の次は "Lcl Rotation"
    if (property.IsValid() && property.GetName() == "Lcl Rotation"){
        printf("%s\n", (const char*)property.GetName());
        value = property.Get<FbxDouble3>();
        if (value[0] != 0.0 || value[2] != 0.0 || value[1] != 0.0){
            fprintf(FP, "r ");
            fprintNonZero(FP, float(value[0] * 3.14159265359 / 180));
            fprintNonZero(FP, float(value[1] * 3.14159265359 / 180));
            fprintNonZero(FP, float(value[2] * 3.14159265359 / 180));
        }
    }

    property = pObject->GetNextProperty(property);//普通"Lcl Rotaion"の次は "Lcl Scaling"
    if (property.IsValid() && property.GetName() == "Lcl Scaling"){
        printf("%s\n", (const char*)property.GetName());
        value = property.Get<FbxDouble3>();
        if (value[0] != 1.0 || value[2] != 1.0 || value[1] != 1.0){
            fprintf(FP, "s ");
            fprintNonZero(FP, (float)value[0]);
            fprintNonZero(FP, (float)value[1]);
            fprintNonZero(FP, (float)value[2]);
        }
    }

    if (Nodes[NodeIdx].setFlag ){//ボーンノードであればマトリックスがセットされている
        fprintf(FP, "m ");
        fprintNonZero(FP, Nodes[NodeIdx].matrix._11);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._12);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._13);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._14);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._21);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._22);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._23);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._24);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._31);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._32);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._33);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._34);
    }

}

/*
void CONVERT_MA::ExtractProperties(FbxObject* pObject){
    //pObject->FindProperty("");

    int i, lCount = 0;
    FbxProperty lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid()){
        lCount++;
        lProperty = pObject->GetNextProperty(lProperty);
    }
    if (lCount == 0){
        return; // there are no properties to display
    }

    i = 0;
    lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid()){

        //lcl(local) transformのデータ以外は飛ばす
        if (i <= 65 || i >= 69){
            i++;
            lProperty = pObject->GetNextProperty(lProperty);
            continue;
        }

        printf("%s\n", lProperty.GetName());
        //FbxString lString;
        //lString = lProperty.GetLabel();
        //lString = lProperty.GetName();
        //lString = lProperty.GetPropertyDataType().GetName();

        switch (lProperty.GetPropertyDataType().GetType()){
        case eFbxDouble3:
        {
            FbxDouble3 value;

            value = lProperty.Get<FbxDouble3>();
            if (i == 66){
                if (value[0] != 0.0 || value[2] != 0.0 || value[1] != 0.0){
                    fprintf(FP, "t ");
                    fprintNonZero(FP, (float)value[0]);

                    //y-upのときはこれ
                    if (SkinMeshNodeFlag == 1){
                        fprintNonZero(FP, (float)value[1]);
                        fprintNonZero(FP, -(float)value[2]);
                    }
                    else{
                        fprintNonZero(FP, (float)value[2]);
                        fprintNonZero(FP, -(float)value[1]);
                    }

                    //z-upのときはこれだけ
                    //fprintNonZero(FP, (float)value[2]);
                    //fprintNonZero(FP, -(float)value[1]);

                }
            }
            else if (i == 67){
                if (value[0] != 0.0 || value[2] != 0.0 || value[1] != 0.0){
                    fprintf(FP, "r ");
                    fprintNonZero(FP, float(value[0] * 3.14159265359 / 180));
                    fprintNonZero(FP, float(value[2] * 3.14159265359 / 180));
                    fprintNonZero(FP, -float(value[1] * 3.14159265359 / 180));
                }
            }
            else if (i == 68){
                if (value[0] != 1.0 || value[2] != 1.0 || value[1] != 1.0){
                    fprintf(FP, "s ");
                    fprintNonZero(FP, (float)value[0]);
                    fprintNonZero(FP, (float)value[2]);
                    fprintNonZero(FP, (float)value[1]);
                }
            }
        }
        break;
        }
        i++;
        lProperty = pObject->GetNextProperty(lProperty);
    }

    if (Nodes[NodeIdx].setFlag){
        fprintf(FP, "m ");
        fprintNonZero(FP, Nodes[NodeIdx].matrix._11);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._12);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._13);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._14);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._21);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._22);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._23);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._24);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._31);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._32);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._33);
        fprintNonZero(FP, Nodes[NodeIdx].matrix._34);
    }
}
*/


//アニメーションデータ
//void CONVERT_MA::ExtractAnimation( FbxScene* pScene );
//    void CONVERT_MA::ExtractAnimation(FbxAnimStack* pAnimStack, FbxNode* node, bool isSwitcher = false);
//        void CONVERT_MA::ExtractAnimation(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher)
//            void CONVERT_MA::ExtractChannels(FbxNode* node, FbxAnimLayer* animLayer, void (*ExtractCurve) (FbxAnimCurve* pCurve, int rotationDataFlag, FILE* fp ), void (*ExtractListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher)void CONVERT_MA::ExtractCurveKeys(FbxAnimCurve* pCurve, int rotationDataFlag);
void CONVERT_MA::ExtractAnimation(FbxScene* scene){
    FbxGlobalSettings* globalSettings = &scene->GetGlobalSettings();
    //char lTimeString[256];
    //DisplayString("Time Mode : ", FbxGetTimeModeName(pGlobalSettings->GetTimeMode()));
    FbxTimeSpan timeSpan;
    FbxTime     start, end;
    globalSettings->GetTimelineDefaultTimeSpan(timeSpan);
    start = timeSpan.GetStart();
    end = timeSpan.GetStop();
    StartTime = (long)start.GetFrameCount();
    EndTime = (long)end.GetFrameCount();

    for ( int i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); i++ ){
        FbxAnimStack* lAnimStack = scene->GetSrcObject<FbxAnimStack>( i );
        ExtractAnimation( lAnimStack, scene->GetRootNode() );
    }
}
void CONVERT_MA::ExtractAnimation(FbxAnimStack* animStack, FbxNode* node){
    int numAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
    for (int i = 0; i < numAnimLayers; i++)    {
        FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);
        fprintf(FP, "a %s %d\n{\n", FileName, NumNodes);
        ExtractAnimation(animLayer, node);
        fprintf( FP, "}\n" );
    }
}
void CONVERT_MA::ExtractAnimation(FbxAnimLayer* animLayer, FbxNode* node){
    //このアニメーションノードのカーブ数を求め出力
    const char* s[] = { "X", "Y", "Z" };
    int num = 0;
    for( int i = 0; i < 3; i++ ){
        FbxAnimCurve* animCurve;
        animCurve = node->LclTranslation.GetCurve(animLayer, s[ i ]);
        if( animCurve ) num++;
        animCurve = node->LclRotation.GetCurve(animLayer, s[ i ]);
        if( animCurve ) num++;
        animCurve = node->LclScaling.GetCurve(animLayer, s[ i ]);
        if( animCurve ) num++;
    }
    fprintf(FP, " n %s %d\n", node->GetName(), num);
    //各カーブのキーデータを出力
    ExtractChannels(node, animLayer);

    for(int i = 0; i < node->GetChildCount(); i++){
        ExtractAnimation(animLayer, node->GetChild(i));
    }
}
void CONVERT_MA::ExtractChannels(FbxNode* node, FbxAnimLayer* animLayer){

    FbxAnimCurve* animCurve = NULL;

    animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve) ExtractCurveKeys(animCurve, TX);
    animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve) ExtractCurveKeys(animCurve, TY);
    animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve) ExtractCurveKeys(animCurve, TZ);

    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve) ExtractCurveKeys(animCurve, RX);
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve) ExtractCurveKeys(animCurve, RY);
    animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve) ExtractCurveKeys(animCurve, RZ);

    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve) ExtractCurveKeys(animCurve, SX);
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve) ExtractCurveKeys(animCurve, SY);
    animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve) ExtractCurveKeys(animCurve, SZ);
}
static int InterpolationFlagToIndex(int flags)
{
	if( (flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant ) return 1;
    if( (flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear ) return 2;
	if( (flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic ) return 3;
    return 0;
}
static int ConstantmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard ) return 1;
    if( (flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext ) return 2;
    return 0;
}
static int TangentmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto ) return 1;
    if( (flags & FbxAnimCurveDef::eTangentAutoBreak)== FbxAnimCurveDef::eTangentAutoBreak ) return 2;
    if( (flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB ) return 3;
    if( (flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser ) return 4;
    if( (flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak ) return 5;
    if( (flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak ) return 6;
    return 0;
}
static int TangentweightFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone ) return 1;
    if( (flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight ) return 2;
    if( (flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft ) return 3;
    return 0;
}
static int TangentVelocityFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone ) return 1;
    if( (flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight ) return 2;
    if( (flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft ) return 3;
    return 0;
}
void CONVERT_MA::ExtractCurveKeys(FbxAnimCurve* animCurve, CURVE_TYPE curveType){
    static const char* interpolation[] = { "?", "constant", "linear", "cubic"};
    static const char* constantMode[] =  { "?", "Standard", "Next" };
    static const char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
    static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };
    
    FbxTime keyTime;
    int i;
    struct CURVE_KEY{
        long time;
        float value;
        int interpolationIdx;
        float leftSlope;
        float rightSlope;
    };
    int numKeys = animCurve->KeyGetCount();
    CURVE_KEY* curveKeys = new CURVE_KEY[numKeys];
    int noStartKeyFlag = 1;
    int noEndKeyFlag = 1;
    for (i = 0; i < numKeys; i++){
        //set curveKeys[i].time
        keyTime  = animCurve->KeyGetTime(i);
        curveKeys[i].time = (long)keyTime.GetFrameCount();
        if (curveKeys[i].time == 0)
            noStartKeyFlag = 0;
        if (curveKeys[i].time == EndTime)
            noEndKeyFlag = 0;
        //set curveKeys[i].value
        curveKeys[i].value = animCurve->KeyGetValue(i);
        if      (curveType == RX)
            curveKeys[i].value *= 3.141592f / 180.0f;
        else if (curveType == RY)
            curveKeys[i].value *= 3.141592f / 180.0f;
        else if (curveType == RZ)
            curveKeys[i].value *= 3.141592f / 180.0f;
        //else if (curveType == TZ)
        //    curveKeys[i].value *= -1.0f;
        //set curveKeys[i].interpolationIdx
        curveKeys[i].interpolationIdx = InterpolationFlagToIndex(animCurve->KeyGetInterpolation(i));
        //set curveKeys[i].leftSlope and rightSlope
        curveKeys[i].leftSlope = 0;
        curveKeys[i].rightSlope = 0;
        if (curveKeys[i].interpolationIdx == 3){
            if (i > 0){
                FbxTime time = keyTime - animCurve->KeyGetTime(i - 1);
                FbxLongLong interval = time.GetFrameCount();
                curveKeys[i].leftSlope = animCurve->KeyGetLeftTangentWeight(i) * interval * (animCurve->KeyGetLeftDerivative(i) * 0.101f);
                if (curveType == RX || curveType == RY || curveType == RZ)curveKeys[i].leftSlope *= 0.0174533f;
            }
            if (i < numKeys - 1){
                FbxTime time = animCurve->KeyGetTime(i + 1) - keyTime;
                FbxLongLong interval = time.GetFrameCount();
                curveKeys[i].rightSlope = animCurve->KeyGetRightTangentWeight(i) * interval * (animCurve->KeyGetRightDerivative(i) * 0.101f);
                if (curveType == RX || curveType == RY || curveType == RZ)curveKeys[i].rightSlope *= 0.0174533f;
            }
        }
    }

    int finalNumKeys = numKeys;
    if (noStartKeyFlag){
        finalNumKeys++;
    }
    if (noEndKeyFlag){
        finalNumKeys++;
    }

    static const char* curveTypeString[] = { "tx", "ty", "tz", "rx", "ry", "rz", "sx", "sy", "sz" };
    fprintf(FP, "  c %s %d\n", curveTypeString[curveType], finalNumKeys);
    i = 0;
    if (noStartKeyFlag){
        fprintf(FP, "   d 0 ");
        fprintNonZero(FP, curveKeys[i].value);
        fprintf(FP, "%d ", curveKeys[i].interpolationIdx);
        fprintf(FP, "0 0\n");
    }
    while (i < numKeys){
        fprintf(FP, "   d %d ", curveKeys[i].time);
        fprintNonZero(FP, curveKeys[i].value);
        fprintf(FP, "%d ", curveKeys[i].interpolationIdx);
        fprintNonZero(FP, curveKeys[i].leftSlope);
        fprintNonZero(FP, curveKeys[i].rightSlope);
        fprintf(FP, "\n");
        i++;
    }
    if (noEndKeyFlag){
        fprintf(FP, "   d %d ", EndTime);
        fprintNonZero(FP, curveKeys[i-1].value);
        fprintf(FP, "%d ", curveKeys[i-1].interpolationIdx);
        fprintf(FP, "0 0\n");
    }
    SAFE_DELETE_ARRAY( curveKeys );
}

//当たり判定用データ出力
//void CONVERT_MA::ExtractCollisionInfo( FbxScene* scene );
//    void CONVERT_MA::ExtractCollisionInfo( FbxNode* node, int depth );
//        void CONVERT_MA::ExtractCollisionNode( FbxObject* pObject );
void CONVERT_MA::ExtractCollisionInfo( FbxScene* scene ){
    FbxNode* rootNode = scene->GetRootNode();
    for( int i = 0; i < rootNode->GetChildCount(); i++ ){
        ExtractCollisionInfo( rootNode->GetChild(i) );
    }
}
void CONVERT_MA::ExtractCollisionInfo( FbxNode* node ){
    SkinMeshNodeFlag = 0;
    if( node->GetNodeAttribute() ){
        FbxNodeAttribute::EType attributeType = node->GetNodeAttribute()->GetAttributeType();
        switch ( attributeType ){
        case FbxNodeAttribute::eMesh:
            ExtractCollisionNode( node );
            break;
        }
    }
    for( int i = 0; i < node->GetChildCount(); i++){
        ExtractCollisionInfo( node->GetChild(i) );
    }
}
void CONVERT_MA::ExtractCollisionNode( FbxNode* node )
{
    FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();

    //三角ポリゴンかチェック
    int numPolygons = fbxMesh->GetPolygonCount();
    for( int i = 0; i < numPolygons; i++ ){
        int polygonSize = fbxMesh->GetPolygonSize( i );
        WARNING( polygonSize != 3, "三角形分割してください", "" );
    }

    //インデックスをoIndicesに抽出する(ＵＶのインデックスを頂点のインデックスとする)
    int numIndices = numPolygons * 3;
    int* oIndices = new int[ numIndices ]();
    FbxGeometryElementUV* iUV;
    int ofstIdx = 0;
    int k = 0;
    int numUVs = fbxMesh->GetElementUVCount();
    for( int i = 0; i < numUVs; i++ ){
        iUV = fbxMesh->GetElementUV( i );
        const char* uvName = iUV->GetName();
        int indexNum = iUV->GetIndexArray().GetCount();
        for( int j = 0; j < indexNum; j++ ){
            int uvIdx = iUV->GetIndexArray().GetAt( j );
            if( uvIdx != -1 ){
                int outIdx = uvIdx + ofstIdx;
                oIndices[ k++ ] = outIdx;
            }
        }
        int numUV = iUV->GetDirectArray().GetCount();
        ofstIdx += numUV;
    }

    //頂点情報をoVerticesに抽出する
    FbxVector4* iPositions = fbxMesh->GetControlPoints();
    //FbxGeometryElementNormal* iNormal = fbxMesh->GetElementNormal();
    int* posIdxs = fbxMesh->GetPolygonVertices();
    int numVertices = ofstIdx;    
    VERTEX* oVertices = new VERTEX[ numVertices ];
    for( int i = 0; i < numIndices; i++ ){
        int vIdx = oIndices[ i ];
        int pIdx = posIdxs[ i ];
        //位置
        oVertices[ vIdx ].position[ 0 ] = (float)iPositions[ pIdx ][ 0 ];
        oVertices[ vIdx ].position[ 1 ] = (float)iPositions[ pIdx ][ 1 ];
        oVertices[ vIdx ].position[ 2 ] = (float)iPositions[ pIdx ][ 2 ];
    }
    
    //インデックス出力
    fprintf( FP, "i %s %d\n", node->GetName(), numIndices );
    for( int i = 0; i < numIndices; i++ ){
        fprintf( FP, "%d ", oIndices[ i ] );
        if( (i+1) % 3 == 0 )fprintf( FP, "\n" );
    }

    //頂点出力
    if (SkinCount){
        fprintf(FP, "v %s pntww %d\n", node->GetName(), numVertices);
    }
    else{
        fprintf(FP, "v %s pnt %d\n", node->GetName(), numVertices);
    }
    for( int i = 0; i < numVertices; i++ ){
        for( int j = 0; j < 3; j++ ){
            fprintNonZero( FP, oVertices[ i ].position[ j ] );
        }
        fprintf( FP, "\n" );
    }

    SAFE_DELETE_ARRAY( oVertices )
    SAFE_DELETE_ARRAY( oIndices )
}

#endif