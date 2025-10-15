// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CreateSessionCallbackProxyAdvanced.h"

#ifdef ADVANCEDSESSIONS_CreateSessionCallbackProxyAdvanced_generated_h
#error "CreateSessionCallbackProxyAdvanced.generated.h already included, missing '#pragma once' in CreateSessionCallbackProxyAdvanced.h"
#endif
#define ADVANCEDSESSIONS_CreateSessionCallbackProxyAdvanced_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UCreateSessionCallbackProxyAdvanced;
class UObject;
struct FSessionPropertyKeyPair;

// ********** Begin Class UCreateSessionCallbackProxyAdvanced **************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_RPC_WRAPPERS \
	DECLARE_FUNCTION(execCreateAdvancedSession);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UCreateSessionCallbackProxyAdvanced_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_INCLASS \
private: \
	static void StaticRegisterNativesUCreateSessionCallbackProxyAdvanced(); \
	friend struct Z_Construct_UClass_UCreateSessionCallbackProxyAdvanced_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UCreateSessionCallbackProxyAdvanced_NoRegister(); \
public: \
	DECLARE_CLASS2(UCreateSessionCallbackProxyAdvanced, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UCreateSessionCallbackProxyAdvanced_NoRegister) \
	DECLARE_SERIALIZER(UCreateSessionCallbackProxyAdvanced)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UCreateSessionCallbackProxyAdvanced(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCreateSessionCallbackProxyAdvanced) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UCreateSessionCallbackProxyAdvanced); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCreateSessionCallbackProxyAdvanced); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UCreateSessionCallbackProxyAdvanced(UCreateSessionCallbackProxyAdvanced&&) = delete; \
	UCreateSessionCallbackProxyAdvanced(const UCreateSessionCallbackProxyAdvanced&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UCreateSessionCallbackProxyAdvanced();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_9_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UCreateSessionCallbackProxyAdvanced;

// ********** End Class UCreateSessionCallbackProxyAdvanced ****************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_CreateSessionCallbackProxyAdvanced_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
