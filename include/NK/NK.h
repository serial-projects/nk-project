/**
 * Ruka Raposa Project by Ruka Raposa Team & Contribuitors.
 * Project is under GPL3-license, read LICENSE.md on the root.
 */
#ifndef NK_h
#define NK_h

#include "NK/Core.h"
#include "NK/Types.h"
#include "NK/Support.h"
#include "NK/Redirects.h"

/**
 * @brief Initialize the NK subsystems that are necessary.
 */
void
NK_Init();

/**
 * @brief Free the NK subsystems that are necessary.
 */
void
NK_Quit();

/**
 * @brief Get the NK initialization status.
 */
const NK_Result
NK_IsInitialized();

#endif