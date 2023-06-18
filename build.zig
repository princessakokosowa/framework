const std = @import("std");

fn concat(a: []const u8, b: []const u8, allocator: std.mem.Allocator) ![]u8 {
    const result = try allocator.alloc(u8, a.len + b.len);

    std.mem.copy(u8, result, a);
    std.mem.copy(u8, result[a.len..], b);

    return result;
}

fn ascending(context: std.mem.Allocator, a: []const u8, b: []const u8) bool {
    _ = context;

    return std.mem.eql(u8, a, b);
}

fn findWindowsKitsAndAddItsLibraryPath(b: *std.build.Builder, lib: *std.Build.CompileStep) !void {
    // This is where Windows Kits has been for years, I don't mind if it's hardcoded.
    const windows_kits_lib_path = "C:\\Program Files (x86)\\Windows Kits\\10\\Lib";
    const libs_subpath = "\\um\\x64";

    // Here, we hope to find _Windows Kits_ folder.
    var windows_kits_lib_dir = try std.fs.openIterableDirAbsolute(windows_kits_lib_path, .{
        .access_sub_paths = true,
    });

    var dirs = std.ArrayList([]const u8).init(b.allocator);
    defer dirs.deinit();

    // If it is found, there _must_ be a specific version of Windows Kits installed,
    // unless something is broken (either deliberately or not).
    //
    // In any case, we are trying to find the latest version here.
    var it = windows_kits_lib_dir.iterate();
    while (try it.next()) |dir| {
        if (dir.kind != .Directory) continue;

        try dirs.append(b.dupe(dir.name));
    }

    // If not found, exit here.
    if (dirs.getLastOrNull() == null) return error.NoVersionAvailable;

    // I am not sure whether we can rely on std library to automatically sort the
    // results for us (they may rely on Windows providing them the already sorted list
    // of directories), so we sort them ourselves.
    std.sort.sort([]const u8, dirs.items, b.allocator, ascending);

    const version = dirs.getLast();
    const version_subpath = try concat("\\", version, b.allocator);
    defer b.allocator.free(version_subpath);

    const windows_kits_lib_version_path = try concat(windows_kits_lib_path, version_subpath, b.allocator);
    defer b.allocator.free(windows_kits_lib_version_path);

    const windows_kits_lib_full_path = try concat(windows_kits_lib_version_path, libs_subpath, b.allocator);
    defer b.allocator.free(windows_kits_lib_full_path);

    // Finally, we add Windows Kits to our library paths.
    lib.addLibraryPath(windows_kits_lib_full_path);
}

fn findWindowsKitsAndAddItsIncludePath(b: *std.build.Builder, lib: *std.Build.CompileStep) !void {
    // This is where Windows Kits has been for years, I don't mind if it's hardcoded.
    const windows_kits_include_path = "C:\\Program Files (x86)\\Windows Kits\\10\\Include";
    const libs_subpath      = "\\um";

    // Here, we hope to find _Windows Kits_ folder.
    var windows_kits_include_dir = try std.fs.openIterableDirAbsolute(windows_kits_include_path, .{
        .access_sub_paths = true,
    });

    var dirs = std.ArrayList([]const u8).init(b.allocator);
    defer dirs.deinit();

    // If it is found, there _must_ be a specific version of Windows Kits installed,
    // unless something is broken (either deliberately or not).
    //
    // In any case, we are trying to find the latest version here.
    var it = windows_kits_include_dir.iterate();
    while (try it.next()) |dir| {
        if (dir.kind != .Directory) continue;

        try dirs.append(b.dupe(dir.name));
    }

    // If not found, exit here.
    if (dirs.getLastOrNull() == null) return error.NoVersionAvailable;

    // I am not sure whether we can rely on std library to automatically sort the
    // results for us (they may rely on Windows providing them the already sorted list
    // of directories), so we sort them ourselves.
    std.sort.sort([]const u8, dirs.items, b.allocator, ascending);

    const version = dirs.getLast();
    const version_subpath = try concat("\\", version, b.allocator);
    defer b.allocator.free(version_subpath);

    const windows_kits_include_version_path = try concat(windows_kits_include_path,version_subpath, b.allocator);
    defer b.allocator.free(windows_kits_include_version_path);

    const windows_kits_include_full_path = try concat(windows_kits_include_version_path, libs_subpath, b.allocator);
    defer b.allocator.free(windows_kits_include_full_path);

    // Finally, we add Windows Kits to our library paths.
    lib.addIncludePath(windows_kits_include_full_path);
}

pub fn build(b: *std.Build) !void {
    const target   = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "framework",
        // .root_source_file = .{ .path = "src/app.c" },
        .target = target,
        .optimize = optimize,
    });

    {
        // const zig_directx_headers = b.dependency("zig_directx_headers", .{
        //     .target = target,
        //     .optimize = .ReleaseFast,
        // });

        // const directx_guids = zig_directx_headers.artifact("DirectX-Guids")
        // const directx_headers = zig_directx_headers.artifact("DirectX-Headers")
        // 
        // exe.linkLibrary(directx_guids);
        // exe.linkLibrary(directx_headers);
        exe.linkLibC();

        // Apparently there is a bug regarding pkg-config (I have no idea what it is
        // exactly), which means we are using `linkSystemLibraryName()` instead of
        // `linkSystemLibrary()`.
        switch (exe.target.getOsTag()) {
            .windows => {
                try findWindowsKitsAndAddItsLibraryPath(b, exe);
                // try findWindowsKitsUserModeIncludePath(b, lib);

                exe.linkSystemLibraryName("d3d12");
                exe.linkSystemLibraryName("dxgi");
                exe.linkSystemLibraryName("dxguid");
            },
            // Yes, this means that you cannot compile this code on any operating system
            // other than Windows. Sorry.
            else => unreachable,
        }

        exe.addIncludePath("src");
        exe.addCSourceFiles(&.{
            "src/app.c",
        }, &.{
            // Warnings and errors.
            "-Wall",
            "-Werror",
            "-Wfatal-errors",
            // "-Wno-gnu-statement-expression",
            // "-Wno-gnu-statement-expression-from-macro-expansion",
            "-Wmissing-field-initializers",
            "-Wno-comment",
            "-Wno-extern-c-compat",
            "-Wno-sequence-point",
            "-Wno-unknown-pragmas",
            "-Wstrict-prototypes",
            "-Wwrite-strings",

            // Code-relared things.
            "-fno-sanitize=undefined",
            "-fpack-struct=8",
            // "-fstack-protector-strong",
            "-fvisibility=hidden",

            // Misc.
            "-g",
            "-pedantic",
            "-std=c17",

            // "-Wno-for-loop-analysis",
            // "-Wno-uninitialized",
        });
    }

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
