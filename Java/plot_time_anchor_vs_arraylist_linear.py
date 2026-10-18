"""
Compare AnchorList against Java's ArrayList for every operation benchmarked by
test/AnchorList_vs_arraylist_half_remove.java -- log x-axis, linear y-axis.

The Java benchmark writes one CSV per (test, implementation) pair into
    <repo>/Java/timeTakenResults/
named <test>_<suffix>_results.csv, with columns: Size,Time.

Missing series are skipped, so this script works with whatever result files
are present.
"""

import os

import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.lines import Line2D

# Results live next to this script.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(SCRIPT_DIR, "timeTakenResults")

# (file suffix, legend label, marker, linestyle)
ANCHOR = ("anchor", "AnchorList", "o", "-")
BASELINES = [
    ("arraylist", "ArrayList", "^", "--"),
]
IMPLEMENTATIONS = [ANCHOR] + BASELINES

# The "Time" column holds the total for the whole batch of `size` operations,
# so Time / Size is the average cost of a single element/operation.
YLABEL = "Average time per element (seconds)"

# Define the test categories and their corresponding file patterns
test_categories = [
    ("pushBack", "push_back"),
    ("get_after_pushBack", "Get"),
    ("popBackAfterPushBack", "pop_back"),
    ("get_after_popBackAfterPushBack", "Get"),
    ("popFrontAfterPushBack", "pop_front"),
    ("get_after_popFrontAfterPushBack", "Get"),
    ("removeRandomIndicesAfterPushBack", "Remove from random positions"),
    (
        "get_after_removeRandomIndicesAfterPushBack",
        "Get",
    ),
    ("pushFront", "push_front"),
    ("get_after_pushFront", "Get"),
    ("popFrontAfterPushFront", "pop_front"),
    ("get_after_popFrontAfterPushFront", "Get"),
    ("popBackAfterPushFront", "pop_back"),
    ("get_after_popBackAfterPushFront", "Get"),
    ("insertRandomIndices", "Insert at random positions"),
    ("get_after_insertRandomIndices", "Get"),
    ("removeRandomIndices", "Remove from random positions"),
    ("get_after_removeRandomIndices", "Get"),
    ("pushFront_pushBack", "push_front"),
    ("get_after_pushFront_pushBack", "Get"),
    ("popFront_popBack", "pop_front"),
    ("get_after_popFront_popBack", "Get"),
    ("pushBack_pushFront", "push_back"),
    ("get_after_pushBack_pushFront", "Get"),
    ("popBack_popFront", "pop_back"),
    ("get_after_popBack_popFront", "Get"),
]


def load_series(test_name):
    """Return {suffix: DataFrame} for every implementation that has a CSV."""
    series = {}
    for suffix, _, _, _ in IMPLEMENTATIONS:
        path = os.path.join(csv_path, f"{test_name}_{suffix}_results.csv")
        if os.path.exists(path):
            series[suffix] = (
                pd.read_csv(path)
                .sort_values("Size", kind="stable")
                .reset_index(drop=True)
            )
    return series


def average_time_per_element(data):
    """Batch total time divided by the number of elements in the batch."""
    return data["Time"] / data["Size"]


def load_and_plot_comparison_linear(test_name, title):
    """Load the available result CSVs and create a log-x / linear-y plot."""
    series = load_series(test_name)
    if not series:
        print(f"Skipping {test_name}: no result files in {csv_path}")
        return

    missing = [label for suffix, label, _, _ in IMPLEMENTATIONS if suffix not in series]
    if missing:
        print(f"Note: {test_name} is missing {', '.join(missing)}")

    try:
        plt.figure(figsize=(12, 8))

        for suffix, label, marker, linestyle in IMPLEMENTATIONS:
            data = series.get(suffix)
            if data is None:
                continue
            plt.plot(
                data["Size"],
                average_time_per_element(data),
                label=label,
                marker=marker,
                linestyle=linestyle,
                linewidth=2,
                markersize=4,
            )

        # Sizes span decades, so the size axis stays logarithmic; the time axis
        # is linear here (see plot_time_anchor_vs_arraylist.py for the log-log version).
        plt.xscale("log")

        plt.xlabel("Size", fontsize=12)
        plt.ylabel(YLABEL, fontsize=12)
        plt.title(
            f"{title}\nAnchorList vs baselines (Linear Scale)",
            fontsize=14,
            fontweight="bold",
        )
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3, which="both")

        plt.tight_layout()

        output_file = os.path.join(
            csv_path, "plots_linear", f"{test_name}_comparison_linear.png"
        )
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        plt.savefig(output_file, dpi=300, bbox_inches="tight")

        print(f"Saved plot: {output_file}")

    except Exception as e:
        print(f"Error processing {test_name}: {e}")
    finally:
        plt.close()


def relevant_tests_for(op_type):
    """The test categories belonging to one operation family."""
    if op_type == "Insert":
        keys = ("push", "insert")
    elif op_type == "Remove":
        keys = ("pop", "remove")
    else:  # Get
        keys = ("get_after",)
    return [cat for cat in test_categories if any(k in cat[0].lower() for k in keys)]


def create_summary_plot_linear():
    """Performance ratio of each baseline against AnchorList, per operation."""
    operation_types = ["Insert", "Remove", "Get"]
    fig, axes = plt.subplots(1, len(operation_types), figsize=(20, 6))

    for ax, op_type in zip(axes, operation_types):
        tests = relevant_tests_for(op_type)[:5]  # limit to avoid overcrowding
        plotted = []

        for i, (test_name, _) in enumerate(tests):
            series = load_series(test_name)
            anchor_data = series.get("anchor")
            if anchor_data is None:
                continue

            color = plt.cm.tab10(i % 10)
            labelled = False
            for suffix, _, _, linestyle in BASELINES:
                baseline = series.get(suffix)
                if baseline is None:
                    continue
                # Align on Size so the ratio is never computed index-wise. Sizes
                # match row-by-row, so Time_base / Time_anchor is already the ratio
                # of the per-element averages.
                merged = pd.merge(
                    anchor_data, baseline, on="Size", suffixes=("_anchor", "_base")
                )
                ratio = merged["Time_base"] / merged["Time_anchor"]
                ax.plot(
                    merged["Size"],
                    ratio,
                    color=color,
                    linestyle=linestyle,
                    marker="o",
                    markersize=3,
                    linewidth=2,
                    label=test_name.replace("_", " ").title() if not labelled else None,
                )
                labelled = True
            if labelled:
                plotted.append(test_name.replace("_", " ").title())

        ax.axhline(y=1, color="red", linestyle=":", alpha=0.7)
        ax.set_xscale("log")
        ax.set_xlabel("Size")
        ax.set_ylabel("Performance ratio (baseline / AnchorList)")
        ax.set_title(f"{op_type} operations")
        ax.grid(True, alpha=0.3, which="both")

        # Two legends: colour encodes the test, linestyle the implementation.
        color_handles = [
            Line2D([], [], color=plt.cm.tab10(i % 10), marker="o", label=name)
            for i, name in enumerate(plotted)
        ]
        color_legend = ax.legend(handles=color_handles, fontsize=7, loc="upper left")
        ax.add_artist(color_legend)
        style_handles = [
            Line2D([], [], color="0.3", linestyle=ls, label=label)
            for _, label, _, ls in BASELINES
        ]
        style_handles.append(
            Line2D([], [], color="red", linestyle=":", label="Equal cost")
        )
        ax.legend(handles=style_handles, fontsize=7, loc="upper right")

    fig.suptitle(
        "Performance ratio vs AnchorList (below 1 = AnchorList is slower)",
        fontsize=14,
        fontweight="bold",
    )
    fig.tight_layout()

    output_file = os.path.join(
        csv_path, "plots_linear", "performance_summary_linear.png"
    )
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    fig.savefig(output_file, dpi=300, bbox_inches="tight")
    plt.close(fig)
    print(f"Saved summary plot: {output_file}")


# Main execution
if __name__ == "__main__":
    print("Creating comparison plots (Linear Scale) for AnchorList vs baselines...")

    if not os.path.isdir(csv_path):
        raise SystemExit(f"Results directory not found: {csv_path}")

    # Create individual comparison plots
    for test_name, title in test_categories:
        load_and_plot_comparison_linear(test_name, title)

    # Create summary plot
    create_summary_plot_linear()

    print("All linear scale plots have been generated!")
