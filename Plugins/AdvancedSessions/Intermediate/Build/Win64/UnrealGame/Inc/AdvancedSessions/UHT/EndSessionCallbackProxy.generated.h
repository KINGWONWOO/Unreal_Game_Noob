// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "EndSessionCallbackProxy.h"

#ifdef ADVANCEDSESSIONS_EndSessionCallbackProxy_generated_h
#error "EndSessionCallbackProxy.generated.h already included, missing '#pragma once' in EndSessionCallbackProxy.h"
#endif
#define ADVANCEDSESSIONS_EndSessionCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UEndSessionCallbackProxy;
class UObject;

// ********** Begin Class UEndSessionCallbackProxy *************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_RPC_WRAPPERS \
	DECLARE_FUNCTION(execEndSession);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UEndSessionCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_INCLASS \
private: \
	static void StaticRegisterNativesUEndSessionCallbackProxy(); \
	friend struct Z_Construct_UClass_UEndSessionCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UEndSessionCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(UEndSessionCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UEndSessionCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(UEndSessionCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UEndSessionCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UEndSessionCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UEndSessionCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UEndSessionCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UEndSessionCallbackProxy(UEndSessionCallbackProxy&&) = delete; \
	UEndSessionCallbackProxy(const UEndSessionCallbackProxy&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UEndSessionCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_9_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UEndSessionCallbackProxy;

// ********** End Class UEndSessionCallbackProxy ***************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_EndSessionCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
