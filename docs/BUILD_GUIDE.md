# Build System Configuration Guide

**Date:** October 11, 2025  
**Status:** ✅ Fixed & Working

---

## 🎯 Issue Resolved

**Problem:** MSBuild not found in PATH  
**Error:** `msbuild: The term 'msbuild' is not recognized...`  
**Solution:** Updated `.vscode/tasks.json` to use full MSBuild path

---

## ✅ Current Build Configuration

### VS Code Tasks (`.vscode/tasks.json`)

**Build Task:**
```json
{
  "label": "Build Catastrophe (Development Editor)",
  "type": "shell",
  "windows": {
    "command": "&'C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe'",
    "args": [
      "Catastrophe.sln",
      "/p:Configuration='Development Editor'",
      "/p:Platform=Win64",
      "/m",
      "/v:minimal"
    ]
  }
}
```

**Key Points:**
- Uses full path to MSBuild (doesn't rely on PATH)
- Configuration name quoted with single quotes (PowerShell-safe)
- `/m` flag enables multi-processor compilation
- `/v:minimal` reduces output verbosity

---

## 🚀 How to Build

### Method 1: VS Code Task (Recommended)

```
Ctrl+Shift+B
```

Or:
1. Press `Ctrl+Shift+P`
2. Type "Run Build Task"
3. Select "Build Catastrophe (Development Editor)"

### Method 2: PowerShell Terminal

```powershell
# From repository root
& 'C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe' `
  Catastrophe.sln `
  /p:Configuration='Development Editor' `
  /p:Platform=Win64 `
  /m
```

### Method 3: Developer PowerShell (If MSBuild in PATH)

```powershell
# Setup MSBuild in PATH for current session
.\scripts\setup-msbuild.ps1

# Then build
msbuild Catastrophe.sln /p:Configuration="Development Editor" /p:Platform=Win64 /m
```

---

## 🔧 Visual Studio Installation Paths

The build task assumes VS 2022 Community edition. If you have a different edition, update `.vscode/tasks.json`:

**Community Edition (Default):**
```
C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe
```

**Professional Edition:**
```
C:/Program Files/Microsoft Visual Studio/2022/Professional/MSBuild/Current/Bin/MSBuild.exe
```

**Enterprise Edition:**
```
C:/Program Files/Microsoft Visual Studio/2022/Enterprise/MSBuild/Current/Bin/MSBuild.exe
```

**Build Tools (No IDE):**
```
C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/MSBuild/Current/Bin/MSBuild.exe
```

---

## 📋 Build Configurations

### Development Editor (Default)
- **Use:** Daily development and testing
- **Optimizations:** Moderate
- **Debug Info:** Full
- **Build:** `Ctrl+Shift+B`

### Debug Editor
- **Use:** Debugging crashes or complex issues
- **Optimizations:** None
- **Debug Info:** Maximum
- **Build:** Update task to use `Debug Editor` config

### Shipping
- **Use:** Final release builds
- **Optimizations:** Maximum
- **Debug Info:** None
- **Build:** Requires packaging through Unreal Editor

---

## 🛠️ Troubleshooting

### Error: "MSBuild.exe not found"

**Cause:** Visual Studio not installed or different path

**Solution 1:** Run setup script
```powershell
.\scripts\setup-msbuild.ps1
```

**Solution 2:** Find your MSBuild path
```powershell
# Search for MSBuild
Get-ChildItem "C:\Program Files\Microsoft Visual Studio" -Recurse -Filter "MSBuild.exe" -ErrorAction SilentlyContinue
```

Update `.vscode/tasks.json` with the correct path.

### Error: "Only one project can be specified"

**Cause:** Configuration name has spaces and quotes are wrong

**Current Fix:** Uses single quotes: `/p:Configuration='Development Editor'`

**Don't Use:** Double quotes in PowerShell context (they get parsed twice)

### Error: "Cannot find Catastrophe.sln"

**Cause:** Solution file not generated

**Solution:**
```powershell
# Generate project files first
& "$env:UE5_DIR\Engine\Build\BatchFiles\Build.bat" -projectfiles `
  -project="$PWD\Catastrophe.uproject" -game -rocket -progress
```

Or use VS Code task: "Generate Project Files"

### Slow Build Times

**Solution 1:** Multi-processor compilation (already enabled with `/m`)

**Solution 2:** Exclude folders from Windows Defender
```powershell
# Run as Administrator
Add-MpPreference -ExclusionPath "C:\git\catastrophe"
Add-MpPreference -ExclusionPath "$env:UE5_DIR"
```

**Solution 3:** Use SSD for project and Unreal Engine

---

## 📊 Build Output Verbosity

**Current:** `/v:minimal` (less noise)

**Options:**
- `/v:quiet` - Errors only
- `/v:minimal` - Errors + warnings (current)
- `/v:normal` - Standard output
- `/v:detailed` - Verbose (for debugging build issues)
- `/v:diagnostic` - Maximum verbosity

To change: Update `tasks.json` → `"args"` → `/v:minimal`

---

## 🔄 Clean Build

Sometimes you need a clean build (delete all intermediate files):

**VS Code Task:**
1. `Ctrl+Shift+P` → "Run Task"
2. Select "Clean Catastrophe"
3. Then build normally (`Ctrl+Shift+B`)

**PowerShell:**
```powershell
& 'C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe' `
  Catastrophe.sln /t:Clean

# Then rebuild
& 'C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe' `
  Catastrophe.sln /p:Configuration='Development Editor' /p:Platform=Win64 /m
```

---

## 🎯 Best Practices

### Before Building:
1. ✅ Ensure Visual Studio 2022 installed
2. ✅ Generate project files if `.sln` missing
3. ✅ Close Unreal Editor (file locking issues)

### During Development:
1. ✅ Use `Ctrl+Shift+B` for quick builds
2. ✅ Check terminal output for errors
3. ✅ Restart Unreal Editor after successful C++ build

### If Build Fails:
1. ❌ Don't panic - read the error message
2. ✅ Check for typos in C++ code
3. ✅ Ensure all includes are correct
4. ✅ Try clean build if getting linker errors
5. ✅ Regenerate project files if adding/removing classes

---

## 📝 Helper Scripts

### `scripts/setup-msbuild.ps1`
Automatically finds and adds MSBuild to PATH (current session)

**Usage:**
```powershell
.\scripts\setup-msbuild.ps1
```

### Future: `scripts/clean-build.ps1` (TODO)
Could automate clean + rebuild process

### Future: `scripts/build-shipping.ps1` (TODO)
Could automate shipping builds with packaging

---

## ✅ Verification

**Test the build:**
```powershell
# Should build successfully
Ctrl+Shift+B
```

**Expected output:**
```
MSBuild version 17.14.23+b0019275e for .NET Framework
Build succeeded.
    0 Warning(s)
    0 Error(s)
```

**Then:**
- Open `Catastrophe.uproject`
- Test gameplay (Alt+P)

---

**Updated:** October 11, 2025  
**Build Status:** ✅ Working  
**Next:** Test jump mechanics in Unreal Editor!
