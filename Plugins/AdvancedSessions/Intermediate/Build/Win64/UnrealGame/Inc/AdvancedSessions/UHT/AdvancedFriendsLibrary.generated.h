// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AdvancedFriendsLibrary.h"

#ifdef ADVANCEDSESSIONS_AdvancedFriendsLibrary_generated_h
#error "AdvancedFriendsLibrary.generated.h already included, missing '#pragma once' in AdvancedFriendsLibrary.h"
#endif
#define ADVANCEDSESSIONS_AdvancedFriendsLibrary_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
enum class EBlueprintResultSwitch : uint8;
struct FBPFriendInfo;
struct FBPOnlineRecentPlayer;
struct FBPUniqueNetId;

// ********** Begin Class UAdvancedFriendsLibrary **************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execIsAFriend); \
	DECLARE_FUNCTION(execGetStoredRecentPlayersList); \
	DECLARE_FUNCTION(execGetStoredFriendsList); \
	DECLARE_FUNCTION(execGetFriend); \
	DECLARE_FUNCTION(execSendSessionInviteToFriend); \
	DECLARE_FUNCTION(execSendSessionInviteToFriends);


ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UAdvancedFriendsLibrary_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUAdvancedFriendsLibrary(); \
	friend struct Z_Construct_UClass_UAdvancedFriendsLibrary_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UAdvancedFriendsLibrary_NoRegister(); \
public: \
	DECLARE_CLASS2(UAdvancedFriendsLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_UAdvancedFriendsLibrary_NoRegister) \
	DECLARE_SERIALIZER(UAdvancedFriendsLibrary)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAdvancedFriendsLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UAdvancedFriendsLibrary(UAdvancedFriendsLibrary&&) = delete; \
	UAdvancedFriendsLibrary(const UAdvancedFriendsLibrary&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAdvancedFriendsLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAdvancedFriendsLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAdvancedFriendsLibrary) \
	NO_API virtual ~UAdvancedFriendsLibrary();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_25_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_INCLASS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h_28_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UAdvancedFriendsLibrary;

// ********** End Class UAdvancedFriendsLibrary ****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedFriendsLibrary_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
