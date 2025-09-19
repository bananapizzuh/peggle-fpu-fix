# Peggle FPU Fix
> [!WARNING]
> This mod has not been tested on Peggle Nights!
This mod was created to try to fix the unintended issue of 3d Acceleration changing the physics in Peggle. Direct3D 9 changes the floating point precision when initialized, in order to cut down computation time. There is a supported compile time flag (`D3DCREATE_FPU_PRESERVE`) to avoid this but PopCap did not use it. The functionality of the smaller precision still exists with this mod, it only fixes reverting back to the original precision from 3d Accelerated mode.

I would like to thank discord user brianshmrian, also known as Brian Rothstein, who worked on Peggle and other games during his time at PopCap for sharing his findings on the issue in The Peggle Project discord while sharing development stories.

## Installation
> [!NOTE]
> This mod requires the [haggle Mod Loader](https://github.com/PeggleCommunity/haggle-mod-loader/releases/latest) as well as the [Haggle SDK](https://github.com/PeggleCommunity/haggle/releases/latest).

Either get the latest release from the [release page](https://github.com/bananapizzuh/peggle-fpu-fix/releases/latest), or build from scratch. Then put the dll in the mods folder of your respective game (usually in `C:\Program Files (x86)\Steam\steamapps\common\`).

## Building
- Clone the github reposity recursively: `git clone --recursive https://github.com/bananapizzuh/peggle-fpu-fix`
  - Alternatively clone then initialize submodules: `git submodule update --init --recursive`
- Configure the build system with premake: `tools\premake5.exe vs2022` 
- Build with either Visual Studio or run `msbuild build\fpu-fix.sln` in a developer command prompt.