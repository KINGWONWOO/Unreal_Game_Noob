// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UpdateSessionCallbackProxyAdvanced.h"

#ifdef ADVANCEDSESSIONS_UpdateSessionCallbackProxyAdvanced_generated_h
#error "UpdateSessionCallbackProxyAdvanced.generated.h already included, missing '#pragma once' in UpdateSessionCallbackProxyAdvanced.h"
#endif
#define ADVANCEDSESSIONS_UpdateSessionCallbackProxyAdvanced_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UObject;
class UUpdateSessionCallbackProxyAdvanced;
struct FSessionPropertyKeyPair;

// ********** Begin Class UUpdateSessionCallbackProxyAdvanced **************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_RPC_WRAPPERS \
	DECLARE_FUNCTION(execUpdateSession);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UUpdateSessionCallbackProxyAdvanced_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_INCLASS \
private: \
	static void StaticRegisterNativesUUpdateSessionCallbackProxyAdvanced(); \
	friend struct Z_Construct_UClass_UUpdateSessionCallbackProxyAdvanced_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UUpdateSessionCallbackProxyAdvanced_NoRegister(); \
public: \
	DECLARE_CLASS2(UUpdateSessionCallbackProxyAdvanced, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UUpdateSessionCallbackProxyAdvanced_NoRegister) \
	DECLARE_SERIALIZER(UUpdateSessionCallbackProxyAdvanced)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UUpdateSessionCallbackProxyAdvanced(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUpdateSessionCallbackProxyAdvanced) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UUpdateSessionCallbackProxyAdvanced); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUpdateSessionCallbackProxyAdvanced); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UUpdateSessionCallbackProxyAdvanced(UUpdateSessionCallbackProxyAdvanced&&) = delete; \
	UUpdateSessionCallbackProxyAdvanced(const UUpdateSessionCallbackProxyAdvanced&) = delete; \
	ADVANCEDSESSIONS_API virtual ~UUpdateSessionCallbackProxyAdvanced();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_9_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UUpdateSessionCallbackProxyAdvanced;

// ********** End Class UUpdateSessionCallbackProxyAdvanced ****************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_UpdateSessionCallbackProxyAdvanced_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
