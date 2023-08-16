#ifndef BASIC_H
#define BASIC_H

// Move that to the build script.
#define BUILD_CORE_LINK_MODE BUILD_LINK_MODE_SOURCE

////////////////////////////////
// Basic headers

#include "basic_context_crack.h"
#include "basic_types.h"
#include "basic_allocator.h"
#include "basic_arena.h"
#include "basic_scratch.h"
#include "basic_array.h"
#include "basic_math.h" // Windows-provided, @TODO make self.
#include "basic_thread.h"

#endif // BASIC_H
