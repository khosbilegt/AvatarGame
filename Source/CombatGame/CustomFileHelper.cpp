#include "CustomFileHelper.h"

// Sets default values
ACustomFileHelper::ACustomFileHelper()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACustomFileHelper::BeginPlay()
{
	Super::BeginPlay();
	
}


FString ACustomFileHelper::loadFile(FString fileName) {
    FString file = FPaths::ProjectSavedDir();
    file.Append(TEXT("Colors.json"));
    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
    FString FileContent;
    if (FileManager.FileExists(*file))
    {
        if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None))
        {
            UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Text From File: %s"), *FileContent);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
    }
    return FileContent;
}

void ACustomFileHelper::saveFile(FString fileName, FString content) {
    FString file = FPaths::ProjectSavedDir();
    file.Append(content);

    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

    if (FileManager.FileExists(*file))
    {
        if (FFileHelper::SaveStringToFile(content, *file))
        {
            UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Sucsesfuly Written: \"%s\" to the text file"), *content);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
    }
}