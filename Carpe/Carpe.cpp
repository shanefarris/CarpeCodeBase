// Fill out your copyright notice in the Description page of Project Settings.

#include "Carpe.h"
#include "Modules/ModuleManager.h"
#include "GenericApplication.h"
#include "GenericApplicationMessageHandler.h"
#include "SlateBasics.h"
#include "SlateExtras.h"

class FCoreFrameworkModule : public ICoreFrameworkModule
{
public:
	virtual void StartupModule() override
	{		
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen() override
	{
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Carpe, "Carpe" );
