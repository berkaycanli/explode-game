#ifndef NETWORK_H
#define NETWORK_H

#include "input.h"
#include <stdbool.h>

// Networking Init
// Returns true on success, false on failure.
// Sets *localPlayerId: 0 for Server (Host), 1 for Client (Joiner)
bool Network_Init(int* localPlayerId);

// Update network state.
// Sends local player's input, receives remote player's input.
void Network_Update(Input* p1Input, Input* p2Input, int localPlayerId);

// Cleanup resources
void Network_Shutdown();

#endif
