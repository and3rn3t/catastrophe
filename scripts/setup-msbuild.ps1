# Find and Add MSBuild to PATH
# This script locates Visual Studio 2022's MSBuild and adds it to the current session PATH

Write-Host "🔍 Locating Visual Studio 2022 MSBuild..." -ForegroundColor Cyan

# Common Visual Studio 2022 installation paths
$vsPaths = @(
    "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin",
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin",
    "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin"
)

$msbuildPath = $null

foreach ($path in $vsPaths) {
    if (Test-Path (Join-Path $path "MSBuild.exe")) {
        $msbuildPath = $path
        Write-Host "✅ Found MSBuild at: $msbuildPath" -ForegroundColor Green
        break
    }
}

if ($null -eq $msbuildPath) {
    Write-Host "❌ MSBuild not found in standard locations!" -ForegroundColor Red
    Write-Host "Please install Visual Studio 2022 or modify the `$vsPaths array in this script." -ForegroundColor Yellow
    exit 1
}

# Add to PATH for current session
if ($env:PATH -notlike "*$msbuildPath*") {
    $env:PATH = "$msbuildPath;$env:PATH"
    Write-Host "✅ Added MSBuild to PATH (current session only)" -ForegroundColor Green
} else {
    Write-Host "✅ MSBuild already in PATH" -ForegroundColor Green
}

# Verify
$msbuildVersion = & msbuild -version 2>&1 | Select-String "Microsoft.*Build.*Engine"
if ($msbuildVersion) {
    Write-Host "✅ MSBuild is ready: $msbuildVersion" -ForegroundColor Green
} else {
    Write-Host "⚠️ MSBuild added to PATH but version check failed" -ForegroundColor Yellow
}

Write-Host "`n📝 To add MSBuild to PATH permanently, run:" -ForegroundColor Cyan
Write-Host "   [System.Environment]::SetEnvironmentVariable('PATH', `"`$env:PATH;$msbuildPath`", 'User')" -ForegroundColor White
Write-Host "   Then restart VS Code." -ForegroundColor White
