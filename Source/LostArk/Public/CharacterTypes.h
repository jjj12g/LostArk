#pragma once




// 플레이어
UENUM(BlueprintType) // 원래 enum은 블루프린트에서 편집이 불가능한데 편집이 가능하게 만들려고 사용하는 매크로
enum class ECharacterState : uint8 // 캐릭터의 현 상태에 따라 행동이 달라짐, 앞에E를 붙여서 상태라는걸 알려줌 // uint8은 인트32와 비슷한걸로 인트32 만큼 많은 메모리를 가질 필요가 없어서 8만큼만 사용하기위해 만들어짐
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),      // 평상시상태, 값을 가짐 첫번째는 0 밑에한손무기는 1식으로 // 보통상태앞에ECS를 붙임 , UMETA매크로는 블루프린트에서 이름이 다른 형식으로 보이게 만들기위한 것
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One Handed Weapon"),  // 한손무기장착시
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two Handed Weapon") // 양손무기 장착시

};






// 에너미
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"), // 죽음
	EES_Patrolling UMETA(DisplayName = "Patrolling"), // 순찰
	EES_Chasing UMETA(DisplayName = "Chasing"),  // 추격
	EES_Attacking UMETA(DisplayName = "Attacking"), //공격
	EES_Engaged UMETA(DisplayName = "Engaged"),

};