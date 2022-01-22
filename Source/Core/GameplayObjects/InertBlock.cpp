// Fill out your copyright notice in the Description page of Project Settings.


#include "InertBlock.h"

AInertBlock::AInertBlock()
{
	bIsYeetable = false;
	Mesh->SetCollisionProfileName("InertBlock");
}