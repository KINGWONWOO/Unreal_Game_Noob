// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "GetUserPrivilegeCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_GetUserPrivilegeCallbackProxy_generated_h
#error "GetUserPrivilegeCallbackProxy.generated.h already included, missing '#pragma once' in GetUserPrivilegeCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_GetUserPrivilegeCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UGetUserPrivilegeCallbackProxy;
class UObject;
enum class EBPUserPrivileges : uint8;
struct FBPUniqueNetId;

// ********** Begin Delegate FBlueprintGetUserPrivilegeDelegate ************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_9_DELEGATE \
ADVANCEDSESSIONS_API void FBlueprintGetUserPrivilegeDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintGetUserPrivilegeDelegate, EBPUserPrivileges QueriedPrivilege, bool HadPrivilege);


// ********** End Delegate FBlueprintGetUserPrivilegeDelegate **************************************

// ********** Begin Class UGetUserPrivilegeCallbackProxy *******************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_RPC_WRAPPERS \
	DECLARE_FUNCTION(execGetUserPrivilege);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetUserPrivilegeCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_INCLASS \
private: \
	static void StaticRegisterNativesUGetUserPrivilegeCallbackProxy(); \
	friend struct Z_Construct_UClass_UGetUserPrivilegeCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UGetUserPrivilegeCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UGetUserPrivilegeCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UGetUserPrivilegeCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UGetUserPrivilegeCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UGetUserPrivilegeCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGetUserPrivilegeCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UGetUserPrivilegeCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGetUserPrivilegeCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UGetUserPrivilegeCallbackProxy(UGetUserPrivilegeCallbackProxy&&) = delete; \
	UGetUserPrivilegeCallbackProxy(const UGetUserPrivilegeCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UGetUserPrivilegeCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_11_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h_14_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UGetUserPrivilegeCallbackProxy;

// ********** End Class UGetUserPrivilegeCallbackProxy *********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_GetUserPrivilegeCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
