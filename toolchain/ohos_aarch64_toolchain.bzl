"""
This file is copied from the SkCMS repository. Original file:
https://skia.googlesource.com/skcms/+/ba39d81f9797aa973bdf01aa6b0363b280352fba/toolchain/ndk_linux_arm64_toolchain_config.bzl

This module defines the ndk_cc_toolchain_config rule.

This file is based on the `external/androidndk/cc_toolchain_config.bzl` file produced by the
built-in `android_ndk_repository` Bazel rule[1], which was used to build the SkCMS repository up
until this revision[2].

The paths in this file point to locations inside the expanded Android NDK ZIP file (found at
external/ndk_linux_amd64), and must be updated every time we upgrade to a new Android NDK version.

[1] https://github.com/bazelbuild/bazel/blob/4710ef82ce34572878e07c52e83a0144d707f140/src/main/java/com/google/devtools/build/lib/bazel/rules/android/AndroidNdkRepositoryFunction.java#L422
[2] https://skia.googlesource.com/skcms/+/30c8e303800c256febb03a09fdcda7f75d119b1b/WORKSPACE#22
"""

load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
    "with_feature_set",
)
load(":download_ndk_linux_amd64_toolchain.bzl", "SDK_PATH")

# Supported CPUs.
_ARM64_ = "arm64"

_all_compile_actions = [
    ACTION_NAMES.c_compile,
    ACTION_NAMES.cpp_compile,
    ACTION_NAMES.linkstamp_compile,
    ACTION_NAMES.assemble,
    ACTION_NAMES.preprocess_assemble,
    ACTION_NAMES.cpp_header_parsing,
    ACTION_NAMES.cpp_module_compile,
    ACTION_NAMES.cpp_module_codegen,
    ACTION_NAMES.clif_match,
    ACTION_NAMES.lto_backend,
]

_all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _get_default_compile_flags(cpu):
    if cpu == _ARM64_:
        return [
            "-target",
            "aarch64-linux-ohos",
            "-fpic",
            "-isystem",
            SDK_PATH + "native/sysroot/usr/include/aarch64-linux-ohos",
            "-no-canonical-prefixes",
            "-Wno-invalid-command-line-argument",
            "-Wno-unused-command-line-argument",
            "-funwind-tables",
            "-fstack-protector-strong",
            "-fno-addrsig",
        ]
    fail("Unknown CPU: " + cpu)

def _get_default_link_flags(cpu):
    if cpu == _ARM64_:
        return [
            "-target",
            "aarch64-none-linux-ohos",
            "-L",
            SDK_PATH + "native/sysroot/usr/include/aarch64-linux-ohos",
            "-L",
            SDK_PATH + "native/lib/aarch64-linux-ohos",
            "-no-canonical-prefixes",
            "-Wl,-z,relro",
            "-lm",
        ]
    fail("Unknown CPU: " + cpu)

def _get_default_dbg_flags(cpu):
    if cpu == _ARM64_:
        return ["-O0", "-g", "-UNDEBUG"]
    fail("Unknown CPU: " + cpu)

def _get_default_opt_flags(cpu):
    if cpu == _ARM64_:
        return ["-O2", "-g", "-DNDEBUG"]
    fail("Unknown CPU: " + cpu)

def _get_toolchain_identifier(cpu):
    if cpu == _ARM64_:
        return "sdk-arm64-v8a-toolchain"
    fail("Unknown CPU: " + cpu)

def _get_target_system_name(cpu):
    if cpu == _ARM64_:
        return "aarch64-linux-ohos"
    fail("Unknown CPU: " + cpu)

def _get_builtin_sysroot(cpu):
    if cpu == _ARM64_:
        return SDK + "native/sysroot/usr/include/aarch64-linux-ohos"
    fail("Unknown CPU: " + cpu)

def _get_tool_paths(cpu):
    # The cc_common.create_cc_toolchain_config_info function expects tool paths to point to files
    # under the directory in which it is invoked. This means we cannot directly reference tools
    # under external/android_sdk. The solution is to use "trampoline" scripts that pass through
    # any command-line arguments to the SDK binaries under external/android_sdk.

    if cpu == _ARM64_:
        return [
            tool_path(
                name = "ar",
                path = "native/llvm/llvm-ar",
            ),
            tool_path(
                name = "cpp",
                path = "native/llvm/clang-15",
            ),
            tool_path(
                name = "gcov",
                path = "native/llvm/llvm-cov",
            ),
            tool_path(
                name = "ld",
                path = "native/llvm/lld",
            ),
            tool_path(
                name = "nm",
                path = "native/llvm/llvm-nm",
            ),
            tool_path(
                name = "objcopy",
                path = "native/llvm/llvm-objcopy",
            ),
            tool_path(
                name = "objdump",
                path = "native/llvm/llvm-objdump",
            ),
            tool_path(
                name = "strip",
                path = "native/llvm/llvm-strip",
            ),
        ]
    fail("Unknown CPU: " + cpu)

def _sdk_cc_toolchain_config_impl(ctx):
    default_compile_flags = _get_default_compile_flags(ctx.attr.cpu)
    unfiltered_compile_flags = [
        "-isystem",
        SDK_PATH + "/sources/cxx-stl/llvm-libc++/include",
        "-isystem",
        SDK_PATH + "/sources/cxx-stl/llvm-libc++abi/include",
        "-isystem",
        SDK_PATH + "/sources/android/support/include",
        "-isystem",
        SDK_PATH + "/sysroot/usr/include",
    ]
    default_link_flags = _get_default_link_flags(ctx.attr.cpu)
    default_fastbuild_flags = [""]
    default_dbg_flags = _get_default_dbg_flags(ctx.attr.cpu)
    default_opt_flags = _get_default_opt_flags(ctx.attr.cpu)

    opt_feature = feature(name = "opt")
    fastbuild_feature = feature(name = "fastbuild")
    dbg_feature = feature(name = "dbg")
    supports_dynamic_linker_feature = feature(name = "supports_dynamic_linker", enabled = True)
    supports_pic_feature = feature(name = "supports_pic", enabled = True)
    static_link_cpp_runtimes_feature = feature(name = "static_link_cpp_runtimes", enabled = True)

    default_compile_flags_feature = feature(
        name = "default_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _all_compile_actions,
                flag_groups = [flag_group(flags = default_compile_flags)],
            ),
            flag_set(
                actions = _all_compile_actions,
                flag_groups = [flag_group(flags = default_fastbuild_flags)],
                with_features = [with_feature_set(features = ["fastbuild"])],
            ),
            flag_set(
                actions = _all_compile_actions,
                flag_groups = [flag_group(flags = default_dbg_flags)],
                with_features = [with_feature_set(features = ["dbg"])],
            ),
            flag_set(
                actions = _all_compile_actions,
                flag_groups = [flag_group(flags = default_opt_flags)],
                with_features = [with_feature_set(features = ["opt"])],
            ),
            flag_set(
                actions = [ACTION_NAMES.cpp_compile],
                flag_groups = [flag_group(flags = ["-std=c++17"])],
            ),
        ],
    )

    default_link_flags_feature = feature(
        name = "default_link_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _all_link_actions,
                flag_groups = [flag_group(flags = default_link_flags)],
            ),
        ],
    )

    user_compile_flags_feature = feature(
        name = "user_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _all_compile_actions,
                flag_groups = [
                    flag_group(
                        flags = ["%{user_compile_flags}"],
                        iterate_over = "user_compile_flags",
                        expand_if_available = "user_compile_flags",
                    ),
                ],
            ),
        ],
    )

    sysroot_feature = feature(
        name = "sysroot",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _all_compile_actions + _all_link_actions,
                flag_groups = [
                    flag_group(
                        flags = ["--sysroot=%{sysroot}"],
                        expand_if_available = "sysroot",
                    ),
                ],
            ),
        ],
    )

    unfiltered_compile_flags_feature = feature(
        name = "unfiltered_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _all_compile_actions,
                flag_groups = [flag_group(flags = unfiltered_compile_flags)],
            ),
        ],
    )

    features = [
        default_compile_flags_feature,
        default_link_flags_feature,
        supports_dynamic_linker_feature,
        supports_pic_feature,
        static_link_cpp_runtimes_feature,
        fastbuild_feature,
        dbg_feature,
        opt_feature,
        user_compile_flags_feature,
        sysroot_feature,
        unfiltered_compile_flags_feature,
    ]

    cxx_builtin_include_directories = [
        SDK_PATH + "/native/llvm/include",
        "%sysroot%/usr/include",
        SDK_PATH + "native/sysroot/usr/include",
    ]

    # https://bazel.build/rules/lib/cc_common#create_cc_toolchain_config_info
    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = _get_toolchain_identifier(ctx.attr.cpu),
        host_system_name = "local",
        target_system_name = _get_target_system_name(ctx.attr.cpu),
        target_cpu = ctx.attr.cpu,
        target_libc = "local",
        compiler = "clang15.0.4",
        abi_version = ctx.attr.cpu,
        abi_libc_version = "local",
        features = features,
        tool_paths = _get_tool_paths(ctx.attr.cpu),
        cxx_builtin_include_directories = cxx_builtin_include_directories,
        builtin_sysroot = _get_builtin_sysroot(ctx.attr.cpu),
    )

sdk_cc_toolchain_config = rule(
    implementation = _sdk_cc_toolchain_config_impl,
    attrs = {
        "cpu": attr.string(
            mandatory = True,
            values = [_ARM64_],
            doc = "Target CPU.",
        ),
    },
    provides = [CcToolchainConfigInfo],
)
