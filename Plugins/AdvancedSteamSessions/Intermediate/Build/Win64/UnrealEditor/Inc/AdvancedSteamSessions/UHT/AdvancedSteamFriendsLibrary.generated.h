// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AdvancedSteamFriendsLibrary.h"

#ifdef ADVANCEDSTEAMSESSIONS_AdvancedSteamFriendsLibrary_generated_h
#error "AdvancedSteamFriendsLibrary.generated.h already included, missing '#pragma once' in AdvancedSteamFriendsLibrary.h"
#endif
#define ADVANCEDSTEAMSESSIONS_AdvancedSteamFriendsLibrary_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UTexture2D;
enum class EBlueprintAsyncResultSwitch : uint8;
enum class EBlueprintResultSwitch : uint8;
enum class EBPTextFilteringContext : uint8;
enum class ESteamUserOverlayType : uint8;
enum class SteamAvatarSize : uint8;
struct FBPSteamGroupInfo;
struct FBPUniqueNetId;

// ********** Begin ScriptStruct FBPSteamGroupInfo *************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_291_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPSteamGroupInfo_Statics; \
	ADVANCEDSTEAMSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPSteamGroupInfo;
// ********** End ScriptStruct FBPSteamGroupInfo ***************************************************

// ********** Begin Class UAdvancedSteamFriendsLibrary *********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execIsSteamInBigPictureMode); \
	DECLARE_FUNCTION(execFilterText); \
	DECLARE_FUNCTION(execInitTextFiltering); \
	DECLARE_FUNCTION(execGetSteamGroups); \
	DECLARE_FUNCTION(execGetSteamFriendGamePlayed); \
	DECLARE_FUNCTION(execGetLocalSteamIDFromSteam); \
	DECLARE_FUNCTION(execCreateSteamIDFromString); \
	DECLARE_FUNCTION(execGetSteamPersonaName); \
	DECLARE_FUNCTION(execGetFriendSteamLevel); \
	DECLARE_FUNCTION(execIsOverlayEnabled); \
	DECLARE_FUNCTION(execOpenSteamUserOverlay); \
	DECLARE_FUNCTION(execRequestSteamFriendInfo); \
	DECLARE_FUNCTION(execGetSteamFriendAvatar);


ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_UAdvancedSteamFriendsLibrary_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUAdvancedSteamFriendsLibrary(); \
	friend struct Z_Construct_UClass_UAdvancedSteamFriendsLibrary_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_UAdvancedSteamFriendsLibrary_NoRegister(); \
public: \
	DECLARE_CLASS2(UAdvancedSteamFriendsLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSteamSessions"), Z_Construct_UClass_UAdvancedSteamFriendsLibrary_NoRegister) \
	DECLARE_SERIALIZER(UAdvancedSteamFriendsLibrary)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAdvancedSteamFriendsLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UAdvancedSteamFriendsLibrary(UAdvancedSteamFriendsLibrary&&) = delete; \
	UAdvancedSteamFriendsLibrary(const UAdvancedSteamFriendsLibrary&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAdvancedSteamFriendsLibrary); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAdvancedSteamFriendsLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAdvancedSteamFriendsLibrary) \
	NO_API virtual ~UAdvancedSteamFriendsLibrary();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_323_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_INCLASS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h_326_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UAdvancedSteamFriendsLibrary;

// ********** End Class UAdvancedSteamFriendsLibrary ***********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_AdvancedSteamFriendsLibrary_h

// ********** Begin Enum SteamAvatarSize ***********************************************************
#define FOREACH_ENUM_STEAMAVATARSIZE(op) \
	op(SteamAvatarSize::SteamAvatar_INVALID) \
	op(SteamAvatarSize::SteamAvatar_Small) \
	op(SteamAvatarSize::SteamAvatar_Medium) \
	op(SteamAvatarSize::SteamAvatar_Large) 

enum class SteamAvatarSize : uint8;
template<> struct TIsUEnumClass<SteamAvatarSize> { enum { Value = true }; };
template<> ADVANCEDSTEAMSESSIONS_API UEnum* StaticEnum<SteamAvatarSize>();
// ********** End Enum SteamAvatarSize *************************************************************

// ********** Begin Enum ESteamUserOverlayType *****************************************************
#define FOREACH_ENUM_ESTEAMUSEROVERLAYTYPE(op) \
	op(ESteamUserOverlayType::steamid) \
	op(ESteamUserOverlayType::chat) \
	op(ESteamUserOverlayType::jointrade) \
	op(ESteamUserOverlayType::stats) \
	op(ESteamUserOverlayType::achievements) \
	op(ESteamUserOverlayType::friendadd) \
	op(ESteamUserOverlayType::friendremove) \
	op(ESteamUserOverlayType::friendrequestaccept) \
	op(ESteamUserOverlayType::friendrequestignore) \
	op(ESteamUserOverlayType::invitetolobby) 

enum class ESteamUserOverlayType : uint8;
template<> struct TIsUEnumClass<ESteamUserOverlayType> { enum { Value = true }; };
template<> ADVANCEDSTEAMSESSIONS_API UEnum* StaticEnum<ESteamUserOverlayType>();
// ********** End Enum ESteamUserOverlayType *******************************************************

// ********** Begin Enum EBPTextFilteringContext ***************************************************
#define FOREACH_ENUM_EBPTEXTFILTERINGCONTEXT(op) \
	op(EBPTextFilteringContext::FContext_Unknown) \
	op(EBPTextFilteringContext::FContext_GameContent) \
	op(EBPTextFilteringContext::FContext_Chat) \
	op(EBPTextFilteringContext::FContext_Name) 

enum class EBPTextFilteringContext : uint8;
template<> struct TIsUEnumClass<EBPTextFilteringContext> { enum { Value = true }; };
template<> ADVANCEDSTEAMSESSIONS_API UEnum* StaticEnum<EBPTextFilteringContext>();
// ********** End Enum EBPTextFilteringContext *****************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
