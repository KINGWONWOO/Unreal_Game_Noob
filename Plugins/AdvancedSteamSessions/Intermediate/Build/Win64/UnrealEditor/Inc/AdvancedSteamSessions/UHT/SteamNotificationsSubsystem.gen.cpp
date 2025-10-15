// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SteamNotificationsSubsystem.h"
#include "Engine/GameInstance.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

void EmptyLinkFunctionForGeneratedCodeSteamNotificationsSubsystem() {}

// ********** Begin Cross Module References ********************************************************
ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamNotificationsSubsystem();
ADVANCEDSTEAMSESSIONS_API UClass* Z_Construct_UClass_USteamNotificationsSubsystem_NoRegister();
ADVANCEDSTEAMSESSIONS_API UFunction* Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature();
ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
UPackage* Z_Construct_UPackage__Script_AdvancedSteamSessions();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOnSteamOverlayActivated **********************************************
struct Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics
{
	struct _Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms
	{
		bool bOverlayState;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SteamNotificationsSubsystem.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bOverlayState_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bOverlayState;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};
void Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::NewProp_bOverlayState_SetBit(void* Obj)
{
	((_Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms*)Obj)->bOverlayState = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::NewProp_bOverlayState = { "bOverlayState", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(_Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms), &Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::NewProp_bOverlayState_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::NewProp_bOverlayState,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::PropPointers) < 2048);
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_AdvancedSteamSessions, nullptr, "OnSteamOverlayActivated__DelegateSignature", Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::PropPointers), sizeof(Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::_Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::_Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOnSteamOverlayActivated_DelegateWrapper(const FMulticastScriptDelegate& OnSteamOverlayActivated, bool bOverlayState)
{
	struct _Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms
	{
		bool bOverlayState;
	};
	_Script_AdvancedSteamSessions_eventOnSteamOverlayActivated_Parms Parms;
	Parms.bOverlayState=bOverlayState ? true : false;
	OnSteamOverlayActivated.ProcessMulticastDelegate<UObject>(&Parms);
}
// ********** End Delegate FOnSteamOverlayActivated ************************************************

// ********** Begin Class USteamNotificationsSubsystem *********************************************
void USteamNotificationsSubsystem::StaticRegisterNativesUSteamNotificationsSubsystem()
{
}
FClassRegistrationInfo Z_Registration_Info_UClass_USteamNotificationsSubsystem;
UClass* USteamNotificationsSubsystem::GetPrivateStaticClass()
{
	using TClass = USteamNotificationsSubsystem;
	if (!Z_Registration_Info_UClass_USteamNotificationsSubsystem.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			StaticPackage(),
			TEXT("SteamNotificationsSubsystem"),
			Z_Registration_Info_UClass_USteamNotificationsSubsystem.InnerSingleton,
			StaticRegisterNativesUSteamNotificationsSubsystem,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_USteamNotificationsSubsystem.InnerSingleton;
}
UClass* Z_Construct_UClass_USteamNotificationsSubsystem_NoRegister()
{
	return USteamNotificationsSubsystem::GetPrivateStaticClass();
}
struct Z_Construct_UClass_USteamNotificationsSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "SteamNotificationsSubsystem.h" },
		{ "ModuleRelativePath", "Classes/SteamNotificationsSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OnSteamOverlayActivated_Bind_MetaData[] = {
		{ "Category", "SteamEvents" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Event thrown when the steam overlay switches states\n" },
#endif
		{ "ModuleRelativePath", "Classes/SteamNotificationsSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Event thrown when the steam overlay switches states" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSteamOverlayActivated_Bind;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USteamNotificationsSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USteamNotificationsSubsystem_Statics::NewProp_OnSteamOverlayActivated_Bind = { "OnSteamOverlayActivated_Bind", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USteamNotificationsSubsystem, OnSteamOverlayActivated_Bind), Z_Construct_UDelegateFunction_AdvancedSteamSessions_OnSteamOverlayActivated__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OnSteamOverlayActivated_Bind_MetaData), NewProp_OnSteamOverlayActivated_Bind_MetaData) }; // 3230519732
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USteamNotificationsSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USteamNotificationsSubsystem_Statics::NewProp_OnSteamOverlayActivated_Bind,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNotificationsSubsystem_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_USteamNotificationsSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_AdvancedSteamSessions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNotificationsSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_USteamNotificationsSubsystem_Statics::ClassParams = {
	&USteamNotificationsSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_USteamNotificationsSubsystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_USteamNotificationsSubsystem_Statics::PropPointers),
	0,
	0x009000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_USteamNotificationsSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_USteamNotificationsSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_USteamNotificationsSubsystem()
{
	if (!Z_Registration_Info_UClass_USteamNotificationsSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_USteamNotificationsSubsystem.OuterSingleton, Z_Construct_UClass_USteamNotificationsSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_USteamNotificationsSubsystem.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR(USteamNotificationsSubsystem);
USteamNotificationsSubsystem::~USteamNotificationsSubsystem() {}
// ********** End Class USteamNotificationsSubsystem ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h__Script_AdvancedSteamSessions_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_USteamNotificationsSubsystem, USteamNotificationsSubsystem::StaticClass, TEXT("USteamNotificationsSubsystem"), &Z_Registration_Info_UClass_USteamNotificationsSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(USteamNotificationsSubsystem), 3725206527U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h__Script_AdvancedSteamSessions_1309998382(TEXT("/Script/AdvancedSteamSessions"),
	Z_CompiledInDeferFile_FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h__Script_AdvancedSteamSessions_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSteamSessions_Source_AdvancedSteamSessions_Classes_SteamNotificationsSubsystem_h__Script_AdvancedSteamSessions_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
