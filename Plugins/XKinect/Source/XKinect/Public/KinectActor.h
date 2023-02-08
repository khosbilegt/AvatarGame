// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#define WIN32_LEAN_AND_MEAN
#include "Kinect.h"
#include <math.h>

#include "KinectActor.generated.h"

UCLASS()
class XKINECT_API AKinectActor : public AActor
{
	GENERATED_BODY()
	
public:
	AKinectActor();

	IKinectSensor* sensor;
	ICoordinateMapper* mapper;
	IBodyFrameReader* bodyFrameReader;
	Joint joints[JointType_Count];
	uint64_t trackingID;
	HandState leftHandState, rightHandState;
	FRotator orientations[JointType_Count];
	BOOLEAN bodyInit = false;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* root;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool initKinect();
	void initBodyFrame();
	void updateBodyFrame();
	FRotator OrientToFRotator(JointOrientation orientation, int i);
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		FVector getJointPosition(int ind);
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		FVector getJointRotation(int ind);
};
