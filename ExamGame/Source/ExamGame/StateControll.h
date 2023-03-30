#pragma once

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_EnemyIdle UMETA(DisplayName = "Idle"),
	EES_EnemyPatrolling UMETA(DisplayName = "Patrolling"),
	EES_EnemyChaseing UMETA(DisplayName = "Chaseing"),
	EES_EnemyAttacking UMETA(DisplayName = "Attacking"),

};