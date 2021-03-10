// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UEOSC/Include/UEOscElement.h"
#include "VMC4UEStreamingData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct VMC4UE_API FVMC4UEStreamingBoneTransform
{
	GENERATED_USTRUCT_BODY()
public:
	FVMC4UEStreamingBoneTransform() {}
	~FVMC4UEStreamingBoneTransform() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Bone Data")
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Bone Data")
	FQuat Rotation = FQuat::Identity;
};


USTRUCT(BlueprintType)
struct VMC4UE_API FVMC4UEBodyRawData
{
	GENERATED_USTRUCT_BODY()
public:
	FVMC4UEBodyRawData() {}
	~FVMC4UEBodyRawData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Body Raw Data")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Body Raw Data")
	float Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Body Raw Data")
	float Time;
};


USTRUCT(BlueprintType)
struct VMC4UE_API FVMC4UEHandRawData
{
	GENERATED_USTRUCT_BODY()
public:
	FVMC4UEHandRawData() {}
	~FVMC4UEHandRawData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Hand Raw Data")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Hand Raw Data")
	int32 HandType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Hand Raw Data")
	float Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VMC4UE Hand Raw Data")
	float Time;
};
/**
 *
 */
UCLASS()
class VMC4UE_API UVMC4UEStreamingSkeletalMeshTransform : public UObject
{
	GENERATED_BODY()

public:
	FRWLock RWLock;
	float Time = 0.0f;
	FVMC4UEStreamingBoneTransform Root;
	// Relative coordinates[/VMC/Ext/Bone/Pos]
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FVMC4UEStreamingBoneTransform> Bones;
	TMap<FName, float> CurrentBlendShapes;
	TMap<FName, float> FutureBlendShapes;

	UFUNCTION()
	void OnReceived(const FName &Address, const TArray<FUEOSCElement> &Data, const FString &SenderIp);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FVector> BodyPositions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FQuat> BodyRotations;

	// Absolute coordinates[/VMC/Ext/Root/Pos]
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	FVector RootPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	FQuat RootRotation;

	// Relative coordinates[/VMC/Ext/Tra/Pos/Local]
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FVMC4UEStreamingBoneTransform> BodyTransformMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	FVMC4UEStreamingBoneTransform RootTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FVMC4UEBodyRawData> BodyRawDataMap;

	// Hand Raw Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FVMC4UEHandRawData> RightHandRawDataMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VMC")
	TMap<FName, FVMC4UEHandRawData> LeftHandRawDataMap;
};
