// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "LogoutUserCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_LogoutUserCallbackProxy_generated_h
#error "LogoutUserCallbackProxy.generated.h already included, missing '#pragma once' in LogoutUserCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_LogoutUserCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class ULogoutUserCallbackProxy;
class UObject;

// ********** Begin Class ULogoutUserCallbackProxy *************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_RPC_WRAPPERS \
	DECLARE_FUNCTION(execLogoutUser);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_ULogoutUserCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_INCLASS \
private: \
	static void StaticRegisterNativesULogoutUserCallbackProxy(); \
	friend struct Z_Construct_UClass_ULogoutUserCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_ULogoutUserCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(ULogoutUserCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_ULogoutUserCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(ULogoutUserCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API ULogoutUserCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ULogoutUserCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, ULogoutUserCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ULogoutUserCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	ULogoutUserCallbackProxy(ULogoutUserCallbackProxy&&) = delete; \
	ULogoutUserCallbackProxy(const ULogoutUserCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~ULogoutUserCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_10_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class ULogoutUserCallbackProxy;

// ********** End Class ULogoutUserCallbackProxy ***************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_LogoutUserCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
