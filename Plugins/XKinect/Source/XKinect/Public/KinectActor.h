// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#define WIN32_LEAN_AND_MEAN
THIRD_PARTY_INCLUDES_START
#include <Kinect.h>
#include "Kinect.h"
THIRD_PARTY_INCLUDES_END
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
	FVector jointLocations[JointType_Count];
	BOOLEAN bodyInit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Kinect")
	bool isKinectAvailable = false;

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

private:
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		FVector getJointPosition(int32 ind);
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		FRotator getJointRotation(int32 ind);
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		bool getLeftHandState();
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		bool getRightHandState();
	UFUNCTION(BlueprintCallable, Category = "Kinect")
		bool getKinectConnected();
	int getCenteredBody(IBody* bodies[], int count);
};
