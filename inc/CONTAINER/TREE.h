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
    void setNextAnimation(const ANIMATION* animation, double numMorphFrames, double startFrame=0.0, double animationSpeed=0.5 );//���̃A�j���[�V�������Z�b�g����ƁAnumMorphFrames�Ń��[�t�B���O���Ă���
    void setAnimationSpeed( double animationSpeed );
    void update( const MATRIX& world );//�e�m�[�h��translate,rotate,scale���A�j���[�V�����f�[�^�ōX�V���A�SWorldArray���X�V���Ă��܂�
    void draw( SHADER* shader );
    NODE* node( const char* name );
    double frameNumber();
    const MATRIX* nodeWorld( const char* name ) const;
private:
    void Create( const TREE_ORIGIN* );
    void Destroy();
    NODE* Nodes;
    int NumNodes;
    double FrameNumber;//�A�j���[�V�����̐i�s�t���[��
    double AnimationSpeed;

    MATRIX* WorldArray;//World Matrix Array �X�L���A�j���[�V����
    ID3D11Buffer*  CBWorldArray;

    double FrameNumber2;//���[�t�B���O�p�A�j���[�V�����̐i�s�t���[��
    double AnimationSpeed2;
    double Weight2;//���[�t�B���O�p
    double Decrease2;//���[�t�B���O�pWeight2�����炵�Ă����l
};

#endif
