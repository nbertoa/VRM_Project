// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UEOSC/Include/UEOSCElement.h"
#include "VMC4UEBlueprintFunctionLibrary.generated.h"

class UVMC4UEStreamingSkeletalMeshTransform;

/**
 * 
 */
UCLASS()
class VMC4UE_API UVMC4UEBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UVMC4UEBlueprintFunctionLibrary();

	UFUNCTION(BlueprintCallable, Category = "VMC")
	static void OnReceivedVMC(UVMC4UEStreamingSkeletalMeshTransform *SkeletalMeshTransform, const FName &Address, const TArray<FUEOSCElement> &Data, const FString &SenderIp);
	static TWeakObjectPtr<UVMC4UEStreamingSkeletalMeshTransform> GetStreamingSkeletalMeshTransform(int32 Port);
	UFUNCTION(BlueprintCallable, Category = "VMC")
	static UVMC4UEStreamingSkeletalMeshTransform* GetStreamingSkeletalMeshTransformBP(int32 Port);

	UFUNCTION(BlueprintCallable, Category = "VMC")
	static void Connect(int32 InA, int32 InB, int32 InC, int32 InD, int32 Port);

	UFUNCTION(BlueprintCallable, Category = "VMC")
	static void Disconnect();

	static void SetBoneTransform(const TArray<FUEOSCElement> &Data, FVector& Position, FQuat& Rotation);
};
