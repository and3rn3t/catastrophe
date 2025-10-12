# CATastrophe Testing Guide

## Overview
This guide explains how to set up and run automated tests for the CATastrophe project. Testing helps ensure code quality, catch bugs early, and make refactoring safer.

---

## 🧪 Types of Tests in This Project

### 1. **Unit Tests**
- Test individual functions/classes in isolation
- Fast to run
- Example: Testing stamina consumption, score tracking

### 2. **Integration Tests**
- Test how systems work together
- Example: Cat makes noise → NPC hears it → NPC investigates

### 3. **Performance Tests**
- Test that systems scale properly
- Example: Multiple NPCs ticking simultaneously

---

## 🚀 Getting Started with Testing

### **Step 1: Enable Testing in Unreal Editor**

1. Open Unreal Editor with your project
2. Go to **Window → Developer Tools → Session Frontend**
3. Click on the **Automation** tab
4. You should see your tests listed under "Catastrophe"

### **Step 2: Running Tests from Unreal Editor**

#### **Run All Tests:**
1. Open **Session Frontend** (Window → Developer Tools → Session Frontend)
2. Click **Automation** tab
3. Check the box next to "Catastrophe" to select all tests
4. Click **Start Tests** button at the bottom

#### **Run Specific Tests:**
1. Expand the "Catastrophe" tree
2. Check individual tests you want to run:
   - `Catastrophe.Unit.CatCharacter.*` - Cat character tests
   - `Catastrophe.Integration.NPC.*` - NPC behavior tests
   - `Catastrophe.Performance.*` - Performance tests
3. Click **Start Tests**

#### **Reading Results:**
- ✅ **Green** = Test passed
- ❌ **Red** = Test failed (click to see error details)
- ⏱️ **Time** = How long test took to run

### **Step 3: Running Tests from Command Line**

For automated testing (CI/CD), you can run tests via command line:

```bash
# Run all tests
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" ^
  "C:\git\catastrophe\Catastrophe.uproject" ^
  -ExecCmds="Automation RunTests Catastrophe" ^
  -unattended ^
  -nopause ^
  -NullRHI ^
  -ReportOutputPath="C:\git\catastrophe\TestResults"

# Run only unit tests
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" ^
  "C:\git\catastrophe\Catastrophe.uproject" ^
  -ExecCmds="Automation RunTests Catastrophe.Unit" ^
  -unattended ^
  -nopause ^
  -NullRHI ^
  -ReportOutputPath="C:\git\catastrophe\TestResults"
```

**Command Explanation:**
- `UnrealEditor-Cmd.exe` - Command-line version of Unreal Editor
- `-ExecCmds="Automation RunTests Catastrophe"` - Run tests matching "Catastrophe"
- `-unattended` - Don't require user input
- `-NullRHI` - Don't use graphics (faster for automated tests)
- `-ReportOutputPath` - Where to save test reports

---

## 📋 Current Test Coverage

### **Unit Tests - Cat Character**

| Test Name | What It Tests | Location |
|-----------|---------------|----------|
| `FCatCharacterStaminaTest` | Stamina consumption, regeneration, min/max limits | CatCharacterTests.cpp:19 |
| `FCatCharacterCrouchTest` | Crouch activation, stealth detection multiplier | CatCharacterTests.cpp:60 |
| `FCatCharacterMischiefScoreTest` | Score tracking and accumulation | CatCharacterTests.cpp:102 |
| `FCatCharacterPounceTest` | Pounce ability and cooldown system | CatCharacterTests.cpp:134 |

### **Integration Tests - NPC & Cat Interactions**

| Test Name | What It Tests | Location |
|-----------|---------------|----------|
| `FNPCDetectionTest` | NPC vision cone detection | CatCharacterTests.cpp:172 |
| `FNPCHearingTest` | NPC hearing system with distance/loudness | CatCharacterTests.cpp:220 |
| `FNPCAlertSystemTest` | Alert level transitions (Relaxed→Suspicious→Alerted) | CatCharacterTests.cpp:261 |
| `FNPCStateTransitionTest` | AI state machine transitions | CatCharacterTests.cpp:294 |

### **Performance Tests**

| Test Name | What It Tests | Location |
|-----------|---------------|----------|
| `FNPCPerformanceTest` | Multiple NPCs updating simultaneously | CatCharacterTests.cpp:331 |

---

## ✍️ Writing New Tests

### **Test Template:**

```cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FYourTestName, 
	"Catastrophe.Category.SubCategory.TestName", 
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FYourTestName::RunTest(const FString& Parameters)
{
	// 1. Setup
	UWorld* TestWorld = FAutomationEditorCommonUtils::CreateNewMap();
	if (!TestWorld)
	{
		AddError(TEXT("Failed to create test world"));
		return false;
	}

	// 2. Create test actors
	ACatCharacter* TestCat = TestWorld->SpawnActor<ACatCharacter>();
	
	// 3. Perform test actions
	TestCat->AddMischiefScore(10.0f);
	
	// 4. Assert expected results
	TestEqual(TEXT("Score should be 10"), TestCat->GetMischiefScore(), 10.0f);
	
	// 5. Cleanup
	TestWorld->DestroyActor(TestCat);
	
	return true;
}
```

### **Test Naming Convention:**

```
"Catastrophe.[Category].[System].[Feature]"

Examples:
- "Catastrophe.Unit.CatCharacter.Stamina"
- "Catastrophe.Integration.NPC.Detection"
- "Catastrophe.Performance.AI.MultipleNPCs"
```

### **Common Test Assertions:**

```cpp
// Equality checks
TestEqual(TEXT("Description"), ActualValue, ExpectedValue);

// Boolean checks
TestTrue(TEXT("Description"), Condition);
TestFalse(TEXT("Description"), Condition);

// Null checks
TestNull(TEXT("Description"), Pointer);
TestNotNull(TEXT("Description"), Pointer);

// Approximate equality (for floats)
TestTrue(TEXT("Description"), FMath::IsNearlyEqual(Value1, Value2, Tolerance));

// Add information
AddInfo(TEXT("Informational message"));

// Add warning
AddWarning(TEXT("Warning message"));

// Add error (marks test as failed)
AddError(TEXT("Error message"));
```

---

## 🔧 Setting Up Continuous Integration (CI)

### **GitHub Actions Example:**

Create `.github/workflows/unreal-tests.yml`:

```yaml
name: Run Unreal Engine Tests

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  test:
    runs-on: windows-latest
    
    steps:
    - name: Checkout code
      uses: actions/checkout@v3
    
    - name: Run Unreal Engine Tests
      run: |
        & "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" `
          "${{ github.workspace }}\Catastrophe.uproject" `
          -ExecCmds="Automation RunTests Catastrophe" `
          -unattended `
          -nopause `
          -NullRHI `
          -ReportOutputPath="${{ github.workspace }}\TestResults"
    
    - name: Upload Test Results
      uses: actions/upload-artifact@v3
      if: always()
      with:
        name: test-results
        path: TestResults/
```

---

## 📊 Test Reports

### **Reading Test Reports:**

After running tests via command line, check the report at:
```
C:\git\catastrophe\TestResults\index.html
```

Open in browser to see:
- ✅ Tests passed
- ❌ Tests failed with details
- ⏱️ Execution time per test
- 📈 Overall pass rate

---

## 🎯 Best Practices

### **DO:**
- ✅ Write tests for new features before implementing them (TDD)
- ✅ Keep tests fast (< 1 second per unit test)
- ✅ Test edge cases (zero values, max values, negative values)
- ✅ Clean up after tests (destroy spawned actors)
- ✅ Use descriptive test names
- ✅ Run tests before committing code

### **DON'T:**
- ❌ Test Unreal Engine's built-in functionality
- ❌ Create tests that depend on specific levels/assets (unless necessary)
- ❌ Write tests that depend on execution order
- ❌ Leave failing tests in the codebase
- ❌ Skip cleanup (causes memory leaks)

---

## 🐛 Debugging Failed Tests

### **When a Test Fails:**

1. **Read the Error Message:**
   ```
   Error: Expected 10.0 but got 5.0
   Test: Catastrophe.Unit.CatCharacter.Stamina
   ```

2. **Check Recent Changes:**
   - Did you modify the tested code?
   - Did you change default values?

3. **Run Test in Debug Mode:**
   - Set breakpoint in test code
   - Run test with debugger attached
   - Step through to find issue

4. **Add Logging:**
   ```cpp
   UE_LOG(LogTemp, Warning, TEXT("Current stamina: %f"), TestCat->GetCurrentStamina());
   ```

5. **Isolate the Problem:**
   - Comment out test sections
   - Run smaller test pieces
   - Verify assumptions

---

## 📝 Test Checklist for New Features

When adding a new feature, create tests for:

- [ ] **Happy Path** - Feature works as expected
- [ ] **Edge Cases** - Boundary conditions (0, max, negative)
- [ ] **Error Handling** - Invalid inputs
- [ ] **State Changes** - Proper transitions
- [ ] **Cleanup** - No memory leaks
- [ ] **Performance** - Acceptable execution time
- [ ] **Integration** - Works with other systems

---

## 🔗 Additional Resources

### **Unreal Engine Documentation:**
- [Automation System](https://docs.unrealengine.com/5.3/en-US/automation-system-overview/)
- [Automation Technical Guide](https://docs.unrealengine.com/5.3/en-US/automation-technical-guide/)
- [Testing with Unreal](https://docs.unrealengine.com/5.3/en-US/testing-and-optimizing/)

### **Learning Resources:**
- [Unit Testing in C++](https://www.learncpp.com/)
- [Test-Driven Development](https://martinfowler.com/bliki/TestDrivenDevelopment.html)
- [Unreal Engine Automation on YouTube](https://www.youtube.com/results?search_query=unreal+engine+automation+testing)

---

## 💡 Quick Reference

### **Run Tests (Editor):**
1. Window → Developer Tools → Session Frontend
2. Automation Tab
3. Check "Catastrophe"
4. Start Tests

### **Run Tests (Command Line):**
```bash
UnrealEditor-Cmd.exe YourProject.uproject -ExecCmds="Automation RunTests Catastrophe" -unattended -NullRHI
```

### **Add New Test:**
1. Create/edit file in `Source/Catastrophe/Tests/`
2. Use `IMPLEMENT_SIMPLE_AUTOMATION_TEST` macro
3. Implement `RunTest()` function
4. Rebuild project
5. Run tests from Session Frontend

---

## 🎉 Summary

You now have:
- ✅ **9 automated tests** covering core gameplay systems
- ✅ **Unit tests** for cat abilities
- ✅ **Integration tests** for NPC interactions
- ✅ **Performance tests** for optimization
- ✅ **Documentation** for running and writing tests
- ✅ **CI/CD ready** for automated testing

**Next Steps:**
1. Run the existing tests to verify everything works
2. Add tests when implementing new features
3. Set up CI/CD to run tests automatically
4. Aim for 80%+ code coverage over time

Happy Testing! 🧪🐱
