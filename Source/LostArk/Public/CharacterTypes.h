#pragma once


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"), // 순찰
	EES_Chasing UMETA(DisplayName = "Chasing"),  // 추격
	EES_Attacking UMETA(DisplayName = "Attacking"), //공격


};