////////////////////////////////
// Includes, unity build

// os features
#define OS_FEATURE_GFX 1
#define OS_FEATURE_NET 1

// [c]
#include "basic/basic_include.h"
#include "os/os_include.h"
#include "tests/tests_include.h"

// [h]
#include "basic/basic_include.c"
#include "os/os_include.c"
#include "tests/tests_include.c"

// entry point
#include "os/os_entry_point.c"

////////////////////////////////
// Entry Points

core_function void entryPoint(void) {
    /*
    // initialize dependency layers
    OS_InitReceipt os_init = OS_Init();
    OS_InitGfxReceipt os_init_gfx = OS_InitGfx(os_init);
    R_InitReceipt r_init = R_Init(os_init, os_init_gfx);
    C_InitReceipt c_init = C_Init(os_init);
    FP_InitReceipt fp_init = FP_Init(c_init);
    F_InitReceipt f_init = F_Init(fp_init, r_init, V2S64(1024, 1024));
    D_InitReceipt d_init = D_Init(r_init, f_init);

    // open window
    OS_Handle window = OS_WindowOpen(0, V2S64(1280, 720), Str8Lit("Application Template"));
    R_Handle window_r = R_WindowEquip(window);
    OS_WindowFirstPaint(window);


    // main loop
    for(bool quit = false; quit == false;) {
        ArenaTemp scratch = GetScratch(0, 0);

        // get events
        OS_EventList events = OS_GetEvents(scratch.arena);

        // draw
        {
            Rng2F32 client_rect = OS_ClientRectFromWindow(window);
            Vec2F32 client_rect_dim = Dim2F32(client_rect);
            Vec2S64 render_dim = Vec2S64FromVec(client_rect_dim);
            R_WindowStart(window_r, render_dim);
            R_WindowFinish(window_r);
        }

        // process window close events
        for(OS_Event *event = events.first; event != 0; event = event->next) {
            if(event->kind == OS_EventKind_WindowClose) {
                quit = 1;
                break;
            }
        }

        ReleaseScratch(scratch);
    }
    */

    ArrayTests_test();
}
