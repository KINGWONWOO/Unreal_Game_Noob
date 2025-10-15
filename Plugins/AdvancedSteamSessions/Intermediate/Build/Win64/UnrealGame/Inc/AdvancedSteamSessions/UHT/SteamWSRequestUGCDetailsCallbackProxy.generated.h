// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "SteamWSRequestUGCDetailsCallbackProxy.h"

#ifdef ADVANCEDSTEAMSESSIONS_SteamWSRequestUGCDetailsCallbackProxy_generated_h
#error "SteamWSRequestUGCDetailsCallbackProxy.generated.h already included, missing '#pragma once' in SteamWSRequestUGCDetailsCallbackProxy.h"
#endif
#define ADVANCEDSTEAMSESSIONS_SteamWSRequestUGCDetailsCallbackProxy_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class UObject;
class USteamWSRequestUGCDetailsCallbackProxy;
struct FBPSteamWorkshopID;
struct FBPSteamWorkshopItemDetails;

// ********** Begin Delegate FBlueprintWorkshopDetailsDelegate *************************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_50_DELEGATE \
ADVANCEDSTEAMSESSIONS_API void FBlueprintWorkshopDetailsDelegate_DelegateWrapper(const FMulticastScriptDelegate& BlueprintWorkshopDetailsDelegate, FBPSteamWorkshopItemDetails const& WorkShopDetails);


// ********** End Delegate FBlueprintWorkshopDetailsDelegate ***************************************

// ********** Begin Class USteamWSRequestUGCDetailsCallbackProxy ***********************************
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_RPC_WRAPPERS \
	DECLARE_FUNCTION(execGetWorkshopItemDetails);


ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamWSRequestUGCDetailsCallbackProxy_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_INCLASS \
private: \
	static void StaticRegisterNativesUSteamWSRequestUGCDetailsCallbackProxy(); \
	friend struct Z_Construct_UClass_USteamWSRequestUGCDetailsCallbackProxy_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamWSRequestUGCDetailsCallbackProxy_NoRegister(); \
public: \
	DECLARE_CLASS2(USteamWSRequestUGCDetailsCallbackProxy, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSteamSessions"), Z_Construct_UClass_USteamWSRequestUGCDetailsCallbackProxy_NoRegister) \
	DECLARE_SERIALIZER(USteamWSRequestUGCDetailsCallbackProxy)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSTEAMSESSIONS_API USteamWSRequestUGCDetailsCallbackProxy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USteamWSRequestUGCDetailsCallbackProxy) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSTEAMSESSIONS_API, USteamWSRequestUGCDetailsCallbackProxy); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USteamWSRequestUGCDetailsCallbackProxy); \
	/** Deleted move- and copy-constructors, should never be used */ \
	USteamWSRequestUGCDetailsCallbackProxy(USteamWSRequestUGCDetailsCallbackProxy&&) = delete; \
	USteamWSRequestUGCDetailsCallbackProxy(const USteamWSRequestUGCDetailsCallbackProxy&) = delete; \
	ADVANCEDSTEAMSESSIONS_API virtual ~USteamWSRequestUGCDetailsCallbackProxy();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_52_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_RPC_WRAPPERS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h_55_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class USteamWSRequestUGCDetailsCallbackProxy;

// ********** End Class USteamWSRequestUGCDetailsCallbackProxy *************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamWSRequestUGCDetailsCallbackProxy_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
