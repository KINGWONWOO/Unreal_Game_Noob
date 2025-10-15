// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AutoLoginUserCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_AutoLoginUserCallbackProxy_generated_h
#error "AutoLoginUserCallbackProxy.generated.h already included, missing '#pragma once' in AutoLoginUserCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_AutoLoginUserCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UAutoLoginUserCallbackProxy;
class UObject;

// ********** Begin Class UAutoLoginUserCallbackProxy **********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_RPC_WRAPPERS \
	DECLARE_FUNCTION(execAutoLoginUser);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UAutoLoginUserCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_INCLASS \
private: \
	static void StaticRegisterNativesUAutoLoginUserCallbackProxy(); \
	friend struct Z_Construct_UClass_UAutoLoginUserCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UAutoLoginUserCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UAutoLoginUserCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UAutoLoginUserCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UAutoLoginUserCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UAutoLoginUserCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAutoLoginUserCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UAutoLoginUserCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAutoLoginUserCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UAutoLoginUserCallbackProxy(UAutoLoginUserCallbackProxy&&) = delete; \
	UAutoLoginUserCallbackProxy(const UAutoLoginUserCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UAutoLoginUserCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_10_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UAutoLoginUserCallbackProxy;

// ********** End Class UAutoLoginUserCallbackProxy ************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AutoLoginUserCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
