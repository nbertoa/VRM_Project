// VRM4U Copyright (c) 2019 Haruyoshi Yamamoto. This software is released under the MIT License.

#include "VrmEditorBPFunctionLibrary.h"
#include "Materials/MaterialInterface.h"

#include "Engine/Engine.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SkeletalMesh.h"
#include "Logging/MessageLog.h"
#include "Engine/Canvas.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/MorphTarget.h"
#include "Misc/EngineVersionComparison.h"
#include "AssetRegistryModule.h"
#include "ARFilter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/LightComponent.h"

#include "Rendering/SkeletalMeshLODModel.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshRenderData.h"

#include "Animation/AnimInstance.h"
#include "VrmAnimInstanceCopy.h"
#include "VrmUtil.h"

#if PLATFORM_WINDOWS
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SWindow.h"
#endif

#include "HAL/ConsoleManager.h"


#if WITH_EDITOR
#include "Editor.h"
#include "EditorViewportClient.h"
#include "EditorSupportDelegates.h"
#include "LevelEditorActions.h"
#include "Editor/EditorPerProjectUserSettings.h"
#endif
#include "Kismet/GameplayStatics.h"

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

//#include "VRM4U.h"


bool UVrmEditorBPFunctionLibrary::VRMBakeAnim(const USkeletalMeshComponent *skc, const FString &FilePath2, const FString &AssetFileName2) {
	if (skc == nullptr) {
		return false;
	}

	FString FilePath = FilePath2;
	FString AssetFileName = AssetFileName2;

	FString dummy[2] = {
		"/Game/",
		"tmpAnimSequence",
	};
	if (FilePath == ""){
		FilePath = dummy[0];
	}
	if (AssetFileName == "") {
		AssetFileName = dummy[1];
	}
	FilePath += TEXT("/");

	while (FilePath.Find(TEXT("//")) >= 0) {
		FilePath = FilePath.Replace(TEXT("//"), TEXT("/"));
	}

	while (AssetFileName.Find(TEXT("/")) >= 0) {
		AssetFileName = AssetFileName.Replace(TEXT("/"), TEXT(""));
	}

	USkeletalMesh *sk = skc->SkeletalMesh;
	USkeleton *k = skc->SkeletalMesh->Skeleton;

	//FString NewPackageName = "/Game/aaaa";
	FString NewPackageName = FilePath + AssetFileName;
	UPackage* Package = CreatePackage(NULL, *NewPackageName);

	UAnimSequence *ase;
	ase = NewObject<UAnimSequence>(Package, *(TEXT("A_") + AssetFileName), EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);

	ase->CleanAnimSequenceForImport();

	ase->SetSkeleton(k);

	float totalTime = 0.f;
	int totalFrameNum = 0;

	const auto componentTransInv = skc->GetComponentTransform().Inverse();

	for (int i = 0; i < k->GetBoneTree().Num(); ++i) {
		FRawAnimSequenceTrack RawTrack;

		const auto BoneName = skc->GetBoneName(i);

		FTransform parentBoneInv = FTransform::Identity;
		if (i > 0) {
			const auto ParentBoneName = skc->GetParentBone(BoneName);
			const auto parentTrans = skc->GetBoneTransform(skc->GetBoneIndex(ParentBoneName));
			auto parentCompTrans = parentTrans * componentTransInv;

			//auto r = parentCompTrans.GetRotation();
			//r = FRotator(0, -90, 0).Quaternion() * r * FRotator(0, 90, 0).Quaternion();
			//parentCompTrans.SetRotation(r);

			parentBoneInv = parentCompTrans.Inverse();
			{

			}
		}

		{
			const auto refPose = sk->RefSkeleton.GetRawRefBonePose()[i];
			RawTrack.PosKeys.Add(refPose.GetLocation());
		}

		const auto srcTrans = skc->GetBoneTransform(i);
		auto compTrans = srcTrans * componentTransInv;
		{
			//auto r = compTrans.GetRotation();
			//r = FRotator(0, -90, 0).Quaternion() * r * FRotator(0, 90, 0).Quaternion();
			//compTrans.SetRotation(r);
		}
		const auto boneTrans = compTrans * parentBoneInv;

		FQuat q = boneTrans.GetRotation();

		{
			//q = FRotator(0, -90, 0).Quaternion() * q * FRotator(0, 90, 0).Quaternion();
		}

		RawTrack.RotKeys.Add(q);

		//FVector s = srcTrans.GetScale3D();
		FVector s(1, 1, 1);
		RawTrack.ScaleKeys.Add(s);


		if (RawTrack.PosKeys.Num() == 0) {
			RawTrack.PosKeys.Add(FVector::ZeroVector);
		}
		if (RawTrack.RotKeys.Num() == 0) {
			RawTrack.RotKeys.Add(FQuat::Identity);
		}
		if (RawTrack.ScaleKeys.Num() == 0) {
			RawTrack.ScaleKeys.Add(FVector::OneVector);
		}

		int32 NewTrackIdx = ase->AddNewRawTrack(BoneName, &RawTrack);

		totalFrameNum = 1;
		totalTime = 1;

#if	UE_VERSION_OLDER_THAN(4,22,0)
		ase->NumFrames = totalFrameNum;
#else
		ase->SetRawNumberOfFrame(totalFrameNum);
#endif

		ase->SequenceLength = totalTime;
		ase->MarkRawDataAsModified();
	}

	const bool bSourceDataExists = ase->HasSourceRawData();
	if (bSourceDataExists)
	{
		ase->BakeTrackCurvesToRawAnimation();
	} else {
		ase->PostProcessSequence();
	}

	return true;
}