//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// ���͗p�̍\���̒�`
struct BufInType
{
    matrix World;
    matrix AnimationMatrix;
    matrix OffsetMatrix;
    int NumChildren;
};

// �o�͗p�̍\���̒�`
struct BufOutType
{
    int i;
};

// �ǂݎ���p�̍\�����o�b�t�@�[
// �A���I�[�_�[�h�A�N�Z�X�o�b�t�@�[�Ƃ��č쐬���ꂽ�o�b�t�@���g�p����
StructuredBuffer<BufInType> BufferIn : register(t0);

// �ǂݏ����\�ȍ\�����o�b�t�@�[
// �A���I�[�_�[�h�A�N�Z�X�o�b�t�@�[�Ƃ��č쐬���ꂽ�o�b�t�@���g�p����
RWStructuredBuffer<BufOutType> BufferOut : register(u0);

[numthreads(4, 4, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{

}