#pragma once




// �÷��̾�
UENUM(BlueprintType) // ���� enum�� �������Ʈ���� ������ �Ұ����ѵ� ������ �����ϰ� ������� ����ϴ� ��ũ��
enum class ECharacterState : uint8 // ĳ������ �� ���¿� ���� �ൿ�� �޶���, �տ�E�� �ٿ��� ���¶�°� �˷��� // uint8�� ��Ʈ32�� ����Ѱɷ� ��Ʈ32 ��ŭ ���� �޸𸮸� ���� �ʿ䰡 ��� 8��ŭ�� ����ϱ����� �������
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),      // ���û���, ���� ���� ù��°�� 0 �ؿ��Ѽչ���� 1������ // ������¾տ�ECS�� ���� , UMETA��ũ�δ� �������Ʈ���� �̸��� �ٸ� �������� ���̰� ��������� ��
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One Handed Weapon"),  // �Ѽչ���������
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two Handed Weapon") // ��չ��� ������

};






// ���ʹ�
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"), // ����
	EES_Patrolling UMETA(DisplayName = "Patrolling"), // ����
	EES_Chasing UMETA(DisplayName = "Chasing"),  // �߰�
	EES_Attacking UMETA(DisplayName = "Attacking"), //����
	EES_Engaged UMETA(DisplayName = "Engaged"),

};