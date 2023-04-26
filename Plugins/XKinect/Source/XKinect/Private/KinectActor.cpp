// Fill out your copyright notice in the Description page of Project Settings.


#include "KinectActor.h"

// Sets default values
AKinectActor::AKinectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	initKinect();
	root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("root"));
	bodyFrameReader = nullptr;
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
	HRESULT result = sensor->get_BodyFrameSource(&bodyFrameSource);
	if (SUCCEEDED(result)) {
		bodyFrameSource->OpenReader(&bodyFrameReader);
		isKinectAvailable = true;
	}
	if (bodyFrameReader == nullptr) {
		isKinectAvailable = false;
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

	isKinectAvailable = true;
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
					jointLocations[j] = FVector(joints[j].Position.X, joints[j].Position.Y, joints[j].Position.Z);
				}
			}
		}
	}
	if (bodyFrame) {
		bodyFrame->Release();
	}
}

FRotator AKinectActor::OrientToFRotator(JointOrientation orientation, int i) {
	float w1 = -orientation.Orientation.w;
	float x1 = -orientation.Orientation.y;
	float y1 = -orientation.Orientation.x;
	float z1 = -orientation.Orientation.z;

	float w2 = 0;
	float x2 = 0;
	float y2 = 1;
	float z2 = 0;

	if (i == 1 || i == 2 || i == 20) {
		return FRotator(FQuat(
			-w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2,
			w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
			w1 * y2 + y1 * w2 + z1 * x2 - x1 * z2,
			w1 * z2 + z1 * w2 + x1 * y2 - y1 * x2
		));
	}
	return FRotator(FQuat(w1, x1, y1, z1));
}

FVector AKinectActor::getJointPosition(int32 ind) {
	return jointLocations[ind];
}

FRotator AKinectActor::getJointRotation(int32 ind) {
	//return FRotator(orientations[ind].Pitch, orientations[ind].Yaw, orientations[ind].Roll);
	return orientations[ind];
}

bool AKinectActor::getRightHandState() {
	return rightHandState == HandState_Closed;
}

bool AKinectActor::getLeftHandState() {
	return leftHandState == HandState_Closed;
}

bool AKinectActor::getKinectConnected() {
	BOOLEAN result;
	sensor->get_IsAvailable(&result);
	if (result) {
		return true;
	}
	return false;
}

int AKinectActor::getCenteredBody(IBody* bodies[], int count) {
	int centeredBody = 0;
	bodies[0]->GetJoints(JointType_Count, joints);
	FVector centeredLocation = FVector(joints[0].Position.X, joints[0].Position.Y, joints[0].Position.Z);
	float centeredDistance = FVector::Dist(centeredLocation, FVector());
	for (int i = 0; i < count; i++) {
		bodies[i]->GetJoints(JointType_Count, joints);
		FVector location = FVector(joints[0].Position.X, joints[0].Position.Y, joints[0].Position.Z);
		if (FVector::Dist(location, FVector()) < centeredDistance) {
			centeredBody = i;
			centeredLocation = location;
			centeredDistance = FVector::Dist(location, FVector());
		}
	}
	return centeredBody;
}

