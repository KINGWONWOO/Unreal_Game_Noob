// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "GetRecentPlayersCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_GetRecentPlayersCallbackProxy_generated_h
#error "GetRecentPlayersCallbackProxy.generated.h already included, missing '#pragma once' in GetRecentPlayersCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_GetRecentPlayersCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UGetRecentPlayersCallbackProxy;
class UObject;
struct FBPOnlineRecentPlayer;
struct FBPUniqueNetId;

// ********** Begin Delegate FBlueprintGetRecentPlayersDelegate ************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_10_DELEGATE \
ADVANCEDSESSIONS_API void FBlueprintGetRecentPlayersDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintGetRecentPlayersDelegate, TArray<FBPOnlineRecentPlayer> const& Results);


// ********** End Delegate FBlueprintGetRecentPlayersDelegate **************************************

// ********** Begin Class UGetRecentPlayersCallbackProxy *******************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_RPC_WRAPPERS \
	DECLARE_FUNCTION(execGetAndStoreRecentPlayersList);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetRecentPlayersCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_INCLASS \
private: \
	static void StaticRegisterNativesUGetRecentPlayersCallbackProxy(); \
	friend struct Z_Construct_UClass_UGetRecentPlayersCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetRecentPlayersCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UGetRecentPlayersCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UGetRecentPlayersCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UGetRecentPlayersCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UGetRecentPlayersCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGetRecentPlayersCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UGetRecentPlayersCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGetRecentPlayersCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UGetRecentPlayersCallbackProxy(UGetRecentPlayersCallbackProxy&&) = delete; \
	UGetRecentPlayersCallbackProxy(const UGetRecentPlayersCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UGetRecentPlayersCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_12_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UGetRecentPlayersCallbackProxy;

// ********** End Class UGetRecentPlayersCallbackProxy *********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetRecentPlayersCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
