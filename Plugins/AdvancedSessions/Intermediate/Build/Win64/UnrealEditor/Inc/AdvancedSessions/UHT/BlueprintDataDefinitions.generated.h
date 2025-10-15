// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BlueprintDataDefinitions.h"

#ifdef ADVANCEDSESSIONS_BlueprintDataDefinitions_generated_h
#error "BlueprintDataDefinitions.generated.h already included, missing '#pragma once' in BlueprintDataDefinitions.h"
#endif
#define ADVANCEDSESSIONS_BlueprintDataDefinitions_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin ScriptStruct FBPUserOnlineAccount **********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_48_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPUserOnlineAccount_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPUserOnlineAccount;
// ********** End ScriptStruct FBPUserOnlineAccount ************************************************

// ********** Begin ScriptStruct FBPUniqueNetId ****************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_150_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPUniqueNetId_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPUniqueNetId;
// ********** End ScriptStruct FBPUniqueNetId ******************************************************

// ********** Begin ScriptStruct FBPOnlineUser *****************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_224_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPOnlineUser_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPOnlineUser;
// ********** End ScriptStruct FBPOnlineUser *******************************************************

// ********** Begin ScriptStruct FBPOnlineRecentPlayer *********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_238_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPOnlineRecentPlayer_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct(); \
	typedef FBPOnlineUser Super;


struct FBPOnlineRecentPlayer;
// ********** End ScriptStruct FBPOnlineRecentPlayer ***********************************************

// ********** Begin ScriptStruct FBPFriendPresenceInfo *********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_249_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPFriendPresenceInfo_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPFriendPresenceInfo;
// ********** End ScriptStruct FBPFriendPresenceInfo ***********************************************

// ********** Begin ScriptStruct FBPFriendInfo *****************************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_282_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FBPFriendInfo_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FBPFriendInfo;
// ********** End ScriptStruct FBPFriendInfo *******************************************************

// ********** Begin ScriptStruct FSessionPropertyKeyPair *******************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_325_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionPropertyKeyPair_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FSessionPropertyKeyPair;
// ********** End ScriptStruct FSessionPropertyKeyPair *********************************************

// ********** Begin ScriptStruct FSessionsSearchSetting ********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h_336_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSessionsSearchSetting_Statics; \
	ADVANCEDSESSIONS_API static class UScriptStruct* StaticStruct();


struct FSessionsSearchSetting;
// ********** End ScriptStruct FSessionsSearchSetting **********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_BlueprintDataDefinitions_h

// ********** Begin Enum EBPUserPrivileges *********************************************************
#define FOREACH_ENUM_EBPUSERPRIVILEGES(op) \
	op(EBPUserPrivileges::CanPlay) \
	op(EBPUserPrivileges::CanPlayOnline) \
	op(EBPUserPrivileges::CanCommunicateOnline) \
	op(EBPUserPrivileges::CanUseUserGeneratedContent) 

enum class EBPUserPrivileges : uint8;
template<> struct TIsUEnumClass<EBPUserPrivileges> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBPUserPrivileges>();
// ********** End Enum EBPUserPrivileges ***********************************************************

// ********** Begin Enum EBPLoginStatus ************************************************************
#define FOREACH_ENUM_EBPLOGINSTATUS(op) \
	op(EBPLoginStatus::NotLoggedIn) \
	op(EBPLoginStatus::UsingLocalProfile) \
	op(EBPLoginStatus::LoggedIn) 

enum class EBPLoginStatus : uint8;
template<> struct TIsUEnumClass<EBPLoginStatus> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBPLoginStatus>();
// ********** End Enum EBPLoginStatus **************************************************************

// ********** Begin Enum ESessionSettingSearchResult ***********************************************
#define FOREACH_ENUM_ESESSIONSETTINGSEARCHRESULT(op) \
	op(ESessionSettingSearchResult::Found) \
	op(ESessionSettingSearchResult::NotFound) \
	op(ESessionSettingSearchResult::WrongType) 

enum class ESessionSettingSearchResult : uint8;
template<> struct TIsUEnumClass<ESessionSettingSearchResult> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<ESessionSettingSearchResult>();
// ********** End Enum ESessionSettingSearchResult *************************************************

// ********** Begin Enum EBlueprintResultSwitch ****************************************************
#define FOREACH_ENUM_EBLUEPRINTRESULTSWITCH(op) \
	op(EBlueprintResultSwitch::OnSuccess) \
	op(EBlueprintResultSwitch::OnFailure) 

enum class EBlueprintResultSwitch : uint8;
template<> struct TIsUEnumClass<EBlueprintResultSwitch> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBlueprintResultSwitch>();
// ********** End Enum EBlueprintResultSwitch ******************************************************

// ********** Begin Enum EBlueprintAsyncResultSwitch ***********************************************
#define FOREACH_ENUM_EBLUEPRINTASYNCRESULTSWITCH(op) \
	op(EBlueprintAsyncResultSwitch::OnSuccess) \
	op(EBlueprintAsyncResultSwitch::AsyncLoading) \
	op(EBlueprintAsyncResultSwitch::OnFailure) 

enum class EBlueprintAsyncResultSwitch : uint8;
template<> struct TIsUEnumClass<EBlueprintAsyncResultSwitch> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBlueprintAsyncResultSwitch>();
// ********** End Enum EBlueprintAsyncResultSwitch *************************************************

// ********** Begin Enum EBPServerPresenceSearchType ***********************************************
#define FOREACH_ENUM_EBPSERVERPRESENCESEARCHTYPE(op) \
	op(EBPServerPresenceSearchType::AllServers) \
	op(EBPServerPresenceSearchType::ClientServersOnly) \
	op(EBPServerPresenceSearchType::DedicatedServersOnly) 

enum class EBPServerPresenceSearchType : uint8;
template<> struct TIsUEnumClass<EBPServerPresenceSearchType> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBPServerPresenceSearchType>();
// ********** End Enum EBPServerPresenceSearchType *************************************************

// ********** Begin Enum EBPOnlinePresenceState ****************************************************
#define FOREACH_ENUM_EBPONLINEPRESENCESTATE(op) \
	op(EBPOnlinePresenceState::Online) \
	op(EBPOnlinePresenceState::Offline) \
	op(EBPOnlinePresenceState::Away) \
	op(EBPOnlinePresenceState::ExtendedAway) \
	op(EBPOnlinePresenceState::DoNotDisturb) \
	op(EBPOnlinePresenceState::Chat) 

enum class EBPOnlinePresenceState : uint8;
template<> struct TIsUEnumClass<EBPOnlinePresenceState> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBPOnlinePresenceState>();
// ********** End Enum EBPOnlinePresenceState ******************************************************

// ********** Begin Enum EBPOnlineSessionState *****************************************************
#define FOREACH_ENUM_EBPONLINESESSIONSTATE(op) \
	op(EBPOnlineSessionState::NoSession) \
	op(EBPOnlineSessionState::Creating) \
	op(EBPOnlineSessionState::Pending) \
	op(EBPOnlineSessionState::Starting) \
	op(EBPOnlineSessionState::InProgress) \
	op(EBPOnlineSessionState::Ending) \
	op(EBPOnlineSessionState::Ended) \
	op(EBPOnlineSessionState::Destroying) 

enum class EBPOnlineSessionState : uint8;
template<> struct TIsUEnumClass<EBPOnlineSessionState> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EBPOnlineSessionState>();
// ********** End Enum EBPOnlineSessionState *******************************************************

// ********** Begin Enum EOnlineComparisonOpRedux **************************************************
#define FOREACH_ENUM_EONLINECOMPARISONOPREDUX(op) \
	op(EOnlineComparisonOpRedux::Equals) \
	op(EOnlineComparisonOpRedux::NotEquals) \
	op(EOnlineComparisonOpRedux::GreaterThan) \
	op(EOnlineComparisonOpRedux::GreaterThanEquals) \
	op(EOnlineComparisonOpRedux::LessThan) \
	op(EOnlineComparisonOpRedux::LessThanEquals) 

enum class EOnlineComparisonOpRedux : uint8;
template<> struct TIsUEnumClass<EOnlineComparisonOpRedux> { enum { Value = true }; };
template<> ADVANCEDSESSIONS_API UEnum* StaticEnum<EOnlineComparisonOpRedux>();
// ********** End Enum EOnlineComparisonOpRedux ****************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
