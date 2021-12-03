#ifndef TREE_HD
#define TREE_HD

class MATRIX;
class NODE;
class TREE_ORIGIN;
class ANIMATION;
struct ID3D11Buffer;
class SHADER;

class TREE{
public:
    TREE();
    TREE( const TREE_ORIGIN* );
    ~TREE();
    void operator=( const TREE_ORIGIN*  );
    void setAnimation( const ANIMATION* animation, double animationSpeed = 0.5 );
    void setNextAnimation(const ANIMATION* animation, double numMorphFrames, double startFrame=0.0, double animationSpeed=0.5 );//次のアニメーションをセットすると、numMorphFramesでモーフィングしていく
    void setAnimationSpeed( double animationSpeed );
    void update( const MATRIX& world );//各ノードのtranslate,rotate,scaleをアニメーションデータで更新し、全WorldArrayも更新してしまう
    void draw( SHADER* shader );
    NODE* node( const char* name );
    double frameNumber();
    const MATRIX* nodeWorld( const char* name ) const;
private:
    void Create( const TREE_ORIGIN* );
    void Destroy();
    NODE* Nodes;
    int NumNodes;
    double FrameNumber;//アニメーションの進行フレーム
    double AnimationSpeed;

    MATRIX* WorldArray;//World Matrix Array スキンアニメーション
    ID3D11Buffer*  CBWorldArray;

    double FrameNumber2;//モーフィング用アニメーションの進行フレーム
    double AnimationSpeed2;
    double Weight2;//モーフィング用
    double Decrease2;//モーフィング用Weight2を減らしていく値
};

#endif
