# Developer Tools Setup Guide

This guide covers the development tools and configurations available in the Catastrophe project
to help you be productive from day one.

## 📦 Included Development Tools

### Editor Configuration

- **`.editorconfig`** - Ensures consistent code formatting across all editors
- **`.clang-format`** - C++ code formatting following Unreal Engine standards
- Automatically formats code on save (when supported by your editor)

### Visual Studio Code Setup

- **`.vscode/extensions.json`** - Recommended VS Code extensions
- **`.vscode/settings.json`** - Project-specific settings
- **`.vscode/tasks.json`** - Build tasks for quick compilation
- **`.vscode/launch.json`** - Debug configurations
- **`Catastrophe.code-workspace`** - VS Code workspace file

### GitHub Integration

- **`.github/workflows/`** - CI/CD pipelines
  - `code-quality.yml` - Automated code quality checks
  - `pr-validation.yml` - Pull request validation
- **`.github/PULL_REQUEST_TEMPLATE.md`** - Standardized PR format
- **`.github/CODEOWNERS`** - Code review assignments

### Code Quality Tools

- **`.markdownlint.json`** - Markdown linting rules
- **`.cspell.json`** - Spell checking configuration
- **`.pre-commit-config.yaml`** - Git pre-commit hooks

## 🚀 Quick Setup

### 1. Install Required Tools

#### All Platforms

```bash
# Install Git (if not already installed)
# Windows: https://git-scm.com/download/win
# Mac: brew install git
# Linux: sudo apt-get install git

# Clone the repository
git clone https://github.com/and3rn3t/catastrophe.git
cd catastrophe
```

#### For VS Code Users

```bash
# Open in VS Code
code Catastrophe.code-workspace

# VS Code will prompt to install recommended extensions
# Click "Install All" when prompted
```

#### Optional: Pre-commit Hooks

```bash
# Install pre-commit (requires Python)
pip install pre-commit

# Install the git hooks
pre-commit install

# Test it out (optional)
pre-commit run --all-files
```

### 2. Editor Setup

#### Visual Studio Code (Recommended for C++)

1. Open `Catastrophe.code-workspace`
2. Install recommended extensions when prompted
3. Reload VS Code
4. Code formatting will now work automatically on save

**Essential Extensions:**

- C/C++ (Microsoft) - IntelliSense and debugging
- C/C++ Extension Pack - Additional C++ tools
- Clang-Format - Code formatting
- EditorConfig - Consistent formatting

**Recommended Extensions:**

- GitLens - Enhanced Git integration
- Markdown All in One - Markdown editing
- TODO Tree - Track TODO comments
- Better Comments - Highlighted comments

#### Visual Studio 2022 (Windows)

1. Open `Catastrophe.sln` (generated after building project files)
2. Install Unreal Engine extensions from Visual Studio Marketplace
3. Enable EditorConfig support (enabled by default in VS 2022)

#### JetBrains Rider

1. Open `Catastrophe.sln`
2. Rider will automatically detect `.editorconfig` and `.clang-format`
3. Configure to use Unreal Engine settings in Preferences → Languages & Frameworks → C++

### 3. Configure Your Environment

#### Set Unreal Engine Path (Required)

Add environment variable for Unreal Engine installation:

**Windows:**

```powershell
setx UE5_DIR "C:\Program Files\Epic Games\UE_5.3"
```

**Mac/Linux:**

```bash
# Add to ~/.bashrc or ~/.zshrc
export UE5_DIR="$HOME/UnrealEngine"
```

## 🔧 Using the Development Tools

### Code Formatting

#### Automatic (Recommended)

- **VS Code:** Code formats automatically on save
- **Visual Studio:** Enable "Format document on save" in options
- **Rider:** Enable "Reformat code on save"

#### Manual Formatting

```bash
# Format a single file
clang-format -i Source/Catastrophe/CatCharacter.cpp

# Format all C++ files
find Source -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

### Building the Project

#### VS Code Tasks

- Press `Ctrl+Shift+B` (Windows/Linux) or `Cmd+Shift+B` (Mac)
- Select "Build Catastrophe (Development Editor)"

#### Command Line

**Windows:**

```bash
msbuild Catastrophe.sln /p:Configuration="Development Editor" /p:Platform=Win64
```

**Linux:**

```bash
make CatastropheEditor
```

### Running Code Quality Checks

#### Manually Run Linters

```bash
# Check C++ formatting (without changing files)
find Source -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror

# Lint Markdown files
npx markdownlint-cli2 "**/*.md"

# Check spelling
npx cspell "**/*.{cpp,h,md}"
```

#### Pre-commit Hooks (if installed)

Pre-commit hooks run automatically before each commit:

```bash
# Skip hooks for a single commit (not recommended)
git commit --no-verify -m "message"

# Run hooks manually on all files
pre-commit run --all-files
```

### Debugging

#### VS Code

1. Press `F5` or use Run → Start Debugging
2. Choose configuration:
   - "Launch Unreal Editor" - Start editor with debugger
   - "Attach to Unreal Editor" - Attach to running editor

#### Visual Studio

1. Set breakpoints in your C++ code
2. Press `F5` or Debug → Start Debugging
3. Unreal Editor will launch with debugger attached

## 📝 GitHub Workflows

### Automated Checks on Pull Requests

Every PR automatically runs:

- **Code Quality Checks** - C++ formatting, markdown linting, spell checking
- **PR Validation** - Checks PR title, branch naming, file sizes

### Branch Naming Convention

Follow these patterns for your branches:

- `feature/description` - New features
- `bugfix/description` - Bug fixes
- `hotfix/description` - Critical fixes
- `docs/description` - Documentation updates
- `refactor/description` - Code refactoring
- `test/description` - Test additions/changes

### Commit Message Convention

Follow conventional commits:

- `feat: add new feature`
- `fix: resolve bug`
- `docs: update documentation`
- `style: code formatting`
- `refactor: restructure code`
- `test: add tests`
- `chore: maintenance tasks`

## 🎯 Best Practices

### Before Committing

1. ✅ Build the project successfully
2. ✅ Test in Unreal Editor (PIE)
3. ✅ Run code formatter (or enable format-on-save)
4. ✅ Check for console errors/warnings
5. ✅ Update documentation if needed

### Code Style

- Follow Unreal Engine C++ Coding Standard
- Use tabs for indentation (4 spaces wide)
- Keep line length under 120 characters
- Use meaningful variable and function names
- Add comments for complex logic

### Pull Requests

1. Fill out the PR template completely
2. Link related issues
3. Add screenshots/videos for visual changes
4. Ensure all automated checks pass
5. Request review from appropriate team members

## 🆘 Troubleshooting

### "clang-format not found"

```bash
# Windows (via Chocolatey)
choco install llvm

# Mac
brew install clang-format

# Linux
sudo apt-get install clang-format
```

### "Pre-commit hooks not running"

```bash
# Reinstall hooks
pre-commit uninstall
pre-commit install

# Update hooks to latest version
pre-commit autoupdate
```

### "VS Code extensions not installing"

1. Open Extensions view (Ctrl+Shift+X)
2. Check for recommended extensions
3. Install manually if needed
4. Reload VS Code

### "Build tasks not working in VS Code"

1. Ensure UE5_DIR environment variable is set
2. Restart VS Code after setting environment variables
3. Check that project files have been generated

## 📚 Additional Resources

### Tools Documentation

- [EditorConfig](https://editorconfig.org/)
- [Clang-Format](https://clang.llvm.org/docs/ClangFormat.html)
- [Pre-commit](https://pre-commit.com/)
- [Markdownlint](https://github.com/DavidAnson/markdownlint)

### Project Documentation

- [README.md](README.md) - Project overview
- [SETUP.md](SETUP.md) - Initial setup guide
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines
- [ROADMAP.md](ROADMAP.md) - Development roadmap

### Unreal Engine Resources

- [UE C++ Coding Standard](https://docs.unrealengine.com/5.3/en-US/epic-cplusplus-coding-standard-for-unreal-engine/)
- [UE5 Documentation](https://docs.unrealengine.com/)
- [UE C++ API Reference](https://docs.unrealengine.com/5.3/en-US/API/)

## ❓ Questions?

If you have questions about the development tools:

1. Check the [CONTRIBUTING.md](CONTRIBUTING.md) guide
2. Review existing [GitHub Discussions](../../discussions)
3. Ask in the project Discord/Slack (if available)
4. Open a new discussion or issue

---

## Happy coding! Let's make CATastrophe awesome! 🐱💻
