#ifndef NAME_HD
#define NAME_HD

class NAME{
public:
    NAME();
    ~NAME();

    //char*
    NAME( const char* s );//�������i�ϊ��R���X�g���N�^�j
    void operator=( const char* s );//���
    int operator==( const char* s ) const;
    int operator!=( const char* s ) const;
    
    //NAME&
    NAME( const NAME& n );//�������i�R�s�[�R���X�g���N�^�j
    void operator=( const NAME& n );//���
    int operator==( const NAME& n ) const;
    int operator!=( const NAME& n ) const;

    //others
    void operator+=( const char* s );//������̘A��
    operator char*(){ return S; }//�ϊ��֐�
    
private:
    char* S=0;
};

#endif