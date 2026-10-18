# Test suite addressing the review's testing/QA paragraph

This package addresses exactly this part of the review, point by point:

> "there are no adequate measures in the code to provide confidence that the
> code is correct. No assertions, no test framework, and no tests for things
> such as out-of-bounds exceptions or empty list operations. On top of that,
> code for data structures with subtle invariants, such as red-black trees,
> should include invariant/properties checks and tests that verify the
> invariants over many runs, including massive runs using both expected and
> unusual patterns of operations. Once all of that is in place, you'd also
> want to be measuring things such as test code coverage."

Your `test.paper/` directory is left alone — this is a separate, additive
`test/` suite. Drop it in, wire it into your build, and clean up
`test.paper/` on your own schedule.

## What's here

- `doctest.h` — single-header test framework (MIT licensed, no build step,
  no external dependency to install). Addresses "no test framework."
- `src/` — your actual `IndexedPPS23RedBlackBinaryTree.h`/`EfficientList.h`/
  `IndexedPPS23RBBinaryTreeNode.h`, with one addition: a public
  `bool checkInvariants(std::ostream* err = nullptr) const` method on the
  tree class (and a one-line wrapper exposing it through `EfficientList`).
  This is what makes "invariant/properties checks" possible at all — see
  "What checkInvariants() verifies" below.
- `test_efficientlist.cpp` — the main suite, addressing every remaining
  item in the paragraph (bounds, empty-list, invariants-over-many-runs,
  massive runs, expected + unusual patterns). See breakdown below.
- `test_invariant_checker_sensitivity.cpp` — a second file that proves
  `checkInvariants()` isn't a rubber stamp, by deliberately corrupting the
  tree four different ways and confirming each is caught.
- `coverage/` — a sample lcov/genhtml HTML coverage report from running
  both files above, so you can see exactly which lines/branches are (and
  aren't) currently exercised. Open `coverage/html_report/index.html`.

## How each item in the paragraph is addressed

### "No assertions, no test framework"
`doctest.h` + `TEST_CASE`/`CHECK`/`REQUIRE` throughout both `.cpp` files.
`REQUIRE` aborts the current test case immediately on failure (use for
things that would make the rest of the test meaningless, e.g. a size
mismatch); `CHECK` records the failure but keeps going (use for independent
assertions you want to see all of, e.g. checking every element in a loop).

### "No tests for out-of-bounds exceptions or empty list operations"
`test_efficientlist.cpp`:
- `TEST_CASE("bounds checks: operator[] throws on invalid index")`
- `TEST_CASE("bounds checks: insert throws on invalid index")`
- `TEST_CASE("bounds checks: remove throws on invalid index")`
- `TEST_CASE("empty list: every operation on a freshly-constructed list throws")`
- `TEST_CASE("empty list: repeatedly emptying and refilling keeps invariants intact")`
  — not just "does it throw once," but: fill, fully drain, refill, fully
  drain again, 50 times in a row, checking invariants after every single
  operation. Emptying-and-refilling is exactly the transition where
  `mFirstInOrderNode`/`mLastInOrderNode` bookkeeping is easiest to get
  wrong (ask me how I know).

### "Invariant/properties checks" (data structures with subtle invariants)
Added directly to `IndexedPPS23RedBlackBinaryTree`:
```cpp
bool checkInvariants(std::ostream *err = nullptr) const
```
One O(n) traversal verifying, together:
1. **Black-height** is equal on every root-to-`nilSentinel` path.
2. **No node has two red children** (the "Sibling" property of your pure
   2-3 variant — a red node must not have a red sibling).
3. **`mLeftSize` is correct at every node**, including the
   `mGlobalLeftSize` adjustment on exactly the nodes on the path to index 0
   (Section 3.2 of your paper).
4. **`mFirstInOrderNode`/`mLastInOrderNode`** really are the leftmost/
   rightmost real nodes.
5. **`mSize`** matches the actual node count found by the traversal.
6. **Parent/child pointers** agree in both directions.

`test_invariant_checker_sensitivity.cpp` then proves this checker actually
works: it reaches into the tree (via a `#define private public` trick,
isolated to its own translation unit so your real library code is never
compiled with altered access) and deliberately breaks each of #1-4 one at a
time, confirming `checkInvariants()` reports a failure every time. Without
this, a passing invariant checker could just mean the checker is broken and
always returns `true` — this rules that out.

### "Verify the invariants over many runs, including massive runs, expected and unusual patterns"

**Many runs / "expected" patterns** —
`TEST_CASE("fuzz: mixed random operations, invariant-checked after every op")`
runs 10 different RNG seeds, each doing 3000 random push/pop/insert/erase
operations at both ends and the middle, checking full contents against
`std::vector` **and calling `checkInvariants()` after every single
operation**.

**Massive runs** —
- `TEST_CASE("massive run: 200,000 mixed operations, ...")`
- `TEST_CASE("massive run: 1,000,000 push_front/push_back, then pop half")`

  (invariants checked periodically rather than every op at this scale,
  since `checkInvariants()` itself is O(n) and checking every op would make
  the test suite itself take far too long — this is a standard tradeoff:
  dense checking on small trees to catch bugs cheaply, sparse checking on
  huge trees to still catch anything that survives to massive scale without
  paying O(n) per operation).

**Unusual patterns** —
- `TEST_CASE("adversarial: all-same-end pushes (skewed insertion pattern)")`
  — long unbroken runs at one end instead of the cancel-each-other-out
  pattern uniform randomness tends to produce.
- `TEST_CASE("adversarial: paper's own worst-case pattern (Section 3.6.3)")`
  — reproduces the exact pop-then-push-on-an-all-black-subtree pattern your
  own paper identifies as the time-complexity worst case, and checks
  *correctness* under it (your Section 3.6.3 only claims a slower amortized
  bound there, not incorrectness — this test confirms invariants and values
  stay correct even in your own stated worst case).
- `TEST_CASE("adversarial: interleaved single-element churn at a fixed middle index")`
  — hammers the general-insert/general-remove path (as opposed to the
  specialized O(1) end-cases) at one stable spot, thousands of times.

### "Measuring test code coverage"
```bash
g++ -std=c++17 --coverage -O0 -g test_efficientlist.cpp -o test_efficientlist_cov
./test_efficientlist_cov --test-suite-exclude=massive   # fast subset
./test_efficientlist_cov --test-suite=massive            # + the slow ones
g++ -std=c++17 --coverage -O0 -g test_invariant_checker_sensitivity.cpp -o test_sensitivity_cov
./test_sensitivity_cov

lcov --capture --directory . --output-file coverage.info --rc branch_coverage=1 \
     --ignore-errors mismatch,unused,inconsistent
lcov --extract coverage.info '*/src/*' --output-file coverage_src_only.info --rc branch_coverage=1
genhtml coverage_src_only.info --output-directory html_report --rc branch_coverage=1
```
Current result (included in `coverage/html_report/`): **86.7% lines, 77.6%
functions, 64.7% branches** across `EfficientList.h` /
`IndexedPPS23RedBlackBinaryTree.h` / `IndexedPPS23RBBinaryTreeNode.h`.

What's *not* covered, and why (check the HTML report to confirm this stays
true after you add more tests):
- The `Iterator`/`InOrderIterator` class (forward/reverse `++`/`--`) —
  entirely untested, because `EfficientList` doesn't expose iteration
  publicly right now (it's dead code today, matching the commented-out
  `iterateInOrder()`/`drawTree()` in `EfficientList.h`). **If you re-enable
  those, write iterator tests before shipping them** — this is exactly the
  kind of gap `checkInvariants()` can't catch, since it doesn't touch the
  code path at all.
- A few defensive `throw` guards inside `insertLeftChild`/`insertRightChild`
  (`"already has a left/right child"`, `"Invalid Pointer"`) and
  `insertRootNode`'s `"Root already exists"` guard — these are internal
  sanity checks that the current public `EfficientList` API structurally
  can't trigger (it never calls these with a bad precondition). Low
  priority, but note them if you ever expose the tree class directly.
- Most of `checkInvariants()`'s own error-message branches — expected,
  since most tests pass; `test_invariant_checker_sensitivity.cpp` exercises
  a sample of these on purpose (see above), which is why coverage went up
  after adding that file (84.0%→86.7% lines, 56.5%→64.7% branches).

## Running everything

```bash
g++ -std=c++17 -O2 -Wall test_efficientlist.cpp -o test_efficientlist
./test_efficientlist                              # full suite (~60s, 1M-element test included)
./test_efficientlist --test-suite-exclude=massive # fast subset (~0.2s) -- good for every push in CI

g++ -std=c++17 -O2 -Wall test_invariant_checker_sensitivity.cpp -o test_sensitivity
./test_sensitivity

# and periodically, under sanitizers, to catch memory bugs invariants alone can't see:
g++ -std=c++17 -g -fsanitize=address,undefined test_efficientlist.cpp -o test_efficientlist_asan
./test_efficientlist_asan --test-suite-exclude=massive
```

## Minimal CI wiring (GitHub Actions)

```yaml
name: tests
on: [push, pull_request]
jobs:
  fast:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: |
          cd C++/test
          g++ -std=c++17 -O2 -Wall test_efficientlist.cpp -o t
          ./t --test-suite-exclude=massive
          g++ -std=c++17 -Wall test_invariant_checker_sensitivity.cpp -o s
          ./s
  full-nightly:
    runs-on: ubuntu-latest
    if: github.event_name == 'schedule'
    steps:
      - uses: actions/checkout@v4
      - run: |
          cd C++/test
          g++ -std=c++17 -O2 test_efficientlist.cpp -o t && ./t
          g++ -std=c++17 -g -fsanitize=address,undefined test_efficientlist.cpp -o t_asan
          ./t_asan
```
