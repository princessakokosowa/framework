const std = @import("std");

inline fn here() []const u8 {
    // Apparently there is a bug regarding pkg-config (I have no idea what it is
    // exactly), which means we are using `linkSystemLibraryName()` instead of
    // `linkSystemLibrary()`.

    return comptime std.fs.path.dirname(@src().file) orelse ".";
}

fn findAndLinkDirect3DRelatedLibraries(exe: *std.Build.CompileStep) void {
    exe.addLibraryPath("C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22000.0\\um\\x64");
    exe.linkSystemLibraryName("d3d12");
    exe.linkSystemLibraryName("dxgi");
    exe.linkSystemLibraryName("dxguid");
}

pub fn build(b: *std.build.Builder) void {
    const target   = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name     = "programme",
        .target   = target,
        .optimize = optimize,
    });
    exe.install();
    exe.linkLibC();

    findAndLinkDirect3DRelatedLibraries(exe);

    exe.addIncludePath("src");
    exe.addCSourceFiles(&.{
        "src/main.c",
    }, &.{
        "-W",
        "-Wall",                    // enables all compiler's warning messages
        "-Werror",                  // treat warning as errors
        // "-Wfatal-errors",           // abort compilation on the first error occurred
        "-Wmissing-field-initializers",
        "-Wno-comment",             // allow multi-line comments
        "-Wno-extern-c-compat",     // disable "empty struct has size 0 in C, size 1 in C++" warning
        "-Wno-sequence-point",      // disable undefined behavior warning
        "-Wno-unknown-pragmas",     // allow unknown pragmas (such as #pragma region)
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        
        "-fno-sanitize=undefined",
        "-fpack-struct=8",          // struct member alignment - 8 bytes
        // "-fstack-protector-strong", // enables stack protection for vulnerable functions that contain: An array of any size and type
        "-fvisibility=hidden",      // hide non-exported symbols
        
        "-g",
        "-pedantic",
        "-std=c17",
    });

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
