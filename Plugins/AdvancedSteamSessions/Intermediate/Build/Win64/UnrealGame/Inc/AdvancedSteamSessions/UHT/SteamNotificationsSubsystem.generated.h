// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SteamNotificationsSubsystem.h"

#ifdef ADVANCEDSTEAMSESSIONS_SteamNotificationsSubsystem_generated_h
#error "SteamNotificationsSubsystem.generated.h already included, missing '#pragma once' in SteamNotificationsSubsystem.h"
#endif
#define ADVANCEDSTEAMSESSIONS_SteamNotificationsSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Delegate FOnSteamOverlayActivated **********************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_16_DELEGATE \
ADVANCEDSTEAMSESSIONS_API void FOnSteamOverlayActivated_DelegateWrapper(const FMulticastScriptDelegate& OnSteamOverlayActivated, bool bOverlayState);


// ********** End Delegate FOnSteamOverlayActivated ************************************************

// ********** Begin Class USteamNotificationsSubsystem *********************************************
ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamNotificationsSubsystem_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSteamNotificationsSubsystem(); \
	friend struct Z_Construct_UClass_USteamNotificationsSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamNotificationsSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(USteamNotificationsSubsystem, UGameInstanceSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSteamSessions"), Z_Construct_UClass_USteamNotificationsSubsystem_NoRegister) \
	DECLARE_SERIALIZER(USteamNotificationsSubsystem)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_21_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	USteamNotificationsSubsystem(USteamNotificationsSubsystem&&) = delete; \
	USteamNotificationsSubsystem(const USteamNotificationsSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USteamNotificationsSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamNotificationsSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(USteamNotificationsSubsystem) \
	NO_API virtual ~USteamNotificationsSubsystem();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_18_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_21_INCLASS_NO_PURE_DECLS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class USteamNotificationsSubsystem;

// ********** End Class USteamNotificationsSubsystem ***********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
