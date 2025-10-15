// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SteamRequestGroupOfficersCallbackProxy.h"

#ifdef ADVANCEDSTEAMSESSIONS_SteamRequestGroupOfficersCallbackProxy_generated_h
#error "SteamRequestGroupOfficersCallbackProxy.generated.h already included, missing '#pragma once' in SteamRequestGroupOfficersCallbackProxy.h"
#endif
#define ADVANCEDSTEAMSESSIONS_SteamRequestGroupOfficersCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UObject;
class USteamRequestGroupOfficersCallbackProxy;
struct FBPSteamGroupOfficer;
struct FBPUniqueNetId;

// ********** Begin ScriptStruct FBPSteamGroupOfficer **********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_51_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPSteamGroupOfficer_Statics; \
	ADVANCEDSTEAMSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPSteamGroupOfficer;
// ********** End ScriptStruct FBPSteamGroupOfficer ************************************************

// ********** Begin Delegate FBlueprintGroupOfficerDetailsDelegate *********************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_63_DELEGATE \
ADVANCEDSTEAMSESSIONS_API void FBlueprintGroupOfficerDetailsDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintGroupOfficerDetailsDelegate, TArray<FBPSteamGroupOfficer> const& OfficerList);


// ********** End Delegate FBlueprintGroupOfficerDetailsDelegate ***********************************

// ********** Begin Class USteamRequestGroupOfficersCallbackProxy **********************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_RPC_WRAPPERS \
	DECLARE_FUNCTION(execGetSteamGroupOfficerList);


ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamRequestGroupOfficersCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_INCLASS \
private: \
	static void StaticRegisterNativesUSteamRequestGroupOfficersCallbackProxy(); \
	friend struct Z_Construct_UClass_USteamRequestGroupOfficersCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamRequestGroupOfficersCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(USteamRequestGroupOfficersCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSteamSessions"), Z_Construct_UClass_USteamRequestGroupOfficersCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(USteamRequestGroupOfficersCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSTEAMSESSIONS_API USteamRequestGroupOfficersCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USteamRequestGroupOfficersCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSTEAMSESSIONS_API, USteamRequestGroupOfficersCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamRequestGroupOfficersCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	USteamRequestGroupOfficersCallbackProxy(USteamRequestGroupOfficersCallbackProxy&&) = delete; \
	USteamRequestGroupOfficersCallbackProxy(const USteamRequestGroupOfficersCallbackProxy&) = delete;


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_65_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h_68_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class USteamRequestGroupOfficersCallbackProxy;

// ********** End Class USteamRequestGroupOfficersCallbackProxy ************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamRequestGroupOfficersCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
