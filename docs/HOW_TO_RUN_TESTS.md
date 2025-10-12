# How to Run and Evaluate Tests - Quick Guide

## 🚀 Three Ways to Run Tests

### Method 1: Batch Script (Easiest)
1. Double-click `RunTests.bat` in project root
2. Choose option:
   - **1** = All tests (~20 sec)
   - **2** = Unit tests only (~5 sec)
   - **3** = Integration tests (~10 sec)
   - **4** = Performance tests (~1 sec)
   - **5** = Quick (Unit + Integration, skip performance)
3. Wait for completion
4. Results open automatically in browser

### Method 2: Unreal Editor (Most Visual)
1. Open Catastrophe.uproject in Unreal Editor
2. **Window → Test Automation** (or **Window → Developer Tools → Session Frontend → Automation**)
3. You'll see test tree:
   ```
   ☐ Catastrophe
      ☐ Unit
         ☐ CatCharacter
            ☐ Stamina
            ☐ StaminaEdgeCases
            ☐ CrouchDetection
            ☐ PounceSystem
            ☐ MischiefScore
      ☐ Integration
         ☐ NPC
            ☐ AlertSystem
            ☐ States
            ☐ VisionDetection
            ☐ HearingSystem
         ☐ Cat
            ☐ NoiseEmission
      ☐ Scenario
         ☐ StealthGameplay
         ☐ SuspicionBuildup
      ☐ Performance
         ☐ MultiNPC
   ```
4. Check boxes for tests you want
5. Click **"Start Tests"** at bottom
6. Watch results in real-time

### Method 3: Command Line (For CI/CD)
```bash
cd C:\git\catastrophe

# Run all tests
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" ^
  "C:\git\catastrophe\Catastrophe.uproject" ^
  -ExecCmds="Automation RunTests Catastrophe; Quit" ^
  -unattended -nopause

# Run specific category
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" ^
  "C:\git\catastrophe\Catastrophe.uproject" ^
  -ExecCmds="Automation RunTests Catastrophe.Unit; Quit" ^
  -unattended -nopause
```

---

## 📊 Reading Test Results

### In Unreal Editor

**Green Checkmark (✓)** = Test Passed
```
✓ Catastrophe.Unit.CatCharacter.Stamina
  - Cat starts with full stamina: PASSED
  - Stamina decreases correctly: PASSED
  - Stamina can't go below zero: PASSED
```

**Red X (✗)** = Test Failed
```
✗ Catastrophe.Unit.CatCharacter.Stamina
  - Expected 100 but got 50: FAILED
```

**Orange Warning (⚠)** = Test Warning
```
⚠ Catastrophe.Unit.CatCharacter.Stamina
  - Warning: Stamina regeneration slower than expected
```

### In HTML Report

Open `TestResults/index.html` to see:
- **Summary**: Total passed/failed
- **Details**: Each test with timing
- **Graphs**: Performance over time
- **Logs**: Full output for debugging

---

## 🔍 Evaluating Test Quality

### What to Look For

#### 1. **All Tests Should Pass**
```
Target: 13/13 passed (100%)
Current: [Check your results]
```

**If tests fail:**
- Read error message carefully
- Check what changed recently
- Run test individually to debug
- Look at assertion that failed

#### 2. **Tests Should Be Fast**
```
Target Times:
- Unit tests: < 1 second each
- Integration tests: < 2 seconds each
- Scenario tests: < 3 seconds each
- Performance tests: < 2 seconds
- Total suite: < 30 seconds
```

**If tests are slow:**
- Check for infinite loops
- Look for expensive operations
- Profile with performance test
- Consider mocking expensive calls

#### 3. **Coverage Should Be Meaningful**
```
Current Coverage: 60%
Target Coverage: 75%

Check that important code is tested:
✅ Core gameplay systems
✅ Critical paths (stealth, detection)
✅ Edge cases (zero stamina, max suspicion)
✅ Integration points (cat → NPC)
```

#### 4. **Tests Should Be Reliable**
Run tests 3 times in a row:
```
Run 1: 13/13 passed ✓
Run 2: 13/13 passed ✓
Run 3: 13/13 passed ✓

If results differ = Flaky test (needs fixing)
```

---

## 📈 Test Metrics to Track

### Essential Metrics

| Metric | How to Check | Target | Priority |
|--------|--------------|--------|----------|
| **Pass Rate** | Count passed/total | 100% | CRITICAL |
| **Execution Time** | Check test duration | < 30 sec | HIGH |
| **Coverage** | See TEST_COVERAGE_REPORT.md | 75% | HIGH |
| **Flakiness** | Run multiple times | 0% | HIGH |

### How to Check Each Metric:

#### 1. Pass Rate
```
In Editor: Look at summary bar
  "Tests: 13  Passed: 13  Failed: 0"

In HTML Report: Check header
  "Pass Rate: 100% (13/13)"
```

#### 2. Execution Time
```
In Editor: Check "Duration" column
  Catastrophe.Unit.CatCharacter.Stamina: 0.245s

In HTML Report: See timing graphs
  Average: 1.5s per test
  Total: 19.5s
```

#### 3. Coverage
```
Check TEST_COVERAGE_REPORT.md:
  Cat Character: 50%
  NPC AI: 75%
  Overall: 60%

Goal: All critical systems > 80%
```

#### 4. Flakiness
```
Run 5 times:
  Attempt 1: 13/13 ✓
  Attempt 2: 12/13 ✗ (StaminaTest failed)
  Attempt 3: 13/13 ✓
  Attempt 4: 13/13 ✓
  Attempt 5: 13/13 ✓

This test is 80% reliable = Flaky, needs fixing
```

---

## ⚠️ Common Test Failures and Fixes

### Failure 1: "Failed to get test world"
**Cause:** Tests need a world to spawn actors

**Fix:** 
- Run tests in PIE mode (hit Play first, then run tests)
- Or use command line (creates world automatically)

### Failure 2: "Failed to spawn cat character"
**Cause:** World exists but can't create actor

**Fix:**
- Check CatCharacter class is valid
- Verify no compile errors
- Try rebuilding project

### Failure 3: "Expected 100 but got 0"
**Cause:** Assertion failed - actual behavior doesn't match expected

**Fix:**
- Check if you changed default values
- Read test code to understand expectation
- Debug the tested function

### Failure 4: Test timeout
**Cause:** Test taking too long (> 60 seconds)

**Fix:**
- Check for infinite loops
- Reduce number of iterations
- Break into smaller tests

### Failure 5: Random failures (flaky tests)
**Cause:** Test depends on timing, random values, or external state

**Fix:**
- Remove random elements
- Use fixed values for testing
- Ensure proper cleanup
- Don't depend on execution order

---

## 🎯 Test Evaluation Checklist

Before committing code, verify:

- [ ] **All tests pass** (100% pass rate)
- [ ] **No new failures** (didn't break existing tests)
- [ ] **Tests run quickly** (< 30 seconds total)
- [ ] **New code is tested** (added tests for new features)
- [ ] **Coverage improved** (or stayed same)
- [ ] **No flaky tests** (ran 3x, all passed)
- [ ] **Meaningful assertions** (tests actually verify behavior)
- [ ] **Good test names** (describes what's being tested)

---

## 📋 Weekly Test Health Check

### Monday Morning Routine:
1. **Run full test suite**
   ```
   RunTests.bat → Option 1
   ```

2. **Check results:**
   - All green? ✅ Good to go
   - Any red? ⚠️ Fix before continuing
   - Any slow? ⚠️ Investigate performance

3. **Review coverage:**
   - Open TEST_COVERAGE_REPORT.md
   - Check if any critical gaps
   - Plan tests for new features

4. **Update documentation:**
   - Did you add new features?
   - Did you add new tests?
   - Update coverage report

---

## 🚀 Advanced: Continuous Integration

### GitHub Actions Example

Create `.github/workflows/tests.yml`:

```yaml
name: Run Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Run Unreal Tests
        run: |
          & "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" `
            "${{ github.workspace }}\Catastrophe.uproject" `
            -ExecCmds="Automation RunTests Catastrophe; Quit" `
            -unattended -nopause -ReportOutputPath="${{ github.workspace }}\TestResults"
      
      - name: Upload Results
        if: always()
        uses: actions/upload-artifact@v3
        with:
          name: test-results
          path: TestResults/
      
      - name: Check Results
        run: |
          if ($LASTEXITCODE -ne 0) {
            Write-Error "Tests failed!"
            exit 1
          }
```

This will:
- ✅ Run tests on every commit
- ✅ Run tests on every pull request
- ✅ Upload test results
- ✅ Fail build if tests fail
- ✅ Prevent merging broken code

---

## 💡 Quick Tips

### Tip 1: Run Tests Before Committing
```bash
# Quick test
RunTests.bat → Option 5 (Skip performance)

# If pass → git commit
# If fail → fix before commit
```

### Tip 2: Test New Features First (TDD)
```
1. Write test (will fail)
2. Implement feature
3. Run test (should pass)
4. Refactor if needed
5. Commit
```

### Tip 3: Use Test Names to Document
```cpp
// Good: Tells you exactly what it tests
FCatCharacterStaminaCannotGoBelowZeroTest

// Bad: Vague
FCatCharacterTest1
```

### Tip 4: One Assertion = One Failure
```cpp
// Good: Clear what failed
TestEqual(TEXT("Stamina is 0"), stamina, 0.0f);
TestEqual(TEXT("Flag is set"), depleted, true);

// Bad: Which one failed?
TestTrue(TEXT("Stamina stuff works"), stamina == 0 && depleted);
```

### Tip 5: Clean Up After Tests
```cpp
// ALWAYS cleanup
TestWorld->DestroyActor(TestCat);
TestWorld->DestroyActor(TestNPC);

// Prevents memory leaks and test pollution
```

---

## 🎉 Success Criteria

Your testing setup is successful when:

- ✅ All 13 tests pass consistently
- ✅ Tests run in under 30 seconds
- ✅ You can run tests before every commit
- ✅ New features come with new tests
- ✅ Coverage is trending upward (60% → 75%)
- ✅ No flaky tests (100% reliable)
- ✅ Tests catch bugs before players do

---

## 📞 Getting Help

**If tests fail and you're stuck:**

1. Read the error message carefully
2. Check TEST_COVERAGE_REPORT.md for examples
3. Look at similar passing tests
4. Check if recent changes broke something
5. Try running test individually
6. Add debug logging to see what's happening

**Common resources:**
- `docs/TESTING_GUIDE.md` - Complete tutorial
- `docs/TEST_COVERAGE_REPORT.md` - What's tested
- `Source/Catastrophe/Tests/CatCharacterTests.cpp` - Example tests

---

## 🎯 Summary

**To run tests:**
1. Double-click `RunTests.bat` → Choose option
2. Or use Unreal Editor → Window → Test Automation

**To evaluate results:**
1. Check pass rate (should be 100%)
2. Check execution time (should be < 30 sec)
3. Check coverage report (should be improving)
4. Verify reliability (run 3x, all pass)

**Before committing:**
- Run tests
- All green? Commit!
- Any red? Fix first!

**You're all set! 🎮✅**

