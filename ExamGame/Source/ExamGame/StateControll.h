#pragma once

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_EnemyIdle UMETA(DisplayName = "Idle"),
	EES_EnemyPatrolling UMETA(DisplayName = "Patrolling"),
	EES_EnemyChaseing UMETA(DisplayName = "Chaseing"),
	EES_EnemyReadyToAttack UMETA(DisplayName = "ReadyToAttack"),
	EES_EnemyAttacking UMETA(DisplayName = "Attacking"),

};

UENUM(BlueprintType)
enum class EEnemyAttackState : uint8
{
	EES_EnemyUnoccupied UMETA(DisplayName = "EnemyUnoccupied"),
	EES_EnemyAttacking UMETA(DisplayName = "EnemyAttacking"),
};