#pragma once
#include <cstdint>
#include "Vector3.h"

namespace Addresses
{
	uintptr_t GWorld = 0x11791DF8;
}

namespace Offsets
{
	// main [draw]
	uintptr_t LocalPawn = 0x338;
	uintptr_t PlayerController = 0x30;
	uintptr_t LocalPlayers = 0x38;
	uintptr_t PlayerState = 0x2b0;
	uintptr_t GameState = 0x158;
	uintptr_t GameInstance = 0x1d0;
	uintptr_t PlayerArray = 0x2A8;
	uintptr_t RootC = 0x198;
	uintptr_t TIndex = 0x10f1;
	//uintptr_t BoneArray = 0x608;
	uintptr_t C2W = 0x1c0;
	uintptr_t ActorMesh = 0x318;
	uintptr_t PersistentLevel = 0x30;
	uintptr_t LastRenderTime = 0x2E4;
	uintptr_t LastSumbitTime = 0x2EC;
	// alt [misc]
}

namespace Cache
{
	uintptr_t BaseAddress;
	uintptr_t World; // GWorld
	uintptr_t PlayerController; // Actor-PlayerController
	uintptr_t LocalPlayers; // GameInstance-LocalPlayers
	uintptr_t LocalPlayer; // GameInstance-LocalPlayers
	uintptr_t GameState; // Uworld-GameState
	uintptr_t PlayerArray; // FortWorld-PlayerArray
	uintptr_t RootComponent; // GameInstace-RootComponent
	uintptr_t LocalPawn; // FortWorld-LocalPawn
	uintptr_t GameInstance; // Uworld-GameInstance
	uintptr_t TIndex; // Actor-TeamIndex
	uintptr_t C2W; // FortWorld-ComponentToWorld
	uintptr_t PState;
	uintptr_t PersistentState;
	uintptr_t CurrentActorMesh;
	Vector3 LocalActorPosition;
}