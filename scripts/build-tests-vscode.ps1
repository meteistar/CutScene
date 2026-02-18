param(
    [string]$BuildDir = "build/vscode-msvc-debug",
    [string]$QtDir = "C:/Qt/6.7.0/msvc2019_64",
    [switch]$RunTests
)

$ErrorActionPreference = "Stop"

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "vswhere not found: $vswhere"
}

$vsInstall = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsInstall) {
    throw "Visual Studio with C++ tools was not found."
}

$devCmd = Join-Path $vsInstall "Common7\Tools\VsDevCmd.bat"
if (-not (Test-Path $devCmd)) {
    throw "VsDevCmd.bat not found: $devCmd"
}

$configureCmd = "cmake -S . -B `"$BuildDir`" -G `"Ninja`" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DCMAKE_PREFIX_PATH=`"$QtDir`""
$buildCmd = "cmake --build `"$BuildDir`" --target cutscene_tests"
$testCmd = "ctest --test-dir `"$BuildDir`" --output-on-failure"

$qtBinDir = Join-Path $QtDir "bin"
$fullCmd = "`"$devCmd`" -arch=x64 && set `"PATH=$qtBinDir;%PATH%`" && $configureCmd && $buildCmd"
if ($RunTests) {
    $fullCmd += " && $testCmd"
}

cmd.exe /c $fullCmd
exit $LASTEXITCODE
