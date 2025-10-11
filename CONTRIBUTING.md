# Contributing to CATastrophe

Thank you for your interest in contributing to CATastrophe! This document provides guidelines and information to help you contribute effectively.

## 🗺️ Development Roadmap

Before contributing, please review our [**Product Roadmap**](ROADMAP.md) to understand:
- Current development phase
- Planned features and priorities
- Timeline and milestones
- Where your contribution fits

## 🎯 How to Contribute

### 1. Find a Task

**Current Phase Tasks**: Check the [Roadmap](ROADMAP.md) for tasks in the current development phase.

**Good First Issues**: Look for issues labeled `good-first-issue` in the GitHub Issues tab.

**Bug Fixes**: Help us squash bugs! Check issues labeled `bug`.

**Feature Requests**: Propose new features by opening an issue first to discuss.

### 2. Set Up Your Development Environment

Follow the setup instructions in [README.md](README.md) and [SETUP.md](SETUP.md):

1. Install Unreal Engine 5.3+
2. Install Visual Studio 2022 (Windows) or Xcode (Mac)
3. Clone the repository
4. Generate project files
5. Build the project
6. Open in Unreal Editor

### 3. Create a Feature Branch

```bash
git checkout -b feature/your-feature-name
# or
git checkout -b bugfix/issue-number-description
```

**Branch Naming Conventions:**
- `feature/` - New features
- `bugfix/` - Bug fixes
- `enhancement/` - Improvements to existing features
- `docs/` - Documentation updates
- `refactor/` - Code refactoring

### 4. Make Your Changes

Follow our coding standards and best practices:

#### C++ Code Standards
- Follow [Unreal Engine C++ Coding Standard](https://docs.unrealengine.com/5.3/en-US/epic-cplusplus-coding-standard-for-unreal-engine/)
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and concise
- Use `UE_LOG` for debugging, not `printf` or `cout`

#### Blueprint Standards
- Organize nodes cleanly with Reroute nodes
- Add comments to explain logic
- Use descriptive variable names
- Group related functionality
- Avoid spaghetti code - use functions and macros

#### Asset Guidelines
- Follow naming conventions: `BP_`, `M_`, `T_`, `SM_`, etc.
- Optimize polygon counts (see [DESIGN.md](DESIGN.md))
- Use PBR materials
- Include proper LODs for 3D models
- Keep texture sizes reasonable (1024-4096px)

### 5. Test Your Changes

**Essential Testing:**
- [ ] Build the project successfully
- [ ] Test in Unreal Editor (PIE - Play In Editor)
- [ ] Verify no new console errors or warnings
- [ ] Test your specific feature/fix works as intended
- [ ] Check that existing features still work
- [ ] Test on different graphics settings if relevant

**If Changing Core Systems:**
- [ ] Test cat movement and controls
- [ ] Verify interaction system still works
- [ ] Check AI behavior isn't broken
- [ ] Validate score tracking
- [ ] Test game over scenarios

### 6. Commit Your Changes

Write clear, descriptive commit messages:

```bash
git add .
git commit -m "Add jump mechanic for cat character

- Implement jump input handling
- Add jump animation trigger
- Tune jump height and physics
- Reference Roadmap Phase 1, Milestone 1.1"
```

**Good Commit Message Format:**
```
Short summary (50 chars or less)

More detailed explanation if needed. Wrap at 72 characters.
Explain the problem this solves and why the change is necessary.

- Use bullet points for multiple changes
- Reference roadmap milestones
- Link to issues: Fixes #123
```

### 7. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then create a Pull Request on GitHub with:
- **Clear title** describing the change
- **Description** explaining what and why
- **Roadmap reference** (which phase/milestone)
- **Testing notes** (how you tested it)
- **Screenshots/video** (for visual changes)

**PR Template:**
```markdown
## Description
Brief description of changes

## Roadmap Reference
Phase X, Milestone X.X - [Feature Name]

## Changes Made
- Change 1
- Change 2
- Change 3

## Testing
- [ ] Tested in PIE
- [ ] No console errors
- [ ] Existing features work
- [ ] [Other relevant testing]

## Screenshots/Video
[If applicable]

## Additional Notes
[Any other context]
```

## 🎨 Contribution Areas

### Programming (C++)
**Current Needs:**
- Cat movement and physics improvements
- AI behavior enhancements
- Performance optimization
- Save system implementation

**Skills Needed:**
- C++ programming
- Unreal Engine experience
- Game physics knowledge
- AI programming (for NPC work)

### Blueprint Scripting
**Current Needs:**
- Interactive object blueprints
- UI widgets and HUD
- Game mode logic
- Mission system

**Skills Needed:**
- Unreal Engine Blueprints
- Visual scripting experience
- Game design knowledge

### 3D Art & Modeling
**Current Needs:**
- Cat character model with rig
- Human NPC models
- Environment props
- Destructible objects

**Skills Needed:**
- Blender, Maya, or 3ds Max
- Character modeling and rigging
- PBR texturing
- Optimization for real-time rendering

**Asset Guidelines:**
- See [DESIGN.md](DESIGN.md) for poly counts
- Export as FBX for Unreal Engine
- Include proper materials and textures
- Provide multiple LODs if applicable

### Animation
**Current Needs:**
- Cat animations (idle, walk, run, jump, interact)
- Human NPC animations (walk, idle, react)
- Interaction animations
- UI animations

**Skills Needed:**
- Character animation
- Motion capture (optional)
- Understanding of animal movement
- Unreal Engine animation system

### Audio & Music
**Current Needs:**
- Sound effects (footsteps, object collisions, cat meows)
- Background music tracks
- Ambient house sounds
- UI sounds

**Skills Needed:**
- Sound design
- Music composition
- Audio editing software
- Unreal Engine audio system

**Audio Specifications:**
- Format: WAV (44.1kHz or 48kHz, 16-bit or 24-bit)
- Sound effects: Mono or stereo
- Music: Stereo, loopable
- File size: Keep reasonable for performance

### Level Design
**Current Needs:**
- Room layouts and flow
- Object placement
- Lighting design
- Gameplay pacing

**Skills Needed:**
- Spatial design
- Unreal Engine level editor
- Understanding of game flow
- Environmental storytelling

### Documentation
**Current Needs:**
- Tutorial content
- API documentation
- Setup guides
- Localization

**Skills Needed:**
- Technical writing
- Markdown formatting
- Clear communication

### Testing & QA
**Current Needs:**
- Bug reports
- Playtesting feedback
- Performance testing
- Usability testing

**How to Help:**
- Play the game and report bugs
- Test on different hardware
- Provide gameplay feedback
- Suggest improvements

## 📝 Code Review Process

All contributions go through code review:

1. **Automated Checks**: Ensure CI passes (if configured)
2. **Code Review**: Maintainers review code quality
3. **Testing Verification**: Confirm testing was thorough
4. **Roadmap Alignment**: Verify alignment with project goals
5. **Approval**: At least one maintainer approval required
6. **Merge**: Once approved, we'll merge your PR!

**Review Criteria:**
- Code quality and standards
- Functionality and correctness
- Performance impact
- Documentation updates
- Test coverage
- Alignment with roadmap

## 🐛 Reporting Bugs

Help us improve by reporting bugs!

**Before Reporting:**
1. Check if it's already reported in Issues
2. Try to reproduce the bug
3. Test on latest version if possible

**Bug Report Template:**
```markdown
## Bug Description
Clear description of the bug

## Steps to Reproduce
1. Step 1
2. Step 2
3. Step 3

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## Environment
- OS: [Windows 10/11, macOS, Linux]
- Unreal Engine Version: [5.3.x]
- Hardware: [GPU, CPU, RAM]
- Graphics Settings: [Low/Medium/High/Epic]

## Screenshots/Logs
[If applicable]

## Additional Context
[Any other relevant information]
```

## 💡 Feature Requests

Have an idea? We'd love to hear it!

**Before Requesting:**
1. Check the [Roadmap](ROADMAP.md) - it might be planned!
2. Search existing issues to avoid duplicates
3. Consider if it fits the game's vision

**Feature Request Template:**
```markdown
## Feature Description
Clear description of the feature

## Problem It Solves
What problem does this address?

## Proposed Solution
How would this work?

## Alternatives Considered
Other ways to solve this

## Roadmap Alignment
Which phase/milestone would this fit?

## Additional Context
[Mockups, examples, references]
```

## 🤝 Community Guidelines

### Code of Conduct
- Be respectful and inclusive
- Give constructive feedback
- Help others learn and grow
- Focus on the project goals
- No harassment or discrimination

### Communication Channels
- **GitHub Issues**: Bug reports, feature requests
- **GitHub Discussions**: General questions, ideas
- **Pull Requests**: Code contributions
- **Discord** (if available): Real-time chat

### Getting Help
- Check [README.md](README.md) and [SETUP.md](SETUP.md) first
- Search existing issues and discussions
- Ask clear, specific questions
- Provide context and details
- Be patient - maintainers are volunteers

## 🎯 Recognition

Contributors will be:
- Listed in project credits
- Mentioned in release notes
- Recognized in the community
- Building portfolio-worthy work!

## 📚 Resources

### Unreal Engine
- [Unreal Engine Documentation](https://docs.unrealengine.com/)
- [Unreal Engine C++ API](https://docs.unrealengine.com/5.3/en-US/API/)
- [Blueprint Visual Scripting](https://docs.unrealengine.com/5.3/en-US/blueprints-visual-scripting-in-unreal-engine/)

### Game Development
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [Gamasutra Articles](https://www.gamasutra.com/)

### Project Specific
- [README.md](README.md) - Project overview
- [ROADMAP.md](ROADMAP.md) - Development plan
- [DESIGN.md](DESIGN.md) - Visual design guide
- [SETUP.md](SETUP.md) - Quick setup guide

## 🎮 Philosophy

CATastrophe is built with these principles:
- **Fun First**: Gameplay enjoyment is priority #1
- **Polish**: Quality over quantity
- **Accessibility**: Everyone should be able to play
- **Community**: Built together with contributors
- **Learning**: Great project for skill development

## ✨ Thank You!

Every contribution, no matter how small, helps make CATastrophe better. Whether you're fixing a typo, reporting a bug, or building a major feature - **thank you for being part of this journey!**

Let's create something amazing together! 😼💥

---

*For questions about contributing, open a Discussion on GitHub.*
