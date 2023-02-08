// Fill out your copyright notice in the Description page of Project Settings.


#include "KinectActor.h"

// Sets default values
AKinectActor::AKinectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKinectActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKinectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updateBodyFrame();

}

bool AKinectActor::initKinect() {
	if (FAILED(GetDefaultKinectSensor(&sensor))) {
		return false;
	}
	if (sensor) {
		sensor->Open();
		sensor->get_CoordinateMapper(&mapper);
		return true;
	}
	return false;
}
void AKinectActor::initBodyFrame() {
	IBodyFrameSource* bodyFrameSource;
	HRESULT hResult = sensor->get_BodyFrameSource(&bodyFrameSource);
	if (SUCCEEDED(hResult)) {
		bodyFrameSource->OpenReader(&bodyFrameReader);
	}
	if (bodyFrameReader == nullptr) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Body Frame Reader is ready..."));
	bodyInit = true;
}

void AKinectActor::updateBodyFrame() {
	if (!bodyInit) {
		initBodyFrame();
	}
	IBody* bodies[BODY_COUNT] = { 0 };
	IBodyFrame* bodyFrame;
	HRESULT res = bodyFrameReader->AcquireLatestFrame(&bodyFrame);
	if (FAILED(res)) {
		return;
	}
	bodyFrame->GetAndRefreshBodyData(BODY_COUNT, bodies);
	BOOLEAN tracked = false;
	
	for (int i = 0; i < BODY_COUNT; i++) {
		bodies[i]->get_IsTracked(&tracked);
		if (tracked) {
			bodies[i]->get_HandLeftState(&leftHandState);
			bodies[i]->get_HandRightState(&rightHandState);
			bodies[i]->GetJoints(JointType_Count, joints);
			bodies[i]->get_TrackingId(&trackingID);
			JointOrientation jointOrientations[JointType_Count];
			res = bodies[i]->GetJointOrientations(JointType_Count, jointOrientations);
			if (SUCCEEDED(res)) {
				for (int j = 0; j < JointType_Count; j++) {
					orientations[j] = OrientToFRotator(jointOrientations[j], j);
				}
			}
		}
	}
	if (bodyFrame) {
		bodyFrame->Release();
	}
}