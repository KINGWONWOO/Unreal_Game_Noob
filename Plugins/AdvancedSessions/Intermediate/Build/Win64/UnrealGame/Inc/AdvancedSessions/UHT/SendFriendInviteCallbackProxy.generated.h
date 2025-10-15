// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SendFriendInviteCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_SendFriendInviteCallbackProxy_generated_h
#error "SendFriendInviteCallbackProxy.generated.h already included, missing '#pragma once' in SendFriendInviteCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_SendFriendInviteCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UObject;
class USendFriendInviteCallbackProxy;
struct FBPUniqueNetId;

// ********** Begin Delegate FBlueprintSendFriendInviteDelegate ************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_11_DELEGATE \
ADVANCEDSESSIONS_API void FBlueprintSendFriendInviteDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintSendFriendInviteDelegate);


// ********** End Delegate FBlueprintSendFriendInviteDelegate **************************************

// ********** Begin Class USendFriendInviteCallbackProxy *******************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_RPC_WRAPPERS \
	DECLARE_FUNCTION(execSendFriendInvite);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_USendFriendInviteCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_INCLASS \
private: \
	static void StaticRegisterNativesUSendFriendInviteCallbackProxy(); \
	friend struct Z_Construct_UClass_USendFriendInviteCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_USendFriendInviteCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(USendFriendInviteCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_USendFriendInviteCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(USendFriendInviteCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API USendFriendInviteCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USendFriendInviteCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, USendFriendInviteCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USendFriendInviteCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	USendFriendInviteCallbackProxy(USendFriendInviteCallbackProxy&&) = delete; \
	USendFriendInviteCallbackProxy(const USendFriendInviteCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~USendFriendInviteCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_13_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class USendFriendInviteCallbackProxy;

// ********** End Class USendFriendInviteCallbackProxy *********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_SendFriendInviteCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
