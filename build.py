import sys
import os
import shutil
import subprocess
import multiprocessing

script_path = os.path.abspath(__file__)

def replace_word(file_name, target_str, replace_str):
    text = ""
    with open(file_name, "r") as file:
        text = file.read()

    text = text.replace(target_str, replace_str)

    with open(file_name, "w") as file:
        file.write(text)

def import_generate_bindings():
    binding_generator = __import__("godot-cpp.binding_generator").binding_generator
    cwd = os.getcwd()
    os.chdir(os.path.join(os.path.dirname(script_path), "godot-cpp"))
    binding_generator.generate_bindings("gdextension/extension_api.json", False)
    os.chdir(cwd)

import_generate_bindings()

os.chdir(os.path.dirname(script_path))

job_opt = " -j" + str(multiprocessing.cpu_count())

if "platform=windows" in sys.argv:
    replace_word("godot-cpp/Sconstruct", "/MD", "/MT")

    subprocess.run("scons platform=windows bits=32 target=template_release" + job_opt, shell = True)
    subprocess.run("scons platform=windows bits=64 target=template_release" + job_opt, shell = True)

    os.makedirs("addons/godot-flexbox/bin/windows", exist_ok = True)

    shutil.copy2("bin/libgdflexbox.x86_32.dll", "addons/godot-flexbox/bin/windows/")
    shutil.copy2("bin/libgdflexbox.x86_64.dll", "addons/godot-flexbox/bin/windows/")

elif "platform=macos" in sys.argv:
    subprocess.run("scons platform=macos bits=64 target=template_release" + job_opt, shell = True)

    os.makedirs("addons/godot-flexbox/bin/macos", exist_ok = True)

    shutil.copy2("bin/libgdflexbox.dylib", "addons/godot-flexbox/bin/macos/")

elif "platform=android" in sys.argv:
    subprocess.run("scons platform=android android_arch=armv7 target=template_release" + job_opt, shell = True)
    subprocess.run("scons platform=android android_arch=arm64v8 target=template_release" + job_opt, shell = True)
    subprocess.run("scons platform=android android_arch=x86 target=template_release" + job_opt, shell = True)
    subprocess.run("scons platform=android android_arch=x86_64 target=template_release" + job_opt, shell = True)

    os.makedirs("addons/godot-flexbox/bin/android", exist_ok = True)

    shutil.copy2("bin/libgdflexbox.arm32.so", "addons/godot-flexbox/bin/android/")
    shutil.copy2("bin/libgdflexbox.arm64.so", "addons/godot-flexbox/bin/android/")
    shutil.copy2("bin/libgdflexbox.x86_32.so", "addons/godot-flexbox/bin/android/")
    shutil.copy2("bin/libgdflexbox.x86_64.so", "addons/godot-flexbox/bin/android/")

elif "platform=ios" in sys.argv:
    subprocess.run("scons platform=ios ios_arch=arm64 target=template_release" + job_opt, shell = True)
    # subprocess.run("scons platform=ios ios_arch=x86_64 target=template_release" + job_opt, shell = True)

    subprocess.run("lipo -create bin/libgdflexbox.arm64.dylib -output bin/libgdflexbox.dylib", shell = True)
    # subprocess.run("lipo -create bin/libgdflexbox.arm64.dylib bin/libgdflexbox.x86_64.dylib -output bin/libgdflexbox.dylib", shell = True)

    os.makedirs("addons/godot-flexbox/bin/ios", exist_ok = True)
    
    shutil.copy2("bin/libgdflexbox.dylib", "addons/godot-flexbox/bin/ios/")

elif "platform=linux" in sys.argv:
    subprocess.run("scons platform=linux bits=32 target=template_release use_llvm=1" + job_opt, shell = True)
    subprocess.run("scons platform=linux bits=64 target=template_release use_llvm=1" + job_opt, shell = True)

    os.makedirs("addons/godot-flexbox/bin/linux", exist_ok = True)

    shutil.copy2("bin/libgdflexbox.x86_32.so", "addons/godot-flexbox/bin/linux/")
    shutil.copy2("bin/libgdflexbox.x86_64.so", "addons/godot-flexbox/bin/linux/")

elif "platform=web" in sys.argv:
    subprocess.run("scons platform=javascript bits=32 target=template_release" + job_opt, shell = True)

    os.makedirs("addons/godot-flexbox/bin/web", exist_ok = True)

    shutil.copy2("bin/libgdflexbox.wasm", "addons/godot-flexbox/bin/web/")
