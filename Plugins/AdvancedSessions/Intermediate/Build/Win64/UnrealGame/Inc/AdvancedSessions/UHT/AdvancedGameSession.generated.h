// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AdvancedGameSession.h"

#ifdef ADVANCEDSESSIONS_AdvancedGameSession_generated_h
#error "AdvancedGameSession.generated.h already included, missing '#pragma once' in AdvancedGameSession.h"
#endif
#define ADVANCEDSESSIONS_AdvancedGameSession_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Class AAdvancedGameSession *****************************************************
ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_AAdvancedGameSession_NoRegister();

#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h_26_INCLASS \
private: \
	static void StaticRegisterNativesAAdvancedGameSession(); \
	friend struct Z_Construct_UClass_AAdvancedGameSession_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_AAdvancedGameSession_NoRegister(); \
public: \
	DECLARE_CLASS2(AAdvancedGameSession, AGameSession, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), Z_Construct_UClass_AAdvancedGameSession_NoRegister) \
	DECLARE_SERIALIZER(AAdvancedGameSession)


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h_26_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AAdvancedGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AAdvancedGameSession) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AAdvancedGameSession); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AAdvancedGameSession); \
	/** Deleted move- and copy-constructors, should never be used */ \
	AAdvancedGameSession(AAdvancedGameSession&&) = delete; \
	AAdvancedGameSession(const AAdvancedGameSession&) = delete; \
	NO_API virtual ~AAdvancedGameSession();


#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h_23_PROLOG
#define FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h_26_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h_26_INCLASS \
	FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h_26_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AAdvancedGameSession;

// ********** End Class AAdvancedGameSession *******************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_qudtn_UnrealEngine_NoobGame_Plugins_AdvancedSessions_Source_AdvancedSessions_Classes_AdvancedGameSession_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
