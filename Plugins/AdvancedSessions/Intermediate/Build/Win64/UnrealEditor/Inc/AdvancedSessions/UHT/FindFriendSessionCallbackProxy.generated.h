// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "FindFriendSessionCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_FindFriendSessionCallbackProxy_generated_h
#error "FindFriendSessionCallbackProxy.generated.h already included, missing '#pragma once' in FindFriendSessionCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_FindFriendSessionCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UFindFriendSessionCallbackProxy;
class UObject;
struct FBlueprintSessionResult;
struct FBPUniqueNetId;

// ********** Begin Delegate FBlueprintFindFriendSessionDelegate ***********************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_11_DELEGATE \
ADVANCEDSESSIONS_API void FBlueprintFindFriendSessionDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintFindFriendSessionDelegate, TArray<FBlueprintSessionResult> const& SessionInfo);


// ********** End Delegate FBlueprintFindFriendSessionDelegate *************************************

// ********** Begin Class UFindFriendSessionCallbackProxy ******************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_RPC_WRAPPERS \
	DECLARE_FUNCTION(execFindFriendSession);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UFindFriendSessionCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_INCLASS \
private: \
	static void StaticRegisterNativesUFindFriendSessionCallbackProxy(); \
	friend struct Z_Construct_UClass_UFindFriendSessionCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UFindFriendSessionCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UFindFriendSessionCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UFindFriendSessionCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UFindFriendSessionCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UFindFriendSessionCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFindFriendSessionCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UFindFriendSessionCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFindFriendSessionCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UFindFriendSessionCallbackProxy(UFindFriendSessionCallbackProxy&&) = delete; \
	UFindFriendSessionCallbackProxy(const UFindFriendSessionCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UFindFriendSessionCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_13_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UFindFriendSessionCallbackProxy;

// ********** End Class UFindFriendSessionCallbackProxy ********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_FindFriendSessionCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
