// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AdvancedExternalUILibrary.h"

#ifdef ADVANCEDSESSIONS_AdvancedExternalUILibrary_generated_h
#error "AdvancedExternalUILibrary.generated.h already included, missing '#pragma once' in AdvancedExternalUILibrary.h"
#endif
#define ADVANCEDSESSIONS_AdvancedExternalUILibrary_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UObject;
enum class EBlueprintResultSwitch : uint8;
struct FBPUniqueNetId;

// ********** Begin Class UAdvancedExternalUILibrary ***********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execShowAccountUpgradeUI); \
	DECLARE_FUNCTION(execShowProfileUI); \
	DECLARE_FUNCTION(execCloseWebURLUI); \
	DECLARE_FUNCTION(execShowWebURLUI); \
	DECLARE_FUNCTION(execShowLeaderBoardUI); \
	DECLARE_FUNCTION(execShowInviteUI); \
	DECLARE_FUNCTION(execShowFriendsUI);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UAdvancedExternalUILibrary_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUAdvancedExternalUILibrary(); \
	friend struct Z_Construct_UClass_UAdvancedExternalUILibrary_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UAdvancedExternalUILibrary_NoRegister(); \
public: \
	DECLARE_CLASS2(UAdvancedExternalUILibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UAdvancedExternalUILibrary_NoRegister) \
	DECLARE_SERIALIZER(UAdvancedExternalUILibrary)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAdvancedExternalUILibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UAdvancedExternalUILibrary(UAdvancedExternalUILibrary&&) = delete; \
	UAdvancedExternalUILibrary(const UAdvancedExternalUILibrary&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAdvancedExternalUILibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAdvancedExternalUILibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAdvancedExternalUILibrary) \
	NO_API virtual ~UAdvancedExternalUILibrary();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_26_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_INCLASS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h_29_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UAdvancedExternalUILibrary;

// ********** End Class UAdvancedExternalUILibrary *************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedExternalUILibrary_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
