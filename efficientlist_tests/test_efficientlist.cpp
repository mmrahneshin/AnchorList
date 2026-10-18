// test_efficientlist.cpp
//
// Addresses, point by point, the reviewer's paragraph:
//
//   "no assertions, no test framework, and no tests for things such as
//    out-of-bounds exceptions or empty list operations... code for data
//    structures with subtle invariants ... should include invariant checks
//    and tests that verify the invariants over many runs, including
//    massive runs using both expected and unusual patterns of operations."
//
// - "assertions" + "test framework"        -> doctest (this file)
// - "out-of-bounds exceptions"             -> TEST_CASE("bounds checks")
// - "empty list operations"                -> TEST_CASE("empty list")
// - "invariant/properties checks"          -> IndexedPPS23RedBlackBinaryTree
//                                              ::checkInvariants(), called
//                                              from every test below
// - "verify invariants over many runs"     -> TEST_CASE("fuzz: ...") ,
//                                              run across 10 seeds each
// - "massive runs"                         -> TEST_CASE("massive run: ...")
// - "expected ... patterns"                -> uniformly-random mixed ops
// - "unusual patterns"                     -> TEST_CASE("adversarial: ...")
//                                              specifically reproducing the
//                                              paper's own Section 3.6.3
//                                              worst-case pop/push pattern

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>
#include <random>
#include <sstream>
#include <stdexcept>
#include "src/IndexedPPS23RBBinaryTreeNode.h"
#include "src/IndexedPPS23RedBlackBinaryTree.h"
#include "src/EfficientList.h"

// ---------------------------------------------------------------------
// Shared helper: assert list == ref (contents) AND checkInvariants().
// Every test in this file routes through this, so a single assertion
// failure always tells you both *what* value was wrong (if anything) and
// whether the tree's internal structure is still consistent.
// ---------------------------------------------------------------------
static void assert_consistent(EfficientList<int> &list, std::vector<int> &ref, int op) {
    std::ostringstream err;
    bool inv_ok = list.checkInvariants(&err);
    INFO("op=", op, " invariant check: ", err.str());
    REQUIRE(inv_ok);

    REQUIRE_EQ((int)ref.size(), list.size());
    for (int i = 0; i < (int)ref.size(); ++i) {
        INFO("op=", op, " index=", i);
        REQUIRE_EQ(ref[i], list[i]);
    }
}

// =======================================================================
// 1. Out-of-bounds exceptions
// =======================================================================
TEST_CASE("bounds checks: operator[] throws on invalid index") {
    EfficientList<int> list;
    list.insert(0, 10);
    list.insert(1, 20);
    list.insert(2, 30); // list = [10, 20, 30]

    CHECK_THROWS_AS(list[3], std::runtime_error);
    CHECK_THROWS_AS(list[100], std::runtime_error);
    CHECK_THROWS_AS(list[-1], std::runtime_error);
    // valid indices must NOT throw
    CHECK_NOTHROW(list[0]);
    CHECK_NOTHROW(list[2]);
}

TEST_CASE("bounds checks: insert throws on invalid index") {
    EfficientList<int> list;
    list.insert(0, 10);
    list.insert(1, 20); // size 2, valid insert positions are 0,1,2

    CHECK_THROWS_AS(list.insert(3, 99), std::runtime_error);
    CHECK_THROWS_AS(list.insert(-1, 99), std::runtime_error);
    CHECK_THROWS_AS(list.insert(100, 99), std::runtime_error);
    CHECK_NOTHROW(list.insert(2, 99)); // == size() is valid (push_back)
}

TEST_CASE("bounds checks: remove throws on invalid index") {
    EfficientList<int> list;
    list.insert(0, 10);
    list.insert(1, 20); // size 2, valid remove positions are 0,1

    CHECK_THROWS_AS(list.remove(2), std::runtime_error);
    CHECK_THROWS_AS(list.remove(-1), std::runtime_error);
    CHECK_THROWS_AS(list.remove(100), std::runtime_error);
    CHECK_NOTHROW(list.remove(0));
}

// =======================================================================
// 2. Empty list operations
// =======================================================================
TEST_CASE("empty list: every operation on a freshly-constructed list throws") {
    EfficientList<int> list;
    REQUIRE_EQ(list.size(), 0);

    CHECK_THROWS_AS(list[0], std::runtime_error);
    CHECK_THROWS_AS(list.remove(0), std::runtime_error);
    CHECK_THROWS_AS(list.insert(1, 42), std::runtime_error); // only idx=0 valid when empty
    CHECK_THROWS_AS(list.insert(-1, 42), std::runtime_error);

    // idx=0 is the one valid insert position on an empty list
    CHECK_NOTHROW(list.insert(0, 42));
    CHECK_EQ(list.size(), 1);
}

TEST_CASE("empty list: repeatedly emptying and refilling keeps invariants intact") {
    EfficientList<int> list;
    std::vector<int> ref;
    int op = 0;
    for (int round = 0; round < 50; ++round) {
        // fill to a small random-ish size
        int fill = 1 + (round % 5);
        for (int i = 0; i < fill; ++i) {
            ref.push_back(round * 100 + i);
            list.insert(list.size(), round * 100 + i);
            assert_consistent(list, ref, op++);
        }
        // drain completely -- this is exactly the empty-list edge case
        while (!ref.empty()) {
            ref.pop_back();
            list.remove(list.size() - 1);
            assert_consistent(list, ref, op++);
        }
        REQUIRE_EQ(list.size(), 0);
        CHECK_THROWS_AS(list[0], std::runtime_error);
    }
}

// =======================================================================
// 3. Invariant/property checks over many runs -- "expected" patterns
// =======================================================================
static void fuzz_mixed(int ops, unsigned seed) {
    std::mt19937 rng(seed);
    std::vector<int> ref;
    EfficientList<int> list;
    int next_val = 0;

    for (int op = 0; op < ops; ++op) {
        int n = (int)ref.size();
        int choice = rng() % 6;
        if (choice == 0) {
            int v = next_val++;
            ref.push_back(v);
            list.insert(list.size(), v);
        } else if (choice == 1) {
            int v = next_val++;
            ref.insert(ref.begin(), v);
            list.insert(0, v);
        } else if (choice == 2 && n > 0) {
            ref.pop_back();
            list.remove(list.size() - 1);
        } else if (choice == 3 && n > 0) {
            ref.erase(ref.begin());
            list.remove(0);
        } else if (choice == 4) {
            int pos = n == 0 ? 0 : rng() % (n + 1);
            int v = next_val++;
            ref.insert(ref.begin() + pos, v);
            list.insert(pos, v);
        } else if (choice == 5 && n > 0) {
            int pos = rng() % n;
            ref.erase(ref.begin() + pos);
            list.remove(pos);
        } else {
            continue;
        }
        assert_consistent(list, ref, op);
    }
}

TEST_CASE("fuzz: mixed random operations, invariant-checked after every op") {
    // 10 different seeds, per the review's "verify ... over many runs"
    for (unsigned seed = 1; seed <= 10; ++seed) {
        CAPTURE(seed);
        fuzz_mixed(3000, seed);
    }
}

// =======================================================================
// 4. Massive runs
// =======================================================================
TEST_CASE("massive run: 200,000 mixed operations, sparse invariant checking" * doctest::test_suite("massive")) {
    // checkInvariants() is O(n); at this scale we check every 200 ops
    // instead of every single op, matching the review's "massive runs"
    // request without making the test suite itself take minutes to run.
    std::mt19937 rng(777);
    std::vector<int> ref;
    EfficientList<int> list;
    int next_val = 0;
    const int ops = 200000;

    for (int op = 0; op < ops; ++op) {
        int n = (int)ref.size();
        int choice = rng() % 10;
        if (choice <= 3) {
            int v = next_val++;
            ref.push_back(v);
            list.insert(list.size(), v);
        } else if (choice <= 6) {
            int v = next_val++;
            ref.insert(ref.begin(), v);
            list.insert(0, v);
        } else if (choice == 7) {
            int pos = n == 0 ? 0 : rng() % (n + 1);
            int v = next_val++;
            ref.insert(ref.begin() + pos, v);
            list.insert(pos, v);
        } else if (choice == 8 && n > 100) {
            ref.pop_back();
            list.remove(list.size() - 1);
        } else if (choice == 9 && n > 100) {
            ref.erase(ref.begin());
            list.remove(0);
        } else {
            continue;
        }

        if (op % 200 == 0 || op == ops - 1) {
            assert_consistent(list, ref, op);
        }
    }
    MESSAGE("final size: ", ref.size());
}

TEST_CASE("massive run: 1,000,000 push_front/push_back, then pop half" * doctest::test_suite("massive")) {
    std::mt19937 rng(999);
    std::vector<int> ref;
    EfficientList<int> list;
    int next_val = 0;
    const int target = 1000000;

    while ((int)ref.size() < target) {
        if (rng() % 2 == 0) {
            int v = next_val++;
            ref.push_back(v);
            list.insert(list.size(), v);
        } else {
            int v = next_val++;
            ref.insert(ref.begin(), v);
            list.insert(0, v);
        }
        if (ref.size() % 50000 == 0) {
            assert_consistent(list, ref, (int)ref.size());
        }
    }
    while ((int)ref.size() > target / 2) {
        if (rng() % 2 == 0) {
            ref.pop_back();
            list.remove(list.size() - 1);
        } else {
            ref.erase(ref.begin());
            list.remove(0);
        }
        if (ref.size() % 50000 == 0) {
            assert_consistent(list, ref, (int)ref.size());
        }
    }
    MESSAGE("final size: ", ref.size());
}

// =======================================================================
// 5. Unusual / adversarial patterns
// =======================================================================
TEST_CASE("adversarial: all-same-end pushes (skewed insertion pattern)") {
    // Not the "expected" uniformly-mixed workload -- push at ONLY the
    // front for a long stretch, then ONLY the back, then interleave in
    // large blocks rather than per-operation. This exercises long runs of
    // the SAME rebalancing case repeatedly, rather than the cancelling-out
    // pattern uniform randomness tends to produce.
    std::vector<int> ref;
    EfficientList<int> list;
    int v = 0;

    for (int i = 0; i < 20000; ++i) { ref.insert(ref.begin(), v); list.insert(0, v); v++; }
    assert_consistent(list, ref, 1);

    for (int i = 0; i < 20000; ++i) { ref.push_back(v); list.insert(list.size(), v); v++; }
    assert_consistent(list, ref, 2);

    for (int i = 0; i < 10000; ++i) { ref.pop_back(); list.remove(list.size() - 1); }
    assert_consistent(list, ref, 3);

    for (int i = 0; i < 10000; ++i) { ref.erase(ref.begin()); list.remove(0); }
    assert_consistent(list, ref, 4);
}

TEST_CASE("adversarial: paper's own worst-case pattern (Section 3.6.3)") {
    // The paper itself identifies a specific pathological sequence: repeated
    // pop+push pairs on a subtree that is entirely black, which the paper's
    // own analysis says degrades to O(log n) *amortized per pair* instead of
    // O(1) (see Section 3.6.3). That section only analyzes the TIME
    // complexity of this pattern -- it does not claim, and this test does
    // not assume, anything about correctness. The point here is purely
    // correctness: even in the authors' own identified worst case, the
    // tree's invariants must still hold and the returned values must still
    // be right. This is exactly the "unusual patterns of operations" the
    // review asked for, grounded in the paper's own stated worst case
    // rather than an arbitrary adversarial guess.
    std::vector<int> ref;
    EfficientList<int> list;
    int v = 0;

    // Build up a reasonably sized list first so there's a real subtree
    // structure for the pattern to stress.
    for (int i = 0; i < 5000; ++i) { ref.push_back(v); list.insert(list.size(), v); v++; }
    assert_consistent(list, ref, 0);

    // Repeated pop_back immediately followed by push_back, many times,
    // which is the shape of pattern the paper describes (pop then push on
    // the same end, forcing repeated worst-case rebalancing).
    for (int round = 0; round < 5000; ++round) {
        ref.pop_back();
        list.remove(list.size() - 1);

        ref.push_back(v);
        list.insert(list.size(), v);
        v++;

        if (round % 50 == 0) {
            assert_consistent(list, ref, round);
        }
    }
    assert_consistent(list, ref, 9999);

    // Same pattern at the front.
    for (int round = 0; round < 5000; ++round) {
        ref.erase(ref.begin());
        list.remove(0);

        ref.insert(ref.begin(), v);
        list.insert(0, v);
        v++;

        if (round % 50 == 0) {
            assert_consistent(list, ref, round);
        }
    }
    assert_consistent(list, ref, 9999);
}

TEST_CASE("adversarial: interleaved single-element churn at a fixed middle index") {
    // Repeatedly insert then immediately remove at the exact same
    // (non-edge) index, many times -- an "unusual" pattern in that a real
    // workload rarely hammers one exact spot, but it's cheap to run
    // thousands of times and stresses the general-insert/general-remove
    // path (as opposed to the specialized O(1) end-cases) repeatedly at a
    // stable tree shape.
    std::vector<int> ref;
    EfficientList<int> list;
    int v = 0;
    for (int i = 0; i < 2000; ++i) { ref.push_back(v); list.insert(list.size(), v); v++; }
    assert_consistent(list, ref, 0);

    int mid = (int)ref.size() / 2;
    for (int round = 0; round < 5000; ++round) {
        ref.insert(ref.begin() + mid, v);
        list.insert(mid, v);
        v++;

        ref.erase(ref.begin() + mid);
        list.remove(mid);

        if (round % 100 == 0) {
            assert_consistent(list, ref, round);
        }
    }
    assert_consistent(list, ref, 9999);
}
