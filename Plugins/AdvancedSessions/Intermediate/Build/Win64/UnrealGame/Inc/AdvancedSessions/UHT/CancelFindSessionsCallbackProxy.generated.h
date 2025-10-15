// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CancelFindSessionsCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_CancelFindSessionsCallbackProxy_generated_h
#error "CancelFindSessionsCallbackProxy.generated.h already included, missing '#pragma once' in CancelFindSessionsCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_CancelFindSessionsCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UCancelFindSessionsCallbackProxy;
class UObject;

// ********** Begin Class UCancelFindSessionsCallbackProxy *****************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_RPC_WRAPPERS \
	DECLARE_FUNCTION(execCancelFindSessions);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UCancelFindSessionsCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_INCLASS \
private: \
	static void StaticRegisterNativesUCancelFindSessionsCallbackProxy(); \
	friend struct Z_Construct_UClass_UCancelFindSessionsCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UCancelFindSessionsCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UCancelFindSessionsCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UCancelFindSessionsCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UCancelFindSessionsCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UCancelFindSessionsCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCancelFindSessionsCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UCancelFindSessionsCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCancelFindSessionsCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UCancelFindSessionsCallbackProxy(UCancelFindSessionsCallbackProxy&&) = delete; \
	UCancelFindSessionsCallbackProxy(const UCancelFindSessionsCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UCancelFindSessionsCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_9_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UCancelFindSessionsCallbackProxy;

// ********** End Class UCancelFindSessionsCallbackProxy *******************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CancelFindSessionsCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
