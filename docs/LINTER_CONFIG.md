# Linter and Code Quality Configuration

This document explains the linting and code quality tools configured for the CATastrophe project and how to resolve common issues.

---

## 📋 Configured Tools

### 1. **Markdown Linting** (`.markdownlint.json`)

**Purpose:** Ensures consistent markdown formatting across documentation files.

**Disabled Rules:**
- `MD022` - Heading blank lines (too strict for our docs)
- `MD032` - List blank lines (too strict for compact lists)
- `MD026` - Trailing punctuation in headings (we use colons in headings)
- `MD033` - HTML tags allowed (for complex formatting)
- `MD041` - First line heading not required

**Custom Settings:**
- Line length: 120 characters (not enforced in code blocks or tables)
- Duplicate headings allowed if not siblings

### 2. **C++ IntelliSense** (`.vscode/c_cpp_properties.json`)

**Status:** Disabled by default to prevent false positives

**Why Disabled:**
- Unreal Engine uses custom macros (`UPROPERTY`, `UFUNCTION`, etc.) that confuse standard C++ IntelliSense
- Generated `.h` files (`*.generated.h`) don't exist until Unreal Header Tool runs
- The project builds successfully with MSBuild despite IntelliSense errors

**How to Enable (Advanced Users):**
1. Set `UE5_DIR` environment variable to your Unreal Engine installation
2. Generate `compile_commands.json` (requires UnrealBuildTool modifications)
3. Update `.vscode/settings.json`:
   ```json
   "C_Cpp.errorSquiggles": "enabled",
   "C_Cpp.intelliSenseEngine": "default"
   ```

**For Now:** Rely on MSBuild compilation errors (they're accurate). IntelliSense warnings in Unreal C++ files are expected and can be ignored.

### 3. **Spell Checking** (`.cspell.json`)

**Purpose:** Catches typos in code comments and documentation.

**Custom Dictionary Includes:**
- Unreal Engine terms: `UPROPERTY`, `UFUNCTION`, `BlueprintCallable`, etc.
- Project-specific: `CATastrophe`, `CatCharacter`, `HumanNPC`, `Mischief`, etc.
- Game dev terms: `gameplay`, `NavMesh`, `playtest`, etc.

**To Add New Words:**
1. Open `.cspell.json`
2. Add to `words` array (alphabetically sorted recommended)
3. Save and errors disappear

**Ignored Paths:**
- `Binaries/`, `Intermediate/`, `Saved/` - Build artifacts
- `.git/`, `.vscode/` - Version control and editor metadata

### 4. **EditorConfig** (`.editorconfig`)

**Purpose:** Ensures consistent code formatting across editors.

**Key Settings:**
- **C/C++:** Tabs, 4-space width, LF line endings
- **Markdown:** Spaces, 2-space width, trim trailing whitespace
- **JSON/YAML:** Spaces, 2-space indents

### 5. **Clang-Format** (`.clang-format`)

**Purpose:** Automatic C++ code formatting.

**Style:** Based on Google C++ Style Guide with Unreal modifications

**To Format:**
- VS Code: File → Format Document (Shift+Alt+F)
- Command line: `clang-format -i Source/Catastrophe/*.cpp Source/Catastrophe/*.h`

---

## ⚠️ Common "Errors" That Are Actually Fine

### C++ IntelliSense Errors (Red Squiggles)

```cpp
cannot open source file "CoreMinimal.h"
identifier "UPROPERTY" is undefined
identifier "UCLASS" is undefined
```

**Status:** ✅ **Ignore these** - They're false positives

**Why:** VS Code's C++ extension doesn't understand Unreal's build system. The code compiles fine with MSBuild.

**Verification:** Run build task (Ctrl+Shift+B). If it succeeds, your code is correct.

### Markdown Linting "Warnings"

```
MD022/blanks-around-headings
MD032/blanks-around-lists
```

**Status:** ✅ **Now disabled** (as of this configuration update)

**Why:** These rules were too strict for documentation with compact formatting.

### Missing `compile_commands.json`

```
Cannot find: "c:/git/catastrophe/compile_commands.json"
```

**Status:** ✅ **Expected** - This file is optional

**Why:** It's used for advanced IntelliSense but not required for building. Unreal projects don't generate it by default.

---

## 🔧 Setting Up UE5_DIR (Optional)

If you want to reduce IntelliSense warnings (though we've disabled them):

**Windows (PowerShell):**
```powershell
# Set user environment variable
[System.Environment]::SetEnvironmentVariable("UE5_DIR", "C:\Program Files\Epic Games\UE_5.3", "User")

# Restart VS Code after setting
```

**Alternative:** Edit `.vscode/c_cpp_properties.json` and replace `${env:UE5_DIR}` with your actual Unreal Engine path.

---

## 🎯 What Errors to Actually Fix

### Build Errors (From MSBuild)

These are **real** and must be fixed:
- Compilation errors (syntax, missing includes, undefined functions)
- Linker errors (undefined references)
- Module dependency issues

**How to See:** Run build task (Ctrl+Shift+B) and check terminal output.

### Spell Check Errors

Real typos in comments or documentation should be fixed or added to `.cspell.json`.

### Markdown Formatting (Not Auto-Fixed)

Some markdown issues like improper link syntax should be fixed manually.

---

## 📊 Verification Workflow

**Before Committing Code:**

1. ✅ **Build succeeds:**
   ```bash
   Ctrl+Shift+B  # In VS Code
   # Or: msbuild Catastrophe.sln /p:Configuration="Development Editor"
   ```

2. ✅ **No spell check errors** (unless adding to dictionary)

3. ✅ **Markdown renders correctly** (preview with Ctrl+Shift+V)

4. ⚠️ **Ignore C++ IntelliSense red squiggles** (if build passes, they're false positives)

---

## 🔍 Troubleshooting

### "Too Many Errors" in VS Code

**Problem:** Hundreds of red squiggles in C++ files  
**Solution:** Already fixed - C++ error squiggles are disabled in `.vscode/settings.json`

### Markdown Linting Too Strict

**Problem:** Warnings about heading/list spacing  
**Solution:** Already fixed - Rules MD022, MD032, MD026 disabled in `.markdownlint.json`

### Spell Check False Positives

**Problem:** Red underlines on valid Unreal/project terms  
**Solution:** Add words to `.cspell.json` → `words` array

### Format on Save Not Working

**Problem:** Code doesn't auto-format  
**Solution:** 
1. Install recommended extensions (VS Code will prompt)
2. Check `.vscode/settings.json` has `"editor.formatOnSave": true`
3. For C++, install Clang-Format extension

---

## 🎨 Recommended VS Code Extensions

Install these for best experience (defined in `.vscode/extensions.json`):

- **C/C++** (ms-vscode.cpptools) - C++ language support
- **Clang-Format** (xaver.clang-format) - C++ formatting
- **EditorConfig** (editorconfig.editorconfig) - Cross-editor config
- **markdownlint** (davidanson.vscode-markdownlint) - Markdown linting
- **Code Spell Checker** (streetsidesoftware.code-spell-checker) - Spell checking

**Install All:**
VS Code will prompt when opening the workspace, or use:
```bash
Ctrl+Shift+X → Search "Recommended"
```

---

## 📝 Summary

**Key Takeaway:** If `Ctrl+Shift+B` builds successfully, your code is correct. Red squiggles from IntelliSense on Unreal macros are false positives and can be ignored.

**Updated:** October 11, 2025
