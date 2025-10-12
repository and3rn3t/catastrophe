# 🎨 Quick HUD Widget Reference Card

## Widget Names (MUST BE EXACT!)

```
┌─────────────────────────────────────────────┐
│ Widget Type      │ EXACT Name              │
├──────────────────┼─────────────────────────┤
│ Text Block       │ ScoreText               │
│ Progress Bar     │ StaminaBar              │
│ Image            │ StealthIcon             │
│ Text Block       │ StealthStatusText       │
│ Text Block       │ StaminaLabel (optional) │
└─────────────────────────────────────────────┘
```

## Layout Visual Guide

```
┌─────────────────────────────────────────────────┐
│  🟢 StealthIcon  HIDDEN                         │ ← Top-Left
│                                                 │
│                                                 │
│                                    Score: 0  📊 │ ← Top-Right
│                                                 │
│                                                 │
│                                                 │
│                                                 │
│                    Stamina                      │ ← Bottom-Center
│             ██████████████████████              │
└─────────────────────────────────────────────────┘
```

## Color States (Automatic!)

### Stamina Bar:
```
> 60%  → 🟢 Green
30-60% → 🟡 Yellow
< 30%  → 🔴 Red
```

### Stealth Indicator:
```
HIDDEN     → 🟢 Green  (standing, not detected)
SNEAKING   → 🔵 Blue   (crouching)
DETECTED!  → 🔴 Red    (NPC sees you)
```

## Anchor Presets Quick Guide

```
Top-Left:      Top-Center:    Top-Right:
  ┌─┐            ┌─┐             ┌─┐
  └─┘            └─┘             └─┘
  
Center-Left:   Center:        Center-Right:
  ┌─┐            ┌─┐             ┌─┐
  └─┘            └─┘             └─┘
  
Bottom-Left:   Bottom-Center: Bottom-Right:
  ┌─┐            ┌─┐             ┌─┐
  └─┘            └─┘             └─┘
```

## Properties Quick Reference

### ScoreText:
- Anchors: Top-Right
- Position: X=-200, Y=20
- Font Size: 32
- Color: Orange

### StaminaBar:
- Anchors: Bottom-Center
- Position: X=-150, Y=-80
- Size: 300x25
- Fill: Green

### StealthIcon:
- Anchors: Top-Left
- Position: X=20, Y=20
- Size: 50x50
- Color: Green

### StealthStatusText:
- Anchors: Top-Left
- Position: X=80, Y=30
- Font Size: 24
- Color: Green

## ✅ Must-Do Checklist

Every widget MUST have:
- [ ] EXACT name (case-sensitive)
- [ ] "Is Variable" checkbox ☑
- [ ] Correct anchors set
- [ ] Position values set

## 🎯 Remember

**The C++ code looks for widgets by name!**

If you get errors like:
```
"HUD: ScoreText widget not found!"
```

→ Widget name is wrong or "Is Variable" not checked

