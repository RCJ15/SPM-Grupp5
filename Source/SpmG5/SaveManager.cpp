// Marcus hopefully approves of this.


#include "SaveManager.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonTypes.h"
#include "Hal/FileManager.h"
#include "JsonObjectConverter.h"



void USaveManager::SaveGame(const FGameData& GameData)
{
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(GameData);
	FString JsonString;
	
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		FString FilePath = FPaths::ProjectSavedDir() / TEXT("SaveData.json");
	
		if (FFileHelper::SaveStringToFile(JsonString, *FilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("JSON FILE SAVED TO: %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("JSON FILE FAILED: %s"), *FilePath);
		}
	}
	
}

void USaveManager::LoadGame(const FGameData& GameData)
{
}

void USaveManager::ApplySettings()
{
}

bool USaveManager::DoesSaveExist()
{
	return true;
}
