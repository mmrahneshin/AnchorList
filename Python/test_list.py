import time
import random
from typing import List
import csv
import os
import psutil
import gc
import tracemalloc
from time import sleep
from anchor_list import AnchorList


def process_mem_usage_kb() -> float:

    gc.collect()

    ram = psutil.virtual_memory()
    return float((ram.total - ram.free) / 1024)


# Global variables for timing
values = []
indexes = []
delete_indexes = []

memory_results = {
    # push_back
    "push_back_anchor": {},
    "push_back_list": {},
    "pop_back_after_push_back_anchor": {},
    "pop_back_after_push_back_list": {},
    "pop_front_after_push_back_anchor": {},
    "pop_front_after_push_back_list": {},
    "remove_random_after_push_back_anchor": {},
    "remove_random_after_push_back_list": {},
    # push_front
    "push_front_anchor": {},
    "push_front_list": {},
    "pop_front_after_push_front_anchor": {},
    "pop_front_after_push_front_list": {},
    "pop_back_after_push_front_anchor": {},
    "pop_back_after_push_front_list": {},
    # random insert/remove
    "insert_random_anchor": {},
    "insert_random_list": {},
    "remove_random_anchor": {},
    "remove_random_list": {},
    # push_front then push_back
    "push_front_push_back_anchor": {},
    "push_front_push_back_list": {},
    "pop_front_pop_back_anchor": {},
    "pop_front_pop_back_list": {},
    # push_back then push_front
    "push_back_push_front_anchor": {},
    "push_back_push_front_list": {},
    "pop_back_pop_front_anchor": {},
    "pop_back_pop_front_list": {},
}

timing_results = {
    # push_back
    "push_back_anchor": {},
    "push_back_list": {},
    "get_after_push_back_anchor": {},
    "get_after_push_back_list": {},
    "pop_back_after_push_back_anchor": {},
    "pop_back_after_push_back_list": {},
    "get_after_pop_back_after_push_back_anchor": {},
    "get_after_pop_back_after_push_back_list": {},
    "pop_front_after_push_back_anchor": {},
    "pop_front_after_push_back_list": {},
    "get_after_pop_front_after_push_back_anchor": {},
    "get_after_pop_front_after_push_back_list": {},
    "remove_random_after_push_back_anchor": {},
    "remove_random_after_push_back_list": {},
    "get_after_remove_random_after_push_back_anchor": {},
    "get_after_remove_random_after_push_back_list": {},
    # push_front
    "push_front_anchor": {},
    "push_front_list": {},
    "get_after_push_front_anchor": {},
    "get_after_push_front_list": {},
    "pop_front_after_push_front_anchor": {},
    "pop_front_after_push_front_list": {},
    "get_after_pop_front_after_push_front_anchor": {},
    "get_after_pop_front_after_push_front_list": {},
    "pop_back_after_push_front_anchor": {},
    "pop_back_after_push_front_list": {},
    "get_after_pop_back_after_push_front_anchor": {},
    "get_after_pop_back_after_push_front_list": {},
    # random insert/remove
    "insert_random_anchor": {},
    "insert_random_list": {},
    "get_after_insert_random_anchor": {},
    "get_after_insert_random_list": {},
    "remove_random_anchor": {},
    "remove_random_list": {},
    "get_after_remove_random_anchor": {},
    "get_after_remove_random_list": {},
    # push_front then push_back
    "push_front_push_back_anchor": {},
    "push_front_push_back_list": {},
    "get_after_push_front_push_back_anchor": {},
    "get_after_push_front_push_back_list": {},
    "pop_front_pop_back_anchor": {},
    "pop_front_pop_back_list": {},
    "get_after_pop_front_pop_back_anchor": {},
    "get_after_pop_front_pop_back_list": {},
    # push_back then push_front
    "push_back_push_front_anchor": {},
    "push_back_push_front_list": {},
    "get_after_push_back_push_front_anchor": {},
    "get_after_push_back_push_front_list": {},
    "pop_back_pop_front_anchor": {},
    "pop_back_pop_front_list": {},
    "get_after_pop_back_pop_front_anchor": {},
    "get_after_pop_back_pop_front_list": {},
}


def save_dict_to_csv(data, foldername, filename):
    gc.collect()
    os.makedirs(foldername, exist_ok=True)
    with open(f"{foldername}/{filename}", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["Size", "Time"])
        for size, t in sorted(data.items()):
            writer.writerow([size, f"{t:.9f}"])


def insert_to_anchor_linear_list(ell, size, scenario=None):

    tracemalloc.start()
    # Take initial memory snapshot

    start = time.time()
    for i in range(size):
        ell.insert(indexes[i], values[i])
    end_time = time.time()

    snapshot = tracemalloc.take_snapshot()
    memory_used = 0
    for stat in snapshot.statistics("traceback"):
        # Check if the allocation is related to the list object
        for frame in stat.traceback:
            if frame.filename.endswith(
                "test_list.py"
            ):  # Replace with your script's name
                memory_used += stat.size
                break

    time_taken = end_time - start
    if scenario != None:
        timing_results[scenario][size] = time_taken
        memory_results[scenario][size] = memory_used / 1024
    tracemalloc.stop()


def insert_to_list(lst, size, scenario=None):
    start_memory = process_mem_usage_kb()

    start = time.time()
    for i in range(size):
        lst.insert(indexes[i], values[i])
    end_time = time.time()

    endMemory = process_mem_usage_kb()
    memory_used = endMemory - start_memory

    time_taken = end_time - start
    if scenario != None:
        timing_results[scenario][size] = time_taken
        memory_results[scenario][size] = memory_used


def remove_from_anchor_linear_list(ell, delete_indexes_size, scenario=None):
    start_memory = process_mem_usage_kb()

    start = time.time()
    for i in range(delete_indexes_size):
        ell.remove(delete_indexes[i])
    end_time = time.time()

    endMemory = process_mem_usage_kb()
    memory_used = start_memory - endMemory

    time_taken = end_time - start
    if scenario != None:
        timing_results[scenario][delete_indexes_size] = time_taken
        memory_results[scenario][delete_indexes_size] = memory_used


def remove_from_list(lst, delete_indexes_size, scenario=None):
    start_memory = process_mem_usage_kb()

    start = time.time()
    for i in range(delete_indexes_size):
        lst.pop(delete_indexes[i])
    end_time = time.time()
    lst = list(lst)

    endMemory = process_mem_usage_kb()
    memory_used = start_memory - endMemory

    time_taken = end_time - start
    if scenario != None:
        timing_results[scenario][delete_indexes_size] = time_taken
        memory_results[scenario][delete_indexes_size] = memory_used


def value_check_ell_list(
    ell, lst, size, ell_time_taken_scenario=None, list_time_taken_scenario=None
):
    result = True

    # Check for value equality
    for i in range(size):
        if ell[i] != lst[i]:
            result = False
            break

    # Time access for AnchorList
    start_time = time.time()
    for i in range(size):
        _ = ell[i]
    ell_time_taken = time.time() - start_time

    # Time access for list
    start_time = time.time()
    for i in range(size):
        _ = lst[i]
    list_time_taken = time.time() - start_time

    # Store timings if dicts are provided
    if ell_time_taken_scenario is not None:
        timing_results[ell_time_taken_scenario][size] = ell_time_taken
    if list_time_taken_scenario is not None:
        timing_results[list_time_taken_scenario][size] = list_time_taken

    return result


def main():
    global values, indexes, delete_indexes
    for step in range(4, 6):
        step = 10**step
        for size in range(step, min(step * 10, 100001), step):
            print(f"\nTesting size: {size}")
            values = list(range(1, size + 1))
            indexes = list(range(size))
            delete_indexes = [0] * (size // 2)
            random.seed(999)

            for input_test in range(0, 14):
                if input_test == 0:
                    print("\n1: test anchor list vs list push_back insert")
                    ell = AnchorList()
                    lst = []
                    insert_to_anchor_linear_list(ell, size, "push_back_anchor")
                    insert_to_list(lst, size, "push_back_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        size,
                        "get_after_push_back_anchor",
                        "get_after_push_back_list",
                    )

                elif input_test == 1:
                    print("\n2: test anchor list vs list pop_back")
                    for i in range(size // 2):
                        delete_indexes[i] = size - (i + 1)
                    remove_from_anchor_linear_list(
                        ell, size // 2, "pop_back_after_push_back_anchor"
                    )
                    remove_from_list(lst, size // 2, "pop_back_after_push_back_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_pop_back_after_push_back_anchor",
                        "get_after_pop_back_after_push_back_list",
                    )

                elif input_test == 2:
                    print("\n3: test anchor list vs list pop_front")
                    ell = AnchorList()
                    lst = []
                    insert_to_anchor_linear_list(ell, size)
                    insert_to_list(lst, size)

                    for i in range(size // 2):
                        delete_indexes[i] = 0
                    remove_from_anchor_linear_list(
                        ell, size // 2, "pop_front_after_push_back_anchor"
                    )
                    remove_from_list(lst, size // 2, "pop_front_after_push_back_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_pop_front_after_push_back_anchor",
                        "get_after_pop_front_after_push_back_list",
                    )

                elif input_test == 3:
                    print("\n4: test anchor list vs list push_front")
                    ell = AnchorList()
                    lst = []
                    for i in range(size):
                        indexes[i] = 0
                    insert_to_anchor_linear_list(ell, size, "push_front_anchor")
                    insert_to_list(lst, size, "push_front_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        size,
                        "get_after_push_front_anchor",
                        "get_after_push_front_list",
                    )

                elif input_test == 4:
                    print("\n5: test anchor list vs list pop_front")
                    remove_from_anchor_linear_list(
                        ell, size // 2, "pop_front_after_push_front_anchor"
                    )
                    remove_from_list(lst, size // 2, "pop_front_after_push_front_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_pop_front_after_push_front_anchor",
                        "get_after_pop_front_after_push_front_list",
                    )

                elif input_test == 5:
                    print("\n6: test anchor list vs list pop_back")
                    ell = AnchorList()
                    lst = []
                    insert_to_anchor_linear_list(ell, size)
                    insert_to_list(lst, size)

                    for i in range(size // 2):
                        delete_indexes[i] = size - (i + 1)
                    remove_from_anchor_linear_list(
                        ell, size // 2, "pop_back_after_push_front_anchor"
                    )
                    remove_from_list(lst, size // 2, "pop_back_after_push_front_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_pop_back_after_push_front_anchor",
                        "get_after_pop_back_after_push_front_list",
                    )

                elif input_test == 6:
                    print(
                        "\n7: test anchor list vs list random indices remove after push_back"
                    )
                    ell = AnchorList()
                    lst = []
                    for i in range(size):
                        indexes[i] = i
                    insert_to_anchor_linear_list(ell, size)
                    insert_to_list(lst, size)

                    for i in range(size // 2):
                        delete_indexes[i] = random.randint(0, size - (i + 1))
                    remove_from_anchor_linear_list(
                        ell, size // 2, "remove_random_after_push_back_anchor"
                    )
                    remove_from_list(
                        lst, size // 2, "remove_random_after_push_back_list"
                    )
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_remove_random_after_push_back_anchor",
                        "get_after_remove_random_after_push_back_list",
                    )

                elif input_test == 7:
                    print(
                        "\n8: test anchor list vs list random indices insert"
                    )
                    ell = AnchorList()
                    lst = []
                    for i in range(size):
                        indexes[i] = random.randint(0, i)
                    insert_to_anchor_linear_list(
                        ell, size, "insert_random_anchor"
                    )
                    insert_to_list(lst, size, "insert_random_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        size,
                        "get_after_insert_random_anchor",
                        "get_after_insert_random_list",
                    )

                elif input_test == 8:
                    print(
                        "\n9: test anchor list vs list random indices remove after random insert"
                    )
                    for i in range(size // 2):
                        delete_indexes[i] = random.randint(0, size - (i + 1))
                    remove_from_anchor_linear_list(
                        ell, size // 2, "remove_random_anchor"
                    )
                    remove_from_list(lst, size // 2, "remove_random_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_remove_random_anchor",
                        "get_after_remove_random_list",
                    )

                elif input_test == 9:
                    print(
                        "\n10: test anchor list vs list half push_front then push_back the other half"
                    )
                    ell = AnchorList()
                    lst = []
                    for i in range(size // 2):
                        indexes[i] = 0
                        indexes[i + size // 2] = i + size // 2
                    insert_to_anchor_linear_list(
                        ell, size, "push_front_push_back_anchor"
                    )
                    insert_to_list(lst, size, "push_front_push_back_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        size,
                        "get_after_push_front_push_back_anchor",
                        "get_after_push_front_push_back_list",
                    )

                elif input_test == 11:
                    print(
                        "\n12: test anchor list vs list begin half pop_front then pop_back the other half"
                    )
                    for i in range(size // 4):
                        delete_indexes[i] = 0
                        delete_indexes[i + size // 4] = size - (i + size // 4 + 1)
                    remove_from_anchor_linear_list(
                        ell, size // 2, "pop_front_pop_back_anchor"
                    )
                    remove_from_list(lst, size // 2, "pop_front_pop_back_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_pop_front_pop_back_anchor",
                        "get_after_pop_front_pop_back_list",
                    )

                elif input_test == 12:
                    print(
                        "\n13: test anchor list vs list half push_back then push_front the other half"
                    )
                    ell = AnchorList()
                    lst = []
                    for i in range(size // 2):
                        indexes[i] = i
                        indexes[i + size // 2] = 0
                    insert_to_anchor_linear_list(
                        ell, size, "push_back_push_front_anchor"
                    )
                    insert_to_list(lst, size, "push_back_push_front_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        size,
                        "get_after_push_back_push_front_anchor",
                        "get_after_push_back_push_front_list",
                    )

                elif input_test == 13:
                    print(
                        "\n14: test anchor list vs list begin half pop_back then pop_front the other half"
                    )
                    for i in range(size // 4):
                        delete_indexes[i] = size - (i + 1)
                        delete_indexes[i + size // 4] = 0
                    remove_from_anchor_linear_list(
                        ell, size // 2, "pop_back_pop_front_anchor"
                    )
                    remove_from_list(lst, size // 2, "pop_back_pop_front_list")
                    result = value_check_ell_list(
                        ell,
                        lst,
                        len(lst),
                        "get_after_pop_back_pop_front_anchor",
                        "get_after_pop_back_pop_front_list",
                    )

                if result:
                    # print("** That was correct!")
                    continue
                else:
                    print("** Doesn't match.")
                    print("Your code did not pass the tests.")
                    return

    print("Your code passed all the tests.")
    # Save memory results
    for key, data in memory_results.items():
        save_dict_to_csv(data, "memoryResults", f"{key}_results.csv")

    # Save time taken results
    for key, data in timing_results.items():
        save_dict_to_csv(data, "timeTakenResults", f"{key}_results.csv")


if __name__ == "__main__":
    main()
