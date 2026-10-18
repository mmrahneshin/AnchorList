import time
import random
from typing import List
from anchor_list import AnchorList
from blist import blist

# Global variables for timing
values = []
indexes = []
delete_indexes = []


def insert_to_anchor_linear_list(ell, size):
    """
    Simulate insertion into AnchorList (using Python list as a substitute).
    In a real implementation, this would use a Red-Black Tree-based structure.
    """
    start = time.time()
    for i in range(size):
        # Insert at specified index
        ell.insert(indexes[i], values[i])
    end_time = time.time()
    time_taken = end_time - start
    print(f"insert time taken anchor list: {time_taken:.6f}")


def insert_to_list(lst, size):
    """Insert into a Python list (equivalent to C++ list)."""
    start = time.time()
    for i in range(size):
        lst.insert(indexes[i], values[i])
    end_time = time.time()
    time_taken = end_time - start
    print(f"insert time taken list: {time_taken:.6f}")


def push_front_to_list(lst, start_idx, size):
    """Simulate push_front using list insertion at index 0."""
    start = time.time()
    for i in range(start_idx, size):
        lst.insert(0, values[i])
    end_time = time.time()
    time_taken = end_time - start
    print(f"insert time taken list (push_front): {time_taken:.6f}")


def push_back_to_list(lst, start_idx, size):
    """Simulate push_back using list append."""
    start = time.time()
    for i in range(start_idx, size):
        lst.append(values[i])
    end_time = time.time()
    time_taken = end_time - start
    print(f"insert time taken list (push_back): {time_taken:.6f}")


def value_check_ell_list(ell, lst, size):
    """Compare values between AnchorList (list) and list."""
    result = True
    ell_time_taken = 0.0
    list_time_taken = 0.0

    for i in range(size):
        start = time.time()
        ell_value = ell[i]
        end_time = time.time()
        ell_time_taken += end_time - start

        start = time.time()
        list_value = lst[i]
        end_time = time.time()
        list_time_taken += end_time - start

        if ell_value != list_value:
            result = False
            break

    print(f"ell get time taken: {ell_time_taken:.6f}")
    print(f"list get time taken: {list_time_taken:.6f}")
    return result


def remove_from_anchor_linear_list(ell, delete_indexes_size):
    """Simulate removal from AnchorList (using Python list)."""
    start = time.time()
    for i in range(delete_indexes_size):
        ell.remove(delete_indexes[i])
    end_time = time.time()
    time_taken = end_time - start
    print(f"remove time taken anchor list: {time_taken:.6f}")


def remove_from_list(lst, delete_indexes_size):
    """Remove from a Python list (equivalent to C++ list)."""
    start = time.time()
    for i in range(delete_indexes_size):
        lst.pop(delete_indexes[i])
    end_time = time.time()
    time_taken = end_time - start
    print(f"remove time taken list: {time_taken:.6f}")


def pop_front_from_list(lst, start_idx, delete_indexes_size):
    """Simulate pop_front using list pop(0)."""
    start = time.time()
    for _ in range(start_idx, delete_indexes_size):
        lst.pop(0)
    end_time = time.time()
    time_taken = end_time - start
    print(f"remove time taken list (pop_front): {time_taken:.6f}")


def pop_back_from_list(lst, start_idx, delete_indexes_size):
    """Simulate pop_back using list pop()."""
    start = time.time()
    for _ in range(start_idx, delete_indexes_size):
        lst.pop()
    end_time = time.time()
    time_taken = end_time - start
    print(f"remove time taken list (pop_back): {time_taken:.6f}")


def main():
    size = 1000000
    global values, indexes, delete_indexes
    values = list(range(1, size + 1))
    indexes = list(range(size))
    delete_indexes = [0] * (size // 2)

    print(f"The test has started now with {size} insertions and {size // 2} deletions:")
    random.seed(999)

    for input_test in range(0, 16):
        if input_test == 0:
            print("\n1: test anchor list vs list push_back insert")
            ell = AnchorList()
            lst = blist()
            insert_to_anchor_linear_list(ell, size)
            push_back_to_list(lst, 0, size)
            result = value_check_ell_list(ell, lst, size)
        elif input_test == 1:
            print("\n2: test anchor list vs list pop_back")
            for i in range(size // 2):
                delete_indexes[i] = size - (i + 1)
            remove_from_anchor_linear_list(ell, size // 2)
            pop_back_from_list(lst, 0, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 2:
            print("\n3: test anchor list vs list push_back insert")
            ell = AnchorList()
            lst = blist()
            insert_to_anchor_linear_list(ell, size)
            push_back_to_list(lst, 0, size)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 3:
            print("\n4: test anchor list vs list pop_front")
            for i in range(size // 2):
                delete_indexes[i] = 0
            remove_from_anchor_linear_list(ell, size // 2)
            pop_front_from_list(lst, 0, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 4:
            print("\n5: test anchor list vs list push_front")
            ell = AnchorList()
            lst = blist()
            for i in range(size):
                indexes[i] = 0
            insert_to_anchor_linear_list(ell, size)
            push_front_to_list(lst, 0, size)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 5:
            print("\n6: test anchor list vs list begin pop_front")
            remove_from_anchor_linear_list(ell, size // 2)
            pop_front_from_list(lst, 0, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 6:
            print("\n7: test anchor list vs list push_front")
            ell = AnchorList()
            lst = blist()
            insert_to_anchor_linear_list(ell, size)
            push_front_to_list(lst, 0, size)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 7:
            print("\n8: test anchor list vs list begin pop_back")
            for i in range(size // 2):
                delete_indexes[i] = size - (i + 1)
            remove_from_anchor_linear_list(ell, size // 2)
            pop_back_from_list(lst, 0, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 8:
            print("\n9: test anchor list vs list push_back")
            ell = AnchorList()
            lst = blist()
            for i in range(size):
                indexes[i] = i
            insert_to_anchor_linear_list(ell, size)
            insert_to_list(lst, size)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 9:
            print("\n10: test anchor list vs list random indices remove")
            for i in range(size // 2):
                delete_indexes[i] = random.randint(0, size - (i + 1))
            remove_from_anchor_linear_list(ell, size // 2)
            remove_from_list(lst, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 10:
            print("\n11: test anchor list vs list random indices insert")
            for i in range(size // 2):
                indexes[i] = random.randint(0, i)
            insert_to_anchor_linear_list(ell, size // 2)
            insert_to_list(lst, size // 2)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 11:
            print("\n12: test anchor list vs list random indices remove")
            for i in range(size // 2):
                delete_indexes[i] = random.randint(0, size - (i + 1))
            remove_from_anchor_linear_list(ell, size // 2)
            remove_from_list(lst, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 12:
            print(
                "\n13: test anchor list vs list half push_front then push_back the other half"
            )
            ell = AnchorList()
            lst = blist()
            for i in range(size // 2):
                indexes[i] = 0
                indexes[i + size // 2] = i + size // 2
            insert_to_anchor_linear_list(ell, size)
            push_front_to_list(lst, 0, size // 2)
            push_back_to_list(lst, size // 2, size)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 13:
            print(
                "\n14: test anchor list vs list begin half pop_front then pop_back the other half"
            )
            for i in range(size // 4):
                delete_indexes[i] = 0
                delete_indexes[i + size // 4] = size - (i + size // 4 + 1)
            remove_from_anchor_linear_list(ell, size // 2)
            pop_front_from_list(lst, 0, size // 4)
            pop_back_from_list(lst, size // 4, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        elif input_test == 14:
            print(
                "\n15: test anchor list vs list half push_back then push_front the other half"
            )
            ell = AnchorList()
            lst = blist()
            for i in range(size // 2):
                indexes[i] = i
                indexes[i + size // 2] = 0
            insert_to_anchor_linear_list(ell, size)
            push_back_to_list(lst, 0, size // 2)
            push_front_to_list(lst, size // 2, size)
            result = value_check_ell_list(ell, lst, size)

        elif input_test == 15:
            print(
                "\n16: test anchor list vs list begin half pop_back then pop_front the other half"
            )
            for i in range(size // 4):
                delete_indexes[i] = size - (i + 1)
                delete_indexes[i + size // 4] = 0
            remove_from_anchor_linear_list(ell, size // 2)
            pop_back_from_list(lst, 0, size // 4)
            pop_front_from_list(lst, size // 4, size // 2)
            result = value_check_ell_list(ell, lst, len(lst))

        if result:
            print("** That was correct!")
        else:
            print("** Doesn't match.")
            print("Your code did not pass the tests.")
            return

    print("Your code passed all the tests.")


if __name__ == "__main__":
    main()
