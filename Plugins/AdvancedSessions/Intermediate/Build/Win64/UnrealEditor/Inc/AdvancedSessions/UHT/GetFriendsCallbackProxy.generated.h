// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "GetFriendsCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_GetFriendsCallbackProxy_generated_h
#error "GetFriendsCallbackProxy.generated.h already included, missing '#pragma once' in GetFriendsCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_GetFriendsCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UGetFriendsCallbackProxy;
class UObject;
struct FBPFriendInfo;

// ********** Begin Delegate FBlueprintGetFriendsListDelegate **************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_11_DELEGATE \
ADVANCEDSESSIONS_API void FBlueprintGetFriendsListDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintGetFriendsListDelegate, TArray<FBPFriendInfo> const& Results);


// ********** End Delegate FBlueprintGetFriendsListDelegate ****************************************

// ********** Begin Class UGetFriendsCallbackProxy *************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_RPC_WRAPPERS \
	DECLARE_FUNCTION(execGetAndStoreFriendsList);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetFriendsCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_INCLASS \
private: \
	static void StaticRegisterNativesUGetFriendsCallbackProxy(); \
	friend struct Z_Construct_UClass_UGetFriendsCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetFriendsCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UGetFriendsCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UGetFriendsCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UGetFriendsCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UGetFriendsCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGetFriendsCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UGetFriendsCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGetFriendsCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UGetFriendsCallbackProxy(UGetFriendsCallbackProxy&&) = delete; \
	UGetFriendsCallbackProxy(const UGetFriendsCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UGetFriendsCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_13_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UGetFriendsCallbackProxy;

// ********** End Class UGetFriendsCallbackProxy ***************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetFriendsCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
