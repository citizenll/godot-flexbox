#!/usr/bin/env python

import os
import sys
import subprocess

if sys.version_info < (3,):
    def decode_utf8(x):
        return x
else:
    import codecs
    def decode_utf8(x):
        return codecs.utf_8_decode(x)[0]

# Workaround for MinGW. See:
# http://www.scons.org/wiki/LongCmdLinesOnWin32
if (os.name=="nt"):
    import subprocess

    def mySubProcess(cmdline,env):
        #print "SPAWNED : " + cmdline
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, startupinfo=startupinfo, shell = False, env = env)
        data, err = proc.communicate()
        rv = proc.wait()
        if rv:
            print("=====")
            print(err.decode("utf-8"))
            print("=====")
        return rv

    def mySpawn(sh, escape, cmd, args, env):

        newargs = " ".join(args[1:])
        cmdline = cmd + " " + newargs

        rv=0
        if len(cmdline) > 32000 and cmd.endswith("ar") :
            cmdline = cmd + " " + args[1] + " " + args[2] + " "
            for i in range(3,len(args)) :
                rv = mySubProcess( cmdline + args[i], env )
                if rv :
                    break
        else:
            rv = mySubProcess( cmdline, env )

        return rv

def add_sources(sources, dir, extension):
    for f in os.listdir(dir):
        if f.endswith("." + extension):
            sources.append(dir + "/" + f)

# Try to detect the host platform automatically.
# This is used if no `platform` argument is passed
if sys.platform.startswith("linux"):
    host_platform = "linux"
elif sys.platform == "darwin":
    host_platform = "macos"
elif sys.platform == "win32" or sys.platform == "msys":
    host_platform = "windows"
else:
    raise ValueError(
        "Could not detect platform automatically, please specify with "
        "platform=<platform>"
    )

opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable(
    "platform",
    "Target platform",
    host_platform,
    allowed_values=("linux", "macos", "windows", "android", "ios", "web"),
    ignorecase=2
))
opts.Add(EnumVariable(
    "bits",
    "Target platform bits",
    "default",
    ("default", "32", "64")
))
opts.Add(BoolVariable(
    "use_llvm",
    "Use the LLVM compiler - only effective when targeting Linux",
    False
))
opts.Add(BoolVariable(
    "use_mingw",
    "Use the MinGW compiler instead of MSVC - only effective on Windows",
    False
))
# Must be the same setting as used for cpp_bindings
opts.Add(EnumVariable(
    "target",
    "Compilation target",
    "debug",
    allowed_values=("debug", "release"),
    ignorecase=2
))
opts.Add(PathVariable(
    "custom_api_file",
    "Path to a custom JSON API file",
    None,
    PathVariable.PathIsFile
))
opts.Add(BoolVariable(
    "generate_bindings",
    "Generate GDNative API bindings",
    False
))
opts.Add(EnumVariable(
    "android_arch",
    "Target Android architecture",
    "armv7",
    ["armv7","arm64v8","x86","x86_64"]
))
opts.Add(EnumVariable(
    "arch",
    "Target MacOS architecture",
    "arm64",
    ["arm64", "x86_64"]
))
opts.Add(EnumVariable(
    "ios_arch",
    "Target iOS architecture",
    "arm64",
    ["armv7", "arm64", "x86_64"]
))
opts.Add(
    "XCODEPATH",
    "Path to Xcode toolchain",
    "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain",
)
opts.Add(
    "android_api_level",
    "Target Android API level",
    "19" if ARGUMENTS.get("android_arch", "armv7") in ["armv7", "x86"] else "21"
)
opts.Add(
    "ANDROID_NDK_ROOT",
    "Path to your Android NDK installation. By default, uses ANDROID_NDK_ROOT from your defined environment variables.",
    os.environ.get("ANDROID_NDK_ROOT", None)
)
opts.Add(
    "EMSDK",
    "Path to your Android NDK installation. By default, uses EMSDK from your defined environment variables.",
    os.environ.get("EMSDK", None)
)

env = Environment(ENV = os.environ)
opts.Update(env)
Help(opts.GenerateHelpText(env))

is64 = sys.maxsize > 2**32
if (
    env["TARGET_ARCH"] == "amd64" or
    env["TARGET_ARCH"] == "emt64" or
    env["TARGET_ARCH"] == "x86_64" or
    env["TARGET_ARCH"] == "arm64-v8a"
):
    is64 = True

if env["bits"] == "default":
    env["bits"] = "64" if is64 else "32"

# This makes sure to keep the session environment variables on Windows.
# This way, you can run SCons in a Visual Studio 2017 prompt and it will find
# all the required tools
if host_platform == "windows" and env["platform"] != "android":
    if env["bits"] == "64":
        env = Environment(TARGET_ARCH="amd64")
    elif env["bits"] == "32":
        env = Environment(TARGET_ARCH="x86")

    opts.Update(env)

if env["platform"] == "linux":
    if env["use_llvm"]:
        env["CXX"] = "clang++"

    env.Append(CCFLAGS=[
        "-fPIC", "-std=c++17", "-Wwrite-strings", 
        "-fvisibility=hidden",
        "-ffunction-sections", "-fdata-sections"
    ])
    env.Append(LINKFLAGS=["-Wl,--gc-sections,-R,'$$ORIGIN'"])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-Ofast"])

    if env["bits"] == "64":
        env.Append(CCFLAGS=["-m64"])
        env.Append(LINKFLAGS=["-m64"])
    elif env["bits"] == "32":
        env.Append(CCFLAGS=["-m32"])
        env.Append(LINKFLAGS=["-m32"])

    target_path = "libgdflexbox.x86_" + env["bits"] + ".so"

elif env["platform"] == "macos":
    if env["bits"] == "32":
        raise ValueError(
            "Only 64-bit builds are supported for the macOS target."
        )

    try:
        sdk_path = decode_utf8(subprocess.check_output(["xcrun", "--sdk", "macosx", "--show-sdk-path"]).strip())
    except (subprocess.CalledProcessError, OSError):
        raise ValueError("Failed to find SDK path while running xcrun --sdk ? --show-sdk-path.".format())

    compiler_path = env["XCODEPATH"] + "/usr/bin/"
    env["ENV"]["PATH"] = env["XCODEPATH"] + "/Developer/usr/bin/:" + env["ENV"]["PATH"]

    env["CC"] = compiler_path + "clang"
    env["CXX"] = compiler_path + "clang++"
    env["AR"] = compiler_path + "ar"
    env["RANLIB"] = compiler_path + "ranlib"

    env.Append(CCFLAGS=[
        "-std=c++17", "-arch", "x86_64", "-arch", "arm64",
        "-fvisibility=hidden",
        "-ffunction-sections", "-fdata-sections",
        "-mmacosx-version-min=10.14",
        "-isysroot", sdk_path
    ])
    env.Append(LINKFLAGS=[
        "-arch", "x86_64", "-arch", "arm64",
        "-framework", "Cocoa",
        "-Wl,-undefined,dynamic_lookup,-dead_strip",
        "-isysroot", sdk_path
    ])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-Ofast", "-Os"])

    target_path = "libgdflexbox.dylib"

elif env["platform"] == "ios":
    if env["ios_arch"] == "x86_64":
        sdk_name = "iphonesimulator"
        env.Append(CCFLAGS=["-mios-simulator-version-min=11.0"])
    else:
        sdk_name = "iphoneos"
        env.Append(CCFLAGS=["-miphoneos-version-min=11.0"])

    try:
        sdk_path = decode_utf8(subprocess.check_output(["xcrun", "--sdk", sdk_name, "--show-sdk-path"]).strip())
    except (subprocess.CalledProcessError, OSError):
        raise ValueError("Failed to find SDK path while running xcrun --sdk {} --show-sdk-path.".format(sdk_name))

    compiler_path = env["XCODEPATH"] + "/usr/bin/"
    env["ENV"]["PATH"] = env["XCODEPATH"] + "/Developer/usr/bin/:" + env["ENV"]["PATH"]

    env["CC"] = compiler_path + "clang"
    env["CXX"] = compiler_path + "clang++"
    env["AR"] = compiler_path + "ar"
    env["RANLIB"] = compiler_path + "ranlib"

    env.Append(CCFLAGS=[
        "-std=c++17", "-fvisibility=hidden",
        "-ffunction-sections", "-fdata-sections",
        "-arch", env["ios_arch"],
        "-isysroot", sdk_path])
    env.Append(LINKFLAGS=[
        "-arch", env["ios_arch"],
        "-Wl,-undefined,dynamic_lookup,-dead_strip",
        "-isysroot", sdk_path,
        "-F" + sdk_path
    ])

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-Og", "-g"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["-Ofast"])

    target_path = "libgdflexbox." + env["ios_arch"] + ".dylib"

elif env["platform"] == "windows":
    # MSVC
    env.Append(LINKFLAGS=["/WX"])
    if env["target"] == "debug":
        env.Append(CCFLAGS=["/Z7", "/Od", "/std:c++17", "/EHsc", "/D_DEBUG", "/MTd", "/source-charset:utf-8"])
    elif env["target"] == "release":
        env.Append(CCFLAGS=["/O2", "/std:c++17", "/EHsc", "/DNDEBUG", "/MT", "/source-charset:utf-8"])

    target_path = "libgdflexbox.x86_" + env["bits"] + ".dll"
    
elif env["platform"] == "android":
    if host_platform == "windows":
        raise ValueError("Not supported that build by windows host platform.")

    # Verify NDK root
    if not "ANDROID_NDK_ROOT" in env:
        raise ValueError("To build for Android, ANDROID_NDK_ROOT must be defined. Please set ANDROID_NDK_ROOT to the root folder of your Android NDK installation.")

    # Validate API level
    api_level = int(env["android_api_level"])
    if env["android_arch"] in ["x86_64", "arm64v8"] and api_level < 21:
        print("WARN: 64-bit Android architectures require an API level of at least 21; setting android_api_level=21")
        env["android_api_level"] = "21"
        api_level = 21

    # Setup toolchain
    toolchain = env["ANDROID_NDK_ROOT"] + "/toolchains/llvm/prebuilt/"
    if host_platform == "windows":
        toolchain += "windows"
        import platform as pltfm
        if pltfm.machine().endswith("64"):
            toolchain += "-x86_64"
    elif host_platform == "linux":
        toolchain += "linux-x86_64"
    elif host_platform == "macos":
        toolchain += "darwin-x86_64"
    env.PrependENVPath("PATH", toolchain + "/bin") # This does nothing half of the time, but we"ll put it here anyways

    # Get architecture info
    arch_info_table = {
        "armv7" : {
            "outname": "arm32",
            "march":"armv7-a",
            "target":"armv7a-linux-androideabi", 
            "tool_path":"arm-linux-androideabi",
            "compiler_path":"armv7a-linux-androideabi",
            "sysroot_path":"arch-arm",
            "ccflags" : ["-mfpu=neon"]
        },
        "arm64v8" : {
            "outname": "arm64",
            "march":"armv8-a",
            "target":"aarch64-linux-android", 
            "tool_path":"aarch64-linux-android",
            "compiler_path":"aarch64-linux-android",
            "sysroot_path":"arch-arm64",
            "ccflags" : []
        },
        "x86" : {
            "outname": "x86_32",
            "march":"i686",
            "target":"i686-linux-android", 
            "tool_path":"i686-linux-android",
            "compiler_path":"i686-linux-android",
            "sysroot_path":"arch-x86",
            "ccflags" : ["-mstackrealign"]
        },
        "x86_64" : {
            "outname": "x86_64",
            "march":"x86-64",
            "target":"x86_64-linux-android", 
            "tool_path":"x86_64-linux-android",
            "compiler_path":"x86_64-linux-android",
            "sysroot_path":"arch-x86_64",
            "ccflags" : []
        }
    }
    arch_info = arch_info_table[env["android_arch"]]
    target_path = "libgdflexbox." + arch_info["outname"] + ".so"
    
    # Setup tools
    env["CC"] = toolchain + "/bin/clang"
    env["CXX"] = toolchain + "/bin/clang++"
    env["AR"] = toolchain + "/bin/" + arch_info["tool_path"] + "-ar"

    env.Append(CCFLAGS=[
        "--target=" + arch_info["target"] + env["android_api_level"],
        "-march=" + arch_info["march"],
        "-std=c++17",
        "-fPIC", "-fvisibility=hidden",
        "-ffunction-sections", "-fdata-sections",
    ])#, "-fPIE", "-fno-addrsig", "-Oz"])
    env.Append(CCFLAGS=arch_info["ccflags"])
    env.Append(LINKFLAGS=[
        "-Wl,--gc-sections",
        "--target=" + arch_info["target"] + env["android_api_level"],
        "-march=" + arch_info["march"],
    ])

elif env["platform"] == "web":
    if host_platform == "windows":
        raise ValueError("Not supported that build by windows host platform.")

    # Verify NDK root
    if not "EMSDK" in env:
        raise ValueError("To build for Web, EMSDK must be defined. Please set EMSDK to the root folder of your Emscripten SDK installation.")

    target_path = "libgdflexbox.wasm"

    # Setup tools
    env["ENV"] = os.environ
    env["CC"] = "emcc"
    env["CXX"] = "em++"
    env["AR"] = "emar"
    env["RANLIB"] = "emranlib"

    if env["target"] == "debug":
        env.Append(CCFLAGS=["-O0"])
    else:
        env.Append(CCFLAGS=["-O3", "-DNDEBUG"])
    
    env.Append(CCFLAGS=["-std=c++17", "-fno-exceptions", "-fvisibility=hidden"])
    env.Append(LINKFLAGS=["-O3", "--llvm-lto 1"])
    env.Append(CCFLAGS=["-s", "SIDE_MODULE=1"])
    env.Append(LINKFLAGS=["-s", "SIDE_MODULE=1"])
    
    # from platform/web/detect.py
    env.Append(CPPDEFINES=["PTHREAD_NO_RENAME"])
    env.Append(CCFLAGS=["-s", "USE_PTHREADS=1"])
    env.Append(LINKFLAGS=["-s", "USE_PTHREADS=1"])
    env.Append(LINKFLAGS=["-s", "PTHREAD_POOL_SIZE=8"])
    env.Append(LINKFLAGS=["-s", "WASM_MEM_MAX=2048MB"])
    
    env["SHOBJSUFFIX"] = ".bc"
    env["SHLIBSUFFIX"] = ".wasm"
    # Use TempFileMunge since some AR invocations are too long for cmd.exe.
    # Use POSIX-style paths, required with TempFileMunge.
    env["ARCOM_POSIX"] = env["ARCOM"].replace("$TARGET", "$TARGET.posix").replace("$SOURCES", "$SOURCES.posix")
    env["ARCOM"] = "${TEMPFILE(ARCOM_POSIX)}"

    # All intermediate files are just LLVM bitcode.
    env["OBJPREFIX"] = ""
    env["OBJSUFFIX"] = ".bc"
    env["PROGPREFIX"] = ""
    # Program() output consists of multiple files, so specify suffixes manually at builder.
    env["PROGSUFFIX"] = ""
    env["LIBPREFIX"] = "lib"
    env["LIBSUFFIX"] = ".bc"
    env["LIBPREFIXES"] = ["$LIBPREFIX"]
    env["LIBSUFFIXES"] = ["$LIBSUFFIX"]
    env.Replace(SHLINKFLAGS='$LINKFLAGS')
    env.Replace(SHLINKFLAGS='$LINKFLAGS')


if env["platform"] == "windows":
    env.Append(CCFLAGS="/DTYPED_METHOD_BIND")
else:
    env.Append(CCFLAGS="-DTYPED_METHOD_BIND")


# Generate bindings?
json_api_file = ""

# Sources to compile
sources = []

## godot-cpp bindings
add_sources(sources, 'godot-cpp/src', 'cpp')
add_sources(sources, 'godot-cpp/src/classes', 'cpp')
add_sources(sources, 'godot-cpp/src/core', 'cpp')
add_sources(sources, 'godot-cpp/src/variant', 'cpp')
add_sources(sources, 'godot-cpp/gen/src', 'cpp')
add_sources(sources, 'godot-cpp/gen/src/classes', 'cpp')
add_sources(sources, 'godot-cpp/gen/src/variant', 'cpp')

## flexbox
add_sources(sources, "src", "cpp")
add_sources(sources, "src/yoga", "cpp")
add_sources(sources, "src/yoga/event", "cpp")

arch_suffix = env["bits"]
if env["platform"] == "android":
    arch_suffix = env["android_arch"]
if env["platform"] == "ios":
    arch_suffix = env["ios_arch"]

# Local dependency paths, adapt them to your setup
godot_headers_path = "godot-cpp/godot-headers/"
godot_cpp_path = "godot-cpp/"
godot_cpp_lib = "libgodot-cpp"

env.Append(CPPPATH=[".", 
    godot_headers_path, 
    godot_cpp_path + "gdextension/", 
    godot_cpp_path + "include/", 
    godot_cpp_path + "gen/include/"
])

#env.Append(LIBPATH=[
#    godot_cpp_path + "bin/"
#])
#env.Append(LIBS=[
#    "libgodot-cpp.{}.{}.{}{}".format(
#        env["platform"],
#        env["target"],
#        arch_suffix,
#        env["LIBSUFFIX"]
#    )
#])

library = env.SharedLibrary("bin/" + target_path, source=sources)
Default(library)
